#include <map>
#include <iostream>
#include <algorithm>
#include <array>
#include "Utils.hpp"

struct PairPattern {
  PairPattern() = default;
  PairPattern(const char a, const char b) : pattern{a, b} {}

  bool operator == (const PairPattern& other) const {
    for(size_t idx = 0; idx < insertionPairSize; ++idx) {
      if(pattern[idx] != other.pattern[idx]) {
        return false;
      }
    }
    return true;
  }

  bool operator < (const PairPattern& other) const {
    return pattern[0] == other.pattern[0] ? pattern[1] < other.pattern[1] : pattern[0] < other.pattern[0];
  }

  static constexpr size_t insertionPairSize = 2;
  char pattern[insertionPairSize];
};

struct PairInsertion {
  PairInsertion(const std::string& input) {
    static const size_t separatorSize = strlen(" -> ");
    std::copy(input.cbegin(), input.cbegin() + PairPattern::insertionPairSize, pairPattern.pattern);
    outputCharacter = input.at(PairPattern::insertionPairSize + separatorSize);
  }

  bool matches(const PairPattern& other) const {
    return pairPattern == other;
  }

  std::array<PairPattern, 2> generatePairPatternsFromInsertion(const PairPattern& pairPattern_) const{
    return {PairPattern{pairPattern_.pattern[0], outputCharacter}, {outputCharacter, pairPattern_.pattern[1]}};
  }

  PairPattern pairPattern{};
  char outputCharacter;
};

struct PairContainer {
  using PatternContainer = std::map<PairPattern, uint64_t>;
  PairContainer(const std::string& input) {
    for(auto it = input.cbegin(); it + 1 != input.cend(); ++it) {
      ++patternFrequency[PairPattern{*it, *(it+1)}];
    }

    lastPair = PairPattern{*(input.cend() - 2), *(input.cend() - 1)};
  }

  void iterate(const std::vector<PairInsertion>& pairInsertions){
    PatternContainer result = patternFrequency;
    for(const auto& [key, value] : patternFrequency) {
      for(const PairInsertion& pairInsertion : pairInsertions) {
        if(pairInsertion.matches(key)) {
          const auto matchingPairFrequency = patternFrequency[key];
          result[key] -= matchingPairFrequency;
          for (const auto &newPair: pairInsertion.generatePairPatternsFromInsertion(key)) {
            result[newPair] += matchingPairFrequency;
          }
        }
      }
    }

    // Calculate last pair in order to aid in calculating number of pairs
    for(const PairInsertion& pairInsertion : pairInsertions) {
      if(pairInsertion.matches(lastPair)) {
        lastPair = pairInsertion.generatePairPatternsFromInsertion(lastPair).at(1);
        break;
      }
    }
    patternFrequency = result;
  }

  void printPairs() const {
    for(const auto& [key, value] : patternFrequency) {
      if(value) {
        std::cout << key.pattern[0] << key.pattern[1] << ": " << value << std::endl;
      }
    }
  }

  std::map<char, uint64_t> calculateCharacterFrequency() const {
    std::map<char, uint64_t> result;

    for(const auto& [key, value] : patternFrequency) {
      if(value) {
        result[key.pattern[0]] += value;
      }
    }

    ++result[lastPair.pattern[1]];
    return result;
  }

  PatternContainer patternFrequency;
  PairPattern lastPair;
};

int main() {
//  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\test.txt)";
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_14.txt)";
  const auto inputData = readLineByLine(inputFile);

  const std::string& polymerTemplate = inputData.front();

  std::vector<PairInsertion> pairInsertion;
  std::copy(inputData.cbegin() + 2, inputData.cend(), std::back_inserter(pairInsertion));

  PairContainer pairContainer(polymerTemplate);

  for(uint64_t idx = 1; idx <= 40; ++idx) {
    std::cout << "Step " << idx << std::endl;
    pairContainer.iterate(pairInsertion);
  }
  std::cout << "Pairs: " << pairContainer.patternFrequency.size() << std::endl;
  pairContainer.printPairs();
  const auto resultMap = pairContainer.calculateCharacterFrequency();
  std::vector<std::pair<char, uint64_t>> result;
  std::copy(resultMap.cbegin(), resultMap.cend(), std::back_inserter(result));

  std::sort(result.begin(), result.end(), [](const auto& a, const auto& b){return a.second < b.second;});
  for(const auto& [key, value] : result) {
      std::cout << key << ": " << value << std::endl;
  }
  std::cout << "Difference is: " << result.back().second - result.front().second << std::endl;
}
