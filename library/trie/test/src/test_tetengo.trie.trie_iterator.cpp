/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/core/ignore_unused.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>
#include <tetengo/trie/trie_iterator.hpp>

struct tetengo_trie_trie;
struct tetengo_trie_trieIterator;


namespace
{
    constexpr char to_c(const unsigned char uc)
    {
        return static_cast<char>(uc);
    }

    static const std::string kumamoto1{
        to_c(0xE7), to_c(0x86), to_c(0x8A), to_c(0x6), to_c(0x9C), to_c(0xAC) // Kumamoto in Kanji in UTF-8
    };

    static const std::string tamana1{
        to_c(0xE7), to_c(0x8E), to_c(0x89), to_c(0xE5), to_c(0x90), to_c(0x8D) // Tamana in Kanji in UTF-8
    };

    static const std::wstring kumamoto2{ 0x718A, 0x672C }; // Kumamoto in Kanji in UTF-16/32

    static const std::wstring tamana2{ 0x7389, 0x540D }; // Tamana in Kanji in UTF-16/32


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
BOOST_AUTO_TEST_SUITE(trie_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::trie::trie_iterator_impl               impl{};
        const tetengo::trie::trie_iterator<std::string> iterator{ std::move(impl) };
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{};
        begin_copy_detection();

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));

        end_copy_detection();
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };
        begin_copy_detection();

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));

        end_copy_detection();
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };
        begin_copy_detection();

        auto iterator = std::begin(trie_);

        ++iterator;

        const auto iterator2 = iterator;

        BOOST_TEST(iterator2->value == tamana1);

        end_copy_detection();
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
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT((p_trie))
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        tetengo_trie_trieIterator* const p_iteartor = tetengo_trie_trieIterator_create(p_trie);
        BOOST_SCOPE_EXIT((p_iteartor))
        {
            tetengo_trie_trieIterator_destroy(p_iteartor);
        }
        BOOST_SCOPE_EXIT_END;
    }
}

BOOST_AUTO_TEST_CASE(operator_dereference)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{};
        const auto                                                          iterator = std::begin(trie_);

        BOOST_CHECK_THROW(*iterator, std::logic_error);
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };
        begin_copy_detection();

        const auto iterator = std::begin(trie_);

        BOOST_TEST(iterator->value == kumamoto1);

        end_copy_detection();
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
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT((p_trie))
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        tetengo_trie_trieIterator* const p_iterator = tetengo_trie_trie_createIterator(p_trie);
        BOOST_SCOPE_EXIT((p_iterator))
        {
            tetengo_trie_trie_destroyIterator(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;

        const auto* const p_value = tetengo_trie_trieIterator_get(p_iterator);
        BOOST_REQUIRE(p_value);
        BOOST_TEST(p_value == &kumamoto_value);
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::trie::trie_iterator_impl               iterator_impl1{};
        const tetengo::trie::trie_iterator<std::string> iterator1{ std::move(iterator_impl1) };
        tetengo::trie::trie_iterator_impl               iterator_impl2{};
        const tetengo::trie::trie_iterator<std::string> iterator2{ std::move(iterator_impl2) };

        BOOST_CHECK(iterator1 == iterator2);
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };
        begin_copy_detection();

        auto iterator1 = std::begin(trie_);
        auto iterator2 = std::begin(trie_);

        BOOST_CHECK(iterator1 == iterator2);

        ++iterator1;

        BOOST_CHECK(iterator1 != iterator2);

        ++iterator2;

        BOOST_CHECK(iterator1 == iterator2);

        ++iterator1;

        tetengo::trie::trie_iterator_impl                              iterator_impl3{};
        const tetengo::trie::trie_iterator<copy_detector<std::string>> iterator3{ std::move(iterator_impl3) };

        BOOST_CHECK(iterator1 == iterator3);
        BOOST_CHECK(iterator2 != iterator3);

        end_copy_detection();
    }


    {
        tetengo_trie_trie* const p_trie = tetengo_trie_trie_create(
            nullptr,
            0,
            tetengo_trie_trie_nullAddingObserver,
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT((p_trie))
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        tetengo_trie_trieIterator* const p_iterator = tetengo_trie_trie_createIterator(p_trie);
        BOOST_SCOPE_EXIT((p_iterator))
        {
            tetengo_trie_trie_destroyIterator(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(!tetengo_trie_trieIterator_hasMore(p_iterator));
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
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT((p_trie))
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        tetengo_trie_trieIterator* const p_iterator = tetengo_trie_trie_createIterator(p_trie);
        BOOST_SCOPE_EXIT((p_iterator))
        {
            tetengo_trie_trie_destroyIterator(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(tetengo_trie_trieIterator_hasMore(p_iterator));

        tetengo_trie_trieIterator_next(p_iterator);

        BOOST_TEST(tetengo_trie_trieIterator_hasMore(p_iterator));

        tetengo_trie_trieIterator_next(p_iterator);

        BOOST_TEST(!tetengo_trie_trieIterator_hasMore(p_iterator));
    }
}

BOOST_AUTO_TEST_CASE(increment)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{};
        auto                                                                iterator = std::begin(trie_);

        ++iterator;

        BOOST_CHECK(iterator == std::end(trie_));
    }
    {
        const tetengo::trie::trie<std::wstring, copy_detector<std::string>> trie_{
            { kumamoto2, detect_copy(kumamoto1) }, { tamana2, detect_copy(tamana1) }
        };
        auto iterator = std::begin(trie_);

        BOOST_REQUIRE(iterator != std::end(trie_));
        BOOST_TEST(iterator->value == kumamoto1);

        ++iterator;

        BOOST_REQUIRE(iterator != std::end(trie_));
        BOOST_TEST(iterator->value == tamana1);

        ++iterator;

        BOOST_CHECK(iterator == std::end(trie_));
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
            nullptr,
            tetengo_trie_trie_nullDoneObserver,
            nullptr,
            tetengo_trie_trie_defaultDoubleArrayDensityFactor());
        BOOST_SCOPE_EXIT((p_trie))
        {
            tetengo_trie_trie_destroy(p_trie);
        }
        BOOST_SCOPE_EXIT_END;

        tetengo_trie_trieIterator* const p_iterator = tetengo_trie_trie_createIterator(p_trie);
        BOOST_SCOPE_EXIT((p_iterator))
        {
            tetengo_trie_trie_destroyIterator(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST_REQUIRE(tetengo_trie_trieIterator_hasMore(p_iterator));
        {
            const auto* const p_value = tetengo_trie_trieIterator_get(p_iterator);
            BOOST_REQUIRE(p_value);
            BOOST_TEST(p_value == &kumamoto_value);
        }

        tetengo_trie_trieIterator_next(p_iterator);

        BOOST_TEST_REQUIRE(tetengo_trie_trieIterator_hasMore(p_iterator));
        {
            const auto* const p_value = tetengo_trie_trieIterator_get(p_iterator);
            BOOST_REQUIRE(p_value);
            BOOST_TEST(p_value == &tamana_value);
        }

        tetengo_trie_trieIterator_next(p_iterator);

        BOOST_TEST(!tetengo_trie_trieIterator_hasMore(p_iterator));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
