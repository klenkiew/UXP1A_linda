#include "tuple_space/IntegerComparerTemplate.h"

IntegerComparerTemplate::IntegerComparerTemplate(Operator operator_, int to_compare) :
        RequiredTypeTemplate(TupleElement::Type::Integer),
        ComparerTemplate(operator_, to_compare)
{ }

bool IntegerComparerTemplate::matches(const TupleElement& tuple_element)
{
    if (!RequiredTypeTemplate::matches(tuple_element))
        return false;

    return ComparerTemplate::matches(tuple_element.get_int());
}