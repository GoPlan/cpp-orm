/*

Copyright 2015 LE, Duc-Anh

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include <cstdlib>
#include "../../Foundation/Data/ValueFactory.h"
#include "../../Foundation/Query/CriteriaComposite.h"
#include "../../Foundation/Query/Comparative.h"
#include "../../Foundation/Query/Helper/SqlHelper.h"
#include "../../Foundation/Query/Helper/CriteriaHelper.h"
#include "../../Foundation/Store/Helper/EntityStoreHelper.h"
#include "../../Foundation/Exception/CLDENonSupportedDataTypeException.h"
#include "Amalgamation/sqlite3.h"
#include "SQLiteSourceDriver.h"

using namespace CLDEPlus::Drivers::SQLite;

namespace CLDEPlus {
    namespace Drivers {
        namespace SQLite {

            class Command {

            public:
                Command(string const &query) : query(query) { };
                Command(Command const &) = default;
                Command(Command &&) = default;
                Command &operator=(Command const &) = default;
                Command &operator=(Command &&) = default;
                ~Command() { if (_ptrStmt != nullptr) { sqlite3_finalize(_ptrStmt); }};

                int prepareStatment(sqlite3 *ptrDb) {

                    if (ptrDb == NULL) {
                        string msg{"sqlite3 pointer is either nullptr or NULL"};
                        throw std::invalid_argument{msg};
                    }

                    int resultCode = sqlite3_prepare_v2(ptrDb, query.c_str(), -1, &_ptrStmt, NULL);

                    if (resultCode != SQLITE_OK) {
                        throw SQLiteSourceException{resultCode};
                    }

                    return 1;
                }

                sqlite3_stmt *_ptrStmt = nullptr;
                string const &query;
            };

            using SqlHelper = Foundation::Query::Helper::SqlHelper;
            using ValueFactory = Foundation::Data::ValueFactory;
            using ValueType = Foundation::Data::ValueType;
            using UPtrCommand = unique_ptr<Command>;

            class SQLiteSourceDriver::SQLiteApiImpl {

                string &connectionString;
                sqlite3 *_ptrSqlite3 = nullptr;

            public:
                SQLiteApiImpl(string &connectionString) : connectionString(connectionString) {
                    //
                };

                ~SQLiteApiImpl() {
                    if (_ptrSqlite3 != nullptr) {
                        sqlite3_close(_ptrSqlite3);
                    }
                };

                int Connect(const char *connString = 0) {

                    const char *tmpConnString = connString == 0 ? connectionString.c_str() : connString;

                    int resultCode = sqlite3_open_v2(tmpConnString, &_ptrSqlite3, SQLITE_OPEN_READWRITE, NULL);

                    if (resultCode != SQLITE_OK) {
                        throw SQLiteSourceException{resultCode};
                    }

                    return 1;
                }

                UPtrCommand createCommand(const string &query) {
                    UPtrCommand command(new Command(query));
                    command->prepareStatment(_ptrSqlite3);
                    return command;
                }

                int initializeParamBindBuffers(const Foundation::SPtrColumnVector &columnsList,
                                               const UPtrCommand &sptrCommand,
                                               Foundation::SPtrEntity &entity) {

                    int index = 1;

                    std::for_each(columnsList.cbegin(), columnsList.cend(),
                                  [&sptrCommand, &entity, &index](const Foundation::SPtrColumn &column) {

                                      auto &field = entity->getCell(column->getName());
                                      auto &value = field->getValue();

                                      if (!value) {
                                          sqlite3_bind_null(sptrCommand->_ptrStmt, index++);
                                          return;
                                      }

                                      auto const ptrValueBuffer = field->getValue()->PointerToBuffer();

                                      switch (column->getDataType()) {

                                          case ValueType::Int32: {
                                              auto tmp = reinterpret_cast<const int32_t *>(ptrValueBuffer);
                                              sqlite3_bind_int(sptrCommand->_ptrStmt, index++, *tmp);
                                              break;
                                          }

                                          case ValueType::Int64: {
                                              auto tmp = reinterpret_cast<const sqlite3_int64 *>(ptrValueBuffer);
                                              sqlite3_bind_int64(sptrCommand->_ptrStmt, index++, *tmp);
                                              break;
                                          }

                                          case ValueType::Double: {
                                              auto tmp = reinterpret_cast<const double *>(ptrValueBuffer);
                                              sqlite3_bind_double(sptrCommand->_ptrStmt, index++, *tmp);
                                              break;
                                          }

                                          case ValueType::VarChar: {
                                              auto tmp = static_cast<const char *>(ptrValueBuffer);
                                              sqlite3_bind_text(sptrCommand->_ptrStmt, index++, tmp, -1, SQLITE_STATIC);
                                              break;
                                          }

                                          case ValueType::Text: {
                                              auto tmp = static_cast<const char *>(ptrValueBuffer);
                                              sqlite3_bind_text(sptrCommand->_ptrStmt, index++, tmp, -1, SQLITE_STATIC);
                                              break;
                                          }

                                          default: {
                                              ++index;
                                              string msg{"This type is not yet supported"};
                                              throw Foundation::Exception::CLDENonSupportedDataTypeException{msg};
                                          }
                                      }
                                  });
                    return 1;
                }

                int initializeParamBindBuffers(const Foundation::Query::SPtrCriteriaVector &params,
                                               const UPtrCommand &uptrCommand) {

                    int index = 1;

                    std::for_each(params.cbegin(), params.cend(),
                                  [&uptrCommand, &index](const Foundation::Query::SPtrCriteria &sptrCriteria) {

                                      auto &column = sptrCriteria->getColumn();
                                      auto &value = sptrCriteria->getValue();

                                      if (!value) {
                                          sqlite3_bind_null(uptrCommand->_ptrStmt, index++);
                                          return;
                                      }

                                      const auto ptrValueBuffer = sptrCriteria->getValue()->PointerToBuffer();

                                      switch (column->getDataType()) {

                                          case ValueType::Int32: {
                                              auto tmp = reinterpret_cast<const int32_t *>(ptrValueBuffer);
                                              sqlite3_bind_int(uptrCommand->_ptrStmt, index++, *tmp);
                                              break;
                                          }

                                          case ValueType::Int64: {
                                              auto tmp = reinterpret_cast<const sqlite3_int64 *>(ptrValueBuffer);
                                              sqlite3_bind_int64(uptrCommand->_ptrStmt, index++, *tmp);
                                              break;
                                          }

                                          case ValueType::Double: {
                                              auto tmp = reinterpret_cast<const double *>(ptrValueBuffer);
                                              sqlite3_bind_double(uptrCommand->_ptrStmt, index++, *tmp);
                                              break;
                                          }

                                          case ValueType::VarChar: {
                                              auto tmp = static_cast<const char *>( ptrValueBuffer);
                                              sqlite3_bind_text(uptrCommand->_ptrStmt, index++, tmp, -1, SQLITE_STATIC);
                                              break;
                                          }

                                          case ValueType::Text: {
                                              auto tmp = static_cast<const char *>( ptrValueBuffer);
                                              sqlite3_bind_text(uptrCommand->_ptrStmt, index++, tmp, -1, SQLITE_STATIC);
                                              break;
                                          }

                                          default: {
                                              ++index;
                                              string msg{"This type is not yet supported"};
                                              throw Foundation::Exception::CLDENonSupportedDataTypeException{msg};
                                          }
                                      }
                                  });

                    return 1;
                }

                int bindResultToFields(const Foundation::SPtrEntity &entity,
                                       const Foundation::SPtrColumnVector &columnsVector,
                                       const UPtrCommand &uptrCommand) const {

                    int index = 0;

                    for_each(columnsVector.cbegin(), columnsVector.cend(),
                             [&entity, &uptrCommand, &index](const CLDEPlus::Foundation::SPtrColumn &column) {

                                 if (sqlite3_column_type(uptrCommand->_ptrStmt, index) == SQLITE_NULL) {
                                     ++index;
                                     return;
                                 }

                                 auto &sptrField = entity->getCell(column->getName());

                                 switch (column->getDataType()) {

                                     case CLDEPlus::Foundation::Data::ValueType::Int32: {
                                         auto value = sqlite3_column_int(uptrCommand->_ptrStmt, index++);
                                         sptrField->setValue(ValueFactory::CreateInt32(value));
                                         break;
                                     }

                                     case CLDEPlus::Foundation::Data::ValueType::Int64: {
                                         auto value = sqlite3_column_int64(uptrCommand->_ptrStmt, index++);
                                         sptrField->setValue(ValueFactory::CreateInt64(value));
                                         break;
                                     }

                                     case CLDEPlus::Foundation::Data::ValueType::VarChar: {
                                         auto value = sqlite3_column_text(uptrCommand->_ptrStmt, index++);
                                         auto cstr = reinterpret_cast<const char *>(value);
                                         sptrField->setValue(ValueFactory::CreateVarChar(cstr));
                                         break;
                                     }

                                     case CLDEPlus::Foundation::Data::ValueType::Text: {
                                         auto value = sqlite3_column_text(uptrCommand->_ptrStmt, index++);
                                         auto cstr = reinterpret_cast<const char *>(value);
                                         sptrField->setValue(ValueFactory::CreateVarChar(cstr));
                                         break;
                                     }

                                     case Foundation::Data::ValueType::Double: {
                                         auto value = sqlite3_column_double(uptrCommand->_ptrStmt, index++);
                                         sptrField->setValue(ValueFactory::CreateDouble(value));
                                         break;
                                     }

                                     default: {
                                         ++index;
                                         string msg{"This type is not yet supported"};
                                         throw Foundation::Exception::CLDENonSupportedDataTypeException{msg};
                                     }
                                 }
                             });

                    return 1;
                }

                int bindResultToFields(const Foundation::SPtrEntityProxy &proxy,
                                       const Foundation::SPtrColumnVector &columnsVector,
                                       const UPtrCommand &uptrCommand) const {

                    int index = 0;

                    for_each(columnsVector.cbegin(), columnsVector.cend(),
                             [&proxy, &uptrCommand, &index](const CLDEPlus::Foundation::SPtrColumn &column) {

                                 if (sqlite3_column_type(uptrCommand->_ptrStmt, index) == SQLITE_NULL) {
                                     ++index;
                                     return;
                                 }

                                 auto &sptrField = proxy->getCell(column->getName());

                                 switch (column->getDataType()) {

                                     case CLDEPlus::Foundation::Data::ValueType::Int32: {
                                         auto value = sqlite3_column_int(uptrCommand->_ptrStmt, index++);
                                         sptrField->setValue(ValueFactory::CreateInt32(value));
                                         break;
                                     }

                                     case CLDEPlus::Foundation::Data::ValueType::Int64: {
                                         auto value = sqlite3_column_int64(uptrCommand->_ptrStmt, index++);
                                         sptrField->setValue(ValueFactory::CreateInt64(value));
                                         break;
                                     }

                                     case Foundation::Data::ValueType::Double: {
                                         auto value = sqlite3_column_double(uptrCommand->_ptrStmt, index++);
                                         sptrField->setValue(ValueFactory::CreateDouble(value));
                                         break;
                                     }

                                     case CLDEPlus::Foundation::Data::ValueType::VarChar: {
                                         auto value = sqlite3_column_text(uptrCommand->_ptrStmt, index++);
                                         auto cstr = reinterpret_cast<const char *>(value);
                                         sptrField->setValue(ValueFactory::CreateVarChar(cstr));
                                         break;
                                     }

                                     case CLDEPlus::Foundation::Data::ValueType::Text: {
                                         auto value = sqlite3_column_text(uptrCommand->_ptrStmt, index++);
                                         auto cstr = reinterpret_cast<const char *>(value);
                                         sptrField->setValue(ValueFactory::CreateVarChar(cstr));
                                         break;
                                     }

                                     default: {
                                         ++index;
                                         string msg{"This type is not yet supported"};
                                         throw Foundation::Exception::CLDENonSupportedDataTypeException{msg};
                                     }
                                 }
                             });

                    return 1;
                }
            };

            SQLiteSourceDriver::SQLiteSourceDriver(const Foundation::SPtrEntityMap &entityMap)
                    : EntitySourceDriver(entityMap), _sqliteApiImpl(new SQLiteApiImpl(_optionArgs.ConnectionString)) {
                Init();
            }

            SQLiteSourceDriver::~SQLiteSourceDriver() {
                Disconnect();
            }

            void SQLiteSourceDriver::Init() {

                assert(!getEntityMap()->getTableName().empty());
                assert(!getEntityMap()->getColumnsMap().empty());
                assert(!getEntityMap()->getColumnsForKey().empty());
                assert(!getEntityMap()->getColumnsForGet().empty());
                assert(!getEntityMap()->getColumnsForUpdate().empty());
                assert(!getEntityMap()->getColumnsForSelect().empty());

                auto fptrValueProcessor = [](const Foundation::SPtrColumn &column, const int &index) -> string {
                    return string("?");
                };

                auto fptrProcessor = [](const Foundation::SPtrColumn &column, const int &index) -> string {
                    return string(column->getDatasourceName() + " = " + "?");
                };

                auto &sourceName = getEntityMap()->getTableName();
                auto &columnsForKey = getEntityMap()->getColumnsForKey();
                auto &columnsForGet = getEntityMap()->getColumnsForGet();
                auto &columnsForUpdate = getEntityMap()->getColumnsForUpdate();

                _getStatement = SqlHelper::CreateGetPreparedQuery(sourceName, columnsForGet, columnsForKey, fptrProcessor);
                _insertStatement = SqlHelper::CreateInsertPreparedQuery(sourceName, columnsForKey, fptrValueProcessor);
                _updateStatement = SqlHelper::CreateUpdatePreparedQuery(sourceName, columnsForUpdate, columnsForKey, fptrProcessor);
                _deleteStatement = SqlHelper::CreateDeletePreparedQuery(sourceName, columnsForKey, fptrProcessor);
            }

            void SQLiteSourceDriver::Connect() {

                if (!_sqliteApiImpl) {
                    _sqliteApiImpl = cldeplus_make_shared<SQLiteApiImpl>(_optionArgs.ConnectionString);
                }

                _sqliteApiImpl->Connect();
            }

            void SQLiteSourceDriver::Disconnect() {
                _sqliteApiImpl.reset();
            }

            int SQLiteSourceDriver::Load(Foundation::SPtrEntity &entity) const {

                auto &columnsForGet = getEntityMap()->getColumnsForGet();
                auto &columnsForKey = getEntityMap()->getColumnsForKey();
                auto uptrCommand = _sqliteApiImpl->createCommand(_getStatement);

                _sqliteApiImpl->initializeParamBindBuffers(columnsForKey, uptrCommand, entity);

                int resultCode = sqlite3_step(uptrCommand->_ptrStmt);

                switch (resultCode) {
                    case SQLITE_DONE:
                        return 0;
                    case SQLITE_ROW:
                        _sqliteApiImpl->bindResultToFields(entity, columnsForGet, uptrCommand);
                        return 1;
                    default:
                        throw SQLiteSourceException(resultCode);
                }
            }

            int SQLiteSourceDriver::Insert(Foundation::SPtrEntity &entity) const {

                auto &columnsForKey = getEntityMap()->getColumnsForKey();
                auto uptrCommand = _sqliteApiImpl->createCommand(_insertStatement);

                _sqliteApiImpl->initializeParamBindBuffers(columnsForKey, uptrCommand, entity);

                int resultCode = sqlite3_step(uptrCommand->_ptrStmt);

                if (resultCode != SQLITE_DONE) {
                    throw SQLiteSourceException(resultCode);
                }

                return 1;
            }

            int SQLiteSourceDriver::Save(Foundation::SPtrEntity &entity) const {

                auto &columnsForUpdate = getEntityMap()->getColumnsForUpdate();
                auto &columnsForKey = getEntityMap()->getColumnsForKey();
                auto size = columnsForUpdate.size() + columnsForKey.size();

                Foundation::SPtrColumnVector columnsList;
                columnsList.reserve(size);
                columnsList.insert(columnsList.end(), columnsForUpdate.begin(), columnsForUpdate.end());
                columnsList.insert(columnsList.end(), columnsForKey.begin(), columnsForKey.end());

                auto uptrCommand = _sqliteApiImpl->createCommand(_updateStatement);

                _sqliteApiImpl->initializeParamBindBuffers(columnsList, uptrCommand, entity);

                int resultCode = sqlite3_step(uptrCommand->_ptrStmt);

                if (resultCode != SQLITE_DONE) {
                    throw SQLiteSourceException(resultCode);
                }

                return 1;
            }

            int SQLiteSourceDriver::Delete(Foundation::SPtrEntity &entity) const {

                auto &columnsForKey = getEntityMap()->getColumnsForKey();
                auto uptrCommand = _sqliteApiImpl->createCommand(_deleteStatement);

                _sqliteApiImpl->initializeParamBindBuffers(columnsForKey, uptrCommand, entity);

                int resultCode = sqlite3_step(uptrCommand->_ptrStmt);

                if (resultCode != SQLITE_DONE) {
                    throw SQLiteSourceException(resultCode);
                }

                return 1;
            }

            Foundation::SPtrEntityProxyVector SQLiteSourceDriver::Select(
                    const Foundation::Query::SPtrCriteria &sptrCriteria,
                    const Foundation::SPtrColumnVector &columnsForProjection) const {

                auto fptrConditionProcessor =
                        [](const Foundation::SPtrColumn &column, const int &index) -> string {
                            return string{"?"};
                        };

                auto tuplQuery = SqlHelper::CreateSelectPreparedQuery(getEntityMap()->getTableName(),
                                                                      columnsForProjection,
                                                                      sptrCriteria,
                                                                      fptrConditionProcessor);

                auto uptrCommand = _sqliteApiImpl->createCommand(tuplQuery.first);

                _sqliteApiImpl->initializeParamBindBuffers(tuplQuery.second, uptrCommand);

                Foundation::SPtrEntityProxyVector proxies;

                int resultCode;

                while ((resultCode = sqlite3_step(uptrCommand->_ptrStmt)) == SQLITE_ROW) {

                    Foundation::SPtrEntityProxy sptrProxy{new Foundation::EntityProxy{}};
                    Foundation::Store::Helper::EntityStoreHelper::GenerateCellsFromColumns(columnsForProjection,
                                                                                           sptrProxy);

                    _sqliteApiImpl->bindResultToFields(sptrProxy, columnsForProjection, uptrCommand);

                    proxies.push_back(sptrProxy);
                }

                if (resultCode != SQLITE_DONE) {
                    throw SQLiteSourceException{resultCode};
                }

                return proxies;
            }
        }
    }
}