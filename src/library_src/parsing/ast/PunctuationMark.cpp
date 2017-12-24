//
// Created by root on 17/12/17.
//

#include "parsing/ast/PunctuationMark.h"

std::string to_string(PunctuationMark punctuation_mark)
{
    switch (punctuation_mark)
    {
        case PunctuationMark::LeftParenthesis: return "(";
        case PunctuationMark::RightParenthesis: return ")";
        case PunctuationMark::Comma: return ",";
    }
}