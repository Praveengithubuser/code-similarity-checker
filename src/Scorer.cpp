#include "Scorer.h"

#include <algorithm>
#include <iostream>

Scorer::Score Scorer::calculate(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2) {
    Score result;
    result.structural = 0.0;
    result.semantic = 0.0;
    result.overall = 0.0;
    result.matched_blocks = 0;
    result.total_blocks = std::max(cfg1.blocks.size(), cfg2.blocks.size());

    // Handle empty CFGs
    if (cfg1.blocks.empty() && cfg2.blocks.empty()) {
        result.structural = 1.0;
        result.semantic = 1.0;
        result.overall = 1.0;
        return result;
    }

    if (cfg1.blocks.empty() || cfg2.blocks.empty()) {
        result.structural = 0.0;
        result.semantic = 0.0;
        result.overall = 0.0;
        return result;
    }

    // Calculate structural similarity
    MatchResult structural_result = matcher.compare(cfg1, cfg2);
    result.structural = structural_result.similarity;
    result.matched_blocks = structural_result.matched_nodes;

    // Calculate semantic similarity for matched blocks
    result.semantic = calculateSemanticSimilarity(cfg1, cfg2, structural_result.node_matches);

    // Calculate overall similarity using weighted combination
    result.overall = structural_weight * result.structural + semantic_weight * result.semantic;

    // Ensure score is between 0 and 1
    result.overall = std::max(0.0, std::min(1.0, result.overall));

    return result;
}

void Scorer::setWeights(double structural_w, double semantic_w) {
    // Normalize weights to sum to 1.0
    double total = structural_w + semantic_w;
    if (total > 0) {
        structural_weight = structural_w / total;
        semantic_weight = semantic_w / total;
    }
}

double Scorer::calculateSemanticSimilarity(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2,
                                           const std::vector<std::pair<int, int>>& matches) {
    if (matches.empty()) {
        return 0.0;
    }

    double total_similarity = 0.0;
    int valid_comparisons = 0;

    for (const auto& match : matches) {
        int block1_idx = match.first;
        int block2_idx = match.second;

        // Ensure indices are valid
        if (block1_idx >= 0 && block1_idx < static_cast<int>(cfg1.blocks.size()) &&
            block2_idx >= 0 && block2_idx < static_cast<int>(cfg2.blocks.size())) {
            const BasicBlock& block1 = cfg1.blocks[block1_idx];
            const BasicBlock& block2 = cfg2.blocks[block2_idx];

            double block_similarity = hasher.compareBlocks(block1, block2);
            total_similarity += block_similarity;
            valid_comparisons++;
        }
    }

    return valid_comparisons > 0 ? total_similarity / valid_comparisons : 0.0;
}
