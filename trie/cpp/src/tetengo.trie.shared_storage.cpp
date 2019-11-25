/*! \file
    \brief A shared storage.

    Copyright (C) 2019 kaoru
*/

#include <cstdint>
#include <istream>
#include <memory>
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

        std::size_t size_impl() const
        {
            return m_p_entity->size();
        }

        double filling_rate_impl() const
        {
            return m_p_entity->filling_rate();
        }

        const std::vector<std::uint32_t>& base_check_array_impl() const
        {
            return m_p_entity->base_check_array();
        }

        void serialize_impl(std::ostream& output_stream) const
        {
            m_p_entity->serialize(output_stream);
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

    std::size_t shared_storage::size_impl() const
    {
        return m_p_impl->size_impl();
    }

    double shared_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
    }

    const std::vector<std::uint32_t>& shared_storage::base_check_array_impl() const
    {
        return m_p_impl->base_check_array_impl();
    }

    void shared_storage::serialize_impl(std::ostream& output_stream) const
    {
        m_p_impl->serialize_impl(output_stream);
    }

    std::unique_ptr<storage> shared_storage::clone_impl() const
    {
        return m_p_impl->clone_impl();
    }


}
