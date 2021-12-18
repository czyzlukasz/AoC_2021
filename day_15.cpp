#include "Utils.hpp"
#include <list>
#include <functional>
#include <map>
#include <memory>
#include <algorithm>
#include <iostream>

struct Node {
  using CoordinateType = std::tuple<int, int>;
  using CostType = uint64_t;
  static constexpr inline CostType infiniteCost = std::numeric_limits<CostType>::max();

  explicit Node(const CoordinateType& coordinates_) : coordinates(coordinates_), cost(infiniteCost){}

  void connectNode(auto nodeToConnect, const CostType linkCost) {
    const bool nodeIsConnected = std::any_of(connectedNodes.cbegin(), connectedNodes.cend(), [&nodeToConnect](const auto& existingNode) {
      return nodeToConnect == existingNode.first;
    });
    if (not nodeIsConnected) {
      connectedNodes.emplace_back(std::move(nodeToConnect), linkCost);
    }
  }

  const auto& getConnectedNodes() const {
    return connectedNodes;
  }

  void calculateCost(const std::function<CostType(CostType)>& costCalculator) {
    cost = costCalculator(cost);
  }

  CostType getCost() const {
    return cost;
  }

  bool hasUnvisitedNodes() const {
    return std::any_of(connectedNodes.cbegin(), connectedNodes.cend(), [](const auto& nodeCostPair) {
      const auto& [nodePtr, nodeCost] = nodeCostPair;
      return nodePtr->getCost() == infiniteCost;
    });
  }

  bool operator ==(const Node& other) const {
    return coordinates == other.coordinates;
  }

private:
  CoordinateType coordinates;

  CostType cost{infiniteCost};

  std::vector<std::pair<std::shared_ptr<Node>, CostType>> connectedNodes{};
};

struct Map2D {
  using FieldType = int;

  void addLine(const std::string& line) {
    auto& row = map.emplace_back();
    for(const char value : line) {
      row.push_back(value - '0');
    }
  }

  Node::CoordinateType getSize() const {
    return {map.front().size(), map.size()};
  }

  FieldType costAt(const size_t x, const size_t y) const {
    return map.at(y).at(x);
  }

  // Create list from 2d map and link all neighbour nodes to each other
  std::list<std::shared_ptr<Node>> mapToNodes() const {
    // Temporary container for easy lookup
    std::map<Node::CoordinateType, std::shared_ptr<Node>> nodes;
    const auto [xSize, ySize] = getSize();

    // Create nodes
    for(auto y = 0; y < ySize; ++y) {
      for(auto x = 0; x < xSize; ++x) {
        const auto newCoordinates = Node::CoordinateType{x, y};
        nodes.emplace(std::make_pair(newCoordinates, std::make_shared<Node>(newCoordinates)));
      }
    }

    // Link nodes
    for(auto y = 0; y < ySize; ++y) {
      for(auto x = 0; x < xSize; ++x) {
        const auto thisNode = nodes.at({x, y});
        const auto thisNodeCost = costAt(x, y);

        const auto connectNodes = [&nodes, thisNode, thisNodeCost, this](const auto x, const auto y){
          const auto destNode = nodes.at({x, y});
          thisNode->connectNode(destNode, costAt(x, y));
          destNode->connectNode(thisNode, thisNodeCost);
        };

        if(x > 0) {
          connectNodes(x - 1, y);
        }
        if(x + 1 < xSize) {
          connectNodes(x + 1, y);
        }
        if(y > 0) {
          connectNodes(x, y - 1);
        }
        if(y + 1 < ySize) {
          connectNodes(x, y + 1);
        }
      }
    }

    std::list<std::shared_ptr<Node>> result;
    for(const auto& [k, v] : nodes) {
      result.emplace_back(v);
    }
    return result;
  }

private:
  std::vector<std::vector<FieldType>> map;
};

int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_15.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  Map2D map2d;
  for(const auto& [line] : inputData) {
    map2d.addLine(line);
  }

  auto nodes = map2d.mapToNodes();

  const auto [sizeX, sizeY] = map2d.getSize();
  const auto destinationX = sizeX - 1;
  const auto destinationY = sizeY - 1;

  const auto markStartNode = [&nodes](const auto x, const auto y) {
    (*std::find_if(nodes.begin(), nodes.end(), [x, y](const auto node) {
      return *node == Node({x, y});
    }))->calculateCost([](auto unused){return 0;});
  };

  const auto reachedNode = [&nodes](const auto x, const auto y) -> bool{
    return (*std::find_if(nodes.begin(), nodes.end(), [x, y](const auto node) {
      return *node == Node({x, y});
    }))->getCost() != Node::infiniteCost;
  };

  const auto costOfNode = [&nodes](const auto x, const auto y) -> Node::CostType {
    return (*std::find_if(nodes.begin(), nodes.end(), [x, y](const auto node) {
      return *node == Node({x, y});
    }))->getCost();
  };
  markStartNode(0, 0);
  while(not reachedNode(destinationX, destinationY)) {
    nodes.sort([](const auto a, const auto b) {
      return a->getCost() < b->getCost();
    });
    const auto it = std::find_if(nodes.begin(), nodes.end(), [](const auto& nodePtr) {
      return nodePtr->hasUnvisitedNodes();
    });
    if(it != nodes.cend()) {
      const auto constOfCurrentNode = (*it)->getCost();
      for(const auto& [adjacentNode, cost_] : (*it)->getConnectedNodes()) {
        const auto cost = cost_;
        adjacentNode->calculateCost([cost, constOfCurrentNode](const auto costOfAdjacentNode){
          return std::min(constOfCurrentNode + cost, costOfAdjacentNode);
        });
      }
    }
  }

  std::cout << "Cost of node " << destinationX << ", " << destinationY << ": " << costOfNode(destinationX, destinationY) << std::endl;
}
