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

  bool tryToRemoveValidChunkFromLine(std::string& line) const {
    const std::string validChunk{static_cast<char>(begin), static_cast<char>(end)};

    if(const auto it = line.find(validChunk); it != std::string::npos) {
      line.erase(it, validChunk.size());
      return true;
    }

    return false;
  }

  const ChunkType chunkType;
  const ChunkCharacter begin;
  const ChunkCharacter end;

  static const inline std::map<ChunkCharacter, unsigned long> characterToScore{
          {ChunkCharacter::ROUND_BEGIN,      1},
          {ChunkCharacter::SQUARE_BEGIN,     2},
          {ChunkCharacter::CURLY_BEGIN,      3},
          {ChunkCharacter::TRIANGLE_BEGIN,   4},
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

  std::vector<unsigned long> score;

  std::vector<Chunk> possibleChunks{
          Chunk(ChunkType::ROUND, ChunkCharacter::ROUND_BEGIN, ChunkCharacter::ROUND_END),
          Chunk(ChunkType::SQUARE, ChunkCharacter::SQUARE_BEGIN, ChunkCharacter::SQUARE_END),
          Chunk(ChunkType::CURLY, ChunkCharacter::CURLY_BEGIN, ChunkCharacter::CURLY_END),
          Chunk(ChunkType::TRIANGLE, ChunkCharacter::TRIANGLE_BEGIN, ChunkCharacter::TRIANGLE_END)};

  for (const auto&[inputLine]: inputData) {

    std::stack<ChunkType> chunkStack;

    bool lineCorrupted = false;
    for(const char character : inputLine){
      for(const Chunk& chunk : possibleChunks) {
        if(chunk.begin == static_cast<ChunkCharacter>(character)){
          chunkStack.push(chunk.chunkType);
        }
        else if(chunk.end == static_cast<ChunkCharacter>(character)){
          if(chunkStack.top() != chunk.chunkType) {
//            std::cout << "Corrupted: " << character << std::endl;
//            score += Chunk::characterToScore.at(static_cast<ChunkCharacter>(character));
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

    if(not chunkStack.empty()) {
      std::cout << "OCHUJ";
    }

    if(not lineCorrupted) {
      std::string mutableLine = inputLine;
      bool chunksBeingRemoved = false;
      do {
        chunksBeingRemoved = false;
        for(const Chunk& chunk : possibleChunks) {
          chunksBeingRemoved |= chunk.tryToRemoveValidChunkFromLine(mutableLine);
        }
      } while (chunksBeingRemoved);
      std::cout << mutableLine << std::endl;

      unsigned long tempScore = 0;
      std::for_each(mutableLine.rbegin(), mutableLine.rend(), [&tempScore](const char endChar) {
        tempScore = tempScore * 5 + Chunk::characterToScore.at(static_cast<ChunkCharacter>(endChar));
      });

      score.push_back(tempScore);
    }
  }

  for(const unsigned long x : score) {
    std::cout << x << std::endl;
  }
  std::sort(score.begin(), score.end());

  std::cout << std::endl;
  for(const unsigned long x : score) {
    std::cout << x << std::endl;
  }
  std::cout << "Middle score: " << score.at((score.size() - 1)  / 2 ) << ". Number of invalid: " << score.size() << std::endl;
}
