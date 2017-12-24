//
// Created by root on 24/12/17.
//

#include "tuple_space/TupleTemplate.h"

bool TupleTemplate::matches(const Tuple& tuple) const
{
    unsigned long tuple_size = tuple.size();
    if (tuple_size != elements.size())
        return false;

    for (int i = 0; i < tuple_size; ++i)
    {
        if (!elements[i]->matches(tuple.get(i)))
            return false;
    }

    return true;
}