/*! \file
    \brief A vocabulary key.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.hpp>


namespace
{
    class concrete_vocabulary_key : public tetengo::lattice::vocabulary_key_base
    {};

    class concrete_vocabulary_key2 : public tetengo::lattice::vocabulary_key_base
    {};


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(vocabulary_key_base)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_vocabulary_key key{};
}

BOOST_AUTO_TEST_CASE(is)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::vocabulary_key_base& key = concrete_vocabulary_key{};

    BOOST_TEST(key.is<concrete_vocabulary_key>());
    BOOST_TEST(!key.is<concrete_vocabulary_key2>());
}

BOOST_AUTO_TEST_CASE(as)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::vocabulary_key_base& key = concrete_vocabulary_key{};

        const auto& casted = key.as<concrete_vocabulary_key>();
        BOOST_TEST(&casted == &key);
    }
    {
        concrete_vocabulary_key                key{};
        tetengo::lattice::vocabulary_key_base& key_ref = key;

        const auto& casted = key_ref.as<concrete_vocabulary_key>();
        BOOST_TEST(&casted == &key);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(vocabulary_key)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::vocabulary_key<int> key{ 42 };
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::vocabulary_key<int> key{ 42 };

        BOOST_TEST(key.value() == 42);
    }
    {
        tetengo::lattice::vocabulary_key<int> key{ 42 };

        BOOST_TEST(key.value() == 42);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
