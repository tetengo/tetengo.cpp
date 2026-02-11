/*! \file
    \brief A constraint element.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONSTRAINTELEMENT_HPP)
#define TETENGO_LATTICE_CONSTRAINTELEMENT_HPP

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    class node;


    /*!
        \brief A constraint element.
    */
    class constraint_element : private boost::noncopyable
    {
    public:
        // constructors

        /*!
            \brief Destroys the constraint element.
        */
        virtual ~constraint_element();


        // functions

        /*!
            \brief Returns whether this constraint element matches the specified node.

            \param node_ A node.

            \retval positive This constraint element matches the specified node, and also may match its preceding nodes.
            \retval 0        This constraint element matches the specified node, and do not match its precedings nodes.
            \retval negative This constraint element does not match the specified node.
        */
        int matches(const node& node_) const;


    private:
        // virtual functions

        virtual int matches_impl(const node& node_) const = 0;
    };


}


#endif
