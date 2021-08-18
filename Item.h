#ifndef ALCHEMYFIX_ITEM_H
#define ALCHEMYFIX_ITEM_H

#include <string>
#include <vector>
#include <set>

#include "BitRecipe.h"
#include "SubsetSolver.h"

typedef uint64_t code_t;
class Item;

struct Recipe {
    const bool method;
    Item* const p1, * const p2;

    Recipe(bool method, Item* const p1, Item* const p2): method(method), p1(p1), p2(p2){}
    [[nodiscard]] code_t GetResult() const;
};

class Item {
public:
    const std::string name;
    code_t code{};

private:
    BitRecipe bitRecipe;
    std::vector<Recipe> recipes;
    enum ExploreStatus { NONE, BUSY, DONE, BASE } exploreStatus = NONE;

public:
    explicit Item(std::string name) : name(std::move(name)) {}
    void AddRecipe(Item* p1, Item* p2, bool method);
    void Trim();

    void CalculateCode();
    void GetBase(std::vector<Item*>& base);
    const BitRecipe& GetBitRecipe(bool mode);
    void SetBitRecipe(const std::vector<BitRecipe::clause_t>& baseClauses);
    void GetEquations(SubsetSolver& solver);
    void GetClauses(std::set<BitRecipe::clause_t>& clauses);
};


#endif //ALCHEMYFIX_ITEM_H
