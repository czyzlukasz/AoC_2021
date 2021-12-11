#include <iostream>
#include <set>
#include <map>
#include <optional>
#include "Utils.hpp"

struct Digit {
  enum class DigitElement : char {
    a = 'a',
    b = 'b',
    c = 'c',
    d = 'd',
    e = 'e',
    f = 'f',
    g = 'g'
  };

  Digit(const std::string_view &stringRepr) : stringRepr(stringRepr) {
    for (const char element: stringRepr) {
      elements.emplace_back(static_cast<DigitElement>(element));
    }
  };

  bool is1or4or7or8() const {
    return stringRepr.size() == 2 ||  // 1
           stringRepr.size() == 4 ||  // 4
           stringRepr.size() == 3 ||  // 7
           stringRepr.size() == 7;    // 8
  }

  std::optional<std::pair<unsigned int, std::vector<DigitElement>>> getNumberFromDigit() const {
    if (is1or4or7or8()) {
      for (const auto&[possibleDigitKey, possibleDigitValue]: digits) {
        if (stringRepr.size() == possibleDigitValue.size()) {
          return std::make_pair(possibleDigitKey, possibleDigitValue);
        }
      }
    }
    return std::nullopt;
  }

  static std::map<DigitElement, DigitElement> calculateMapping(const std::initializer_list<Digit> &signals) {
    auto possiblePermutations = generateEveryMappingPermutation<DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::e, DigitElement::f, DigitElement::g>();
    std::cout << "Starting point" << std::endl;
    printMultimap(possiblePermutations);
    determine1and4and7and8(signals, possiblePermutations);
    std::cout << "After determining 1, 4, 7, 8" << std::endl;
    printMultimap(possiblePermutations);

    std::cout << "Iterating over all permutations" << std::endl;
    calculateRemainingMapping(possiblePermutations);
    return {};
  }

private:
  static void printMultimap(const std::multimap<DigitElement, DigitElement> &multimap) {
    for (const auto digitElement : allDigitElements) {
      const auto[lower, upper] = multimap.equal_range(digitElement);
      std::cout << static_cast<char>(digitElement) << ": ";
      for (auto it = lower; it != upper; ++it) {
        std::cout << static_cast<char>(it->second) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
  }

  // Generate every possible permutation of elements. Each key from parameter elements will be mapped to whole set of parameter elements
  template<DigitElement... elements>
  static std::multimap<DigitElement, DigitElement> generateEveryMappingPermutation() {
    static_assert(sizeof...(elements), "Set of elements cannot be empty");
    std::multimap<DigitElement, DigitElement> result;

    for (const auto digitElement : allDigitElements) {
      result.insert({{digitElement, elements}...});
    }

    return result;
  }

  static void determine1and4and7and8(const std::initializer_list<Digit> &signals,
                                     std::multimap<DigitElement, DigitElement> &possiblePermutations) {
    for (const auto &signal: signals) {
      // If given signal is a known number (1, 4, 7, 8)
      if (const auto digitOpt = signal.getNumberFromDigit()) {
        const auto&[digitValue, digitElements] = digitOpt.value();
        // Remove all impossible permutations and substitute them with possible ones
        for (const auto &digitElement: digits.at(digitValue)) {
          possiblePermutations.erase(digitElement);

          for (const auto possibleDigitElement: digitElements) {
            possiblePermutations.emplace(digitElement, possibleDigitElement);
          }
        }
      }
    }
  }

  static void calculateRemainingMapping(const std::multimap<DigitElement, DigitElement> &possiblePermutations) {

  }

  const std::string_view &stringRepr;
  std::vector<DigitElement> elements;

  // Definition of 7-segment digits. Each collection of digits elements[a-g] represents one unique digit[0-9]
  static const inline std::map<unsigned int, std::vector<DigitElement>> digits = {
          {0, {DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::e, DigitElement::f, DigitElement::g}},
          {1, {DigitElement::c, DigitElement::f}},
          {2, {DigitElement::a, DigitElement::c, DigitElement::d, DigitElement::e, DigitElement::g}},
          {3, {DigitElement::a, DigitElement::c, DigitElement::d, DigitElement::f, DigitElement::g}},
          {4, {DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::f}},
          {5, {DigitElement::a, DigitElement::b, DigitElement::d, DigitElement::f, DigitElement::g}},
          {6, {DigitElement::a, DigitElement::b, DigitElement::d, DigitElement::e, DigitElement::f, DigitElement::g}},
          {7, {DigitElement::a, DigitElement::c, DigitElement::f}},
          {8, {DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::e, DigitElement::f, DigitElement::g}},
          {9, {DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::f, DigitElement::g}},
  };

  static constexpr inline std::initializer_list<DigitElement> allDigitElements{DigitElement::a, DigitElement::b,
                                                                               DigitElement::c, DigitElement::d,
                                                                               DigitElement::e, DigitElement::f,
                                                                               DigitElement::g};
};

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\test.txt)";
  const auto inputData = readFromFile<std::string, std::string, std::string, std::string, std::string,
          std::string, std::string, std::string, std::string, std::string,
          char,
          std::string, std::string, std::string, std::string>(inputFile);

  for (const auto &inputLine: inputData) {
    const auto&[signal1, signal2, signal3, signal4, signal5, signal6, signal7, signal8, signal9, signal10, _, input1, input2, input3, input4] = inputLine;
    Digit::calculateMapping(
            {Digit(signal1), Digit(signal2), Digit(signal3), Digit(signal4), Digit(signal5), Digit(signal6),
             Digit(signal7), Digit(signal8), Digit(signal9), Digit(signal10), Digit(input1), Digit(input2), Digit(input3), Digit(input4)});
  }
}