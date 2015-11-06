//
// Created by LE, Duc Anh on 6/10/15.
//

#include "Double.h"
#include "../../Exception/CLDENonSupportedFunctionException.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Data {
            namespace Type {

                Double::Double(double value) : _value(value),
                                               NumericValue(ValueType::Double, sizeof(double)) {
                    //
                }

                void *Double::PointerToBuffer() {
                    return &_value;
                }

                string Double::ToString() const {
                    return std::to_string(_value);
                }

                Value &Double::operator+(const Value &rhs) {
                    auto cast = dynamic_cast<const Double &>(rhs);
                    this->_value = this->_value + cast._value;
                    return *this;
                }

                Value &Double::operator-(const Value &rhs) {
                    auto cast = dynamic_cast<const Double &>(rhs);
                    this->_value = this->_value - cast._value;
                    return *this;
                }

                Value &Double::operator*(const Value &rhs) {
                    auto cast = dynamic_cast<const Double &>(rhs);
                    this->_value = this->_value * cast._value;
                    return *this;
                }

                Value &Double::operator/(const Value &rhs) {
                    auto cast = dynamic_cast<const Double &>(rhs);
                    this->_value = this->_value / cast._value;
                    return *this;
                }

                Value &Double::operator%(const Value &rhs) {
                    throw Exception::CLDENonSupportedFunctionException("operator% can not be applied to Double type");
                }
            }
        }
    }
}


