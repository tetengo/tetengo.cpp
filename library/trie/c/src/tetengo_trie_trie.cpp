/*! \file
    \brief A trie.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>

#include "tetengo_trie_trie.hpp"


void tetengo_trie_trie_nullAddingObserver(const char*, void*) {}

void tetengo_trie_trie_nullDoneObserver(void*) {}

size_t tetengo_trie_trie_defaultDoubleArrayDensityFactor(void)
{
    try
    {
        return tetengo::trie::trie_impl::default_double_array_density_factor();
    }
    catch (...)
    {
        return 0;
    }
}

tetengo_trie_trie_t* tetengo_trie_trie_create(
    const tetengo_trie_trieElement_t* const  p_elements,
    const size_t                             element_count,
    const size_t                             element_value_size,
    const tetengo_trie_trie_addingObserver_t adding_observer,
    void* const                              p_adding_observer_context,
    const tetengo_trie_trie_doneObserver_t   done_observer,
    void* const                              p_done_observer_context,
    const size_t                             double_array_density_factor)
{
    try
    {
        if (!p_elements && element_count > 0)
        {
            throw std::invalid_argument{ "p_elements is NULL." };
        }
        if (!adding_observer)
        {
            throw std::invalid_argument{ "adding_observer is NULL." };
        }
        if (!done_observer)
        {
            throw std::invalid_argument{ "done_observer is NULL." };
        }

        std::vector<std::pair<std::string_view, std::vector<char>>> elements{};
        elements.reserve(element_count);
        std::transform(
            p_elements, p_elements + element_count, std::back_inserter(elements), [element_value_size](const auto& e) {
                return std::make_pair(
                    e.key,
                    std::vector<char>{ static_cast<const char*>(e.p_value),
                                       static_cast<const char*>(e.p_value) + element_value_size });
            });

        const cpp_trie_type::building_observer_set_type observer_set{
            [adding_observer, p_adding_observer_context](const std::string_view& key) {
                adding_observer(std::string{ key }.c_str(), p_adding_observer_context);
            },
            [done_observer, p_done_observer_context]() { done_observer(p_done_observer_context); }
        };
        auto p_cpp_trie = std::make_unique<cpp_trie_type>(
            std::begin(elements),
            std::end(elements),
            tetengo::trie::default_serializer<std::string_view>{ true },
            observer_set,
            double_array_density_factor);

        auto p_instance = std::make_unique<tetengo_trie_trie_t>(std::move(p_cpp_trie));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_trie_trie_t* tetengo_trie_trie_createWithStorage(tetengo_trie_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!p_storage->p_cpp_storage_owned)
        {
            tetengo_trie_storage_destroy(p_storage);
            return nullptr;
        }

        auto p_cpp_trie = std::make_unique<cpp_trie_type>(std::move(p_storage->p_cpp_storage_owned));
        tetengo_trie_storage_destroy(p_storage);

        auto p_instance = std::make_unique<tetengo_trie_trie_t>(std::move(p_cpp_trie));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_trie_trie_destroy(const tetengo_trie_trie_t* const p_trie)
{
    try
    {
        const std::unique_ptr<const tetengo_trie_trie_t> p_instance{ p_trie };
    }
    catch (...)
    {}
}

bool tetengo_trie_trie_empty(const tetengo_trie_trie_t* const p_trie)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }

        return std::empty(*p_trie->p_cpp_trie);
    }
    catch (...)
    {
        return false;
    }
}

size_t tetengo_trie_trie_size(const tetengo_trie_trie_t* const p_trie)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }

        return std::size(*p_trie->p_cpp_trie);
    }
    catch (...)
    {
        return static_cast<size_t>(-1);
    }
}

bool tetengo_trie_trie_contains(const tetengo_trie_trie_t* const p_trie, const char* const key)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        return p_trie->p_cpp_trie->contains(key);
    }
    catch (...)
    {
        return false;
    }
}

const void* tetengo_trie_trie_find(const tetengo_trie_trie_t* const p_trie, const char* const key)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        const auto* const p_found = p_trie->p_cpp_trie->find(key);
        if (!p_found)
        {
            return nullptr;
        }
        return std::data(*p_found);
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_trie_trieIterator_t* tetengo_trie_trie_createIterator(const tetengo_trie_trie_t* p_trie)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }
        return tetengo_trie_trieIterator_create(p_trie);
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_trie_trie_destroyIterator(const tetengo_trie_trieIterator_t* p_iterator)
{
    try
    {
        if (!p_iterator)
        {
            throw std::invalid_argument{ "p_iterator is NULL." };
        }
        tetengo_trie_trieIterator_destroy(p_iterator);
    }
    catch (...)
    {}
}

const tetengo_trie_trie_t*
tetengo_trie_trie_subtrie(const tetengo_trie_trie_t* const p_trie, const char* const key_prefix)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }
        if (!key_prefix)
        {
            throw std::invalid_argument{ "key_prefix is NULL." };
        }

        auto p_subtrie = p_trie->p_cpp_trie->subtrie(key_prefix);
        if (!p_subtrie)
        {
            return nullptr;
        }
        auto p_instance = std::make_unique<tetengo_trie_trie_t>(std::move(p_subtrie));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

const tetengo_trie_storage_t* tetengo_trie_trie_getStorage(const tetengo_trie_trie_t* p_trie)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }
        if (!p_trie->p_storage)
        {
            const_cast<tetengo_trie_trie_t*>(p_trie)->p_storage = std::unique_ptr<tetengo_trie_storage_t>{
                tetengo_trie_storage_createStorage(p_trie),
            };
        }
        return p_trie->p_storage.get();
    }
    catch (...)
    {
        return nullptr;
    }
}
