/*! \file
    \brief A storage.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef> // IWYU pragma: keep
#include <cstdint>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/value_serializer.hpp>


namespace
{
    class concrete_storage : public tetengo::trie::storage
    {
    private:
        // virtual functions

        virtual std::size_t base_check_size_impl() const override
        {
            return 4;
        }

        virtual std::int32_t base_at_impl(const std::size_t /*base_check_index*/) const override
        {
            return 42;
        }

        virtual void set_base_at_impl(const std::size_t /*base_check_index*/, const std::int32_t /*value*/) override {}

        virtual std::uint8_t check_at_impl(const std::size_t /*base_check_index*/) const override
        {
            return 24;
        }

        virtual void set_check_at_impl(const std::size_t /*base_check_index*/, const std::uint8_t /*value*/) override {}

        virtual std::size_t value_count_impl() const override
        {
            return 3;
        }

        virtual const std::any* value_at_impl(const std::size_t /*value_index*/) const override
        {
            return nullptr;
        }

        virtual void add_value_at_impl(const std::size_t /*index*/, std::any /*value*/) override {}

        virtual double filling_rate_impl() const override
        {
            return 0.9;
        }

        virtual void serialize_impl(
            std::ostream& /*output_stream*/,
            const tetengo::trie::value_serializer& /*value_serializer_*/) const override
        {}

        std::unique_ptr<storage> clone_impl() const override
        {
            return std::make_unique<concrete_storage>();
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage_{};
}

BOOST_AUTO_TEST_CASE(base_check_size)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage_{};

    BOOST_TEST(storage_.base_check_size() == 4U);
}

BOOST_AUTO_TEST_CASE(base_at)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage_{};

    BOOST_TEST(storage_.base_at(24) == 42);
}

BOOST_AUTO_TEST_CASE(set_base_at)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage_{};

    storage_.set_base_at(42, 4242);
}

BOOST_AUTO_TEST_CASE(check_at)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage_{};

    BOOST_TEST(storage_.check_at(42) == 24);
}

BOOST_AUTO_TEST_CASE(set_check_at)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage_{};

    storage_.set_check_at(24, 124);
}

BOOST_AUTO_TEST_CASE(value_count)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage_{};

    BOOST_TEST(storage_.value_count() == 3U);
}

BOOST_AUTO_TEST_CASE(value_at)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage_{};

    BOOST_TEST(!storage_.value_at(42));
}

BOOST_AUTO_TEST_CASE(add_value_at)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage_{};

    storage_.add_value_at(42, std::make_any<std::string>("hoge"));
}

BOOST_AUTO_TEST_CASE(filling_rate)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage_{};

    BOOST_CHECK_CLOSE(storage_.filling_rate(), 0.9, 0.01);
}

BOOST_AUTO_TEST_CASE(serialize)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage_{};

    std::ostringstream                    output_stream{};
    const tetengo::trie::value_serializer serializer{ [](const std::any&) { return std::vector<char>{}; }, 0 };
    storage_.serialize(output_stream, serializer);
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage_{};

    const auto p_clone = storage_.clone();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
