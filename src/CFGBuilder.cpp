#include "CFGBuilder.h"
#include <set>

CFGBuilder::CFG CFGBuilder::build(const std::vector<Token>& tokens) {
    CFG cfg;
    int current_block_id = 0;
    BasicBlock current_block;
    current_block.id = current_block_id;
    
    for (size_t i = 0; i < tokens.size(); i++) {
        const Token& token = tokens[i];
        
        // Control flow keywords that end current block
        if (token.type == "keyword" && 
            (token.value == "if" || token.value == "else" || 
             token.value == "while" || token.value == "for")) {
            
            // Close current block if it has tokens
            if (!current_block.tokens.empty()) {
                cfg.blocks.push_back(current_block);
                cfg.block_map[current_block.id] = current_block;
            }
            
            // Start new block with control keyword
            current_block_id++;
            current_block = BasicBlock();
            current_block.id = current_block_id;
            current_block.tokens.push_back(token);
            
        } else if (token.value == "{" || token.value == "}") {
            // Block delimiters - close current block
            current_block.tokens.push_back(token);
            
            if (!current_block.tokens.empty()) {
                cfg.blocks.push_back(current_block);
                cfg.block_map[current_block.id] = current_block;
            }
            
            current_block_id++;
            current_block = BasicBlock();
            current_block.id = current_block_id;
            
        } else if (token.value == ";") {
            // Statement end - add to current block and potentially close
            current_block.tokens.push_back(token);
            
        } else {
            // Regular token - add to current block
            current_block.tokens.push_back(token);
        }
    }
    
    // Add final block if it has tokens
    if (!current_block.tokens.empty()) {
        cfg.blocks.push_back(current_block);
        cfg.block_map[current_block.id] = current_block;
    }
    
    // Build successor relationships
    buildSuccessors(cfg);
    
    return cfg;
}

void CFGBuilder::buildSuccessors(CFG& cfg) {
    for (size_t i = 0; i < cfg.blocks.size(); i++) {
        BasicBlock& block = cfg.blocks[i];
        
        // Check if this block contains control flow
        bool has_control_flow = false;
        for (const Token& token : block.tokens) {
            if (token.type == "keyword" && 
                (token.value == "if" || token.value == "while" || token.value == "for")) {
                has_control_flow = true;
                break;
            }
        }
        
        if (has_control_flow) {
            // Control flow block - can branch to multiple successors
            if (i + 1 < cfg.blocks.size()) {
                block.successors.push_back(cfg.blocks[i + 1].id);
            }
            if (i + 2 < cfg.blocks.size()) {
                block.successors.push_back(cfg.blocks[i + 2].id);
            }
        } else {
            // Sequential block - goes to next block
            if (i + 1 < cfg.blocks.size()) {
                block.successors.push_back(cfg.blocks[i + 1].id);
            }
        }
        
        // Update in block_map
        cfg.block_map[block.id] = block;
    }
}
