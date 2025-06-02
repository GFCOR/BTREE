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
    for (int i = 0; i < curr->count; i++) {
     if (curr->keys[i] > key) {
      break;
     }
    }
    curr = curr->children[i];
    s.push(curr);
   }
   while (!inserted) {
    curr->insert(key);
    if (curr->count < M) {
     inserted = true;
    }
    else {
     Node<TK>* newNode = new Node<TK>(M);
     for (int i = 0; i < curr->count/2; i++) {
      newNode->keys[i] = curr->keys[i];
      newNode->count = newNode->count + 1;
     }
     Node<TK>* parent = new Node<TK>(M);
     for (int i = curr->count+1; i < curr->count; i++) {
      parent->keys[i] = curr->keys[i];
      parent->count = parent->count + 1;
     }
     key = curr->keys[curr->count/2];
     s.pop();
     curr = s.top();
    }
   }
  }
   //inserta un elemento
  void remove(TK key);//elimina un elemento
  int height();//altura del arbol. Considerar altura 0 para arbol vacio
  string toString(const string& sep);  // recorrido inorder
  vector<TK> rangeSearch(TK begin, TK end);

  TK minKey();  // minimo valor de la llave en el arbol
  TK maxKey();  // maximo valor de la llave en el arbol
  void clear(); // eliminar todos lo elementos del arbol
  int size(); // retorna el total de elementos insertados  
  
  // Construya un árbol B a partir de un vector de elementos ordenados
  static BTree* build_from_ordered_vector(vector<TK> elements);
  // Verifique las propiedades de un árbol B
  bool check_properties();

  ~BTree();     // liberar memoria
};

#endif
