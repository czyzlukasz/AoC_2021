#include "Utils.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <array>
#include <queue>


class Map2D {
  std::vector<std::vector<std::tuple<unsigned, bool, bool>>> map;
  using ValueType = decltype(map)::value_type::value_type;
public:
  Map2D(const auto& inputData) {
    for(const auto& [line] : inputData) {
      map.emplace_back();
      for(const char number : line) {
        map.back().emplace_back(number - '0', number == '9', false);
      }
    }
  }

  // Get size in [x, y]
  std::tuple<size_t, size_t> getSize() const {
    return {map.back().size(), map.size()};
  };

  const ValueType& at(size_t x, size_t y) const{
    return map.at(y).at(x);
  }

  ValueType& at(size_t x, size_t y){
    return map.at(y).at(x);
  }

  std::array<ValueType , 4> getAdjacent(size_t x, size_t y) const {
    // I know thats bad
    ValueType up = {std::numeric_limits<unsigned>::max(), true, true};
    ValueType right = {std::numeric_limits<unsigned>::max(), true, true};
    ValueType down = {std::numeric_limits<unsigned>::max(), true, true};
    ValueType left = {std::numeric_limits<unsigned>::max(), true, true};

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

};

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_9.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  Map2D map2d(inputData);
  std::vector<size_t> basinSizes;

  const auto [sizeX, sizeY] = map2d.getSize();

  for(size_t y = 0; y < sizeY; ++y) {
    for(size_t x = 0; x < sizeX; ++x) {
      const auto& [value, basinEdge, used] = map2d.at(x, y);

      if(not basinEdge and not used) {
        struct BasinField {
          size_t x, y;
          auto operator <=>(const BasinField& other) const = default;
        };

        std::vector<BasinField> fieldsVerified;
        std::queue<BasinField> fieldsToCheck;

        fieldsToCheck.push(BasinField{x, y});

        while(not fieldsToCheck.empty()) {
          const auto [xToCheck, yToCheck] = fieldsToCheck.front();
          fieldsToCheck.pop();

          const auto[up, right, down, left] = map2d.getAdjacent(xToCheck, yToCheck);
          static const auto checkIfFieldIsInBasin = [&fieldsToCheck, &fieldsVerified, &map2d](const auto &field, unsigned x,
                                                                             unsigned y) {
            const auto&[value, basinEdge, used] = field;
            if (not basinEdge and not used) {
              std::get<2>(map2d.at(x, y)) = true;
              fieldsVerified.emplace_back(BasinField{x, y});
              fieldsToCheck.push(BasinField{x, y});
            }
          };

          checkIfFieldIsInBasin(up, xToCheck, yToCheck - 1);
          checkIfFieldIsInBasin(right, xToCheck + 1, yToCheck);
          checkIfFieldIsInBasin(down, xToCheck, yToCheck + 1);
          checkIfFieldIsInBasin(left, xToCheck - 1, yToCheck);
        }

        std::cout << "Basin size: " << fieldsVerified.size() << std::endl;
        basinSizes.emplace_back(fieldsVerified.size());
      }
    }
  }
  std::sort(basinSizes.begin(), basinSizes.end());
  for(const auto basinSize : basinSizes) {
    std::cout << basinSize << ", ";
  }
  std::cout << std::endl;
}