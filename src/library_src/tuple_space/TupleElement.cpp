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

