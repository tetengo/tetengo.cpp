/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_INPUT_HPP)
#define TETENGO_LATTICE_INPUT_HPP

#include <cassert>
#include <cstddef>
#include <utility>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    /*!
        \brief An input base.
    */
    class input_base : private boost::noncopyable
    {
    public:
        // constructors

        /*!
            \brief Destroys the input base.
        */
        virtual ~input_base();


        // functions

        /*!
            \brief Returns the length.

            \return The length.
        */
        [[nodiscard]] std::size_t length() const;

        /*!
            \brief Returns true when this object can be casted to the specified concrete input.

            \tparam C A concrete input type.

            \retval true  When this object can be casted to the specified concrete input.
            \retval false Otherwise.
        */
        template <typename C>
        [[nodiscard]] bool is() const
        {
            return dynamic_cast<const C*>(this);
        }

        /*!
            \brief Casts this object to the specified concrete input.

            \tparam C A concrete input type.

            \return The casted concrete vocabulary.
        */
        template <typename C>
        [[nodiscard]] const C& as() const
        {
            assert(is<C>());
            return static_cast<const C&>(*this);
        }

        /*!
            \brief Casts this object to the specified concrete input.

            \tparam C A concrete input type.

            \return The casted concrete vocabulary.
        */
        template <typename C>
        [[nodiscard]] C& as()
        {
            assert(is<C>());
            return static_cast<C&>(*this);
        }


    private:
        // virtual functions

        virtual std::size_t length_impl() const = 0;
    };


    /*!
        \brief An input.

        \tparam T A type.
    */
    template <typename T>
    class input : public input_base
    {
    public:
        // types

        //! The value type
        using value_type = T;


        // constructors and destructor

        /*!
            \brief Creates an input key.

            \param value A value.
        */
        explicit input(value_type value) : m_value{ std::move(value) } {}

        /*!
            \brief Destroys the input.
        */
        virtual ~input() = default;


        // functions

        /*!
            \brief Returns the value.

            \return The value.
        */
        const value_type& value() const
        {
            return m_value;
        }

        /*!
            \brief Returns the value.

            \return The value.
        */
        value_type& value()
        {
            return m_value;
        }


    private:
        // variables

        value_type m_value;


        // virtual functions

        virtual std::size_t length_impl() const override
        {
            return 0;
        }
    };


}


#endif
