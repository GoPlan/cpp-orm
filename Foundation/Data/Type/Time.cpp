//
// Created by LE, Duc Anh on 6/24/15.
//

#include <Foundation/Data/Helper/TimeBasedHelper.h>
#include <Foundation/Data/TimeBasedValue.h>
#include "Time.h"

namespace Cloude {
    namespace Foundation {
        namespace Data {
            namespace Type {

                Time::Time()
                        : TimeBasedValue{ValueType::Time, sizeof(TimeBasedValue::TSTime)} {
                    //
                }

                Time::Time(int hour, int minute, int second, int millisecond, bool hasOffSet, int offset)
                        : TimeBasedValue{ValueType::Time, sizeof(TimeBasedValue::TSTime)} {
                    _time.hour = hour;
                    _time.minute = minute;
                    _time.second = second;
                    _time.milliSecs = millisecond;
                    _time.offset = offset;
                    _hasOffSet = hasOffSet;
                }

                void *Time::RawPointerToValueBuffer() {
                    return &_time;
                }

                std::string Time::ToString() const {
                    return Data::Helper::TimeBasedHelper::TimeToISO8601String(_time, true, _hasOffSet);
                }

                Value &Time::operator+(const Value &rhs) {
                    throw Exception::cldeNonSupportedFunctionException("operator% can not be applied to Time type");
                }

                Value &Time::operator-(const Value &rhs) {
                    throw Exception::cldeNonSupportedFunctionException("operator% can not be applied to Time type");
                }

                Value &Time::operator*(const Value &rhs) {
                    throw Exception::cldeNonSupportedFunctionException("operator% can not be applied to Time type");
                }

                Value &Time::operator/(const Value &rhs) {
                    throw Exception::cldeNonSupportedFunctionException("operator% can not be applied to Time type");
                }

                Value &Time::operator%(const Value &rhs) {
                    throw Exception::cldeNonSupportedFunctionException("operator% can not be applied to Time type");
                }
            }
        }
    }
}


