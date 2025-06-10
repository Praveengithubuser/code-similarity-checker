#ifndef SEMANTICHASHER_H
#define SEMANTICHASHER_H

#include <string>
#include <unordered_set>
#include <vector>

#include "CFGBuilder.h"

class SemanticHasher {
   public:
    // Hash a basic block's semantic content
    std::string hashBlock(const BasicBlock& block);

    // Compare two blocks semantically
    double compareBlocks(const BasicBlock& block1, const BasicBlock& block2);

   private:
    // Extract semantic patterns from tokens
    std::string extractSemanticPattern(const std::vector<Token>& tokens);

    // Normalize operations (e.g., += becomes ADD_ASSIGN)
    std::string normalizeOperation(const std::vector<Token>& tokens);

    // Check if tokens represent similar operations
    bool areOperationsSimilar(const std::string& op1, const std::string& op2);

    // Simple hash function
    std::size_t simpleHash(const std::string& str);
};

#endif
