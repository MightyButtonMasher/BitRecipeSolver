#include "BitRecipe.h"

using namespace std;

void BitRecipe::Sort() {
    sort(data.begin(), data.end());
    data.erase(unique(data.begin(), data.end()), data.end());
}

void BitRecipe::Set(const size_t index) {
    data.push_back(clause_t(1) << index);
    data.shrink_to_fit();
}

void BitRecipe::Set(const BitRecipe &r1, const BitRecipe &r2, const bool method, const bool mode) {
    data.clear();
    if (method) {
        if (mode) {
            data.resize(min(r1.data.size(), r2.data.size()));
            const auto it = set_intersection(r1.data.begin(), r1.data.end(), r2.data.begin(), r2.data.end(), data.begin());
            data.erase(it, data.end());
        } else {
            for (const auto c1 : r1.data) {
                for (const auto c2 : r2.data)
                    data.push_back(c1 | c2);
            }
            Sort();
        }
    } else {
        SetUnion(r1, r2);
    }
}

void BitRecipe::SetUnion(const BitRecipe &r1, const BitRecipe &r2) {
    data.resize(r1.data.size() + r2.data.size());
    const auto it = set_union(r1.data.begin(), r1.data.end(), r2.data.begin(), r2.data.end(), data.begin());
    data.erase(it, data.end());
}

void BitRecipe::UnionWith(const BitRecipe &other) {
    vector<clause_t> result;
    result.resize(other.data.size() + data.size());
    const auto it = set_union(other.data.begin(), other.data.end(), data.begin(), data.end(), result.begin());
    result.erase(it, result.end());
    data = result;
}

bool BitRecipe::ContainsSuperset(const BitRecipe::clause_t c1, bool strict) const {
    for (const auto c2 : data) {
        if (c1 == (c2 | c1) && (!strict || c1 != c2))
            return true;
    }
    return false;
}

void BitRecipe::ReplaceClauses(const BitRecipe& from, const BitRecipe &with, unordered_set<clause_t>& except) {
    bool hasChanges = false;
    for (vector<clause_t>::difference_type i = data.size() - 1; i >= 0; --i) {
        auto baseClause = data[i];
        if (!from.ContainsSuperset(baseClause)) continue;

        bool canStay = false;
        for (const auto c2 : with.data) {
            if (baseClause == (baseClause | c2)) {
                canStay = true;
            } else if (!except.contains(baseClause | c2)) {
                data.push_back(baseClause | c2);
                hasChanges = true;
            }
        }
        if (!canStay) {
            except.insert(baseClause);
            data.erase(data.begin() + i);
        }
    }
    if (hasChanges)
        Sort();
}

void BitRecipe::ReplaceClause(const clause_t clause, const BitRecipe &with) {
    bool hasChanges = false;
    for (size_t i = data.size(); i > 0; --i) {
        auto baseClause = data[i - 1];
        if (baseClause != (clause | baseClause)) continue;
        hasChanges = true;

        auto c2 = with.data.begin();
        data[i - 1] = baseClause | *(c2++);
        while (c2 < with.data.end())
            data.push_back(baseClause | *(c2++));
    }
    if (hasChanges)
        Sort();
}

bool BitRecipe::operator!=(const BitRecipe &other) const {
    if (data.size() != other.data.size())
        return true;
    for (int i = 0; i < data.size(); ++i)
        if (data[i] != other.data[i])
            return true;
    return false;
}

BitRecipe BitRecipe::WithoutSuperSet(const BitRecipe &other) const {
    BitRecipe output;
    for (const auto clause : data) {
        if (!other.ContainsSuperset(clause))
            output.data.push_back(clause);
    }
    return output;
}

void BitRecipe::Print(ostream& stream, const size_t bits, const clause_t clause) {
    for (size_t bit = 0; bit < bits; ++bit)
        if (((clause >> bit) & 1) == 1)
            stream << bit;
}

void BitRecipe::Print(ostream& stream, const size_t bits) const {
    for (const auto clause : data) {
        Print(stream, bits, clause);
        stream << ' ';
    }
}
