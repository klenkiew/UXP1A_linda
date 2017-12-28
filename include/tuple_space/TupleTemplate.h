#ifndef UXP1A_LINDA_TUPLETEMPLATE_H
#define UXP1A_LINDA_TUPLETEMPLATE_H


#include "TupleTemplateElement.h"
#include "Tuple.h"

class TupleTemplate
{
public:
    TupleTemplate(std::vector<std::unique_ptr<TupleTemplateElement>> elements) : elements(std::move(elements))
    { }

    bool matches(const Tuple& tuple) const;

private:
    std::vector<std::unique_ptr<TupleTemplateElement>> elements;
};


#endif //UXP1A_LINDA_TUPLETEMPLATE_H
