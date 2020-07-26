/*! \file
    \brief A reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
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
            return m_index + 1 < m_values.length();
        }

        virtual char get_impl() const override
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

BOOST_AUTO_TEST_CASE(get)
{
    BOOST_TEST_PASSPOINT();

    const concrete_reader reader{};

    BOOST_TEST(reader.get() == 'A');
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    concrete_reader reader{};

    reader.next();
    BOOST_TEST(reader.get() == 'B');
    BOOST_TEST_REQUIRE(reader.has_next());

    reader.next();
    BOOST_TEST(reader.get() == 'C');
    BOOST_TEST(!reader.has_next());

    reader.next();
    BOOST_CHECK_THROW(reader.get(), std::logic_error);

    BOOST_CHECK_THROW(reader.next(), std::logic_error);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
