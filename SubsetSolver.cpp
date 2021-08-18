#include "SubsetSolver.h"

using namespace std;

void SubsetSolver::AddRecipe(const BitRecipe &recipe) {
    yes.insert(recipe.begin(), recipe.end());
}

void SubsetSolver::AddSubset(const BitRecipe& sub, const BitRecipe& super) {
    const auto reducedSub = sub.WithoutSuperSet(super);
    if (reducedSub)
        subsets.emplace_back(reducedSub, super);
}

void SubsetSolver::ReplaceClauses(const BitRecipe& from, const BitRecipe &with) {
    vector<BitRecipe::clause_t> toAdd;
    erase_if(yes, [&](const auto baseClause) {
        if (!from.ContainsSuperset(baseClause))
            return false;

        bool shouldRemove = true;
        for (const auto c2 : with) {
            if (baseClause == (baseClause | c2))
                shouldRemove = false;
            else if (!no.contains(baseClause | c2))
                toAdd.push_back(baseClause | c2);
        }
        if (shouldRemove)
            no.insert(baseClause);
        return shouldRemove;
    });
    yes.insert(toAdd.begin(), toAdd.end());
}

vector<BitRecipe::clause_t> SubsetSolver::SatisfySubsets() {
    size_t oldYes, oldNo;
    do {
        oldYes = yes.size();
        oldNo = no.size();
        for (const auto& subset : subsets)
            ReplaceClauses(subset.first, subset.second);
    } while (yes.size() != oldYes || no.size() != oldNo);
    return vector<BitRecipe::clause_t>(yes.begin(), yes.end());
}
