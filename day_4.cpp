#include <string_view>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>

template<size_t boardSize_ = 5>
struct BingoBoard {
  BingoBoard(const unsigned boardId) : boardId(boardId), boardWon(false), board{} {}
  static constexpr size_t boardSize = boardSize_;
  using BingoNumber = std::pair<unsigned, bool>;

  void markNumber(const unsigned number) {
    for (auto &column: board) {
      for (auto&[numberOnBoard, marked]: column) {
        if (numberOnBoard == number) {
          marked = true;
        }
      }
    }
  }

  bool anyColumnMarked() {
    for (const auto &column: board) {
      if (std::all_of(column.cbegin(), column.cend(), [](const BingoNumber &number) {
        return number.second;
      })) {
        boardWon = true;
        return true;
      }
    }

    return false;
  }

  bool anyRowMarked() {
    for (size_t y = 0; y < BingoBoard<>::boardSize; ++y) {
      if (std::all_of(board.cbegin(), board.cend(), [y](const auto &column) {
        return column.at(y).second;
      })) {
        boardWon = true;
        return true;
      }
    }
    return false;
  }

  unsigned sumOfUnmarked() const {
    unsigned result{};
    for (size_t y = 0; y < BingoBoard<>::boardSize; ++y) {
      for (size_t x = 0; x < BingoBoard<>::boardSize; ++x) {
        auto& [number, marked] = board.at(x).at(y);
        if(not marked) {
          result += number;
        }
      }
    }
    return result;
  }

  const unsigned boardId;
  bool boardWon;
  std::array<std::array<BingoNumber, boardSize_>, boardSize_> board;
};

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

  // Parse boards
  std::vector<BingoBoard<>> bingoBoards{};
  unsigned boardId = 1;
  while (not input_file_stream.eof()) {
    auto &currentBoard = bingoBoards.emplace_back(boardId++);
    for (size_t y = 0; y < BingoBoard<>::boardSize; ++y) {
      for (size_t x = 0; x < BingoBoard<>::boardSize; ++x) {
        input_file_stream >> currentBoard.board.at(x).at(y).first;
      }
    }
  }

  return std::make_tuple(std::move(numbers), std::move(bingoBoards));
}

int main() {
  auto[numbers, boards] = loadData(R"(C:\Code\aoc_2021\input\day_4.txt)");

  std::vector<unsigned> scores{};
  for (const auto& number: numbers) {
    for (auto& board: boards) {
      board.markNumber(number);

      if (not board.boardWon and (board.anyColumnMarked() or board.anyRowMarked())) {
        std::cout << "Board that won: " << board.boardId << ", with score: " << board.sumOfUnmarked() * number << std::endl;
      }
    }
  }
}