#include "tuple_space/parsing/ast/PunctuationMark.h"

std::string to_string(PunctuationMark punctuation_mark)
{
    switch (punctuation_mark)
    {
        case PunctuationMark::LeftParenthesis: return "(";
        case PunctuationMark::RightParenthesis: return ")";
        case PunctuationMark::Comma: return ",";
    }
}