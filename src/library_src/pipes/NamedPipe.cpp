#include "pipes/NamedPipe.h"

#include <pipes/exceptions/NamedPipeException.h>
#include <pipes/utils/SystemTimer.h>
#include <pipes/utils/TimeoutHandler.h>
#include <pipes/exceptions/NamedPipeTimeoutException.h>

#include <sys/stat.h>
#include <cstdio>
#include <iostream>
#include <fcntl.h>

#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

NamedPipe::NamedPipe(const std::string &fifoPath) : fifo_path(fifoPath)
{}

void NamedPipe::create()
{
    if (mkfifo(fifo_path.c_str(), fifo_permissions) < 0)
        throw NamedPipeException("An error occurred during creating a pipe. ERR: " + std::to_string(errno));
}

void NamedPipe::createIfNotExists()
{
    if (boost::filesystem::exists(fifo_path))
        return;

    // 17 - EEXIST - we don't care
    if (mkfifo(fifo_path.c_str(), fifo_permissions) < 0 && errno != 17)
        throw NamedPipeException("An error occurred during creating a pipe. ERR: " + std::to_string(errno));
}

void NamedPipe::open(NamedPipe::Mode mode, bool blocking)
{
    const int flags = get_flags(mode, blocking);
    const int fifo_descriptor = ::open(fifo_path.c_str(), flags);
    if (fifo_descriptor < 0)
        throw_on_error("Unable to open the pipe. ERR: " + std::to_string(errno));

    this->fifo_descriptor = fifo_descriptor;
    fifo_buffer_size = get_fifo_buffer_size();
}

void NamedPipe::write(const std::string &data)
{
    BOOST_LOG_TRIVIAL(debug) << "NamedPipe::write. Data: " + data;

    if (::write(fifo_descriptor, data.c_str(), data.length()) < data.length())
        throw_on_error("Error during writing to a pipe.");

    BOOST_LOG_TRIVIAL(debug) << "Data: " <<  data << " written to the pipe " << this->fifo_path;
}


void NamedPipe::write(const std::string& data, int timeout_seconds)
{
    // Initialize file descriptor sets
    fd_set read_fds, write_fds, except_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(fifo_descriptor, &write_fds);

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds;
    timeout.tv_usec = 0;

    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    if (select(fifo_descriptor + 1, &read_fds, &write_fds, &except_fds, &timeout) != 1)
        throw NamedPipeTimeoutException("Write timed out");

    return write(data);
}

std::string NamedPipe::read()
{
    BOOST_LOG_TRIVIAL(debug) << "NamedPipe::read";
    const int buffer_size = get_fifo_buffer_size();
    std::vector<char> buffer((unsigned long) buffer_size);
    ssize_t bytes_read = ::read(fifo_descriptor, &buffer[0], (size_t) buffer_size);

    if (bytes_read < 0)
        throw NamedPipeException("Cannot read from a pipe.");

    const auto& output = std::string(buffer.begin(), buffer.begin() + bytes_read);
    BOOST_LOG_TRIVIAL(debug) << "Data: " << output << " read from the pipe " << this->fifo_path;
    return output;
}

std::string NamedPipe::read(int timeout_seconds)
{
    // Initialize file descriptor sets
    fd_set read_fds, write_fds, except_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(fifo_descriptor, &read_fds);

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds;
    timeout.tv_usec = 0;

    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    if (select(fifo_descriptor + 1, &read_fds, &write_fds, &except_fds, &timeout) != 1)
        throw NamedPipeTimeoutException("Read timed out");

    return read();
}

int NamedPipe::close()
{
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

void NamedPipe::throw_on_error(const std::string& message) const
{
    // EINTR - timeout
    if (errno == 4)
        throw NamedPipeTimeoutException("TIMEOUT. " + message);

    throw NamedPipeException(message);
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

void NamedPipe::open(NamedPipe::Mode mode, int timeout_seconds)
{
    TimeoutHandler timeout_handler(timeout_seconds);
    open(mode);

    // actually never happens - because open returns an error (EINTR) and an exception is thrown earlier
    // but let's keep it just in case
    if (timeout_handler.timed_out())
        throw NamedPipeTimeoutException("Open timed out.");
}

NamedPipe::~NamedPipe()
{
    close();
}







