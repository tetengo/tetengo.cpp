/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_LATTICE_HPP)
#define TETENGO_LATTICE_LATTICE_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    class vocabulary;


    /*!
        \brief A lattice.
    */
    class lattice : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a lattice.

            \param p_vocabulary A unique pointer to a vocabulary.
        */
        explicit lattice(std::unique_ptr<vocabulary>&& p_vocabulary);

        /*!
            \brief Destroys the lattice.
        */
        ~lattice();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
