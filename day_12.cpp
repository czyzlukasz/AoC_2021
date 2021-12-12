#include "Utils.hpp"
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>


struct NodeName {
  using NodeIdType = size_t;

  explicit NodeName(const std::string_view& nodeName) : nodeId(idToNameMap.size()){
    idToNameMap.emplace_back(std::make_pair(idToNameMap.size(), nodeName));
  }

  bool isBigCave() const {
    const std::string name = getNameByNodeId(nodeId);
    return std::all_of(name.cbegin(), name.cend(), std::isupper);
  }

  static bool isBigCave(const NodeIdType& nodeId){
    const std::string name = getNameByNodeId(nodeId);
    return std::all_of(name.cbegin(), name.cend(), std::isupper);
  }

  bool operator == (const NodeName& other) const {
    return nodeId == other.nodeId;
  }

  bool operator == (const std::string_view& other) const {
    return nodeId == getNodeIdByName(other);
  }

  static NodeIdType getNodeIdByName(const std::string_view& nameToFind){
    return std::find_if(idToNameMap.cbegin(), idToNameMap.cend(), [nameToFind](const auto& idNamePair){
      const auto& [id, name] = idNamePair;
      return name == nameToFind;
    })->first;
  }

  static std::string getNameByNodeId(const NodeIdType& idToFind){
    return std::find_if(idToNameMap.cbegin(), idToNameMap.cend(), [idToFind](const auto& idNamePair){
      const auto& [id, name] = idNamePair;
      return id == idToFind;
    })->second;
  }

  const unsigned nodeId;

private:
  // Program-wide storage mapping ID and names. Useful for reducing overhead of string comparison - all algorithms
  // operate on integers as identifiers
  static inline std::vector<std::pair<NodeIdType, std::string>> idToNameMap;
};


struct Node {
  Node(const std::string_view& nodeName_) : nodeName(nodeName_), isBig(nodeName.isBigCave()) {}

  bool operator == (const Node& other) const {
    return nodeName == other.nodeName;
  }

  bool operator == (const std::string_view& other) const {
    return nodeName == other;
  }

  void connectNode(std::shared_ptr<Node> connectedNode) {
    connectedNodes.emplace_back(std::move(connectedNode));
  }

  const auto& getConnectedNodes() const {
    return connectedNodes;
  }

  const NodeName nodeName;
  const bool isBig;

private:
  std::vector<std::shared_ptr<Node>> connectedNodes;
};


struct Path {
  struct NodeEntry {
    NodeName::NodeIdType nodeVisited;
    bool canBeVisited;
  };

  // Check if we can enter specified node based on path history
  enum class VisitationStatus {
    YES,
    NO,
    YES_BUT_THATS_A_SMALL_CAVE
  };

  VisitationStatus canBeVisited(const NodeName::NodeIdType& nodeId) const {
    if(const auto it = std::find_if(pathHistory.cbegin(), pathHistory.cend(), [&nodeId, this](const auto& nodeEntry) {
      return nodeEntry.nodeVisited == nodeId;
    }); it != pathHistory.cend()) {
      if(it->canBeVisited) {
        return VisitationStatus::YES;
      }
      const auto nodeName = NodeName::getNameByNodeId(nodeId);
      // Magic numbers, but whatever
      if(nodeName != "start" and nodeName != "end" and smallCaveVisited == false) {
        return VisitationStatus::YES_BUT_THATS_A_SMALL_CAVE;
      }
      return VisitationStatus::NO;
    }
    return VisitationStatus::YES;
  }

  void addNodeToPathHistory(const NodeName::NodeIdType& nodeId) {
    pathHistory.emplace_back(NodeEntry{nodeId, NodeName::isBigCave(nodeId)});
  }

  // Pretty print as in example
  void printPath() const {
    for(auto it = pathHistory.cbegin(); it != pathHistory.cend(); ++it) {
      std::cout << NodeName::getNameByNodeId(it->nodeVisited);
      if(std::distance(it, pathHistory.cend()) > 1) {
        std::cout << ",";
      }
    }
  }

  void markPathVisitedSmallCaveTwice() {
    smallCaveVisited = true;
  }

  std::vector<NodeEntry> pathHistory;
  mutable bool smallCaveVisited{false};
};


struct NodeStorage {

  void addNode(const std::string_view& nodeName) {
    nodes.emplace_back(std::make_shared<Node>(nodeName));
  }

  // Connect two nodes with two-way link (A to B and B to A)
  void connectNodes(const std::string_view& a, const std::string_view& b) const {
    const auto nodeFinder = [](const std::string_view& nodeToFind, const std::shared_ptr<Node>& nodePtr) -> bool{
      return *nodePtr == nodeToFind;
    };

    const std::shared_ptr<Node> nodeA = *std::find_if(nodes.cbegin(), nodes.cend(), std::bind(nodeFinder, a, std::placeholders::_1));
    const std::shared_ptr<Node> nodeB = *std::find_if(nodes.cbegin(), nodes.cend(), std::bind(nodeFinder, b, std::placeholders::_1));

    nodeA->connectNode(nodeB);
    nodeB->connectNode(nodeA);
  }

  // Find node in storage
  std::shared_ptr<Node> findNode(const std::string_view& node) const {
    return *std::find_if(nodes.cbegin(), nodes.cend(), [node](const std::shared_ptr<Node>& nodePtr){
      return *nodePtr == node;
    });
  }

  // Recursive function finding all valid paths. This method works as follows:
  // - Find all connected nodes to a current one
  // - Iterate over all adjacent nodes:
  //   - If connected node is a final one append that path to vector of valid ones and skip to next adjacent node
  //   - Check if node can be visited. If it can:
  //     - Add this node to current path history
  //     - Recursively call this algorithm with a copy of current path history
  //     - Concatenate output with vector of valid paths
  std::vector<Path> walkOverPathToNode(const Path& path, const std::shared_ptr<Node>& currentNode, const std::shared_ptr<Node>& finishNode) const {
    std::vector<Path> result;

    for(const auto& visitedNode : currentNode->getConnectedNodes()) {
      if(visitedNode == finishNode) {
        result.emplace_back(path);
        // Add finish node to path
        result.back().addNodeToPathHistory(finishNode->nodeName.nodeId);
        continue;
      }
      const auto visitedNodeId = visitedNode->nodeName.nodeId;
      if(const auto visitationStatus = path.canBeVisited(visitedNodeId); visitationStatus != Path::VisitationStatus::NO) {
        Path newPath = path;
        newPath.addNodeToPathHistory(visitedNodeId);
        if(visitationStatus == Path::VisitationStatus::YES_BUT_THATS_A_SMALL_CAVE) {
          newPath.markPathVisitedSmallCaveTwice();
        }
        const auto finishedPath = walkOverPathToNode(newPath, visitedNode, finishNode);
        if(not finishedPath.empty()) {
          result.insert(result.end(), finishedPath.cbegin(), finishedPath.cend());
        }
      }
    }

    return result;
  }

  auto findPath(const std::string_view& start, const std::string_view& finish) const {
    const auto startNode = findNode(start);
    const auto endNode = findNode(finish);

    Path startPath;
    // Initialize path with starting node
    startPath.addNodeToPathHistory(startNode->nodeName.nodeId);
    return walkOverPathToNode(startPath, startNode, endNode);
  }

private:
  std::vector<std::shared_ptr<Node>> nodes;
};


int main() {
  static constexpr auto inputFile = R"(C:\Code\aoc_2021\input\day_12.txt)";
  const auto inputData = readFromFile<std::string>(inputFile);

  NodeStorage nodeStorage;

  for(const auto& [line] : inputData) {
    const auto delimiter = line.find('-');
    const std::string firstNode = line.substr(0, delimiter);
    const std::string secondNode = line.substr(delimiter + 1);

    nodeStorage.addNode(firstNode);
    nodeStorage.addNode(secondNode);
    nodeStorage.connectNodes(firstNode, secondNode);
  }

  const auto paths = nodeStorage.findPath("start", "end");
  std::cout << "Found " << paths.size() << " possible paths:" << std::endl;
//  for(const auto& path : paths) {
//    path.printPath();
//    std::cout << std::endl;
//  }
}
