/*! \file
    \brief A storage.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP)
#define TETENGO_TRIE_STORAGE_HPP

#include <cstdint>
#include <istream>
#include <memory>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::trie
{
    /*!
        \brief A storage.
    */
    class storage : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a storage.
        */
        storage() = default;

        /*!
            \brief Destroys the storage.
        */
        virtual ~storage() = default;


        // functions

        /*!
            \brief Returns the base value.

            \param base_check_index A base-check index.

            \return The base value.
        */
        std::int32_t base_at(std::size_t base_check_index) const;

        /*!
            \brief Sets a base value.

            \param base_check_index A base-check index.
            \param value            A value.
        */
        void set_base_at(std::size_t base_check_index, std::int32_t value);

        /*!
            \brief Returns the check value.

            \param base_check_index A base-check index.

            \return The check value.
        */
        std::uint8_t check_at(std::size_t base_check_index) const;

        /*!
            \brief Sets a check value.

            \param base_check_index A base-check index.
            \param value            A value.
        */
        void set_check_at(std::size_t base_check_index, std::uint8_t value);

        /*!
            \brief Returns the size of this storage.

            \return The size.
        */
        std::size_t size() const;

        /*!
            \brief Returns the filling rate.

            \return The filling rate.
        */
        double filling_rate() const;

        /*!
            \brief Returns the base-check array.

            \return The base-check array.
        */
        const std::vector<std::uint32_t>& base_check_array() const;

        /*!
            \brief Serializes this storage.

            \param output_stream An output stream.
        */
        void serialize(std::ostream& output_stream) const;

        /*!
            \brief Clones this storage.

            \return A unique pointer to a clone of this storage.
        */
        std::unique_ptr<storage> clone() const;


    private:
        // virtual functions

        virtual std::int32_t base_at_impl(std::size_t base_check_index) const = 0;

        virtual void set_base_at_impl(std::size_t base_check_index, std::int32_t value) = 0;

        virtual std::uint8_t check_at_impl(std::size_t base_check_index) const = 0;

        virtual void set_check_at_impl(std::size_t base_check_index, std::uint8_t value) = 0;

        virtual std::size_t size_impl() const = 0;

        virtual double filling_rate_impl() const = 0;

        virtual const std::vector<std::uint32_t>& base_check_array_impl() const = 0;

        virtual void serialize_impl(std::ostream& output_stream) const = 0;

        virtual std::unique_ptr<storage> clone_impl() const = 0;
    };


}


#endif
