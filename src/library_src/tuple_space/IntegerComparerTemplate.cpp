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

std::string IntegerComparerTemplate::to_string() const
{
    return ::to_string(RequiredTypeTemplate::required_type) + ":" +
           to_string_skip_equal(operator_) + std::to_string(to_compare);
}
