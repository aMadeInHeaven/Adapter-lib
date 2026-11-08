#include <processing.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>
#include <string>

TEST(TakeTest, BasicTake) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    auto result = AsDataFlow(input) | Take(3) | AsVector();
    ASSERT_THAT(result, testing::ElementsAre(1, 2, 3));
}

TEST(TakeTest, TakeZero) {
    std::vector<int> input = {1, 2, 3};
    auto result = AsDataFlow(input) | Take(0) | AsVector();
    ASSERT_TRUE(result.empty());
}

TEST(TakeTest, TakeMoreThanAvailable) {
    std::vector<int> input = {10, 20};
    auto result = AsDataFlow(input) | Take(5) | AsVector();
    ASSERT_THAT(result, testing::ElementsAre(10, 20));
}

TEST(TakeTest, TakeFromEmpty) {
    std::vector<int> input = {};
    auto result = AsDataFlow(input) | Take(1) | AsVector();
    ASSERT_TRUE(result.empty());
}

TEST(TakeTest, LazyEvaluationProof) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    int transformations = 0;

    auto result = AsDataFlow(input) 
        | Take(2) 
        | Transform([&transformations](int x) { 
            transformations++; 
            return x * 10; 
          }) 
        | AsVector();

    ASSERT_THAT(result, testing::ElementsAre(10, 20));
    EXPECT_EQ(transformations, 2);
}

TEST(TakeTest, PipelineCombination) {
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto result = AsDataFlow(input)
        | Filter([](int x) { return x % 2 != 0; })
        | Take(2)
        | Transform([](int x) { return x * x; })
        | AsVector();

    ASSERT_THAT(result, testing::ElementsAre(1, 9));
}

TEST(TakeTest, TakeAfterSplit) {
    std::vector<std::stringstream> streams(1);
    streams[0] << "word1 word2 word3 word4";

    auto result = AsDataFlow(streams)
        | Split(" ")
        | Take(2)
        | AsVector();

    ASSERT_THAT(result, testing::ElementsAre("word1", "word2"));
}