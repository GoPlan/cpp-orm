//
// Created by GoPlan on 15/05/2015.
//

#ifndef CLOUD_E_CPLUS_STOCKGROUPLOADER_H
#define CLOUD_E_CPLUS_STOCKGROUPLOADER_H

#include "../../Architecture/Foundation/EntityLoader.h"
#include "../../Architecture/Foundation/EntityStore.h"
#include "../Entity/StockGroup.h"

using namespace Cloude::Architecture::Foundation;

namespace Cloude {
    namespace Application {

        class StockGroupLoader : public EntityLoader {
            virtual Entity *CreateEntityInstance(Identity &ident);
            virtual Identity *NextPrimaryKey();

            virtual void LoadEntity(Entity &entity);
            virtual void EstablishEntityRelationship(Entity &entity);

            virtual int InsertEntity(Entity &entity);
            virtual int SaveEntity(Entity &entity);
            virtual int DeleteEntity(Entity &entity);
        };
    }
}


#endif //CLOUD_E_CPLUS_STOCKGROUPLOADER_H
