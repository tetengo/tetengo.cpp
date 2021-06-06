/*! \file
    \brief A reader.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <stdexcept>
#include <string>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/reader.hpp>


namespace
{
    class concrete_reader : public tetengo::json::reader
    {
    public:
        // constructors and destructor

        concrete_reader() : m_values{ "ABC" }, m_index{ 0 } {}

        virtual ~concrete_reader() = default;


    private:
        // variables

        const std::string m_values;

        std::size_t m_index;


        // virtual functions

        virtual bool has_next_impl() const override
        {
            return m_index < m_values.length();
        }

        virtual char peek_impl() const override
        {
            if (m_index >= m_values.length())
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            return m_values[m_index];
        }

        virtual void next_impl() override
        {
            if (m_index >= m_values.length())
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            ++m_index;
        }

        virtual const reader& base_reader_impl() const override
        {
            throw std::logic_error{ "No base reader." };
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(rader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_reader reader{};
}

BOOST_AUTO_TEST_CASE(has_next)
{
    BOOST_TEST_PASSPOINT();

    const concrete_reader reader{};

    BOOST_TEST(reader.has_next());
}

BOOST_AUTO_TEST_CASE(peek)
{
    BOOST_TEST_PASSPOINT();

    const concrete_reader reader{};

    BOOST_TEST(reader.peek() == 'A');
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    concrete_reader reader{};

    BOOST_TEST_REQUIRE(reader.has_next());
    BOOST_TEST(reader.peek() == 'A');
    reader.next();

    BOOST_TEST_REQUIRE(reader.has_next());
    BOOST_TEST(reader.peek() == 'B');
    reader.next();

    BOOST_TEST_REQUIRE(reader.has_next());
    BOOST_TEST(reader.peek() == 'C');
    reader.next();

    BOOST_TEST(!reader.has_next());
    BOOST_CHECK_THROW([[maybe_unused]] const auto& peeked = reader.peek(), std::logic_error);
    BOOST_CHECK_THROW(reader.next(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(base_reader)
{
    BOOST_TEST_PASSPOINT();

    const concrete_reader reader{};

    BOOST_CHECK_THROW([[maybe_unused]] const auto& base_reader = reader.base_reader(), std::logic_error);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
