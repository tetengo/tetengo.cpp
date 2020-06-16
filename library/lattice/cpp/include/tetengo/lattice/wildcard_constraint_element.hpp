/*! \file
    \brief A wildcard constraint element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_WILDCARDCONSTRAINTELEMENT_HPP)
#define TETENGO_LATTICE_WILDCARDCONSTRAINTELEMENT_HPP

#include <cstddef>
#include <memory>

#include <tetengo/lattice/constraint_element.hpp>


namespace tetengo::lattice
{
    class node;


    /*!
        \brief A wildcard constraint element.
    */
    class wildcard_constraint_element : public constraint_element
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a wildcard constraint element.

            \param preceding_step An index of a preceding step.
        */
        explicit wildcard_constraint_element(std::size_t preceding_step);

        /*!
            \brief Destroys the node constraint element.
        */
        virtual ~wildcard_constraint_element();


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
