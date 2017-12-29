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
    return std::unique_ptr<Tuple>(new Tuple(std::move(elements)));
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
    else
        throw ParseException("Parse error: " + to_string(punctuation_mark) + " expected at line "
                             + std::to_string(scanner->get_current_line()));
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
    advance();
    if (token.get_type() == Token::Type::Integer)
        return std::unique_ptr<TupleElement>(new TupleElement(token.get_integer()));
    else if (token.get_type() == Token::Type::StringLiteral)
        return std::unique_ptr<TupleElement>(new TupleElement(token.get_string_literal()));

    throw ParseException("Parse error: identifier or integer expected at line "
                         + std::to_string(scanner->get_current_line()));
}

