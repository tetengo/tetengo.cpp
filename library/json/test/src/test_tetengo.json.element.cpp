/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/element.h>
#include <tetengo/json/element.hpp>


namespace
{
    using element_type = tetengo::json::element;


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(element)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::string,
                                         element_type::type_category_type::structure_open },
                "test",
                std::unordered_map<std::string, std::string>{}),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::string,
                                         element_type::type_category_type::primitive },
                "test",
                std::unordered_map<std::string, std::string>{ { "key", "value" } }),
            std::invalid_argument);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::object,
                                         element_type::type_category_type::primitive },
                std::string{},
                std::unordered_map<std::string, std::string>{}),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::object,
                                         element_type::type_category_type::structure_open },
                "test",
                std::unordered_map<std::string, std::string>{}),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::object,
                                         element_type::type_category_type::structure_open },
                std::string{},
                std::unordered_map<std::string, std::string>{ { "key", "value" } }),
            std::invalid_argument);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::member,
                                         element_type::type_category_type::structure_open },
                std::string{},
                std::unordered_map<std::string, std::string>{ { "key", "value" } }),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::member,
                                         element_type::type_category_type::structure_close },
                std::string{},
                std::unordered_map<std::string, std::string>{ { "name", "value" } }),
            std::invalid_argument);
    }

    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_element);
    }
    {
        const auto* const p_element = tetengo_json_element_create(nullptr, "test", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, nullptr, nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "key", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "test", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureClose() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "key", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "test", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "key", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureClose() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element1 = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element1)
        {
            tetengo_json_element_destroy(p_element1);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element1);

        const auto* const p_element2 = tetengo_json_element_copy(p_element1);
        BOOST_SCOPE_EXIT(p_element2)
        {
            tetengo_json_element_destroy(p_element2);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element2);

        // TODO equivalence checks
    }
    {
        const auto* const p_element = tetengo_json_element_copy(nullptr);
        BOOST_TEST(!p_element);
    }
}

BOOST_AUTO_TEST_CASE(type)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_CHECK(element.type().name == element_type::type_name_type::string);
        BOOST_CHECK(element.type().category == element_type::type_category_type::primitive);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_CHECK(element.type().name == element_type::type_name_type::object);
        BOOST_CHECK(element.type().category == element_type::type_category_type::structure_close);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };

        BOOST_CHECK(element.type().name == element_type::type_name_type::member);
        BOOST_CHECK(element.type().category == element_type::type_category_type::structure_open);
    }

    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const p_type = tetengo_json_element_type(p_element);
        BOOST_TEST_REQUIRE(p_type);
        BOOST_TEST(p_type->name == tetengo_json_element_typeName_string());
        BOOST_TEST(p_type->category == tetengo_json_element_typeCategory_primitive());
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureClose() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const p_type = tetengo_json_element_type(p_element);
        BOOST_TEST_REQUIRE(p_type);
        BOOST_TEST(p_type->name == tetengo_json_element_typeName_object());
        BOOST_TEST(p_type->category == tetengo_json_element_typeCategory_structureClose());
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const p_type = tetengo_json_element_type(p_element);
        BOOST_TEST_REQUIRE(p_type);
        BOOST_TEST(p_type->name == tetengo_json_element_typeName_member());
        BOOST_TEST(p_type->category == tetengo_json_element_typeCategory_structureOpen());
    }
    {
        const auto* const p_type = tetengo_json_element_type(nullptr);
        BOOST_TEST(!p_type);
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.value() == "test");
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.value().empty());
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };

        BOOST_TEST(element.value().empty());
    }

    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const value = tetengo_json_element_value(p_element);
        BOOST_TEST_REQUIRE(value);
        BOOST_TEST(std::string_view(value) == "test");
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureClose() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const value = tetengo_json_element_value(p_element);
        BOOST_TEST_REQUIRE(value);
        BOOST_TEST(std::string_view(value) == "");
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const value = tetengo_json_element_value(p_element);
        BOOST_TEST_REQUIRE(value);
        BOOST_TEST(std::string_view(value) == "");
    }
    {
        const auto* const value = tetengo_json_element_value(nullptr);
        BOOST_TEST(!value);
    }
}

BOOST_AUTO_TEST_CASE(attributes)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.attributes().empty());
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.attributes().empty());
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };

        BOOST_TEST(element.attributes().size() == 1U);
        BOOST_CHECK(element.attributes().find("name") != std::end(element.attributes()));
        BOOST_TEST(element.attributes().find("name")->second == "value");
    }

    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto attribute_count = tetengo_json_element_attributeKeys(p_element, nullptr);
        BOOST_TEST(attribute_count == 0U);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureClose() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto attribute_count = tetengo_json_element_attributeKeys(p_element, nullptr);
        BOOST_TEST(attribute_count == 0U);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto attribute_count = tetengo_json_element_attributeKeys(p_element, nullptr);
        BOOST_TEST_REQUIRE(attribute_count == 1U);
        std::vector<const char*> attribute_keys(attribute_count, nullptr);
        const auto attribute_count_again = tetengo_json_element_attributeKeys(p_element, attribute_keys.data());
        BOOST_TEST(attribute_count_again == attribute_count);
        BOOST_TEST_REQUIRE(attribute_keys[0]);
        BOOST_TEST(std::string_view(attribute_keys[0]) == "name");
        const auto* const attribute_value = tetengo_json_element_attributeValueOf(p_element, attribute_keys[0]);
        BOOST_TEST_REQUIRE(attribute_value);
        BOOST_TEST(std::string_view(attribute_value) == "value");
    }
    {
        const auto attribute_count = tetengo_json_element_attributeKeys(nullptr, nullptr);
        BOOST_TEST(attribute_count == 0U);
    }
    {
        const auto* const attribute_value = tetengo_json_element_attributeValueOf(nullptr, "name");
        BOOST_TEST(!attribute_value);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const attribute_value = tetengo_json_element_attributeValueOf(p_element, nullptr);
        BOOST_TEST(!attribute_value);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_element);

        const auto* const attribute_value = tetengo_json_element_attributeValueOf(p_element, "key");
        BOOST_TEST(!attribute_value);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
