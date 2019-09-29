/*!
    \brief A storage_.

    Copyright (C) 2019 kaoru
 */

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/storage.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::storage storage_{};
}

BOOST_AUTO_TEST_CASE(base_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::storage storage_{};

    storage_.base_at(42);
}

BOOST_AUTO_TEST_CASE(set_base_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::storage storage_{};

    storage_.set_base_at(42, 4242);

    BOOST_TEST(storage_.base_at(42) == 4242);
}

BOOST_AUTO_TEST_CASE(check_at)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::trie::storage storage_{};

    storage_.check_at(24);
}

BOOST_AUTO_TEST_CASE(set_check_at)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::storage storage_{};

    storage_.set_check_at(24, 124);

    BOOST_TEST(storage_.check_at(24) == 124);
}

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::storage storage_{};

    BOOST_TEST(storage_.size() == 0);

    storage_.base_at(42);

    BOOST_TEST(storage_.size() == 43);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
