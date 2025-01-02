/*! \file
    \brief A node constraint element.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_NODECONSTRAINTELEMENT_HPP)
#define TETENGO_LATTICE_NODECONSTRAINTELEMENT_HPP

#include <memory>

#include <tetengo/lattice/constraint_element.hpp>


namespace tetengo::lattice
{
    class node;


    /*!
        \brief A node constraint element.
    */
    class node_constraint_element : public constraint_element
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a node constraint element.

            \param node_ A node.
        */
        explicit node_constraint_element(node node_);

        /*!
            \brief Destroys the node constraint element.
        */
        virtual ~node_constraint_element();


    private:
        // types

        class impl;


        // variables

        std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual int matches_impl(const node& node_) const override;
    };


}


#endif
