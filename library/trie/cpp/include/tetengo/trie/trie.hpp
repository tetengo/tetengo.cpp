/*! \file
    \brief A trie.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIE_HPP)
#define TETENGO_TRIE_TRIE_HPP

#include <algorithm> // IWYU pragma: keep
#include <any>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/trie_iterator.hpp>


namespace tetengo::trie
{
    class double_array;
    class storage;


    /*!
        \brief An implementation of trie.
    */
    class trie_impl : private boost::noncopyable
    {
    public:
        // types

        //! The building observer set type.
        struct building_observer_set_type
        {
            /*!
                \brief Called when a key is adding.

                \param serialized_key A serialized key.
            */
            std::function<void(const std::string_view& serialized_key)> adding;

            /*!
                \brief Called when the building is done.
            */
            std::function<void()> done;
        };


        // static functions

        /*!
            \brief Returns the null building observer set.

            \return The null bulding observer set.
        */
        [[nodiscard]] static const building_observer_set_type& null_building_observer_set();

        /*!
            \brief Returns the default double array density factor.

            \return The default double array density factor.
        */
        [[nodiscard]] static std::size_t default_double_array_density_factor();


        // constructors and destructor

        /*!
            \brief Creates an implementation of trie.
        */
        trie_impl();

        /*!
            \brief Creates a trie.

            \param elements                    Initial elements.
            \param building_observer_set       A building observer set.
            \param double_array_density_factor A double array density factor.
         */
        trie_impl(
            std::vector<std::pair<std::string_view, std::any>> elements,
            const building_observer_set_type&                  building_observer_set,
            std::size_t                                        double_array_density_factor);

        /*!
            \brief Creates a trie.

            \param elements                    Initial elements.
            \param building_observer_set       A building observer set.
            \param double_array_density_factor A double array density factor.
         */
        trie_impl(
            std::vector<std::pair<std::string, std::any>> elements,
            const building_observer_set_type&             building_observer_set,
            std::size_t                                   double_array_density_factor);

        /*!
            \brief Creates a trie.

            \param p_storage A unique pointer to a storage.
        */
        explicit trie_impl(std::unique_ptr<storage>&& p_storage);

        /*!
            \brief Creates a trie.

            \param p_double_array A unique pointer to a double array.
        */
        explicit trie_impl(std::unique_ptr<double_array>&& p_double_array);

        /*!
            \brief Moves a trie.

            \param another Another trie.
        */
        trie_impl(trie_impl&& another) noexcept;

        /*!
            \brief Destroys an implementation of trie.
        */
        ~trie_impl();


        // functions

        /*!
            \brief Returns true when the trie is empty.

            \retval true  When the trie is empty.
            \retval false Otherwise.
        */
        [[nodiscard]] bool empty() const;

        /*!
            \brief Returns the size of the trie.

            \return The size.
        */
        [[nodiscard]] std::size_t size() const;

        /*!
            \brief Returns true when the trie contains the given key.

            \param key A key.

            \retval true  When the trie contains the given key.
            \retval false Otherwise.
        */
        [[nodiscard]] bool contains(const std::string_view& key) const;

        /*!
            \brief Finds the value object correspoinding the given key.

            \param key A key.

            \return A pointer to the value object. Or nullptr when the trie does not have the given key.
        */
        [[nodiscard]] const std::any* find(const std::string_view& key) const;

        /*!
            \brief Returns the first iterator.

            \return The first iterator.
        */
        [[nodiscard]] trie_iterator_impl begin() const;

        /*!
            \brief Returns the last iterator.

            \return The last iterator.
        */
        [[nodiscard]] trie_iterator_impl end() const;

        /*!
            \brief Returns a subtrie.

            \param key_prefix A key prefix.

            \return A unique pointer to a subtrie.
                    Or nullptr when the trie does not have the given key prefix.
        */
        [[nodiscard]] std::unique_ptr<trie_impl> subtrie(const std::string_view& key_prefix) const;

        /*!
            \brief Returns the storage.

            \return The storage.
        */
        [[nodiscard]] const storage& get_storage() const;


    private:
        // types

        class impl;


        // variables

        std::unique_ptr<impl> m_p_impl;
    };


    /*!
        \brief A trie.

        \tparam Key           A key type.
        \tparam Value         A value type.
        \tparam KeySerializer A key serializer type.
    */
    template <typename Key, typename Value, typename KeySerializer = default_serializer<Key>>
    class trie : private boost::noncopyable
    {
    public:
        // types

        //! The key type.
        using key_type = Key;

        //! The value type.
        using value_type = Value;

        //! The key serializer_type.
        using key_serializer_type = KeySerializer;

        //! The iterator type.
        using iterator = trie_iterator<value_type>;

        //! The building observer set type.
        using building_observer_set_type = trie_impl::building_observer_set_type;


        // static functions

        /*!
            \brief Returns the null building observer set.

            \return The null bulding observer set.
        */
        [[nodiscard]] static const building_observer_set_type& null_building_observer_set()
        {
            return trie_impl::null_building_observer_set();
        }

        /*!
            \brief Returns the default double array density factor.

            \return The default double array density factor.
        */
        [[nodiscard]] static std::size_t default_double_array_density_factor()
        {
            return trie_impl::default_double_array_density_factor();
        }


        // constructors and destructor

        /*!
            \brief Creates a trie.

            \param key_serializer A key serializer.
        */
        explicit trie(const key_serializer_type& key_serializer = default_serializer<key_type>{ true }) :
        m_impl{},
        m_key_serializer{ key_serializer }
        {}

        /*!
            \brief Creates a trie.

            \param elements                    Initial elements.
            \param key_serializer              A key serializer.
            \param building_observer_set       A building observer set.
            \param double_array_density_factor A double array density factor.
        */
        explicit trie(
            std::initializer_list<std::pair<key_type, value_type>> elements,
            const key_serializer_type&        key_serializer = default_serializer<key_type>{ true },
            const building_observer_set_type& building_observer_set = null_building_observer_set(),
            std::size_t                       double_array_density_factor = default_double_array_density_factor()) :
        m_impl{ serialize_key<true>(std::begin(elements), std::end(elements), key_serializer),
                building_observer_set,
                double_array_density_factor },
        m_key_serializer{ key_serializer }
        {}

        /*!
            \brief Creates a trie.

            \tparam InputIterator An input iterator type.

            \param first                       An iterator to the first element.
            \param last                        An iterator to the last element.
            \param key_serializer              A key serializer.
            \param building_observer_set       A building observer set.
            \param double_array_density_factor A double array density factor.
        */
        template <typename InputIterator>
        trie(
            InputIterator                     first,
            InputIterator                     last,
            const key_serializer_type&        key_serializer = default_serializer<key_type>{ true },
            const building_observer_set_type& building_observer_set = null_building_observer_set(),
            std::size_t                       double_array_density_factor = default_double_array_density_factor()) :
        m_impl{ serialize_key<false>(first, last, key_serializer), building_observer_set, double_array_density_factor },
        m_key_serializer{ key_serializer }
        {}

        /*!
            \brief Creates a trie.

            \tparam InputIterator An input iterator type.

            \param first                       An iterator to the first element.
            \param last                        An iterator to the last element.
            \param key_serializer              A key serializer.
            \param building_observer_set       A building observer set.
            \param double_array_density_factor A double array density factor.
        */
        template <typename InputIterator>
        trie(
            std::move_iterator<InputIterator> first,
            std::move_iterator<InputIterator> last,
            const key_serializer_type&        key_serializer = default_serializer<key_type>{ true },
            const building_observer_set_type& building_observer_set = null_building_observer_set(),
            std::size_t                       double_array_density_factor = default_double_array_density_factor()) :
        m_impl{ serialize_key<true>(first, last, key_serializer), building_observer_set, double_array_density_factor },
        m_key_serializer{ key_serializer }
        {}

        /*!
            \brief Creates a trie.

            \param p_storage      A unique pointer to a storage.
            \param key_serializer A key serializer.
        */
        explicit trie(
            std::unique_ptr<storage>&& p_storage,
            const key_serializer_type& key_serializer = default_serializer<key_type>{ true }) :
        m_impl{ std::move(p_storage) },
        m_key_serializer{ key_serializer }
        {}


        // functions

        /*!
            \brief Returns true when the trie is empty.

            \retval true  When the trie is empty.
            \retval false Otherwise.
        */
        [[nodiscard]] bool empty() const
        {
            return std::empty(m_impl);
        }

        /*!
            \brief Returns the size of the trie.

            \return The size.
        */
        [[nodiscard]] std::size_t size() const
        {
            return std::size(m_impl);
        }

        /*!
            \brief Returns true when the trie contains the given key.

            \param key A key.

            \retval true  When the trie contains the given key.
            \retval false Otherwise.
        */
        [[nodiscard]] bool contains(const key_type& key) const
        {
            if constexpr (std::is_same_v<key_type, std::string_view> || std::is_same_v<key_type, std::string>)
            {
                return m_impl.contains(m_key_serializer(key));
            }
            else
            {
                const auto serialized_key = m_key_serializer(key);
                return m_impl.contains(std::string_view{ std::data(serialized_key), std::size(serialized_key) });
            }
        }

        /*!
            \brief Finds the value object correspoinding the given key.

            \param key A key.

            \return A pointer to the value object. Or nullptr when the trie does not have the given key.
        */
        [[nodiscard]] const value_type* find(const key_type& key) const
        {
            const auto* const p_found = [this, &key]() {
                if constexpr (std::is_same_v<key_type, std::string_view> || std::is_same_v<key_type, std::string>)
                {
                    return m_impl.find(m_key_serializer(key));
                }
                else
                {
                    const auto serialized_key = m_key_serializer(key);
                    return m_impl.find(std::string_view{ std::data(serialized_key), std::size(serialized_key) });
                }
            }();
            if (!p_found)
            {
                return nullptr;
            }
            return std::any_cast<value_type>(p_found);
        }

        /*!
            \brief Returns the first iterator.

            \return The first iterator.
        */
        [[nodiscard]] iterator begin() const
        {
            return iterator{ std::move(std::begin(m_impl)) };
        }

        /*!
            \brief Returns the last iterator.

            \return The last iterator.
        */
        [[nodiscard]] iterator end() const
        {
            return iterator{ std::move(std::end(m_impl)) };
        }

        /*!
            \brief Returns a subtrie.

            \param key_prefix A key prefix.

            \return A unique pointer to a subtrie.
                    Or nullptr when the trie does not have the given key prefix.
        */
        [[nodiscard]] std::unique_ptr<trie> subtrie(const key_type& key_prefix) const
        {
            auto p_trie_impl = [this, &key_prefix]() {
                if constexpr (std::is_same_v<key_type, std::string_view> || std::is_same_v<key_type, std::string>)
                {
                    return m_impl.subtrie(m_key_serializer(key_prefix));
                }
                else
                {
                    const auto serialized_key_prefix = m_key_serializer(key_prefix);
                    return m_impl.subtrie(
                        std::string_view{ std::data(serialized_key_prefix), std::size(serialized_key_prefix) });
                }
            }();
            if (!p_trie_impl)
            {
                return std::unique_ptr<trie>{};
            }
            std::unique_ptr<trie> p_trie{ new trie{ std::move(p_trie_impl), m_key_serializer } };
            return p_trie;
        }

        /*!
            \brief Returns the storage.

            \return The storage.
        */
        [[nodiscard]] const storage& get_storage() const
        {
            return m_impl.get_storage();
        }


    private:
        // types

        using serialized_element_type = std::pair<
            std::conditional_t<std::is_same_v<key_type, std::string_view>, std::string_view, std::string>,
            std::any>;


        // static functions

        template <bool MoveValue, typename InputIterator>
        static std::vector<serialized_element_type>
        serialize_key(InputIterator first, InputIterator last, const key_serializer_type& key_serializer)
        {
            std::vector<serialized_element_type> serialized{};
            if constexpr (std::is_convertible_v<
                              typename std::iterator_traits<InputIterator>::iterator_category,
                              std::forward_iterator_tag>)
            {
                serialized.reserve(std::distance(first, last));
            }
            std::transform(first, last, std::back_inserter(serialized), [&key_serializer](auto&& value) {
                if constexpr (std::is_same_v<key_type, std::string_view> || std::is_same_v<key_type, std::string>)
                {
                    if constexpr (MoveValue)
                    {
                        return serialized_element_type{ key_serializer(value.first), std::move(value.second) };
                    }
                    else
                    {
                        return serialized_element_type{ key_serializer(value.first), value.second };
                    }
                }
                else
                {
                    const auto  serialized_key_vector = key_serializer(value.first);
                    std::string serialized_key{ std::begin(serialized_key_vector), std::end(serialized_key_vector) };
                    if constexpr (MoveValue)
                    {
                        return serialized_element_type{ std::move(serialized_key), std::move(value.second) };
                    }
                    else
                    {
                        return serialized_element_type{ std::move(serialized_key), value.second };
                    }
                }
            });
            return serialized;
        }


        // variables

        const trie_impl m_impl;

        const key_serializer_type m_key_serializer;


        // constructors

        explicit trie(std::unique_ptr<trie_impl>&& p_impl, const key_serializer_type& ker_serializer) :
        m_impl{ std::move(*p_impl) },
        m_key_serializer{ ker_serializer }
        {}
    };


}


#endif
