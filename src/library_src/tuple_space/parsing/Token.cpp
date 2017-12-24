//
// Created by root on 16/12/17.
//

#include "tuple_space/parsing/Token.h"

Token::Token(): type(Type::Eof)
{
}

Token::Token(::PunctuationMark p): type(Type::PunctuationMark)
{
    value = p;
}


Token::Token(::Operator o): type(Type::Operator)
{
    value = o;
}

Token::Token(std::string string, bool string_literal)
{
    type = string_literal ? Type::StringLiteral : Type::Identifier;
    value = string;
}

Token::Token(int number): type(Type::Integer)
{
    value = number;
}

Token Token::from_string_literal(std::string literal)
{
    return Token(literal, true);
}

Token Token::from_identifier(std::string identifier)
{
    return Token(identifier, false);
}

Token::Type Token::get_type() const
{
    return type;
}

std::string Token::get_string_literal() const
{
    return boost::get<std::string>(value);
}

std::string Token::get_identifier() const
{
    return boost::get<std::string>(value);
}

int Token::get_integer() const
{
    return boost::get<int>(value);
}

PunctuationMark Token::get_punctuation_mark() const
{
    return boost::get<::PunctuationMark>(value);
}

Operator Token::get_operator() const
{
    return boost::get<::Operator>(value);
}


