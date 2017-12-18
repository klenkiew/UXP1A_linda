//
// Created by root on 16/12/17.
//

#ifndef UXP1A_LINDA_TOKEN_H
#define UXP1A_LINDA_TOKEN_H

#include <string>
#include "parsing/ast/PunctuationMark.h"
#include <boost/variant.hpp>

class Token
{
public:
    enum class Type
    {
        Eof,
        Integer,
        // string which is surrounded by " "
        StringLiteral,
        // just a string
        Identifier,
        Operator,
        PunctuationMark
    };

    Token();
    Token(PunctuationMark p);
    Token(int number);

    static Token from_string_literal(std::string literal);
    static Token from_identifier(std::string identifier);

    std::string get_string_literal() const;
    std::string get_identifier() const;
    int get_integer() const;
    PunctuationMark get_punctuation_mark() const;

    Type get_type() const;

private:
    Type type;
    boost::variant<
            int,
            std::string,
            ::PunctuationMark
    > value;

    Token(std::string string, bool string_literal);
};


#endif //UXP1A_LINDA_TOKEN_H
