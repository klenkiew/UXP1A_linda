#ifndef UXP1A_LINDA_PUNCTUATOR_H
#define UXP1A_LINDA_PUNCTUATOR_H

#include <string>

enum class PunctuationMark
{
    LeftParenthesis,
    RightParenthesis,
    Comma,
    Star,
    Colon
};

std::string to_string(PunctuationMark punctuation_mark);

#endif //UXP1A_LINDA_PUNCTUATOR_H
