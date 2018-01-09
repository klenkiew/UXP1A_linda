#ifndef UXP1A_LINDA_TUPLETEMPLATEELEMENT_H
#define UXP1A_LINDA_TUPLETEMPLATEELEMENT_H


#include "tuple_space/TupleElement.h"

class TupleTemplateElement
{
public:
    virtual bool matches(const TupleElement &tupleElement) = 0;
    virtual std::string to_string() const = 0;
};


#endif //UXP1A_LINDA_TUPLETEMPLATEELEMENT_H
