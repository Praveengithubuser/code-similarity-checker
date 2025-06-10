#include "Utils/StringUtils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>
#include <unordered_set>
#include <cmath>
#include <functional>

std::string StringUtils::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::string StringUtils::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StringUtils::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

std::string StringUtils::join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) return "";
    
    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); i++) {
        result += delimiter + strings[i];
    }
    
    return result;
}

double StringUtils::calculateJaccardSimilarity(const std::string& str1, const std::string& str2) {
    std::unordered_set<char> set1(str1.begin(), str1.end());
    std::unordered_set<char> set2(str2.begin(), str2.end());
    
    std::unordered_set<char> intersection;
    for (char c : set1) {
        if (set2.count(c)) {
            intersection.insert(c);
        }
    }
    
    std::unordered_set<char> union_set = set1;
    union_set.insert(set2.begin(), set2.end());
    
    return union_set.empty() ? 0.0 : static_cast<double>(intersection.size()) / union_set.size();
}

int StringUtils::calculateLevenshteinDistance(const std::string& str1, const std::string& str2) {
    int m = str1.length();
    int n = str2.length();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
            }
        }
    }
    
    return dp[m][n];
}

std::string StringUtils::calculateHash(const std::string& input) {
    return std::to_string(std::hash<std::string>{}(input));
}

std::size_t StringUtils::simpleHash(const std::string& str) {
    return std::hash<std::string>{}(str);
}

std::string StringUtils::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    return content;
}

bool StringUtils::writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    return true;
}

bool StringUtils::isValidIdentifier(const std::string& str) {
    if (str.empty() || (!std::isalpha(str[0]) && str[0] != '_')) {
        return false;
    }
    
    for (char c : str) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool StringUtils::containsKeyword(const std::string& str, const std::vector<std::string>& keywords) {
    for (const std::string& keyword : keywords) {
        if (str.find(keyword) != std::string::npos) {
            return true;
        }
    }
    return false;
}
