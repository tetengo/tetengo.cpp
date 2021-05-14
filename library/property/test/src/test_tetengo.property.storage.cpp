/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/property/storage.hpp>


namespace
{
    class concrete_storage : public tetengo::property::storage
    {
    public:
        concrete_storage() : m_value{ 0 } {}

        virtual ~concrete_storage() = default;

    private:
        std::uint32_t m_value;

        virtual std::optional<std::uint32_t> get_uint32_impl(const std::filesystem::path& /*key*/) const override
        {
            if (m_value == 0)
            {
                return std::nullopt;
            }
            return m_value;
        }

        virtual void set_uint32_impl(const std::filesystem::path& /*key*/, const std::uint32_t value) override
        {
            m_value = value;
        };
    };

    class concrete_storage_factory : public tetengo::property::storage_factory
    {
    public:
        virtual ~concrete_storage_factory() = default;

    private:
        virtual std::unique_ptr<tetengo::property::storage> load_impl() const override
        {
            return std::make_unique<concrete_storage>();
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage{};
}

BOOST_AUTO_TEST_CASE(get_uint32)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage{};
    const auto             key = std::filesystem::path{ "hoge" } / "fuga";
    BOOST_TEST(!storage.get_uint32(key));
}

BOOST_AUTO_TEST_CASE(set_uint32)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage{};
    const auto       key = std::filesystem::path{ "hoge" } / "fuga";
    storage.set_uint32(key, 42);
    const auto o_value = storage.get_uint32(key);
    BOOST_REQUIRE(o_value);
    BOOST_TEST(*o_value == 42U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(storage_factory)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage_factory factory{};
    }
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage_factory factory{};

        const auto p_storage = factory.load();
        BOOST_CHECK(p_storage);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
