/*! \file
    \brief The usage test of tetengo::json.

    $12026 kaoru  https://www.tetengo.org/
*/


#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include "usage_tetengo.json.parsing_c.h"
#include "usage_tetengo.json.parsing_cpp.hpp"


BOOST_AUTO_TEST_SUITE(usagetest_tetengo)
BOOST_AUTO_TEST_SUITE(json)


BOOST_AUTO_TEST_CASE(parsing)
{
    BOOST_TEST_PASSPOINT();

    usage_tetengo::json::parsing();
    usage_tetengo_json_parsing();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
