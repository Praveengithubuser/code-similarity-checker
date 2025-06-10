#ifndef STRUCTURALMATCHER_H
#define STRUCTURALMATCHER_H

#include "CFGBuilder.h"
#include <vector>
#include <map>

struct MatchResult {
    double similarity;
    std::vector<std::pair<int, int>> node_matches;
    int matched_nodes;
    int total_nodes;
};

class StructuralMatcher {
public:
    // Compare two CFGs and return structural similarity
    MatchResult compare(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2);
    
private:
    // Find matching nodes between two CFGs
    std::vector<std::pair<int, int>> findNodeMatches(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2);
    
    // Calculate similarity between two basic blocks
    double calculateBlockSimilarity(const BasicBlock& block1, const BasicBlock& block2);
    
    // Check if control flow patterns match
    bool controlFlowMatches(const BasicBlock& block1, const BasicBlock& block2);
    
    // Calculate edge similarity
    double calculateEdgeSimilarity(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2, 
                                  const std::vector<std::pair<int, int>>& node_matches);
};

#endif
