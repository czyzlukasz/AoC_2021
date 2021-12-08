#include <algorithm>
#include <numeric>
#include <iostream>
#include "Utils.hpp"

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_7.txt)";
  auto values = readCommaSeparatedValues<unsigned int>(inputFile);

  std::sort(values.begin(), values.end());

  const unsigned int lowestValue = values.front();
  const unsigned int highestValue = values.back();

  auto lowestCost = std::numeric_limits<unsigned int>::max();
  for(auto position = lowestValue; position <= highestValue; ++position) {
    lowestCost = std::min(lowestCost, std::accumulate(values.cbegin(), values.cend(), unsigned(0), [position](unsigned int result, unsigned int positionOfCrab) -> unsigned int{
      const auto distance = std::abs(static_cast<signed>(positionOfCrab) - static_cast<signed>(position));

      return result + static_cast<unsigned int>((distance / 2.f) * static_cast<float>(distance + 1));
    }));
  }

  std::cout << "Lowest cost is: " << lowestCost << std::endl;
}
