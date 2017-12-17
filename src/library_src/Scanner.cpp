//
// Created by root on 16/12/17.
//

#include "parsing/exceptions/TokenizerException.h"
#include "parsing/Scanner.h"

void Scanner::read_next()
{
    skip_whitespaces();
    if (try_eof() || try_punctuation_mark() || try_integer() || try_string_literal())
        return;
    const char first = static_cast<const char>(input.peek());
    const std::string& identifier = read_identifier();
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
        buffer += input.get();
    if (buffer.empty())
        return false;
    current_token = std::stoi(buffer);
    return true;
}

std::string Scanner::read_identifier() const
{
    std::string buffer;
    while (input && (isalnum(input.peek())))
        buffer += input.get();
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
    char first = static_cast<char>(input.peek());
    if (first != '"')
        return false;
    input.get();
    std::string buffer;
    while (input && input.peek() != '"')
        buffer += input.get();
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