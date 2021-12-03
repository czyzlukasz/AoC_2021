#pragma once

#include <string_view>
#include <fstream>
#include <iterator>
#include <vector>
#include <tuple>


template<typename... Format>
std::vector<std::tuple<Format...>> readFromFile(const std::string_view& input_file) {
  std::ifstream input_file_stream(input_file.data(), std::ios::in);
  std::vector<std::tuple<Format...>> result{};

  while(not input_file_stream.eof()) {
    result.emplace_back(std::make_tuple(*std::istream_iterator<std::decay_t<Format>>(input_file_stream)...));
  }

  return result;
}
