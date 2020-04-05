/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_VOCABULARY_HPP)
#define TETENGO_LATTICE_VOCABULARY_HPP

#include <string_view>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    class entry;


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

            \return Entries.
        */
        std::vector<entry> find(const std::string_view& key) const;


    private:
        // virtual functions

        virtual std::vector<entry> find_impl(const std::string_view& key) const = 0;
    };


}


#endif
