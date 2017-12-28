#ifndef UXP1A_LINDA_TUPLE_H
#define UXP1A_LINDA_TUPLE_H

#include "TupleElement.h"

class Tuple {
public:
    Tuple();
    Tuple(std::vector<std::unique_ptr<TupleElement>> elements);

    void add_element(std::unique_ptr<TupleElement> element);
    unsigned long size() const;
    const TupleElement& get(int index) const;

private:
    std::vector<std::unique_ptr<TupleElement>> elements;
};


#endif //UXP1A_LINDA_TUPLE_H
