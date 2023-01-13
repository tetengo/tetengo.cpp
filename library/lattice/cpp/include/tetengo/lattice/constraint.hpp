/*! \file
    \brief A constraint.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONSTRAINT_HPP)
#define TETENGO_LATTICE_CONSTRAINT_HPP

#include <memory>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    class constraint_element;
    class node;


    /*!
        \brief A constraint.
    */
    class constraint : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an empty constraint.

            It matches any path.
        */
        constraint();

        /*!
            \brief Creates a constraint.

            \param pattern A pattern.
        */
        explicit constraint(std::vector<std::unique_ptr<constraint_element>>&& pattern);

        /*!
            \brief Destroys the constraint.
        */
        ~constraint();


        // functions

        /*!
            \brief Returns true when the path matches the pattern.

            \param reverse_path A path in reverse order.

            \retval true  When the path matches the pattern.
            \retval false Otherwise.
        */
        [[nodiscard]] bool matches(const std::vector<node>& reverse_path) const;

        /*!
            \brief Returns true when the tail path matches the tail of the pattern.

            \param reverse_tail_path A tail path in reverse order.

            \retval true  When the tail path matches the tail of the pattern.
            \retval false Otherwise.
        */
        [[nodiscard]] bool matches_tail(const std::vector<node>& reverse_tail_path) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
