//
// Created by LE, Duc Anh on 6/5/15.
//

#include <iostream>
#include <mongoc.h>
#include <Foundation/Type/cldeValueFactory.h>
#include <Foundation/Exception/cldeNonSupportedDataTypeException.h>
#include "MongoDbSourceDriver.h"

namespace Cloude {
    namespace SourceDriver {
        namespace MongoDb {

            using Field = Foundation::Field;
            using Column = Foundation::Column;
            using Entity = Foundation::Entity;
            using Type = Foundation::Type::cldeValueType;

            class Command {

            public:
                Command(const mongoc_client_t &client, const mongoc_collection_t &collection)
                        : collection(collection),
                          client(client) {
                    _ptrBsonPredicate = bson_new();
                    _ptrBsonProjection = bson_new();
                };

                ~Command() {

                    if (_ptrBsonPredicate != nullptr) {
                        bson_destroy(_ptrBsonPredicate);
                    }

                    if (_ptrBsonProjection != nullptr) {
                        bson_destroy(_ptrBsonProjection);
                    }
                }

                bson_t *_ptrBsonPredicate = nullptr;
                bson_t *_ptrBsonProjection = nullptr;

                bson_error_t error;
                bson_oid_t oid;

                const mongoc_client_t &client;
                const mongoc_collection_t &collection;

            private:
                //
            };

            class MongoDbSourceDriver::MongoDbApiImpl {

            public:
                MongoDbApiImpl() {
                    mongoc_init();
                }

                ~MongoDbApiImpl() {

                    if (_ptrCollection != nullptr) {
                        mongoc_collection_destroy(_ptrCollection);
                    }

                    if (_ptrClient != nullptr) {
                        mongoc_client_destroy(_ptrClient);
                    }

                    mongoc_cleanup();
                }

                shared_ptr<Command> createCommand() {

                    if (_ptrClient == nullptr || _ptrCollection == nullptr) {
                        string message("Either client or collection is a nullptr");
                        MongoDbSourceException ex;
                        ex.setMessage(message);
                        throw ex;
                    }

                    auto command = cldeplus_make_shared<Command>(*_ptrClient, *_ptrCollection);

                    return command;
                }

                string parseConnectionString(Options &optionArgs) {

                    string connStr = "mongodb://";

                    if (optionArgs.User.length() > 0 && optionArgs.Pass.length() > 0) {
                        connStr += optionArgs.User + ":" + optionArgs.Pass;
                        connStr += "@";
                    }

                    connStr += optionArgs.Host;
                    connStr += ":";
                    connStr += std::to_string(optionArgs.Port);

                    return connStr;
                }

                void initializeBindBuffers(const CLDEPlus::vector<shared_ptr<Column>> &columns,
                                           bson_t *bsonDoc,
                                           shared_ptr<Entity> &entity) {

                    std::for_each(columns.cbegin(),
                                  columns.cend(),
                                  [&bsonDoc, &entity](const shared_ptr<Column> &column) {

                                      auto &field = entity->getField(column->getName());
                                      auto &value = field->getValue();
                                      auto key = column->getDatasourceName().c_str();

                                      if (!value) {
                                          BSON_APPEND_NULL(bsonDoc, key);
                                          return;
                                      }

                                      auto ptrBuffer = value->RawPointerToValueBuffer();

                                      switch (column->getDataType()) {
                                          case Type::Int64:
                                              BSON_APPEND_INT64(bsonDoc,
                                                                key,
                                                                *(reinterpret_cast<const int64_t *>(ptrBuffer)));
                                              break;
                                          case Type::Varchar:
                                              BSON_APPEND_UTF8(bsonDoc,
                                                               key,
                                                               reinterpret_cast<const char *>(ptrBuffer));
                                              break;
                                          default:
                                              break;
                                      }
                                  });

                }

                void setFieldValue(const bson_iter_t *ptrIter,
                                   const shared_ptr<Column> &column,
                                   const shared_ptr<Field> &field) const {

                    using cldeFactory = Foundation::Type::cldeValueFactory;

                    bson_type_t iterType = bson_iter_type(ptrIter);

                    if (iterType == BSON_TYPE_NULL || iterType == BSON_TYPE_UNDEFINED) {
                        return;
                    }

                    switch (column->getDataType()) {
                        case Foundation::Type::cldeValueType::Int64:
                            field->setValue(cldeFactory::CreateInt64(bson_iter_as_int64(ptrIter)));
                            break;
                        case Foundation::Type::cldeValueType::Varchar:
                            field->setValue(cldeFactory::CreateVarchar(bson_iter_utf8(ptrIter, 0)));
                            break;
                        default:
                            const char *msg = "MongoDbSourceDriver does not support this type yet.";
                            throw Foundation::Exception::cldeNonSupportedDataTypeException(msg);
                    }
                }

                mongoc_client_t *_ptrClient = nullptr;
                mongoc_collection_t *_ptrCollection = nullptr;

            private:
                //
            };


            MongoDbSourceDriver::MongoDbSourceDriver(const Foundation::EntityMap &entityMap)
                    : EntitySourceDriver(entityMap),
                      _mongoDbApiImpl(new MongoDbApiImpl()) {
                Init();
            }

            void MongoDbSourceDriver::Connect() {

                string uriString = _mongoDbApiImpl->parseConnectionString(_optionArgs);

                _mongoDbApiImpl->_ptrClient = mongoc_client_new(uriString.c_str());
                _mongoDbApiImpl->_ptrCollection = mongoc_client_get_collection(_mongoDbApiImpl->_ptrClient,
                                                                               _optionArgs.Base.c_str(),
                                                                               _entityMap.TableName().c_str());
            }

            void MongoDbSourceDriver::Disconnect() {
                //
            }

            void MongoDbSourceDriver::Init() {
                //
            }

            int MongoDbSourceDriver::Load(shared_ptr<Foundation::Entity> &entity) const {

                auto &columnsForKey = _entityMap.getColumnsForKey();
                auto &columnsForGet = _entityMap.getColumnsForGet();

                shared_ptr<Command> command = _mongoDbApiImpl->createCommand();

                _mongoDbApiImpl->initializeBindBuffers(columnsForKey,
                                                       command->_ptrBsonPredicate,
                                                       entity);

                std::for_each(columnsForGet.cbegin(),
                              columnsForGet.cend(),
                              [&command](const shared_ptr<Column> &column) {
                                  BSON_APPEND_INT32(command->_ptrBsonProjection,
                                                    column->getDatasourceName().c_str(),
                                                    1);
                              });

                const bson_t *ptrDoc;
                mongoc_cursor_t *ptrCursor;
                bson_iter_t iter;

                // TODO: Implement explicit projection
                ptrCursor = mongoc_collection_find(_mongoDbApiImpl->_ptrCollection,
                                                   MONGOC_QUERY_NONE,
                                                   0,
                                                   0,
                                                   0,
                                                   command->_ptrBsonPredicate,
                                                   command->_ptrBsonProjection,
                                                   NULL);

                if (!mongoc_cursor_next(ptrCursor, &ptrDoc)) {
                    mongoc_cursor_destroy(ptrCursor);
                    return 0;
                }

                size_t err_offset;

                if (!bson_validate(ptrDoc, BSON_VALIDATE_NONE, &err_offset)) {
                    // TODO: throw an appropriate exception if document is invalid
                    fprintf(stderr, "The document failed to validate at offset: %u\n", (unsigned) err_offset);
                    mongoc_cursor_destroy(ptrCursor);
                    return 0;
                }

                if (bson_iter_init(&iter, ptrDoc)) {

                    while (bson_iter_next(&iter)) {

                        string columnName(bson_iter_key(&iter));

                        try {

                            auto &field = entity->getField(columnName);
                            auto &column = field->getColumn();

                            _mongoDbApiImpl->setFieldValue(&iter, column, field);

                        } catch (Foundation::Exception::cldeEntityException &ex) {
                            continue;
                        }
                    }
                }

                mongoc_cursor_destroy(ptrCursor);

                return 1;
            }

            int MongoDbSourceDriver::Insert(shared_ptr<Foundation::Entity> &entity) const {

                auto &columnsForKey = _entityMap.getColumnsForKey();

                shared_ptr<Command> command = _mongoDbApiImpl->createCommand();

                bson_oid_init(&command->oid, NULL);

                _mongoDbApiImpl->initializeBindBuffers(columnsForKey,
                                                       command->_ptrBsonProjection,
                                                       entity);

                if (!mongoc_collection_insert(_mongoDbApiImpl->_ptrCollection,
                                              MONGOC_INSERT_NONE,
                                              command->_ptrBsonProjection,
                                              NULL,
                                              &command->error)) {
                    // TODO: throw an approriate exception
                    return 0;
                }

                return 1;
            }

            int MongoDbSourceDriver::Save(shared_ptr<Foundation::Entity> &entity) const {

                auto &columnsForKey = _entityMap.getColumnsForKey();
                auto &columnsForUpdate = _entityMap.getColumnsForUpdate();

                shared_ptr<Command> command = _mongoDbApiImpl->createCommand();

                _mongoDbApiImpl->initializeBindBuffers(columnsForKey,
                                                       command->_ptrBsonPredicate,
                                                       entity);

                bson_t *ptrProj;
                ptrProj = bson_new();

                _mongoDbApiImpl->initializeBindBuffers(columnsForUpdate,
                                                       ptrProj,
                                                       entity);

                BSON_APPEND_DOCUMENT(command->_ptrBsonProjection, "$set", ptrProj);

                if (!mongoc_collection_update(_mongoDbApiImpl->_ptrCollection,
                                              MONGOC_UPDATE_NONE,
                                              command->_ptrBsonPredicate,
                                              command->_ptrBsonProjection,
                                              NULL,
                                              &command->error)) {
                    // TODO: throw an appropriate exception
                    printf("%s\n", command->error.message);
                    return 0;
                }

                return 1;
            }

            int MongoDbSourceDriver::Delete(shared_ptr<Foundation::Entity> &entity) const {

                auto &columnsForKey = _entityMap.getColumnsForKey();

                shared_ptr<Command> command = _mongoDbApiImpl->createCommand();

                _mongoDbApiImpl->initializeBindBuffers(columnsForKey,
                                                       command->_ptrBsonPredicate,
                                                       entity);

                if (!mongoc_collection_remove(_mongoDbApiImpl->_ptrCollection,
                                              MONGOC_REMOVE_NONE,
                                              command->_ptrBsonPredicate,
                                              NULL,
                                              &command->error)) {
                    // TODO: throw an approriate exception
                    return 0;
                }

                return 1;
            }

            MongoDbSourceDriver::SPtrProxySPtrVector MongoDbSourceDriver::Select(
                    const Foundation::EntitySourceDriver::SPtrPredicate &predicate,
                    Foundation::EntityStore &entityStore) const {

                SPtrProxySPtrVector proxies;

                return proxies;
            }
        }
    }
}


