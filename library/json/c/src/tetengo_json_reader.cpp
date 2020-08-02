/*! \file
    \brief A reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>

#include <stddef.h>

#include <tetengo/json/reader.h>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>


struct tetengo_json_reader_tag
{
    std::unique_ptr<tetengo::json::reader> p_cpp_reader;

    tetengo_json_reader_tag(std::unique_ptr<tetengo::json::reader>&& p_cpp_reader) :
    p_cpp_reader{ std::move(p_cpp_reader) }
    {}
};


size_t tetengo_json_reader_streamReaderDefaultBufferCapacity()
{
    return 4096;
}

tetengo_json_reader_t* tetengo_json_reader_createStreamReader(const char* const file_path, const size_t buffer_capacity)
{
    try
    {
        auto p_stream = std::make_unique<std::ifstream>(file_path);
        if (!*p_stream)
        {
            throw std::runtime_error{ "Can't open the file." };
        }

        auto p_cpp_reader = std::make_unique<tetengo::json::stream_reader>(std::move(p_stream), buffer_capacity);

        auto p_instance = std::make_unique<tetengo_json_reader_t>(std::move(p_cpp_reader));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_json_reader_destroy(const tetengo_json_reader_t* const p_reader)
{
    try
    {
        const std::unique_ptr<const tetengo_json_reader_t> p_instance{ p_reader };
    }
    catch (...)
    {}
}

int tetengo_json_reader_hasNext(const tetengo_json_reader_t* const p_reader)
{
    try
    {
        if (!p_reader)
        {
            throw std::invalid_argument{ "p_reader is NULL." };
        }

        return p_reader->p_cpp_reader->has_next() ? 1 : 0;
    }
    catch (...)
    {
        return 0;
    }
}

char tetengo_json_reader_get(const tetengo_json_reader_t* const p_reader)
{
    try
    {
        if (!p_reader)
        {
            throw std::invalid_argument{ "p_reader is NULL." };
        }

        return p_reader->p_cpp_reader->get();
    }
    catch (...)
    {
        return 0;
    }
}

void tetengo_json_reader_next(tetengo_json_reader_t* const p_reader)
{
    try
    {
        if (!p_reader)
        {
            throw std::invalid_argument{ "p_reader is NULL." };
        }

        p_reader->p_cpp_reader->next();
    }
    catch (...)
    {}
}
