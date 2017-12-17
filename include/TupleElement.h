//
// Created by root on 16/12/17.
//

#ifndef UXP1A_LINDA_TUPLEELEMENT_H
#define UXP1A_LINDA_TUPLEELEMENT_H

#include <boost/variant.hpp>


class TupleElement {
private:
    boost::variant<std::string, int> value;

public:
    TupleElement(const std::string& value) : value(value) { }

    TupleElement(int value) : value(value) { }

    std::string get_string();
    int get_int();
};


#endif //UXP1A_LINDA_TUPLEELEMENT_H
