#include <sstream>
#include <pipes/NamedPipe.h>
#include <pipes/exceptions/NamedPipeException.h>
#include <boost/log/trivial.hpp>
#include <tuple_space/parsing/exceptions/ParseException.h>
#include "linda_api/LindaApi.h"

TupleSpace::TupleSpace(const std::string &name)
    : tuple_space_name(name)
{
    std::string home_dir = get_home_dir_path();
    std::string common_files_path = home_dir + "/linda_" + tuple_space_name + "_";
    tuples_path = common_files_path + "tuples";
    templates_path = common_files_path + "templates";

    { // ensure that the files will exist
        std::fstream tuples(tuples_path, std::fstream::out | std::fstream::app);
        std::fstream templates(templates_path, std::fstream::out | std::fstream::app);
    }
}

void TupleSpace::reset()
{
    ExclusiveFileAccessor templates_file(templates_path);
    templates_file.rewrite_whole_file("");
    ExclusiveFileAccessor tuples_file(tuples_path);
    tuples_file.rewrite_whole_file("");
}

bool TupleSpace::linda_output(std::string tuple)
{
    std::unique_ptr<Tuple> parsed_tuple;
    try {
        parsed_tuple = get_parsed_tuple(tuple);
    } catch (ParseException &e) {
        BOOST_LOG_TRIVIAL(debug) << e.what();
        return false;
    } catch (EndOfFile &e) {
        BOOST_LOG_TRIVIAL(debug) << "empty tuple passed into linda_output";
        return false;
    }

    { // templates_file exclusive scope
        ExclusiveFileAccessor templates_file(templates_path);
        auto templates = get_template_with_fifo_pairs(templates_file);

        for (auto &&template_with_fifo : templates) {
            if (template_with_fifo.first->matches(*parsed_tuple)) {
                return send_tuple(tuple, template_with_fifo.second);
            }
        }
    }
    // nobody waits for this kind of tuple, write it into tuples_file
    { // tuples_file exclusive scope
        ExclusiveFileAccessor tuples_file(tuples_path);
        tuples_file.append(tuple + "\n");
    }
}

std::unique_ptr<Tuple> TupleSpace::get_parsed_tuple(const std::string &input)
{
    std::istringstream inputStream((input));
    TupleParser parser(std::make_unique<Scanner>(inputStream));
    auto output = parser.parse();
    return output;
}

bool TupleSpace::send_tuple(const std::string &tuple, const std::string &fifo)
{
    try {
        NamedPipe pipe(fifo);
        pipe.open(NamedPipe::Mode::Write, false); // open in nonblocking mode
        pipe.write(tuple);
    } catch (NamedPipeException &e) {
        BOOST_LOG_TRIVIAL(debug) << e.what();
        return false;
    }
    return true;
}

std::string TupleSpace::linda_input(std::string pattern, int timeout)
{

}

std::string TupleSpace::linda_read(std::string pattern, int timeout)
{

}

std::string TupleSpace::get_home_dir_path()
{
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return std::string(homedir);
}

std::vector<std::pair<std::unique_ptr<TupleTemplate>, std::string>>
TupleSpace::get_template_with_fifo_pairs(const ExclusiveFileAccessor &file)
{
    std::stringstream file_content(file.read_whole_file());
    std::string fifo_name;
    std::string template_as_string;
    std::vector<std::pair<std::unique_ptr<TupleTemplate>, std::string>> tuples_with_fifos;

    while (file_content)
    {
        file_content >> fifo_name;
        file_content >> template_as_string;
        if (fifo_name.empty() || template_as_string.empty())
            continue;
        tuples_with_fifos.emplace_back(get_parsed_tuple_template(template_as_string), fifo_name);
    }
    return tuples_with_fifos;
}

std::unique_ptr<TupleTemplate> TupleSpace::get_parsed_tuple_template(const std::string &input)
{
    std::istringstream inputStream((input));
    TupleTemplateElementParser parser(std::make_unique<Scanner>(inputStream));
    auto output = parser.parse();
    return output;
}