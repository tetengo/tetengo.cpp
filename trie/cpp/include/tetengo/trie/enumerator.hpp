/*! \file
    \brief An enumerator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_ENUMERATOR_HPP)
#define TETENGO_TRIE_ENUMERATOR_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <vector>


namespace tetengo::trie
{
    class storage;


    /*!
        \brief An enumerator.
    */
    class enumerator
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an enumerator.

            \param storage_      A storage.
            \param root_index    A root index.
            \param initial_index An intial_index.
        */
        enumerator(const storage& storage_, std::size_t root_index, std::size_t initial_index);


        // functions

        /*!
            \brief Returns the next element.

            \return The next element. Or std::nullopt when no next element.
        */
        std::optional<std::pair<std::string, std::int32_t>> next() const;


    private:
        // variables

        const storage& m_storage;

        mutable std::stack<std::pair<std::size_t, std::string>, std::vector<std::pair<std::size_t, std::string>>>
            m_index_key_stack;
    };


}


#endif
