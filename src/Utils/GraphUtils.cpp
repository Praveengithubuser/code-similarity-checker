#include "Utils/GraphUtils.h"
#include "CFGBuilder.h"  // Changed from "../CFGBuilder.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <set>

template <typename NodeType>
void Graph<NodeType>::addNode(const NodeType& node) {
    nodes.push_back(node);
}

template <typename NodeType>
void Graph<NodeType>::addEdge(int from, int to) {
    adjacency_list[from].push_back(to);
}

template <typename NodeType>
std::vector<int> Graph<NodeType>::getNeighbors(int node_id) const {
    auto it = adjacency_list.find(node_id);
    if (it != adjacency_list.end()) {
        return it->second;
    }
    return std::vector<int>();
}

template <typename NodeType>
size_t Graph<NodeType>::size() const {
    return nodes.size();
}

template <typename NodeType>
bool Graph<NodeType>::empty() const {
    return nodes.empty();
}

template <typename NodeType>
void Graph<NodeType>::clear() {
    nodes.clear();
    adjacency_list.clear();
}

double GraphAlgorithms::calculateSimilarity(const std::vector<int>& graph1_structure,
                                            const std::vector<int>& graph2_structure) {
    if (graph1_structure.empty() && graph2_structure.empty()) {
        return 1.0;
    }

    if (graph1_structure.empty() || graph2_structure.empty()) {
        return 0.0;
    }

    double size_diff = std::abs(static_cast<int>(graph1_structure.size()) -
                                static_cast<int>(graph2_structure.size()));
    double max_size = std::max(graph1_structure.size(), graph2_structure.size());

    return max_size > 0 ? 1.0 - (size_diff / max_size) : 0.0;
}

template <typename NodeType>
std::vector<std::set<int>> GraphAlgorithms::findConnectedComponents(const Graph<NodeType>& graph) {
    std::vector<std::set<int>> components;
    std::vector<bool> visited(graph.size(), false);

    for (size_t i = 0; i < graph.size(); i++) {
        if (!visited[i]) {
            std::set<int> component;
            std::queue<int> queue;
            queue.push(i);
            visited[i] = true;

            while (!queue.empty()) {
                int current = queue.front();
                queue.pop();
                component.insert(current);

                auto neighbors = graph.getNeighbors(current);
                for (int neighbor : neighbors) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        queue.push(neighbor);
                    }
                }
            }
            components.push_back(component);
        }
    }

    return components;
}

// Explicit template instantiation for BasicBlock
template class Graph<BasicBlock>;
template std::vector<std::set<int>> GraphAlgorithms::findConnectedComponents(
    const Graph<BasicBlock>& graph);
