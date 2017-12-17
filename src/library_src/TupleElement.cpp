//
// Created by root on 16/12/17.
//

#include "TupleElement.h"

std::string TupleElement::get_string()
{
    return boost::get<std::string>(value);
}

int TupleElement::get_int()
{
    return boost::get<int>(value);
}