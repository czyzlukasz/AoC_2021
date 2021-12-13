#include "Utils.hpp"
#include <map>
#include <iterator>
#include <optional>
#include <iostream>
#include <functional>
#include <algorithm>

struct Dot {
  static constexpr char marked = '#';
  static constexpr char unmarked = '.';

  Dot() : dot(unmarked){}
  Dot(char character) : dot(character){}

  Dot& operator=(const Dot& other) {
    dot = other.dot == marked ? marked : dot;
    return *this;
  }

  char dot;
};

struct Map2D {
  struct Iterator : std::input_iterator_tag {
    // Direction of iteration
    enum class Direction{
      UP, // -Y
      RIGHT,  // +X
      DOWN, // +Y
      LEFT  // -X
    };

    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Dot;
    using pointer = value_type *;
    using reference = value_type &;

//    Iterator() : direction(Direction::UP), current(0, 0), map(nullptr) {}
    Iterator(Direction direction, const std::tuple<int, int>& begin, Map2D* const map) : direction(direction), current(begin), map(map){}
    Iterator(const std::tuple<int, int>& begin) : direction(Direction::UP), current(begin), map(nullptr){}

    const value_type &operator*() const {
      const auto& [x, y] = current;
      return map->at(x, y);
    }

    value_type &operator*() {
      const auto& [x, y] = current;
      return map->at(x, y);
    }

    pointer operator->() {
      const auto& [x, y] = current;
      return &map->at(x, y);
    }

    // Pre
    Iterator &operator++() {
      auto& [x, y] = current;
      switch (direction) {
        case Direction::UP:
          --y;
          break;
        case Direction::RIGHT:
          ++x;
          break;
        case Direction::DOWN:
          ++y;
          break;
        case Direction::LEFT:
          --x;
          break;
      }
      return *this;
    }

    // Post
    void operator++(int) {
      while (true) {}
    }

    bool operator !=(const Iterator& other) const{
      auto [nextX, nextY] = current;
//      switch (direction) {
//        case Direction::UP:
//          --nextY;
//          break;
//        case Direction::RIGHT:
//          ++nextX;
//          break;
//        case Direction::DOWN:
//          ++nextY;
//          break;
//        case Direction::LEFT:
//          --nextX;
//          break;
//      }

      const auto& [otherX, otherY] = other.current;
      return nextX != otherX or nextY != otherY;
    }

  private:
    Direction direction;
    std::tuple<int, int> current;
    // Old school
    Map2D* map;
  };

  Dot& at(const int x, const int y) {
    auto& [xSizePlus, ySizePlus] = mapSizePlus;
    auto& [xSizeMinus, ySizeMinus] = mapSizeMinus;
    xSizePlus = std::max(xSizePlus, x);
    ySizePlus = std::max(ySizePlus, y);
    xSizeMinus = std::min(xSizeMinus, x);
    ySizeMinus = std::min(ySizeMinus, y);
    return map[y][x];
  }

//  const int& at(const int x, const int y) const {
//    return map.at(y).at(x);
//  }

  void fillMissingFields() {
    auto& [xSizePlus, ySizePlus] = mapSizePlus;
    auto& [xSizeMinus, ySizeMinus] = mapSizeMinus;
    for(int y = ySizeMinus; y <= ySizePlus; ++y) {
      auto& row = map[y];
      for(int x = xSizeMinus; x <= xSizePlus; ++x) {
        if(not row.contains(x)) {
          row[x] = Dot(0);
        }
      }
    }
  }

  void print() const {
    for(const auto& row : map) {
      for(const auto& character : row.second) {
        std::cout << character.second.dot;
      }
      std::cout << std::endl;
    }
  }

  int getEndOfMap(Iterator::Direction direction) const {
    const auto& [xSizePlus, ySizePlus] = mapSizePlus;
    const auto& [xSizeMinus, ySizeMinus] = mapSizeMinus;
    switch (direction) {
      case Iterator::Direction::UP:
        return ySizeMinus - 1;
      case Iterator::Direction::RIGHT:
        return xSizePlus + 1;
      case Iterator::Direction::DOWN:
        return ySizePlus + 1;
      case Iterator::Direction::LEFT:
        return xSizeMinus - 1;
    }

    throw std::exception{};
  }

  int countDots() const {
    int result{};
    for (const auto &row: map) {
      for (const auto &character: row.second) {
        result += character.second.dot == Dot::marked;
      }
    }

    return result;
  }

  // Remove margins where there is no dot (crop to size)
  void groomMap() {
    int lowestX{}, lowestY{}, highestX{}, highestY{};
    for (const auto &[y, row] : map) {
      for (const auto &[x, character] : row) {
        if(character.dot == Dot::marked) {
          lowestX = std::min(lowestX, x);
          lowestY = std::min(lowestY, y);
          highestX = std::max(highestX, x);
          highestY = std::max(highestY, y);
        }
      }
    }

    decltype(map) newMap;
    for(int y = lowestY; y <= highestY; ++y) {
      for(int x = lowestX; x <= highestX; ++x) {
        newMap[y][x] = map[y][x];
      }
    }

    map = newMap;
  }

private:
  std::map<int, std::map<int, Dot>> map;
  std::tuple<int, int> mapSizePlus;
  std::tuple<int, int> mapSizeMinus;
};

struct Fold {
  std::optional<int> alongX;
  std::optional<int> alongY;
};

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\test.txt)";
  const auto inputData = readLineByLine(inputFile);

  Map2D map2d;
  std::vector<Fold> folds;
  // Parse data into map and folds
  for(const auto& line : inputData) {
    if(line.empty()) [[unlikely]]{
      continue;
    }
    else if(line.find("fold along") != std::string::npos) {
      const auto equalSignPos = line.find('=');
      char direction = line.at(equalSignPos - 1);
      const auto position = std::atoi(line.substr(equalSignPos + 1).c_str());

      folds.emplace_back(Fold{.alongX = direction == 'x' ? std::optional<int>{position} : std::nullopt, .alongY = direction == 'y' ? std::optional<int>{position} : std::nullopt});
    }
    else {
      const auto comma = line.find(',');
      const std::string xPos = line.substr(0, comma);
      const std::string yPos = line.substr(comma + 1);
      map2d.at(std::atoi(xPos.c_str()), std::atoi(yPos.c_str())) = Dot{'#'};
    }
  }

  map2d.fillMissingFields();
//  std::cout << "Initial map looks like this:" << std::endl;
//  map2d.print();
  std::cout << std::endl;

  for(const auto& fold : folds) {
    Map2D newMap;
    struct FoldData {
      // Direction of iterator of source map (read)
      Map2D::Iterator::Direction readDirection;
      // Direction of iterator of destination map (write)
      Map2D::Iterator::Direction writeDirection;
      // Begin of iteration for copying - std::copy will be invoked iteratively from this point
      Map2D::Iterator::Direction iterationBegin;
      // End of iteration for copying - std::copy will be invoked iteratively to this point
      Map2D::Iterator::Direction iterationEnd;

      int readWritePosition;
      std::function<std::tuple<int, int>(int, int)> coordinateMaker;
    };

    static const auto generateFoldData = [](const auto& fold) -> FoldData{
      static const auto xCoordinateMaker = [](int a, int b){return std::make_tuple(b, a);};
      static const auto yCoordinateMaker = [](int a, int b){return std::make_tuple(a, b);};
      if(fold.alongX.has_value()) {
        return {Map2D::Iterator::Direction::RIGHT, Map2D::Iterator::Direction::LEFT, Map2D::Iterator::Direction::UP, Map2D::Iterator::Direction::DOWN, fold.alongX.value(), xCoordinateMaker};
      }
      else if(fold.alongY.has_value()) {
        return {Map2D::Iterator::Direction::DOWN, Map2D::Iterator::Direction::UP, Map2D::Iterator::Direction::LEFT, Map2D::Iterator::Direction::RIGHT, fold.alongY.value(), yCoordinateMaker};
      }
      else {
        throw std::exception{};
      }
    };

    const auto foldData = generateFoldData(fold);

    for(auto idx = map2d.getEndOfMap(foldData.iterationBegin) + 1; idx < map2d.getEndOfMap(foldData.iterationEnd); ++idx) {
      // Copy 'flipped' side
      const auto startFlippedPos = foldData.coordinateMaker(idx, foldData.readWritePosition);
      const auto endFlippedPos = foldData.coordinateMaker(idx, map2d.getEndOfMap(foldData.readDirection));
      const auto flippedDestinationIt = Map2D::Iterator(foldData.writeDirection, startFlippedPos, &newMap);
      std::copy(Map2D::Iterator(foldData.readDirection, startFlippedPos, &map2d), Map2D::Iterator(foldData.readDirection, endFlippedPos, &map2d), flippedDestinationIt);

      // Copy 'unflipped side
      const auto startUnFlippedPos = foldData.coordinateMaker(idx, map2d.getEndOfMap(foldData.writeDirection));
      const auto endUnFlippedPos = foldData.coordinateMaker(idx, foldData.readWritePosition);
      const auto unFlippedDestinationIt = Map2D::Iterator(foldData.readDirection, startUnFlippedPos, &newMap);
      std::copy(Map2D::Iterator(foldData.readDirection, startUnFlippedPos, &map2d), Map2D::Iterator(foldData.readDirection, endUnFlippedPos, &map2d), unFlippedDestinationIt);
    }

    newMap.fillMissingFields();
    std::cout << "After fold I see " << newMap.countDots() << " dots here:" << std::endl;
//    newMap.print();
    std::cout << std::endl;
    map2d = newMap;
    // For now, break here
    break;
  }
  map2d.groomMap();
  map2d.print();
}
