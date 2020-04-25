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
    /*!
        \brief A lattice.
    */
    class lattice : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a lattice.
        */
        lattice();

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
