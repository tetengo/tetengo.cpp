/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class memory_storage::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = memory_storage::value_map_type;


        // constructors and destructor

        explicit impl(value_map_type& master_value_map) : m_master_value_map{ master_value_map } {}


        // functions

        void save_impl(const memory_storage& self) const
        {
            m_master_value_map = self.value_map();
        }


    private:
        // variables

        value_map_type& m_master_value_map;
    };


    memory_storage::memory_storage(value_map_type& master_value_map) :
    storage{ master_value_map },
        m_p_impl{ std::make_unique<impl>(master_value_map) }
    {}

    memory_storage::~memory_storage() = default;

    void memory_storage::save_impl() const
    {
        m_p_impl->save_impl(*this);
    }


    class memory_storage_loader::impl : private boost::noncopyable
    {
    public:
        // constructors

        impl() : m_master_value_map_map{} {}


        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path& path) const
        {
            auto found = m_master_value_map_map.find(path.string());
            if (found == std::end(m_master_value_map_map))
            {
                const auto inserted =
                    m_master_value_map_map.insert(std::make_pair(path.string(), storage::value_map_type{}));
                found = inserted.first;
            }
            return std::make_unique<memory_storage>(found->second);
        }


    private:
        // variables

        mutable std::unordered_map<std::string, storage::value_map_type> m_master_value_map_map;
    };


    memory_storage_loader::memory_storage_loader() : m_p_impl{ std::make_unique<impl>() } {}

    memory_storage_loader::~memory_storage_loader() = default;

    std::unique_ptr<storage> memory_storage_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
