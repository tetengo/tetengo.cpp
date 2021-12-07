/*! \file
    \brief A string input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.h>
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

        virtual std::unique_ptr<input> clone_impl() const override
        {
            return std::make_unique<another_input>();
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

    {
        const tetengo::lattice::string_input input{ "hoge" };
    }

    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_input);
    }
    {
        const auto* const p_input = tetengo_lattice_input_createStringInput(nullptr);
        BOOST_TEST(!p_input);
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::string_input input{ "hoge" };

        BOOST_TEST(input.value() == "hoge");
    }
    {
        tetengo::lattice::string_input input{ "hoge" };

        input.value() = "fuga";
        BOOST_TEST(input.value() == "fuga");
    }

    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto* const value = tetengo_lattice_stringInput_value(p_input);
        BOOST_TEST(std::string{ value } == "hoge");
    }
    {
        BOOST_TEST(!tetengo_lattice_stringInput_value(nullptr));
    }
    {
        auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto result = tetengo_lattice_stringInput_setValue(p_input, "fuga");
        BOOST_TEST_REQUIRE(result);
        const auto* const value = tetengo_lattice_stringInput_value(p_input);
        BOOST_TEST(std::string{ value } == "fuga");
    }
    {
        const auto result = tetengo_lattice_stringInput_setValue(nullptr, "fuga");
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

        const auto result = tetengo_lattice_stringInput_setValue(p_input, nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::string_input input{ "hoge" };

        BOOST_TEST(input.length() == 4U);
    }

    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        BOOST_TEST(tetengo_lattice_input_length(p_input) == 4U);
    }
    {
        BOOST_TEST(tetengo_lattice_input_length(nullptr) == static_cast<size_t>(-1));
    }
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::string_input input{ "hoge" };

        const auto p_clone = input.clone();
        BOOST_REQUIRE(p_clone);
        BOOST_TEST_REQUIRE(p_clone->is<tetengo::lattice::string_input>());
        BOOST_TEST(p_clone->as<tetengo::lattice::string_input>().value() == "hoge");
    }
}

BOOST_AUTO_TEST_CASE(create_subrange)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::string_input input{ "hoge" };

        const auto p_subrange = input.create_subrange(0, 4);
        BOOST_REQUIRE(p_subrange);
        BOOST_TEST_REQUIRE(p_subrange->is<tetengo::lattice::string_input>());
        BOOST_TEST(p_subrange->as<tetengo::lattice::string_input>().value() == "hoge");
    }
    {
        const tetengo::lattice::string_input input{ "hoge" };

        const auto p_subrange = input.create_subrange(1, 2);
        BOOST_REQUIRE(p_subrange);
        BOOST_TEST_REQUIRE(p_subrange->is<tetengo::lattice::string_input>());
        BOOST_TEST(p_subrange->as<tetengo::lattice::string_input>().value() == "og");
    }
    {
        const tetengo::lattice::string_input input{ "hoge" };

        const auto p_subrange = input.create_subrange(4, 0);
        BOOST_REQUIRE(p_subrange);
        BOOST_TEST_REQUIRE(p_subrange->is<tetengo::lattice::string_input>());
        BOOST_TEST(p_subrange->as<tetengo::lattice::string_input>().value() == "");
    }
    {
        const tetengo::lattice::string_input input{ "hoge" };

        BOOST_CHECK_THROW(const auto p_subrange = input.create_subrange(0, 5), std::out_of_range);
    }
    {
        const tetengo::lattice::string_input input{ "hoge" };

        BOOST_CHECK_THROW(const auto p_subrange = input.create_subrange(5, 0), std::out_of_range);
    }

    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto* const p_subrange = tetengo_lattice_input_createSubrange(p_input, 0, 4);
        BOOST_SCOPE_EXIT(p_subrange)
        {
            tetengo_lattice_input_destroy(p_subrange);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_subrange);
        BOOST_TEST(std::string{ tetengo_lattice_stringInput_value(p_subrange) } == "hoge");
    }
    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto* const p_subrange = tetengo_lattice_input_createSubrange(p_input, 1, 2);
        BOOST_SCOPE_EXIT(p_subrange)
        {
            tetengo_lattice_input_destroy(p_subrange);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_subrange);
        BOOST_TEST(std::string{ tetengo_lattice_stringInput_value(p_subrange) } == "og");
    }
    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto* const p_subrange = tetengo_lattice_input_createSubrange(p_input, 4, 0);
        BOOST_SCOPE_EXIT(p_subrange)
        {
            tetengo_lattice_input_destroy(p_subrange);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_subrange);
        BOOST_TEST(std::string{ tetengo_lattice_stringInput_value(p_subrange) } == "");
    }
    {
        const auto* const p_subrange = tetengo_lattice_input_createSubrange(nullptr, 0, 4);
        BOOST_TEST(!p_subrange);
    }
    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto* const p_subrange = tetengo_lattice_input_createSubrange(p_input, 0, 5);
        BOOST_TEST(!p_subrange);
    }
    {
        const auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        const auto* const p_subrange = tetengo_lattice_input_createSubrange(p_input, 5, 0);
        BOOST_TEST(!p_subrange);
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

    {
        auto* const p_input = tetengo_lattice_input_createStringInput("hoge");
        BOOST_SCOPE_EXIT(p_input)
        {
            tetengo_lattice_input_destroy(p_input);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_input);

        auto* const p_another = tetengo_lattice_input_createStringInput("fuga");
        BOOST_TEST_REQUIRE(p_another);
        const auto result = tetengo_lattice_input_append(p_input, p_another);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(std::string{ tetengo_lattice_stringInput_value(p_input) } == "hogefuga");
    }
    {
        auto* const p_another = tetengo_lattice_input_createStringInput("fuga");
        BOOST_TEST_REQUIRE(p_another);
        const auto result = tetengo_lattice_input_append(nullptr, p_another);
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

        const auto result = tetengo_lattice_input_append(p_input, nullptr);
        BOOST_TEST(!result);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
