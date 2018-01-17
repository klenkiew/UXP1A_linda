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
    ~NamedPipe();

    void create();
    void createIfNotExists();
    void open(Mode mode, bool blocking = true);
    void open(Mode mode, int timeout_seconds);
    void set_nonblocking();
    int close();
    void destroy();

    void write(const std::string& data);
    void write(const std::string &data, int timeout_seconds);
    std::string read();
    std::string read(int timeout_seconds);

private:
    const int fifo_permissions = 0777;
    const int default_fifo_size = 512;

    const std::string fifo_path;
    int fifo_buffer_size = -1;
    int fifo_descriptor = -1;

    int get_flags(Mode mode, bool b);
    int get_fifo_buffer_size();

    void throw_on_error(const std::string& message) const;

};


#endif //UXP1A_LINDA_NAMEDPIPE_H
