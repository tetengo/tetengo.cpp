/*! \file
    \brief A reader.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    reader::~reader() = default;

    bool reader::has_next() const
    {
        return has_next_impl();
    }

    char reader::peek() const
    {
        return peek_impl();
    }

    void reader::next()
    {
        next_impl();
    }

    const reader& reader::base_reader() const
    {
        return base_reader_impl();
    }


}
