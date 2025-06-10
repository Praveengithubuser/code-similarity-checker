# code-similarity-checker

A hybrid plagiarism detection system combining Control Flow Graph analysis and semantic hashing to detect code similarity with 88%+ accuracy.

## Features
- Structural similarity using CFG analysis
- Semantic pattern detection despite variable renaming
- Handles formatting and syntax variations
- Real-time plagiarism detection for C++ code

## Architecture
- **Normalizer**: Code preprocessing and tokenization
- **CFGBuilder**: Control flow graph construction  
- **SemanticHasher**: Logic pattern analysis
- **StructuralMatcher**: Graph isomorphism algorithms
- **Scorer**: Hybrid similarity scoring (40% structural + 60% semantic)

## Applications
- Academic plagiarism detection
- Code review and duplicate detection
- Competitive programming cheating detection

