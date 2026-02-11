/*! \file
    \brief A path.

    $12026 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <utility>
#include <vector>

#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/path.hpp>


namespace tetengo::lattice
{
    path::path() : m_nodes{}, m_cost{ 0 } {}

    path::path(std::vector<node> nodes, const int cost) : m_nodes{ std::move(nodes) }, m_cost{ cost } {}

    bool path::empty() const
    {
        return std::empty(m_nodes);
    }

    const std::vector<node>& path::nodes() const
    {
        return m_nodes;
    }

    int path::cost() const
    {
        return m_cost;
    }


}
