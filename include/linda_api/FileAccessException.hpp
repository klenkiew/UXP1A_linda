#ifndef UXP1A_LINDA_SYSTEMCALLEXCEPTION_HPP
#define UXP1A_LINDA_SYSTEMCALLEXCEPTION_HPP

#include <stdexcept>

class FileAccessException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};


#endif //UXP1A_LINDA_SYSTEMCALLEXCEPTION_HPP
