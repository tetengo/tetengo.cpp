/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_NODE_HPP)
#define TETENGO_LATTICE_NODE_HPP

#include <any>
#include <string_view>


namespace tetengo::lattice
{
    /*!
        \brief A node.
    */
    class node
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a node.

            \param key       A key.
            \param value     A value.
            \param node_cost A node cost.
            \param path_cost A path cost.
        */
        node(std::string_view key, std::any value, int node_cost, int path_cost);


        // functions

        /*!
            \brief Returns the key.

            \return The key.
        */
        const std::string_view& key() const;

        /*!
            \brief Returns the value.

            \return The value.
        */
        const std::any& value() const;

        /*!
            \brief Returns the node cost.

            \return The node cost.
        */
        int node_cost() const;

        /*!
            \brief Returns the path cost.

            \return The path cost.
        */
        int path_cost() const;


    private:
        // variables

        std::string_view m_key;

        std::any m_value;

        int m_node_cost;

        int m_path_cost;
    };


}


#endif
