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

#ifndef CLDEPLUS_SEGMENTATION_TRANSFORMATION_TYPECASTER_H
#define CLDEPLUS_SEGMENTATION_TRANSFORMATION_TYPECASTER_H

#include <memory>
#include "../../Foundation/Data/Value.h"
#include "../../Foundation/Column.h"
#include <unordered_map>

namespace CLDEPlus {
    namespace Segmentation {
        namespace Transformation {

            class TypeCaster {

            public:
                TypeCaster(const TypeCaster &) = default;
                TypeCaster(TypeCaster &&) = default;
                TypeCaster &operator=(const TypeCaster &) = default;
                TypeCaster &operator=(TypeCaster &&) = default;
                ~TypeCaster() = default;

                // Locals
                virtual Foundation::Data::SPtrValue Cast(Foundation::Data::ValueType dataType,
                                                         Foundation::Data::SPtrValue const &value) const;

            protected:
                TypeCaster() = default;
            };


            using SPtrCaster = shared_ptr<TypeCaster>;
        }
    }
}


#endif //CLDEPLUS_SEGMENTATION_TRANSFORMATION_TYPECASTER_H
