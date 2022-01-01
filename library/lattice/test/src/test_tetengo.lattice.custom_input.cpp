/*! \file
    \brief A custom input.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <numeric>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.h>


namespace
{
    bool equal_to_procedure(void* const p_context, void* const p_another_context)
    {
        const auto* const p_vector = reinterpret_cast<std::vector<int>*>(p_context);
        const auto* const p_another_vector = reinterpret_cast<std::vector<int>*>(p_another_context);

        return *p_vector == *p_another_vector;
    }

    size_t hash_value_procedure(void* const p_context)
    {
        const auto* const p_vector = reinterpret_cast<std::vector<int>*>(p_context);
        return std::accumulate(std::begin(*p_vector), std::end(*p_vector), 0);
    }

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

    bool append_procedure(void* const p_context, void* const p_another_context)
    {
        auto* const p_vector = reinterpret_cast<std::vector<int>*>(p_context);
        auto* const p_another_vector = reinterpret_cast<std::vector<int>*>(p_another_context);
        p_vector->insert(
            std::end(*p_vector),
            std::make_move_iterator(std::begin(*p_another_vector)),
            std::make_move_iterator(std::end(*p_another_vector)));
        return true;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(custom_input)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    std::vector<int>                              context{ 3, 1, 4, 1, 5, 9 };
    const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                              equal_to_procedure,
                                                              hash_value_procedure,
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

BOOST_AUTO_TEST_CASE(context)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<int>                              context{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                                  equal_to_procedure,
                                                                  hash_value_procedure,
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

        const auto* const p_context = tetengo_lattice_customInput_context(p_input);
        BOOST_TEST_REQUIRE(p_context);
        const auto* const p_vector = reinterpret_cast<const std::vector<int>*>(p_context);
        BOOST_TEST(*p_vector == context);
    }
    {
        const auto* const p_context = tetengo_lattice_customInput_context(nullptr);
        BOOST_TEST(!p_context);
    }
    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto* const p_context = tetengo_lattice_customInput_context(p_input);
        BOOST_TEST(!p_context);
    }
}

BOOST_AUTO_TEST_CASE(set_context)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<int>                              context{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                                  equal_to_procedure,
                                                                  hash_value_procedure,
                                                                  length_procedure,
                                                                  create_subrange_context_procedure,
                                                                  destroy_subraneg_context_procedure,
                                                                  append_procedure };
        auto* const                                   p_input = tetengo_lattice_input_createCustomInput(&definition);
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        std::vector<int> context2{ 2, 7, 1, 8, 2, 8 };
        const auto       result = tetengo_lattice_customInput_setContext(p_input, &context2);
        BOOST_TEST(result);

        const auto* const p_context = tetengo_lattice_customInput_context(p_input);
        BOOST_TEST_REQUIRE(p_context);
        const auto* const p_vector = reinterpret_cast<const std::vector<int>*>(p_context);
        BOOST_TEST(*p_vector == context2);
    }
    {
        std::vector<int>                              context{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                                  equal_to_procedure,
                                                                  hash_value_procedure,
                                                                  length_procedure,
                                                                  create_subrange_context_procedure,
                                                                  destroy_subraneg_context_procedure,
                                                                  append_procedure };
        auto* const                                   p_input = tetengo_lattice_input_createCustomInput(&definition);
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto result = tetengo_lattice_customInput_setContext(p_input, nullptr);
        BOOST_TEST(result);

        const auto* const p_context = tetengo_lattice_customInput_context(p_input);
        BOOST_TEST(!p_context);
    }
    {
        std::vector<int> context2{ 2, 7, 1, 8, 2, 8 };
        const auto       result = tetengo_lattice_customInput_setContext(nullptr, &context2);
        BOOST_TEST(!result);
    }
    {
        auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        std::vector<int> context2{ 2, 7, 1, 8, 2, 8 };
        const auto       result = tetengo_lattice_customInput_setContext(p_input, &context2);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<int>                              context1{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition1{ &context1,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input1 = tetengo_lattice_input_createCustomInput(&definition1);
        BOOST_SCOPE_EXIT(p_input1)
        {
            tetengo_lattice_input_destroy(p_input1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input1);

        std::vector<int>                              context2{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition2{ &context2,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input2 = tetengo_lattice_input_createCustomInput(&definition2);
        BOOST_SCOPE_EXIT(p_input2)
        {
            tetengo_lattice_input_destroy(p_input2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input2);

        BOOST_TEST(tetengo_lattice_input_equal(p_input1, p_input2));
    }
    {
        std::vector<int>                              context1{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition1{ &context1,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input1 = tetengo_lattice_input_createCustomInput(&definition1);
        BOOST_SCOPE_EXIT(p_input1)
        {
            tetengo_lattice_input_destroy(p_input1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input1);

        std::vector<int>                              context2{ 2, 7, 1, 8, 2, 8 };
        const tetengo_lattice_customInputDefinition_t definition2{ &context2,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input2 = tetengo_lattice_input_createCustomInput(&definition2);
        BOOST_SCOPE_EXIT(p_input2)
        {
            tetengo_lattice_input_destroy(p_input2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input2);

        BOOST_TEST(!tetengo_lattice_input_equal(p_input1, p_input2));
    }
    {
        std::vector<int>                              context2{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition2{ &context2,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input2 = tetengo_lattice_input_createCustomInput(&definition2);
        BOOST_SCOPE_EXIT(p_input2)
        {
            tetengo_lattice_input_destroy(p_input2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input2);

        BOOST_TEST(!tetengo_lattice_input_equal(nullptr, p_input2));
    }
    {
        std::vector<int>                              context1{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition1{ &context1,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input1 = tetengo_lattice_input_createCustomInput(&definition1);
        BOOST_SCOPE_EXIT(p_input1)
        {
            tetengo_lattice_input_destroy(p_input1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input1);

        BOOST_TEST(!tetengo_lattice_input_equal(p_input1, nullptr));
    }
    {
        BOOST_TEST(!tetengo_lattice_input_equal(nullptr, nullptr));
    }
}

BOOST_AUTO_TEST_CASE(hash_value)
{
    BOOST_TEST_PASSPOINT();

    {
        std::vector<int>                              context1{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition1{ &context1,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input1 = tetengo_lattice_input_createCustomInput(&definition1);
        BOOST_SCOPE_EXIT(p_input1)
        {
            tetengo_lattice_input_destroy(p_input1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input1);

        std::vector<int>                              context2{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition2{ &context2,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input2 = tetengo_lattice_input_createCustomInput(&definition2);
        BOOST_SCOPE_EXIT(p_input2)
        {
            tetengo_lattice_input_destroy(p_input2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input2);

        BOOST_TEST(tetengo_lattice_input_hashValue(p_input1) == tetengo_lattice_input_hashValue(p_input2));
    }
    {
        std::vector<int>                              context1{ 3, 1, 4, 1, 5, 9 };
        const tetengo_lattice_customInputDefinition_t definition1{ &context1,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input1 = tetengo_lattice_input_createCustomInput(&definition1);
        BOOST_SCOPE_EXIT(p_input1)
        {
            tetengo_lattice_input_destroy(p_input1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input1);

        std::vector<int>                              context2{ 2, 7, 1, 8, 2, 8 };
        const tetengo_lattice_customInputDefinition_t definition2{ &context2,
                                                                   equal_to_procedure,
                                                                   hash_value_procedure,
                                                                   length_procedure,
                                                                   create_subrange_context_procedure,
                                                                   destroy_subraneg_context_procedure,
                                                                   append_procedure };
        const auto* const                             p_input2 = tetengo_lattice_input_createCustomInput(&definition2);
        BOOST_SCOPE_EXIT(p_input2)
        {
            tetengo_lattice_input_destroy(p_input2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input2);

        BOOST_TEST(tetengo_lattice_input_hashValue(p_input1) != tetengo_lattice_input_hashValue(p_input2));
    }
    {
        BOOST_TEST(tetengo_lattice_input_hashValue(nullptr) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    std::vector<int>                              context{ 3, 1, 4, 1, 5, 9 };
    const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                              equal_to_procedure,
                                                              hash_value_procedure,
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
                                                              equal_to_procedure,
                                                              hash_value_procedure,
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
    const auto* const p_subrange_context = tetengo_lattice_customInput_context(p_subrange);
    BOOST_TEST_REQUIRE(p_subrange_context);
    const auto* const p_subrange_vector = reinterpret_cast<const std::vector<int>*>(p_subrange_context);
    BOOST_TEST((*p_subrange_vector == std::vector<int>{ 4, 1, 5 }));
}

BOOST_AUTO_TEST_CASE(append)
{
    BOOST_TEST_PASSPOINT();

    std::vector<int>                              context{ 3, 1, 4, 1, 5, 9 };
    const tetengo_lattice_customInputDefinition_t definition{ &context,
                                                              equal_to_procedure,
                                                              hash_value_procedure,
                                                              length_procedure,
                                                              create_subrange_context_procedure,
                                                              destroy_subraneg_context_procedure,
                                                              append_procedure };
    auto* const                                   p_input = tetengo_lattice_input_createCustomInput(&definition);
    BOOST_SCOPE_EXIT(p_input)
    {
        tetengo_lattice_input_destroy(p_input);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_input);

    std::vector<int>                              context2{ 2, 6 };
    const tetengo_lattice_customInputDefinition_t definition2{ &context2,
                                                               equal_to_procedure,
                                                               hash_value_procedure,
                                                               length_procedure,
                                                               create_subrange_context_procedure,
                                                               destroy_subraneg_context_procedure,
                                                               append_procedure };
    auto* const                                   p_input2 = tetengo_lattice_input_createCustomInput(&definition2);

    const auto result = tetengo_lattice_input_append(p_input, p_input2);
    BOOST_TEST(result);
    BOOST_TEST((context == std::vector<int>{ 3, 1, 4, 1, 5, 9, 2, 6 }));
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
