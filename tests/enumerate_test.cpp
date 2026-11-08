#include <processing.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>
#include <string>

TEST(EnumerateTest, BasicEnumerate) {
    std::vector<std::string> input = {"apple", "banana", "cherry"};

    auto result = AsDataFlow(input) | Enumerate() | AsVector();
    
    ASSERT_EQ(result.size(), 3);

    EXPECT_EQ(result[0], std::make_pair(0uz, std::string("apple")));
    EXPECT_EQ(result[1], std::make_pair(1uz, std::string("banana")));
    EXPECT_EQ(result[2], std::make_pair(2uz, std::string("cherry")));
}

TEST(EnumerateTest, EnumerateEmpty) {
    std::vector<int> input = {};
    auto result = AsDataFlow(input) | Enumerate() | AsVector();
    ASSERT_TRUE(result.empty());
}

TEST(EnumerateTest, EnumerateAfterFilter) {
    std::vector<int> input = {1, 2, 3, 4, 5, 6};
    
    auto result = AsDataFlow(input) 
        | Filter([](int x) { return x % 2 == 0; }) 
        | Enumerate() 
        | AsVector();
        
    ASSERT_THAT(
        result, 
        testing::ElementsAre(
            std::make_pair(0uz, 2),
            std::make_pair(1uz, 4),
            std::make_pair(2uz, 6)
        )
    );
}

TEST(EnumerateTest, FilterAfterEnumerate) {
    std::vector<std::string> input = {"keep", "ignore", "keep", "ignore"};

    auto result = AsDataFlow(input)
        | Enumerate() 
        | Filter([](const std::pair<size_t, std::string>& p) { return p.second == "keep"; })
        | AsVector();
        
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].first, 0uz);
    EXPECT_EQ(result[1].first, 2uz);
}

TEST(EnumerateTest, EnumerateTransformCombination) {
    std::vector<std::string> input = {"red", "green", "blue"};
    
    auto result = AsDataFlow(input)
        | Enumerate()
        | Transform([](const std::pair<size_t, std::string>& p) {
            return std::to_string(p.first) + ": " + p.second;
        })
        | AsVector();
        
    EXPECT_THAT(result, testing::ElementsAre("0: red", "1: green", "2: blue"));
}

TEST(EnumerateTest, HugeIndex) {
    std::vector<int> input(1001, 42);
    
    auto result = AsDataFlow(input)
        | Enumerate()
        | Take(1001)
        | AsVector();
        
    EXPECT_EQ(result.back().first, 1000uz);
    EXPECT_EQ(result.back().second, 42);
}