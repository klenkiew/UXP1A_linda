#include <sstream>
#include <pipes/NamedPipe.h>
#include <unistd.h>
#include <pipes/exceptions/NamedPipeException.h>
#include <boost/log/trivial.hpp>
#include <tuple_space/parsing/exceptions/ParseException.h>
#include <pipes/exceptions/NamedPipeTimeoutException.h>
#include <thread>
#include <string>
#include "linda_api/LindaApi.h"

TupleSpace::TupleSpace(const std::string &name)
    : tuple_space_name(name)
{
    std::string home_dir = get_home_dir_path();
    std::string common_files_path = home_dir + "/linda_" + tuple_space_name + "_";
    tuples_path = common_files_path + "tuples";
    templates_path = common_files_path + "templates";
}

void TupleSpace::reset()
{
    ExclusiveFileAccessor templates_file(templates_path);
    templates_file.unlink();
    ExclusiveFileAccessor tuples_file(tuples_path);
    tuples_file.unlink();

}

void TupleSpace::linda_output(std::string tuple)
{
    std::unique_ptr<Tuple> parsed_tuple = get_parsed_tuple(tuple);

    { // templates_file exclusive scope
        ExclusiveFileAccessor templates_file(templates_path);
        auto templates = get_template_with_fifo_pairs(templates_file);

        for (auto &&template_with_fifo : templates) {
            if (template_with_fifo.first->matches(*parsed_tuple)) {
                // get first character of FIFO name. It indicates if it is read or input
                char read_type = template_with_fifo.second.at(0);
                if (read_type == 'I') {
                    // input - discard this tuple
                    send_tuple(tuple, template_with_fifo.second);
                    return;
                }
                if (read_type == 'R') {
                    // read - keep this tuple
                    send_tuple(tuple, template_with_fifo.second);
                    continue;
                }
            }
        }
    }
    // nobody waits for this kind of tuple, write it into tuples_file
    { // tuples_file exclusive scope
        ExclusiveFileAccessor tuples_file(tuples_path);
        tuples_file.append(tuple + "\n\n");
    }
}

std::unique_ptr<Tuple> TupleSpace::get_parsed_tuple(const std::string &input)
{
    std::istringstream inputStream((input));
    TupleParser parser(std::make_unique<Scanner>(inputStream));
    auto output = parser.parse();
    return output;
}

std::vector<std::pair<std::unique_ptr<TupleTemplate>, std::string>>
TupleSpace::get_template_with_fifo_pairs(const ExclusiveFileAccessor &file)
{
    std::stringstream file_content(file.read_whole_file());
    std::string fifo_name;
    std::string template_as_string;
    std::string line;
    std::vector<std::pair<std::unique_ptr<TupleTemplate>, std::string>> tuples_with_fifos;

    while (std::getline(file_content, line))
    {
        const auto first_space_index = line.find(" ");
        fifo_name = line.substr(0, first_space_index);
        template_as_string = line.substr(first_space_index + 1);
        if (fifo_name.empty() || template_as_string.empty())
            continue;

        try {
            tuples_with_fifos.emplace_back(get_parsed_tuple_template(template_as_string), fifo_name);
        } catch (ParseException) {
            BOOST_LOG_TRIVIAL(debug) << "get_template_with_fifo_pairs: " << fifo_name << " " << template_as_string;
            throw;
        }
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

void TupleSpace::send_tuple(const std::string &tuple, const std::string &fifo)
{
    NamedPipe pipe(fifo);
    pipe.open(NamedPipe::Mode::Write, false); // open in nonblocking mode
    pipe.write(tuple);
}

std::string TupleSpace::linda_input(std::string pattern, int timeout)
{
    return tuple_read_util(pattern, timeout, true);
}

std::string TupleSpace::linda_read(std::string pattern, int timeout)
{
    return tuple_read_util(pattern, timeout, false);
}

std::string TupleSpace::tuple_read_util(const std::string &pattern, int timeout, bool is_input)
{
    std::unique_ptr<TupleTemplate> parsed_template = get_parsed_tuple_template(pattern);

    { // tuples_file exclusive scope
        ExclusiveFileAccessor tuples_file(tuples_path);
        auto tuples = get_tuples(tuples_file);

        for (auto &&tuple_with_string : tuples) {
            if (parsed_template->matches(*tuple_with_string.first)) {
                // tuple found in already placed tuples
                std::string matching_tuple = tuple_with_string.second;
                if (is_input)
                    tuples_file.erase(matching_tuple + "\n");
                return matching_tuple;
            }
        }
    }
    // tuple not found - put pattern into templates_file
    // IMPORTANT: first letter of fifo name ({R, I}) signals the intention: R for read, I for input
    std::string fifo_name = (is_input? "Ipid" : "Rpid") + std::to_string(getpid()) + "_Tid_"+ std::to_string(
            std::hash<std::thread::id>{}(std::this_thread::get_id()))
                            + "_rand" + get_random_string(5);
    std::string template_entry = fifo_name + " " + pattern + "\n";

    { // templates_file exclusive scope
        ExclusiveFileAccessor templates_file(templates_path);
        templates_file.append(template_entry);
    }

    std::string read_tuple;
    NamedPipe pipe(fifo_name);
    pipe.createIfNotExists();
    // open pipe
    try {
        pipe.open(NamedPipe::Mode::Read, timeout);
    } catch (NamedPipeTimeoutException &e) {
        ExclusiveFileAccessor templates_file(templates_path);
        templates_file.erase(template_entry);
        pipe.close();
        pipe.destroy();
        throw;
    }

    // read from pipe
    try {
        read_tuple = pipe.read(timeout);
    } catch (const NamedPipeTimeoutException &e) {
        ExclusiveFileAccessor templates_file(templates_path);
        templates_file.erase(template_entry);
        pipe.close();
        pipe.destroy();
        throw;
    }


    // tuple successfully read - remove template entry
    ExclusiveFileAccessor templates_file(templates_path);
    templates_file.erase(template_entry);
    pipe.close();
    pipe.destroy();
    return read_tuple;
}

std::vector<std::pair<std::unique_ptr<Tuple>, std::string>> TupleSpace::get_tuples(const ExclusiveFileAccessor &tuples_file)
{
    std::stringstream file_content(tuples_file.read_whole_file());
    std::string tuple_as_string;
    std::vector<std::pair<std::unique_ptr<Tuple>, std::string>> tuples;

    while (file_content)
    {
        std::getline(file_content, tuple_as_string);
        if (tuple_as_string.empty())
            continue;
        try {
            tuples.emplace_back(get_parsed_tuple(tuple_as_string), tuple_as_string);
        } catch (ParseException) {
            BOOST_LOG_TRIVIAL(debug) << "get_tuples: " << tuple_as_string;
            throw;
        }
    }
    return tuples;
}

std::string TupleSpace::get_random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}

std::string TupleSpace::get_home_dir_path()
{
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return std::string(homedir);
}