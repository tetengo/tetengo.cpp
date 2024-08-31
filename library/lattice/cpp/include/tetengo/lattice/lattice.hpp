/*! \file
    \brief A lattice.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_LATTICE_HPP)
#define TETENGO_LATTICE_LATTICE_HPP

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    class input;
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

            \param vocabulary_ A vocabulary.
        */
        explicit lattice(const vocabulary& vocabulary_);

        /*!
            \brief Destroys the lattice.
        */
        ~lattice();


        // functions

        /*!
            \brief Returns the step count.

            \return The step count.
        */
        [[nodiscard]] std::size_t step_count() const;

        /*!
            \brief Returns the nodes at the specified step.

            \param step A step.

            \return The nodes.

            \throw std::out_of_rage When step is too large.
        */
        [[nodiscard]] const std::vector<node>& nodes_at(std::size_t step) const;

        /*!
            \brief Pushes back an input.

            \param p_input A unique pointer to an input.
        */
        void push_back(std::unique_ptr<input>&& p_input);

        /*!
            \brief Settles this lattice.

            You can modify the lattice after settlement.
            Modification of the lattice after settlement invalidate the EOS node.

            \return The EOS node and its preceding edge costs.
        */
        [[nodiscard]] std::pair<node, std::unique_ptr<std::vector<int>>> settle();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
