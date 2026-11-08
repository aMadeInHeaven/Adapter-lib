#include <processing.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <format>

namespace fs = std::filesystem;

class LabExampleTest : public ::testing::Test {
protected:
    void SetUp() override {
        sandbox = fs::current_path() / "test_sandbox";
        fs::create_directories(sandbox / "sub");

        std::ofstream(sandbox / "file1.txt") << "Apple, banana; apple.";
        std::ofstream(sandbox / "sub/file2.txt") << "BANANA! apple?";
        std::ofstream(sandbox / "exclude.log") << "this should be ignored";
    }

    void TearDown() override {
        fs::remove_all(sandbox);
    }

    fs::path sandbox;
};

TEST_F(LabExampleTest, FullPipelineLogic) {
    std::stringstream output;

    Dir(sandbox, true)
        | Filter([](const fs::path& p) { 
            return p.extension() == ".txt"; 
          })
        | OpenFiles()
        | Split("\n ,.;!?")
        | Filter([](const std::string& token) { 
            return !token.empty(); 
          })
        | Transform([](std::string token) { 
            std::transform(token.begin(), token.end(), token.begin(), 
                           [](unsigned char c) { return std::tolower(c); });
            return token;
        })
        | AggregateByKey(
            0uz,
            [](const std::string&, size_t& count) { ++count; },
            [](const std::string& token) { return token; }
          )
        | Transform([](const std::pair<std::string, size_t>& stat) {
            return std::format("{} - {}", stat.first, stat.second);
        })
        | Out(output);

    std::string final_str = output.str();
    
    EXPECT_THAT(final_str, ::testing::HasSubstr("apple - 3"));
    EXPECT_THAT(final_str, ::testing::HasSubstr("banana - 2"));
    EXPECT_THAT(final_str, ::testing::Not(::testing::HasSubstr("ignored")));
}
