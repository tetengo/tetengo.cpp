/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <vector>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    class input;
    class node;


    vocabulary::vocabulary() = default;

    vocabulary::~vocabulary() = default;

    std::vector<entry_view> vocabulary::find_entries(const input& key) const
    {
        return find_entries_impl(key);
    }

    connection vocabulary::find_connection(const node& from, const entry_view& to) const
    {
        return find_connection_impl(from, to);
    }


}
