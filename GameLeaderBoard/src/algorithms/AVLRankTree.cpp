#include "algorithms/AVLRankTree.h"

#include <algorithm>

AVLRankTree::~AVLRankTree() {
    destroy(root);
}

bool AVLRankTree::comesBefore(
    int leftRate,
    const std::string& leftUsername,
    int rightRate,
    const std::string& rightUsername) {
    if (leftRate != rightRate) {
        return leftRate > rightRate;
    }
    return leftUsername < rightUsername;
}

int AVLRankTree::height(const Node* node) {
    return node == nullptr ? 0 : node->height;
}

std::size_t AVLRankTree::subtreeSize(const Node* node) {
    return node == nullptr ? 0 : node->subtreeSize;
}

void AVLRankTree::updateMetadata(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->height = 1 + std::max(height(node->left), height(node->right));
    node->subtreeSize = 1 + subtreeSize(node->left) + subtreeSize(node->right);
}

int AVLRankTree::balanceFactor(const Node* node) {
    return node == nullptr ? 0 : height(node->left) - height(node->right);
}

AVLRankTree::Node* AVLRankTree::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    updateMetadata(node);
    updateMetadata(newRoot);
    return newRoot;
}

AVLRankTree::Node* AVLRankTree::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    updateMetadata(node);
    updateMetadata(newRoot);
    return newRoot;
}

AVLRankTree::Node* AVLRankTree::rebalance(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    updateMetadata(node);
    const int balance = balanceFactor(node);
    if (balance > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    if (balance < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
}

AVLRankTree::Node* AVLRankTree::insert(
    Node* node,
    int rate,
    const std::string& username) {
    if (node == nullptr) {
        return new Node{rate, username};
    }
    if (comesBefore(rate, username, node->winRateBasisPoints, node->username)) {
        node->left = insert(node->left, rate, username);
    } else {
        node->right = insert(node->right, rate, username);
    }
    return rebalance(node);
}

AVLRankTree::Node* AVLRankTree::detachMinimum(Node* node, Node*& minimum) {
    if (node->left == nullptr) {
        minimum = node;
        return node->right;
    }
    node->left = detachMinimum(node->left, minimum);
    return rebalance(node);
}

AVLRankTree::Node* AVLRankTree::erase(
    Node* node,
    int rate,
    const std::string& username,
    bool& erased) {
    if (node == nullptr) {
        return nullptr;
    }

    if (rate == node->winRateBasisPoints && username == node->username) {
        erased = true;
        if (node->left == nullptr) {
            Node* right = node->right;
            delete node;
            return right;
        }
        if (node->right == nullptr) {
            Node* left = node->left;
            delete node;
            return left;
        }

        Node* replacement = nullptr;
        node->right = detachMinimum(node->right, replacement);
        replacement->left = node->left;
        replacement->right = node->right;
        delete node;
        return rebalance(replacement);
    }

    if (comesBefore(rate, username, node->winRateBasisPoints, node->username)) {
        node->left = erase(node->left, rate, username, erased);
    } else {
        node->right = erase(node->right, rate, username, erased);
    }
    return erased ? rebalance(node) : node;
}

int AVLRankTree::countGreaterThan(const Node* node, int rate) {
    if (node == nullptr) {
        return 0;
    }
    if (node->winRateBasisPoints > rate) {
        return static_cast<int>(subtreeSize(node->left)) + 1 +
               countGreaterThan(node->right, rate);
    }
    return countGreaterThan(node->left, rate);
}

void AVLRankTree::destroy(Node* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

bool AVLRankTree::validate(
    const Node* node,
    const Node* lower,
    const Node* upper,
    int& calculatedHeight,
    std::size_t& calculatedSize) {
    if (node == nullptr) {
        calculatedHeight = 0;
        calculatedSize = 0;
        return true;
    }
    if ((lower != nullptr &&
         !comesBefore(
             lower->winRateBasisPoints,
             lower->username,
             node->winRateBasisPoints,
             node->username)) ||
        (upper != nullptr &&
         !comesBefore(
             node->winRateBasisPoints,
             node->username,
             upper->winRateBasisPoints,
             upper->username))) {
        return false;
    }

    int leftHeight = 0;
    int rightHeight = 0;
    std::size_t leftSize = 0;
    std::size_t rightSize = 0;
    if (!validate(node->left, lower, node, leftHeight, leftSize) ||
        !validate(node->right, node, upper, rightHeight, rightSize)) {
        return false;
    }
    calculatedHeight = 1 + std::max(leftHeight, rightHeight);
    calculatedSize = 1 + leftSize + rightSize;
    return node->height == calculatedHeight &&
           node->subtreeSize == calculatedSize &&
           std::abs(leftHeight - rightHeight) <= 1;
}

void AVLRankTree::insert(
    int winRateBasisPoints,
    const std::string& username) {
    root = insert(root, winRateBasisPoints, username);
}

bool AVLRankTree::erase(
    int winRateBasisPoints,
    const std::string& username) {
    bool erased = false;
    root = erase(root, winRateBasisPoints, username, erased);
    return erased;
}

int AVLRankTree::rankForWinRate(int winRateBasisPoints) const {
    return 1 + countGreaterThan(root, winRateBasisPoints);
}

std::size_t AVLRankTree::size() const {
    return subtreeSize(root);
}

bool AVLRankTree::validate() const {
    int calculatedHeight = 0;
    std::size_t calculatedSize = 0;
    return validate(root, nullptr, nullptr, calculatedHeight, calculatedSize);
}
