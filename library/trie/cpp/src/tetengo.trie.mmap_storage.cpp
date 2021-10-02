/*! \file
    \brief A mmap storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <ios>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/interprocess/file_mapping.hpp>

#include <tetengo/trie/mmap_storage.hpp>
#include <tetengo/trie/value_serializer.hpp> // IWYU pragma: keep


namespace tetengo::trie
{
    class storage;


    class mmap_storage::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(const std::filesystem::path& path_, const std::size_t /*offset*/) :
        m_file_mapping{ make_file_mapping(path_) }
        {}


        // functions

        std::int32_t base_at_impl(const std::size_t /*base_check_index*/) const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        void set_base_at_impl(const std::size_t /*base_check_index*/, const std::int32_t /*base*/)
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        std::uint8_t check_at_impl(const std::size_t /*base_check_index*/) const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        void set_check_at_impl(const std::size_t /*base_check_index*/, const std::uint8_t /*check*/)
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        std::size_t size_impl() const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        double filling_rate_impl() const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        const std::vector<std::uint32_t>& base_check_array_impl() const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        const std::any* value_at_impl(const std::size_t /*value_index*/) const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        void add_value_at_impl(const std::size_t /*value_index*/, std::any /*value*/)
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        void serialize_impl(std::ostream& /*output_stream*/, const value_serializer& /*value_serializer_*/) const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }

        std::unique_ptr<storage> clone_impl() const
        {
            assert(false);
            throw std::logic_error{ "Impelment it." };
        }


    private:
        // static functions

        static boost::interprocess::file_mapping make_file_mapping(const std::filesystem::path& path_)
        {
            try
            {
                return boost::interprocess::file_mapping{ path_.c_str(), boost::interprocess::read_only };
            }
            catch (const boost::interprocess::interprocess_exception& e)
            {
                throw std::ios_base::failure{ e.what() };
            }
        }


        // variables

        const boost::interprocess::file_mapping m_file_mapping;
    };


    mmap_storage::mmap_storage(const std::filesystem::path& path_, const std::size_t offset) :
    m_p_impl{ std::make_unique<impl>(path_, offset) }
    {}

    mmap_storage::~mmap_storage() = default;

    std::int32_t mmap_storage::base_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->base_at_impl(base_check_index);
    }

    void mmap_storage::set_base_at_impl(const std::size_t base_check_index, const std::int32_t base)
    {
        m_p_impl->set_base_at_impl(base_check_index, base);
    }

    std::uint8_t mmap_storage::check_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->check_at_impl(base_check_index);
    }

    void mmap_storage::set_check_at_impl(const std::size_t base_check_index, const std::uint8_t check)
    {
        m_p_impl->set_check_at_impl(base_check_index, check);
    }

    std::size_t mmap_storage::size_impl() const
    {
        return m_p_impl->size_impl();
    }

    double mmap_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
    }

    const std::vector<std::uint32_t>& mmap_storage::base_check_array_impl() const
    {
        return m_p_impl->base_check_array_impl();
    }

    const std::any* mmap_storage::value_at_impl(const std::size_t value_index) const
    {
        return m_p_impl->value_at_impl(value_index);
    }

    void mmap_storage::add_value_at_impl(const std::size_t value_index, std::any value)
    {
        return m_p_impl->add_value_at_impl(value_index, std::move(value));
    }

    void mmap_storage::serialize_impl(std::ostream& output_stream, const value_serializer& value_serializer_) const
    {
        m_p_impl->serialize_impl(output_stream, value_serializer_);
    }

    std::unique_ptr<storage> mmap_storage::clone_impl() const
    {
        return m_p_impl->clone_impl();
    }


}
