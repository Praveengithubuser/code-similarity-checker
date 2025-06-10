#ifndef SCORER_H
#define SCORER_H

#include "CFGBuilder.h"
#include "SemanticHasher.h"
#include "StructuralMatcher.h"

class Scorer {
   public:
    struct Score {
        double structural;
        double semantic;
        double overall;
        int matched_blocks;
        int total_blocks;
    };

    // Calculate comprehensive similarity score between two CFGs
    Score calculate(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2);

    // Set weights for combining structural and semantic scores
    void setWeights(double structural_weight, double semantic_weight);

   private:
    double structural_weight = 0.4;  // Default weights
    double semantic_weight = 0.6;

    StructuralMatcher matcher;
    SemanticHasher hasher;

    // Calculate semantic similarity between matched blocks
    double calculateSemanticSimilarity(const CFGBuilder::CFG& cfg1, const CFGBuilder::CFG& cfg2,
                                       const std::vector<std::pair<int, int>>& matches);
};

#endif
