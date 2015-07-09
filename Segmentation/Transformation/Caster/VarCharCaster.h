//
// Created by LE, Duc Anh on 7/2/15.
//

#ifndef CLOUD_E_PLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_VARCHARCASTER_H
#define CLOUD_E_PLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_VARCHARCASTER_H

#include <Segmentation/Transformation/TypeCaster.h>

namespace Cloude {
    namespace Segmentation {
        namespace Transformation {
            namespace Caster {

                class VarCharCaster : public TypeCaster {

                public:
                    VarCharCaster() = default;
                    VarCharCaster(const VarCharCaster &) = default;
                    VarCharCaster(VarCharCaster &&) = default;
                    VarCharCaster &operator=(const VarCharCaster &) = default;
                    VarCharCaster &operator=(VarCharCaster &&) = default;
                    virtual ~VarCharCaster() = default;

                    // TypeCaster
                    virtual Foundation::Data::SPtrValue Cast(Foundation::Data::ValueType dataType,
                                                             const Foundation::Data::SPtrValue &value) const override;
                };
            }
        }
    }
}


#endif //CLOUD_E_PLUS_SEGMENTATION_TRANSFORMATION_CONVERTER_VARCHARCASTER_H
