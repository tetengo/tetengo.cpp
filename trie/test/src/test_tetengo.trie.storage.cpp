/*!
    \brief A storage.

    Copyright (C) 2019 kaoru
 */

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/storage.hpp>


namespace
{
    class concrete_storage : public tetengo::trie::storage
    {
    public:
        // constructors and destructor

        concrete_storage() : m_values{} {}

        concrete_storage(const concrete_storage& another) : m_values{ another.m_values } {}

        virtual ~concrete_storage() = default;


    private:
        // variables

        mutable std::vector<std::pair<std::int32_t, std::uint8_t>> m_values;


        // virtual functions

        virtual std::int32_t base_at_impl(const std::size_t index) const override
        {
            ensure_value_size(index + 1);
            return m_values[index].first;
        }

        virtual void set_base_at_impl(const std::size_t index, const std::int32_t value) override
        {
            ensure_value_size(index + 1);
            m_values[index].first = value;
        }

        virtual std::uint8_t check_at_impl(const std::size_t index) const override
        {
            ensure_value_size(index + 1);
            return m_values[index].second;
        }

        virtual void set_check_at_impl(const std::size_t index, const std::uint8_t value) override
        {
            ensure_value_size(index + 1);
            m_values[index].second = value;
        }

        virtual std::size_t size_impl() const override
        {
            return m_values.size();
        }

        virtual std::unique_ptr<storage> clone_impl() const override
        {
            return std::make_unique<concrete_storage>(*this);
        }


        // functions

        void ensure_value_size(const std::size_t size) const
        {
            if (size > m_values.size())
            {
                m_values.resize(size);
            }
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(storage)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage{};
}

BOOST_AUTO_TEST_CASE(base_at)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage{};

    storage.base_at(42);
}

BOOST_AUTO_TEST_CASE(set_base_at)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage{};

    storage.set_base_at(42, 4242);

    BOOST_TEST(storage.base_at(42) == 4242);
}

BOOST_AUTO_TEST_CASE(check_at)
{
    BOOST_TEST_PASSPOINT();

    const concrete_storage storage{};

    storage.check_at(24);
}

BOOST_AUTO_TEST_CASE(set_check_at)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage{};

    storage.set_check_at(24, 124);

    BOOST_TEST(storage.check_at(24) == 124);
}

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage{};

    BOOST_TEST(storage.size() == 0);

    storage.base_at(42);

    BOOST_TEST(storage.size() == 43);
}

BOOST_AUTO_TEST_CASE(clone)
{
    BOOST_TEST_PASSPOINT();

    concrete_storage storage1{};

    storage1.set_base_at(42, 4242);
    storage1.set_check_at(24, 124);

    const auto p_storage2 = storage1.clone();

    BOOST_TEST(p_storage2->base_at(42) == 4242);
    BOOST_TEST(p_storage2->check_at(24) == 124);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
