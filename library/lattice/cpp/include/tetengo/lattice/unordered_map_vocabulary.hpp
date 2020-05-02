/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_UNORDEREDMAPVOCABULARY_HPP)
#define TETENGO_LATTICE_UNORDEREDMAPVOCABULARY_HPP

#include <cstddef>
#include <functional> // IWYU pragma: keep
#include <memory>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    class connection;
    class node;


    /*!
        \brief An unordered_map vocabulary.
    */
    class unordered_map_vocabulary : public vocabulary
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an unordered_map vocabulary.

            \param entries     Entries.
            \param connections Connections.
        */
        unordered_map_vocabulary(
            std::vector<std::pair<std::string, std::vector<entry>>> entries,
            std::vector<std::pair<std::pair<entry, entry>, int>>    connections);

        /*!
            \brief Destroys the unordered_map vocabulary.
        */
        virtual ~unordered_map_vocabulary();


    private:
        // types

        class impl;


        // variables

        std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::vector<entry_view> find_entries_impl(const std::string_view& key) const override;

        virtual connection find_connection_impl(const node& from, const node& to) const override;
    };


}


namespace std
{
    template <>
    struct hash<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>>
    {
        std::size_t operator()(const std::pair<tetengo::lattice::entry, tetengo::lattice::entry>& key) const;
    };


}


#endif
