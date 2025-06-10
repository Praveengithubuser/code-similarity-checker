#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

#include <vector>
#include <map>
#include <string>
#include <set>

// Simple graph structure for CFG representation
template<typename NodeType>
class Graph {
public:
    std::vector<NodeType> nodes;
    std::map<int, std::vector<int>> adjacency_list;
    
    // Add a node to the graph
    void addNode(const NodeType& node);
    
    // Add an edge between two nodes
    void addEdge(int from, int to);
    
    // Get neighbors of a node
    std::vector<int> getNeighbors(int node_id) const;
    
    // Get number of nodes
    size_t size() const;
    
    // Check if graph is empty
    bool empty() const;
    
    // Clear the graph
    void clear();
};

// Graph algorithms
class GraphAlgorithms {
public:
    // Calculate simple graph similarity (node count based)
    static double calculateSimilarity(const std::vector<int>& graph1_structure, 
                                    const std::vector<int>& graph2_structure);
    
    // Find connected components
    template<typename NodeType>
    static std::vector<std::set<int>> findConnectedComponents(const Graph<NodeType>& graph);
    
    // Calculate graph diameter
    template<typename NodeType>
    static int calculateDiameter(const Graph<NodeType>& graph);
};

#endif
