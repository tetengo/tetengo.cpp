/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.hpp>


namespace
{
    class concrete_input : public tetengo::lattice::input_base
    {};

    class concrete_input2 : public tetengo::lattice::input_base
    {};


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(input_base)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_input key{};
}

BOOST_AUTO_TEST_CASE(is)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::input_base& key = concrete_input{};

    BOOST_TEST(key.is<concrete_input>());
    BOOST_TEST(!key.is<concrete_input2>());
}

BOOST_AUTO_TEST_CASE(as)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::input_base& key = concrete_input{};

        const auto& casted = key.as<concrete_input>();
        BOOST_TEST(&casted == &key);
    }
    {
        concrete_input                key{};
        tetengo::lattice::input_base& key_ref = key;

        const auto& casted = key_ref.as<concrete_input>();
        BOOST_TEST(&casted == &key);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(input)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::input<int> key{ 42 };
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::input<int> key{ 42 };

        BOOST_TEST(key.value() == 42);
    }
    {
        tetengo::lattice::input<int> key{ 42 };

        BOOST_TEST(key.value() == 42);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
