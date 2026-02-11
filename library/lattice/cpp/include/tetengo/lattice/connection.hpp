/*! \file
    \brief A connection.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONNECTION_HPP)
#define TETENGO_LATTICE_CONNECTION_HPP


namespace tetengo::lattice
{
    /*!
        \brief A connection.
    */
    class connection
    {
    public:
        // constructors

        /*!
            \brief Creates a connection.

            \param cost A cost.
        */
        explicit constexpr connection(int cost) : m_cost{ cost } {}


        // functions

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

        int m_cost;
    };


}


#endif
