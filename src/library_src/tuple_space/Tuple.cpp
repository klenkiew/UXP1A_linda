#include "tuple_space/Tuple.h"
#include <sstream>

Tuple::Tuple()
{ }

Tuple::Tuple(std::vector<std::unique_ptr<TupleElement>> elements) : elements(std::move(elements))
{ }

unsigned long Tuple::size() const
{
    return elements.size();
}

const TupleElement &Tuple::get(int index) const
{
    return *elements[index];
}

std::string Tuple::to_string() const
{
    std::ostringstream result;
    result << "(";
    for (int i = 0; i < elements.size(); ++i)
        result << (i ? ", " : "") << elements[i]->to_string();
    result << ")";
    return result.str();
}
