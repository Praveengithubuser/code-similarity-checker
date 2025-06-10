#include "Normalizer.h"

#include <cctype>
#include <map>
#include <regex>
#include <string>
#include <unordered_set>

std::vector<Token> Normalizer::process(const std::string& code) {
    std::string cleaned = code;
    removeComments(cleaned);
    auto tokens = tokenize(cleaned);
    normalizeVariables(tokens);
    return tokens;
}

void Normalizer::removeComments(std::string& code) {
    // Remove single-line comments
    size_t pos = 0;
    while ((pos = code.find("//", pos)) != std::string::npos) {
        size_t end = code.find('\n', pos);
        if (end == std::string::npos)
            end = code.length();
        code.erase(pos, end - pos);
    }

    // Remove multi-line comments
    pos = 0;
    while ((pos = code.find("/*", pos)) != std::string::npos) {
        size_t end = code.find("*/", pos + 2);
        if (end == std::string::npos)
            break;
        code.erase(pos, end - pos + 2);
    }
}

std::vector<Token> Normalizer::tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::string current = "";

    for (size_t i = 0; i < code.length(); i++) {
        char c = code[i];

        if (std::isalnum(c) || c == '_') {
            current += c;
        } else {
            if (!current.empty()) {
                std::string type = isKeyword(current) ? "keyword" : "identifier";
                tokens.push_back({type, current});
                current = "";
            }

            if (!std::isspace(c)) {
                std::string symbol(1, c);
                tokens.push_back({"symbol", symbol});
            }
        }
    }

    if (!current.empty()) {
        std::string type = isKeyword(current) ? "keyword" : "identifier";
        tokens.push_back({type, current});
    }

    return tokens;
}

void Normalizer::normalizeVariables(std::vector<Token>& tokens) {
    std::map<std::string, std::string> var_map;
    int counter = 1;

    for (auto& token : tokens) {
        if (token.type == "identifier" && !isKeyword(token.value)) {
            if (var_map.find(token.value) == var_map.end()) {
                var_map[token.value] = "VAR_" + std::to_string(counter++);
            }
            token.value = var_map[token.value];
        }
    }
}

bool Normalizer::isKeyword(const std::string& word) {
    static const std::unordered_set<std::string> keywords = {
        "int",    "float",  "double", "char", "if",     "else",    "while",
        "for",    "return", "class",  "void", "public", "private", "const",
        "static", "struct", "bool",   "true", "false"};
    return keywords.count(word) > 0;
}
