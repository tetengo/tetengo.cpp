/*! \file
    \brief A double array builder.

    Copyright (C) 2019 kaoru
*/

#if !defined(DOCUMENTATION)

#if !defined(TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP)
#define TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>


namespace tetengo::trie
{
    class double_array_builder
    {
    public:
        // static functions

        static std::int32_t default_density_factor();

        static std::unique_ptr<memory_storage> build(
            std::vector<const std::pair<std::string, std::int32_t>*> element_pointers,
            const double_array::building_observer_type&              observer,
            std::int32_t                                             density_factor);

        static std::unique_ptr<memory_storage> build(
            const std::vector<std::pair<std::string, std::int32_t>>& elements,
            const double_array::building_observer_type&              observer,
            std::int32_t                                             density_factor);


        // constructors

        double_array_builder() = delete;


    private:
        // types

        using element_vector_type = std::vector<const std::pair<std::string, std::int32_t>*>;

        using element_iterator_type = element_vector_type::const_iterator;


        // static functions

        static void build_iter(
            element_iterator_type                       first,
            element_iterator_type                       last,
            std::size_t                                 key_offset,
            memory_storage&                             storage_,
            std::size_t                                 storage_index,
            std::unordered_set<std::int32_t>&           base_uniquer,
            const double_array::building_observer_type& observer,
            std::int32_t                                density_factor);

        static std::int32_t calc_base(
            const std::vector<element_iterator_type>& firsts,
            std::size_t                               key_offset,
            const memory_storage&                     storage_,
            std::size_t                               storage_index,
            std::int32_t                              density_factor,
            std::unordered_set<std::int32_t>&         base_uniquer);

        static std::vector<element_iterator_type>
        children_firsts(element_iterator_type first, element_iterator_type last, std::size_t key_offset);

        static std::uint8_t char_code_at(const std::string& string, std::size_t index);
    };


}


#endif

#endif
