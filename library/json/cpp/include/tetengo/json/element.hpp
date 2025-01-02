/*! \file
    \brief An element.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_ELEMENT_HPP)
#define TETENGO_JSON_ELEMENT_HPP

#include <string>
#include <unordered_map>

#include <tetengo/json/file_location.hpp>


namespace tetengo::json
{
    /*!
        \brief An element.
    */
    class element
    {
    public:
        // types

        //! The type name type.
        enum class type_name_type
        {
            string, //!< A string. primitive.
            number, //!< A number. primitive.
            boolean, //!< A boolean. primitive.
            null, //!< A null. primitive.
            object, //!< An object. structure.
            member, //!< A member. structure.
            array, //!< An array. structure.
        };

        //! The type category type.
        enum class type_category_type
        {
            primitive, //!< A primitive.
            structure_open, //!< An opening structure.
            structure_close, //!< A closing structure.
        };

        //! The type type.
        struct type_type
        {
            //! A name.
            type_name_type name;

            //! A category.
            type_category_type category;
        };


        // constructors and destructor

        /*!
            \brief Creates an element.

            \param type           A type.
            \param value          A value.
            \param attributes     Attributes.
            \param file_location_ A file location.

            \throw std::invalid_argument When the arguments are mismatched.
        */
        element(
            type_type                                    type,
            std::string                                  value,
            std::unordered_map<std::string, std::string> attributes,
            file_location                                file_location_);


        // functions

        /*!
            \brief Returns the type.

            \return The type.
        */
        [[nodiscard]] const type_type& type() const;

        /*!
            \brief Returns the value.

            \return The value.
        */
        [[nodiscard]] const std::string& value() const;

        /*!
            \brief Returns the attributes.

            \return The attributes.
        */
        [[nodiscard]] const std::unordered_map<std::string, std::string>& attributes() const;

        /*!
            \brief Returns the file location.

            \return The file location.
        */
        [[nodiscard]] const file_location& get_file_location() const;


    private:
        // variables

        type_type m_type;

        std::string m_value;

        std::unordered_map<std::string, std::string> m_attributes;

        file_location m_file_location;
    };


}


#endif
