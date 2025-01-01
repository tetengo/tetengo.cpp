/*! \file
    \brief An input.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
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

        custom_input(const tetengo_lattice_customInputDefinition_t& definition, const bool is_subrange) :
        m_definition{ definition },
        m_is_subrange{ is_subrange }
        {}

        virtual ~custom_input()
        {
            if (m_is_subrange)
            {
                m_definition.destroy_subraneg_context_proc(m_definition.p_context);
            }
        }


        // functions

        [[nodiscard]] const void* p_context() const
        {
            return m_definition.p_context;
        }

        void set_context(void* const p_context)
        {
            m_definition.p_context = p_context;
        }


    private:
        // variables

        tetengo_lattice_customInputDefinition_t m_definition;

        const bool m_is_subrange;


        // virtual functions

        virtual bool equal_to_impl(const input& another) const override
        {
            return m_definition.equal_to_proc(
                m_definition.p_context, another.as<custom_input>().m_definition.p_context);
        }

        virtual std::size_t hash_value_impl() const override
        {
            return m_definition.hash_value_proc(m_definition.p_context);
        }

        virtual std::size_t length_impl() const override
        {
            return m_definition.length_proc(m_definition.p_context);
        }

        virtual std::unique_ptr<input> clone_impl() const override
        {
            tetengo_lattice_customInputDefinition_t subrange_definition = m_definition;
            subrange_definition.p_context = m_definition.create_subrange_context_proc(
                m_definition.p_context, 0, m_definition.length_proc(m_definition.p_context));
            return std::make_unique<custom_input>(subrange_definition, true);
        }

        virtual std::unique_ptr<input>
        create_subrange_impl(const std::size_t offset, const std::size_t length) const override
        {
            if (offset + length > this->length())
            {
                throw std::out_of_range{ "offset and/or length are out of the range." };
            }

            tetengo_lattice_customInputDefinition_t subrange_definition = m_definition;
            subrange_definition.p_context =
                m_definition.create_subrange_context_proc(m_definition.p_context, offset, length);
            return std::make_unique<custom_input>(subrange_definition, true);
        }

        virtual void append_impl(std::unique_ptr<input>&& p_another) override
        {
            if (!p_another->is<custom_input>())
            {
                throw std::invalid_argument{ "p_another is not a custom input." };
            }

            const auto result =
                m_definition.append_proc(m_definition.p_context, p_another->as<custom_input>().m_definition.p_context);
            if (!result)
            {
                throw std::runtime_error{ "The input appending failed." };
            }
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

        auto p_cpp_input = std::make_unique<custom_input>(*p_definition, false);

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

bool tetengo_lattice_input_equal(
    const tetengo_lattice_input_t* const p_one,
    const tetengo_lattice_input_t* const p_another)
{
    try
    {
        if (!p_one)
        {
            throw std::invalid_argument{ "p_one is NULL." };
        }
        if (!p_another)
        {
            throw std::invalid_argument{ "p_another is NULL." };
        }

        return p_one->cpp_input() == p_another->cpp_input();
    }
    catch (...)
    {
        return false;
    }
}

size_t tetengo_lattice_input_hashValue(const tetengo_lattice_input_t* p_input)
{
    try
    {
        if (!p_input)
        {
            throw std::invalid_argument{ "p_input is NULL." };
        }

        return p_input->cpp_input().hash_value();
    }
    catch (...)
    {
        return static_cast<size_t>(-1);
    }
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

tetengo_lattice_input_t* tetengo_lattice_input_clone(const tetengo_lattice_input_t* const p_input)
{
    try
    {
        if (!p_input)
        {
            throw std::invalid_argument{ "p_input is NULL." };
        }

        auto p_cpp_clone = p_input->cpp_input().clone();

        auto p_instance = std::make_unique<tetengo_lattice_input_t>(std::move(p_cpp_clone));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
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

bool tetengo_lattice_input_append(tetengo_lattice_input_t* const p_input, tetengo_lattice_input_t* const p_another)
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

        return true;
    }
    catch (...)
    {
        return false;
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

bool tetengo_lattice_stringInput_setValue(tetengo_lattice_input_t* const p_string_input, const char* const value)
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

        return true;
    }
    catch (...)
    {
        return false;
    }
}

const void* tetengo_lattice_customInput_context(const tetengo_lattice_input_t* const p_custom_input)
{
    try
    {
        if (!p_custom_input)
        {
            throw std::invalid_argument{ "p_custom_input is NULL." };
        }
        if (!p_custom_input->cpp_input().is<custom_input>())
        {
            throw std::invalid_argument{ "p_custom_input is not a custom input." };
        }

        return p_custom_input->cpp_input().as<custom_input>().p_context();
    }
    catch (...)
    {
        return nullptr;
    }
}


bool tetengo_lattice_customInput_setContext(tetengo_lattice_input_t* const p_custom_input, void* const p_context)
{
    try
    {
        if (!p_custom_input)
        {
            throw std::invalid_argument{ "p_custom_input is NULL." };
        }
        if (!p_custom_input->cpp_input().is<custom_input>())
        {
            throw std::invalid_argument{ "p_custom_input is not a custom input." };
        }

        p_custom_input->p_cpp_input()->as<custom_input>().set_context(p_context);

        return true;
    }
    catch (...)
    {
        return false;
    }
}
