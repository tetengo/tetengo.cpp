/*! \file
    \brief A string input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/string_input.hpp>


namespace tetengo::lattice
{
    class string_input::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::string value) : m_value{ std::move(value) } {}


        // functions

        const std::string& value() const
        {
            return m_value;
        }

        std::string& value()
        {
            return m_value;
        }

        std::size_t length_impl() const
        {
            return m_value.length();
        }

        void append_impl(std::unique_ptr<input>&& p_another)
        {
            if (!p_another)
            {
                throw std::invalid_argument{ "p_another is nullptr." };
            }
            if (!p_another->is<string_input>())
            {
                throw std::invalid_argument{ "Mismatch type of p_another." };
            }

            m_value += std::move(static_cast<string_input&>(*p_another).value());
        }


    private:
        // variables

        std::string m_value;
    };


    string_input::string_input(std::string value) : m_p_impl{ std::make_unique<impl>(std::move(value)) } {}

    string_input::~string_input() = default;

    const std::string& string_input::value() const
    {
        return m_p_impl->value();
    }

    std::string& string_input::value()
    {
        return m_p_impl->value();
    }

    std::size_t string_input::length_impl() const
    {
        return m_p_impl->length_impl();
    }

    void string_input::append_impl(std::unique_ptr<input>&& p_another)
    {
        m_p_impl->append_impl(std::move(p_another));
    }

}
