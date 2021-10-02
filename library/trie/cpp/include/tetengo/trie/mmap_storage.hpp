/*! \file
    \brief A mmap storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_MMAPSTORAGE_HPP)
#define TETENGO_TRIE_MMAPSTORAGE_HPP

#include <any>
#include <cstdint>
#include <filesystem>
#include <istream>
#include <memory>
#include <vector>

#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    class value_serializer;


    /*!
        \brief A mmap storage.
    */
    class mmap_storage : public storage
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a mmap storage.

            \param path_   A path.
            \param offset  An content offset in the file of the path.

            \throw std::ios_base::failure When path_ cannot be opened.
        */
        mmap_storage(const std::filesystem::path& path_, std::size_t offset);

        /*!
            \brief Destroys the mmap storage.
        */
        virtual ~mmap_storage();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::int32_t base_at_impl(std::size_t base_check_index) const override;

        virtual void set_base_at_impl(std::size_t base_check_index, std::int32_t base) override;

        virtual std::uint8_t check_at_impl(std::size_t base_check_index) const override;

        virtual void set_check_at_impl(std::size_t base_check_index, std::uint8_t check) override;

        virtual std::size_t size_impl() const override;

        virtual double filling_rate_impl() const override;

        virtual const std::vector<std::uint32_t>& base_check_array_impl() const override;

        virtual const std::any* value_at_impl(std::size_t value_index) const override;

        virtual void add_value_at_impl(std::size_t value_index, std::any value) override;

        virtual void
        serialize_impl(std::ostream& output_stream, const value_serializer& value_serializer_) const override;

        virtual std::unique_ptr<storage> clone_impl() const override;
    };


}


#endif
