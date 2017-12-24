//
// Created by root on 18/12/17.
//

#ifndef UXP1A_LINDA_COMPARERTEMPLATE_H
#define UXP1A_LINDA_COMPARERTEMPLATE_H


#include <TupleElement.h>
#include <iostream>
#include "parsing/RequiredTypeTemplate.h"
#include "parsing/ast/Operator.h"
#include "TupleTemplateElement.h"
#include "RequiredTypeTemplate.h"

template <typename T>
class ComparerTemplate
{
public:
    ComparerTemplate(Operator operator_, T to_compare)
            : operator_(operator_), to_compare(std::move(to_compare))
    { }

    bool virtual matches(const T& element)
    {
        T value = element;
        switch (operator_)
        {
            case Operator::Equal:
                return value == to_compare;
            case Operator::NotEqual:
                return value != to_compare;
            case Operator::Less:
                return value < to_compare;
            case Operator::LessEqual:
                return value <= to_compare;
            case Operator::Greater:
                return value > to_compare;
            case Operator::GreaterEqual:
                return value >= to_compare;
        }
    }

protected:
    Operator operator_;
    T to_compare;
};


#endif //UXP1A_LINDA_COMPARERTEMPLATE_H
