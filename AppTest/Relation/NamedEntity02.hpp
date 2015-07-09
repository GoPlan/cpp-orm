//
// Created by LE, Duc Anh on 7/9/15.
//

#ifndef CLOUD_E_PLUS_NAMEDENTITY02_HPP
#define CLOUD_E_PLUS_NAMEDENTITY02_HPP

#include "gtest/gtest.h"
#include <Foundation/Foundation.h>
#include <Relation/Relation.h>
#include <Drivers/SQLite/SQLiteSourceDriver.h>
#include <AppTest/Application/CustomerMap.h>
#include <AppTest/Application/CustomerLoader.h>
#include <AppTest/Application/PreOrderMap.h>
#include <AppTest/Application/PreOrderLoader.h>
#include <AppTest/Entity/Customer.h>
#include <AppTest/Entity/PreOrder.h>


namespace Cloude {
    namespace AppTest {
        namespace Store {

            TEST(Relation, NamedEntity02) {

                Application::CustomerMap mapCustomer{};
                Application::PreOrderMap mapPreOrder{};
                Application::CustomerLoader loaderCustomer{};
                Application::PreOrderLoader loaderPreOrder{};

                Drivers::SQLite::SQLiteSourceDriver driverCustomer{mapCustomer};
                Drivers::SQLite::SQLiteSourceDriver driverPreOrder{mapPreOrder};
                driverCustomer.OptionArgs().ConnectionString = "../ex1.db";
                driverPreOrder.OptionArgs().ConnectionString = "../ex1.db";

                auto sptrCustomerStore = Foundation::CreateStoreSharedPtr(mapCustomer, loaderCustomer, driverCustomer);
                auto sptrPreOrderStore = Foundation::CreateStoreSharedPtr(mapPreOrder, loaderPreOrder, driverPreOrder);
                auto sptrCustomerQuery = Foundation::CreateQuerySharedPtr(sptrCustomerStore);
                auto sptrPreOrderQuery = Foundation::CreateQuerySharedPtr(sptrPreOrderStore);

                auto fptrCustomerCreator =
                        [&mapPreOrder, &sptrPreOrderQuery](const Foundation::Entity &entity) -> Entity::Customer {

                            using namespace Foundation::Data;
                            using namespace Foundation::Query;

                            Entity::Customer customer{};
                            customer.setId(NumericToPrimative<int64_t>(entity.getCell("Id")->getValue()));
                            customer.setFirstName(entity.getCell("FirstName")->ToString());
                            customer.setLastName(entity.getCell("LastName")->ToString());
                            customer.setEmail(entity.getCell("Email")->ToString());

                            // LinkToMany to PreOrder
                            SPtrCriteria criteria{new Comparative::Equal{mapPreOrder.CustId, ValueFactory::CreateInt64(customer.getId())}};
                            customer.setSptrPreOrders(Relation::CreateLinkToMany(sptrPreOrderQuery, criteria));

                            return customer;
                        };

                auto fptrPreOrderCreator =
                        [&mapCustomer, &sptrCustomerQuery](const Foundation::Entity &entity) -> Entity::PreOrder {

                            using namespace Foundation::Data;
                            using namespace Foundation::Query;

                            Entity::PreOrder preOrder{};
                            preOrder.setId(NumericToPrimative<int64_t>(entity.getCell("Id")->getValue()));
                            preOrder.setCustomerId(NumericToPrimative<int64_t>(entity.getCell("CustId")->getValue()));
                            preOrder.setName(entity.getCell("Name")->ToString());

                            // LinkToOne to Customer
                            SPtrCriteria criteria{new Comparative::Equal{mapCustomer.Id, ValueFactory::CreateInt64(preOrder.getCustomerId())}};
                            preOrder.setSptrCustomer(Relation::CreateLinkToOne(sptrCustomerQuery, criteria));

                            return preOrder;
                        };

                driverCustomer.Connect();
                driverPreOrder.Connect();

                // PreOrder referencing Customer (LinkToOne)
                Foundation::Query::SPtrCriteria sptrPreOrderIdGt00;
                {
                    using namespace Foundation::Query;
                    using namespace Foundation::Data;
                    sptrPreOrderIdGt00.reset(new Comparative::GreaterOrEqual{mapPreOrder.Id,
                                                                             ValueFactory::CreateInt64(0)});
                }

                auto sptrPreOrderProxy = sptrPreOrderQuery->SelectFirst(sptrPreOrderIdGt00);
                auto sptrPreOrderEntity = sptrPreOrderProxy->Summon(sptrPreOrderStore);
                auto preOrder = sptrPreOrderEntity->NamedEntity<Entity::PreOrder>(fptrPreOrderCreator);

                {
                    std::cout << "Referencing Customer from PreOrder" << std::endl;

                    auto sptrCustomerProxy = preOrder.sptrCustomer()->Call();
                    auto sptrCustomerEntity = sptrCustomerProxy->Summon(sptrCustomerStore);

                    EXPECT_TRUE(sptrCustomerProxy.get());
                    EXPECT_TRUE(sptrCustomerEntity.get());

                    std::cout << sptrCustomerEntity->ToString() << std::endl;
                }


                // Customer referencing PreOrder (LinkToMany)
                Foundation::Query::SPtrCriteria sptrCustomerIdGt00;
                {
                    using namespace Foundation::Query;
                    using namespace Foundation::Data;
                    sptrCustomerIdGt00.reset(new Comparative::GreaterOrEqual{mapPreOrder.Id,
                                                                             ValueFactory::CreateInt64(0)});
                }

                auto sptrCustomerProxy = sptrCustomerQuery->SelectFirst(sptrCustomerIdGt00);
                auto sptrCustomerEntity = sptrCustomerProxy->Summon(sptrCustomerStore);
                auto customer = sptrCustomerEntity->NamedEntity<Entity::Customer>(fptrCustomerCreator);

                {
                    std::cout << "Referencing PreOrder from Customer" << std::endl;
                    auto sptrPreOrdersVector = customer.sptrPreOrders()->Call();
                    for(auto &order : sptrPreOrdersVector){
                        std::cout << order->ToString() << std::endl;
                    }
                }

                driverCustomer.Disconnect();
                driverPreOrder.Disconnect();
            }
        }
    }
}

#endif //CLOUD_E_PLUS_NAMEDENTITY02_HPP
