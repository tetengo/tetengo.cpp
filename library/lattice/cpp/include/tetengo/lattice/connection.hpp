/*! \file
    \brief A connection.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
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
        explicit connection(int cost);


        // functions

        /*!
            \brief Returns the cost.

            \return The cost.
        */
        [[nodiscard]] int cost() const;


    private:
        // variables

        int m_cost;
    };


}


#endif
