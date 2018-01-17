#ifndef UXP1A_LINDA_SYSTEMCALLEXCEPTION_HPP
#define UXP1A_LINDA_SYSTEMCALLEXCEPTION_HPP

#include <stdexcept>

class FileAccessException : std::runtime_error {
public:
    FileAccessException(const std::string &name)
            : std::runtime_error(name) { }
};


#endif //UXP1A_LINDA_SYSTEMCALLEXCEPTION_HPP
