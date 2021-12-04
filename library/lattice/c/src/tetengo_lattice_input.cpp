/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "tetengo_lattice_input.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/lattice/input.h>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/string_input.hpp>


tetengo_lattice_input_t* tetengo_lattice_input_createStringInput(const char* const value)
{
    try
    {
        if (!value)
        {
            throw std::invalid_argument{ "value is NULL." };
        }

        auto p_cpp_input = std::make_unique<tetengo::lattice::string_input>(value);

        auto p_instance = std::make_unique<tetengo_lattice_input_t>(std::move(p_cpp_input));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

namespace
{
    class custom_input : public tetengo::lattice::input
    {
    public:
        // constructors and destructor

        explicit custom_input(const tetengo_lattice_customInputDefinition_t* const p_definition) :
        m_p_definition{ p_definition }
        {}

        virtual ~custom_input() = default;


    private:
        // variables

        const tetengo_lattice_customInputDefinition_t* const m_p_definition;


        // virtual functions

        virtual std::size_t length_impl() const override
        {
            return m_p_definition->length_proc(m_p_definition->p_context);
        }

        virtual std::unique_ptr<input>
        create_subrange_impl(const std::size_t /*offset*/, const std::size_t /*length*/) const override
        {
            throw std::logic_error{ "Implement it." };
        }

        virtual void append_impl(std::unique_ptr<input>&& /*p_another*/) override
        {
            throw std::logic_error{ "Implement it." };
        }
    };

}

tetengo_lattice_input_t*
tetengo_lattice_input_createCustomInput(const tetengo_lattice_customInputDefinition_t* const p_definition)
{
    try
    {
        if (!p_definition)
        {
            throw std::invalid_argument{ "p_definition is NULL." };
        }

        auto p_cpp_input = std::make_unique<custom_input>(p_definition);

        auto p_instance = std::make_unique<tetengo_lattice_input_t>(std::move(p_cpp_input));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_input_destroy(const tetengo_lattice_input_t* const p_input)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_input_t> p_instance{ p_input };
    }
    catch (...)
    {}
}

size_t tetengo_lattice_input_length(const tetengo_lattice_input_t* const p_input)
{
    try
    {
        if (!p_input)
        {
            throw std::invalid_argument{ "p_input is NULL." };
        }

        return p_input->cpp_input().length();
    }
    catch (...)
    {
        return static_cast<size_t>(-1);
    }
}

tetengo_lattice_input_t* tetengo_lattice_input_createSubrange(
    const tetengo_lattice_input_t* const p_input,
    const size_t                         offset,
    const size_t                         length)
{
    try
    {
        if (!p_input)
        {
            throw std::invalid_argument{ "p_input is NULL." };
        }

        auto p_cpp_subrange = p_input->cpp_input().create_subrange(offset, length);

        auto p_instance = std::make_unique<tetengo_lattice_input_t>(std::move(p_cpp_subrange));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

int tetengo_lattice_input_append(tetengo_lattice_input_t* const p_input, tetengo_lattice_input_t* const p_another)
{
    try
    {
        BOOST_SCOPE_EXIT(p_another)
        {
            tetengo_lattice_input_destroy(p_another);
        }
        BOOST_SCOPE_EXIT_END;

        if (!p_input)
        {
            throw std::invalid_argument{ "p_input is NULL." };
        }
        if (!p_another)
        {
            throw std::invalid_argument{ "p_another is NULL." };
        }

        p_input->p_cpp_input()->append(std::move(p_another->p_cpp_input()));

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

const char* tetengo_lattice_stringInput_value(const tetengo_lattice_input_t* const p_string_input)
{
    try
    {
        if (!p_string_input)
        {
            throw std::invalid_argument{ "p_string_input is NULL." };
        }
        if (!p_string_input->cpp_input().is<tetengo::lattice::string_input>())
        {
            throw std::invalid_argument{ "p_string_input is not a string input." };
        }

        return p_string_input->cpp_input().as<tetengo::lattice::string_input>().value().c_str();
    }
    catch (...)
    {
        return nullptr;
    }
}

int tetengo_lattice_stringInput_setValue(tetengo_lattice_input_t* const p_string_input, const char* const value)
{
    try
    {
        if (!p_string_input)
        {
            throw std::invalid_argument{ "p_string_input is NULL." };
        }
        if (!p_string_input->cpp_input().is<tetengo::lattice::string_input>())
        {
            throw std::invalid_argument{ "p_string_input is not a string input." };
        }
        if (!value)
        {
            throw std::invalid_argument{ "value is NULL." };
        }

        p_string_input->p_cpp_input()->as<tetengo::lattice::string_input>().value() = value;

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}
