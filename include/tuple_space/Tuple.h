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

    unsigned long size() const
    {
        return elements.size();
    }

    const TupleElement& get(int index) const
    {
        return *elements[index].get();
    }
};


#endif //UXP1A_LINDA_TUPLE_H
