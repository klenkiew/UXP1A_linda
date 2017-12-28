#include <regex>
#include <boost/algorithm/string/replace.hpp>
#include "tuple_space/StringComparerTemplate.h"

StringComparerTemplate::StringComparerTemplate(Operator operator_, std::string to_compare) :
        RequiredTypeTemplate(TupleElement::Type::String), ComparerTemplate(operator_, to_compare)
{

}

bool StringComparerTemplate::matches(const TupleElement &tuple_element)
{
    if (!RequiredTypeTemplate::matches(tuple_element))
        return false;

    if (ComparerTemplate::operator_ != Operator::Equal)
        return ComparerTemplate::matches(tuple_element.get_string());

    return matches_wildcard(tuple_element.get_string());
}

bool StringComparerTemplate::matches_wildcard(const std::string string)
{
    const std::regex regex = std::regex(wildcard_to_regex(to_compare));
    return std::regex_match(string, regex);
}

std::string StringComparerTemplate::wildcard_to_regex(const std::string &wildcard_string)
{
    std::string escaped = escape_regex(wildcard_string);
    boost::replace_all(escaped, "\\*", "*");
    return "^" + escaped + "$";
}

std::string StringComparerTemplate::escape_regex(const std::string &string)
{
    // matches any characters that need to be escaped in regex
    const std::regex specialChars { R"([-[\]{}()*+?.,\^$|#\s])" };

    const std::string sanitized = std::regex_replace(string, specialChars, R"(\$&)" );
    return sanitized;
}









