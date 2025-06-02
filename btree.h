#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm> // para std::min

#include "node.h"
using namespace std;

template <typename TK>
class BTree {
private:
    Node<TK>* root;
    int M;  // grado u orden del árbol
    int n;  // total de elementos en el árbol

public:
    explicit BTree(int order) : root(nullptr), M(order), n(0) {}

    bool search(TK key);

    void insert(TK key);

    void remove(TK key) {
        // Método no implementado aún
    }

    int height();

    string toString(Node<TK>* nodo, string& sep);

    vector<TK> rangeSearch(TK begin, TK end);

    TK minKey();

    TK maxKey();

    void clear();

    int size(Node<TK>* nodo, int& result);

    static BTree* build_from_ordered_vector(const vector<TK>& elements, int order);

    bool check_properties();

    bool check_properties(Node<TK>* node, int min_keys, int level, int& leaf_level);

    ~BTree(); // liberar memoria
};

template <typename TK>
bool BTree<TK>::search(TK key) {
    if (!this->root) return false;
    Node<TK>* curr = this->root;
    while (!curr->leaf) {
        int i;
        for (i = 0; i < curr->count; ++i) {
            if (curr->keys[i] == key) return true;
            if (curr->keys[i] > key) break;
        }
        curr = curr->children[i];
    }
    for (int i = 0; i < curr->count; ++i) {
        if (curr->keys[i] == key) {
            return true;
        }
    }
    return false;
}

template <typename TK>
void BTree<TK>::insert(TK key) {
    stack<Node<TK>*> s;
    bool inserted = false;
    Node<TK>* curr = this->root;
    s.push(curr);
    while (!curr->leaf) {
        int i;
        for (i = 0; i < curr->count; ++i) {
            if (curr->keys[i] > key) break;
        }
        curr = curr->children[i];
        s.push(curr);
    }
    while (!inserted) {
        curr->insert(key);
        if (curr->count < this->M) {
            inserted = true;
        } else {
            int mid = curr->count / 2;
            TK middleKey = curr->keys[mid];
            auto newNode = new Node<TK>(this->M);
            newNode->leaf = curr->leaf;
            for (int i = mid + 1, j = 0; i < curr->count; ++i, ++j) {
                newNode->keys[j] = curr->keys[i];
                newNode->count++;
            }
            if (!curr->leaf) {
                for (int i = mid + 1, j = 0; i <= curr->count; ++i, ++j) {
                    newNode->children[j] = curr->children[i];
                }
            }
            curr->count = mid;
            s.pop();
            if (s.empty()) {
                auto newRoot = new Node<TK>(this->M);
                newRoot->keys[0] = middleKey;
                newRoot->children[0] = curr;
                newRoot->children[1] = newNode;
                newRoot->count = 1;
                newRoot->leaf = false;
                this->root = newRoot;
                inserted = true;
            } else {
                Node<TK>* parent = s.top();
                int i;
                for (i = parent->count - 1; i >= 0 && parent->keys[i] > middleKey; --i) {
                    parent->keys[i + 1] = parent->keys[i];
                    parent->children[i + 2] = parent->children[i + 1];
                }
                parent->keys[i + 1] = middleKey;
                parent->children[i + 2] = newNode;
                parent->count++;
                curr = parent;
                key = middleKey;
            }
        }
    }
}

template <typename TK>
int BTree<TK>::height() {
    if (!this->root) return 0;
    Node<TK>* curr = this->root;
    int ans = 1;
    while (!curr->leaf) {
        curr = curr->children[0];
        ++ans;
    }
    return ans;
}

template <typename TK>
string BTree<TK>::toString(Node<TK>* nodo, string& sep) {
    int i = 0;
    for (; i < nodo->count; ++i) {
        if (!nodo->leaf) toString(nodo->children[i], sep);
        sep += std::to_string(nodo->keys[i]) + ",";
    }
    if (!nodo->leaf) toString(nodo->children[i], sep);
    return sep;
}

template <typename TK>
vector<TK> BTree<TK>::rangeSearch(TK begin, TK end) {
    vector<TK> result;
    // Implementación pendiente o personalizada
    return result;
}

template <typename TK>
TK BTree<TK>::minKey() {
    if (!this->root) throw std::runtime_error("Empty tree");
    Node<TK>* curr = this->root;
    while (!curr->leaf) {
        curr = curr->children[0];
    }
    return curr->keys[0];
}

template <typename TK>
TK BTree<TK>::maxKey() {
    if (!this->root) throw std::runtime_error("Empty tree");
    Node<TK>* curr = this->root;
    while (!curr->leaf) {
        curr = curr->children[curr->count - 1];
    }
    return curr->keys[curr->count - 1];
}

template <typename TK>
void BTree<TK>::clear() {
    if (this->root) this->root->killSelf();
    this->root = nullptr;
}

template <typename TK>
int BTree<TK>::size(Node<TK>* nodo, int& result) {
    result = 0;
    if (!nodo) return 0;
    int i = 0;
    for (; i < nodo->count; ++i) {
        if (!nodo->leaf) size(nodo->children[i], result);
        result += 1;
    }
    if (!nodo->leaf) size(nodo->children[i], result);
    return result;
}

template <typename TK>
BTree<TK>* BTree<TK>::build_from_ordered_vector(const vector<TK>& elements, int order) {
    if (elements.empty()) return new BTree<TK>(order);

    BTree<TK>* tree = new BTree<TK>(order);
    int max_keys = order - 1;
    int n = static_cast<int>(elements.size());

    vector<Node<TK>*> current_level_nodes;

    // Crear nodos hoja
    for (int i = 0; i < n; i += max_keys) {
        auto leaf = new Node<TK>(order);
        leaf->leaf = true;
        int end = std::min(i + max_keys, n);
        for (int j = i; j < end; ++j) {
            leaf->keys[leaf->count++] = elements[j];
        }
        current_level_nodes.push_back(leaf);
    }

    // Construir niveles superiores hasta la raíz
    while (current_level_nodes.size() > 1) {
        vector<Node<TK>*> next_level_nodes;

        for (size_t i = 0; i < current_level_nodes.size(); i += order) {
            auto parent = new Node<TK>(order);
            parent->leaf = false;

            size_t group_end = std::min(i + order, current_level_nodes.size());
            int children_count = 0;

            for (size_t j = i; j < group_end; ++j) {
                parent->children[children_count++] = current_level_nodes[j];
            }

            parent->count = children_count - 1;

            for (int k = 0; k < parent->count; ++k) {
                parent->keys[k] = parent->children[k + 1]->keys[0];
            }

            next_level_nodes.push_back(parent);
        }

        current_level_nodes = next_level_nodes;
    }

    tree->root = current_level_nodes[0];
    tree->M = order;
    return tree;
}

template <typename TK>
bool BTree<TK>::check_properties() {
    int min_keys = (this->M % 2 == 0) ? (this->M / 2) : (this->M / 2);
    int leaf_level = -1;
    return check_properties(this->root, min_keys, 0, leaf_level);
}

template <typename TK>
bool BTree<TK>::check_properties(Node<TK>* node, int min_keys, int level, int& leaf_level) {
    if (!node) return true;
    if (node != this->root && node->count < min_keys) return false;
    if (node->count > this->M - 1) return false;
    if (node->leaf) {
        if (leaf_level == -1) leaf_level = level;
        return leaf_level == level;
    }
    for (int i = 0; i <= node->count; ++i) {
        if (!check_properties(node->children[i], min_keys, level + 1, leaf_level))
            return false;
    }
    return true;
}

template <typename TK>
BTree<TK>::~BTree() {
    clear();
}

#endif
