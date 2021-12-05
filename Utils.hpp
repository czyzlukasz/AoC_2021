#pragma once

#include <string_view>
#include <fstream>
#include <iterator>
#include <vector>
#include <tuple>


template<typename... Format>
std::vector<std::tuple<Format...>> readFromFile(const std::string_view& input_file) {
  std::ifstream inputFileStream(input_file.data(), std::ios::in);
  std::vector<std::tuple<Format...>> result{};

  while(not inputFileStream.eof()) {
    result.emplace_back(std::tuple<Format...>{*std::istream_iterator<std::decay_t<Format>>(inputFileStream)...});
  }

  return result;
}
