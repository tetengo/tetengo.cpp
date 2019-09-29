/*! \file
    \brief A storage.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP)
#define TETENGO_TRIE_STORAGE_HPP

#include <cstdint>
#include <istream>
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

        /*!
            \brief Creates a storage.

            \param input_stream An input stream.
        */
        explicit storage(std::istream& input_stream);


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

        /*!
            \brief Returns the all values.

            \return The all values.
        */
        const std::vector<std::uint32_t>& values() const;

        /*!
            \brief Serializes this storage.
            
            \param output_stream An output stream.
        */
        void serialize(std::ostream& output_stream) const;


    private:
        // variables

        mutable std::vector<std::uint32_t> m_values;
    };


}


#endif
