#ifndef UXP1A_LINDA_TUPLE_H
#define UXP1A_LINDA_TUPLE_H

#include <vector>
#include "TupleElement.h"

class Tuple {
public:
    Tuple();
    Tuple(std::vector<std::unique_ptr<TupleElement>> elements);

    unsigned long size() const;
    const TupleElement& get(int index) const;
    std::string to_string() const;

private:
    std::vector<std::unique_ptr<TupleElement>> elements;
};


#endif //UXP1A_LINDA_TUPLE_H
