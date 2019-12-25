/*! \file
    \brief A shared storage.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_SHAREDSTORAGE_HPP)
#define TETENGO_TRIE_SHAREDSTORAGE_HPP

#include <any>
#include <cstdint>
#include <functional>
#include <istream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    /*!
        \brief A shared storage.
    */
    class shared_storage : public storage
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a shared storage.
        */
        shared_storage();

        /*!
            \brief Creates a shared storage.

            \param input_stream        An input stream.
            \param mapped_deserializer A deserializer for mapped objects.
        */
        explicit shared_storage(
            std::istream&                                           input_stream,
            const std::function<std::any(const std::string_view&)>& mapped_deserializer);

        /*!
            \brief Destroys the shared storage.
        */
        virtual ~shared_storage();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::int32_t base_at_impl(std::size_t base_check_index) const override;

        virtual void set_base_at_impl(std::size_t base_check_index, std::int32_t value) override;

        virtual std::uint8_t check_at_impl(std::size_t base_check_index) const override;

        virtual void set_check_at_impl(std::size_t base_check_index, std::uint8_t value) override;

        virtual double filling_rate_impl() const override;

        virtual const std::vector<std::uint32_t>& base_check_array_impl() const override;

        virtual const std::any* mapped_at_impl(std::size_t mapped_index) const override;

        virtual void add_mapped_at_impl(std::size_t mapped_index, std::any mapped) override;

        virtual void serialize_impl(
            std::ostream&                                      output_stream,
            const std::function<std::string(const std::any&)>& mapped_serializer) const override;

        virtual std::unique_ptr<storage> clone_impl() const override;
    };


}


#endif
