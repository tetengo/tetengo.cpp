/*! \file
    \brief A vocabulary key.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_VOCABULARYKEY_HPP)
#define TETENGO_LATTICE_VOCABULARYKEY_HPP

#include <cassert>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    /*!
        \brief A vocabulary key base.
    */
    class vocabulary_key_base : private boost::noncopyable
    {
    public:
        // constructors

        /*!
            \brief Destroys the vocabulary key base.
        */
        virtual ~vocabulary_key_base() = 0;


        // functions

        /*!
            \brief Returns true when this object can be casted to the specified concrete vocabulary key.

            \tparam C A concrete vocabulary key type.

            \retval true  When this object can be casted to the specified concrete vocabulary key.
            \retval false Otherwise.
        */
        template <typename C>
        bool is() const
        {
            return dynamic_cast<const C*>(this);
        }

        /*!
            \brief Casts this object to the specified concrete vocabulary key.

            \tparam C A concrete vocabulary key type.

            \return The casted concrete vocabulary.
        */
        template <typename C>
        const C& as() const
        {
            assert(is<C>());
            return static_cast<const C&>(*this);
        }

        /*!
            \brief Casts this object to the specified concrete vocabulary key.

            \tparam C A concrete vocabulary key type.

            \return The casted concrete vocabulary.
        */
        template <typename C>
        C& as()
        {
            assert(is<C>());
            return static_cast<C&>(*this);
        }
    };


}


#endif
