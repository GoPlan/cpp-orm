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

#ifndef CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_DOUBLECASTER_H
#define CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_DOUBLECASTER_H

#include "../TypeCaster.h"

namespace CLDEPlus {
    namespace Segmentation {
        namespace Transformation {
            namespace Caster {

                class DoubleCaster : public TypeCaster {

                public:
                    DoubleCaster() = default;
                    DoubleCaster(const DoubleCaster &) = default;
                    DoubleCaster(DoubleCaster &&) = default;
                    DoubleCaster &operator=(const DoubleCaster &) = default;
                    DoubleCaster &operator=(DoubleCaster &&) = default;
                    virtual ~DoubleCaster() = default;

                    // TypeCaster
                    virtual Foundation::Data::SPtrValue Cast(Foundation::Data::ValueType dataType,
                                                             const Foundation::Data::SPtrValue &value) const override;
                };
            }
        }
    }
}


#endif //CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_DOUBLECASTER_H
