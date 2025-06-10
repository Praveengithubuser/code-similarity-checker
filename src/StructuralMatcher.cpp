#include "StructuralMatcher.h"
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <map>

MatchResult StructuralMatcher::compare(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2) {
    MatchResult result;
    result.similarity = 0.0;
    result.matched_nodes = 0;
    result.total_nodes = std::max(cfg1.blocks.size(), cfg2.blocks.size());
    
    if (cfg1.blocks.empty() && cfg2.blocks.empty()) {
        result.similarity = 1.0;
        return result;
    }
    
    if (cfg1.blocks.empty() || cfg2.blocks.empty()) {
        result.similarity = 0.0;
        return result;
    }
    
    // Find node matches
    result.node_matches = findNodeMatches(cfg1, cfg2);
    result.matched_nodes = result.node_matches.size();
    
    // Calculate structural similarity
    double node_similarity = static_cast<double>(result.matched_nodes) / result.total_nodes;
    
    // Calculate edge similarity (successor relationships)
    double edge_similarity = calculateEdgeSimilarity(cfg1, cfg2, result.node_matches);
    
    // Combined similarity score
    result.similarity = 0.6 * node_similarity + 0.4 * edge_similarity;
    
    return result;
}

std::vector<std::pair<int, int>> StructuralMatcher::findNodeMatches(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2) {
    std::vector<std::pair<int, int>> matches;
    std::vector<bool> used_cfg2(cfg2.blocks.size(), false);
    
    for (size_t i = 0; i < cfg1.blocks.size(); i++) {
        int best_match = -1;
        double best_similarity = 0.0;
        
        for (size_t j = 0; j < cfg2.blocks.size(); j++) {
            if (used_cfg2[j]) continue;
            
            double similarity = calculateBlockSimilarity(cfg1.blocks[i], cfg2.blocks[j]);
            if (similarity > best_similarity && similarity > 0.5) { // Threshold for matching
                best_similarity = similarity;
                best_match = j;
            }
        }
        
        if (best_match != -1) {
            matches.push_back({static_cast<int>(i), best_match});
            used_cfg2[best_match] = true;
        }
    }
    
    return matches;
}

double StructuralMatcher::calculateBlockSimilarity(const BasicBlock& block1, const BasicBlock& block2) {
    // Check control flow similarity
    if (!controlFlowMatches(block1, block2)) {
        return 0.0;
    }
    
    // Count similar token types
    std::map<std::string, int> types1, types2;
    
    for (const Token& token : block1.tokens) {
        if (token.type == "keyword" || token.type == "symbol") {
            types1[token.value]++;
        } else if (token.type == "identifier") {
            types1["VAR"]++;
        }
    }
    
    for (const Token& token : block2.tokens) {
        if (token.type == "keyword" || token.type == "symbol") {
            types2[token.value]++;
        } else if (token.type == "identifier") {
            types2["VAR"]++;
        }
    }
    
    // Calculate Jaccard similarity
    std::unordered_set<std::string> all_types;
    for (const auto& pair : types1) all_types.insert(pair.first);
    for (const auto& pair : types2) all_types.insert(pair.first);
    
    int intersection = 0;
    for (const std::string& type : all_types) {
        intersection += std::min(types1[type], types2[type]);
    }
    
    int union_size = 0;
    for (const std::string& type : all_types) {
        union_size += std::max(types1[type], types2[type]);
    }
    
    return union_size > 0 ? static_cast<double>(intersection) / union_size : 0.0;
}

bool StructuralMatcher::controlFlowMatches(const BasicBlock& block1, const BasicBlock& block2) {
    // Check if both blocks have similar control flow keywords
    std::unordered_set<std::string> control1, control2;
    
    for (const Token& token : block1.tokens) {
        if (token.type == "keyword" && 
            (token.value == "if" || token.value == "else" || token.value == "while" || 
             token.value == "for" || token.value == "return")) {
            control1.insert(token.value);
        }
    }
    
    for (const Token& token : block2.tokens) {
        if (token.type == "keyword" && 
            (token.value == "if" || token.value == "else" || token.value == "while" || 
             token.value == "for" || token.value == "return")) {
            control2.insert(token.value);
        }
    }
    
    // Both should have same control flow type or both should have none
    return control1 == control2;
}

double StructuralMatcher::calculateEdgeSimilarity(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2, 
                                                 const std::vector<std::pair<int, int>>& node_matches) {
    if (node_matches.empty()) return 0.0;
    
    int matching_edges = 0;
    int total_edges = 0;
    
    // Create mapping from cfg1 block indices to cfg2 block indices
    std::map<int, int> block_mapping;
    for (const auto& match : node_matches) {
        block_mapping[match.first] = match.second;
    }
    
    // Check edge preservation
    for (const auto& match : node_matches) {
        int block1_idx = match.first;
        int block2_idx = match.second;
        
        const BasicBlock& block1 = cfg1.blocks[block1_idx];
        const BasicBlock& block2 = cfg2.blocks[block2_idx];
        
        total_edges += block1.successors.size();
        
        for (int successor1 : block1.successors) {
            // Find if this successor is also matched
            for (const auto& succ_match : node_matches) {
                if (succ_match.first == successor1) {
                    int mapped_successor = succ_match.second;
                    
                    // Check if block2 has this successor
                    for (int successor2 : block2.successors) {
                        if (successor2 == mapped_successor) {
                            matching_edges++;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    
    return total_edges > 0 ? static_cast<double>(matching_edges) / total_edges : 1.0;
}
