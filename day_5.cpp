#include <iostream>
#include <type_traits>
#include <iterator>
#include "Utils.hpp"

struct VentLine {
  using PointValue = bool;
  int beginX, beginY;
  int endX, endY;

  struct Iterator : std::input_iterator_tag {

    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = PointValue;
    using pointer = value_type *;
    using reference = value_type &;

    reference operator*() const {
      static bool x = 2;
      return x;
    }

    pointer operator->() {
      static bool x = 2;
      return &x;
    }

    Iterator &operator++() {
      return *this;
    }

    void operator++(int) {
    }

  private:
  };

  static_assert(std::input_iterator<Iterator>);
};

struct Map {
  using Point = int;

  struct Iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Point;
    using pointer = value_type *;
    using reference = value_type &;
  };

  Point &getPoint(const size_t x, const size_t y) {
    if (map.size() <= x) {
      map.resize(x + 1);
    }
    if (map.at(x).size() <= y) {
      map.at(x).resize(y + 1);
    }

    return map.at(x).at(y);
  }

  std::vector<std::vector<Point>> map;
};

auto inputDataToVentLines(const auto &inputData) {
  std::vector<VentLine> ventLines{};
  for (const auto&[beginX, comma1, beginY, arrow, endX, comma2, endY]: inputData) {
    ventLines.emplace_back(VentLine{.beginX = beginX, .beginY = beginY, .endX = endX, .endY = endY});
  }

  return ventLines;
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_5.txt)";
  const auto inputData = readFromFile<int, char, int, std::string, int, char, int>(inputFile);

  const auto ventLines = inputDataToVentLines(inputData);
}
