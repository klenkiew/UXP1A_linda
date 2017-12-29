#ifndef UXP1A_LINDA_TOKENIZEREXCEPTION_H
#define UXP1A_LINDA_TOKENIZEREXCEPTION_H

#include <stdexcept>

class TokenizerException : public std::runtime_error
{
public:
	explicit TokenizerException(const std::string& _Message)
		: runtime_error(_Message)
	{
	}

	explicit TokenizerException(const char* const _Message)
		: runtime_error(_Message)
	{
	}
};

#endif //UXP1A_LINDA_TOKENIZEREXCEPTION_H
