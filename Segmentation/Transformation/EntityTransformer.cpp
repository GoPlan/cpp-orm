//
// Created by LE, Duc Anh on 6/30/15.
//

#include <stdexcept>
#include "EntityTransformer.h"

namespace Cloude {
    namespace Segmentation {

        void Transformation::EntityTransformer::Transform(
                const Foundation::SPtrEntityProxy &srcProxy,
                Foundation::SPtrEntityProxy &dstProxy) const {

            if(!srcProxy)
                throw std::invalid_argument{"srcProxy is invalid or a nullptr"};

            if(!dstProxy)
                throw std::invalid_argument{"dstProxy is invalid or a nullptr"};

            for (auto &pairCellMap : _mapCellTransformers) {
                auto &srcColumnName = pairCellMap.first;
                auto &sptrCellTransfomer = pairCellMap.second;
                Foundation::SPtrCell sptrNewCell = sptrCellTransfomer->Transform(srcProxy->getCell(srcColumnName));
                dstProxy->setCell(sptrNewCell);
            }
        }
    }
}


