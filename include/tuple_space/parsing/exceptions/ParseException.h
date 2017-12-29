

#ifndef UXP1A_LINDA_PARSEREXCEPTION_H
#define UXP1A_LINDA_PARSEREXCEPTION_H

#include <stdexcept>

class ParseException : public std::runtime_error
{
public:
	explicit ParseException(const std::string& _Message)
		: runtime_error(_Message)
	{
	}

	explicit ParseException(const char* const _Message)
		: runtime_error(_Message)
	{
	}
};

#endif //UXP1A_LINDA_PARSEREXCEPTION_H
