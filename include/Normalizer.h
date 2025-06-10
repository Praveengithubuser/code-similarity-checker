#ifndef NORMALIZER_H
#define NORMALIZER_H

#include <cctype>
#include <map>    // Add this line
#include <regex>  // Add this line
#include <string>
#include <unordered_set>  // Add this line
#include <vector>

struct Token {
    std::string type;
    std::string value;

    bool operator==(const Token& other) const { return type == other.type && value == other.value; }
};

class Normalizer {
   public:
    std::vector<Token> process(const std::string& code);

   private:
    void removeComments(std::string& code);
    void normalizeVariables(std::vector<Token>& tokens);
    std::vector<Token> tokenize(const std::string& code);
    bool isKeyword(const std::string& word);
};

#endif
