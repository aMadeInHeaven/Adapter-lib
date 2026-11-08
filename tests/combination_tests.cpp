#include <processing.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <expected>

struct User {
    int id;
    std::string name;
    bool operator==(const User& other) const = default;
};

struct Order {
    int user_id;
    int amount;
};

TEST(CombinationTest, FilterTransformAggregate) {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto result = AsDataFlow(numbers)
        | Filter([](int x) { return x % 2 == 0; })
        | Transform([](int x) { return x * x; })
        | AggregateByKey(
            0,
            [](int val, int& acc) { acc += val; },
            [](int val) { return val % 10; }
        )
        | AsVector();

    EXPECT_THAT(result, testing::UnorderedElementsAre(
        std::make_pair(4, 68),
        std::make_pair(6, 52),
        std::make_pair(0, 100)
    ));
}

TEST(CombinationTest, SplitJoinDropNullopt) {
    std::vector<std::string> raw_data = {"1:Alice", "2:Bob", "3:Charlie"};

    std::vector<KV<int, int>> ages = {{1, 20}, {3, 30}};

    auto result = AsDataFlow(raw_data)
        | Transform([](const std::string& s) {
            auto pos = s.find(':');
            return KV<int, std::string>{ std::stoi(s.substr(0, pos)), s.substr(pos+1) };
        })
        | Join(AsDataFlow(ages)) 
        | AsVector();

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].base, "Alice");
    EXPECT_EQ(result[0].joined, 20);
    EXPECT_EQ(result[1].joined, std::nullopt);

    auto names_with_age = AsDataFlow(result)
        | Transform([](const auto& res) { return res.joined; })
        | DropNullopt()
        | AsVector();

    EXPECT_THAT(names_with_age, testing::ElementsAre(20, 30));
}

TEST(ComplexCombinationTest, UserOrderAnalytics) {
    std::vector<User> users = {{1, "Alice"}, {2, "Bob"}, {3, "Charlie"}};
    std::vector<Order> orders = {
        {1, 100}, {1, 200},
        {2, 50},
        {1, 50}
    };
    
    auto result = AsDataFlow(orders)
        | Filter([](const Order& o) { return o.amount > 60; })
        | Join(
            AsDataFlow(users),
            [](const Order& o) { return o.user_id; },
            [](const User& u) { return u.id; }
          )
        | AggregateByKey(
            0,
            [](const JoinResult<Order, User>& res, int& sum) { 
                sum += res.base.amount; 
            },
            [](const JoinResult<Order, User>& res) { 
                return res.joined.has_value() ? res.joined->name : "Unknown"; 
            }
          )
        | Enumerate()
        | AsVector();

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].first, 0uz);
    EXPECT_EQ(result[0].second.first, "Alice");
    EXPECT_EQ(result[0].second.second, 300);
}

TEST(ComplexCombinationTest, CSVErrorHandling) {
    std::vector<std::string> csv_lines = {
        "100,Valid",
        "invalid_line",
        "200,AlsoValid",
        ""
    };

    auto parse_csv = [](const std::string& line) -> std::expected<KV<int, std::string>, std::string> {
        auto pos = line.find(',');
        if (pos == std::string::npos) return std::unexpected("Format error: " + line);
        return KV<int, std::string>{std::stoi(line.substr(0, pos)), line.substr(pos + 1)};
    };

    auto [bad_flows, good_flows] = AsDataFlow(csv_lines)
        | Enumerate()
        | Transform([&](const std::pair<size_t, std::string>& p) {
            auto res = parse_csv(p.second);
            if (!res) return std::expected<std::string, std::string>(std::unexpected("Line " + std::to_string(p.first) + ": " + res.error()));
            return std::expected<std::string, std::string>(res->value);
          })
        | SplitExpected();

    EXPECT_THAT(bad_flows, testing::ElementsAre(
        "Line 1: Format error: invalid_line",
        "Line 3: Format error: "
    ));

    EXPECT_THAT(good_flows, testing::ElementsAre("Valid", "AlsoValid"));
}

TEST(ComplexCombinationTest, TopNWordsFrequency) {
    std::vector<std::stringstream> files(2);
    files[0] << "apple banana apple";
    files[1] << "cherry banana banana apple orange";

    auto result = AsDataFlow(files)
        | Split(" ")
        | Transform([](std::string s) {
            for (char& c : s) c = std::tolower(c);
            return s;
        })
        | AggregateByKey(
            0uz,
            [](const std::string&, size_t& count) { ++count; },
            [](const std::string& s) { return s; }
        )
        | Take(2)
        | AsVector();

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].first, "apple");
    EXPECT_EQ(result[0].second, 3uz);
    EXPECT_EQ(result[1].first, "banana");
    EXPECT_EQ(result[1].second, 3uz);
}
