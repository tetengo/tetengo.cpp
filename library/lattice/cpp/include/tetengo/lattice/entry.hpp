/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_ENTRY_HPP)
#define TETENGO_LATTICE_ENTRY_HPP

#include <string>
#include <string_view>


namespace tetengo::lattice
{
    /*!
        \brief A template of an entry.

        \tparam Key A key type.
    */
    template <typename Key>
    class basic_entry
    {
    public:
        // types

        //! The key type.
        using key_type = Key;


        // constructors

        /*!
            \brief Creates an entry.

            \param key     A key.
            \param surface A surface.
            \param cost    A cost.
        */
        basic_entry(key_type key, key_type surface, int cost);

        /*!
            \brief Copies an entry.

            \tparam K A key type.

            \param another Another entry.
        */
        template <typename K>
        basic_entry(const basic_entry<K>& another);


        // functions

        /*!
            \brief Returns the key.

            \return The key.
        */
        const key_type& key() const;

        /*!
            \brief Returns the surface.

            \return The surface.
        */
        const key_type& surface() const;

        /*!
            \brief Returns the cost.

            \return The cost.
        */
        int cost() const;


    private:
        // variables

        key_type m_key;

        key_type m_surface;

        int m_cost;
    };


    /*!
        \brief An entry.
    */
    using entry = basic_entry<std::string>;

    /*!
        \brief An entry view.
    */
    using entry_view = basic_entry<std::string_view>;


}


#endif
