/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_LATTICE_HPP)
#define TETENGO_LATTICE_LATTICE_HPP

#include <cstddef>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

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
            \brief Returns the step count.

            \return The step count.
        */
        std::size_t step_count() const;

        /*!
            \brief Returns the nodes at the specified step.

            \param step A step.

            \return The nodes.

            \throw std::out_of_rage When step is too large.
        */
        const std::vector<node>& nodes_at(std::size_t step) const;

        /*!
            \brief Pushes back an input.

            \param input An input.
        */
        void push_back(const std::string_view& input);

        /*!
            \brief Settles this lattice.

            You can modify the lattice after settlement.
            Modification of the lattice after settlement invalidate the EOS node.

            \return The EOS node and its preceding edge costs.
        */
        std::pair<node, std::unique_ptr<std::vector<int>>> settle();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
