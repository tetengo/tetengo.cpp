/*! \file
    \brief A timetable vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TIMETABLEVOCABULARY_HPP)
#define TIMETABLEVOCABULARY_HPP

#include <istream>
#include <memory>

#include <boost/core/noncopyable.hpp>


/*!
    \brief A timetable vocabulary.
*/
class timetable_vocabulary : private boost::noncopyable
{
public:
    // constructors and destructor

    /*!
        \brief Creates a timetable vocabulary.

        \param p_input_stream A unique pointer to an input stream.
    */
    explicit timetable_vocabulary(std::unique_ptr<std::istream>&& p_input_stream);

    /*!
        \brief Destroys the timetable vocabulary.
    */
    ~timetable_vocabulary();


private:
    // types

    class impl;


    // variables

    const std::unique_ptr<impl> m_p_impl;
};


#endif
