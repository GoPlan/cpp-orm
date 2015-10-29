//
// Created by LE, Duc Anh on 7/2/15.
//

#ifndef CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_BOOLEANCASTER_H
#define CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_BOOLEANCASTER_H

#include "../TypeCaster.h"

namespace CLDEPlus {
    namespace Segmentation {
        namespace Transformation {
            namespace Caster {

                class BooleanCaster : public TypeCaster {

                public:
                    BooleanCaster() = default;
                    BooleanCaster(const BooleanCaster &) = default;
                    BooleanCaster(BooleanCaster &&) = default;
                    BooleanCaster &operator=(const BooleanCaster &) = default;
                    BooleanCaster &operator=(BooleanCaster &&) = default;
                    virtual ~BooleanCaster() = default;

                    // TypeCaster
                    virtual Foundation::Data::SPtrValue Cast(Foundation::Data::ValueType dataType,
                                                             const Foundation::Data::SPtrValue &value) const override;
                };
            }
        }
    }
}


#endif //CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_BOOLEANCASTER_H