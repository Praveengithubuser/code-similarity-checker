#include "../include/CFGBuilder.h"
#include "../include/Normalizer.h"
#include <iostream>
#include <cassert>

void test_simple_cfg() {
    Normalizer normalizer;
    CFGBuilder builder;
    
    std::string code = "int x = 5; x = x + 1;";
    auto tokens = normalizer.process(code);
    auto cfg = builder.build(tokens);
    
    // Should have at least one block
    assert(!cfg.blocks.empty());
    std::cout << "✓ Simple CFG test passed" << std::endl;
}

void test_conditional_cfg() {
    Normalizer normalizer;
    CFGBuilder builder;
    
    std::string code = "if (x > 0) { y = 1; } else { y = 2; }";
    auto tokens = normalizer.process(code);
    auto cfg = builder.build(tokens);
    
    // Should have multiple blocks for conditional
    assert(cfg.blocks.size() >= 2);
    
    // Check for if keyword in one of the blocks
    bool found_if = false;
    for (const auto& block : cfg.blocks) {
        for (const auto& token : block.tokens) {
            if (token.type == "keyword" && token.value == "if") {
                found_if = true;
                break;
            }
        }
    }
    assert(found_if);
    std::cout << "✓ Conditional CFG test passed" << std::endl;
}

void test_loop_cfg() {
    Normalizer normalizer;
    CFGBuilder builder;
    
    std::string code = "while (i < 10) { i = i + 1; }";
    auto tokens = normalizer.process(code);
    auto cfg = builder.build(tokens);
    
    // Should have blocks for loop
    assert(!cfg.blocks.empty());
    
    // Check for while keyword
    bool found_while = false;
    for (const auto& block : cfg.blocks) {
        for (const auto& token : block.tokens) {
            if (token.type == "keyword" && token.value == "while") {
                found_while = true;
                break;
            }
        }
    }
    assert(found_while);
    std::cout << "✓ Loop CFG test passed" << std::endl;
}

void test_block_successors() {
    Normalizer normalizer;
    CFGBuilder builder;
    
    std::string code = "int x = 1; int y = 2;";
    auto tokens = normalizer.process(code);
    auto cfg = builder.build(tokens);
    
    // Check that block map is populated
    assert(!cfg.block_map.empty());
    std::cout << "✓ Block successors test passed" << std::endl;
}

int main() {
    std::cout << "Running CFGBuilder tests..." << std::endl;
    
    test_simple_cfg();
    test_conditional_cfg();
    test_loop_cfg();
    test_block_successors();
    
    std::cout << "All CFGBuilder tests passed!" << std::endl;
    return 0;
}
