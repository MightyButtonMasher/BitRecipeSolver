#ifndef ALCHEMYFIX_BITRECIPE_H
#define ALCHEMYFIX_BITRECIPE_H

#include <cstdint>
#include <ostream>
#include <vector>
#include <unordered_set>

class BitRecipe {
public:
    typedef uint64_t clause_t;
    std::vector<clause_t> data;

    void Set(size_t index);
    void Set(const BitRecipe& r1, const BitRecipe& r2, bool method, bool mode);
    void UnionWith(const BitRecipe& other);
    void SetUnion(const BitRecipe& r1, const BitRecipe& r2);
    [[nodiscard]] BitRecipe WithoutSuperSet(const BitRecipe &other) const;
    void ReplaceClauses(const BitRecipe& from, const BitRecipe& with, std::unordered_set<clause_t>& except);
    void ReplaceClause(clause_t clause, const BitRecipe &with);
    [[nodiscard]] bool ContainsSuperset(clause_t c1, bool strict = false) const;
    bool operator!=(const BitRecipe& other) const;
    explicit operator bool() const { return !data.empty(); }

    void Print(std::ostream &stream, size_t bits) const;
    static void Print(std::ostream &stream, size_t bits, clause_t clause);

    [[nodiscard]] auto begin() const { return data.begin(); }
    [[nodiscard]] auto end() const { return data.end(); }

private:
    void Sort();
};


#endif //ALCHEMYFIX_BITRECIPE_H
