/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_ENTRY_HPP)
#define TETENGO_LATTICE_ENTRY_HPP

#include <any>
#include <string>
#include <string_view>
#include <utility>


namespace tetengo::lattice
{
    class entry_view;


    /*!
        \brief An entry.
    */
    class entry
    {
    public:
        // static functions

        /*!
            \brief Returns the BOS/EOS (Beginning/End of Sequence) entry.

            \return The BOS/EOS entry.
        */
        [[nodiscard]] static const entry& bos_eos();


        // constructors

        /*!
            \brief Creates an entry.

            \param key   A key.
            \param value A value.
            \param cost  A cost.
        */
        entry(std::string key, std::any value, int cost);

        /*!
            \brief Creates an entry.

            \param view An entry view.
        */
        entry(const entry_view& view);


        // functions

        /*!
            \brief Returns the key.

            \return The key.
        */
        [[nodiscard]] const std::string& key() const;

        /*!
            \brief Returns the value.

            \return The value.
        */
        [[nodiscard]] const std::any& value() const;

        /*!
            \brief Returns the cost.

            \return The cost.
        */
        [[nodiscard]] int cost() const;


    private:
        // variables

        std::string m_key;

        std::any m_value;

        int m_cost;
    };


    /*!
        \brief An entry view.
    */
    class entry_view
    {
    public:
        // static functions

        /*!
            \brief Returns the BOS/EOS (Beginning/End of Sequence) entry.

            \return The BOS/EOS entry.
        */
        [[nodiscard]] static const entry_view& bos_eos();


        // constructors

        /*!
            \brief Creates an entry view.

            \param key   A key.
            \param value A value.
            \param cost  A cost.
        */
        constexpr entry_view(std::string_view key, const std::any* value, int cost) :
        m_key{ std::move(key) },
            m_value{ std::move(value) },
            m_cost{ cost }
        {}

        /*!
            \brief Creates an entry view.

            \param entry An entry.
        */
        entry_view(const entry& entry);


        // functions

        /*!
            \brief Returns the key.

            \return The key.
        */
        [[nodiscard]] constexpr const std::string_view& key() const
        {
            return m_key;
        }

        /*!
            \brief Returns the value.

            \return The value.
        */
        [[nodiscard]] constexpr const std::any* value() const
        {
            return m_value;
        }

        /*!
            \brief Returns the cost.

            \return The cost.
        */
        [[nodiscard]] constexpr int cost() const
        {
            return m_cost;
        }


    private:
        // variables

        std::string_view m_key;

        const std::any* m_value;

        int m_cost;
    };


}


#endif
