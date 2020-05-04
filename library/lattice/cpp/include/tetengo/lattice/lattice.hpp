/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_LATTICE_HPP)
#define TETENGO_LATTICE_LATTICE_HPP

#include <memory>
#include <string_view>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    class vocabulary;
    class node;


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


        // functions

        /*!
            \brief Pushes back an input.

            \param input An input.
        */
        void push_back(const std::string_view& input);

        /*!
            \brief Settles this lattice.

            You can modify the lattice after settlement.
            Modification of the lattice after settlement invalidate the EOS node.

            \return The EOS node.
        */
        node settle();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
