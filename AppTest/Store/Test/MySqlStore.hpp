//
// Created by LE, Duc-Anh on 5/21/15.
//

#ifndef CLOUD_E_CPLUS_STORECREATETEST_H
#define CLOUD_E_CPLUS_STORECREATETEST_H

#include <Cloude.h>
#include <gtest/gtest.h>
#include <Application/Mapper/StockGroupMap.h>
#include <Application/Mapper/StockGroupLoader.h>
#include "../StockGroupMySqlStore.h"

using namespace std;

namespace Cloude {
    namespace AppTest {
        namespace Store {
            namespace Test{

                using Column = Cloude::Architecture::Column;
                using Field = Cloude::Architecture::Field;
                using Identity = Cloude::Architecture::Identity;
                using EnumDbType = Cloude::Architecture::Enumeration::DbType;

                using StockGroupMap = Cloude::Application::Mapper::StockGroupMap;
                using StockGroupLoader = Cloude::Application::Mapper::StockGroupLoader;
                using StockGroupMySqlStore = Cloude::AppTest::Store::StockGroupMySqlStore;

                TEST_F(StockGroupMySqlStore, CreateGetSaveDelete) {

                    std::string codeValue("VNM");
                    std::string nameValue("Vinamilk");

                    auto spFieldCode = make_shared<Field>(StockGroupMap::Code);
                    spFieldCode->setCString(codeValue.c_str());

                    // setMultiFields(initializer_list<shared_ptr<Field>>()
                    auto initFieldList{spFieldCode};
                    auto spIdentity = std::make_shared<Identity>(initFieldList);

                    // CREATE
                    {
                        auto spEntity = _entityStore.Create(spIdentity);
                        auto spCodeFieldAlt = spEntity->GetField("Code");
                        auto codeValue = spCodeFieldAlt->getCString();
                        auto codeLen = strlen(codeValue);

                        ASSERT_TRUE(spEntity.get() != 0);
                        ASSERT_TRUE(codeLen > 0);
                        ASSERT_TRUE(_entityStore.HasIdentityInMap(spIdentity));
                    }

                    // SAVE
                    {
                        auto spEntity = _entityStore.Get(spIdentity);
                        auto spNameField = spEntity->operator[](StockGroupMap::UniqueName->getName());

                        spNameField->setCString(nameValue.c_str());

                        _entityStore.Save(spEntity);
                    }

                    // CLEAR
                    {
                        _entityStore.Clear();
                        ASSERT_TRUE(_entityStore.HasIdentityInMap(spIdentity) == false);
                        ASSERT_TRUE(_entityStore.Size() == 0);
                    }

                    // GET - Check for saved field
                    {
                        auto spEntity = _entityStore.Get(spIdentity);
                        ASSERT_TRUE(spEntity.get() != 0);
                        auto spCodeFieldAlt = spEntity->operator[](StockGroupMap::Code->getName());
                        auto spNameFieldAlt = spEntity->operator[](StockGroupMap::UniqueName->getName());
                        EXPECT_TRUE(_entityStore.Size() > 0);
                        EXPECT_TRUE(_entityStore.HasIdentityInMap(spIdentity) == true);
                        EXPECT_TRUE(strcmp(codeValue.c_str(), spCodeFieldAlt->getCString()) == 0);
                        EXPECT_TRUE(strcmp(nameValue.c_str(), spNameFieldAlt->getCString()) == 0);

                        _entityStore.Delete(spEntity);
                    }

                    // DELETE - Check if Entity is nullptr
                    {
                        auto spEntity = _entityStore.Get(spIdentity);
                        ASSERT_TRUE(spEntity.get() == 0);
                    }
                }
            }
        }
    }
}

#endif //CLOUD_E_CPLUS_STORECREATETEST_H