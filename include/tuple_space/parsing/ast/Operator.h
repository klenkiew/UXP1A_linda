#ifndef UXP1A_LINDA_OPERATOR_H
#define UXP1A_LINDA_OPERATOR_H

#include <string>

enum class Operator
{
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Minus,
};

std::string to_string(Operator operator_);
std::string to_string_skip_equal(Operator operator_);

#endif
