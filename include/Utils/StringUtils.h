#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

class StringUtils {
   public:
    // String manipulation functions
    static std::string trim(const std::string& str);
    static std::string toLowerCase(const std::string& str);
    static std::string toUpperCase(const std::string& str);

    // String splitting and joining
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

    // String similarity functions
    static double calculateJaccardSimilarity(const std::string& str1, const std::string& str2);
    static int calculateLevenshteinDistance(const std::string& str1, const std::string& str2);
    static double calculateCosineSimilarity(const std::string& str1, const std::string& str2);

    // Hash functions
    static std::string calculateHash(const std::string& input);
    static std::size_t simpleHash(const std::string& str);

    // File operations
    static std::string readFile(const std::string& filename);
    static bool writeFile(const std::string& filename, const std::string& content);

    // Validation functions
    static bool isValidIdentifier(const std::string& str);
    static bool containsKeyword(const std::string& str, const std::vector<std::string>& keywords);
};

#endif
