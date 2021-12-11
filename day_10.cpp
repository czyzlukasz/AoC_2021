#include "Utils.hpp"
#include <map>
#include <iostream>
#include <algorithm>
#include <stack>

enum class ChunkCharacter : char {
  TRIANGLE_BEGIN = '<',
  TRIANGLE_END = '>',
  SQUARE_BEGIN = '[',
  SQUARE_END = ']',
  CURLY_BEGIN = '{',
  CURLY_END = '}',
  ROUND_BEGIN = '(',
  ROUND_END = ')',
};

enum class ChunkType {
  TRIANGLE,
  SQUARE,
  CURLY,
  ROUND
};

struct Chunk {
  explicit Chunk(const ChunkType chunkType, ChunkCharacter begin, ChunkCharacter end)
          : chunkType(chunkType), begin(begin), end(end) {};

  const ChunkType chunkType;
  const ChunkCharacter begin;
  const ChunkCharacter end;

  static const inline std::map<ChunkCharacter, unsigned> characterToScore{
          {ChunkCharacter::TRIANGLE_BEGIN, 25137},
          {ChunkCharacter::TRIANGLE_END,   25137},
          {ChunkCharacter::SQUARE_BEGIN,   57},
          {ChunkCharacter::SQUARE_END,     57},
          {ChunkCharacter::CURLY_BEGIN,    1197},
          {ChunkCharacter::CURLY_END,      1197},
          {ChunkCharacter::ROUND_BEGIN,    3},
          {ChunkCharacter::ROUND_END,      3}
  };
};

std::vector<ChunkCharacter> parseLine(const std::string_view &line) {
  std::vector<ChunkCharacter> result;

  for (const char character: line) {
    result.emplace_back(static_cast<ChunkCharacter>(character));
  }

  return result;
}

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_10.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  unsigned score = 0;

  for (const auto&[inputLine]: inputData) {
    std::vector<Chunk> possibleChunks{
      Chunk(ChunkType::ROUND, ChunkCharacter::ROUND_BEGIN, ChunkCharacter::ROUND_END),
      Chunk(ChunkType::SQUARE, ChunkCharacter::SQUARE_BEGIN, ChunkCharacter::SQUARE_END),
      Chunk(ChunkType::CURLY, ChunkCharacter::CURLY_BEGIN, ChunkCharacter::CURLY_END),
      Chunk(ChunkType::TRIANGLE, ChunkCharacter::TRIANGLE_BEGIN, ChunkCharacter::TRIANGLE_END)};

    std::stack<ChunkType> chunkStack;

    bool lineCorrupted = false;
    for(const char character : inputLine){
      for(const Chunk& chunk : possibleChunks) {
        if(chunk.begin == static_cast<ChunkCharacter>(character)){
          chunkStack.push(chunk.chunkType);
        }
        else if(chunk.end == static_cast<ChunkCharacter>(character)){
          if(chunkStack.top() != chunk.chunkType) {
            std::cout << "Corrupted: " << character << std::endl;
            score += Chunk::characterToScore.at(static_cast<ChunkCharacter>(character));
            lineCorrupted = true;
            break;
          }
          else {
            chunkStack.pop();
          }
        }
      }
      if(lineCorrupted) {
        break;
      }
    }
  }

  std::cout << "Total score: " << score << std::endl;
}
