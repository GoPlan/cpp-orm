//
// Created by LE, Duc Anh on 7/1/15.
//

#ifndef CLOUD_E_PLUS_SEGMENTATION_TRANSFORMATION_IMPLEMENTATION_INT16CONVERTER_H
#define CLOUD_E_PLUS_SEGMENTATION_TRANSFORMATION_IMPLEMENTATION_INT16CONVERTER_H

namespace Cloude {
    namespace Segmentation {
        namespace Transformation {
            namespace Implementation {

                class Int16Converter {

                public:
                    Int16Converter() = default;
                    Int16Converter(const Int16Converter &) = default;
                    Int16Converter(Int16Converter &&) = default;
                    Int16Converter &operator=(const Int16Converter &) = default;
                    Int16Converter &operator=(Int16Converter &&) = default;
                    virtual ~Int16Converter() = default;
                };
            }
        }
    }
}


#endif //CLOUD_E_PLUS_SEGMENTATION_TRANSFORMATION_IMPLEMENTATION_INT16CONVERTER_H
