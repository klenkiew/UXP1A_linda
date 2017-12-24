//
// Created by root on 18/12/17.
//

#include "parsing/ast/Operator.h"
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

    const std::unordered_map<Operator, std::string, EnumClassHash> operatorStrings
    {
        {Operator::Equal,        "=="},
        {Operator::NotEqual,     "!="},
        {Operator::Less,         "<"},
        {Operator::LessEqual,    "<="},
        {Operator::Greater,      ">"},
        {Operator::GreaterEqual, ">="},
    };
}

std::string toString(Operator operator_)
{
    return (operatorStrings.find(operator_)->second);
}
