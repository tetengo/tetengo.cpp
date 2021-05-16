/*! \file
    \brief A file storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/comment_removing_reader.hpp>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/line_counting_reader.hpp>
#include <tetengo/json/stream_reader.hpp>
#include <tetengo/platform_dependent/propertyX.hpp>
#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/storage.hpp>

#include "tetengo.property.json_parser.hpp"


namespace tetengo::property
{
    class file_storage::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = file_storage::value_map_type;


        // constructors and destructor

        explicit impl(const std::filesystem::path path) : m_path{ path } {}


        // functions

        void save_impl(const file_storage& self) const
        {
            [[maybe_unused]] const auto& value_map = self.value_map();
        }


    private:
        // variables

        const std::filesystem::path m_path;
    };


    file_storage::file_storage(value_map_type value_map, const std::filesystem::path path) :
    storage{ std::move(value_map) },
        m_p_impl{ std::make_unique<impl>(path) }
    {}

    file_storage::~file_storage() = default;

    void file_storage::save_impl() const
    {
        m_p_impl->save_impl(*this);
    }


    class file_storage_loader::impl : private boost::noncopyable
    {
    public:
        // constructors

        impl() : m_master_value_map{} {}


        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path& path) const
        {
            const auto native_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_path(path);
            return std::make_unique<file_storage>(load_value_map(native_path), path);
        }


    private:
        // types

        using value_map_type = storage::value_map_type;


        // static functions

        static value_map_type load_value_map(const std::filesystem::path& native_path)
        {
            auto p_stream = std::make_unique<std::ifstream>(native_path);
            if (!p_stream || !*p_stream)
            {
                return value_map_type{};
            }

            auto p_parser = build_json_parser(std::move(p_stream));
            try
            {
                return json_parser::instance().parse(*p_parser);
            }
            catch (const std::runtime_error&)
            {
                return value_map_type{};
            }
        }

        static std::unique_ptr<tetengo::json::json_parser> build_json_parser(std::unique_ptr<std::istream>&& p_stream)
        {
            auto p_stream_reader = std::make_unique<tetengo::json::stream_reader>(std::move(p_stream));
            auto p_line_counting_reader =
                std::make_unique<tetengo::json::line_counting_reader>(std::move(p_stream_reader));
            auto p_comment_removing_reader =
                std::make_unique<tetengo::json::comment_removing_reader>(std::move(p_line_counting_reader), "#");
            return std::make_unique<tetengo::json::json_parser>(std::move(p_comment_removing_reader));
        }


        // variables

        mutable storage::value_map_type m_master_value_map;
    };


    file_storage_loader::file_storage_loader() : m_p_impl{ std::make_unique<impl>() } {}

    file_storage_loader::~file_storage_loader() = default;

    std::unique_ptr<storage> file_storage_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
