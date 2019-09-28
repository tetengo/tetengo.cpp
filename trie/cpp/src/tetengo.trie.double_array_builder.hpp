/*! \file
    \brief A double array builder.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP)
#define TETENGO_TRIE_DOUBLEARRAYBUILDER_HPP

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>


namespace tetengo::trie
{
    class double_array_builder
    {
    public:
        // static functions

        static std::vector<std::uint32_t> build(std::initializer_list<std::pair<std::string, std::int32_t>> elements);


        // constructors

        double_array_builder() = delete;


    private:
        // types

        using element_vector_type = std::vector<const std::pair<std::string, std::int32_t>*>;

        using element_iterator_type = element_vector_type::const_iterator;


        // static functions

        static void build_iter(
            const element_iterator_type first,
            const element_iterator_type last,
            const std::size_t           offset,
            std::vector<std::uint32_t>& base_check_array,
            const std::size_t           base_check_array_index);

        static std::int32_t calc_base(
            const std::vector<element_iterator_type>& firsts,
            const std::size_t                         offset,
            const std::vector<std::uint32_t>&         base_check_array,
            const std::size_t                         base_check_array_index);

        static void
        set_base_at(std::vector<std::uint32_t>& base_check_array, const std::size_t index, const std::int32_t value);

        static std::uint8_t check_at(const std::vector<std::uint32_t>& base_check_array, const std::size_t index);

        static void
        set_check_at(std::vector<std::uint32_t>& base_check_array, const std::size_t index, const std::uint8_t value);

        static std::vector<element_iterator_type>
        children_firsts(const element_iterator_type first, const element_iterator_type last, const std::size_t offset);

        static char char_code_at(const std::string& string, const std::size_t index);
    };


}


#endif
