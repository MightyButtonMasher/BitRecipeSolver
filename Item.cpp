#include "Item.h"

using namespace std;


code_t Recipe::GetResult() const  {
    if (method)
        return p1->code & p2->code;
    else
        return p1->code | p2->code;
}

void Item::AddRecipe(Item* const p1, Item* const p2, const bool method) {
    recipes.emplace_back(method, p1, p2);
}

void Item::Trim() {
    recipes.shrink_to_fit();
}

void Item::CalculateCode() {
    if (code) return;
    const auto& recipe = recipes.back();
    recipe.p1->CalculateCode();
    recipe.p2->CalculateCode();
    code = recipe.GetResult();
}

void CalculateBitRecipe(const Recipe& recipe, BitRecipe& output, const bool mode) {
    const auto& r1 = recipe.p1->GetBitRecipe(mode);
    const auto& r2 = recipe.p2->GetBitRecipe(mode);
    if (!output)
        output.Set(r1, r2, recipe.method, mode);
}

const BitRecipe& Item::GetBitRecipe(const bool mode) {
    if (bitRecipe)
        return bitRecipe;

    CalculateBitRecipe(recipes.back(), bitRecipe, mode);
    bitRecipe.data.shrink_to_fit();
    return bitRecipe;
}

void Item::GetEquations(SubsetSolver& solver) {
    solver.AddRecipe(GetBitRecipe(false));
    if (exploreStatus == BASE ? recipes.empty() : recipes.size() < 2)
        return;

    BitRecipe prev = bitRecipe;
    for (const auto & recipe : recipes) {
        BitRecipe curr;
        CalculateBitRecipe(recipe, curr, false);
        solver.AddSubset(prev, curr);
        prev = curr;
    }

    if (exploreStatus == BASE)
        solver.AddSubset(prev, bitRecipe);
}

void Item::GetBase(vector<Item *> &base) {
    if (exploreStatus >= DONE) return;
    if (exploreStatus == BUSY || recipes.empty()) {
        bitRecipe.Set(base.size());
        base.push_back(this);
        exploreStatus = BASE;
    } else {
        exploreStatus = BUSY;
        recipes.back().p1->GetBase(base);
        recipes.back().p2->GetBase(base);
        if (exploreStatus == BUSY)
            exploreStatus = DONE;
    }
}

void Item::GetClauses(set<BitRecipe::clause_t> &clauses) {
    if (!bitRecipe)
        GetBitRecipe(true);
    for (const auto clause : bitRecipe) {
        if (!bitRecipe.ContainsSuperset(clause, true))
            clauses.insert(clause);
    }
}

void Item::SetBitRecipe(const vector<BitRecipe::clause_t>& baseClauses) {
    if (exploreStatus != BASE) {
        bitRecipe.data.clear();
        return;
    }
    const auto ownClause = bitRecipe.data.front();
    bitRecipe.data.clear();
    for (const auto clause : baseClauses) {
        if ((ownClause & clause) != 0)
            bitRecipe.data.push_back(clause);
    }
}
