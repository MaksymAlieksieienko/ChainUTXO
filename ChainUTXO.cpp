#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <optional>

struct Transaction {
    std::string txid;
    std::string input;
    std::vector<std::string> outputs;
    std::vector<int> children;  // Child transaction
};

bool isValidTransaction(const Transaction& tx) {
    return (!tx.input.empty() && tx.outputs.size() == 2);
}

// Read csv file
std::vector<Transaction> loadTransactions(const std::string& filename) {
    std::vector<Transaction> txs;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file: " << filename << "\n";
        return txs;
    }
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream ss(line);
        std::string token;
        Transaction tx;
        std::getline(ss, tx.txid, ',');
        std::getline(ss, tx.input, ',');
        std::string output;
        while (std::getline(ss, output, ',')) {
            tx.outputs.push_back(output);
        }
        if (isValidTransaction(tx)) {
            txs.push_back(tx);
        }
    }
    return txs;
}

// Build a parent-child graph 
void buildGraph(std::vector<Transaction>& txs) {
    // Connect outputs with parent id
    std::unordered_map<std::string, int> outputToTx;
    for (size_t i = 0; i < txs.size(); i++) {
        for (const auto& out : txs[i].outputs) {
            outputToTx[out] = static_cast<int>(i);
        }
    }
    // Build children links.
    for (size_t i = 0; i < txs.size(); i++) {
        // Check if the input can be traced to any transaction's output
        if (outputToTx.find(txs[i].input) != outputToTx.end()) {
            int parentIndex = outputToTx[txs[i].input];
            txs[parentIndex].children.push_back(static_cast<int>(i));
        }
    }
}

std::pair<int, std::optional<int>> dfs(int idx, const std::vector<Transaction>& txs, std::vector<int>& dp, std::vector<std::optional<int>>& nextChild) {
    // If already computed, return the stored result
    if (dp[idx] != -1) {
        return { dp[idx], nextChild[idx] };
    }

    int maxLength = 1;
    std::optional<int> chosenChild = std::nullopt;

    // Compute length of chain
    for (int childIdx : txs[idx].children) {
        auto [childLength, _] = dfs(childIdx, txs, dp, nextChild);
        if (1 + childLength > maxLength) {
            maxLength = 1 + childLength;
            chosenChild = childIdx;
        }
    }

    dp[idx] = maxLength;
    nextChild[idx] = chosenChild;
    return { maxLength, chosenChild };
}

void findLongestChain(const std::vector<Transaction>& txs) {
    int n = static_cast<int>(txs.size());
    std::vector<int> dp(n, -1); // Save max chain length
    std::vector<std::optional<int>> nextChild(n, std::nullopt); // For chain reconstruction

    int bestChainLength = 0;
    int bestStart = -1;

    std::vector<bool> isChild(n, false);    // Save only child transactions
    for (const auto& tx : txs) {
        for (int child : tx.children) {
            isChild[child] = true;
        }
    }

    // Run DFS from every root
    for (int i = 0; i < n; i++) {
        if (!isChild[i]) {
            auto [chainLength, _] = dfs(i, txs, dp, nextChild);
            if (chainLength > bestChainLength) {
                bestChainLength = chainLength;
                bestStart = i;
            }
        }
    }

    // Also try all nodes in case there is no clear root in our data
    for (int i = 0; i < n; i++) {
        auto [chainLength, _] = dfs(i, txs, dp, nextChild);
        if (chainLength > bestChainLength) {
            bestChainLength = chainLength;
            bestStart = i;
        }
    }

    // Output the longest chain
    if (bestStart == -1) {
        std::cout << "No valid UTXO chain found.\n";
    }
    else {
        std::cout << "Longest chain length: " << bestChainLength << "\n";
        std::cout << "Chain of transactions: \n";
        int current = bestStart;
        while (current != -1) {
            std::cout << txs[current].txid << " -> ";
            if (nextChild[current].has_value())
                current = nextChild[current].value();
            else
                break;
        }
        std::cout << "END\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <transactions.csv>\n";
        return 1;
    }

    std::string filename = argv[1];
    auto transactions = loadTransactions(filename);
    if (transactions.empty()) {
        std::cerr << "No valid transactions loaded. Check input format and filtering rules.\n";
        return 1;
    }

    buildGraph(transactions);
    findLongestChain(transactions);
    return 0;
}