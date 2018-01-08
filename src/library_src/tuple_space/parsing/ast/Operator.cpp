#include "tuple_space/parsing/ast/Operator.h"
#include <unordered_map>

namespace
{
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

    const std::unordered_map<Operator, std::string, EnumClassHash> operator_strings
    {
        {Operator::Equal,        "=="},
        {Operator::NotEqual,     "!="},
        {Operator::Less,         "<"},
        {Operator::LessEqual,    "<="},
        {Operator::Greater,      ">"},
        {Operator::GreaterEqual, ">="},
    };
}

std::string to_string(Operator operator_)
{
    return (operator_strings.find(operator_)->second);
}

std::string to_string_skip_equal(Operator operator_)
{
    // equal operator has two representations - "==" and an empty string (e.g integer:==5 or integer:5)
    // we choose the latter representation to print tuple templates so we skip the operator if it's '=='
    return operator_ == Operator::Equal ? "" : ::to_string(operator_);
}
