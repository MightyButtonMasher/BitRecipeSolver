#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

#include "Item.h"

template<typename T>
void Output(ostream& stream, const size_t bits, const T code) {
    for (size_t bit = 0; bit < bits; ++bit)
        stream << ((code >> bit) & 1);
}

size_t CalculateCodes(const vector<Item*>& items, const vector<Item*>& base, const set<BitRecipe::clause_t>& clauses) {
    int i = 0;
    for (const auto clause : clauses) {
        for (size_t j = 0; j < base.size(); ++j)
            base[j]->code |= ((clause >> j) & code_t(1)) << i;
        ++i;
    }

    for (Item* const item : items)
        item->CalculateCode();

    return clauses.size();
}

auto MinimiseClauses(const vector<Item*>& items, const vector<BitRecipe::clause_t>& oldClauses) {
    for (Item *const item : items)
        item->SetBitRecipe(oldClauses);
    set<BitRecipe::clause_t> newClauses;
    for (Item *const item : items)
        item->GetClauses(newClauses);
    return newClauses;
}

auto SatisfyEquations(const vector<Item*>& items, const vector<Item*>& base) {
    SubsetSolver solver;
    for (Item *const item : items)
        item->GetEquations(solver);
    return solver.SatisfySubsets();
}

size_t Solve(const vector<Item*>& items) {
    vector<Item*> base;
    for (Item *const item : items)
        item->GetBase(base);
    const auto clauses = SatisfyEquations(items, base);
    const auto newClauses = MinimiseClauses(items, clauses);
    return CalculateCodes(items, base, newClauses);
}

Item* TryGetItem(map<string, Item*>& itemMap, const string& name) {
    Item*& item = itemMap[name];
    if (!item)
        item = new Item(name);
    return item;
}

vector<Item*> ReadRecipes() {
    map<string, Item*> itemMap;
    ifstream recipe("../recipes.txt");

    string i1, i2, i3, method, equals;
    while (recipe >> i1 >> method >> i2 >> equals >> i3) {
        if (equals != "=") {
            cout << "Expected '=', got: " << equals << endl;
            throw;
        }
        Item* const item1 = TryGetItem(itemMap, i1);
        Item* const item2 = TryGetItem(itemMap, i2);
        Item* const item3 = TryGetItem(itemMap, i3);
        item3->AddRecipe(item1, item2, method == "&&");
    }

    vector<Item*> items;
    items.reserve(itemMap.size());
    for (const auto& item : itemMap) {
        item.second->Trim();
        items.push_back(item.second);
    }
    return items;
}

void WriteCodes(vector<Item*>& items, const size_t bits) {
    const auto comp = [](const Item* const a, const Item* const b) {
        return a->code < b->code;
    };
    sort(items.begin(), items.end(), comp);
//    ofstream out("../codes.txt");
    ostream& out = cout;
    for (const Item* it : items) {
        Output(out, bits, it->code);
        out << ' ' << it->name << endl;
    }
}


int main() {
    vector<Item*> items = ReadRecipes();
    size_t bits = Solve(items);
    cout << "Bits: " << bits << endl;
    WriteCodes(items, bits);
    return 0;
}
