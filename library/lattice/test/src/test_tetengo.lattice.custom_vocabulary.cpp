/*! \file
    \brief A custom vocabulary.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/vocabulary.h>


namespace
{
    size_t find_entries_procedure(
        void* const /*p_context*/,
        const tetengo_lattice_vocabulary_t* const /*p_vocabulary*/,
        const tetengo_lattice_input_t* const /*p_key*/,
        tetengo_lattice_entryView_t* const /*p_entries*/)
    {
        return 0;
    }

    int find_connection_procedure(
        void* const /*p_context*/,
        const tetengo_lattice_vocabulary_t* const /*p_vocabulary*/,
        const tetengo_lattice_node_t* const /*p_from*/,
        const tetengo_lattice_entryView_t* const /*p_to*/,
        tetengo_lattice_connection_t* const /*p_connection*/)
    {
        return 0;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(custom_vocabulary)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo_lattice_customVocabularyDefinition_t definition{
        42, nullptr, find_entries_procedure, find_connection_procedure
    };
    std::string       context{ "hoge" };
    const auto* const p_vocabulary = tetengo_lattice_vocabulary_createCustomVocabulary(&definition, &context);
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

    BOOST_WARN_MESSAGE(false, "Implemtn it.");
}

BOOST_AUTO_TEST_CASE(find_connection)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implemtn it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
