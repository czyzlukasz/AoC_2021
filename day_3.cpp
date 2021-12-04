#include "Utils.hpp"
#include <string>
#include <bitset>
#include <array>
#include <iostream>
#include <numeric>


template<size_t numOfBits>
void partOne(const auto &input) {
  using BitSet = std::bitset<numOfBits>;
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

template<size_t numOfBits>
void partTwo(const auto &input) {
  using BitSet = std::bitset<numOfBits>;
  using BitSetVector = std::vector<BitSet>;

  BitSetVector bitSetVector{};

  // Convert string input to vector of bitsets
  for (const auto&[line]: input) {
    bitSetVector.emplace_back(BitSet(line));
  }

  // Calculate collection matching requirements. First requirement is operator (most common bit - std::greater or least common - std::less), second is value that is being counted
  static const auto determineNextBitSetCollection = []<typename UnaryOp>(const BitSetVector& inputCollection, const size_t bitPosition) -> BitSetVector{
    BitSetVector result{};
    // Invert position (bitPostition == 0 -> MSB)
    const size_t invertedBitPosition = BitSet{}.size() - bitPosition - 1;
    const auto numOfCountedValue = std::accumulate(inputCollection.cbegin(), inputCollection.cend(), 0, [invertedBitPosition](const int result, const BitSet& input) {
      return result + (input.test(invertedBitPosition)? 1 : -1);
    });

    const bool bitsToSave = UnaryOp{}(numOfCountedValue, 0);
    std::cout << "numOfCountedValue: " << numOfCountedValue << ", bitsToSave: " << bitsToSave << std::endl;
    for(const auto& input : inputCollection) {
      if (input.test(invertedBitPosition) == bitsToSave) [[likely]] {
        result.push_back(input);
      }
    }

    return result;
  };

  static const auto determineRating = [&bitSetVector]<typename UnaryOp>() -> auto{
    auto collectionForIteration = bitSetVector;
    for(size_t idx = 0; idx < numOfBits && collectionForIteration.size() > 1; ++idx) {
      collectionForIteration = determineNextBitSetCollection.template operator()<UnaryOp>(collectionForIteration, idx);
      std::cout << "Collection size: " << collectionForIteration.size() << std::endl;
    }

    return collectionForIteration.cbegin()->to_ulong();
  };

  const auto oxygenRating = determineRating.template operator()<std::greater_equal<>>();
  const auto co2Rating = determineRating.template operator()<std::less<>>();

  std::cout << "Result for Oxygen: " <<  oxygenRating << ", CO2: " << co2Rating << ", multiplying gives: " << oxygenRating * co2Rating << std::endl;
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_3.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  partTwo<12>(inputData);
}