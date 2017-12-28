#ifndef UXP1A_LINDA_NAMEDPIPEEXCEPTION_H
#define UXP1A_LINDA_NAMEDPIPEEXCEPTION_H


#include <stdexcept>

class NamedPipeException: public std::runtime_error
{
public:
    explicit NamedPipeException(const std::string& _Message)
            : runtime_error(_Message)
    {
    }

    explicit NamedPipeException(const char* const _Message)
            : runtime_error(_Message)
    {
    }
};

#endif //UXP1A_LINDA_NAMEDPIPEEXCEPTION_H
