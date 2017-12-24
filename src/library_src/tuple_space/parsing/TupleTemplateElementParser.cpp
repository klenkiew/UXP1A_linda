//
// Created by root on 18/12/17.
//

#include "tuple_space/IntegerComparerTemplate.h"
#include "tuple_space/StringComparerTemplate.h"
#include "tuple_space/parsing/exceptions/ParseException.h"
#include "tuple_space/parsing/TupleTemplateElementParser.h"

std::unique_ptr<TupleTemplateElement> TupleTemplateElementParser::parse()
{
    advance();
    TupleElement::Type required_type = read_type();

    skip(PunctuationMark::Colon);
    const Token &token = scanner->get_token();
    if (is(PunctuationMark::Star))
    {
        skip(PunctuationMark::Star);
        return std::unique_ptr<RequiredTypeTemplate>(new RequiredTypeTemplate(required_type));
    }
    if (is_operator())
        return parse_comparer_template(required_type);

    return parse_equal_template(required_type);
}

TupleElement::Type TupleTemplateElementParser::read_type() const
{
    const Token& token = scanner->get_token();
    if (token.get_type() != Token::Type::Identifier)
        throw ParseException("Identifier expected");

    const std::string identifier = token.get_identifier();
    if (identifier != "string" && identifier != "integer")
        throw ParseException("Identifier must be integer or string");

    advance();
    return identifier == "string" ? TupleElement::Type::String : TupleElement::Type::Integer;
}


void TupleTemplateElementParser::advance() const
{
    scanner->read_next();
}

bool TupleTemplateElementParser::is(PunctuationMark punctuation_mark) const
{
    const auto token = scanner->get_token();
    return token.get_type() == Token::Type::PunctuationMark && token.get_punctuation_mark() == punctuation_mark;
}

void TupleTemplateElementParser::skip(PunctuationMark punctuation_mark) const
{
    if (is(punctuation_mark))
        advance();
    else
        throw ParseException("Parse error: " + to_string(punctuation_mark) + " expected at line "
                             + std::to_string(scanner->get_current_line()));
}

bool TupleTemplateElementParser::try_skip_comma() const
{
    if (is(PunctuationMark::Comma))
    {
        skip(PunctuationMark::Comma);
        return true;
    }
    return false;
}

bool TupleTemplateElementParser::is_operator()
{
    return scanner->get_token().get_type() == Token::Type::Operator;
}

std::unique_ptr<TupleTemplateElement> TupleTemplateElementParser::parse_comparer_template(TupleElement::Type type)
{
    const Operator operator_ = scanner->get_token().get_operator();
    advance();
    if (type == TupleElement::Type::String)
        return parse_string_comparer_template(operator_);

    if (type == TupleElement::Type::Integer)
        return parse_integer_comparer_template(operator_);

    throw std::invalid_argument("type");
}

std::unique_ptr<TupleTemplateElement> TupleTemplateElementParser::parse_integer_comparer_template(
        const Operator &operator_) const
{
    if (scanner->get_token().get_type() != Token::Type::Integer)
        throw ParseException("Parse error: integer expected at line "
                             + std::__cxx11::to_string(scanner->get_current_line()));

    const int to_compare = scanner->get_token().get_integer();
    advance();
    return std::unique_ptr<IntegerComparerTemplate>(new IntegerComparerTemplate(operator_, to_compare));
}

std::unique_ptr<TupleTemplateElement> TupleTemplateElementParser::parse_string_comparer_template(
        const Operator &operator_) const
{
    if (scanner->get_token().get_type() != Token::Type::StringLiteral)
        throw ParseException("Parse error: string literal expected at line "
                             + std::to_string(scanner->get_current_line()));

    const std::string to_compare = scanner->get_token().get_string_literal();
    advance();
    return std::unique_ptr<StringComparerTemplate>(new StringComparerTemplate(operator_, to_compare));
}

std::unique_ptr<TupleTemplateElement> TupleTemplateElementParser::parse_equal_template(TupleElement::Type type)
{
    if (type == TupleElement::Type::Integer)
        return parse_integer_comparer_template(Operator::Equal);

    if (type == TupleElement::Type::String)
        return parse_string_comparer_template(Operator::Equal);

    throw std::invalid_argument("type");
}