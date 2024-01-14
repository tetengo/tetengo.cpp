/*! \file
    \brief A node.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <limits>
#include <vector>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    node node::bos(const std::vector<int>* const p_preceding_edge_costs)
    {
        return node{ entry_view::bos_eos(),
                     0,
                     std::numeric_limits<std::size_t>::max(),
                     p_preceding_edge_costs,
                     std::numeric_limits<std::size_t>::max(),
                     0 };
    }

    node node::eos(
        const std::size_t             preceding_step,
        const std::vector<int>* const p_preceding_edge_costs,
        const std::size_t             best_preceding_node,
        const int                     path_cost)
    {
        return node{ entry_view::bos_eos(), 0, preceding_step, p_preceding_edge_costs, best_preceding_node, path_cost };
    }

    bool node::is_bos() const
    {
        static const std::vector<int> bos_preceding_edge_costs{};
        static const node             bos_ = bos(&bos_preceding_edge_costs);
        return *this == bos_;
    }


}
