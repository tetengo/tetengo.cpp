/*! \file
    \brief A shared storage.

    Copyright (C) 2019 kaoru
*/

#include <any>
#include <cstdint>
#include <istream>
#include <memory>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/shared_storage.hpp>
#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    class shared_storage::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() : m_p_entity{ std::make_shared<memory_storage>() } {};

        explicit impl(std::istream& input_stream) : m_p_entity{ std::make_shared<memory_storage>(input_stream) } {};


        // functions

        std::int32_t base_at_impl(const std::size_t base_check_index) const
        {
            return m_p_entity->base_at(base_check_index);
        }

        void set_base_at_impl(const std::size_t base_check_index, const std::int32_t value)
        {
            m_p_entity->set_base_at(base_check_index, value);
        }

        std::uint8_t check_at_impl(const std::size_t base_check_index) const
        {
            return m_p_entity->check_at(base_check_index);
        }

        void set_check_at_impl(const std::size_t base_check_index, const std::uint8_t value)
        {
            m_p_entity->set_check_at(base_check_index, value);
        }

        double filling_rate_impl() const
        {
            return m_p_entity->filling_rate();
        }

        const std::vector<std::uint32_t>& base_check_array_impl() const
        {
            return m_p_entity->base_check_array();
        }

        const std::any* mapped_at_impl(const std::size_t mapped_index) const
        {
            return m_p_entity->mapped_at(mapped_index);
        }

        void add_mapped_at_impl(const std::size_t mapped_index, std::any mapped)
        {
            m_p_entity->add_mapped_at(mapped_index, std::move(mapped));
        }

        void
        serialize_impl(std::ostream& output_stream, const std::function<std::string(const std::any&)>& serializer) const
        {
            m_p_entity->serialize(output_stream, serializer);
        }

        std::unique_ptr<storage> clone_impl() const
        {
            auto p_clone = std::make_unique<shared_storage>();
            p_clone->m_p_impl->m_p_entity = m_p_entity;
            return p_clone;
        }


    private:
        // variables

        std::shared_ptr<storage> m_p_entity;
    };


    shared_storage::shared_storage() : m_p_impl{ std::make_unique<impl>() } {}

    shared_storage::shared_storage(std::istream& input_stream) : m_p_impl{ std::make_unique<impl>(input_stream) } {}

    shared_storage::~shared_storage() = default;

    std::int32_t shared_storage::base_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->base_at_impl(base_check_index);
    }

    void shared_storage::set_base_at_impl(const std::size_t base_check_index, const std::int32_t value)
    {
        m_p_impl->set_base_at_impl(base_check_index, value);
    }

    std::uint8_t shared_storage::check_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->check_at_impl(base_check_index);
    }

    void shared_storage::set_check_at_impl(const std::size_t base_check_index, const std::uint8_t value)
    {
        m_p_impl->set_check_at_impl(base_check_index, value);
    }

    double shared_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
    }

    const std::vector<std::uint32_t>& shared_storage::base_check_array_impl() const
    {
        return m_p_impl->base_check_array_impl();
    }

    const std::any* shared_storage::mapped_at_impl(const std::size_t mapped_index) const
    {
        return m_p_impl->mapped_at_impl(mapped_index);
    }

    void shared_storage::add_mapped_at_impl(const std::size_t mapped_index, std::any mapped)
    {
        return m_p_impl->add_mapped_at_impl(mapped_index, std::move(mapped));
    }

    void shared_storage::serialize_impl(
        std::ostream&                                      output_stream,
        const std::function<std::string(const std::any&)>& serializer) const
    {
        m_p_impl->serialize_impl(output_stream, serializer);
    }

    std::unique_ptr<storage> shared_storage::clone_impl() const
    {
        return m_p_impl->clone_impl();
    }


}
