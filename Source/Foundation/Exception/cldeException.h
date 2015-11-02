//
// Created by LE, Duc Anh on 6/10/15.
//

#ifndef CLOUD_E_CPLUS_FOUNDATION_EXCEPTION_CLDEEXCEPTION_H
#define CLOUD_E_CPLUS_FOUNDATION_EXCEPTION_CLDEEXCEPTION_H

#include "../../Port/Definitions.h"

namespace CLDEPlus {
    namespace Foundation {
        namespace Exception {

            class CLDEException : public exception {

                mutable string _message;

            public:
                explicit CLDEException(string const &message) : _message{message} { };
                explicit CLDEException(char const *message) : _message(message) { };
                CLDEException(CLDEException const &) = default;
                CLDEException(CLDEException &&) = default;
                CLDEException &operator=(CLDEException const &) = default;
                CLDEException &operator=(CLDEException &&) = default;
                virtual ~CLDEException() = default;

                // Exception
                virtual const char *what() const noexcept override {

                    string tmp{std::move(_message)};

                    _message.reserve(Name().length() + tmp.length() + 3);
                    _message += "[" + Name() + "]";
                    _message += tmp;

                    return _message.c_str();
                };

                // Locals
                virtual const string &Name() const noexcept = 0;
            };
        }
    }
}


#endif //CLOUD_E_CPLUS_FOUNDATION_EXCEPTION_CLDEEXCEPTION_H
