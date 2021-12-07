#include <iostream>
#include <type_traits>
#include <iterator>
#include "Utils.hpp"

struct VentLine {
  int beginX, beginY;
  int endX, endY;

  struct Iterator : std::input_iterator_tag {
    Iterator() : current(), endX(), endY() {}

    Iterator(const VentLine &ventLine) : current(ventLine.beginX, ventLine.beginY), endX(ventLine.endX),
                                         endY(ventLine.endY) {
      auto&[currentX, currentY] = current;
      stepX = currentX != endX ? (currentX < endX ? 1 : -1) : 0;
      stepY = currentY != endY ? (currentY < endY ? 1 : -1) : 0;
    }

    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::tuple<int, int>;
    using pointer = value_type *;
    using reference = value_type &;

    const value_type &operator*() const {
      return current;
    }

    pointer operator->() {
      return &current;
    }

    // Pre
    Iterator &operator++() {
      auto&[currentX, currentY] = current;
      currentX += stepX;
      currentY += stepY;
      return *this;
    }

    // Post
    void operator++(int) {
      while (true) {}
    }

    bool operator!=(const Iterator &other) const {
      auto&[currentX, currentY] = current;
      if (stepX != 0) {
        return currentX - stepX != endX;
      } else if (stepY != 0) {
        return currentY - stepY != endY;
      }

      return false;
    }

  private:
    value_type current;
    int stepX, stepY;
    int endX, endY;
  };

  Iterator cbegin() const {
    return Iterator{*this};
  }

  Iterator cend() const {
    return Iterator();
  }

  Iterator begin() const {
    return Iterator{*this};
  }

  Iterator end() const {
    return Iterator();
  }

  static_assert(std::input_iterator<Iterator>);
};

struct Map {
  using Point = int;

  Point &getPoint(const size_t x, const size_t y) {
    if (map.size() <= x) {
      map.resize(x + 1);
    }
    if (map.at(x).size() <= y) {
      map.at(x).resize(y + 1);
    }

    return map.at(x).at(y);
  }

  int getNumOfIntersections() const {
    int result = 0;
    for (const auto &column: map) {
      for (const auto &point: column) {
        result += point > 1;
      }
    }
    return result;
  }

  std::vector<std::vector<Point>> map;
};

auto inputDataToVentLines(const auto &inputData) {
  std::vector<VentLine> ventLines{};
  for (const auto&[beginX, comma1, beginY, arrow, endX, comma2, endY]: inputData) {
    if (not(beginX == endX || beginY == endY)) {
      static constexpr bool acceptDiagonals = true;
      if constexpr(not acceptDiagonals) {
        continue;
      }
    }
    ventLines.emplace_back(VentLine{.beginX = beginX, .beginY = beginY, .endX = endX, .endY = endY});
  }

  return ventLines;
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_5.txt)";
  const auto inputData = readFromFile<int, char, int, std::string, int, char, int>(inputFile);

  const auto ventLines = inputDataToVentLines(inputData);

  Map map{};
  for (const auto &ventLine: ventLines) {
    for (const auto&[x, y]: ventLine) {
//      std::cout << x << " " << y << std::endl;
      ++map.getPoint(x, y);
    }
  }

  std::cout << "Num of intersections: " << map.getNumOfIntersections() << std::endl;
}
