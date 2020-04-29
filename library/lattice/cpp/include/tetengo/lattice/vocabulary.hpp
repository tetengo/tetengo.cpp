/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_VOCABULARY_HPP)
#define TETENGO_LATTICE_VOCABULARY_HPP

#include <string_view>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    class connection;


    /*!
        \brief A vocabulary.
    */
    class vocabulary : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a vocabulary.
        */
        vocabulary();

        /*!
            \brief Destroys the vocabulary.
        */
        virtual ~vocabulary();


        // functions

        /*!
            \brief Finds entries.

            \param key A key.

            \return Entry views.
        */
        std::vector<entry_view> find_entries(const std::string_view& key) const;

        /*!
            \brief Finds a connection between entries.

            \param from An origin entry view.
            \param to   A destination entry view.

            \return A connection between the entries.
        */
        connection find_connection(const entry_view& from, const entry_view& to) const;


    private:
        // virtual functions

        virtual std::vector<entry_view> find_entries_impl(const std::string_view& key) const = 0;

        virtual connection find_connection_impl(const entry_view& from, const entry_view& to) const = 0;
    };


}


#endif
