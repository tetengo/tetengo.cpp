/*! \file
    \brief A double array builder.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP)
#define TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    class double_array_builder
    {
    public:
        // static functions

        static storage build(std::vector<const std::pair<std::string, std::int32_t>*> element_pointers);

        static storage build(const std::vector<std::pair<std::string, std::int32_t>>& elements);


        // constructors

        double_array_builder() = delete;


    private:
        // types

        using element_vector_type = std::vector<const std::pair<std::string, std::int32_t>*>;

        using element_iterator_type = element_vector_type::const_iterator;


        // static functions

        static void build_iter(
            element_iterator_type first,
            element_iterator_type last,
            std::size_t           key_offset,
            storage&              storage_,
            std::size_t           storage_index);

        static std::int32_t calc_base(
            const std::vector<element_iterator_type>& firsts,
            std::size_t                               key_offset,
            const storage&                            storage_,
            std::size_t                               storage_index);

        static std::vector<element_iterator_type>
        children_firsts(element_iterator_type first, element_iterator_type last, std::size_t key_offset);

        static char char_code_at(const std::string& string, std::size_t index);
    };


}


#endif
