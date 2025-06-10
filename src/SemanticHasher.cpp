#include "SemanticHasher.h"

#include <algorithm>
#include <functional>
#include <sstream>

std::string SemanticHasher::hashBlock(const BasicBlock& block) {
    if (block.tokens.empty()) {
        return "EMPTY_BLOCK";
    }

    std::string semantic_pattern = extractSemanticPattern(block.tokens);
    return std::to_string(simpleHash(semantic_pattern));
}

double SemanticHasher::compareBlocks(const BasicBlock& block1, const BasicBlock& block2) {
    std::string hash1 = hashBlock(block1);
    std::string hash2 = hashBlock(block2);

    if (hash1 == hash2) {
        return 1.0;  // Identical semantic content
    }

    // Extract operation patterns for more nuanced comparison
    std::string op1 = normalizeOperation(block1.tokens);
    std::string op2 = normalizeOperation(block2.tokens);

    if (areOperationsSimilar(op1, op2)) {
        return 0.8;  // Similar operations
    }

    return 0.0;  // Different semantic content
}

std::string SemanticHasher::extractSemanticPattern(const std::vector<Token>& tokens) {
    std::stringstream pattern;

    for (size_t i = 0; i < tokens.size(); i++) {
        const Token& token = tokens[i];

        if (token.type == "keyword") {
            pattern << token.value << " ";
        } else if (token.type == "identifier") {
            // All variables become VAR for semantic similarity
            pattern << "VAR ";
        } else if (token.value == "+" || token.value == "-" || token.value == "*" ||
                   token.value == "/") {
            pattern << "ARITH_OP ";
        } else if (token.value == "=" || token.value == "+=" || token.value == "-=" ||
                   token.value == "*=" || token.value == "/=") {
            pattern << "ASSIGN_OP ";
        } else if (token.value == "==" || token.value == "!=" || token.value == "<" ||
                   token.value == ">" || token.value == "<=" || token.value == ">=") {
            pattern << "COMP_OP ";
        } else if (token.value == "&&" || token.value == "||" || token.value == "!") {
            pattern << "LOGIC_OP ";
        } else if (token.value == "{" || token.value == "}") {
            pattern << "BLOCK ";
        } else if (token.value == "(" || token.value == ")") {
            pattern << "PAREN ";
        } else if (token.value == ";") {
            pattern << "STMT_END ";
        } else {
            // Other symbols
            pattern << "SYM ";
        }
    }

    return pattern.str();
}

std::string SemanticHasher::normalizeOperation(const std::vector<Token>& tokens) {
    std::stringstream operation;

    for (const Token& token : tokens) {
        if (token.type == "keyword") {
            if (token.value == "if")
                operation << "CONDITIONAL ";
            else if (token.value == "while" || token.value == "for")
                operation << "LOOP ";
            else if (token.value == "return")
                operation << "RETURN ";
            else
                operation << token.value << " ";
        } else if (token.value == "+" || token.value == "+=") {
            operation << "ADD ";
        } else if (token.value == "-" || token.value == "-=") {
            operation << "SUB ";
        } else if (token.value == "*" || token.value == "*=") {
            operation << "MUL ";
        } else if (token.value == "/" || token.value == "/=") {
            operation << "DIV ";
        } else if (token.value == "=") {
            operation << "ASSIGN ";
        } else if (token.value == "==" || token.value == "!=") {
            operation << "EQUALITY ";
        } else if (token.value == "<" || token.value == ">" || token.value == "<=" ||
                   token.value == ">=") {
            operation << "COMPARISON ";
        }
    }

    return operation.str();
}

bool SemanticHasher::areOperationsSimilar(const std::string& op1, const std::string& op2) {
    // Define groups of similar operations
    static const std::unordered_set<std::string> arithmetic = {"ADD", "SUB", "MUL", "DIV"};
    static const std::unordered_set<std::string> comparison = {"EQUALITY", "COMPARISON"};
    static const std::unordered_set<std::string> control = {"CONDITIONAL", "LOOP"};

    // Check if both operations belong to the same category
    if ((arithmetic.count(op1) && arithmetic.count(op2)) ||
        (comparison.count(op1) && comparison.count(op2)) ||
        (control.count(op1) && control.count(op2))) {
        return true;
    }

    return false;
}

std::size_t SemanticHasher::simpleHash(const std::string& str) {
    return std::hash<std::string>{}(str);
}
