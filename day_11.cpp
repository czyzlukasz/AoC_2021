#include "Utils.hpp"
#include <map>
#include <iostream>


struct Octopus {
  Octopus(int energyLevel) : energyLevel(energyLevel), flashedInThisIteration(false){}

  bool aboutToFlash() const {
    return energyLevel > 9 && not flashedInThisIteration;
  }

  bool resetEnergyIfFlashed() {
    if(flashedInThisIteration) {
      energyLevel = 0;
      flashedInThisIteration = false;
      return true;
    }
    flashedInThisIteration = false;
    return false;
  }

  void flash() {
    flashedInThisIteration = true;
  }

  void increaseEnergy() {
    if(not flashedInThisIteration) {
      ++energyLevel;
    }
  }

  const auto& getEnergy() const {
    return energyLevel;
  }

private:
  int energyLevel;
  bool flashedInThisIteration;
};

struct Map {
  using Point = std::pair<size_t, size_t>;

  Map(const std::vector<std::tuple<std::string>>& input) {
    size_t y = 0;
    for(const auto& [line] : input) {
      size_t x = 0;
      for(const char value : line) {
        map[y].emplace(x,Octopus{value - '0'});
        ++x;
      }
      ++y;
    }
  }

  unsigned long iterate() {
    const size_t sizeX = map.size();
    const size_t sizeY = map.cbegin()->second.size();


    // Increment energy of all octopuses
    for (size_t x = 0; x < sizeX; ++x) {
      for (size_t y = 0; y < sizeY; ++y) {
        at(x, y).increaseEnergy();
      }
    }

    // Iterate over all octopuses and flash them as soon as possible
    for (size_t x = 0; x < sizeX; ++x) {
      for (size_t y = 0; y < sizeY; ++y) {
        if(at(x, y).aboutToFlash()) {
          at(x, y).flash();
          flashAdjacentPoints(x, y);
        }
      }
    }

    // Calculate total number of flashes in current iteration
    unsigned long result = 0;

    for (size_t x = 0; x < sizeX; ++x) {
      for (size_t y = 0; y < sizeY; ++y) {
        result += at(x, y).resetEnergyIfFlashed();
      }
    }

    return result;
  }

  // Recursively flash all adjacent points (if not flashed previously)
  void flashAdjacentPoints(const size_t x, const size_t y) {
    for(const auto [adjacentX, adjacentY] : generateAdjacentPoints(x, y)) {
      at(adjacentX, adjacentY).increaseEnergy();
      if(at(adjacentX, adjacentY).aboutToFlash()) {
        at(adjacentX, adjacentY).flash();
        flashAdjacentPoints(adjacentX, adjacentY);
      }
    }
  }

  void print() {
    const size_t sizeX = map.size();
    const size_t sizeY = map.cbegin()->second.size();
    for (size_t y = 0; y < sizeY; ++y) {
      for (size_t x = 0; x < sizeX; ++x) {
        const auto energy = at(x, y).getEnergy();
        // Substitute zero with more readable character
        // I miss you Elvis operator :(
        std::cout << char(energy ? '0' + energy : '*');
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  const Octopus& at(const size_t x, const size_t y) const {
    return map.at(y).at(x);
  }

  Octopus& at(const size_t x, const size_t y) {
    return map.at(y).at(x);
  }

  bool allOctopusesFlashed() const{
    const size_t sizeX = map.size();
    const size_t sizeY = map.cbegin()->second.size();
    for (size_t x = 0; x < sizeX; ++x) {
      for (size_t y = 0; y < sizeY; ++y) {
        if(at(x,y).getEnergy() != 0) {
          return false;
        }
      }
    }
    return true;
  }

  std::vector<Point> generateAdjacentPoints(const size_t x, const size_t y) {
    const size_t sizeX = map.size();
    const size_t sizeY = map.cbegin()->second.size();

    std::vector<Point> result;

    https://google.com
    // Check bounds
    // Top-left
    if (x > 0 and y > 0) result.emplace_back(x - 1, y - 1);
    // Top
    if (y > 0) result.emplace_back(x, y - 1);
    // Top-right
    if (x + 1 < sizeX and y > 0) result.emplace_back(x + 1, y - 1);
    // Left
    if (x > 0) result.emplace_back(x - 1, y);
    // Right
    if (x + 1 < sizeX) result.emplace_back(x + 1, y);
    // Bottom-left
    if (x > 0 and y + 1 < sizeY) result.emplace_back(x - 1, y + 1);
    // Bottom
    if (y + 1 < sizeY) result.emplace_back(x, y + 1);
    // Bottom-left
    if (x + 1 < sizeX and y + 1 < sizeY) result.emplace_back(x + 1, y + 1);

    return result;
  }

private:
  std::map<size_t, std::map<size_t, Octopus>> map;
};

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_11.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);


  Map map(inputData);
  map.print();

  unsigned long result = 0;

  for(size_t iteration = 1; iteration <= 500; ++iteration) {
    std::cout << "After step " << iteration << std::endl;
    result += map.iterate();
    map.print();
    if(map.allOctopusesFlashed()) {
      break;
    }
  }

  std::cout << "Total number of flashes: " << result << std::endl;
}
