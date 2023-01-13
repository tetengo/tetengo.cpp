/*! \file
    \brief A trie.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <algorithm> // IWYU pragma: keep
#include <any>
#include <cstddef> // IWYU pragma: keep
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <functional>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/stl_interfaces/iterator_interface.hpp> // IWYU pragma: keep
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>
#include <tetengo/trie/trie_iterator.hpp>
#include <tetengo/trie/value_serializer.hpp>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    static const std::string kumamoto1{ 0xE7_c, 0x86_c, 0x8A_c, 0x6_c, 0x9C_c, 0xAC_c }; // Kumamoto in Kanji in UTF-8

    static const std::string tamana1{ 0xE7_c, 0x8E_c, 0x89_c, 0xE5_c, 0x90_c, 0x8D_c }; // Tamana in Kanji in UTF-8

    static const std::string tamarai1{ 0xE7_c, 0x8E_c, 0x89_c, 0xE6_c, 0x9D_c, 0xA5_c }; // Tamarai in Kanji in UTF-8

    static const std::wstring kumamoto2{ 0x718A, 0x672C }; // Kumamoto in Kanji in UTF-16/32

    static const std::wstring tamana2{ 0x7389, 0x540D }; // Tamana in Kanji in UTF-16/32

    static const std::wstring tamarai2{ 0x7389, 0x6765 }; // Tamarai in Kanji in UTF-16/32

    static const std::wstring tama2{ 0x7389 }; // Tama in Kanji in UTF-16/32

    static const std::wstring uto2{ 0x5B87, 0x571F }; // Uto in Kanji in UTF-16/32

    const std::vector<char> serialized{
        // clang-format off
        // base check array
        0x00_c, 0x00_c, 0x00_c, 0x0B_c, 
        0xFF_c, 0xFF_c, 0x90_c, 0xFF_c, 
        0xFF_c, 0xFF_c, 0x78_c, 0x71_c, 
        0xFF_c, 0xFF_c, 0x9D_c, 0x8A_c, 
        0xFF_c, 0xFF_c, 0x7E_c, 0x73_c, 
        0xFF_c, 0xFF_c, 0xD9_c, 0x67_c, 
        0x00_c, 0x00_c, 0x06_c, 0x2C_c, 
        0x00_c, 0x00_c, 0x00_c, 0x00_c, 
        0xFF_c, 0xFF_c, 0xB4_c, 0x89_c, 
        0xFF_c, 0xFF_c, 0xFC_c, 0x54_c, 
        0x00_c, 0x00_c, 0x0A_c, 0x0D_c, 
        0x00_c, 0x00_c, 0x01_c, 0x00_c, 

        // value array
        0x00_c, 0x00_c, 0x00_c, 0x02_c, 
        0x00_c, 0x00_c, 0x00_c, 0x00_c, 
        0x00_c, 0x00_c, 0x00_c, 0x06_c, 
        0xE7_c, 0x86_c, 0x8A_c, 0x06_c, 0x9C_c, 0xAC_c,
        0x00_c, 0x00_c, 0x00_c, 0x06_c,
        0xE7_c, 0x8E_c, 0x89_c, 0xE5_c, 0x90_c, 0x8D_c,
        // clang-format on
    };

    std::unique_ptr<std::istream> create_input_stream()
    {
        return std::make_unique<std::stringstream>(std::string{ std::begin(serialized), std::end(serialized) });
    }

    void adding_observer(const char* const key, void* const p_context)
    {
        std::vector<std::string>* const p_added_serialized_keys = static_cast<std::vector<std::string>*>(p_context);
        p_added_serialized_keys->push_back(key);
    }

    void done_observer(void* const p_context)
    {
        bool* const p_done = static_cast<bool*>(p_context);
        *p_done = true;
    }

    const std::vector<char> serialized_c_if{
        // clang-format off
        0x00_c, 0x00_c, 0x00_c, 0x11_c,
        0xFF_c, 0xFF_c, 0xB6_c, 0xFF_c,
        0xFF_c, 0xFF_c, 0x8D_c, 0x4B_c,
        0xFF_c, 0xFF_c, 0x96_c, 0x75_c,
        0xFF_c, 0xFF_c, 0xA3_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0x98_c, 0x61_c,
        0xFF_c, 0xFF_c, 0x97_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0x93_c, 0x6F_c,
        0xFF_c, 0xFF_c, 0x99_c, 0x74_c,
        0x00_c, 0x00_c, 0x09_c, 0x6F_c,
        0x00_c, 0x00_c, 0x00_c, 0x00_c,
        0xFF_c, 0xFF_c, 0xAA_c, 0x54_c,
        0xFF_c, 0xFF_c, 0x9F_c, 0x61_c,
        0xFF_c, 0xFF_c, 0xAC_c, 0x6D_c,
        0xFF_c, 0xFF_c, 0xA0_c, 0x61_c,
        0xFF_c, 0xFF_c, 0xAE_c, 0x6E_c,
        0x00_c, 0x00_c, 0x10_c, 0x61_c,
        0x00_c, 0x00_c, 0x01_c, 0x00_c,
        0x00_c, 0x00_c, 0x00_c, 0x02_c,
        0x00_c, 0x00_c, 0x00_c, 0x04_c,
        0x2A_c, 0x00_c, 0x00_c, 0x00_c,
        0x18_c, 0x00_c, 0x00_c, 0x00_c,
        // clang-format on
    };

    std::filesystem::path temporary_file_path(const std::vector<char>& initial_content = std::vector<char>{})
    {
        const auto path = std::filesystem::temp_directory_path() / "test_tetengo.trie.trie";

        {
            std::ofstream stream{ path, std::ios_base::binary };
            stream.write(std::data(initial_content), std::size(initial_content));
        }

        return path;
    }

    bool& copy_detecting()
    {
        static bool singleton = false;
        return singleton;
    }

    void begin_copy_detection()
    {
        copy_detecting() = true;
    }

    void end_copy_detection()
    {
        copy_detecting() = false;
    }

    template <typename T>
    struct copy_detector
    {
        T value;

        explicit copy_detector(const T& value_) : value{ value_ } {}

        explicit copy_detector(T&& value_) : value{ std::move(value_) } {}

        copy_detector(const copy_detector& another) : value{ another.value }
        {
            if (copy_detecting())
            {
                BOOST_CHECK_MESSAGE(false, "Object copy detected.");
            }
        }

        copy_detector(copy_detector&& another) noexcept : value{ std::move(another.value) } {}
    };

    template <typename T>
    copy_detector<T> detect_copy(const T& value)
    {
        return copy_detector<T>{ value };
    }

    template <typename T>
    copy_detector<T> detect_copy(T&& value)
    {
        return copy_detector<T>{ std::move(value) };
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(trie)


BOOST_AUTO_TEST_CASE(null_building_observer_set)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& observer_set = tetengo::trie::trie<std::string, int>::null_building_observer_set();

    tetengo_trie_trie_nullAddingObserver("hoge", nullptr);
    tetengo_trie_trie_nullDoneObserver(nullptr);
}

BOOST_AUTO_TEST_CASE(default_double_array_density_factor)
{
    BOOST_TEST_PASSPOINT();

    {
        using trie_type = tetengo::trie::trie<std::string, int>;
        BOOST_TEST(
            trie_type::default_double_array_density_factor() == tetengo::trie::double_array::default_density_factor());
    }
    {
        BOOST_TEST(
            tetengo_trie_trie_defaultDoubleArrayDensityFactor() ==
            tetengo::trie::double_array::default_density_factor());
    }
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::string, int> trie_{};
    }
    {
        const tetengo::trie::trie<std::string, int> trie_{ tetengo::trie::default_serializer<std::string>{ true } };
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 }, { "Tamana", 24 } };
    }
    {
        const tetengo::trie::trie<std::string, int> trie_{ { "Kumamoto", 42 }, { "Tamana", 24 } };
    }
    {
        const tetengo::trie::trie<std::string, int> trie_{ { kumamoto1, 42 }, { tamana1, 24 } };
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };
    }
    {
        std::vector<std::string>                                          added_serialized_keys{};
        auto                                                              done = false;
        tetengo::trie::trie<std::string, int>::building_observer_set_type observer{
            [&added_serialized_keys](const std::string_view& serialized_key) {
                added_serialized_keys.emplace_back(serialized_key);
            },
            [&done]() { done = true; }
        };
        const tetengo::trie::trie<std::string, int> trie_{
            { { "Kumamoto", 42 }, { "Tamana", 24 } },
            tetengo::trie::default_serializer<std::string>{ true },
            observer,
            tetengo::trie::trie<std::string, int>::default_double_array_density_factor()
        };

        static const tetengo::trie::default_deserializer<std::string> key_deserializer{ true };
        BOOST_TEST_REQUIRE(std::size(added_serialized_keys) == 2U);
        BOOST_TEST(key_deserializer(added_serialized_keys[0]) == "Kumamoto");
        BOOST_TEST(key_deserializer(added_serialized_keys[1]) == "Tamana");
        BOOST_TEST(done);
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::begin(content), std::end(content) };
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };
    }
    {
        std::unordered_map<std::string_view, std::string> content{ { "Kumamoto", kumamoto1 }, { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{
            std::begin(content), std::end(content), tetengo::trie::default_serializer<std::string_view>{ true }
        };
    }
    {
        auto                                    p_input_stream = create_input_stream();
        const tetengo::trie::value_deserializer value_deserializer_{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::string>string_deserializer{ false };
            return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
        } };
        auto p_storage = std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, value_deserializer_);
        const tetengo::trie::trie<std::string, std::string> trie_{ std::move(p_storage) };
    }
    {
        auto                                    p_input_stream = create_input_stream();
        const tetengo::trie::value_deserializer value_deserializer_{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::string>string_deserializer{ false };
            return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
        } };
        auto p_storage = std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, value_deserializer_);
        const tetengo::trie::trie<std::string, std::string> trie_{
            std::move(p_storage), tetengo::trie::default_serializer<std::string>{ true }
        };
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        std::vector<std::string> added_serialized_keys{};
        auto                     done = false;
        const auto* const        p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            adding_observer,
            &added_serialized_keys,
            done_observer,
            &done,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST_REQUIRE(std::size(added_serialized_keys) == 2U);
        BOOST_TEST(added_serialized_keys[0] == "Kumamoto");
        BOOST_TEST(added_serialized_keys[1] == "Tamana");
        BOOST_TEST(done);
    }
    {
        const auto file_path = temporary_file_path(serialized_c_if);
        BOOST_SCOPE_EXIT(&file_path)
        {
            std::filesystem::remove(file_path);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createMemoryStorage(file_path.c_str());
        BOOST_TEST(tetengo_trie_storage_valueCount(p_storage) == 2U);

        const auto* const p_trie = tetengo_trie_trie_createWithStorage(p_storage);
        BOOST_SCOPE_EXIT(&p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_trie);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        std::vector<std::string> added_serialized_keys{};
        auto                     done = false;
        const auto* const        p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            adding_observer,
            &added_serialized_keys,
            done_observer,
            &done,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        auto* const p_storage = tetengo_trie_storage_createStorage(p_trie);

        const auto* const p_trie2 = tetengo_trie_trie_createWithStorage(p_storage);
        BOOST_TEST(!p_trie2);
    }
    {
        std::vector<std::string> added_serialized_keys{};
        auto                     done = false;
        const auto* const        p_trie = tetengo_trie_trie_create(
            nullptr,
            0,
            sizeof(int),
            adding_observer,
            &added_serialized_keys,
            done_observer,
            &done,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());

        BOOST_TEST(p_trie);
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;
    }
    {
        std::vector<std::string> added_serialized_keys{};
        auto                     done = false;
        const auto* const        p_trie = tetengo_trie_trie_create(
            nullptr,
            1,
            sizeof(int),
            adding_observer,
            &added_serialized_keys,
            done_observer,
            &done,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());

        BOOST_TEST(!p_trie);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        std::vector<std::string> added_serialized_keys{};
        auto                     done = false;
        const auto* const        p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            nullptr,
            &added_serialized_keys,
            done_observer,
            &done,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());

        BOOST_TEST(!p_trie);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        std::vector<std::string> added_serialized_keys{};
        auto                     done = false;
        const auto* const        p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            adding_observer,
            &added_serialized_keys,
            nullptr,
            &done,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());

        BOOST_TEST(!p_trie);
    }
    {
        const auto* const p_trie2 = tetengo_trie_trie_createWithStorage(nullptr);
        BOOST_TEST(!p_trie2);
    }
}

BOOST_AUTO_TEST_CASE(empty)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        BOOST_TEST(std::empty(trie_));
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 } };

        BOOST_TEST(!std::empty(trie_));
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        BOOST_TEST(!std::empty(trie_));
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        BOOST_TEST(std::empty(trie_));
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        BOOST_TEST(!std::empty(trie_));
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(std::empty(trie_));
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(!std::empty(trie_));
    }

    {
        const auto* const p_trie = tetengo_trie_trie_create(
            nullptr,
            0,
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(tetengo_trie_trie_empty(p_trie));
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(!tetengo_trie_trie_empty(p_trie));
    }
    {
        BOOST_TEST(!tetengo_trie_trie_empty(nullptr));
    }
}

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        BOOST_TEST(std::size(trie_) == 0U);
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 } };

        BOOST_TEST(std::size(trie_) == 1U);
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        BOOST_TEST(std::size(trie_) == 2U);
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        BOOST_TEST(std::size(trie_) == 0U);
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        BOOST_TEST(std::size(trie_) == 1U);
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(std::size(trie_) == 0U);
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(std::size(trie_) == 2U);
    }

    {
        const auto* const p_trie = tetengo_trie_trie_create(
            nullptr,
            0,
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(tetengo_trie_trie_size(p_trie) == 0U);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(tetengo_trie_trie_size(p_trie) == 1U);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(tetengo_trie_trie_size(p_trie) == 2U);
    }
    {
        BOOST_TEST(tetengo_trie_trie_size(nullptr) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(contains)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        BOOST_TEST(!trie_.contains(kumamoto2));
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };
        begin_copy_detection();

        BOOST_TEST(trie_.contains(kumamoto2));
        BOOST_TEST(trie_.contains(tamana2));
        BOOST_TEST(!trie_.contains(uto2));

        end_copy_detection();
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        BOOST_TEST(!trie_.contains("Kumamoto"));
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        BOOST_TEST(trie_.contains("Kumamoto"));
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(!trie_.contains("Kumamoto"));
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(trie_.contains("Kumamoto"));
    }

    {
        const auto* const p_trie = tetengo_trie_trie_create(
            nullptr,
            0,
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(!tetengo_trie_trie_contains(p_trie, "Kumamoto"));
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(tetengo_trie_trie_contains(p_trie, "Kumamoto"));
        BOOST_TEST(tetengo_trie_trie_contains(p_trie, "Tamana"));
        BOOST_TEST(!tetengo_trie_trie_contains(p_trie, "Uto"));
    }
    {
        BOOST_TEST(!tetengo_trie_trie_contains(nullptr, "Kumamoto"));
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(!tetengo_trie_trie_contains(p_trie, nullptr));
    }
}

BOOST_AUTO_TEST_CASE(find)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        const auto* const p_found = trie_.find(kumamoto2);
        BOOST_TEST(!p_found);
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };
        begin_copy_detection();

        {
            const auto* const p_found = trie_.find(kumamoto2);
            BOOST_REQUIRE(p_found);
            BOOST_TEST(p_found->value == kumamoto1);
        }
        {
            const auto* const p_found = trie_.find(tamana2);
            BOOST_REQUIRE(p_found);
            BOOST_TEST(p_found->value == tamana1);
        }
        {
            const auto* const p_found = trie_.find(uto2);
            BOOST_TEST(!p_found);
        }

        end_copy_detection();
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        const auto* const p_found = trie_.find("Kumamoto");
        BOOST_TEST(!p_found);
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        const auto* const p_found = trie_.find("Kumamoto");
        BOOST_REQUIRE(p_found);
        BOOST_TEST(*p_found == 42);
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        const auto* const p_found = trie_.find("Kumamoto");
        BOOST_TEST(!p_found);
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        const auto* const p_found = trie_.find("Kumamoto");
        BOOST_REQUIRE(p_found);
        BOOST_TEST(*p_found == kumamoto1);
    }

    {
        const auto* const p_trie = tetengo_trie_trie_create(
            nullptr,
            0,
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_found = tetengo_trie_trie_find(p_trie, "Kumamoto");
        BOOST_TEST(!p_found);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_found = tetengo_trie_trie_find(p_trie, "Kumamoto");
        BOOST_REQUIRE(p_found);
        BOOST_TEST(*static_cast<const int*>(p_found) == kumamoto_value);
    }
    {
        const auto* const p_found = tetengo_trie_trie_find(nullptr, "Kumamoto");
        BOOST_TEST(!p_found);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_found = tetengo_trie_trie_find(p_trie, nullptr);
        BOOST_TEST(!p_found);
    }
}

BOOST_AUTO_TEST_CASE(begin_end)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        const auto first = std::begin(trie_);
        const auto last = std::end(trie_);
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        const auto first = std::begin(trie_);
        const auto last = std::end(trie_);
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        const auto first = std::begin(trie_);
        const auto last = std::end(trie_);
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        const auto first = std::begin(trie_);
        const auto last = std::end(trie_);
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        const auto first = std::begin(trie_);
        const auto last = std::end(trie_);
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        const auto first = std::begin(trie_);
        const auto last = std::end(trie_);
    }

    {
        const auto* const p_trie = tetengo_trie_trie_create(
            nullptr,
            0,
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_iterator = tetengo_trie_trie_createIterator(p_trie);
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_trie_trie_destroyIterator(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_iterator);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_iterator = tetengo_trie_trie_createIterator(p_trie);
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_trie_trie_destroyIterator(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_iterator);
    }
    {
        const auto* const p_iterator = tetengo_trie_trie_createIterator(nullptr);
        BOOST_TEST(!p_iterator);
    }
}

BOOST_AUTO_TEST_CASE(subtrie)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{};

        const auto p_subtrie = trie_.subtrie(tama2);
        BOOST_CHECK(!p_subtrie);
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{ { kumamoto2,
                                                                                     detect_copy(kumamoto1) } };

        const auto p_subtrie = trie_.subtrie(tama2);
        BOOST_CHECK(!p_subtrie);
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };

        const auto p_subtrie = trie_.subtrie(tama2);
        BOOST_REQUIRE(p_subtrie);

        auto iterator_ = std::begin(*p_subtrie);
        BOOST_REQUIRE(iterator_ != std::end(*p_subtrie));
        BOOST_CHECK(iterator_->value == tamana1);

        ++iterator_;
        BOOST_CHECK(iterator_ == std::end(*p_subtrie));
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) },
            { tamana2, detect_copy(tamana1) },
            { tamarai2, detect_copy(tamarai1) }
        };

        const auto p_subtrie = trie_.subtrie(tama2);
        BOOST_REQUIRE(p_subtrie);

        auto iterator_ = std::begin(*p_subtrie);
        BOOST_REQUIRE(iterator_ != std::end(*p_subtrie));
        BOOST_CHECK(iterator_->value == tamana1);

        ++iterator_;
        BOOST_REQUIRE(iterator_ != std::end(*p_subtrie));
        BOOST_CHECK(iterator_->value == tamarai1);

        ++iterator_;
        BOOST_CHECK(iterator_ == std::end(*p_subtrie));
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        const auto p_subtrie = trie_.subtrie("Kuma");
        BOOST_CHECK(!p_subtrie);
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        const auto p_subtrie = trie_.subtrie("Kuma");
        BOOST_REQUIRE(p_subtrie);

        auto iterator_ = std::begin(*p_subtrie);
        BOOST_REQUIRE(iterator_ != std::end(*p_subtrie));
        BOOST_CHECK(*iterator_ == 42);

        ++iterator_;
        BOOST_CHECK(iterator_ == std::end(*p_subtrie));
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        const auto p_subtrie = trie_.subtrie("Kuma");
        BOOST_CHECK(!p_subtrie);
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        const auto p_subtrie = trie_.subtrie("Kuma");
        BOOST_REQUIRE(p_subtrie);

        auto iterator_ = std::begin(*p_subtrie);
        BOOST_REQUIRE(iterator_ != std::end(*p_subtrie));
        BOOST_CHECK(*iterator_ == kumamoto1);

        ++iterator_;
        BOOST_CHECK(iterator_ == std::end(*p_subtrie));
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        constexpr auto                          tamarai_value = static_cast<int>(35);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value },
                                                          { "Tamarai", &tamarai_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        {
            const auto* const p_subtrie = tetengo_trie_trie_subtrie(p_trie, "Tama");
            BOOST_SCOPE_EXIT(p_subtrie)
            {
                tetengo_trie_trie_destroy(p_subtrie);
            }
            BOOST_SCOPE_EXIT_END;

            BOOST_TEST_REQUIRE(p_subtrie);

            auto* const p_subtrie_iterator = tetengo_trie_trie_createIterator(p_subtrie);
            BOOST_SCOPE_EXIT(p_subtrie_iterator)
            {
                tetengo_trie_trie_destroyIterator(p_subtrie_iterator);
            }
            BOOST_SCOPE_EXIT_END;

            BOOST_TEST_REQUIRE(tetengo_trie_trieIterator_hasNext(p_subtrie_iterator));
            BOOST_TEST(*static_cast<const int*>(tetengo_trie_trieIterator_get(p_subtrie_iterator)) == tamana_value);
            tetengo_trie_trieIterator_next(p_subtrie_iterator);

            BOOST_TEST_REQUIRE(tetengo_trie_trieIterator_hasNext(p_subtrie_iterator));
            BOOST_TEST(*static_cast<const int*>(tetengo_trie_trieIterator_get(p_subtrie_iterator)) == tamarai_value);
            tetengo_trie_trieIterator_next(p_subtrie_iterator);

            BOOST_TEST(!tetengo_trie_trieIterator_hasNext(p_subtrie_iterator));
        }
        {
            const auto* const p_subtrie = tetengo_trie_trie_subtrie(p_trie, "Uto");
            BOOST_SCOPE_EXIT(p_subtrie)
            {
                tetengo_trie_trie_destroy(p_subtrie);
            }
            BOOST_SCOPE_EXIT_END;

            BOOST_TEST(!p_subtrie);
        }
    }
    {
        const auto* const p_subtrie = tetengo_trie_trie_subtrie(nullptr, "Tama");
        BOOST_TEST(!p_subtrie);
    }
    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        constexpr auto                          tamarai_value = static_cast<int>(35);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value },
                                                          { "Tamarai", &tamarai_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_subtrie = tetengo_trie_trie_subtrie(p_trie, nullptr);
        BOOST_TEST(!p_subtrie);
    }
}

BOOST_AUTO_TEST_CASE(get_storage)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        [[maybe_unused]] const auto& storage = trie_.get_storage();
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        [[maybe_unused]] const auto& storage = trie_.get_storage();
    }
    {
        auto                                    p_input_stream = create_input_stream();
        const tetengo::trie::value_deserializer value_deserializer_{ [](const std::vector<char>& serialized) {
            static const tetengo::trie::default_deserializer<std::string>string_deserializer{ false };
            return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
        } };
        auto p_storage = std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, value_deserializer_);
        const tetengo::trie::trie<std::string, std::string> trie_{ std::move(p_storage) };

        const auto& storage = trie_.get_storage();

        std::stringstream                     stream{};
        const tetengo::trie::value_serializer serializer{
            [](const std::any& value) {
                static const tetengo::trie::default_serializer<std::string> serializer{ false };
                const auto serialized = serializer(std::any_cast<std::string>(value));
                return std::vector<char>{ std::begin(serialized), std::end(serialized) };
            },
            0
        };
        storage.serialize(stream, serializer);
        const auto storage_serialized = stream.str();

        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(storage_serialized), std::end(storage_serialized), std::begin(serialized), std::end(serialized));
    }

    {
        constexpr auto                          kumamoto_value = static_cast<int>(42);
        constexpr auto                          tamana_value = static_cast<int>(24);
        constexpr auto                          tamarai_value = static_cast<int>(35);
        std::vector<tetengo_trie_trieElement_t> elements{ { "Kumamoto", &kumamoto_value },
                                                          { "Tamana", &tamana_value },
                                                          { "Tamarai", &tamarai_value } };

        const auto* const p_trie = tetengo_trie_trie_create(
            std::data(elements),
            std::size(elements),
            sizeof(int),
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT(p_trie)
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_storage = tetengo_trie_trie_getStorage(p_trie);
        BOOST_TEST(p_storage);
    }
    {
        const auto* const p_storage = tetengo_trie_trie_getStorage(nullptr);
        BOOST_TEST(!p_storage);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
