/*! \file
    \brief A double array builder.

    Copyright (C) 2019 kaoru
*/

#if !defined(DOCUMENTATION)

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <unordered_set>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
    std::int32_t double_array_builder::default_density_factor()
    {
        return 1000;
    }

    memory_storage double_array_builder::build(
        std::vector<const std::pair<std::string, std::int32_t>*> element_pointers,
        const double_array::building_observer_type&              observer,
        const std::int32_t                                       density_factor)
    {
        if (density_factor <= 0)
        {
            throw std::invalid_argument{ "density_factor must be greater than 0." };
        }

        std::stable_sort(element_pointers.begin(), element_pointers.end(), [](const auto& e1, const auto& e2) {
            return e1->first < e2->first;
        });

        memory_storage storage_{};

        if (!element_pointers.empty())
        {
            std::unordered_set<std::int32_t> base_uniquer{};
            build_iter(
                element_pointers.begin(),
                element_pointers.end(),
                0,
                storage_,
                0,
                base_uniquer,
                observer,
                density_factor);
        }

        observer.done();
        return storage_;
    }

    memory_storage double_array_builder::build(
        const std::vector<std::pair<std::string, std::int32_t>>& elements,
        const double_array::building_observer_type&              observer,
        const std::int32_t                                       density_factor)
    {
        element_vector_type element_pointers{};
        element_pointers.reserve(elements.size());
        std::transform(
            elements.begin(), elements.end(), std::back_inserter(element_pointers), [](const auto& e) { return &e; });

        return build(std::move(element_pointers), observer, density_factor);
    }

    void double_array_builder::build_iter(
        const element_iterator_type                 first,
        const element_iterator_type                 last,
        const std::size_t                           key_offset,
        memory_storage&                             storage_,
        const std::size_t                           storage_index,
        std::unordered_set<std::int32_t>&           base_uniquer,
        const double_array::building_observer_type& observer,
        const std::int32_t                          density_factor)
    {
        const auto children_firsts_ = children_firsts(first, last, key_offset);

        const auto base =
            calc_base(children_firsts_, key_offset, storage_, storage_index, density_factor, base_uniquer);
        storage_.set_base_at(storage_index, base);

        for (auto i = children_firsts_.begin(); i != std::prev(children_firsts_.end()); ++i)
        {
            const auto char_code = char_code_at((**i)->first, key_offset);
            const auto next_storage_index = base + char_code;
            storage_.set_check_at(next_storage_index, char_code);
        }
        for (auto i = children_firsts_.begin(); i != std::prev(children_firsts_.end()); ++i)
        {
            const auto char_code = char_code_at((**i)->first, key_offset);
            const auto next_storage_index = base + char_code;
            if (char_code == double_array::key_terminator())
            {
                observer.adding((**i)->first);
                storage_.set_base_at(next_storage_index, (**i)->second);
                continue;
            }
            build_iter(
                *i,
                *std::next(i),
                key_offset + 1,
                storage_,
                next_storage_index,
                base_uniquer,
                observer,
                density_factor);
        }
    }

    std::int32_t double_array_builder::calc_base(
        const std::vector<element_iterator_type>& firsts,
        const std::size_t                         key_offset,
        const memory_storage&                     storage_,
        const std::size_t                         storage_index,
        const std::int32_t                        density_factor,
        std::unordered_set<std::int32_t>&         base_uniquer)
    {
        const auto base_first = static_cast<std::int32_t>(storage_index - storage_index / density_factor) -
                                char_code_at((*firsts[0])->first, key_offset) + 1;
        for (auto base = base_first;; ++base)
        {
            const auto first_last = std::prev(firsts.end());
            const auto occupied =
                std::find_if(firsts.begin(), first_last, [key_offset, &storage_, base](const auto& first) {
                    const auto next_index = base + char_code_at((*first)->first, key_offset);
                    return storage_.check_at(next_index) != double_array::vacant_check_value();
                });
            if (occupied == first_last && base_uniquer.find(base) == base_uniquer.end())
            {
                base_uniquer.insert(base);
                return base;
            }
        }
    }

    std::vector<double_array_builder::element_iterator_type> double_array_builder::children_firsts(
        const element_iterator_type first,
        const element_iterator_type last,
        const std::size_t           key_offset)
    {
        std::vector<element_iterator_type> firsts{ first };
        for (auto child_first = first; child_first != last;)
        {
            auto child_last = std::find_if(child_first, last, [child_first, key_offset](const auto& e) {
                return char_code_at(e->first, key_offset) != char_code_at((*child_first)->first, key_offset);
            });

            firsts.push_back(child_last);

            child_first = child_last;
        }
        assert(!firsts.empty());
        return firsts;
    }

    std::uint8_t double_array_builder::char_code_at(const std::string& string, const std::size_t index)
    {
        return index < string.length() ? string[index] : double_array::key_terminator();
    }
}

#endif
