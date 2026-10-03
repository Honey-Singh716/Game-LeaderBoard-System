#pragma once

#include <cstddef>
#include <string>

class AVLRankTree {
public:
    AVLRankTree() = default;
    AVLRankTree(const AVLRankTree&) = delete;
    AVLRankTree& operator=(const AVLRankTree&) = delete;
    ~AVLRankTree();

    void insert(int winRateBasisPoints, const std::string& username);
    bool erase(int winRateBasisPoints, const std::string& username);
    int rankForWinRate(int winRateBasisPoints) const;
    std::size_t size() const;
    bool validate() const;

private:
    struct Node {
        int winRateBasisPoints;
        std::string username;
        int height = 1;
        std::size_t subtreeSize = 1;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    Node* root = nullptr;

    static bool comesBefore(
        int leftRate,
        const std::string& leftUsername,
        int rightRate,
        const std::string& rightUsername);
    static int height(const Node* node);
    static std::size_t subtreeSize(const Node* node);
    static void updateMetadata(Node* node);
    static int balanceFactor(const Node* node);
    static Node* rotateLeft(Node* node);
    static Node* rotateRight(Node* node);
    static Node* rebalance(Node* node);
    static Node* insert(Node* node, int rate, const std::string& username);
    static Node* erase(
        Node* node,
        int rate,
        const std::string& username,
        bool& erased);
    static Node* detachMinimum(Node* node, Node*& minimum);
    static int countGreaterThan(const Node* node, int rate);
    static void destroy(Node* node);
    static bool validate(
        const Node* node,
        const Node* lower,
        const Node* upper,
        int& calculatedHeight,
        std::size_t& calculatedSize);
};
