/*! \file
    \brief A vocabulary key.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_VOCABULARYKEY_HPP)
#define TETENGO_LATTICE_VOCABULARYKEY_HPP

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
        virtual ~vocabulary_key_base();


        // functions


    private:
        // virtual functions
    };


}


#endif
