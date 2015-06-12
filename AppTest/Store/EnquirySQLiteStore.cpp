//
// Created by LE, Duc Anh on 6/8/15.
//

#include <SourceDriver/SQLite/SQLiteSourceDriver.h>
#include "EnquirySQLiteStore.h"


namespace Cloude {
    namespace AppTest {

        EnquirySQLiteStore::EnquirySQLiteStore() : _sqliteSourceDriver(_enquiryMap),
                                                   _entityStore(_enquiryMap,
                                                                _enquiryLoader,
                                                                _sqliteSourceDriver) {
            //
        }

        void EnquirySQLiteStore::SetUp() {

            auto &options = _sqliteSourceDriver.getOptionArgs();
            options.ConnectionString = "../ex1.db";

            _sqliteSourceDriver.Connect();
        }

        void EnquirySQLiteStore::TearDown() {
            _sqliteSourceDriver.Connect();
        }
    }
}

