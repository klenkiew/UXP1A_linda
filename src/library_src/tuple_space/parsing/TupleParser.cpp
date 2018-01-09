#include <iostream>
#include "tuple_space/parsing/TupleParser.h"
#include "tuple_space/parsing/exceptions/ParseException.h"

std::unique_ptr<Tuple> TupleParser::parse()
{
    advance();
    skip(PunctuationMark::LeftParenthesis);
    std::vector<std::unique_ptr<TupleElement>> elements;
    do
    {
        elements.push_back(parse_tuple_element());
    } while (try_skip_comma());
    skip(PunctuationMark::RightParenthesis);
    return std::make_unique<Tuple>(std::move(elements));
}


void TupleParser::advance() const
{
    scanner->read_next();
}

bool TupleParser::is(PunctuationMark punctuation_mark) const
{
    const auto token = scanner->get_token();
    return token.get_type() == Token::Type::PunctuationMark && token.get_punctuation_mark() == punctuation_mark;
}

void TupleParser::skip(PunctuationMark punctuation_mark) const
{
    if (is(punctuation_mark))
        advance();
    else if (is_eof())
        throw EndOfFile("End of file at line: " + std::to_string(scanner->get_current_line()));
    else
        throw ParseException("Parse error: " + to_string(punctuation_mark) + " expected at line "
                             + std::to_string(scanner->get_current_line()));
}

bool TupleParser::is_eof() const
{
    const auto token = scanner->get_token();
    return token.get_type() == Token::Type::Eof;
}

bool TupleParser::try_skip_comma() const
{
    if (is(PunctuationMark::Comma))
    {
        skip(PunctuationMark::Comma);
        return true;
    }
    return false;
}

std::unique_ptr<TupleElement> TupleParser::parse_tuple_element()
{
    const Token& token = scanner->get_token();
    std::unique_ptr<TupleElement> tuple_element;

    if (token.get_type() == Token::Type::Integer 
        || token.get_type() == Token::Type::Operator && token.get_operator() == Operator::Minus)
    {
        tuple_element = std::make_unique<TupleElement>(parse_integer());
    }
    else if (token.get_type() == Token::Type::StringLiteral)
        tuple_element = std::make_unique<TupleElement>(token.get_string_literal());
    else
        throw ParseException("Parse error: identifier or integer expected at line "
                             + std::to_string(scanner->get_current_line()));
    advance();
    return tuple_element;
}

int TupleParser::parse_integer() const
{
    const Token &token = scanner->get_token();
    if (token.get_type() == Token::Type::Integer)
        return token.get_integer();

    if (token.get_type() == Token::Type::Operator && token.get_operator() == Operator::Minus)
    {
        advance();
        return -parse_integer();
    }

    // happens only if there's a '-' character without any digits after
    throw ParseException("Parse error: a digit expected after '-' at line "
                         + std::to_string(scanner->get_current_line()));
}

