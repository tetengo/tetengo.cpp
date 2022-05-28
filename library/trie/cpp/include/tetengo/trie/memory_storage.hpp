/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_MEMORYSTORAGE_HPP)
#define TETENGO_TRIE_MEMORYSTORAGE_HPP

#include <any>
#include <cstddef> // IWYU pragma: keep
#include <cstdint>
#include <istream>
#include <memory>

#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    class value_deserializer;
    class value_serializer;


    /*!
        \brief A memory storage.
    */
    class memory_storage : public storage
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a memory storage.
        */
        memory_storage();

        /*!
            \brief Creates a memory storage.

            \param input_stream        An input stream.
            \param value_deserializer_ A deserializer for value objects.
        */
        memory_storage(std::istream& input_stream, const value_deserializer& value_deserializer_);

        /*!
            \brief Destroys the memory storage.
        */
        virtual ~memory_storage();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::size_t base_check_size_impl() const override;

        virtual std::int32_t base_at_impl(std::size_t base_check_index) const override;

        virtual void set_base_at_impl(std::size_t base_check_index, std::int32_t base) override;

        virtual std::uint8_t check_at_impl(std::size_t base_check_index) const override;

        virtual void set_check_at_impl(std::size_t base_check_index, std::uint8_t check) override;

        virtual std::size_t value_count_impl() const override;

        virtual const std::any* value_at_impl(std::size_t value_index) const override;

        virtual void add_value_at_impl(std::size_t value_index, std::any value) override;

        virtual double filling_rate_impl() const override;

        virtual void
        serialize_impl(std::ostream& output_stream, const value_serializer& value_serializer_) const override;

        virtual std::unique_ptr<storage> clone_impl() const override;
    };


}


#endif
