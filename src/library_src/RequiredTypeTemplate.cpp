//
// Created by root on 18/12/17.
//

#include "parsing/RequiredTypeTemplate.h"

bool RequiredTypeTemplate::matches(const TupleElement &tuple_element)
{
    return tuple_element.get_type() == required_type;
}

RequiredTypeTemplate::RequiredTypeTemplate(TupleElement::Type required_type) : required_type(required_type)
{ }