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

#include "FloatCaster.h"
#include "../../../Foundation/Data/Helper/TypeHelper.h"
#include "../../../Foundation/Data/ValueFactory.h"
#include "../../Exception/TransformationException.h"

namespace CLDEPlus {
    namespace Segmentation {

        Foundation::Data::SPtrValue Transformation::Caster::FloatCaster::Cast(
                Foundation::Data::ValueType dataType, const Foundation::Data::SPtrValue &value) const {

            if (value->getDataType() != Foundation::Data::ValueType::Float) {
                string type{Foundation::Data::Helper::TypeHelper::CopyValueTypeToString(value->getDataType())};
                string msg{type + " is not supported by FloatCaster"};
                throw Segmentation::Exception::TransformationException{msg};
            }

            float *tmp = reinterpret_cast<float *>(value->PointerToBuffer());

            switch (dataType) {
                case Foundation::Data::ValueType::Int16:
                    return Foundation::Data::ValueFactory::CreateInt16((int16_t) *tmp);
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
                    return Foundation::Data::ValueFactory::CreateFloat((double) *tmp);
                case Foundation::Data::ValueType::Boolean:
                    return Foundation::Data::ValueFactory::CreateBoolean((bool) *tmp);
                case Foundation::Data::ValueType::Byte:
                    return Foundation::Data::ValueFactory::CreateByte((char) *tmp);
                default: {
                    string type{Foundation::Data::Helper::TypeHelper::CopyValueTypeToString(dataType)};
                    string msg{"This converter can not convert float into " + type};
                    throw Segmentation::Exception::TransformationException{msg};
                }
            }
        }
    }
}