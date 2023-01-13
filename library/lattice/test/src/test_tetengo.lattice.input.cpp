/*! \file
    \brief An input.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <stdexcept>

#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.hpp>


namespace
{
    class concrete_input : public tetengo::lattice::input
    {
    public:
        explicit concrete_input(const int value = 42) : m_value{ value } {}

    private:
        const int m_value;

        virtual bool equal_to_impl(const input& another) const override
        {
            return another.as<concrete_input>().m_value == m_value;
        }

        virtual std::size_t hash_value_impl() const override
        {
            return 314159;
        }

        virtual std::size_t length_impl() const override
        {
            return 42;
        }

        virtual std::unique_ptr<input> clone_impl() const override
        {
            return std::make_unique<concrete_input>();
        }

        virtual std::unique_ptr<input>
        create_subrange_impl(const std::size_t offset, const std::size_t length) const override
        {
            if (offset + length > 42)
            {
                throw std::out_of_range{ "out of range." };
            }
            return std::make_unique<concrete_input>();
        }

        virtual void append_impl(std::unique_ptr<input>&& p_another) override
        {
            if (!p_another || !p_another->is<concrete_input>())
            {
                throw std::invalid_argument{ "p_another type mismatch." };
            }
        }
    };

    class concrete_input2 : public tetengo::lattice::input
    {
    private:
        virtual bool equal_to_impl(const input& /*another*/) const override
        {
            return true;
        }

        virtual std::size_t hash_value_impl() const override
        {
            return 271828;
        }

        virtual std::size_t length_impl() const override
        {
            return 0;
        }

        virtual std::unique_ptr<input> clone_impl() const override
        {
            return std::make_unique<concrete_input2>();
        }

        virtual std::unique_ptr<input>
        create_subrange_impl(const std::size_t /*offset*/, const std::size_t /*length*/) const override
        {
            return std::make_unique<concrete_input2>();
        }

        virtual void append_impl(std::unique_ptr<input>&& /*p_another*/) override {}
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(input)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_input input_{};
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_input input1{ 42 };
        const concrete_input input2{ 42 };

        BOOST_CHECK(input1 == input2);
        BOOST_CHECK(input2 == input1);
    }
    {
        const concrete_input input1{ 42 };
        const concrete_input input2{ 24 };

        BOOST_CHECK(input1 != input2);
        BOOST_CHECK(input2 != input1);
    }
    {
        const concrete_input  input1{ 42 };
        const concrete_input2 input2{};

        BOOST_CHECK(input1 != input2);
        BOOST_CHECK(input2 != input1);
    }
}

BOOST_AUTO_TEST_CASE(hash_value)
{
    BOOST_TEST_PASSPOINT();

    const concrete_input input_{};

    [[maybe_unused]] const auto hash_value_ = input_.hash_value();
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    const concrete_input input_{};

    BOOST_TEST(input_.length() == 42U);
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    const concrete_input input_{};

    const auto p_clone = input_.clone();
}

BOOST_AUTO_TEST_CASE(create_subrange)
{
    BOOST_TEST_PASSPOINT();

    const concrete_input input_{};

    {
        const auto p_subrange = input_.create_subrange(0, 42);
    }
    {
        const auto p_subrange = input_.create_subrange(42, 0);
    }
    {
        BOOST_CHECK_THROW(const auto p_subrange = input_.create_subrange(0, 43), std::out_of_range);
    }
    {
        BOOST_CHECK_THROW(const auto p_subrange = input_.create_subrange(43, 0), std::out_of_range);
    }
}

BOOST_AUTO_TEST_CASE(append)
{
    BOOST_TEST_PASSPOINT();

    concrete_input input_{};

    input_.append(std::make_unique<concrete_input>());
    BOOST_CHECK_THROW(input_.append(nullptr), std::invalid_argument);
    BOOST_CHECK_THROW(input_.append(std::make_unique<concrete_input2>()), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(is)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::input& input_ = concrete_input{};

    BOOST_TEST(input_.is<concrete_input>());
    BOOST_TEST(!input_.is<concrete_input2>());
}

BOOST_AUTO_TEST_CASE(as)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::input& input_ = concrete_input{};

        const auto& casted = input_.as<concrete_input>();
        BOOST_TEST(&casted == &input_);
    }
    {
        concrete_input           input_{};
        tetengo::lattice::input& input_ref = input_;

        const auto& casted = input_ref.as<concrete_input>();
        BOOST_TEST(&casted == &input_);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
