/*

Copyright 2015 LE, Duc-Anh

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#ifndef CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_NOTEQUAL_H
#define CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_NOTEQUAL_H

#include "../CriteriaLeaf.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Query {
            namespace Comparative {

                class NotEqual : public CriteriaLeaf {

                    static ComparativeType _type;

                public:
                    NotEqual(const SPtrColumn &column, const Data::SPtrValue &value) : CriteriaLeaf(column, value) { };
                    NotEqual(const NotEqual &) = default;
                    NotEqual(NotEqual &&) = default;
                    NotEqual &operator=(const NotEqual &) = default;
                    NotEqual &operator=(NotEqual &&) = default;
                    ~NotEqual() = default;

                    // CriteriaLeaf
                    const ComparativeType &getComparativeType() const override { return _type; };
                };
            }
        }
    }
}


#endif //CLOUD_E_CPLUS_FOUNDATION_QUERY_COMPARATIVE_NOTEQUAL_H
