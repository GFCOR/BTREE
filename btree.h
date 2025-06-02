#ifndef BTree_H
#define BTree_H
#include <iostream>
#include <vector>
#include <stack>

#include "node.h"

using namespace std;

template <typename TK>
class BTree {
 private:
  Node<TK>* root;
  int M;  // grado u orden del arbol
  int n; // total de elementos en el arbol 

 public:
  BTree(int _M) : root(nullptr), M(_M) {}

 bool search(TK key){
   Node<TK>* curr = root;
   while (!curr->leaf){
    int i;
    for (i = 0; i < curr->count; i++) {
     if (curr->keys[i] == key) return true;
     if (curr->keys[i] > key) break;
    }
    curr = curr->children[i];
   }
   for(int i = 0; i < curr->count; i++){
    if(curr->keys[i] == key){
     return true;
    }
   }
   return false;
  }//indica si se encuentra o no un elemento//indica si se encuentra o no un elemento
  void insert(TK key) {
    stack<Node<TK>*> s;
    bool inserted = false;
    Node<TK>* curr = root;
    s.push(curr);
    while (!curr->leaf) {
        int i;
        for (i = 0; i < curr->count; i++) {
            if (curr->keys[i] > key) break;
        }
        curr = curr->children[i];
        s.push(curr);
    }
    while (!inserted) {
        curr->insert(key);
        if (curr->count < M) {
            inserted = true;
        } else {
            int mid = curr->count / 2;
            TK middleKey = curr->keys[mid];
            Node<TK>* newNode = new Node<TK>(M);
            newNode->leaf = curr->leaf;
            for (int i = mid + 1, j = 0; i < curr->count; i++, j++) {
                newNode->keys[j] = curr->keys[i];
                newNode->count++;
            }
            if (!curr->leaf) {
                for (int i = mid + 1, j = 0; i <= curr->count; i++, j++) {
                    newNode->children[j] = curr->children[i];
                }
            }
            curr->count = mid;
            s.pop();
            if (s.empty()) {
                Node<TK>* newRoot = new Node<TK>(M);
                newRoot->keys[0] = middleKey;
                newRoot->children[0] = curr;
                newRoot->children[1] = newNode;
                newRoot->count = 1;
                newRoot->leaf = false;
                root = newRoot;
                inserted = true;
            } else {
                Node<TK>* parent = s.top();
                int i;
                for (i = parent->count - 1; i >= 0 && parent->keys[i] > middleKey; i--) {
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
  void remove(TK key);//elimina un elemento
  int height() {
      Node<TK>* curr = root;
      int ans = 1;
      while (!curr->leaf) {
          curr = curr->children[0];
          ans++;
      }
      return ans;
  }
      //altura del arbol. Considerar altura 0 para arbol vacio
  string toString(Node<TK>* nodo, string& sep) {
          int i = 0;
          for (; i < nodo->count; i++) {
              if (!nodo->leaf) toString(nodo->children[i], sep);
              sep += std::to_string(nodo->keys[i]) + ",";
          }
          if (!nodo->leaf) toString(nodo->children[i], sep);
      return sep;
  }
      // recorrido inorder
    vector<TK> rangeSearch(TK begin, TK end){
      vector<int> result;
      Node<TK>* node = root;
      for (int i = 0; i < node->count; i++) {
          if (!node->leaf && node->keys[i] > begin) {
              rangeSearch(node->children[i]);
          }
          if (node->keys[i] >= begin && node->keys[i] <= end) {
              result.push_back(node->keys[i]);
          }
      }
      if (!node->leaf && node->keys[node->count] < end) {
          rangeSearch(node->children[node->count]);
      }
      return result;
  }

  TK minKey() {
      Node<TK>* curr = root;
      while (!curr->leaf) {
          curr = curr->children[0];
      }
      return curr->keys[0];
  }// minimo valor de la llave en el arbol
  TK maxKey() {
      Node<TK>* curr = root;
      while (!curr->leaf) {
          curr = curr->children[curr->count];
      }
      return curr->keys[curr->count -1];
  }
      // maximo valor de la llave en el arbol
  void clear() {
      root->killSelf();
  }
      // eliminar todos lo elementos del arbol
    int size(Node<TK> nodo,int& result){
      result = 0;
      int i = 0;
      for (; i < nodo->count; i++) {
          if (!nodo->leaf) size(nodo->children[i], result);
          result += 1;
      }
      if (!nodo->leaf) size(nodo->children[i], result);
      return result;
  }
  static BTree<int>* build_from_ordered_vector(const vector<int>& vector, int i);

  // Construya un árbol B a partir de un vector de elementos ordenados
  static BTree* build_from_ordered_vector(vector<TK> elements);
  // Verifique las propiedades de un árbol B
    bool check_properties(){
        int min_keys = (M % 2 == 0) ? (M / 2) : (M / 2);
        int leaf_level = -1;
        return check_properties(root, min_keys, 0, leaf_level);
    }
    bool check_properties(Node<TK>* node, int min_keys, int level, int& leaf_level)
    {
        if (!node) return true;
        if (node != root && node->count < min_keys) return false;
        if (node->count > M - 1) return false;
        if (node->leaf) {
            if (leaf_level == -1) leaf_level = level;
            return leaf_level == level;
        }
        for (int i = 0; i <= node->count; ++i) {
            if (!check_properties(node->children[i], min_keys, level + 1,
            leaf_level))
                return false;
        }
        return true;
    }
  ~BTree();     // liberar memoria
};

#endif
