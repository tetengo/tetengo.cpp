/*! \file
    \brief A timetable vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <istream>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include "timetable_vocabulary.hpp"


class timetable_vocabulary::impl : private boost::noncopyable
{
public:
    // constructors and destructor

    explicit impl(std::unique_ptr<std::istream>&& /*p_input_stream*/) {}


private:
    // variables
};


timetable_vocabulary::timetable_vocabulary(std::unique_ptr<std::istream>&& p_input_stream) :
m_p_impl{ std::make_unique<impl>(std::move(p_input_stream)) }
{}

timetable_vocabulary::~timetable_vocabulary() = default;
