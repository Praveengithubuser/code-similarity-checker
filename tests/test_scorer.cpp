#include "../include/Scorer.h"
#include "../include/CFGBuilder.h"
#include "../include/Normalizer.h"
#include <iostream>
#include <cassert>
#include <cmath>

void test_identical_code() {
    Normalizer normalizer;
    CFGBuilder builder;
    Scorer scorer;
    
    std::string code1 = "int sum = 0; for (int i = 0; i < 10; i++) { sum = sum + i; }";
    std::string code2 = "int total = 0; for (int j = 0; j < 10; j++) { total = total + j; }";
    
    auto tokens1 = normalizer.process(code1);
    auto tokens2 = normalizer.process(code2);
    
    auto cfg1 = builder.build(tokens1);
    auto cfg2 = builder.build(tokens2);
    
    auto score = scorer.calculate(cfg1, cfg2);
    
    // Should be very similar (>90%)
    assert(score.overall > 0.9);
    std::cout << "✓ Identical code test passed (Score: " << score.overall * 100 << "%)" << std::endl;
}

void test_different_code() {
    Normalizer normalizer;
    CFGBuilder builder;
    Scorer scorer;
    
    std::string code1 = "int sum = 0; sum = sum + 5;";
    std::string code2 = "if (x > 0) { return x; } else { return 0; }";
    
    auto tokens1 = normalizer.process(code1);
    auto tokens2 = normalizer.process(code2);
    
    auto cfg1 = builder.build(tokens1);
    auto cfg2 = builder.build(tokens2);
    
    auto score = scorer.calculate(cfg1, cfg2);
    
    // Should be different (<50%)
    assert(score.overall < 0.5);
    std::cout << "✓ Different code test passed (Score: " << score.overall * 100 << "%)" << std::endl;
}

void test_empty_code() {
    Normalizer normalizer;
    CFGBuilder builder;
    Scorer scorer;
    
    std::string code1 = "";
    std::string code2 = "";
    
    auto tokens1 = normalizer.process(code1);
    auto tokens2 = normalizer.process(code2);
    
    auto cfg1 = builder.build(tokens1);
    auto cfg2 = builder.build(tokens2);
    
    auto score = scorer.calculate(cfg1, cfg2);
    
    // Empty code should be 100% similar
    assert(score.overall == 1.0);
    std::cout << "✓ Empty code test passed" << std::endl;
}

void test_weight_setting() {
    Scorer scorer;
    
    // Test weight setting
    scorer.setWeights(0.7, 0.3);  // 70% structural, 30% semantic
    
    // No assertion needed, just testing that it doesn't crash
    std::cout << "✓ Weight setting test passed" << std::endl;
}

void test_score_bounds() {
    Normalizer normalizer;
    CFGBuilder builder;
    Scorer scorer;
    
    std::string code1 = "int x = 5;";
    std::string code2 = "int y = 10;";
    
    auto tokens1 = normalizer.process(code1);
    auto tokens2 = normalizer.process(code2);
    
    auto cfg1 = builder.build(tokens1);
    auto cfg2 = builder.build(tokens2);
    
    auto score = scorer.calculate(cfg1, cfg2);
    
    // Score should be between 0 and 1
    assert(score.overall >= 0.0 && score.overall <= 1.0);
    assert(score.structural >= 0.0 && score.structural <= 1.0);
    assert(score.semantic >= 0.0 && score.semantic <= 1.0);
    
    std::cout << "✓ Score bounds test passed" << std::endl;
}

int main() {
    std::cout << "Running Scorer tests..." << std::endl;
    
    test_identical_code();
    test_different_code();
    test_empty_code();
    test_weight_setting();
    test_score_bounds();
    
    std::cout << "All Scorer tests passed!" << std::endl;
    return 0;
}
