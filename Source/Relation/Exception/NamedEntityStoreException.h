//
// Created by LE, Duc Anh on 7/9/15.
//

#ifndef CLDEPLUS_RELATION_EXCEPTION_NAMEDENTITYSTOREEXCEPTION_H
#define CLDEPLUS_RELATION_EXCEPTION_NAMEDENTITYSTOREEXCEPTION_H

#include "../../Foundation/Exception/CLDEException.h"

namespace CLDEPlus {
    namespace Relation {
        namespace Exception {

            class NamedEntityStoreException : Foundation::Exception::CLDEException {

                static std::string _name;

            public:
                NamedEntityStoreException(const std::string &message) : Foundation::Exception::CLDEException{message} { };
                NamedEntityStoreException(const NamedEntityStoreException &) = default;
                NamedEntityStoreException(NamedEntityStoreException &&) = default;
                NamedEntityStoreException &operator=(const NamedEntityStoreException &) = default;
                NamedEntityStoreException &operator=(NamedEntityStoreException &&) = default;
                virtual ~NamedEntityStoreException() = default;


            private:
                virtual const std::string &Name() const noexcept override;
            };
        }
    }
}


#endif //CLDEPLUS_RELATION_EXCEPTION_NAMEDENTITYSTOREEXCEPTION_H