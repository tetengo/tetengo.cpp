/*! \file
    \brief A JSON writer.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_JSONWRITER_HPP_)
#define TETENGO_PROPERTY_JSONWRITER_HPP_

#include <memory>
#include <ostream>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class json_writer : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = storage::value_map_type;


        // static functions

        static const json_writer& instance();


        // constructors and destructor

        ~json_writer();


        // functions

        void write(const value_map_type& value_map, std::ostream& stream) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        json_writer();
    };


}


#endif
