//
// Created by LE, Duc Anh on 6/9/15.
//

#ifndef CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_NOTLIKE_H
#define CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_NOTLIKE_H

#include "../CriteriaLeaf.h"

namespace Cloude {
    namespace Foundation {
        namespace Query {
            namespace Comparative {

                class NotLike : public CriteriaLeaf {
                    static ComparativeType _type;

                public:
                    NotLike(const SPtrColumn &column, const Type::SPtrCldeValue &value)
                            : CriteriaLeaf(column, value) { };
                    ~NotLike() = default;
                    NotLike(const NotLike &rhs) = default;
                    NotLike &operator=(const NotLike &rhs) = default;

                    // Criteria
                    const ComparativeType &getComparativeType() const override { return _type; };
                };
            }
        }
    }
}


#endif //CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_NOTLIKE_H
