/*! \file
    \brief A double array builder.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(DOCUMENTATION)

#if !defined(TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP)
#define TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>


namespace tetengo::trie
{
    class storage;


    class double_array_builder
    {
    public:
        // static functions

        static std::size_t default_density_factor();

        static std::unique_ptr<storage> build(
            std::vector<std::pair<std::string_view, std::int32_t>> elements,
            const double_array::building_observer_set_type&        observer,
            std::size_t                                            density_factor);


        // constructors

        double_array_builder() = delete;


    private:
        // types

        using element_vector_type = std::vector<std::pair<std::string_view, std::int32_t>>;

        using element_iterator_type = element_vector_type::const_iterator;


        // static functions

        static void build_iter(
            element_iterator_type                           first,
            element_iterator_type                           last,
            std::size_t                                     key_offset,
            storage&                                        storage_,
            std::size_t                                     base_check_index,
            std::unordered_set<std::int32_t>&               base_uniquer,
            const double_array::building_observer_set_type& observer,
            std::size_t                                     density_factor);

        static std::int32_t calc_base(
            const std::vector<element_iterator_type>& firsts,
            std::size_t                               key_offset,
            const storage&                            storage_,
            std::size_t                               base_check_index,
            std::size_t                               density_factor,
            std::unordered_set<std::int32_t>&         base_uniquer);

        static std::vector<element_iterator_type>
        children_firsts(element_iterator_type first, element_iterator_type last, std::size_t key_offset);

        static std::uint8_t char_code_at(const std::string_view& string, std::size_t index);
    };


}


#endif

#endif
