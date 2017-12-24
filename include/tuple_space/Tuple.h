//
// Created by root on 16/12/17.
//

#ifndef UXP1A_LINDA_TUPLE_H
#define UXP1A_LINDA_TUPLE_H

#include "TupleElement.h"

class Tuple {
private:
    std::vector<std::unique_ptr<TupleElement>> elements;

public:
    Tuple()
    { }

    Tuple(std::vector<std::unique_ptr<TupleElement>> elements) : elements(std::move(elements))
    { }

    void add_element(std::unique_ptr<TupleElement> element)
    {
        elements.push_back(std::move(element));
    }

    TupleElement get(int index)
    {
        return *elements[index].get();
    }
};


#endif //UXP1A_LINDA_TUPLE_H
