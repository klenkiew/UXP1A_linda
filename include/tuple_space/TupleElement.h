#ifndef UXP1A_LINDA_TUPLEELEMENT_H
#define UXP1A_LINDA_TUPLEELEMENT_H

#include <boost/variant.hpp>


class TupleElement {

public:
    enum class Type { Integer, String };

    TupleElement(const std::string& value) : value(value), type(Type::String) { }

    TupleElement(int value) : value(value), type(Type::Integer) { }

    std::string get_string() const;
    int get_int() const;
    Type get_type() const;

private:
    boost::variant<std::string, int> value;
    Type type;
};


#endif //UXP1A_LINDA_TUPLEELEMENT_H
