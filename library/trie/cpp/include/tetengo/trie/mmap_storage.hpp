/*! \file
    \brief An mmap storage.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_MMAPSTORAGE_HPP)
#define TETENGO_TRIE_MMAPSTORAGE_HPP

#include <any>
#include <cstddef> // IWYU pragma: keep
#include <cstdint>
#include <istream>
#include <memory>

#include <boost/interprocess/file_mapping.hpp> // IWYU pragma: keep

#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/value_serializer.hpp> // IWYU pragma: keep


namespace tetengo::trie
{
    /*!
        \brief An mmap storage.
    */
    class mmap_storage : public storage
    {
    public:
        // functions

        /*!
            \brief Returns the default value cache capacity.

            \return The default value cache capacity.
        */
        static std::size_t default_value_cache_capacity();


        // constructors and destructor

        /*!
            \brief Creates an mmap storage.

            \param file_mapping_        A file mapping.
            \param content_offset       A content offset in the file.
            \param file_size            The file size.
            \param value_deserializer_  A deserializer for value objects.
            \param value_cache_capacity A value cache capacity.

            \throw std::invalid_argument When content_offset is greater than file_size, or the value size is not fixed.
        */
        mmap_storage(
            const boost::interprocess::file_mapping& file_mapping_,
            std::size_t                              content_offset,
            std::size_t                              file_size,
            value_deserializer                       value_deserializer_,
            std::size_t                              value_cache_capacity = default_value_cache_capacity());

        /*!
            \brief Destroys the mmap storage.
        */
        virtual ~mmap_storage();


    private:
        // types

        class impl;


        // variables

        const std::shared_ptr<impl> m_p_impl;


        // constructors

        mmap_storage(const mmap_storage& another);


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
