/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <stdexcept>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/input.hpp>


namespace
{
    class concrete_input : public tetengo::lattice::input
    {
    private:
        virtual std::size_t length_impl() const override
        {
            return 42;
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
        virtual std::size_t length_impl() const override
        {
            return 0;
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

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    const concrete_input input_{};

    BOOST_TEST(input_.length() == 42U);
}

BOOST_AUTO_TEST_CASE(append)
{
    BOOST_TEST_PASSPOINT();

    concrete_input input_{};

    input_.append(std::make_unique<concrete_input>());
    BOOST_CHECK_THROW(input_.append(std::unique_ptr<concrete_input>{}), std::invalid_argument);
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
