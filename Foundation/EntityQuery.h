//
// Created by LE, Duc Anh on 6/9/15.
//

#ifndef CLOUD_E_CPLUS_FOUNDATION_ENTITYQUERY_H
#define CLOUD_E_CPLUS_FOUNDATION_ENTITYQUERY_H

#include <memory>
#include <vector>
#include "EntityProxy.h"
#include "EntityStore.h"
#include "Query/Criteria.h"

namespace Cloude {
    namespace Foundation {

        class EntityQuery {

            SPtrEntityStore _entityStore;

        public:
            explicit EntityQuery(const SPtrEntityStore &entityStore);
            EntityQuery(const EntityQuery &) = default;
            EntityQuery(EntityQuery &&) = default;
            EntityQuery &operator=(const EntityQuery &) = default;
            EntityQuery &operator=(EntityQuery &&) = default;
            ~EntityQuery() = default;

            // Locals
            virtual SPtrEntityProxy SelectFirst(const Query::SPtrCriteria &sptrCriteria);
            virtual SPtrEntityProxyVector Select(const Query::SPtrCriteria &sptrCriteria);
            virtual SPtrEntityProxyVector Select(const Query::SPtrCriteria &sptrCriteria,
                                                 const SPtrColumnVector &columnsForProjection);
        };

        using SPtrEntityQuery = std::shared_ptr<EntityQuery>;

        SPtrEntityQuery CreateQuerySharedPtr(const SPtrEntityStore &store);
    }
}

#endif //CLOUD_E_CPLUS_FOUNDATION_ENTITYQUERY_H
