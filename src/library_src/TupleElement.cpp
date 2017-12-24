//
// Created by root on 16/12/17.
//

#include "TupleElement.h"

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

