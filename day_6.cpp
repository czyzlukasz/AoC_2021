#include <string_view>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <map>

auto loadData(const std::string_view &filename) {

  std::ifstream input_file_stream(filename.data(), std::ios::in);

  // Get bingo numbers
  std::string numbersString{};
  std::getline(input_file_stream, numbersString);
  // Add comma to make parsing easier
  numbersString += ',';

  std::vector<unsigned> numbers{};

  auto begin = numbersString.cbegin();
  auto end = begin + numbersString.find(',');
  while (true) {
    const std::string_view number(begin, end);
    numbers.emplace_back(std::stoi(number.data()));

    begin = end + 1;
    const auto nextComma = numbersString.find(',', std::distance(numbersString.cbegin(), begin));
    if (nextComma == std::string::npos) {
      break;
    }
    end = numbersString.cbegin() + nextComma;
  }

  return numbers;
}

void printFishes(const std::string_view& prefix, const unsigned day, const auto& container) {
  std::cout << prefix << day << ": ";
  for(const auto& item : container) {
    std::cout << item << ",";
  }
  std::cout << ". Total number of fishes: " << container.size() << std::endl;
}

using Fish = char;
static constexpr Fish reproductionCycle = 7;
static constexpr Fish additionalTimeToFirstReproduction = 2;

void fishBehaviour(Fish& currentFish, std::vector<Fish>& fishContainer) {
  --currentFish;
  if(currentFish < 0) {
    currentFish += reproductionCycle;
    fishContainer.emplace_back(reproductionCycle + additionalTimeToFirstReproduction);
  }
}

std::map<Fish, unsigned long long> calculateLut() {
  const std::initializer_list<Fish> possibleInputs = {1, 2, 3, 4, 5};
  std::map<Fish, unsigned long long> result{};
  for(const auto input : possibleInputs) {
    static constexpr auto simulationTime = 256;

    std::vector<Fish> fishes;
    fishes.emplace_back(input);

    std::cout << "Calculating LUT for value " << static_cast<int>(input) << std::endl;
    for(auto day = 1; day <= simulationTime; ++day) {
      for(size_t idx = 0; idx < fishes.size(); ++idx)
        fishBehaviour(fishes[idx], fishes);
      std::cout << "Day " << day << std::endl;
//    printFishes("Day :", day, fishes);
    }

    result[input] = fishes.size();
  }

  return result;
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_6.txt)";
  const auto inputData = loadData(inputFile);

  const auto lut = calculateLut();
  unsigned long long result = 0;
  for(const auto fish : inputData) {
    result += lut.at(fish);
  }

  std::cout << "Total number of fishes: " << result << std::endl;
}
