//
// Created by LE, Duc Anh on 6/10/15.
//

#include "Float.h"
#include "../../Exception/CLDENonSupportedFunctionException.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Data {
            namespace Type {

                Float::Float(float value) : _value(value),
                                            NumericValue(ValueType::Float, sizeof(float)) {
                    //
                }

                void *Float::PointerToBuffer() {
                    return &_value;
                }

                std::string Float::ToString() const {
                    return std::to_string(_value);
                }

                Value &Float::operator+(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const Float &>(rhs);
                        this->_value = this->_value + cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &Float::operator-(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const Float &>(rhs);
                        this->_value = this->_value - cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &Float::operator*(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const Float &>(rhs);
                        this->_value = this->_value * cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &Float::operator/(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const Float &>(rhs);
                        this->_value = this->_value / cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &Float::operator%(const Value &rhs) {
                    throw Exception::CLDENonSupportedFunctionException("operator% can not be applied to Double type");
                }
            }
        }
    }
}

