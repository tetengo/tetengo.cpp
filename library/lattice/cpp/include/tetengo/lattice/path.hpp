/*! \file
    \brief A path.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_PATH_HPP)
#define TETENGO_LATTICE_PATH_HPP

#include <vector>

#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    /*!
        \brief A path.
    */
    class path
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an empty path.
        */
        path();

        /*!
            \brief Creates a path.

            \param nodes Nodes.
            \param cost  A cost.
        */
        path(std::vector<node> nodes, int cost);


        // functions

        /*!
            \brief Returns true when this path is empty.

            \retval true  When this path is empty.
            \retval false Otherwise.
        */
        [[nodiscard]] bool empty() const;

        /*!
            \brief Returns the nodes.

            \return The nodes.
        */
        [[nodiscard]] const std::vector<node>& nodes() const;

        /*!
            \brief Returns the cost.

            \return The cost.
        */
        [[nodiscard]] int cost() const;


    private:
        // variables

        std::vector<node> m_nodes;

        int m_cost;
    };


}


#endif
