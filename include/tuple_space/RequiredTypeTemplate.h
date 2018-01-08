#ifndef UXP1A_LINDA_REQUIREDTYPETEMPLATE_H
#define UXP1A_LINDA_REQUIREDTYPETEMPLATE_H


#include "TupleTemplateElement.h"

class RequiredTypeTemplate: public TupleTemplateElement
{
public:
    RequiredTypeTemplate(TupleElement::Type required_type);

    virtual bool matches(const TupleElement &tuple_element);
    std::string to_string() const override;

protected:
    TupleElement::Type required_type;
};


#endif //UXP1A_LINDA_REQUIREDTYPETEMPLATE_H
