#include <list>
#include <iostream>
#include <algorithm>
#include "Utils.hpp"

struct PairInsertion {
  PairInsertion(const std::string& input) {
    static const size_t separatorSize = strlen(" -> ");
    std::fill(pattern, pattern + sizeof(pattern), '\0');
    std::copy(input.cbegin(), input.cbegin() + insertionPairSize, pattern);
    outputCharacter = input.at(insertionPairSize + separatorSize);
  }

  static constexpr size_t insertionPairSize = 2;
  char pattern[insertionPairSize + 1]{};
  char outputCharacter;
};

void findPolymer(const std::vector<PairInsertion>& pairInsertion, const auto inputIt, std::insert_iterator<std::list<char>> resultIt) {
  for(const PairInsertion& pairToMatch : pairInsertion) {
    const std::string_view polymerPair(inputIt, inputIt + PairInsertion::insertionPairSize);
    if(polymerPair.compare(0, PairInsertion::insertionPairSize, pairToMatch.pattern) == 0) {
      resultIt = pairToMatch.outputCharacter;
      return;
    }
  }
}

std::string iterate(const std::vector<PairInsertion>& pairInsertion, const std::string& polymerTemplate) {
  std::list<char> resultPolymerTemplate(polymerTemplate.cbegin(), polymerTemplate.cend());

  auto resultIt = std::next(resultPolymerTemplate.begin());
  for(auto inputIt = polymerTemplate.cbegin(); inputIt <= polymerTemplate.cend() - PairInsertion::insertionPairSize; ++inputIt) {
    findPolymer(pairInsertion, inputIt, std::inserter(resultPolymerTemplate, resultIt));
    ++resultIt;
  }

  return {resultPolymerTemplate.cbegin(), resultPolymerTemplate.cend()};
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_14.txt)";
  const auto inputData = readLineByLine(inputFile);

  const std::string& polymerTemplate = inputData.front();

  std::vector<PairInsertion> pairInsertion;
  std::copy(inputData.cbegin() + 2, inputData.cend(), std::back_inserter(pairInsertion));

  std::string result = polymerTemplate;
  for(size_t idx = 1; idx <= 10; ++idx) {
    std::cout << "Step " << idx << std::endl;
    result = iterate(pairInsertion, result);
  }

  std::cout << "Result has length of " << result.size() << std::endl;

  std::vector<std::pair<char, size_t>> numOfOccurrences;

  for(char character = 'A'; character <= 'Z'; ++character) {
    numOfOccurrences.emplace_back(character, std::count(result.cbegin(), result.cend(), character));
  }

  std::sort(numOfOccurrences.begin(), numOfOccurrences.end(), [](const auto& a, const auto& b) {
    return a.second < b.second;
  });

  for(const auto& [character, frequency] : numOfOccurrences) {
    std::cout << character << ": " << frequency << std::endl;
  }
}
