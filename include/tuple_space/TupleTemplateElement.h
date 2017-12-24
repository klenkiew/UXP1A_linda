//
// Created by root on 18/12/17.
//

#ifndef UXP1A_LINDA_TUPLETEMPLATEELEMENT_H
#define UXP1A_LINDA_TUPLETEMPLATEELEMENT_H


#include "tuple_space/TupleElement.h"

class TupleTemplateElement
{
public:
    virtual bool matches(const TupleElement &tupleElement) = 0;
};


#endif //UXP1A_LINDA_TUPLETEMPLATEELEMENT_H
