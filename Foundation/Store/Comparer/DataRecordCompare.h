//
// Created by LE, Duc Anh on 6/22/15.
//

#ifndef CLOUD_E_PLUS_FOUNDATION_STORE_COMPARER_DATARECORDCOMPARE_H
#define CLOUD_E_PLUS_FOUNDATION_STORE_COMPARER_DATARECORDCOMPARE_H

#include <Foundation/EntityProxy.h>
#include <Foundation/Exception/cldeSegmentationException.h>
#include "DataRecordLess.h"

namespace Cloude {
    namespace Foundation {
        namespace Store {
            namespace Comparer {

                template<class T = Comparer::DataRecordLess>
                class DataRecordCompare
                        : public std::binary_function<Store::AbstractionEntity, Store::AbstractionEntity, bool> {

                    T _compare;

                public:
                    DataRecordCompare(const SPtrColumnVector &lhsComparingColumns,
                                      const SPtrColumnVector &rhsComparingColumns)
                            : _compare{lhsComparingColumns,
                                       rhsComparingColumns} { };

                    bool operator()(const Store::AbstractionEntity &lhs, const Store::AbstractionEntity &rhs) const {
                        return !_compare(lhs, rhs) && !_compare(rhs, lhs);
                    };
                };
            }
        }
    }
}

#endif //CLOUD_E_PLUS_FOUNDATION_STORE_COMPARER_DATARECORDCOMPARE_H
