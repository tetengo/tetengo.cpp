/*! \file
    \brief A custom input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.h>


namespace
{
    size_t length_procedure(void* const p_context)
    {
        const auto* const p_vector = reinterpret_cast<std::vector<int>*>(p_context);
        return std::size(*p_vector);
    }

    void* create_subrange_context_procedure(void* const p_context, const size_t offset, const size_t length)
    {
        const auto* const p_superrange_vector = reinterpret_cast<std::vector<int>*>(p_context);
        auto              p_vector = std::make_unique<std::vector<int>>(
            std::next(std::begin(*p_superrange_vector), offset),
            std::next(std::begin(*p_superrange_vector), offset + length));
        return p_vector.release();
    }

    void destroy_subraneg_context_procedure(void* const p_context)
    {
        const std::unique_ptr<std::vector<int>> p_instance{ reinterpret_cast<std::vector<int>*>(p_context) };
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

    std::vector<int> context{
        3, 1, 4, 1, 5, 9,
    };
    const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                              length_procedure,
                                                              create_subrange_context_procedure,
                                                              destroy_subraneg_context_procedure,
                                                              append_procedure };
    const auto* const                             p_input = tetengo_lattice_input_createCustomInput(&definition);
    BOOST_SCOPE_EXIT(p_input)
    {
        tetengo_lattice_input_destroy(p_input);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST(p_input);
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    std::vector<int> context{
        3, 1, 4, 1, 5, 9,
    };
    const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                              length_procedure,
                                                              create_subrange_context_procedure,
                                                              destroy_subraneg_context_procedure,
                                                              append_procedure };
    const auto* const                             p_input = tetengo_lattice_input_createCustomInput(&definition);
    BOOST_SCOPE_EXIT(p_input)
    {
        tetengo_lattice_input_destroy(p_input);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_input);

    BOOST_TEST(tetengo_lattice_input_length(p_input) == 6U);
}

BOOST_AUTO_TEST_CASE(create_subrange)
{
    std::vector<int> context{
        3, 1, 4, 1, 5, 9,
    };
    const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                              length_procedure,
                                                              create_subrange_context_procedure,
                                                              destroy_subraneg_context_procedure,
                                                              append_procedure };
    const auto* const                             p_input = tetengo_lattice_input_createCustomInput(&definition);
    BOOST_SCOPE_EXIT(p_input)
    {
        tetengo_lattice_input_destroy(p_input);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_input);

    const auto* const p_subrange = tetengo_lattice_input_createSubrange(p_input, 2, 3);
    BOOST_SCOPE_EXIT(p_subrange)
    {
        tetengo_lattice_input_destroy(p_subrange);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_subrange);
    BOOST_TEST(tetengo_lattice_input_length(p_subrange) == 3U);
}

BOOST_AUTO_TEST_CASE(append)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
