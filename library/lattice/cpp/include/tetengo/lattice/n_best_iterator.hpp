/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_NBESTITERATOR_HPP)
#define TETENGO_LATTICE_NBESTITERATOR_HPP

#include <iterator>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>

#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    class lattice;


    /*!
        \brief An N-best lattice path iterator.
    */
    class n_best_iterator : public boost::iterator_facade<n_best_iterator, std::vector<node>, std::input_iterator_tag>
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an iterator.

            It points to the last of the paths.
        */
        n_best_iterator();

        /*!
            \brief Creates an iterator.

            \param lattice_ A lattice.
            \param eos_node An EOS node.
        */
        n_best_iterator(const lattice& lattice_, node eos_node);


    private:
        // variables

        const lattice* m_p_lattice;

        node m_eos_node;
    };


}


#endif
