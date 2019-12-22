/*!
    \brief A shared storage.

    Copyright (C) 2019 kaoru
 */

#include <any>
#include <cstdint>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/shared_storage.hpp>
#include <tetengo/trie/storage.hpp>


namespace
{
    constexpr char to_c(unsigned char uc)
    {
        return uc;
    }

    const std::vector<char> serialized{
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), to_c(0x01), to_c(0x23), to_c(0x45),
        to_c(0x67), to_c(0x89), to_c(0xAB), to_c(0xCD), to_c(0xEF), to_c(0x00), to_c(0x00),
        to_c(0x00), to_c(0x03), to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), to_c(0x00),
        to_c(0x00), to_c(0x00), to_c(0x02), to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x01),
    };

    const std::vector<uint32_t> base_check_array{ 0x01234567, 0x89ABCDEF };

    std::unique_ptr<std::istream> create_input_stream()
    {
        return std::make_unique<std::stringstream>(std::string{ std::begin(serialized), std::end(serialized) });
    }

    const std::vector<char> serialized_broken{
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), to_c(0x01), to_c(0x23), to_c(0x45), to_c(0x67), to_c(0x89),
    };

    std::unique_ptr<std::istream> create_broken_input_stream()
    {
        return std::make_unique<std::stringstream>(
            std::string{ std::begin(serialized_broken), std::end(serialized_broken) });
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(shared_storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::shared_storage storage_{};
    }
    {
        const auto                          p_input_stream = create_input_stream();
        const tetengo::trie::shared_storage storage_{ *p_input_stream };

        BOOST_TEST(storage_.base_check_array() == base_check_array);
        // BOOST_REQUIRE(storage_.mapped_at(4));
        // BOOST_TEST(*storage_.mapped_at(4) == 0U);
        // BOOST_REQUIRE(storage_.mapped_at(2));
        // BOOST_TEST(*storage_.mapped_at(2) == 1U);
        // BOOST_REQUIRE(storage_.mapped_at(1));
        // BOOST_TEST(*storage_.mapped_at(1) == 2U);
    }
    {
        const auto p_input_stream = create_broken_input_stream();

        BOOST_CHECK_THROW(const tetengo::trie::shared_storage storage_{ *p_input_stream }, std::ios_base::failure);
    }
}

BOOST_AUTO_TEST_CASE(base_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::shared_storage storage_{};

    BOOST_TEST(storage_.base_at(42) == 0);
}

BOOST_AUTO_TEST_CASE(set_base_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_base_at(42, 4242);

    BOOST_TEST(storage_.base_at(42) == 4242);
}

BOOST_AUTO_TEST_CASE(check_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::shared_storage storage_{};

    BOOST_TEST(storage_.check_at(24) == tetengo::trie::double_array::vacant_check_value());
}

BOOST_AUTO_TEST_CASE(set_check_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_check_at(24, 124);

    BOOST_TEST(storage_.check_at(24) == 124);
}

BOOST_AUTO_TEST_CASE(filling_rate)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    for (auto i = static_cast<std::size_t>(0); i < 9; ++i)
    {
        if (i % 3 == 0)
        {
            storage_.set_base_at(i, static_cast<std::int32_t>(i * i));
            storage_.set_check_at(i, static_cast<std::uint8_t>(i));
        }
        else
        {
            storage_.set_base_at(i, storage_.base_at(i));
            storage_.set_check_at(i, storage_.check_at(i));
        }
    }

    BOOST_CHECK_CLOSE(storage_.filling_rate(), 3.0 / 9.0, 0.1);
}

BOOST_AUTO_TEST_CASE(base_check_array)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_base_at(0, 42);
    storage_.set_check_at(1, 24);

    const auto base_check_array = storage_.base_check_array();

    static const std::vector<std::uint32_t> expected{ 0x00002AFF, 0x00000018 };
    BOOST_TEST(base_check_array == expected);
}

BOOST_AUTO_TEST_CASE(mapped_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::shared_storage storage_{};

    BOOST_TEST(!storage_.mapped_at(42));
}

BOOST_AUTO_TEST_CASE(add_mapped_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.add_mapped_at(24, std::make_any<std::string>("hoge"));

    BOOST_TEST(!storage_.mapped_at(0));
    BOOST_REQUIRE(storage_.mapped_at(24));
    BOOST_TEST(std::any_cast<std::string>(*storage_.mapped_at(24)) == "hoge");
    BOOST_TEST(!storage_.mapped_at(42));

    storage_.add_mapped_at(42, std::make_any<std::string>("fuga"));

    BOOST_REQUIRE(storage_.mapped_at(42));
    BOOST_TEST(std::any_cast<std::string>(*storage_.mapped_at(42)) == "fuga");
    BOOST_TEST(!storage_.mapped_at(4242));

    storage_.add_mapped_at(0, std::make_any<std::string>("piyo"));

    BOOST_REQUIRE(storage_.mapped_at(0));
    BOOST_TEST(std::any_cast<std::string>(*storage_.mapped_at(0)) == "piyo");
    BOOST_REQUIRE(storage_.mapped_at(42));
    BOOST_TEST(std::any_cast<std::string>(*storage_.mapped_at(42)) == "fuga");
}

BOOST_AUTO_TEST_CASE(serialize)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_base_at(0, 42);
    storage_.set_check_at(1, 24);

    storage_.add_mapped_at(4, std::make_any<std::string>("hoge"));
    storage_.add_mapped_at(2, std::make_any<std::string>("fuga"));
    storage_.add_mapped_at(1, std::make_any<std::string>("piyo"));

    std::ostringstream output_stream{};
    storage_.serialize(output_stream, [](const std::any&) { return std::string{}; });

    static const std::string expected{
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x2A), to_c(0xFF), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x18), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x03), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x01), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x03), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x68), to_c(0x6F), to_c(0x67), to_c(0x65), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x66), to_c(0x75), to_c(0x67), to_c(0x61), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x70), to_c(0x69), to_c(0x79), to_c(0x6F), /*                                                            */
    };
    const std::string serialized = output_stream.str();
    //BOOST_CHECK_EQUAL_COLLECTIONS(serialized.begin(), serialized.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_base_at(0, 42);
    storage_.set_check_at(1, 24);

    const auto p_clone = storage_.clone();

    const auto base_check_array = p_clone->base_check_array();

    static const std::vector<std::uint32_t> expected{ 0x00002AFF, 0x00000018 };
    BOOST_TEST(base_check_array == expected);

    p_clone->set_base_at(0, 2424);
    p_clone->set_check_at(5, 42);

    BOOST_TEST(p_clone->base_at(0) == 2424);
    BOOST_TEST(p_clone->check_at(5) == 42);

    BOOST_TEST(storage_.base_at(0) == 2424);
    BOOST_TEST(storage_.check_at(5) == 42);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
