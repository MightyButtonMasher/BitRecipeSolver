#ifndef ALCHEMYFIX_SUBSETSOLVER_H
#define ALCHEMYFIX_SUBSETSOLVER_H

#include "BitRecipe.h"


class SubsetSolver {
private:
    std::vector<std::pair<BitRecipe, BitRecipe>> subsets;
    std::unordered_set<BitRecipe::clause_t> yes, no;

    void ReplaceClauses(const BitRecipe &from, const BitRecipe &with);
public:
    void AddRecipe(const BitRecipe& recipe);
    void AddSubset(const BitRecipe& sub, const BitRecipe& super);
    [[nodiscard]] std::vector<BitRecipe::clause_t> SatisfySubsets();
};


#endif //ALCHEMYFIX_SUBSETSOLVER_H
