/*! \file
    \brief A timetable vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TIMETABLEVOCABULARY_HPP)
#define TIMETABLEVOCABULARY_HPP

#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <boost/core/noncopyable.hpp>

namespace tetengo::lattice
{
    class vocabulary;
}

/*!
    \brief A station.
*/
class station
{
public:
    // constructors and destructor

    /*!
        \brief Creates a station.

        \param name          A name.
        \param telegram_code A telegram code.
    */
    station(std::string name, std::string telegram_code);

    // functions

    /*!
        \brief Returns the name.

        \return The name.
    */
    const std::string& name() const;

    /*!
        \brief Returns the telegram code.

        \return The telegram code.
    */
    const std::string& telegram_code() const;


private:
    // variables

    std::string m_name;

    std::string m_telegram_code;
};


/*!
    \brief A timetable vocabulary.
*/
class timetable : private boost::noncopyable
{
public:
    // constructors and destructor

    /*!
        \brief Creates a timetable vocabulary.

        \param p_input_stream A unique pointer to an input stream.
    */
    explicit timetable(std::unique_ptr<std::istream>&& p_input_stream);

    /*!
        \brief Destroys the timetable vocabulary.
    */
    ~timetable();


    // functions

    /*!
        \brief Returns the stations.

        \return The stations.
    */
    const std::vector<station>& stations() const;

    /*!
        \brief Creates a vocabulary.

        \return A vocabulary.
    */
    std::unique_ptr<tetengo::lattice::vocabulary> create_vocabulary() const;


private:
    // types

    class impl;


    // variables

    const std::unique_ptr<impl> m_p_impl;
};


#endif
