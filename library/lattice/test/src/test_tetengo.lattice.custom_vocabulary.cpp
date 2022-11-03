/*! \file
    \brief A custom vocabulary.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/string_input.hpp>
#include <tetengo/lattice/vocabulary.h>


namespace
{
    size_t find_entries_procedure(
        void* const p_context,
        const tetengo_lattice_input_t* const /*p_key*/,
        tetengo_lattice_entryView_t* const p_entries)
    {
        if (p_entries)
        {
            const auto* const p_values = reinterpret_cast<std::vector<std::string>*>(p_context);
            assert(std::size(*p_values) == 2U);
            static const tetengo::lattice::string_input cpp_key0{ "key0" };
            p_entries[0].key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&cpp_key0);
            static const std::any value0{ reinterpret_cast<const void*>((*p_values)[0].c_str()) };
            p_entries[0].value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value0);
            p_entries[0].cost = 0;

            static const tetengo::lattice::string_input cpp_key1{ "key1" };
            p_entries[1].key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&cpp_key1);
            static const std::any value1{ reinterpret_cast<const void*>((*p_values)[1].c_str()) };
            p_entries[1].value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value1);
            p_entries[1].cost = 1;
        }

        return 2;
    }

    bool find_connection_procedure(
        void* const /*p_context*/,
        const tetengo_lattice_node_t* const /*p_from*/,
        const tetengo_lattice_entryView_t* const /*p_to*/,
        tetengo_lattice_connection_t* const p_connection)
    {
        if (p_connection)
        {
            p_connection->cost = 42;
            return true;
        }
        else
        {
            return false;
        }
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(custom_vocabulary)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    std::vector<std::string>                           context{ "hoge", "fuga" };
    const tetengo_lattice_customVocabularyDefinition_t definition{ &context,
                                                                   find_entries_procedure,
                                                                   find_connection_procedure };
    const auto* const p_vocabulary = tetengo_lattice_vocabulary_createCustomVocabulary(&definition);
    BOOST_SCOPE_EXIT(p_vocabulary)
    {
        tetengo_lattice_vocabulary_destroy(p_vocabulary);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST(p_vocabulary);
}

BOOST_AUTO_TEST_CASE(find_entries)
{
    BOOST_TEST_PASSPOINT();

    std::vector<std::string>                           context{ "hoge", "fuga" };
    const tetengo_lattice_customVocabularyDefinition_t definition{ &context,
                                                                   find_entries_procedure,
                                                                   find_connection_procedure };
    const auto* const p_vocabulary = tetengo_lattice_vocabulary_createCustomVocabulary(&definition);
    BOOST_SCOPE_EXIT(p_vocabulary)
    {
        tetengo_lattice_vocabulary_destroy(p_vocabulary);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_vocabulary);

    const auto* const p_input = tetengo_lattice_input_createStringInput("foo");
    BOOST_SCOPE_EXIT(p_input)
    {
        tetengo_lattice_input_destroy(p_input);
    }
    BOOST_SCOPE_EXIT_END;

    const auto entry_count = tetengo_lattice_vocabulary_findEntries(p_vocabulary, p_input, nullptr);
    BOOST_TEST_REQUIRE(entry_count == 2U);

    std::vector<tetengo_lattice_entryView_t> entries(entry_count);
    const auto entry_count_again = tetengo_lattice_vocabulary_findEntries(p_vocabulary, p_input, std::data(entries));
    BOOST_TEST(entry_count_again == entry_count);
    const auto* const p_entry_key0 = tetengo_lattice_entryView_createKeyOf(entries[0].key_handle);
    BOOST_SCOPE_EXIT(p_entry_key0)
    {
        tetengo_lattice_input_destroy(p_entry_key0);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_entry_key0);
    BOOST_TEST((std::string_view{ tetengo_lattice_stringInput_value(p_entry_key0) } == "key0"));
    const auto* const entry_value0 = reinterpret_cast<const char*>(
        *std::any_cast<const void*>(reinterpret_cast<const std::any*>(entries[0].value_handle)));
    BOOST_TEST((std::string_view{ entry_value0 } == "hoge"));
    BOOST_TEST(entries[0].cost == 0);
    const auto* const p_entry_key1 = tetengo_lattice_entryView_createKeyOf(entries[1].key_handle);
    BOOST_SCOPE_EXIT(p_entry_key1)
    {
        tetengo_lattice_input_destroy(p_entry_key1);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_entry_key1);
    BOOST_TEST((std::string_view{ tetengo_lattice_stringInput_value(p_entry_key1) } == "key1"));
    const auto* const entry_value1 = reinterpret_cast<const char*>(
        *std::any_cast<const void*>(reinterpret_cast<const std::any*>(entries[1].value_handle)));
    BOOST_TEST((std::string_view{ entry_value1 } == "fuga"));
    BOOST_TEST(entries[1].cost == 1);
}

BOOST_AUTO_TEST_CASE(find_connection)
{
    BOOST_TEST_PASSPOINT();

    std::vector<std::string>                           context{ "hoge", "fuga" };
    const tetengo_lattice_customVocabularyDefinition_t definition{ &context,
                                                                   find_entries_procedure,
                                                                   find_connection_procedure };
    const auto* const p_vocabulary = tetengo_lattice_vocabulary_createCustomVocabulary(&definition);
    BOOST_SCOPE_EXIT(p_vocabulary)
    {
        tetengo_lattice_vocabulary_destroy(p_vocabulary);
    }
    BOOST_SCOPE_EXIT_END;
    BOOST_TEST_REQUIRE(p_vocabulary);

    const auto* const p_key_from = tetengo_lattice_input_createStringInput("key_from");
    BOOST_SCOPE_EXIT(p_key_from)
    {
        tetengo_lattice_input_destroy(p_key_from);
    }
    BOOST_SCOPE_EXIT_END;
    const std::any               value_from{ reinterpret_cast<const void*>("value_from") };
    const tetengo_lattice_node_t from{ tetengo_lattice_entryView_toKeyHandle(p_key_from),
                                       reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value_from),
                                       0,
                                       0,
                                       nullptr,
                                       0,
                                       0,
                                       0,
                                       0 };
    const auto* const            p_key_to = tetengo_lattice_input_createStringInput("key_to");
    BOOST_SCOPE_EXIT(p_key_to)
    {
        tetengo_lattice_input_destroy(p_key_to);
    }
    BOOST_SCOPE_EXIT_END;
    const std::any                    value_to{ reinterpret_cast<const void*>("value_to") };
    const tetengo_lattice_entryView_t to{ tetengo_lattice_entryView_toKeyHandle(p_key_to),
                                          reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&value_to),
                                          0 };
    {
        tetengo_lattice_connection_t connection{};
        const auto result = tetengo_lattice_vocabulary_findConnection(p_vocabulary, &from, &to, &connection);
        BOOST_TEST_REQUIRE(result);
        BOOST_TEST(connection.cost == 42);
    }
    {
        const auto result = tetengo_lattice_vocabulary_findConnection(p_vocabulary, &from, &to, nullptr);
        BOOST_TEST(!result);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
