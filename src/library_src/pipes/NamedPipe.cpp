#include <sys/stat.h>
#include <pipes/exceptions/NamedPipeException.h>
#include "pipes/NamedPipe.h"
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <fcntl.h>
#include <iostream>

NamedPipe::NamedPipe(const std::string &fifoPath) : fifo_path(fifoPath)
{}

void NamedPipe::create()
{
    if (mkfifo(fifo_path.c_str(), fifo_permissions) < 0)
        throw NamedPipeException("An error occurred during creating a pipe.");
}

void NamedPipe::createIfNotExists()
{   
    if (!boost::filesystem::exists(fifo_path))
        create();
}

void NamedPipe::open(NamedPipe::Mode mode, bool blocking)
{
    const int flags = get_flags(mode, blocking);
    const int fifo_descriptor = ::open(fifo_path.c_str(), flags);
    if (fifo_descriptor < 0)
        throw NamedPipeException("Unable to open the pipe. ERR: " + std::to_string(errno));
    
    this->fifo_descriptor = fifo_descriptor;
    fifo_buffer_size = get_fifo_buffer_size();
}


void NamedPipe::write(const std::string &data)
{
    // TODO replace with a logging mechanism
    std::cout << "data to write: " + data << std::endl;
    if (::write(fifo_descriptor, data.c_str(), data.length()) < data.length())
        throw NamedPipeException("Error during writing to a pipe.");
    std::cout << "data written: " + data << std::endl;
}

std::string NamedPipe::read()
{
    std::cout << "NamedPipe::read" << std::endl;
    const int buffer_size = get_fifo_buffer_size();
    std::vector<char> buffer((unsigned long) buffer_size);
    ssize_t bytes_read = ::read(fifo_descriptor, &buffer[0], (size_t) buffer_size);
    std::cout << "bytes read: " + bytes_read << std::endl;

    if (bytes_read < 0)
        throw NamedPipeException("Cannot read from a pipe.");

    return std::string(buffer.begin(), buffer.begin() + bytes_read);
}

int NamedPipe::close()
{
    if (fifo_descriptor < 0)
        throw NamedPipeException("Invalid state - a pipe must be opened to be closed.");

    const int close_result = ::close(fifo_descriptor);
    if (close_result == 0)
        fifo_descriptor = -1;

    return close_result;
}

void NamedPipe::destroy()
{
    if (fifo_descriptor >= 0)
        close();
    
    boost::filesystem::remove(fifo_path);
}

int NamedPipe::get_flags(Mode mode, bool blocking)
{
    int flags;
    switch (mode)
    {
        case Mode::Read:
            flags = O_RDONLY;
            break;
        case Mode::Write:
            flags = O_WRONLY;
            break;
        case Mode::ReadWrite:
            flags = O_RDWR;
            break;
    }

    if (!blocking)
        flags |= O_NDELAY;

    return flags;
}

int NamedPipe::get_fifo_buffer_size()
{
    if (fifo_buffer_size > -1)
        return fifo_buffer_size;

    int pipe_size = fcntl(fifo_descriptor, F_GETPIPE_SZ);
    if (pipe_size < 0)
        pipe_size = default_fifo_size;

    fifo_buffer_size = pipe_size;
    return fifo_buffer_size;
}







