#include "tuple_space/TupleElement.h"

std::string TupleElement::get_string() const
{
    return boost::get<std::string>(value);
}

int TupleElement::get_int() const
{
    return boost::get<int>(value);
}

TupleElement::Type TupleElement::get_type() const
{
    return type;
}

std::string TupleElement::to_string() const
{
    return type == TupleElement::Type::String ? get_string() : std::to_string(get_int());
}

std::string to_string(TupleElement::Type type)
{
    return type == TupleElement::Type::String ? "string" : "integer";
}
