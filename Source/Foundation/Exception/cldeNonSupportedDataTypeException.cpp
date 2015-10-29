//
// Created by LE, Duc Anh on 5/26/15.
//

#include "CLDENonSupportedDataTypeException.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Exception {

            const std::string CLDENonSupportedDataTypeException::_name{"CLDENonSupportedDataTypeException"};

            const std::string &CLDENonSupportedDataTypeException::Name() const noexcept {
                return _name;
            }

        }
    }
}

