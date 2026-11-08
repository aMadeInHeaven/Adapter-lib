#include <iostream>
#include <expected>
#include <cstring>
#include <algorithm>
#include <string>
#include <ranges>
#include <iterator>
#include <optional>

#include "lib/processing.h"

int main() {
    int global = 0;
    Dir("../empty", true) 
          | Filter([](const std::filesystem::path& p){ return p.extension() == ".cpp"; }) 
          | OpenFiles() 
          | Split("() ;\n,") 
          | Transform([&global](auto&& g) -> std::optional<std::string> {
              
              if (g == "const") {
                  global++;
                  return g;
                }

              return std::nullopt;
            })
          | DropNullopt()
          | Transform([](auto&& g){
              return *g;
            })
          | Out(std::cout);
    std::cout << global;
    return 0;
}