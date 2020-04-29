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
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    class connection;


    /*!
        \brief An unordered_map vocabulary.
    */
    class unordered_map_vocabulary : public vocabulary
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an unordered_map vocabulary.

            \param entry_map      An entry map.
            \param connection_map A connection map.
        */
        unordered_map_vocabulary(
            std::unordered_map<std::string, std::vector<entry>> entry_map,
            std::unordered_map<std::pair<entry, entry>, int>    connection_map);

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

        virtual connection find_connection_impl(const entry_view& from, const entry_view& to) const override;
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
