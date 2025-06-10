#ifndef CFGBUILDER_H
#define CFGBUILDER_H

#include <map>
#include <vector>

#include "Normalizer.h"

struct BasicBlock {
    int id;
    std::vector<Token> tokens;
    std::vector<int> successors;
};

class CFGBuilder {
   public:
    struct CFG {
        std::vector<BasicBlock> blocks;
        std::map<int, BasicBlock> block_map;
    };

    CFG build(const std::vector<Token>& tokens);

   private:
    void buildSuccessors(CFG& cfg);
};

#endif
