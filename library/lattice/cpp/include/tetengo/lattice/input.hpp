/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_INPUT_HPP)
#define TETENGO_LATTICE_INPUT_HPP

#include <cassert>
#include <cstddef>

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


}


#endif
