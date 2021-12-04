#include "Utils.hpp"
#include <string>
#include <bitset>
#include <array>
#include <iostream>


template<size_t numOfBits>
void partOne(const auto &input) {
  using BitSet = std::bitset<numOfBits>;
//  using BitSetVector = std::vector<BitSet>;
  using BitCountArray = std::array<int, numOfBits>;
  // Assume true adds 1 to result value, false subtracts 1
  BitCountArray bitCount{};
  static const auto sumBits = [&bitCount](const BitSet& bits) {
    for(size_t idx = 0; idx < numOfBits; ++idx) {
      bitCount.at(idx) += bits.test(idx) ? 1 : -1;
    }
  };

  for (const auto&[line]: input) {
    sumBits(BitSet(line));
  }

  BitSet result;
  for(size_t idx = 0; idx < numOfBits; ++idx) {
    result.set(idx, bitCount.at(idx) > 0);
  }
  const auto gamma = result.to_ullong();
  result.flip();
  const auto epsilon = result.to_ullong();

  std::cout << "Gamma: " << gamma << ", epsilon: " << epsilon << ", multiplied: " << gamma * epsilon << std::endl;
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_3.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  partOne<12>(inputData);
}