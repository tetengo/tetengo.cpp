/*! \file
    \brief A vocabulary key.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_VOCABULARYKEY_HPP)
#define TETENGO_LATTICE_VOCABULARYKEY_HPP

#include <cassert>
#include <utility>

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


    /*!
        \brief A vocabulary key.

        \tparam T A type.
    */
    template <typename T>
    class vocabulary_key : public vocabulary_key_base
    {
    public:
        // types

        //! The value type
        using value_type = T;


        // constructors and destructor

        /*!
            \brief Creates a vocabulary key.

            \param value A value.
        */
        explicit vocabulary_key(value_type value) : m_value{ std::move(value) } {}

        /*!
            \brief Destroys the vocabulary key.
        */
        virtual ~vocabulary_key() = default;


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
    };


}


#endif
