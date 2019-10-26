/*!
    \brief A shared storage.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
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
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), to_c(0x01), to_c(0x23),
        to_c(0x45), to_c(0x67), to_c(0x89), to_c(0xAB), to_c(0xCD), to_c(0xEF),
    };

    const std::vector<uint32_t> values{ 0x01234567, 0x89ABCDEF };

    std::unique_ptr<std::istream> create_input_stream()
    {
        return std::make_unique<std::stringstream>(std::string{ serialized.begin(), serialized.end() });
    }

    const std::vector<char> serialized_broken{
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), to_c(0x01), to_c(0x23), to_c(0x45), to_c(0x67), to_c(0x89),
    };

    std::unique_ptr<std::istream> create_broken_input_stream()
    {
        return std::make_unique<std::stringstream>(std::string{ serialized_broken.begin(), serialized_broken.end() });
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

        BOOST_TEST(storage_.values() == values);
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

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    BOOST_TEST(storage_.size() == 1U);

    storage_.base_at(42);

    BOOST_TEST(storage_.size() == 43U);
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

BOOST_AUTO_TEST_CASE(values)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_base_at(0, 42);
    storage_.set_check_at(1, 24);

    const auto values = storage_.values();

    static const std::vector<std::uint32_t> expected{ 0x00002AFF, 0x00000018 };
    BOOST_TEST(values == expected);
}

BOOST_AUTO_TEST_CASE(serialize)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_base_at(0, 42);
    storage_.set_check_at(1, 24);

    std::ostringstream output_stream{};
    storage_.serialize(output_stream);

    static const std::string expected{ to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), to_c(0x00), to_c(0x00),
                                       to_c(0x2A), to_c(0xFF), to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x18) };
    BOOST_TEST(output_stream.str() == expected);
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::shared_storage storage_{};

    storage_.set_base_at(0, 42);
    storage_.set_check_at(1, 24);

    const auto p_clone = storage_.clone();

    const auto values = p_clone->values();

    static const std::vector<std::uint32_t> expected{ 0x00002AFF, 0x00000018 };
    BOOST_TEST(values == expected);

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
