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

        \tparam String A string type.
    */
    template <typename String>
    class basic_entry
    {
    public:
        // types

        //! The string type.
        using string_type = String;


        // constructors

        /*!
            \brief Creates an entry.

            \param key     A key.
            \param surface A surface.
            \param cost    A cost.
        */
        basic_entry(string_type key, string_type surface, int cost);

        /*!
            \brief Copies an entry.

            \tparam S A string type.

            \param another Another entry.
        */
        template <typename S>
        basic_entry(const basic_entry<S>& another);


        // functions

        /*!
            \brief Returns the key.

            \return The key.
        */
        const string_type& key() const;

        /*!
            \brief Returns the surface.

            \return The surface.
        */
        const string_type& surface() const;

        /*!
            \brief Returns the cost.

            \return The cost.
        */
        int cost() const;


    private:
        // variables

        string_type m_key;

        string_type m_surface;

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
