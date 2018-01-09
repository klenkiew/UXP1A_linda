#include <iostream>
#include "tuple_space/parsing/ast/Operator.h"
#include "tuple_space/parsing/exceptions/TokenizerException.h"
#include "tuple_space/parsing/Scanner.h"

void Scanner::read_next()
{
    skip_whitespaces();
    if (try_eof() || try_punctuation_mark() || try_integer() || try_string_literal() || try_simple_operator())
        return;
    const auto first = static_cast<const char>(input.get());
    if (try_complex_operator(first))
        return;
    const std::string& identifier = read_identifier(first);
    if (identifier.length() == 0)
        throw TokenizerException("Unknown token - " + std::string(1, first) + " -  at line: " + std::to_string(current_line));
    current_token = Token::from_identifier(identifier);
}


Token Scanner::get_token() const
{
    return current_token;
}

int Scanner::get_current_line() const
{
    return current_line;
}

bool Scanner::try_integer()
{
    std::string buffer;
    while (input && std::isdigit(input.peek()))
        buffer += static_cast<const char>(input.get());
    if (buffer.empty())
        return false;
    current_token = std::stoi(buffer);
    return true;
}

std::string Scanner::read_identifier(const char first) const
{
    std::string buffer;
    buffer += first;
    while (input && (isalnum(input.peek())))
        buffer += static_cast<const char>(input.get());
    return buffer;
}

bool Scanner::try_eof()
{
    if (!input || input.eof())
    {
        current_token = Token{};
        return true;
    }
    return false;
}

bool Scanner::try_string_literal()
{
    const auto first = static_cast<char>(input.peek());
    if (first != '"')
        return false;
    input.get();
    std::string buffer;
    while (input && input.peek() != '"')
        buffer += static_cast<const char>(input.get());
    if (input.peek() == '"')
        input.get();
    else
        throw TokenizerException("Cannot parse string literal, closing \" not found");
    current_token = Token::from_string_literal(buffer);
    return true;
}

void Scanner::skip_whitespaces()
{
    while (input && std::isspace(input.peek()))
    {
        if (input.get() == '\n')
            ++current_line;
    }
}

bool Scanner::try_punctuation_mark()
{
    const auto iterator = punctuation_marks_map.find(static_cast<const char &>(input.peek()));
    if (iterator != punctuation_marks_map.end())
    {
        current_token = iterator->second;
        input.get();
        return true;
    }
    return false;
}

bool Scanner::try_simple_operator()
{
    if (input.peek() == '-')
    {
        input.get();
        current_token = Operator::Minus;
        return true;
    }

    return false;
}

bool Scanner::try_complex_operator(const char first)
{
    switch (first)
    {
        case '=':
            if (handle_equal())
                return true;
            return false;
        case '!':
            if (handle_not_equal())
                return true;
            break;
        case '<':
            handle_less();
            return true;
        case '>':
            handle_greater();
            return true;
    }
    return false;
}

bool Scanner::handle_equal()
{
    if (input.peek() == '=')
    {
        input.get();
        current_token = Operator::Equal;
        return true;
    }
    return false;
}

bool Scanner::handle_not_equal()
{
    if (input.peek() == '=')
    {
        input.get();
        current_token = Operator::NotEqual;
        return true;
    }
    return false;
}

void Scanner::handle_less()
{
    if (input.peek() == '=')
    {
        input.get();
        current_token = Operator::LessEqual;
    }
    else
        current_token = Operator::Less;
}

void Scanner::handle_greater()
{
    if (input.peek() == '=')
    {
        input.get();
        current_token = Operator::GreaterEqual;
    }
    else
        current_token = Operator::Greater;
}









