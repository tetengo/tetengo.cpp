/*! \file
    \brief The usage of property saving and loading.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.property.save_load_cpp.hpp"

// [save_load]
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/property_set.hpp>
#include <tetengo/property/storage.hpp>


namespace usage_tetengo::property
{
    const std::filesystem::path& setting_file_path();

    void save_load()
    {
        assert(!std::filesystem::exists(setting_file_path()));

        // Creates separate property sets with the same path for illustration purpose.
        // Uses the one for setting and the other for getting.
        tetengo::property::property_set props_for_setting{ std::make_unique<tetengo::property::file_storage_loader>(),
                                                           "tetengo_property_sample" };
        tetengo::property::property_set props_for_getting{ std::make_unique<tetengo::property::file_storage_loader>(),
                                                           "tetengo_property_sample" };

        // On Windows, by using tetengo::property::windows_registry_storage_loader, the values will be stored into the
        // Windows registry.
        // tetengo::property::property_set props_for_setting{
        //    std::make_unique<tetengo::property::windows_registry_storage_loader>(), "tetengo_property_sample"
        //};
        // tetengo::property::property_set props_for_getting{
        //    std::make_unique<tetengo::property::windows_registry_storage_loader>(), "tetengo_property_sample"
        //};

        // Sets a string value "bar" for the key "foo".
        props_for_setting.set_string("foo", "bar");

        // The value cannot be yet obtained from the property set for getting.
        const auto loaded_value1 = props_for_getting.get_string("foo");
        assert(!loaded_value1);

        // Saves the values of the property set for setting to the file.
        props_for_setting.commit();
        assert(std::filesystem::exists(setting_file_path()));

        // The value cannot be yet obtained from the property set for getting.
        const auto loaded_value2 = props_for_getting.get_string("foo");
        assert(!loaded_value2);

        // Loads the property set values from the file.
        props_for_getting.update();

        // The value can now be obtained from the property set for getting.
        const auto loaded_value3 = props_for_getting.get_string("foo");
        assert(*loaded_value3 == "bar");

        std::filesystem::remove(setting_file_path());
    }

    const std::filesystem::path& setting_file_path()
    {
        // When specifying the path "tetengo_property_sample" (with no path delimiter),
#if defined(_WIN32)
        // in the usual case on Windows, the path of the setting file will be
        // "C:\Users\USER_NAME\AppData\Roaming\tetengo_property_sample\tetengo_property_sample.json".
        static const std::filesystem::path path_{ std::filesystem::path{ std::getenv("APPDATA") } /
                                                  "tetengo_property_sample" / "tetengo_property_sample.json" };
#else
        // in the usual case on UNIX, the path of the setting file will be
        // "/home/USER_NAME/.tetengo_property_sample.json".
        static const std::filesystem::path path_{ std::filesystem::path{ std::getenv("HOME") } /
                                                  ".tetengo_property_sample.json" };
#endif
        return path_;
    }
}
// [save_load]
