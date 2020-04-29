/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_UNORDEREDMAPVOCABULARY_HPP)
#define TETENGO_LATTICE_UNORDEREDMAPVOCABULARY_HPP

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    /*!
        \brief An unordered_map vocabulary.
    */
    class unordered_map_vocabulary : public vocabulary
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an unordered_map vocabulary.

            \param map A entry map.
        */
        explicit unordered_map_vocabulary(std::unordered_map<std::string, std::vector<entry>> map);

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
    };


}


#endif
