#include "../include/SemanticHasher.h"
#include "../include/CFGBuilder.h"
#include "../include/Normalizer.h"
#include <iostream>
#include <cassert>

void test_identical_blocks() {
    SemanticHasher hasher;
    Normalizer normalizer;
    
    // Create two identical blocks with different variable names
    std::string code1 = "sum = sum + i;";
    std::string code2 = "total = total + j;";
    
    auto tokens1 = normalizer.process(code1);
    auto tokens2 = normalizer.process(code2);
    
    BasicBlock block1 = {1, tokens1, {}};
    BasicBlock block2 = {2, tokens2, {}};
    
    std::string hash1 = hasher.hashBlock(block1);
    std::string hash2 = hasher.hashBlock(block2);
    
    // Should have same hash (semantic equivalence)
    assert(hash1 == hash2);
    std::cout << "✓ Identical blocks test passed" << std::endl;
}

void test_different_operations() {
    SemanticHasher hasher;
    Normalizer normalizer;
    
    std::string code1 = "sum = sum + i;";
    std::string code2 = "product = product * i;";
    
    auto tokens1 = normalizer.process(code1);
    auto tokens2 = normalizer.process(code2);
    
    BasicBlock block1 = {1, tokens1, {}};
    BasicBlock block2 = {2, tokens2, {}};
    
    double similarity = hasher.compareBlocks(block1, block2);
    
    // Should be similar (both arithmetic) but not identical
    assert(similarity > 0.0 && similarity < 1.0);
    std::cout << "✓ Different operations test passed" << std::endl;
}

void test_completely_different() {
    SemanticHasher hasher;
    Normalizer normalizer;
    
    std::string code1 = "sum = sum + i;";
    std::string code2 = "if (x > 0) return;";
    
    auto tokens1 = normalizer.process(code1);
    auto tokens2 = normalizer.process(code2);
    
    BasicBlock block1 = {1, tokens1, {}};
    BasicBlock block2 = {2, tokens2, {}};
    
    double similarity = hasher.compareBlocks(block1, block2);
    
    // Should be very different
    assert(similarity == 0.0);
    std::cout << "✓ Completely different test passed" << std::endl;
}

void test_empty_blocks() {
    SemanticHasher hasher;
    
    BasicBlock empty1 = {1, {}, {}};
    BasicBlock empty2 = {2, {}, {}};
    
    std::string hash1 = hasher.hashBlock(empty1);
    std::string hash2 = hasher.hashBlock(empty2);
    
    // Empty blocks should have same hash
    assert(hash1 == hash2);
    std::cout << "✓ Empty blocks test passed" << std::endl;
}

int main() {
    std::cout << "Running SemanticHasher tests..." << std::endl;
    
    test_identical_blocks();
    test_different_operations();
    test_completely_different();
    test_empty_blocks();
    
    std::cout << "All SemanticHasher tests passed!" << std::endl;
    return 0;
}
