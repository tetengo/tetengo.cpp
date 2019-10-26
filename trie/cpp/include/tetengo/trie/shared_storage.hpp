/*! \file
    \brief A shared storage.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_SHAREDSTORAGE_HPP)
#define TETENGO_TRIE_SHAREDSTORAGE_HPP

#include <cstdint>
#include <istream>
#include <memory>
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

            \param input_stream An input stream.
        */
        explicit shared_storage(std::istream& input_stream);

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

        virtual std::int32_t base_at_impl(std::size_t index) const override;

        virtual void set_base_at_impl(std::size_t index, std::int32_t value) override;

        virtual std::uint8_t check_at_impl(std::size_t index) const override;

        virtual void set_check_at_impl(std::size_t index, std::uint8_t value) override;

        virtual std::size_t size_impl() const override;

        virtual double filling_rate_impl() const override;

        virtual const std::vector<std::uint32_t>& values_impl() const override;

        virtual void serialize_impl(std::ostream& output_stream) const override;

        virtual std::unique_ptr<storage> clone_impl() const override;
    };


}


#endif