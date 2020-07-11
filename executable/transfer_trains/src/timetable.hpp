/*! \file
    \brief A timetable vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TIMETABLEVOCABULARY_HPP)
#define TIMETABLEVOCABULARY_HPP

#include <istream>
#include <memory>
#include <optional>
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
    \brief A stop.
*/
class stop
{
public:
    // constructors and destructor

    /*!
        \brief Creates a stop.

        \param arrival_time   An arrival time.
        \param departure_time A departure time.
    */
    stop(std::optional<std::size_t> arrival_time, std::optional<std::size_t> departure_time);


    // functions

    /*!
        \brief Returns the arrival time.

        \return The arrival time.
    */
    std::optional<std::size_t> arrival_time() const;

    /*!
        \brief Sets an arrival time.

        \param time An arrival time.
    */
    void set_arrival_time(std::size_t time);

    /*!
        \brief Returns the departure time.

        \return The departure time.
    */
    std::optional<std::size_t> departure_time() const;

    /*!
        \brief Sets a departure time.

        \param time A departure time.
    */
    void set_departure_time(std::size_t time);


private:
    // variables

    std::optional<std::size_t> m_arrival_time;

    std::optional<std::size_t> m_departure_time;
};


/*!
    \brief A train.
*/
class train
{
public:
    // constructors and destructor

    /*!
        \brief Creates a train.

        \param number A number.
        \param name   A name.
        \param stops  Stops.
    */
    train(std::string number, std::string name, std::vector<stop> stops);


    // functions

    /*!
        \brief Returns the number.

        \return The number.
    */
    const std::string& number() const;

    /*!
        \brief Returns the name.

        \return The name.
    */
    const std::string& name() const;

    /*!
        \brief Returns the stops.

        \return The stops.
    */
    const std::vector<stop>& stops() const;

    /*!
        \brief Returns the stops.

        \return The stops.
    */
    std::vector<stop>& stops();


private:
    // variables

    std::string m_number;

    std::string m_name;

    std::vector<stop> m_stops;
};


/*!
    \brief A section.
*/
class section
{
public:
    // constructors and destructor

    /*!
        \brief Creates a section.

        \param p_train A pointer to a train.
        \param from    A departure station index.
        \param to      An arrival station index.
    */
    section(const train* p_train, std::size_t from, std::size_t to);


    // functions

    /*!
        \brief Returns a pointer to the train.

        \return A pointer to the train.
    */
    const train* p_train() const;

    /*!
        \brief Returns the departure station index.

        \return The departure station index.
    */
    std::size_t from() const;

    /*!
        \brief Returns the arrival station index.

        \return The arrival station index.
    */
    std::size_t to() const;


private:
    // variables

    const train* m_p_train;

    std::size_t m_from;

    std::size_t m_to;
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
        \brief Returns the station index.

        \param name_or_telegram_code A name or telegram code.

        \return The index. Or stations().size() when no station is found.
    */
    std::size_t station_index(const std::string& name_or_telegram_code) const;

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
