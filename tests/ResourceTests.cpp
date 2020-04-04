//
// Created by Ciaran on 4/4/2020.
//

#include <semsim/Resource.h>
#include <semsim/uri.h>
#include "gtest/gtest.h"


class ResourceTests : public ::testing::Test {
public:

    std::string ns;
    std::string id;

    void SetUp() override {
        ns = "genecards";
        id = "ABL1";
    }
};

TEST_F(ResourceTests, TestGetResourceNamespace) {
    semsim::Resource resource(ns, id);
    const std::string &expected = "genecards";
    const std::string &ns2 = resource.getResourceNamespace();
    ASSERT_STREQ(expected.c_str(), ns2.c_str());
}


TEST_F(ResourceTests, TestGetUri) {
    semsim::Resource resource(ns, id);
    const std::string &expected = "ABL1";
    const std::string &actual = resource.getIdentifier();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(ResourceTests, TestEqualityOperator) {
    semsim::Resource resource1(ns, id);
    semsim::Resource resource2(ns, id);
    ASSERT_EQ(resource1, resource2);
}

TEST_F(ResourceTests, TestInequalityOperator) {
    semsim::Resource resource1(ns, id);
    semsim::Resource resource2(ns, "cheese");
    ASSERT_NE(resource1, resource2);
}

TEST_F(ResourceTests, TestStreamOperator) {
    semsim::Resource resource1(ns, id);
    std::ostringstream os;
    os << resource1;
    std::string expected = "Resource(https://identifiers.org/genecards/ABL1)";
    ASSERT_STREQ(expected.c_str(), os.str().c_str());
}


TEST_F(ResourceTests, TestStringSpliter1) {
    std::vector<std::string> expected = {"cheese"};
    std::vector<std::string> actual = semsim::Resource::splitStringBy("cheese", ':');
    ASSERT_EQ(expected, actual);
}

TEST_F(ResourceTests, TestStringSpliter2) {
    std::vector<std::string> expected = {"cheese", "biscuits"};
    std::vector<std::string> actual = semsim::Resource::splitStringBy("cheese:biscuits", ':');
    ASSERT_EQ(expected, actual);
}


TEST_F(ResourceTests, TestFromString1) {
    semsim::Resource resource1("genecards/ABL1");
    const std::string& actual = resource1.getResourceNamespace();
    std::string expected = "genecards";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(ResourceTests, TestFromString2) {
    semsim::Resource resource1("genecards:ABL1");
    const std::string& actual = resource1.getResourceNamespace();
    std::string expected = "genecards";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(ResourceTests, TestFromString3) {
    ASSERT_THROW(semsim::Resource resource1("genecardsABL1"), std::logic_error);
}














