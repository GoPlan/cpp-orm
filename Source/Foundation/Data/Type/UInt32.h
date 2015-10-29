//
// Created by LE, Duc Anh on 6/24/15.
//

#ifndef CLDEPLUS_FOUNDATION_DATA_TYPE_UINT32_H
#define CLDEPLUS_FOUNDATION_DATA_TYPE_UINT32_H

#include "../NumericValue.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Data {
            namespace Type {

                class UInt32 : public NumericValue {

                    uint32_t _value;

                public:
                    explicit UInt32(int value);
                    UInt32(const UInt32 &) = default;
                    UInt32(UInt32 &&) = default;
                    UInt32 &operator=(const UInt32 &) = default;
                    UInt32 &operator=(UInt32 &&) = default;
                    ~UInt32() = default;

                    // Value
                    virtual void *PointerToBuffer() override;

                    // IPrintable
                    virtual std::string ToString() const override;

                    // IComputable
                    virtual Value &operator+(const Value &rhs) override;
                    virtual Value &operator-(const Value &rhs) override;
                    virtual Value &operator*(const Value &rhs) override;
                    virtual Value &operator/(const Value &rhs) override;
                    virtual Value &operator%(const Value &rhs) override;
                };
            }
        }
    }
}

#endif //CLDEPLUS_FOUNDATION_DATA_TYPE_UINT32_H