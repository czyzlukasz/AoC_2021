#include "Utils.hpp"
#include <string>


int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_3.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  partOne(inputData);
}