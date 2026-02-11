/*! \file
    \brief An input.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_INPUT_HPP)
#define TETENGO_LATTICE_INPUT_HPP

#include <cassert>
#include <cstddef>
#include <memory>

#include <boost/core/noncopyable.hpp>
#include <boost/operators.hpp>


namespace tetengo::lattice
{
    /*!
        \brief An input.
    */
    class input : public boost::equality_comparable<input>, private boost::noncopyable
    {
    public:
        // constructors

        /*!
            \brief Destroys the input.
        */
        virtual ~input();


        // functions

        /*!
            \brief Returns true if one input is equal to another.

            \param one     One input.
            \param another Another input.

            \retval true  When one input is equal to another.
            \retval valse Otherwise.
        */
        friend bool operator==(const input& one, const input& another);

        /*!
            \brief Returns the hash value.

            \return The hash value.
        */
        [[nodiscard]] std::size_t hash_value() const;

        /*!
            \brief Returns the length.

            \return The length.
        */
        [[nodiscard]] std::size_t length() const;

        /*!
            \brief Clone this input.

            \return A unique pointer to a clone.
        */
        [[nodiscard]] std::unique_ptr<input> clone() const;

        /*!
            \brief Creates a subrange.

            \param offset An offset.
            \param length A length.

            \return A unique pointer to a subrange.

            \throw std::out_of_range When offset and/or length are out of the range of the input.
        */
        [[nodiscard]] std::unique_ptr<input> create_subrange(std::size_t offset, std::size_t length) const;

        /*!
            \brief Appends another input.

            \param p_another A unique pointer to another input.

            \throw std::invalid_argument When p_another is nullptr or its type does not match.
        */
        void append(std::unique_ptr<input>&& p_another);

        /*!
            \brief Returns true when this object can be casted to the specified concrete input.

            \tparam C A concrete input type.

            \retval true  When this object can be casted to the specified concrete input.
            \retval false Otherwise.
        */
        template <typename C>
        [[nodiscard]] bool is() const
        {
            return dynamic_cast<const C*>(this);
        }

        /*!
            \brief Casts this object to the specified concrete input.

            \tparam C A concrete input type.

            \return The casted concrete vocabulary.
        */
        template <typename C>
        [[nodiscard]] const C& as() const
        {
            assert(is<C>());
            return static_cast<const C&>(*this);
        }

        /*!
            \brief Casts this object to the specified concrete input.

            \tparam C A concrete input type.

            \return The casted concrete vocabulary.
        */
        template <typename C>
        [[nodiscard]] C& as()
        {
            assert(is<C>());
            return static_cast<C&>(*this);
        }


    private:
        // virtual functions

        virtual bool equal_to_impl(const input& another) const = 0;

        virtual std::size_t hash_value_impl() const = 0;

        virtual std::size_t length_impl() const = 0;

        virtual std::unique_ptr<input> clone_impl() const = 0;

        virtual std::unique_ptr<input> create_subrange_impl(std::size_t offset, std::size_t length) const = 0;

        virtual void append_impl(std::unique_ptr<input>&& p_another) = 0;
    };


}


#endif
