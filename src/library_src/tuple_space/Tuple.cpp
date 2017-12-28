#include "tuple_space/Tuple.h"

Tuple::Tuple()
{ }

Tuple::Tuple(std::vector<std::unique_ptr<TupleElement>> elements) : elements(std::move(elements))
{ }

void Tuple::add_element(std::unique_ptr<TupleElement> element)
{
    elements.push_back(std::move(element));
}

unsigned long Tuple::size() const
{
    return elements.size();
}

const TupleElement &Tuple::get(int index) const
{
    return *elements[index].get();
}