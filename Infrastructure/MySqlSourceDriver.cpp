//
// Created by GoPlan on 18/05/2015.
//

#include <string>
#include <iostream>
#include <mysql.h>
#include <Architecture/Exception/NonSupportedDataTypeException.h>
#include <Architecture/Helper/SqlGenerator.h>
#include "Exception/MySqlSourceDriverException.h"
#include "MySqlSourceDriver.h"

using namespace std;
using namespace Cloude;

namespace Cloude {
    namespace Infrastructure {

        class Command {
        public:
            Command() {
                //
            };

            virtual ~Command() {

                if (PtrResultLength != nullptr) {
                    free(PtrResultLength);
                }

                if (PtrResultIsNull != nullptr) {
                    free(PtrResultIsNull);
                }

                if (PtrResultError != nullptr) {
                    free(PtrResultError);
                }

                if (PtrParamsBind != nullptr) {
                    free(PtrParamsBind);
                }

                if (PtrResultBind != nullptr) {
                    free(PtrResultBind);
                }

                if (PtrStmt != nullptr) {
                    mysql_stmt_close(PtrStmt);
                }
            }

            MYSQL_STMT *PtrStmt = nullptr;
            MYSQL_BIND *PtrParamsBind = nullptr;
            MYSQL_BIND *PtrResultBind = nullptr;
            unsigned long *PtrParamsLength = nullptr;
            unsigned long *PtrResultLength = nullptr;
            my_bool *PtrResultIsNull = nullptr;
            my_bool *PtrResultError = nullptr;
        };

        class MySqlSourceDriver::MySqlApiImpl {
        public:
            MYSQL *PtrMySql = nullptr;

        public:
            MySqlApiImpl(Options &driverOptions) {
                mysql_library_init;
                PtrMySql = mysql_init(PtrMySql);
            }

            ~MySqlApiImpl() {

                if (PtrMySql != nullptr) {
                    mysql_close(PtrMySql);
                }

                mysql_library_end;
            }

            shared_ptr<Command> createCommand(const string &query) {

                auto command = make_shared<Command>();
                command->PtrStmt = mysql_stmt_init(PtrMySql);

                if (!command->PtrStmt) {
                    assertSqlError();
                }

                if (mysql_stmt_prepare(command->PtrStmt, query.c_str(), query.length())) {
                    assertStmtError(command->PtrStmt);
                }

                return command;
            }

            void assertSqlError() {

                if (mysql_errno(PtrMySql)) {
                    auto errorCharPtr = mysql_error(PtrMySql);
                    throw Exception::MySqlSourceDriverException(errorCharPtr);
                }
            }

            void assertStmtError(MYSQL_STMT *ptrMySqlStmt) {

                if (mysql_stmt_errno(ptrMySqlStmt)) {
                    auto errorCharPtr = mysql_stmt_error(ptrMySqlStmt);
                    throw Exception::MySqlSourceDriverException(errorCharPtr);
                }
            }

            void bindParamsBuffer(std::shared_ptr<Entity> &entity,
                                  const ColumnsList &columnsList,
                                  std::shared_ptr<Command> &command) {

                auto columnsListSize = columnsList.size();

                command->PtrParamsBind = (MYSQL_BIND *) calloc(columnsListSize, sizeof(MYSQL_BIND));
                command->PtrParamsLength = (unsigned long *) calloc(columnsListSize, sizeof(unsigned long));

                int i = 0;

                std::for_each(columnsList.cbegin(), columnsList.cend(),
                              [&](const std::shared_ptr<Column> &column) -> void {

                                  auto field = entity->operator[](column->getName());
                                  auto ptrLength = static_cast<unsigned long *>(field->getColumn()
                                                                                     ->PointerToLengthVariable());

                                  command->PtrParamsBind[i].is_null = 0;
                                  command->PtrParamsBind[i].error = 0;
                                  command->PtrParamsBind[i].length = ptrLength;
                                  command->PtrParamsBind[i].buffer = field->PointerToFieldValue();

                                  setupBindBuffers(field, &command->PtrParamsBind[i]);

                                  i++;
                              });
            }

            void bindResultBuffer(std::shared_ptr<Entity> &entity,
                                  const ColumnsList &columnsList,
                                  std::shared_ptr<Command> &command) {

                auto columnsListSize = columnsList.size();

                command->PtrResultBind = (MYSQL_BIND *) calloc(columnsListSize, sizeof(MYSQL_BIND));
                command->PtrResultError = (my_bool *) calloc(columnsListSize, sizeof(my_bool));
                command->PtrResultIsNull = (my_bool *) calloc(columnsListSize, sizeof(my_bool));
                command->PtrResultLength = (unsigned long *) calloc(columnsListSize, sizeof(unsigned long));

                int i = 0;

                std::for_each(columnsList.cbegin(), columnsList.cend(),
                              [&](const std::shared_ptr<Column> &column) -> void {

                                  auto field = entity->operator[](column->getName());

                                  command->PtrResultBind[i].is_null = &command->PtrResultIsNull[i];
                                  command->PtrResultBind[i].error = &command->PtrResultError[i];
                                  command->PtrResultBind[i].length = &command->PtrResultLength[i];
                                  command->PtrResultBind[i].buffer = field->PointerToFieldValue();

                                  setupBindBuffers(field, &command->PtrResultBind[i]);

                                  i++;
                              });
            }

            void setupBindBuffers(std::shared_ptr<Field> &field,
                                  MYSQL_BIND *ptrBind) {

                switch (field->getColumn()->getDbType()) {
                    case Architecture::Enumeration::DbType::Boolean:
                        ptrBind->buffer_type = MYSQL_TYPE_TINY;
                        ptrBind->buffer_length = sizeof(bool);
                        break;
                    case Architecture::Enumeration::DbType::Byte:
                        ptrBind->buffer_type = MYSQL_TYPE_TINY;
                        ptrBind->buffer_length = sizeof(char);
                        break;
                    case Architecture::Enumeration::DbType::Int16:
                        ptrBind->buffer_type = MYSQL_TYPE_SHORT;
                        ptrBind->buffer_length = sizeof(int16_t);
                        break;
                    case Architecture::Enumeration::DbType::Int32:
                        ptrBind->buffer_type = MYSQL_TYPE_LONG;
                        ptrBind->buffer_length = sizeof(int32_t);
                        break;
                    case Architecture::Enumeration::DbType::Int64:
                        ptrBind->buffer_type = MYSQL_TYPE_LONGLONG;
                        ptrBind->buffer_length = sizeof(int64_t);
                        break;
                    case Architecture::Enumeration::DbType::UInt16:
                        ptrBind->buffer_type = MYSQL_TYPE_SHORT;
                        ptrBind->buffer_length = sizeof(int16_t);
                        break;
                    case Architecture::Enumeration::DbType::UInt32:
                        ptrBind->buffer_type = MYSQL_TYPE_LONG;
                        ptrBind->buffer_length = sizeof(int16_t);
                        break;
                    case Architecture::Enumeration::DbType::UInt64:
                        ptrBind->buffer_type = MYSQL_TYPE_LONGLONG;
                        ptrBind->buffer_length = sizeof(int16_t);
                        break;
                    case Architecture::Enumeration::DbType::Double:
                        ptrBind->buffer_type = MYSQL_TYPE_DOUBLE;
                        ptrBind->buffer_length = sizeof(double);
                        break;
                    case Architecture::Enumeration::DbType::Float:
                        ptrBind->buffer_type = MYSQL_TYPE_FLOAT;
                        ptrBind->buffer_length = sizeof(float);
                        break;
                    case Architecture::Enumeration::DbType::String:
                        ptrBind->buffer_type = MYSQL_TYPE_STRING;
                        ptrBind->buffer_length = field->getColumn()->getLength();
                        break;
                    default:
                        throw Architecture::Exception::NonSupportedDataTypeException();
                }
            }
        }; // END - MySqlApiImpl

        MySqlSourceDriver::MySqlSourceDriver(EntityMap &entityMap) : EntitySourceDriver(entityMap),
                                                                     _ptrMySqlApiImpl(new MySqlApiImpl(OptionArgs)) {
            init();
        }

        MySqlSourceDriver::~MySqlSourceDriver() {
            delete _ptrMySqlApiImpl;
        }

        void MySqlSourceDriver::Connect() {

            if (_ptrMySqlApiImpl->PtrMySql == nullptr) {
                _ptrMySqlApiImpl->assertSqlError();
            }

            // TODO: To research the use of mysql_options();
            if (!mysql_real_connect(_ptrMySqlApiImpl->PtrMySql,
                                    OptionArgs.Host.c_str(),
                                    OptionArgs.User.c_str(),
                                    OptionArgs.Pass.c_str(),
                                    OptionArgs.Base.c_str(),
                                    OptionArgs.Port,
                                    NULL, 0)) {

                _ptrMySqlApiImpl->assertSqlError();
            }
        }

        void MySqlSourceDriver::Disconnect() {
            //
        }

        void MySqlSourceDriver::LoadEntity(shared_ptr<Architecture::Entity> &entity, const EntityMap &entityMap) {

            auto command = _ptrMySqlApiImpl->createCommand(_getStatement);

            _ptrMySqlApiImpl->bindParamsBuffer(entity, entityMap.getColumnsForKey(), command);
            _ptrMySqlApiImpl->bindResultBuffer(entity, entityMap.getColumnsForGet(), command);

            if (mysql_stmt_bind_param(command->PtrStmt, command->PtrParamsBind)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }

            if (mysql_stmt_bind_result(command->PtrStmt, command->PtrResultBind)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }

            if (mysql_stmt_execute(command->PtrStmt)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }

            auto rowStatus = mysql_stmt_fetch(command->PtrStmt);

            switch (rowStatus) {
                case 0:
                    break;
                case 1:
                    _ptrMySqlApiImpl->assertSqlError();
                    _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
                    break;
                case MYSQL_NO_DATA:
                    break;
                case MYSQL_DATA_TRUNCATED:
                    // TODO: To research MYSQL_DATA_TRUNCATED condition
                    _ptrMySqlApiImpl->assertSqlError();
                    _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
                    throw Exception::MySqlSourceDriverException("fetch error: MYSQL_DATA_TRUNCATED");
                default:
                    throw Exception::MySqlSourceDriverException("fetch error: unknown return status code");
            }
        }

        void MySqlSourceDriver::CreateEntity(shared_ptr<Architecture::Entity> &entity, const EntityMap &entityMap) {

            auto command = _ptrMySqlApiImpl->createCommand(_insertStatement);

            _ptrMySqlApiImpl->bindParamsBuffer(entity, entityMap.getColumnsForKey(), command);

            if (mysql_stmt_bind_param(command->PtrStmt, command->PtrParamsBind)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }

            if (mysql_stmt_execute(command->PtrStmt)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }
        }

        void MySqlSourceDriver::SaveEntity(std::shared_ptr<Entity> &entity, const EntityMap &entityMap) {

            auto command = _ptrMySqlApiImpl->createCommand(_updateStatement);
            auto columnsForUpdate = entityMap.getColumnsForUpdate();
            auto columnsForKey = entityMap.getColumnsForKey();
            auto joinedColumnsList = make_shared<ColumnsList>();

            joinedColumnsList->insert(joinedColumnsList->end(), columnsForUpdate.begin(), columnsForUpdate.end());
            joinedColumnsList->insert(joinedColumnsList->end(), columnsForKey.begin(), columnsForKey.end());

            _ptrMySqlApiImpl->bindParamsBuffer(entity, *joinedColumnsList, command);

            if (mysql_stmt_bind_param(command->PtrStmt, command->PtrParamsBind)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }

            if (mysql_stmt_execute(command->PtrStmt)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }
        }

        void MySqlSourceDriver::DeleteEntity(std::shared_ptr<Entity> &entity, const EntityMap &entityMap) {

            auto command = _ptrMySqlApiImpl->createCommand(_deleteStatement);

            _ptrMySqlApiImpl->bindParamsBuffer(entity, entityMap.getColumnsForKey(), command);

            if (mysql_stmt_bind_param(command->PtrStmt, command->PtrParamsBind)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }

            if (mysql_stmt_execute(command->PtrStmt)) {
                _ptrMySqlApiImpl->assertStmtError(command->PtrStmt);
            }
        }

        void MySqlSourceDriver::init() {

            _getStatement = Architecture::Helper::CreateGetPreparedQuery(_entityMap);
            _insertStatement = Architecture::Helper::CreateInsertPreparedQuery(_entityMap);
            _updateStatement = Architecture::Helper::CreateUpdatePreparedQuery(_entityMap);
            _deleteStatement = Architecture::Helper::CreateDeletePreparedQuery(_entityMap);

        }
    }
}