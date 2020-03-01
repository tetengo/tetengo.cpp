/*! \file
    \brief A trie.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <functional>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/core/ignore_unused.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trie_iterator.hpp>

struct tetengo_trie_trie;


namespace
{
    constexpr char to_c(const unsigned char uc)
    {
        return static_cast<char>(uc);
    }

    constexpr char nul_byte()
    {
        return to_c(0xFE);
    }


    static const std::string kumamoto1{
        to_c(0xE7), to_c(0x86), to_c(0x8A), to_c(0x6), to_c(0x9C), to_c(0xAC) // Kumamoto in Kanji in UTF-8
    };

    static const std::string tamana1{
        to_c(0xE7), to_c(0x8E), to_c(0x89), to_c(0xE5), to_c(0x90), to_c(0x8D) // Tamana in Kanji in UTF-8
    };

    static const std::string tamarai1{
        to_c(0xE7), to_c(0x8E), to_c(0x89), to_c(0xE6), to_c(0x9D), to_c(0xA5) // Tamarai in Kanji in UTF-8
    };

    static const std::wstring kumamoto2{ 0x718A, 0x672C }; // Kumamoto in Kanji in UTF-16/32

    static const std::wstring tamana2{ 0x7389, 0x540D }; // Tamana in Kanji in UTF-16/32

    static const std::wstring tamarai2{ 0x7389, 0x6765 }; // Tamarai in Kanji in UTF-16/32

    static const std::wstring tama2{ 0x7389 }; // Tama in Kanji in UTF-16/32

    static const std::wstring uto2{ 0x5B87, 0x571F }; // Uto in Kanji in UTF-16/32

    const std::vector<char> serialized{
        nul_byte(), nul_byte(), nul_byte(), to_c(0x0B), /*  base check array                                          */
        to_c(0xFF), to_c(0xFF), to_c(0x90), to_c(0xFF), /*                                                            */
        to_c(0xFF), to_c(0xFF), to_c(0x78), to_c(0x71), /*                                                            */
        to_c(0xFF), to_c(0xFF), to_c(0x9D), to_c(0x8A), /*                                                            */
        to_c(0xFF), to_c(0xFF), to_c(0x7E), to_c(0x73), /*                                                            */
        to_c(0xFF), to_c(0xFF), to_c(0xD9), to_c(0x67), /*                                                            */
        nul_byte(), nul_byte(), to_c(0x06), to_c(0x2C), /*                                                            */
        nul_byte(), nul_byte(), nul_byte(), nul_byte(), /*                                                            */
        to_c(0xFF), to_c(0xFF), to_c(0xB4), to_c(0x89), /*                                                            */
        to_c(0xFF), to_c(0xFF), to_c(0xFC), to_c(0x54), /*                                                            */
        nul_byte(), nul_byte(), to_c(0x0A), to_c(0x0D), /*                                                            */
        nul_byte(), nul_byte(), to_c(0x01), nul_byte(), /*                                                            */
        nul_byte(), nul_byte(), nul_byte(), to_c(0x02), /* value array                                                */
        nul_byte(), nul_byte(), nul_byte(), to_c(0x06), /*                                                            */
        to_c(0xE7), to_c(0x86), to_c(0x8A), to_c(0x06), to_c(0x9C), to_c(0xAC), /*                                    */
        nul_byte(), nul_byte(), nul_byte(), to_c(0x06), /*                                                            */
        to_c(0xE7), to_c(0x8E), to_c(0x89), to_c(0xE5), to_c(0x90), to_c(0x8D), /*                                    */
    };

    std::unique_ptr<std::istream> create_input_stream()
    {
        return std::make_unique<std::stringstream>(std::string{ std::begin(serialized), std::end(serialized) });
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

        copy_detector(copy_detector&& another) : value{ std::move(another.value) } {}
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

    tetengo::trie::trie<std::string, int>::null_building_observer_set();

    tetengo_trie_trie_nullAddingObserver("hoge");
    tetengo_trie_trie_nullDoneObserver();
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
        const tetengo::trie::trie<std::string, int> trie_{ tetengo::trie::default_serializer<std::string>{} };
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
            tetengo::trie::default_serializer<std::string>{},
            observer,
            tetengo::trie::trie<std::string, int>::default_double_array_density_factor()
        };

        static const tetengo::trie::default_deserializer<std::string> key_deserializer{};
        BOOST_TEST_REQUIRE(added_serialized_keys.size() == 2U);
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
            std::begin(content), std::end(content), tetengo::trie::default_serializer<std::string_view>{}
        };
    }
    {
        auto p_input_stream = create_input_stream();
        auto p_storage =
            std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, [](const std::vector<char>& serialized) {
                static const tetengo::trie::default_deserializer<std::string> string_deserializer{};
                return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
            });
        const tetengo::trie::trie<std::string, std::string> trie_{ std::move(p_storage) };
    }
    {
        auto p_input_stream = create_input_stream();
        auto p_storage =
            std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, [](const std::vector<char>& serialized) {
                static const tetengo::trie::default_deserializer<std::string> string_deserializer{};
                return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
            });
        const tetengo::trie::trie<std::string, std::string> trie_{ std::move(p_storage),
                                                                   tetengo::trie::default_serializer<std::string>{} };
    }

    {
        const int                                kumamoto_value = 42;
        const int                                tamana_value = 24;
        std::vector<tetengo_trie_trie_element_t> elements{ { "Kumamoto", &kumamoto_value },
                                                           { "Tamana", &tamana_value } };

        tetengo_trie_trie* const p_trie = tetengo_trie_trie_create(
            elements.data(),
            elements.size(),
            tetengo_trie_trie_nullAddingObserver,
            tetengo_trie_trie_nullDoneObserver,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT((p_trie))
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;
    }
}

BOOST_AUTO_TEST_CASE(empty)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        BOOST_TEST(trie_.empty());
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 } };

        BOOST_TEST(!trie_.empty());
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        BOOST_TEST(!trie_.empty());
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        BOOST_TEST(trie_.empty());
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        BOOST_TEST(!trie_.empty());
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(trie_.empty());
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(!trie_.empty());
    }
}

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        BOOST_TEST(trie_.size() == 0U);
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 } };

        BOOST_TEST(trie_.size() == 1U);
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        BOOST_TEST(trie_.size() == 2U);
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        BOOST_TEST(trie_.size() == 0U);
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        BOOST_TEST(trie_.size() == 1U);
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(trie_.size() == 0U);
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        BOOST_TEST(trie_.size() == 2U);
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
}

BOOST_AUTO_TEST_CASE(begin_end)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
    }

    {
        const tetengo::trie::trie<std::string_view, int> trie_{};

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
    }

    {
        std::vector<std::pair<std::string_view, std::string>>    content{};
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
    }
    {
        std::vector<std::pair<std::string_view, std::string>>    content{ { "Kumamoto", kumamoto1 },
                                                                       { "Tamana", tamana1 } };
        const tetengo::trie::trie<std::string_view, std::string> trie_{ std::make_move_iterator(std::begin(content)),
                                                                        std::make_move_iterator(std::end(content)) };

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
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
}

BOOST_AUTO_TEST_CASE(get_storage)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        trie_.get_storage();
    }
    {
        const tetengo::trie::trie<std::string_view, int> trie_{ { "Kumamoto", 42 } };

        trie_.get_storage();
    }
    {
        auto p_input_stream = create_input_stream();
        auto p_storage =
            std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, [](const std::vector<char>& serialized) {
                static const tetengo::trie::default_deserializer<std::string> string_deserializer{};
                return string_deserializer(std::string{ std::begin(serialized), std::end(serialized) });
            });
        const tetengo::trie::trie<std::string, std::string> trie_{ std::move(p_storage) };

        const auto& storage = trie_.get_storage();

        std::stringstream stream{};
        storage.serialize(stream, [](const std::any& value) {
            static const tetengo::trie::default_serializer<std::string> serializer{};
            const auto serialized = serializer(std::any_cast<std::string>(value));
            return std::vector<char>{ std::begin(serialized), std::end(serialized) };
        });
        const auto storage_serialized = stream.str();

        BOOST_CHECK_EQUAL_COLLECTIONS(
            std::begin(storage_serialized), std::end(storage_serialized), std::begin(serialized), std::end(serialized));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
