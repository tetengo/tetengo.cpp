/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONSTRAINT_HPP)
#define TETENGO_LATTICE_CONSTRAINT_HPP

#include <memory>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::lattice
{
    class constraint_element;


    /*!
        \brief A constraint.
    */
    class constraint : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a constraint.

            \param pattern A pattern.
        */
        explicit constraint(std::vector<std::unique_ptr<constraint_element>>&& pattern);

        /*!
            \brief Destroys the constraint.
        */
        ~constraint();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
