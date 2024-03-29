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

#ifndef CLDEPLUS_FOUNDATION_TYPE_COMPARER_COMAPARE_H
#define CLDEPLUS_FOUNDATION_TYPE_COMPARER_COMAPARE_H

#include "../Value.h"
#include "Greater.h"
#include "Less.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Data {
            namespace Comparer {

                class Compare : public std::binary_function<SPtrValue, SPtrValue, bool> {

                public:
                    Compare() = default;
                    bool operator()(const SPtrValue &lhs, const SPtrValue &rhs) const {
                        Less lesser;
                        Greater greater;
                        return !lesser(lhs, rhs) && !greater(lhs, rhs);
                    }
                };
            }
        }
    }
}

#endif //CLDEPLUS_FOUNDATION_TYPE_COMPARER_COMAPARE_H
