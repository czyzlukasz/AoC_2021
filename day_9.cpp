#include "Utils.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <array>


struct Map2D {
  Map2D(const auto& inputData) {
    for(const auto& [line] : inputData) {
      map.emplace_back();
      for(const char number : line) {
        map.back().push_back(number - '0');
      }
    }
  }

  // Get size in [x, y]
  std::tuple<size_t, size_t> getSize() const {
    return {map.back().size(), map.size()};
  };

  const unsigned at(size_t x, size_t y) const{
    return map.at(y).at(x);
  }

  std::array<unsigned, 4> getAdjacent(size_t x, size_t y) const {
    // I know thats bad
    unsigned up = std::numeric_limits<unsigned>::max();
    unsigned right = std::numeric_limits<unsigned>::max();
    unsigned down = std::numeric_limits<unsigned>::max();
    unsigned left = std::numeric_limits<unsigned>::max();

    const auto [sizeX, sizeY] = getSize();
    if(sizeY > y + 1) {
      down = at(x, y + 1);
    }
    if(y > 0) {
      up = at(x, y - 1);
    }
    if(sizeX > x + 1) {
      right = at(x + 1, y);
    }
    if(x > 0) {
      left = at(x - 1, y);
    }

    return {up, right, down, left};
  }

private:
  std::vector<std::vector<unsigned>> map;
};

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_9.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  Map2D map2d(inputData);


  unsigned result = 0;

  const auto [sizeX, sizeY] = map2d.getSize();

  for(size_t y = 0; y < sizeY; ++y) {
    for(size_t x = 0; x < sizeX; ++x) {
      const unsigned value = map2d.at(x, y);
      const auto adjacent = map2d.getAdjacent(x, y);
      if(std::all_of(adjacent.begin(), adjacent.end(), [value](unsigned adjacentField){
        return value < adjacentField;
      })){
        result += value + 1;
      }
    }
  }

  std::cout << "Result is: " << result << std::endl;
}