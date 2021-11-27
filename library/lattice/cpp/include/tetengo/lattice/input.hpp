/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_INPUT_HPP)
#define TETENGO_LATTICE_INPUT_HPP

#include <cassert>
#include <cstddef>
#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    /*!
        \brief An input.
    */
    class input : private boost::noncopyable
    {
    public:
        // constructors

        /*!
            \brief Destroys the input.
        */
        virtual ~input();


        // functions

        /*!
            \brief Returns the length.

            \return The length.
        */
        [[nodiscard]] std::size_t length() const;

        /*!
            \brief Appends another input.

            \param p_another A unique pointer to another input.

            \throw std::invalid_argument When p_another is nullptr or its type does not match.
        */
        void append(std::unique_ptr<input>&& p_another);

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

        virtual void append_impl(std::unique_ptr<input>&& p_another) = 0;
    };


}


#endif
