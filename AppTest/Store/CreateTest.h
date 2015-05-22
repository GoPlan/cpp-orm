//
// Created by LE, Duc-Anh on 5/21/15.
//

#ifndef CLOUD_E_CPLUS_CREATETEST_H
#define CLOUD_E_CPLUS_CREATETEST_H

#include "gtest/gtest.h"
#include "StoreSimple.h"
#include "../../Architecture/Cloude.h"

using namespace Cloude::AppTest::Store;
using namespace Cloude::Architecture::Foundation;

namespace Cloude {
    namespace AppTest {
        namespace EntityStore {

            TEST_F(StoreSimple, CreateEntity) {

                const std::string codeName = "Code";
                const std::string codeValue = "VNM";

                Column columnId(codeName, codeName, DbType::String);

                Field fieldId(columnId);
                fieldId.setString(codeValue);

                Identity identity(&fieldId);

                auto stockGroupPtr = _entityStore.Create(identity);
                auto stockGroupIdentyPtr = &stockGroupPtr->getIdentity();
                auto stockGroupFieldIdPtr = stockGroupIdentyPtr->GetFieldPtr("Code");

                EXPECT_EQ(true, stockGroupPtr != NULL && stockGroupPtr != nullptr);
                EXPECT_EQ(&identity, stockGroupIdentyPtr);
                EXPECT_EQ(&fieldId, stockGroupFieldIdPtr);
                EXPECT_EQ(codeName, stockGroupFieldIdPtr->getColumn().getName());
                EXPECT_EQ(codeValue, stockGroupFieldIdPtr->getString());
            }
        }
    }
}


#endif //CLOUD_E_CPLUS_CREATETEST_H