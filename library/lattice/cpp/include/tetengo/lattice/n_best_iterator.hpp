/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_NBESTITERATOR_HPP)
#define TETENGO_LATTICE_NBESTITERATOR_HPP

#include <functional>
#include <iterator>
#include <queue>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/operators.hpp>

#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    class lattice;


    /*!
        \brief A cap.
    */
    class cap : public boost::less_than_comparable<cap>
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a cap.

            \param tail_path       A tail path.
            \param tail_path_cost  A tail path cost.
            \param whole_path_cost A whole path cost.
        */
        cap(std::vector<node> tail_path, int tail_path_cost, int whole_path_cost);


        // functions

        /*!
            \brief Returns true if one is less than another.

            \param one One cap.
            \param another Another cap.

            \retval true  When one is less than another.
            \retval false Otherwise.
        */
        friend bool operator<(const cap& one, const cap& another);

        /*!
            \brief Returns the tail path.

            \return The tail path.
        */
        const std::vector<node>& tail_path() const;

        /*!
            \brief Returns the tail path cost.

            \return The tail path cost.
        */
        int tail_path_cost() const;

        /*!
            \brief Returns the whole path cost.

            \return The whole path cost.
        */
        int whole_path_cost() const;


    private:
        // variables

        std::vector<node> m_tail_path;

        int m_tail_path_cost;

        int m_whole_path_cost;
    };


    /*!
        \brief An N-best lattice path iterator.
    */
    class n_best_iterator :
    public boost::iterator_facade<n_best_iterator, std::vector<node>, std::input_iterator_tag, std::vector<node>>
    {
    public:
        // friends

        //! Allows boost::iterator_facade to access the private members.
        friend class boost::iterators::iterator_core_access;


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

        std::priority_queue<cap, std::vector<cap>, std::greater<cap>> m_caps;


        // functions

        std::vector<node> dereference() const;

        void increment();
    };


}


#endif
