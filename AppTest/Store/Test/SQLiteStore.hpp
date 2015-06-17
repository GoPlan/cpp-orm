//
// Created by LE, Duc Anh on 6/8/15.
//

#ifndef CLOUD_E_CPLUS_SQLITESTORE_HPP
#define CLOUD_E_CPLUS_SQLITESTORE_HPP

#include "gtest/gtest.h"
#include "../EnquirySQLiteStore.h"

using namespace std;

namespace Cloude {
    namespace AppTest {
        namespace Store {
            namespace Test {

                using Field = Cloude::Foundation::Field;
                using Identity = Cloude::Foundation::Identity;

                TEST_F(EnquirySQLiteStore, CreateGetSaveDelete) {

                    const char *email = "goplan@cloud-e.biz";

                    auto spEnquiryId = Foundation::Type::cldeValueFactory::CreateInt64(15);
                    auto spEnquiryEmail = Foundation::Type::cldeValueFactory::CreateVarchar(email);

                    auto spEnquiryIdField = make_shared<Field>(_enquiryMap.EnquiryId);
                    spEnquiryIdField->setValue(spEnquiryId);

                    auto initFieldList{spEnquiryIdField};
                    auto spIdentity = std::make_shared<Identity>(initFieldList);

                    // CREATE
                    {
                        auto spEntity = _entityStore.Create(spIdentity);
                        ASSERT_TRUE(spEntity.get() != 0);
                        ASSERT_TRUE(_entityStore.HasIdentityInMap(spIdentity));
                    }

                    // GET & SAVE
                    {
                        auto spEntity = _entityStore.Get(spIdentity);
                        ASSERT_TRUE(spEntity.get() != 0);
                        ASSERT_TRUE(_entityStore.HasIdentityInMap(spIdentity));

                        auto &spEmailField = spEntity->getField("Email");
                        ASSERT_TRUE(spEmailField.get() != 0);

                        spEmailField->setValue(spEnquiryEmail);

                        _entityStore.Save(spEntity);
                        _entityStore.Clear();

                        ASSERT_TRUE(!_entityStore.HasIdentityInMap(spIdentity));
                        ASSERT_TRUE(_entityStore.Size() == 0);
                    }

                    // DELETE
                    {
                        auto sptrEntity = _entityStore.Get(spIdentity);
                        ASSERT_TRUE(sptrEntity.get() != 0);
                        ASSERT_TRUE(_entityStore.HasIdentityInMap(spIdentity));

                        auto &sptrEmailField = sptrEntity->getField("Email");
                        auto &sptrEmailValue = sptrEmailField->getValue();
                        EXPECT_TRUE(strcmp(email, sptrEmailValue->ToCString()) == 0);

                        _entityStore.Delete(sptrEntity);
                        EXPECT_TRUE(!_entityStore.HasIdentityInMap(spIdentity));
                        EXPECT_TRUE(_entityStore.Size() == 0);

                        auto spEntityAlt = _entityStore.Get(spIdentity);
                        EXPECT_TRUE(spEntityAlt.get() == 0);
                    }
                }
            }
        }
    }
}

#endif //CLOUD_E_CPLUS_SQLITESTORE_HPP
