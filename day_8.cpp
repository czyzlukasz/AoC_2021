#include <iostream>
#include <set>
#include <map>
#include "Utils.hpp"

struct Digit {
  enum class DigitElement : char{
    a = 'a',
    b = 'b',
    c = 'c',
    d = 'd',
    e = 'e',
    f = 'f',
    g = 'g'
  };

  Digit(const std::string_view& stringRepr) : stringRepr(stringRepr) {
    for(const char element : stringRepr) {
      elements.emplace_back(static_cast<DigitElement>(element));
    }
  };

  bool is1or4or7or8() const {
    return stringRepr.size() == 2 ||  // 1
           stringRepr.size() == 4 ||  // 4
           stringRepr.size() == 3 ||  // 7
           stringRepr.size() == 7;    // 8
  }

  static std::map<DigitElement, DigitElement> calculateMapping(const std::initializer_list<Digit>& signals) {
    auto possiblePermutations = generateEveryMappingPermutation<DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::e, DigitElement::f, DigitElement::g>();
    return {};
  }

private:
  // Generate every possible permutation of elements. Each key from parameter elements will be mapped to whole set of parameter elements
  template<DigitElement... elements>
  static std::multimap<DigitElement, DigitElement> generateEveryMappingPermutation() {
    static_assert(sizeof...(elements), "Set of elements cannot be empty");
    static constexpr auto allDigitElements = {elements...};
    std::multimap<DigitElement, DigitElement> result;

    static constexpr auto generatePermutation = [] (const DigitElement digitElement) constexpr -> std::initializer_list<decltype(result)::value_type> {
      return {{digitElement, elements}...};
    };

    for(const auto digitElement : allDigitElements) {
      result.insert(generatePermutation(digitElement));
    }

    return result;
  }

  std::multimap<DigitElement, DigitElement> determine1and4and7and8() {
    return {};
  }

  const std::string_view& stringRepr;
  std::vector<DigitElement> elements;

  // Definition of 7-segment digits. Each collection of digits elements[a-g] represents one unique digit[0-9]
  static const inline std::map<std::vector<DigitElement>, unsigned int> digits = {
          {{DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::e, DigitElement::f, DigitElement::g}, 0},
          {{DigitElement::c, DigitElement::f}, 1},
          {{DigitElement::a, DigitElement::c, DigitElement::d, DigitElement::e, DigitElement::g}, 2},
          {{DigitElement::a, DigitElement::c, DigitElement::d, DigitElement::f, DigitElement::g}, 3},
          {{DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::f}, 4},
          {{DigitElement::a, DigitElement::b, DigitElement::d, DigitElement::f, DigitElement::g}, 5},
          {{DigitElement::a, DigitElement::b, DigitElement::d, DigitElement::e, DigitElement::f, DigitElement::g}, 6},
          {{DigitElement::a, DigitElement::c, DigitElement::f}, 7},
          {{DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::e, DigitElement::f, DigitElement::g}, 8},
          {{DigitElement::a, DigitElement::b, DigitElement::c, DigitElement::d, DigitElement::f, DigitElement::g}, 9},
  };
};

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_8.txt)";
  const auto inputData = readFromFile<std::string, std::string, std::string, std::string, std::string,
                                      std::string, std::string, std::string, std::string, std::string,
                                      char,
                                      std::string, std::string, std::string, std::string>(inputFile);

  unsigned numOf1_4_7_8 = 0;
  for(const auto& inputLine : inputData) {
    const auto& [signal1, signal2, signal3, signal4, signal5, signal6, signal7, signal8, signal9, signal10, _, input1, input2, input3, input4] = inputLine;
    Digit::calculateMapping({});
  }

  std::cout << "1, 4, 7, 8 occurs " << numOf1_4_7_8 << " times" << std::endl;
}