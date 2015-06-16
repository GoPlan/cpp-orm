//
// Created by LE, Duc Anh on 6/9/15.
//

#ifndef CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_GREATER_H
#define CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_GREATER_H


#include "../PredicateLeaf.h"

namespace Cloude {
    namespace Foundation {
        namespace Query {
            namespace Comparative {

                class Greater : public Query::PredicateLeaf {

                public:
                    Greater(const SPtrColumn &column, const Type::SPtrCldeValue &value)
                            : PredicateLeaf(column, value) { };
                    ~Greater() = default;
                    Greater(const Greater &rhs) = default;
                    Greater &operator=(const Greater &rhs) = default;

                    // Predicate
                    const ComparativeType &getComparativeType() const override { return _type; }

                private:
                    static ComparativeType _type;
                };
            }
        }
    }
}


#endif //CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_GREATER_H
