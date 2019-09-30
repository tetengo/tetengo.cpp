/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAY_HPP)
#define TETENGO_TRIE_DOUBLEARRAY_HPP

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/enumerator.hpp>
#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    /*!
        \brief A double array.
    */
    class double_array
    {
    public:
        // static functions

        /*!
            \brief Returns the key terminator.

            \return The key terminator.
        */
        static constexpr char key_terminator()
        {
            return '\0';
        }

        /*!
            \brief Returns the check value for a vacant element.

            \return The check value for a vacant element.
        */
        static constexpr std::uint8_t vacant_check_value()
        {
            return 0xFF;
        }


        // constructors and destructor

        /*!
            \brief Creates a double array.
        */
        double_array();

        /*!
            \brief Creates a double array.

            \param element_pointers Pointers to initial elements.
        */
        explicit double_array(std::vector<const std::pair<std::string, std::int32_t>*> element_pointers);

        /*!
            \brief Creates a double array.

            \tparam InputIterator An input iterator type.

            \param first An iterator to the first element.
            \param last  An iterator to the last element.
        */
        template <typename InputIterator>
        double_array(InputIterator first, InputIterator last) :
        double_array{ [first, last]() {
            std::vector<const std::pair<std::string, std::int32_t>*> element_pointers;
            std::transform(first, last, std::back_inserter(element_pointers), [](const auto& e) { return &e; });
            return element_pointers;
        }() }
        {}

        /*!
            \brief Creates a double array.

            \param elements Initial elements.
        */
        explicit double_array(const std::vector<std::pair<std::string, std::int32_t>>& elements);


        // functions

        /*!
            \brief Returns the base check array.

            \return The base check array.
        */
        const std::vector<std::uint32_t>& base_check_array() const;

        /*!
            \brief Finds the value correspoinding the given key.

            \param key A key.

            \return The value. Or std::nullpot when the double array does not have the given key.
        */
        std::optional<std::int32_t> find(const std::string& key) const;

        /*!
            \brief Returns an enumerator.

            \return An enumerator.
        */
        enumerator get_enumerator() const;


    private:
        // variables

        storage m_storage;
    };


}


#endif
