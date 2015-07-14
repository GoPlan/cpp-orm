//
// Created by LE, Duc Anh on 5/29/15.
//

#include "MySqlSourceException.h"

namespace Cloude {
    namespace Drivers {
        namespace MySql {

            MySqlSourceException::MySqlSourceException(const std::string &message)
                    : cldeException{message} {
                //
            }

            const std::string &Drivers::MySql::MySqlSourceException::Name() const noexcept {
                return _name;
            }
        }
    }
}


