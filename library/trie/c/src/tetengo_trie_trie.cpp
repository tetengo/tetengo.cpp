/*! \file
    \brief A trie.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>

struct tetengo_trie_trie;
struct tetengo_trie_trieIterator;


namespace
{
    using trie_type = tetengo::trie::trie<std::string_view, const void*>;
}


void tetengo_trie_trie_nullAddingObserver(const char*, void*) {}

void tetengo_trie_trie_nullDoneObserver(void*) {}

size_t tetengo_trie_trie_defaultDoubleArrayDensityFactor()
{
    return tetengo::trie::trie_impl::default_double_array_density_factor();
}

tetengo_trie_trie* tetengo_trie_trie_create(
    const tetengo_trie_trie_element_t* const p_elements,
    const size_t                             element_count,
    const tetengo_trie_trie_addingObserver_t adding_observer,
    void* const                              p_adding_observer_context,
    const tetengo_trie_trie_doneObserver_t   done_observer,
    void* const                              p_done_observer_context,
    const size_t                             double_array_density_factor)
{
    std::vector<std::pair<std::string_view, const void*>> elements{};
    elements.reserve(element_count);
    std::transform(p_elements, p_elements + element_count, std::back_inserter(elements), [](const auto& e) {
        return std::make_pair(e.key, e.p_value);
    });

    const trie_type::building_observer_set_type observer_set{
        [adding_observer, p_adding_observer_context](const std::string_view& key) {
            adding_observer(std::string{ key }.c_str(), p_adding_observer_context);
        },
        [done_observer, p_done_observer_context]() { done_observer(p_done_observer_context); }
    };
    auto p_trie = std::make_unique<trie_type>(
        elements.begin(),
        elements.end(),
        tetengo::trie::default_serializer<std::string_view>{},
        observer_set,
        double_array_density_factor);

    return reinterpret_cast<tetengo_trie_trie*>(p_trie.release());
}

void tetengo_trie_trie_destroy(tetengo_trie_trie* const p_trie)
{
    const std::unique_ptr<trie_type> p_cpp_trie{ reinterpret_cast<trie_type*>(p_trie) };
}

int tetengo_trie_trie_empty(const tetengo_trie_trie* const p_trie)
{
    const auto* const p_cpp_trie = reinterpret_cast<const trie_type*>(p_trie);
    return p_cpp_trie->empty() ? 1 : 0;
}

size_t tetengo_trie_trie_size(const tetengo_trie_trie* const p_trie)
{
    const auto* const p_cpp_trie = reinterpret_cast<const trie_type*>(p_trie);
    return p_cpp_trie->size();
}

int tetengo_trie_trie_contains(const tetengo_trie_trie* const p_trie, const char* const key)
{
    const auto* const p_cpp_trie = reinterpret_cast<const trie_type*>(p_trie);
    return p_cpp_trie->contains(key);
}

const void* tetengo_trie_trie_find(const tetengo_trie_trie* const p_trie, const char* const key)
{
    const auto* const p_cpp_trie = reinterpret_cast<const trie_type*>(p_trie);
    return p_cpp_trie->find(key);
}

tetengo_trie_trieIterator* tetengo_trie_trie_createIterator(const tetengo_trie_trie* p_trie)
{
    return tetengo_trie_trieIterator_create(p_trie);
}

void tetengo_trie_trie_destroyIterator(tetengo_trie_trieIterator* p_iterator)
{
    tetengo_trie_trieIterator_destroy(p_iterator);
}

const tetengo_trie_trie* tetengo_trie_trie_subtrie(const tetengo_trie_trie* const p_trie, const char* const key_prefix)
{
    const auto* const p_cpp_trie = reinterpret_cast<const trie_type*>(p_trie);
    auto              p_subtrie = p_cpp_trie->subtrie(key_prefix);
    return reinterpret_cast<tetengo_trie_trie*>(p_subtrie.release());
}
