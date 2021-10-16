/*! \file
    \brief An mmap storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <ios>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/mmap_storage.hpp>
#include <tetengo/trie/value_serializer.hpp> // IWYU pragma: keep


namespace
{
    class value_cache : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit value_cache(const std::size_t cache_capacity) :
        m_cache_capacity{ cache_capacity },
        m_access_orders{},
        m_map{}
        {}


        // functions

        bool has(const std::size_t index) const
        {
            return m_map.find(index) != std::end(m_map);
        }

        const std::any* at(const std::size_t index) const
        {
            auto& value = at_impl(index);

            m_access_orders.erase(value.first);
            m_access_orders.push_front(index);
            value.first = std::begin(m_access_orders);

            return value.second ? &*value.second : nullptr;
        }

        void insert(const std::size_t index, std::optional<std::any> o_value)
        {
            assert(!has(index));

            while (std::size(m_access_orders) >= m_cache_capacity)
            {
                const auto oldest_index = m_access_orders.back();
                m_access_orders.pop_back();
                m_map.erase(oldest_index);
            }

            m_access_orders.push_front(index);
            m_map.insert(std::make_pair(index, std::make_pair(std::begin(m_access_orders), std::move(o_value))));
            assert(std::size(m_access_orders) == std::size(m_map));
        }


    private:
        // types

        using access_order_list_type = std::list<std::size_t>;

        using map_value_type = std::pair<access_order_list_type::const_iterator, std::optional<std::any>>;

        using map_type = std::unordered_map<std::size_t, map_value_type>;


        // variables

        const std::size_t m_cache_capacity;

        mutable access_order_list_type m_access_orders;

        mutable map_type m_map;


        // functions

        map_value_type& at_impl(const std::size_t index) const
        {
            const auto found = m_map.find(index);
            assert(found != std::end(m_map));
            return found->second;
        }
    };


}


namespace tetengo::trie
{
    class storage;


    class mmap_storage::impl : private boost::noncopyable
    {
    public:
        // static functions

        static std::size_t default_value_cache_capacity()
        {
            return 10000;
        }


        // constructors and destructor

        impl(
            const std::filesystem::path& path_,
            const std::size_t            offset,
            value_deserializer           value_deserializer_,
            const std::size_t            value_cache_capacity) :
        m_file_mapping{ make_file_mapping(path_) },
        m_content_offset{ offset },
        m_value_deserializer{ std::move(value_deserializer_) },
        m_value_cache{ value_cache_capacity }
        {}


        // functions

        std::size_t base_check_size_impl() const
        {
            const auto base_check_count = read_uint32(0);
            return base_check_count;
        }

        std::int32_t base_at_impl(const std::size_t base_check_index) const
        {
            const auto base_check = read_uint32(sizeof(std::uint32_t) * (1 + base_check_index));
            return static_cast<std::int32_t>(base_check) >> 8;
        }

        void set_base_at_impl(const std::size_t /*base_check_index*/, const std::int32_t /*base*/)
        {
            throw std::logic_error{ "Unsupported operation." };
        }

        std::uint8_t check_at_impl(const std::size_t base_check_index) const
        {
            const auto base_check = read_uint32(sizeof(std::uint32_t) * (1 + base_check_index));
            return base_check & 0xFF;
        }

        void set_check_at_impl(const std::size_t /*base_check_index*/, const std::uint8_t /*check*/)
        {
            throw std::logic_error{ "Unsupported operation." };
        }

        std::size_t value_size_impl() const
        {
            const auto base_check_count = base_check_size_impl();
            const auto size = read_uint32(sizeof(std::uint32_t) * (1 + base_check_count));
            return size;
        }

        const std::any* value_at_impl(const std::size_t value_index) const
        {
            if (!m_value_cache.has(value_index))
            {
                const auto base_check_count = base_check_size_impl();
                const auto fixed_value_size = read_uint32(sizeof(std::uint32_t) * (1 + base_check_count + 1));
                const auto offset = sizeof(std::uint32_t) * (1 + base_check_count + 2) + fixed_value_size * value_index;
                const auto serialized = read_bytes(offset, fixed_value_size);
                if (serialized == std::vector<char>(fixed_value_size, uninitialized_byte()))
                {
                    m_value_cache.insert(value_index, std::nullopt);
                }
                else
                {
                    auto value = m_value_deserializer(serialized);
                    m_value_cache.insert(value_index, std::move(value));
                }
            }
            return m_value_cache.at(value_index);
        }

        void add_value_at_impl(const std::size_t /*value_index*/, std::any /*value*/)
        {
            throw std::logic_error{ "Unsupported operation." };
        }

        double filling_rate_impl() const
        {
            const auto base_check_count = base_check_size_impl();
            auto       empty_count = static_cast<std::uint32_t>(0);
            for (auto i = static_cast<std::size_t>(0); i < base_check_count; ++i)
            {
                const auto base_check = read_uint32(sizeof(std::uint32_t) * (1 + i));
                if (base_check == 0x000000FF)
                {
                    ++empty_count;
                }
            }
            return static_cast<double>(empty_count) / base_check_count;
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

        static constexpr char uninitialized_byte()
        {
            return static_cast<char>(0xFF);
        }


        // variables

        const boost::interprocess::file_mapping m_file_mapping;

        const std::size_t m_content_offset;

        const value_deserializer m_value_deserializer;

        mutable value_cache m_value_cache;


        // functions

        std::vector<char> read_bytes(const std::size_t offset, const std::size_t size) const
        {
            const boost::interprocess::mapped_region region{ m_file_mapping,
                                                             boost::interprocess::read_only,
                                                             static_cast<boost::interprocess::offset_t>(
                                                                 m_content_offset + offset),
                                                             size };
            return std::vector<char>{ reinterpret_cast<const char*>(region.get_address()),
                                      reinterpret_cast<const char*>(region.get_address()) + region.get_size() };
        }

        std::uint32_t read_uint32(const std::size_t offset) const
        {
            static const default_deserializer<std::uint32_t> uint32_deserializer{ false };
            return uint32_deserializer(read_bytes(offset, sizeof(std::uint32_t)));
        }
    };


    std::size_t mmap_storage::default_value_cache_capacity()
    {
        return impl::default_value_cache_capacity();
    }

    mmap_storage::mmap_storage(
        const std::filesystem::path& path_,
        const std::size_t            offset,
        value_deserializer           value_deserializer_,
        const std::size_t            value_cache_capacity /*= default_value_cache_capacity()*/) :
    m_p_impl{ std::make_unique<impl>(path_, offset, std::move(value_deserializer_), value_cache_capacity) }
    {}

    mmap_storage::~mmap_storage() = default;

    std::size_t mmap_storage::base_check_size_impl() const
    {
        return m_p_impl->base_check_size_impl();
    }

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

    std::size_t mmap_storage::value_size_impl() const
    {
        return m_p_impl->value_size_impl();
    }

    const std::any* mmap_storage::value_at_impl(const std::size_t value_index) const
    {
        return m_p_impl->value_at_impl(value_index);
    }

    void mmap_storage::add_value_at_impl(const std::size_t value_index, std::any value)
    {
        return m_p_impl->add_value_at_impl(value_index, std::move(value));
    }

    double mmap_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
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
