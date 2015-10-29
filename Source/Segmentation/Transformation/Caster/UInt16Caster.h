//
// Created by LE, Duc Anh on 7/1/15.
//

#ifndef CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_UINT16CASTER_H
#define CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_UINT16CASTER_H

#include "../TypeCaster.h"

namespace CLDEPlus {
    namespace Segmentation {
        namespace Transformation {
            namespace Caster {

                class UInt16Caster : public TypeCaster {

                public:
                    UInt16Caster() = default;
                    UInt16Caster(const UInt16Caster &) = default;
                    UInt16Caster(UInt16Caster &&) = default;
                    UInt16Caster &operator=(const UInt16Caster &) = default;
                    UInt16Caster &operator=(UInt16Caster &&) = default;
                    virtual ~UInt16Caster() = default;

                    // TypeCaster
                    virtual Foundation::Data::SPtrValue Cast(Foundation::Data::ValueType dataType,
                                                             const Foundation::Data::SPtrValue &value) const override;
                };
            }
        }
    }
}


#endif //CLDEPLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_UINT16CASTER_H