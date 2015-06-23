//
// Created by LE, Duc Anh on 6/19/15.
//

#ifndef CLOUD_E_PLUS_SEGMENTATION_HPP
#define CLOUD_E_PLUS_SEGMENTATION_HPP

#include "gtest/gtest.h"
#include <memory>
#include <iostream>
#include <functional>
#include <Cloude.h>
#include <Application/Mapper/EnquiryMap.h>
#include <Application/Mapper/EnquiryLoader.h>
#include <Foundation/Query/Helper/SqlHelper.h>
#include <Foundation/Comparer/EntityProxyComparer.h>
#include <Foundation/Segmentation/Segment.h>
#include <SourceDriver/SQLite/SQLiteSourceDriver.h>

namespace Cloude {
    namespace AppTest {

        TEST(Segmentation, case01) {

            using namespace Foundation;
            using namespace Foundation::Query;

            const char *email_01 = "goplan@cloud-e.biz";
            const char *email_02 = "ducanh.ki@cloud-e.biz";

            auto sptrEnquiryId_01 = Type::cldeValueFactory::CreateInt64(1);
            auto sptrEnquiryId_02 = Type::cldeValueFactory::CreateInt64(2);
            auto sptrEmail_01 = Type::cldeValueFactory::CreateVarchar(email_01);
            auto sptrEmail_02 = Type::cldeValueFactory::CreateVarchar(email_02);

            Application::Mapper::EnquiryMap enquiryMap;
            Application::Mapper::EnquiryLoader enquiryLoader;
            SourceDriver::SQLite::SQLiteSourceDriver sqliteSourceDriver{enquiryMap};
            auto enquiryStore = std::make_shared<EntityStore>(enquiryMap, enquiryLoader, sqliteSourceDriver);
            auto enquiryQuery = std::make_shared<EntityQuery>(enquiryStore);



        }
    }
}

#endif //CLOUD_E_PLUS_SEGMENTATION_HPP
