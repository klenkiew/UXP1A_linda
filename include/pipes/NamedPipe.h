#ifndef UXP1A_LINDA_NAMEDPIPE_H
#define UXP1A_LINDA_NAMEDPIPE_H


#include <string>

class NamedPipe
{
public:
    enum class Mode
    {
        Read,
        Write,
        ReadWrite
    };

    explicit NamedPipe(const std::string &fifoPath);
    void create();
    void createIfNotExists();
    void open(Mode mode, bool blocking = true);
    int close();
    void destroy();

    void write(const std::string& data);
    std::string read();

private:
    static const int fifo_permissions = 777;
    static const int default_fifo_size = 512;

    const std::string fifo_path;
    int fifo_buffer_size = -1;
    int fifo_descriptor = -1;

    int get_flags(Mode mode, bool b);
    int get_fifo_buffer_size();
};


#endif //UXP1A_LINDA_NAMEDPIPE_H
