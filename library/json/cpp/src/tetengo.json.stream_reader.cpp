/*! \file
    \brief A stream reader.

    $12026 kaoru  https://www.tetengo.org/
*/

#include <cstddef> // IWYU pragma: keep
#include <istream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/circular_buffer.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/core/pointer_traits.hpp> // IWYU pragma: keep

#include <tetengo/json/stream_reader.hpp>


namespace tetengo::json
{
    class reader;


    class stream_reader::impl : private boost::noncopyable
    {
    public:
        // static functions

        static constexpr std::size_t default_buffer_capacity()
        {
            return 4096;
        }


        // constructors and destructor

        impl(std::unique_ptr<std::istream>&& p_stream, const std::size_t buffer_capacity) :
        m_p_stream{ std::move(p_stream) },
        m_buffer{ buffer_capacity }
        {
            if (!m_p_stream)
            {
                throw std::invalid_argument{ "p_stream is nullptr." };
            }
            if (buffer_capacity == 0)
            {
                throw std::invalid_argument{ "buffer_capacity is 0." };
            }
        }


        // functions

        bool has_next_impl() const
        {
            ensure_buffer_filled();
            return !std::empty(m_buffer);
        }

        char peek_impl() const
        {
            ensure_buffer_filled();
            if (std::empty(m_buffer))
            {
                throw std::logic_error{ "No more element." };
            }
            return m_buffer.front();
        }

        void next_impl()
        {
            ensure_buffer_filled();
            if (std::empty(m_buffer))
            {
                throw std::logic_error{ "No more element." };
            }
            m_buffer.pop_front();
        }

        const reader& base_reader_impl() const
        {
            throw std::logic_error{ "No base reader." };
        }


    private:
        // variables

        const std::unique_ptr<std::istream> m_p_stream;

        mutable boost::circular_buffer<char> m_buffer;


        // functions

        void ensure_buffer_filled() const
        {
            if (!std::empty(m_buffer))
            {
                return;
            }

            std::vector<char> chars(m_buffer.capacity(), '\0');
            m_p_stream->read(std::data(chars), std::size(chars));
            const auto read_size = m_p_stream->gcount();

            m_buffer.insert(
                std::end(m_buffer),
                std::begin(chars),
                std::next(std::begin(chars), static_cast<std::ptrdiff_t>(read_size)));
        }
    };


    std::size_t stream_reader::default_buffer_capacity()
    {
        return impl::default_buffer_capacity();
    }

    stream_reader::stream_reader(
        std::unique_ptr<std::istream>&& p_stream,
        const std::size_t               buffer_capacity /*= default_buffer_capacity()*/) :
    m_p_impl{ std::make_unique<impl>(std::move(p_stream), buffer_capacity) }
    {}

    stream_reader::~stream_reader() = default;

    bool stream_reader::has_next_impl() const
    {
        return m_p_impl->has_next_impl();
    }

    char stream_reader::peek_impl() const
    {
        return m_p_impl->peek_impl();
    }

    void stream_reader::next_impl()
    {
        m_p_impl->next_impl();
    }

    const reader& stream_reader::base_reader_impl() const
    {
        return m_p_impl->base_reader_impl();
    }


}
