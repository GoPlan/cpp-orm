//
// Created by LE, Duc Anh on 6/10/15.
//

#include "String.h"
#include "../../Exception/CLDENonSupportedFunctionException.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Data {
            namespace Type {

                String::String(const std::string &string)
                        : _value(string), CharacterValue(ValueType::String, string.length() + 1) {
                    //
                }

                String::String(unsigned long size)
                        : CharacterValue{ValueType::String, size} {
                    _value.reserve(size);
                }

                void *String::PointerToBuffer() {
                    return &_value;
                }

                size_t String::getActualSize() {
                    return _value.length() + 1;
                }

                std::string String::ToString() const {
                    return _value;
                }
            }
        }
    }
}

