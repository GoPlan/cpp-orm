//
// Created by GoPlan on 14/05/2015.
//

#include "Column.h"

namespace Cloude {
    namespace Architecture {
        namespace Foundation {


            Column::Column(string name, string dataSourceName, DbType dbType) :
                    dataSourceName(dataSourceName),
                    name(name),
                    dbType(dbType) {
            }

            Column::~Column() {

            }
        }
    }
}
