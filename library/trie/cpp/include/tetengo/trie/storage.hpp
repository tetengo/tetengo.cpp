/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP)
#define TETENGO_TRIE_STORAGE_HPP

#include <any>
#include <cstdint>
#include <functional>
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
        storage();

        /*!
            \brief Destroys the storage.
        */
        virtual ~storage();


        // functions

        /*!
            \brief Returns the base value.

            \param base_check_index A base-check index.

            \return The base value.
        */
        [[nodiscard]] std::int32_t base_at(std::size_t base_check_index) const;

        /*!
            \brief Sets a base value.

            \param base_check_index A base-check index.
            \param base             A base value.
        */
        void set_base_at(std::size_t base_check_index, std::int32_t base);

        /*!
            \brief Returns the check value.

            \param base_check_index A base-check index.

            \return The check value.
        */
        [[nodiscard]] std::uint8_t check_at(std::size_t base_check_index) const;

        /*!
            \brief Sets a check value.

            \param base_check_index A base-check index.
            \param check            A check value.
        */
        void set_check_at(std::size_t base_check_index, std::uint8_t check);

        /*!
            \brief Returns the size.

            \return The size.
        */
        [[nodiscard]] std::size_t size() const;

        /*!
            \brief Returns the filling rate.

            \return The filling rate.
        */
        [[nodiscard]] double filling_rate() const;

        /*!
            \brief Returns the base-check array.

            \return The base-check array.
        */
        [[nodiscard]] const std::vector<std::uint32_t>& base_check_array() const;

        /*!
            \brief Returns the value object.

            \param value_index A value index.

            \return A pointer to the value object. Or nullptr when there is no corresponding value object.
        */
        [[nodiscard]] const std::any* value_at(std::size_t value_index) const;

        /*!
            \brief Adds a value storage index mapping.

            \param value_index A value index.
            \param value       A value object.
        */
        void add_value_at(std::size_t value_index, std::any value);

        /*!
            \brief Serializes this storage.

            \param output_stream    An output stream.
            \param value_serializer A serializer for value objects.
        */
        void serialize(
            std::ostream&                                            output_stream,
            const std::function<std::vector<char>(const std::any&)>& value_serializer) const;

        /*!
            \brief Clones this storage.

            \return A unique pointer to a clone of this storage.
        */
        [[nodiscard]] std::unique_ptr<storage> clone() const;


    private:
        // virtual functions

        virtual std::int32_t base_at_impl(std::size_t base_check_index) const = 0;

        virtual void set_base_at_impl(std::size_t base_check_index, std::int32_t base) = 0;

        virtual std::uint8_t check_at_impl(std::size_t base_check_index) const = 0;

        virtual void set_check_at_impl(std::size_t base_check_index, std::uint8_t check) = 0;

        virtual std::size_t size_impl() const = 0;

        virtual double filling_rate_impl() const = 0;

        virtual const std::vector<std::uint32_t>& base_check_array_impl() const = 0;

        virtual const std::any* value_at_impl(std::size_t value_index) const = 0;

        virtual void add_value_at_impl(std::size_t value_index, std::any value) = 0;

        virtual void serialize_impl(
            std::ostream&                                            output_stream,
            const std::function<std::vector<char>(const std::any&)>& value_serializer) const = 0;

        virtual std::unique_ptr<storage> clone_impl() const = 0;
    };


}


#endif
