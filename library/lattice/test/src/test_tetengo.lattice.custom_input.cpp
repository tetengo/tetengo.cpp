/*! \file
    \brief A custom input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.h>


namespace
{
    size_t length_procedure(void* const /*p_context*/)
    {
        return 0;
    }

    tetengo_lattice_input_t*
    create_subrange_procedure(void* const /*p_context*/, const size_t /*offset*/, const size_t /*length*/)
    {
        return nullptr;
    }

    int append_procedure(void* const /*p_context*/, tetengo_lattice_input_t* const /*p_another*/)
    {
        return 0;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(custom_input)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    std::vector<int>                              context{};
    const tetengo_lattice_customInputDefinition_t definition{
        &context, length_procedure, create_subrange_procedure, append_procedure
    };
    const auto* const p_input = tetengo_lattice_input_createCustomInput(&definition);
    BOOST_SCOPE_EXIT(p_input)
    {
        tetengo_lattice_input_destroy(p_input);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST(p_input);
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(create_subrange)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(append)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
