//
// Created by root on 16/12/17.
//

#ifndef UXP1A_LINDA_PUNCTUATOR_H
#define UXP1A_LINDA_PUNCTUATOR_H

#include <string>

enum class PunctuationMark
{
    LeftParenthesis,
    RightParenthesis,
    Comma
};

std::string to_string(PunctuationMark punctuation_mark);

#endif //UXP1A_LINDA_PUNCTUATOR_H
