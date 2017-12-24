//
// Created by root on 18/12/17.
//

#ifndef UXP1A_LINDA_REQUIREDTYPETEMPLATE_H
#define UXP1A_LINDA_REQUIREDTYPETEMPLATE_H


#include "TupleTemplateElement.h"

class RequiredTypeTemplate: public TupleTemplateElement
{
public:
    RequiredTypeTemplate(TupleElement::Type required_type);
    virtual bool matches(const TupleElement &tuple_element);

private:
    TupleElement::Type required_type;
};


#endif //UXP1A_LINDA_REQUIREDTYPETEMPLATE_H
