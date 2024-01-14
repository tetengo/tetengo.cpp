/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_NBESTITERATOR_HPP)
#define TETENGO_LATTICE_NBESTITERATOR_HPP

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <queue>
#include <vector>

#include <boost/operators.hpp>
#include <boost/stl_interfaces/iterator_interface.hpp>

#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/path.hpp>


namespace tetengo::lattice
{
    class constraint;
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
        [[nodiscard]] const std::vector<node>& tail_path() const;

        /*!
            \brief Returns the tail path cost.

            \return The tail path cost.
        */
        [[nodiscard]] int tail_path_cost() const;

        /*!
            \brief Returns the whole path cost.

            \return The whole path cost.
        */
        [[nodiscard]] int whole_path_cost() const;


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
    public boost::stl_interfaces::iterator_interface<n_best_iterator, std::forward_iterator_tag, path>
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

            \param lattice_     A lattice.
            \param eos_node     An EOS node.
            \param p_constraint A unique pointer to a constraint.

            \throw std::invalid_argument When p_constraint is nullptr.
        */
        n_best_iterator(const lattice& lattice_, node eos_node, std::unique_ptr<constraint>&& p_constraint);


        // functions

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] const path& operator*() const;

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] path& operator*();

        /*!
            \brief Returns the pointer to the value.

            \return The pointer to the value.
        */
        [[nodiscard]] const path* operator->() const;

        /*!
            \brief Returns the pointer to the value.

            \return The pointer to the value.
        */
        [[nodiscard]] path* operator->();

        /*!
            \brief Returns true when one iterator is equal to another.

            \param one   One iterator.
            \param another Another iterator.

            \retval true  When one is equal to another.
            \retval false Otherwise.
        */
        friend bool operator==(const n_best_iterator& one, const n_best_iterator& another);

        /*!
            \brief Increments the iterator.

            \return This iterator.
        */
        n_best_iterator& operator++();

        /*!
            \brief Postincrements the iterator.

            \return The iterator before the incrementation.
        */
        n_best_iterator operator++(int);


    private:
        // variables

        const lattice* m_p_lattice;

        std::priority_queue<cap, std::vector<cap>, std::greater<cap>> m_caps;

        std::size_t m_eos_hash;

        std::shared_ptr<constraint> m_p_constraint;

        path m_path;

        std::size_t m_index;
    };


}


#endif
