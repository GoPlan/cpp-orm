//
// Created by LE, Duc Anh on 6/9/15.
//

#ifndef CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_LIKE_H
#define CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_LIKE_H

#include "../PredicateLeaf.h"

namespace Cloude {
    namespace Foundation {
        namespace Query {
            namespace Comparative {

                class Like : public PredicateLeaf {

                public:
                    Like(const Column &column, const Type::SPtrCldeValue &value) : PredicateLeaf(column, value) { };
                    ~Like() = default;
                    Like(const Like &rhs) = default;
                    Like &operator=(const Like &rhs) = default;

                    // Predicate
                    const Enumeration::ComparativeType &getComparativeType() const override { return _type; };

                private:
                    static Enumeration::ComparativeType _type;

                };
            }
        }
    }
}


#endif //CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_LIKE_H
