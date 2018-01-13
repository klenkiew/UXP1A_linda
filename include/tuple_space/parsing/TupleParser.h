#ifndef UXP1A_LINDA_TUPLEPARSER_H
#define UXP1A_LINDA_TUPLEPARSER_H


#include "tuple_space/Tuple.h"
#include "Scanner.h"

class TupleParser
{
public:
    explicit TupleParser(std::unique_ptr<Scanner> scanner): scanner(std::move(scanner)) {}

    TupleParser(const TupleParser&) = delete;
    const TupleParser& operator=(const TupleParser&) = delete;

    std::unique_ptr<Tuple> parse();

private:
    std::unique_ptr<Scanner> scanner;

    void advance() const;
    void skip(PunctuationMark punctuation_mark) const;
    bool try_skip_comma() const;
    void throwOnEof() const;

    bool is(PunctuationMark punctuation_mark) const;
    bool is_eof() const;

    int parse_integer() const;
    std::unique_ptr<TupleElement> parse_tuple_element();
};


#endif //UXP1A_LINDA_TUPLEPARSER_H
