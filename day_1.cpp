#include "Utils.hpp"
#include <numeric>
#include <iostream>
#include <algorithm>
#include <functional>
#include <concepts>

void partOne(const auto& inputData) {
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

template<unsigned averagingSize>
void partTwo(const auto& inputData) {
  const auto intermediateValues = [inputData]() -> std::vector<std::tuple<unsigned>>{
    std::vector<std::tuple<unsigned>> result{};
    result.emplace_back(std::numeric_limits<unsigned>::max());

    std::transform(inputData.cbegin(), inputData.cend() - averagingSize, std::back_inserter(result), [&inputData](const auto& element) -> unsigned {
      const auto it = inputData.cbegin() + std::distance(inputData.data(), &element);
      return std::accumulate(it, it + averagingSize, 0, [](const unsigned& result, const auto& value) {
        return result + std::get<0>(value);
      });
    });

    return result;
  }();

  partOne(intermediateValues);
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\test.txt)";
  const auto inputData = readFromFile<unsigned>(inputFile);

  partOne(inputData);
  partTwo<3>(inputData);
}