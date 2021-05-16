/*! \file
    \brief A json writer.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <filesystem>
#include <iterator>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include "tetengo.property.json_writer.hpp"


namespace tetengo::property
{
    class json_writer::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = json_writer::value_map_type;


        // static functions

        static const json_writer& instance()
        {
            static const json_writer singleton{};
            return singleton;
        }


        // functions

        void write(const value_map_type& value_map, std::ostream& /*stream*/) const
        {
            const auto keys = keys_of(value_map);
        }


    private:
        // static functions

        static std::vector<std::filesystem::path> keys_of(const value_map_type& value_map)
        {
            std::vector<std::filesystem::path> keys{};
            keys.reserve(value_map.size());
            std::transform(std::begin(value_map), std::end(value_map), std::back_inserter(keys), [](const auto& v) {
                return v.first;
            });
            std::sort(std::begin(keys), std::end(keys));
            return keys;
        }
    };


    const json_writer& json_writer::instance()
    {
        return impl::instance();
    }

    json_writer::~json_writer() = default;

    void json_writer::write(const value_map_type& value_map, std::ostream& stream) const
    {
        m_p_impl->write(value_map, stream);
    }

    json_writer::json_writer() : m_p_impl{ std::make_unique<impl>() } {}
}
