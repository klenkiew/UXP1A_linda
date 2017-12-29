#ifndef UXP1A_LINDA_NAMEDPIPETIMEOUTEXCEPTION_H
#define UXP1A_LINDA_NAMEDPIPETIMEOUTEXCEPTION_H


#include "NamedPipeException.h"

class NamedPipeTimeoutException: public NamedPipeException
{
public:
    explicit NamedPipeTimeoutException(const std::string& _Message)
            : NamedPipeException(_Message)
    {
    }

    explicit NamedPipeTimeoutException(const char *const _Message)
            : NamedPipeException(_Message)
    {

    }
};

#endif //UXP1A_LINDA_NAMEDPIPETIMEOUTEXCEPTION_H
