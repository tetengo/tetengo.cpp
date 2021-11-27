/*! \file
    \brief A string input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/string_input.hpp>


namespace
{
    class another_input : public tetengo::lattice::input
    {
    private:
        virtual std::size_t length_impl() const override
        {
            return 0;
        }

        virtual std::unique_ptr<input>
        create_subrange_impl(const std::size_t /*offset*/, const std::size_t /*length*/) const override
        {
            return std::make_unique<another_input>();
        }

        virtual void append_impl(std::unique_ptr<input>&& /*p_another*/) override {}
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(string_input)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::string_input input{ "hoge" };
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::string_input input{ "hoge" };

        BOOST_TEST(input.value() == "hoge");
    }
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::string_input input{ "hoge" };

    BOOST_TEST(input.length() == 4U);
}

BOOST_AUTO_TEST_CASE(create_subrange)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::string_input input{ "hoge" };

    {
        const auto p_subrange = input.create_subrange(0, 4);
        BOOST_REQUIRE(p_subrange);
        BOOST_TEST_REQUIRE(p_subrange->is<tetengo::lattice::string_input>());
        BOOST_TEST(p_subrange->as<tetengo::lattice::string_input>().value() == "hoge");
    }
    {
        const auto p_subrange = input.create_subrange(1, 2);
        BOOST_REQUIRE(p_subrange);
        BOOST_TEST_REQUIRE(p_subrange->is<tetengo::lattice::string_input>());
        BOOST_TEST(p_subrange->as<tetengo::lattice::string_input>().value() == "og");
    }
    {
        const auto p_subrange = input.create_subrange(4, 0);
        BOOST_REQUIRE(p_subrange);
        BOOST_TEST_REQUIRE(p_subrange->is<tetengo::lattice::string_input>());
        BOOST_TEST(p_subrange->as<tetengo::lattice::string_input>().value() == "");
    }
    {
        BOOST_CHECK_THROW(const auto p_subrange = input.create_subrange(0, 5), std::out_of_range);
    }
    {
        BOOST_CHECK_THROW(const auto p_subrange = input.create_subrange(5, 0), std::out_of_range);
    }
}

BOOST_AUTO_TEST_CASE(append)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::lattice::string_input input{ "hoge" };

        input.append(std::make_unique<tetengo::lattice::string_input>("fuga"));

        BOOST_TEST(input.value() == "hogefuga");
    }
    {
        tetengo::lattice::string_input input{ "hoge" };

        BOOST_CHECK_THROW(input.append(std::unique_ptr<tetengo::lattice::string_input>{}), std::invalid_argument);
        BOOST_CHECK_THROW(input.append(std::make_unique<another_input>()), std::invalid_argument);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
