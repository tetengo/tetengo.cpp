/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAY_HPP)
#define TETENGO_TRIE_DOUBLEARRAY_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <tetengo/trie/enumerator.hpp>


namespace tetengo::trie
{
    class storage;


    /*!
        \brief A double array.
    */
    class double_array
    {
    public:
        // tyes

        //! The building observer type.
        struct building_observer_type
        {
            /*!
                \brief Called when a key is adding.

                \param key A key.
            */
            std::function<void(const std::pair<std::string_view, std::int32_t>& element)> adding;

            /*!
                \brief Called when the building is done.
            */
            std::function<void()> done;
        };


        // static functions

        /*!
            \brief Returns the null building observer.

            \return The null bulding observer.
        */
        static const building_observer_type& null_building_observer();

        /*!
            \brief Returns the default density factor.

            \return The default density factor.
        */
        static std::int32_t default_density_factor();

        /*!
            \brief Returns the key terminator.

            \return The key terminator.
        */
        static constexpr char key_terminator()
        {
            return '\0';
        }

        /*!
            \brief Returns the check value for a vacant element.

            \return The check value for a vacant element.
        */
        static constexpr std::uint8_t vacant_check_value()
        {
            return 0xFF;
        }


        // constructors and destructor

        /*!
            \brief Creates a double array.
        */
        double_array();

        /*!
            \brief Creates a double array.

            \param elements          Initial elements.
            \param building_observer A building observer.
            \param density_factor    A density factor. Must be greater than 0.

            \throw std::invalid_argument When density_factor is 0 or less than 0.
        */
        explicit double_array(
            const std::vector<std::pair<std::string_view, std::int32_t>>& elements,
            const building_observer_type&                                 building_observer = null_building_observer(),
            std::int32_t                                                  density_factor = default_density_factor());

        /*!
            \brief Creates a double array.

            \param elements          Initial elements.
            \param building_observer A building observer.
            \param density_factor    A density factor. Must be greater than 0.

            \throw std::invalid_argument When density_factor is 0 or less than 0.
        */
        explicit double_array(
            const std::vector<std::pair<std::string, std::int32_t>>& elements,
            const building_observer_type&                            building_observer = null_building_observer(),
            std::int32_t                                             density_factor = default_density_factor());

        /*!
            \brief Creates a double array.

            \tparam InputIterator An input iterator type.

            \param first             An iterator to the first element.
            \param last              An iterator to the last element.
            \param building_observer A building observer.
            \param density_factor    A density factor. Must be greater than 0.

            \throw std::invalid_argument When density_factor is 0 or less than 0.
        */
        template <typename InputIterator>
        double_array(
            InputIterator                 first,
            InputIterator                 last,
            const building_observer_type& building_observer = null_building_observer(),
            std::int32_t                  density_factor = default_density_factor()) :
        double_array{ std::vector<typename InputIterator::value_type>{ first, last },
                      building_observer,
                      density_factor }
        {}

        /*!
            \brief Creates a double array.

            \param p_storage  A unique pointer to a storage.
            \param root_index A root index.
        */
        double_array(std::unique_ptr<storage>&& p_storage, std::size_t root_index);

        /*!
            \brief Destroys the double array.
        */
        ~double_array();


        // functions

        /*!
            \brief Finds the value correspoinding the given key.

            \param key A key.

            \return The value. Or std::nullpot when the double array does not have the given key.
        */
        std::optional<std::int32_t> find(const std::string_view& key) const;

        /*!
            \brief Returns an enumerator.

            \return An enumerator.
        */
        enumerator get_enumerator() const;

        /*!
            \brief Returns a subtrie.

            \param key_prefix A key prefix.

            \return A unique pointer to a double array of the subtrie.
                    Or nullptr when the double array does not have the given key prefix.
        */
        std::unique_ptr<double_array> subtrie(const std::string_view& key_prefix) const;

        /*!
            \brief Returns the storage.

            \return The storage.
        */
        const storage& get_storage() const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
