//
// Created by LE, Duc Anh on 6/10/15.
//

#include "cldeCharacterValue.h"

namespace Cloude {
    namespace Foundation {
        namespace Type {

            cldeCharacterValue::cldeCharacterValue(cldeValueType dataType, size_t length) : cldeValue(dataType,
                                                                                                      length) {
                //
            }

            cldeValueCategory cldeCharacterValue::getCategory() {
                return cldeValueCategory::CharacterBased;
            }

            bool cldeCharacterValue::isNumeric() {
                return false;
            }

            cldeValue &cldeCharacterValue::operator+(const cldeValue &rhs) {
                throw Exception::cldeNonSupportedFunctionException("operator+ can not be applied to Double type");
            }

            cldeValue &cldeCharacterValue::operator-(const cldeValue &rhs) {
                throw Exception::cldeNonSupportedFunctionException("operator- can not be applied to Double type");
            }

            cldeValue &cldeCharacterValue::operator*(const cldeValue &rhs) {
                throw Exception::cldeNonSupportedFunctionException("operator* can not be applied to Double type");
            }

            cldeValue &cldeCharacterValue::operator/(const cldeValue &rhs) {
                throw Exception::cldeNonSupportedFunctionException("operator/ can not be applied to Double type");
            }

            cldeValue &cldeCharacterValue::operator%(const cldeValue &rhs) {
                throw Exception::cldeNonSupportedFunctionException("operator% can not be applied to Double type");
            }
        }
    }
}


