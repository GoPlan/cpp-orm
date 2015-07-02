//
// Created by LE, Duc Anh on 7/1/15.
//

#include <Foundation/Data/Helper/TypeHelper.h>
#include <Foundation/Data/ValueFactory.h>
#include <Segmentation/Exception/TransformationException.h>
#include "Int16Converter.h"

namespace Cloude {
    namespace Segmentation {
        namespace Transformation {

            Foundation::Data::SPtrValue Converter::Int16Converter::Convert(
                    Foundation::Data::ValueType dataType, const Foundation::Data::SPtrValue &value) const {

                if (value->getDataType() != Foundation::Data::ValueType::Int16) {
                    std::string type{Foundation::Data::Helper::TypeHelper::CopyToString(value->getDataType())};
                    std::string msg{type + " is not supported by Int16Converter"};
                    throw Segmentation::Exception::TransformationException{msg};
                }

                int16_t *tmp = reinterpret_cast<int16_t *>(value->RawPointerToValueBuffer());

                switch (dataType) {
                    case Foundation::Data::ValueType::Int32:
                        return Foundation::Data::ValueFactory::CreateInt32((int32_t) *tmp);
                    case Foundation::Data::ValueType::Int64:
                        return Foundation::Data::ValueFactory::CreateInt64((int64_t) *tmp);
                    case Foundation::Data::ValueType::UInt16:
                        return Foundation::Data::ValueFactory::CreateUInt16((uint16_t) *tmp);
                    case Foundation::Data::ValueType::UInt32:
                        return Foundation::Data::ValueFactory::CreateUInt32((uint32_t) *tmp);
                    case Foundation::Data::ValueType::UInt64:
                        return Foundation::Data::ValueFactory::CreateUInt64((uint64_t) *tmp);
                    case Foundation::Data::ValueType::Double:
                        return Foundation::Data::ValueFactory::CreateDouble((double) *tmp);
                    case Foundation::Data::ValueType::Float:
                        return Foundation::Data::ValueFactory::CreateFloat((float) *tmp);
                    case Foundation::Data::ValueType::Boolean:
                        return Foundation::Data::ValueFactory::CreateBoolean((bool) *tmp);
                    case Foundation::Data::ValueType::Byte:
                        return Foundation::Data::ValueFactory::CreateByte((char) *tmp);
                    default: {
                        std::string type{Foundation::Data::Helper::TypeHelper::CopyToString(dataType)};
                        std::string msg{"This converter can not convert Int16 into " + type};
                        throw Segmentation::Exception::TransformationException{msg};
                    }
                }
            }
        }
    }
}

