/*! \file
    \brief An unordered map vocabulary.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_UNORDEREDMAPVOCABULARY_HPP)
#define TETENGO_LATTICE_UNORDEREDMAPVOCABULARY_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    class connection;
    class input;
    class node;


    /*!
        \brief An unordered map vocabulary.
    */
    class unordered_map_vocabulary : public vocabulary
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an unordered map vocabulary.

            \param entries        Entries.
            \param connections    Connections.
            \param entry_hash     A hash function for an entry.
            \param entry_equal_to An eqaul_to function for an entry.
        */
        unordered_map_vocabulary(
            std::vector<std::pair<std::string, std::vector<entry>>>   entries,
            std::vector<std::pair<std::pair<entry, entry>, int>>      connections,
            std::function<std::size_t(const entry_view&)>             entry_hash,
            std::function<bool(const entry_view&, const entry_view&)> entry_equal_to);

        /*!
            \brief Destroys the unordered map vocabulary.
        */
        virtual ~unordered_map_vocabulary();


    private:
        // types

        class impl;


        // variables

        std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::vector<entry_view> find_entries_impl(const input& key) const override;

        virtual connection find_connection_impl(const node& from, const entry_view& to) const override;
    };


}


#endif
