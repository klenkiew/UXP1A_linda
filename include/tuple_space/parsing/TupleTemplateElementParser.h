#ifndef UXP1A_LINDA_TUPLETEMPLATEELEMENTPARSER_H
#define UXP1A_LINDA_TUPLETEMPLATEELEMENTPARSER_H

#include "tuple_space/TupleTemplate.h"
#include "Scanner.h"
#include "tuple_space/TupleTemplateElement.h"

class TupleTemplateElementParser
{
public:
    explicit TupleTemplateElementParser(std::unique_ptr<Scanner> scanner);

    TupleTemplateElementParser(const TupleTemplateElementParser&) = delete;
    const TupleTemplateElementParser& operator=(const TupleTemplateElementParser&) = delete;

    std::unique_ptr<TupleTemplate> parse();
    std::unique_ptr<TupleTemplateElement> parse_tuple_template_element();

private:
    std::unique_ptr<Scanner> scanner;

    void advance() const;
    bool is(PunctuationMark punctuation_mark) const;
    void skip(PunctuationMark punctuation_mark) const;
    bool try_skip_comma() const;
    TupleElement::Type read_type() const;
    bool is_operator() const;

    std::unique_ptr<TupleTemplateElement> parse_comparer_template(TupleElement::Type type);
    std::unique_ptr<TupleTemplateElement> parse_equal_template(TupleElement::Type type);
    std::unique_ptr<TupleTemplateElement> parse_string_comparer_template(const Operator &operator_) const;
    std::unique_ptr<TupleTemplateElement> parse_integer_comparer_template(const Operator &operator_) const;

};


#endif //UXP1A_LINDA_TUPLETEMPLATEELEMENTPARSER_H
