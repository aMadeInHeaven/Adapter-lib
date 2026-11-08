#include <processing.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <expected>
#include <sstream>
#include <string>
#include <vector>

struct Department {
    std::string name;

    bool operator==(const Department& other) const = default;
};

std::expected<Department, std::string> ParseDepartment(const std::string& str) {
    if (str.empty()) {
        return std::unexpected("Department name is empty");
    }
    if (str.contains(' ')) {
        return std::unexpected("Department name contains space");
    }
    return Department{str};
}

TEST(SplitExpectedTest, SplitExpected) {
    std::vector<std::stringstream> files(1);
    files[0] << "good-department|bad department||another-good-department";

    auto [unexpected_flow, good_flow] = AsDataFlow(files) | Split("|") | Transform(ParseDepartment) | SplitExpected();

    std::stringstream unexpected_file;
    unexpected_flow | Write(unexpected_file, '.');

    auto expected_result = good_flow | AsVector();

    ASSERT_EQ(unexpected_file.str(), "Department name contains space.Department name is empty.");
    ASSERT_THAT(expected_result, testing::ElementsAre(Department{"good-department"}, Department{"another-good-department"}));
}

/////////////////////////// My tests ////////////////////////////////////////////

TEST(SplitExpectedTest, OnlyErrors) {
    using Exp = std::expected<int, std::string>;
    std::vector<Exp> input = { 
        std::unexpected("err1"), 
        std::unexpected("err2") 
    };

    auto [errors, goods] = AsDataFlow(input) | SplitExpected();

    EXPECT_THAT(errors, testing::ElementsAre("err1", "err2"));
    auto res = goods | AsVector();
    EXPECT_TRUE(res.empty());
}

TEST(SplitExpectedTest, EmptyFlow) {
    using Exp = std::expected<int, std::string>;
    std::vector<Exp> input = {};

    auto [errors, goods] = AsDataFlow(input) | SplitExpected();

    auto res1 = errors | AsVector();
    auto res2 = goods | AsVector();
    EXPECT_TRUE(res1.empty());
    EXPECT_TRUE(res2.empty());
}