//
// Created by LE, Duc Anh on 5/31/15.
//

#ifndef CLOUD_E_CPLUS_ENTITYSOURCEDRIVER_H
#define CLOUD_E_CPLUS_ENTITYSOURCEDRIVER_H

#include <memory>

namespace Cloude {
    namespace Architecture {

        class EntityMap;

        class Entity;

        class EntitySourceDriver {
        public:
            virtual ~EntitySourceDriver() = default;

            explicit EntitySourceDriver(EntityMap &entityMap) : _entityMap(entityMap) { };

            virtual int LoadEntity(std::shared_ptr<Entity> &entity, const EntityMap &entityMap) const = 0;
            virtual int CreateEntity(std::shared_ptr<Entity> &entity, const EntityMap &entityMap) const = 0;
            virtual int SaveEntity(std::shared_ptr<Entity> &entity, const EntityMap &entityMap) const = 0;
            virtual int DeleteEntity(std::shared_ptr<Entity> &entity, const EntityMap &entityMap) const = 0;

        protected:
            const EntityMap &_entityMap;
        };
    }
}


#endif //CLOUD_E_CPLUS_ENTITYSOURCEDRIVER_H
