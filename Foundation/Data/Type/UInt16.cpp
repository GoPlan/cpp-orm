//
// Created by LE, Duc Anh on 6/24/15.
//

#include "UInt16.h"

namespace Cloude {
    namespace Foundation {
        namespace Data {
            namespace Type {

                UInt16::UInt16(uint16_t value) : _value(value),
                                                 NumericValue(ValueType::UInt16, sizeof(int16_t)) {
                    //
                }

                void *UInt16::RawPointerToValueBuffer() {
                    return &this->_value;
                }

                bool UInt16::Equal(const Common::IEquatable &rhs) const {
                    try {
                        auto cast = dynamic_cast<const UInt16 &>(rhs);
                        return (cast._value == this->_value);
                    } catch (std::bad_cast &ex) {
                        return false;
                    }
                }

                std::string UInt16::ToString() const {
                    return std::to_string(_value);
                }

                Value &UInt16::operator+(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const UInt16 &>(rhs);
                        this->_value = this->_value + cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &UInt16::operator-(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const UInt16 &>(rhs);
                        this->_value = this->_value - cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &UInt16::operator*(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const UInt16 &>(rhs);
                        this->_value = this->_value * cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &UInt16::operator/(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const UInt16 &>(rhs);
                        this->_value = this->_value / cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }

                Value &UInt16::operator%(const Value &rhs) {
                    try {
                        auto cast = dynamic_cast<const UInt16 &>(rhs);
                        this->_value = this->_value % cast._value;
                        return *this;
                    } catch (std::bad_cast &ex) {
                        throw;
                    }
                }
            }
        }
    }
}





