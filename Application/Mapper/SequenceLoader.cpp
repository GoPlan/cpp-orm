//
// Created by LE, Duc Anh on 5/27/15.
//

#include "SequenceLoader.h"

namespace Cloude {
    namespace Application {
        namespace Mapper {

            std::shared_ptr<Architecture::Identity> SequenceLoader::NextPrimaryKey() {
                return std::make_shared<Architecture::Identity>();
            }

            void SequenceLoader::LoadEntity(std::shared_ptr<Architecture::Identity> &identity) {

            }
        }
    }
}


