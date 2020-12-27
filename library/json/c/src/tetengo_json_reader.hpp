/*! \file
    \brief A reader.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_READER_HPP_)
#define TETENGO_JSON_READER_HPP_

#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>

#include <tetengo/json/reader.hpp>


struct tetengo_json_reader_tag
{
private:
    std::unique_ptr<tetengo::json::reader> p_cpp_reader;

    const tetengo::json::reader* p_cpp_reader_ref;

public:
    mutable std::unique_ptr<tetengo_json_reader_tag> p_base_reader_placeholder;

    explicit tetengo_json_reader_tag(std::unique_ptr<tetengo::json::reader>&& p_cpp_reader) :
    p_cpp_reader{ std::move(p_cpp_reader) },
        p_cpp_reader_ref{},
        p_base_reader_placeholder{}
    {}

    explicit tetengo_json_reader_tag(const tetengo::json::reader* const p_cpp_reader_ref) :
    p_cpp_reader{},
        p_cpp_reader_ref{ p_cpp_reader_ref },
        p_base_reader_placeholder{}
    {}

    const tetengo::json::reader& cpp_reader() const
    {
        if (!p_cpp_reader && !p_cpp_reader_ref)
        {
            assert(false);
            throw std::logic_error{ "cpp_reader is NULL." };
        }
        return p_cpp_reader ? *p_cpp_reader : *p_cpp_reader_ref;
    }

    tetengo::json::reader& cpp_reader()
    {
        if (!p_cpp_reader)
        {
            assert(false);
            throw std::logic_error{ "cpp_reader is NULL." };
        }
        return *p_cpp_reader;
    }

    std::unique_ptr<tetengo::json::reader>&& move_cpp_reader()
    {
        if (!p_cpp_reader)
        {
            assert(false);
            throw std::logic_error{ "cpp_reader is NULL." };
        }
        return std::move(p_cpp_reader);
    }

    void set_cpp_reader_ref(const tetengo::json::reader& reader)
    {
        assert(!p_cpp_reader);
        p_cpp_reader_ref = &reader;
    }
};


#endif
