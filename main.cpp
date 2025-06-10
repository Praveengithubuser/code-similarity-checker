#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "include/CFGBuilder.h"
#include "include/Normalizer.h"
#include "include/Scorer.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        std::cerr << "   Make sure the file exists in the current directory." << std::endl;
        return "";
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    if (content.empty()) {
        std::cerr << "Warning: File '" << filename << "' is empty." << std::endl;
    }

    return content;
}

void printHeader() {
    std::cout << "==============================================" << std::endl;
    std::cout << "           CODE SIMILARITY CHECKER             " << std::endl;
    std::cout << "        Structural + Semantic Analysis         " << std::endl;
    std::cout << "==============================================" << std::endl;
}

void printResults(const Scorer::Score& score, const std::string& file1, const std::string& file2) {
    std::cout << "\nANALYSIS RESULTS:" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Files Compared: " << file1 << " <-> " << file2 << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Structural Similarity: " << score.structural * 100 << "%" << std::endl;
    std::cout << "Semantic Similarity:   " << score.semantic * 100 << "%" << std::endl;
    std::cout << "Overall Similarity:    " << score.overall * 100 << "%" << std::endl;
    std::cout << "Matched Blocks:        " << score.matched_blocks << "/" << score.total_blocks
              << std::endl;

    std::cout << "\nVERDICT:" << std::endl;
    if (score.overall >= 0.90) {
        std::cout << "VERY HIGH SIMILARITY (>=90%) - Very likely plagiarism!" << std::endl;
    } else if (score.overall >= 0.75) {
        std::cout << "HIGH SIMILARITY (75-89%) - Suspicious, requires investigation" << std::endl;
    } else if (score.overall >= 0.50) {
        std::cout << "MODERATE SIMILARITY (50-74%) - Some common patterns detected" << std::endl;
    } else if (score.overall >= 0.25) {
        std::cout << "LOW SIMILARITY (25-49%) - Minor similarities found" << std::endl;
    } else {
        std::cout << "VERY LOW SIMILARITY (<25%) - Likely independent work" << std::endl;
    }

    std::cout << "----------------------------------------------" << std::endl;
}

void printUsage(const std::string& program_name) {
    std::cout << "\nUSAGE:" << std::endl;
    std::cout << "   " << program_name << " <file1.cpp> <file2.cpp>" << std::endl;
    std::cout << "\nEXAMPLES:" << std::endl;
    std::cout << "   " << program_name << " student1.cpp student2.cpp" << std::endl;
    std::cout << "   " << program_name << " assignment1.cpp assignment2.cpp" << std::endl;
    std::cout << "\nNOTE: Place your .cpp files in the same directory as this program."
              << std::endl;
}

int main(int argc, char* argv[]) {
    printHeader();

    if (argc != 3) {
        std::cout << "\nError: Incorrect number of arguments." << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::string file1 = argv[1];
    std::string file2 = argv[2];

    std::cout << "\nAnalyzing files..." << std::endl;
    std::cout << "   File 1: " << file1 << std::endl;
    std::cout << "   File 2: " << file2 << std::endl;

    // Read the code files
    std::string code1 = readFile(file1);
    std::string code2 = readFile(file2);

    if (code1.empty() && code2.empty()) {
        std::cout << "\nBoth files are empty - 100% similarity" << std::endl;
        return 0;
    }

    if (code1.empty() || code2.empty()) {
        std::cout << "\nError: One or both files are empty or couldn't be read." << std::endl;
        return 1;
    }

    try {
        std::cout << "Processing tokens..." << std::endl;

        // Initialize components
        Normalizer normalizer;
        CFGBuilder cfgBuilder;
        Scorer scorer;

        // Set weights (40% structural, 60% semantic for better plagiarism detection)
        scorer.setWeights(0.4, 0.6);

        // Process first file
        auto tokens1 = normalizer.process(code1);
        auto cfg1 = cfgBuilder.build(tokens1);

        // Process second file
        auto tokens2 = normalizer.process(code2);
        auto cfg2 = cfgBuilder.build(tokens2);

        std::cout << "Calculating similarity..." << std::endl;

        // Calculate similarity
        auto score = scorer.calculate(cfg1, cfg2);

        // Display results
        printResults(score, file1, file2);

    } catch (const std::exception& e) {
        std::cerr << "\nError during analysis: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\nUnknown error occurred during analysis." << std::endl;
        return 1;
    }

    return 0;
}
