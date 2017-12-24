//
// Created by root on 18/12/17.
//

#ifndef UXP1A_LINDA_INTEGERCOMPARERTEMPLATE_H
#define UXP1A_LINDA_INTEGERCOMPARERTEMPLATE_H

#include "tuple_space/parsing/ast/Operator.h"
#include "TupleTemplateElement.h"
#include "RequiredTypeTemplate.h"
#include "ComparerTemplate.h"

class IntegerComparerTemplate: public RequiredTypeTemplate, public ComparerTemplate<int>
{
public:
    IntegerComparerTemplate(Operator operator_, int to_compare);
    virtual bool matches(const TupleElement& tuple_element);

private:
    Operator operator_;
    int to_compare;
};


#endif //UXP1A_LINDA_COMPARERTEMPLATE_H
