#include <memory>
#include <string>
#include <tuple_space/TupleTemplate.h>


class TupleTemplateInfo
{
public:
    TupleTemplateInfo(std::unique_ptr<TupleTemplate> tuple_template,
                      std::string fifo_name,
                      std::string tuple_template_as_string)
            : tuple_template(std::move(tuple_template)),
              fifo_name(std::move(fifo_name)),
              tuple_template_as_string(std::move(tuple_template_as_string))
    {}

    const TupleTemplate& get_tuple_template() const
    {
        return *tuple_template;
    }

    const std::string & get_fifo_name() const
    {
        return fifo_name;
    }

    const std::string & get_tuple_template_as_string() const
    {
        return tuple_template_as_string;
    }

    std::string to_string() const
    {
        return fifo_name + " " + tuple_template_as_string;
    }

private:
    std::unique_ptr<TupleTemplate> tuple_template;
    const std::string fifo_name;
    // original tuple template string - because TupleTemplate::to_string may differ
    const std::string tuple_template_as_string;
};