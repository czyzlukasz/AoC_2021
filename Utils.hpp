#pragma once

#include <string_view>
#include <fstream>
#include <iterator>
#include <vector>
#include <tuple>
#include <string>


template<typename... Format>
std::vector<std::tuple<Format...>> readFromFile(const std::string_view& input_file) {
  std::ifstream inputFileStream(input_file.data(), std::ios::in);
  std::vector<std::tuple<Format...>> result{};

  while(not inputFileStream.eof()) {
    result.emplace_back(std::tuple<Format...>{*std::istream_iterator<std::decay_t<Format>>(inputFileStream)...});
  }

  return result;
}

template<typename T>
auto readCommaSeparatedValues(const std::string_view &filename) {

  std::ifstream input_file_stream(filename.data(), std::ios::in);

  std::string numbersString{};
  std::getline(input_file_stream, numbersString);
  // Add comma to make parsing easier
  numbersString += ',';

  std::vector<T> numbers{};

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
