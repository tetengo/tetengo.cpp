/*! \file
    \brief An entry.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_ENTRY_HPP)
#define TETENGO_LATTICE_ENTRY_HPP

#include <any>
#include <memory>
#include <utility>

#include <tetengo/lattice/input.hpp>


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

            \param p_key A unique pointer to a key.
            \param value A value.
            \param cost  A cost.
        */
        entry(std::unique_ptr<input>&& p_key, std::any value, int cost);

        /*!
            \brief Creates an entry.

            \param view An entry view.
        */
        entry(const entry_view& view);

        /*!
            \brief Copies an entry.

            \param another Another entry.
        */
        entry(const entry& another);

        /*!
            \brief Moves an entry.

            \param another Another entry.
        */
        entry(entry&& another);


        // functions

        /*!
            \brief Returns the pointer to the key.

            \return The pointer to the key.
        */
        [[nodiscard]] const input* p_key() const;

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

        std::unique_ptr<input> m_p_key;

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

            \param p_key A pointer to a key.
            \param value A value.
            \param cost  A cost.
        */
        constexpr entry_view(const input* p_key, const std::any* value, int cost) :
        m_p_key{ p_key },
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
            \brief Returns the pointer to the key.

            \return The pointer to the key.
        */
        [[nodiscard]] constexpr const input* p_key() const
        {
            return m_p_key;
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

        const input* m_p_key;

        const std::any* m_value;

        int m_cost;
    };


}


#endif
