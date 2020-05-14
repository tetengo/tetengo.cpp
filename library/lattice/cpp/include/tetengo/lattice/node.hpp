/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_NODE_HPP)
#define TETENGO_LATTICE_NODE_HPP

#include <any>
#include <cstddef>
#include <limits>
#include <string_view>
#include <vector>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    /*!
        \brief A node.
    */
    class node
    {
    public:
        // static functions

        /*!
            \brief Returns the BOS (Beginning of Sequence).

            \return The BOS.
        */
        static const node& bos();

        /*!
            \brief Returns an EOS (End of Sequence).

            \param preceding_step      An index of a preceding step.
            \param preceding_edge_costs Preceding edge costs.
            \param best_preceding_node An index of a best preceding node.
            \param path_cost           A path cost.

            \return An EOS.
        */
        static node
        eos(std::size_t      preceding_step,
            std::vector<int> preceding_edge_costs,
            std::size_t      best_preceding_node,
            int              path_cost);


        // constructors and destructor

        /*!
            \brief Creates a node.

            \param key                  A key.
            \param p_value              A pointer to a value.
            \param preceding_step       An index of a preceding step.
            \param preceding_edge_costs Preceding edge costs.
            \param best_preceding_node  An index of a best preceding node.
            \param node_cost            A node cost.
            \param path_cost            A path cost.

            \throw std::invalid_argument When p_value is nullptr.
        */
        node(
            std::string_view key,
            const std::any*  p_value,
            std::size_t      preceding_step,
            std::vector<int> preceding_edge_costs,
            std::size_t      best_preceding_node,
            int              node_cost,
            int              path_cost);

        /*!
            \brief Creates a node from a vocabulary entry.

            \param entry                An entry.
            \param preceding_step       An index of a preceding step.
            \param preceding_edge_costs Preceding edge costs.
            \param best_preceding_node  An index of a best preceding node.
            \param path_cost            A path cost.
        */
        explicit node(
            const entry_view& entry,
            std::size_t       preceding_step = std::numeric_limits<std::size_t>::max(),
            std::vector<int>  preceding_edge_costs = std::vector<int>{},
            std::size_t       best_preceding_node = std::numeric_limits<std::size_t>::max(),
            int               path_cost = std::numeric_limits<int>::max());


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
            \brief Returns the index of the preceding step.

            \return The index of the preceding step.
        */
        std::size_t preceding_step() const;

        /*!
            \brief Returns the preceding edge costs.

            \return The preceding edge costs.
        */
        const std::vector<int>& preceding_edge_costs() const;

        /*!
            \brief Returns the index of the best preceding node.

            \return The index of the best preceding node.
        */
        std::size_t best_preceding_node() const;

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

        /*!
            \brief Returns true is this node is the BOS.

            \retval true  When this node is the BOS.
            \retval false Otherwise.
        */
        bool is_bos() const;


    private:
        // variables

        std::string_view m_key;

        const std::any* m_p_value;

        std::size_t m_preceding_step;

        std::vector<int> m_preceding_edge_costs;

        std::size_t m_best_preceding_node;

        int m_node_cost;

        int m_path_cost;
    };


}


#endif
