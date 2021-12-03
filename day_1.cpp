#include "Utils.hpp"
#include <numeric>
#include <iostream>

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\test.txt)";

  const auto inputData = readFromFile<unsigned>(inputFile);

  unsigned previousValue = std::get<0>(inputData.front());
  static const auto comparator = [&previousValue](const unsigned result, const auto& value){
    const auto [currentValue] = value;
    const unsigned isBigger = previousValue < currentValue;
    previousValue = currentValue;

    return result + isBigger;
  };
  const auto result = std::accumulate(std::next(inputData.cbegin()), inputData.cend(), 0, comparator);

  std::cout << "Result is: " << result << std::endl;
}