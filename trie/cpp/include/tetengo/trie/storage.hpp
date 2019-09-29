/*! \file
    \brief A storage.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP)
#define TETENGO_TRIE_STORAGE_HPP

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>


namespace tetengo::trie
{
    /*!
        \brief A storage.
    */
    class storage
    {
    public:
        /*!
            \brief Creates a storage.
        */
        storage();


        // functions

        /*!
            \brief Returns the base value.

            \param index An index.

            \return The base value.
        */
        std::int32_t base_at(std::size_t index) const;

        /*!
            \brief Sets a base value.

            \param index An index.
            \param value A value.
        */
        void set_base_at(std::size_t index, std::int32_t value);

        /*!
            \brief Returns the check value.

            \param index An index.

            \return The check value.
        */
        std::uint8_t check_at(std::size_t index) const;

        /*!
            \brief Sets a check value.

            \param index An index.
            \param value A value.
        */
        void set_check_at(std::size_t index, std::uint8_t value);

        /*!
            \brief Returns the size of this storage.

            \return The size.
        */
        std::size_t size() const;


    private:
        // variables

        mutable std::vector<std::pair<std::int32_t, std::uint8_t>> m_values;
    };


}


#endif
