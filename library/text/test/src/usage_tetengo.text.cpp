/*! \file
    \brief The usage test of tetengo::text.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/


#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include "usage_tetengo.text.encoding_c.h"
#include "usage_tetengo.text.encoding_cpp.hpp"
#include "usage_tetengo.text.graphemeSplit_c.h"
#include "usage_tetengo.text.grapheme_split_cpp.hpp"


BOOST_AUTO_TEST_SUITE(usagetest_tetengo)
BOOST_AUTO_TEST_SUITE(text)


BOOST_AUTO_TEST_CASE(encoding)
{
    BOOST_TEST_PASSPOINT();

    usage_tetengo::text::encoding();
    usage_tetengo_text_encoding();

    usage_tetengo::text::grapheme_split();
    usage_tetengo_text_graphemeSplit();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
