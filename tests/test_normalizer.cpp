#include "../include/Normalizer.h"
#include <iostream>
#include <cassert>

void test_tokenization() {
    Normalizer normalizer;
    std::string code = "int x = 5; if (x > 0) { return x; }";
    
    auto tokens = normalizer.process(code);
    
    // Check that we got some tokens
    assert(!tokens.empty());
    
    // Check for expected keywords
    bool found_int = false, found_if = false, found_return = false;
    for (const auto& token : tokens) {
        if (token.type == "keyword" && token.value == "int") found_int = true;
        if (token.type == "keyword" && token.value == "if") found_if = true;
        if (token.type == "keyword" && token.value == "return") found_return = true;
    }
    
    assert(found_int && found_if && found_return);
    std::cout << "✓ Tokenization test passed" << std::endl;
}

void test_variable_normalization() {
    Normalizer normalizer;
    std::string code = "int sum = 0; int count = 1;";
    
    auto tokens = normalizer.process(code);
    
    // Check that variables are normalized
    bool found_var1 = false, found_var2 = false;
    for (const auto& token : tokens) {
        if (token.type == "identifier" && token.value == "VAR_1") found_var1 = true;
        if (token.type == "identifier" && token.value == "VAR_2") found_var2 = true;
    }
    
    assert(found_var1 && found_var2);
    std::cout << "✓ Variable normalization test passed" << std::endl;
}

void test_comment_removal() {
    Normalizer normalizer;
    std::string code = "int x = 5; // This is a comment\n/* Multi-line comment */ int y = 10;";
    
    auto tokens = normalizer.process(code);
    
    // Check that comments are removed (no comment tokens should exist)
    for (const auto& token : tokens) {
        assert(token.value.find("//") == std::string::npos);
        assert(token.value.find("/*") == std::string::npos);
    }
    
    std::cout << "✓ Comment removal test passed" << std::endl;
}

int main() {
    std::cout << "Running Normalizer tests..." << std::endl;
    
    test_tokenization();
    test_variable_normalization();
    test_comment_removal();
    
    std::cout << "All Normalizer tests passed!" << std::endl;
    return 0;
}
