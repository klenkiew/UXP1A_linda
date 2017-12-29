#ifndef UXP1A_LINDA_STRINGCOMPARERTEMPLATE_H
#define UXP1A_LINDA_STRINGCOMPARERTEMPLATE_H

#include <regex>
#include "tuple_space/TupleElement.h"
#include "RequiredTypeTemplate.h"
#include "ComparerTemplate.h"

class StringComparerTemplate: public RequiredTypeTemplate, public ComparerTemplate<std::string>
{
public:
    StringComparerTemplate(Operator operator_, std::string to_compare);
    virtual bool matches(const TupleElement& tuple_element);

private:
    std::unique_ptr<std::regex> cachedRegex = nullptr;

    bool matches_wildcard(const std::string string);
    std::string wildcard_to_regex(const std::string &wildcard_string);
    std::string escape_regex(const std::string &string);

    std::regex & getRegex();
};


#endif //UXP1A_LINDA_STRINGCOMPARERTEMPLATE_H
