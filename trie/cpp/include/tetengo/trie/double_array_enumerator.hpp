/*! \file
    \brief A double array enumerator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAYENUMERATOR_HPP)
#define TETENGO_TRIE_DOUBLEARRAYENUMERATOR_HPP

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
        \brief A double array enumerator.
    */
    class double_array_enumerator
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an enumerator.

            \param storage_              A storage.
            \param root_base_check_index A root base-check index.
        */
        double_array_enumerator(const storage& storage_, std::size_t root_base_check_index);


        // functions

        /*!
            \brief Returns the next element.

            \return The next element. Or std::nullopt when no next element.
        */
        std::optional<std::pair<std::string, std::int32_t>> next();


    private:
        // variables

        const storage& m_storage;

        std::stack<std::pair<std::size_t, std::string>, std::vector<std::pair<std::size_t, std::string>>>
            m_base_check_index_key_stack;
    };


}


#endif
