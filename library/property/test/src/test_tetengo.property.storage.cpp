/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

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
        concrete_storage() {}

        virtual ~concrete_storage() = default;

    private:
        virtual void save_impl() const override {}
    };

    class concrete_storage_loader : public tetengo::property::storage_loader
    {
    public:
        virtual ~concrete_storage_loader() = default;

    private:
        virtual std::unique_ptr<tetengo::property::storage>
        load_impl(const std::filesystem::path& /*path*/) const override
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
    // BOOST_TEST(!storage.get_uint32(key));
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

BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage{};
    storage.save();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(storage_loader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage_loader factory{};
    }
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_storage_loader factory{};

        const auto path = std::filesystem::path{ "foo" } / "bar";
        const auto p_storage = factory.load(path);
        BOOST_CHECK(p_storage);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
