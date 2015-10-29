//
// Created by LE, Duc Anh on 7/17/15.
//

#ifndef CLDEPLUS_FOUNDATION_COMMON_ICOMPARABLE_H
#define CLDEPLUS_FOUNDATION_COMMON_ICOMPARABLE_H

namespace CLDEPlus {
    namespace Foundation {
        namespace Common {

            class IComparable {

            public:
                virtual bool LessThan(const IComparable &target) const = 0;
                virtual bool GreaterThan(const IComparable &target) const = 0;
                virtual bool EquivalentTo(const IComparable &target) const = 0;
            };
        }
    }
}

#endif //CLDEPLUS_FOUNDATION_COMMON_ICOMPARABLE_H