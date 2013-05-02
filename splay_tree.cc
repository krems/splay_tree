#include <stdlib.h>
#include <vector>
#include <stack>

template <typename T>
struct Node {
  T key;
  Node<T>* left;
  Node<T>* right;
  Node<T>* parent;
  Node(T key_) : key(key_), left(0), right(0), parent(0) {}
  Node(T key_, Node<T>* parent_) : key(key_), left(0), right(0),
                                   parent(parent_) {}
};

template <typename T>
int compare(Node<T>* lhs, T rhs) {
  if (lhs->key < rhs) {
    return -1;
  }
  if (rhs < lhs->key) {
    return 1;
  }
  return 0;
}

template <typename T>
class SplayTree {
  Node<T>* root;
 public:
  SplayTree(T key) {
    root = new Node<T>(key);
  }
  
  SplayTree() : SplayTree(0) {};

  template <size_t N>
  SplayTree(T (&keys)[N]) : SplayTree(keys[0]) {
    for (size_t i = 1; i < N; ++i) {
      this->add(keys[i]);
    }
  }

  SplayTree(const std::vector<T>& keys) : SplayTree(keys[0]) {
    for (auto it = keys.cbegin() + 1; it != keys.cend(); ++it) {
      this->add(keys[i]);
    }
  }

  Node<T>* add(T key) {
    add(key, root);
    return root;
  }

  Node<T>* remove(T key) {
    Node<T>* removed = find(key);
    if (!removed) {
      throw std::exception;
    }
    remove(removed);
    return root;
  }

  Node<T>* find(T key) {
    return find(key, root);
  }

  ~SplayTree() {
    std::stack<Node<T>*> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
      Node<T>* node = nodes.top();
      if (node->left) {
        nodes.push(node->left);
        node->left = 0;
        continue;
      }
      if (node->right) {
        nodes.push(node->right);
        node->right = 0;
        continue;
      }
      nodes.pop();
      delete node;
    }
  }
  
 private:
  void add(T key, Node<T>* node) {
    while (node) {
      switch (compare(node, key)) {
        case 0:
          splay(node);
          return;
        case -1:
          if (!node->left) {
            node->left = new Node<T>(key, node);
            splay(node->left);
            return;
          }
          node = node->left;
          break;
        case 1:
          if (!node->right) {
            node->right = new Node<T>(key, node);
            splay(node->right);
            return;
          }
          node = node->right;
    }
    splay(node);
  }

  void remove(Node<T>* node) {
    // todo: null chek
    splay(node);
    Node<T>* lhs = node->left;
    Node<T>* rhs = node->right;
    lhs->parent = 0;
    rhs->parent = 0;
    delete root;
    Node<T>* newRoot = findMax(lhs);
    splay(newRoot);
    newRoot->right = rhs;
  }

  Node<T>* find(T key, Node<T>* node) {
    while (node) {
      switch (compare(node, key)) {
        case 0:
          return node;
        case -1:
          if (!node->left) {
            return 0;
          }
          node = node->left;
          break;
        case 1:
          if (!node->right) {
            return 0;
          }
          node = node->right;
      }
    }
    return 0;
  }
  
  Node<T>* find_recursive(T key, Node<T>* node) {
    if (!node) {
      return 0;
    }
    switch (compare(node, key)) {
      case 0:
        return node;
      case -1:
        return find(key, node->left);
      case 1:
        return find(key, node->right);
    }
  }
  
  void splay(Node<T>* node) {
    while (node->parent) {
      if (!node->parent->parent) { // ZIG
        if (node->parent->left == node) {
          node = rotateRight(node->parent);
        } else {
          node = rotateLeft(node->parent);
        }
        break;
      }
      if (node->parent->left == node) {
        if (node->parent->parent->left == node->parent) { // ZIG-ZIG
          node->parent = rotateRight(node->parent->parent);
          node = rotateRight(node->parent);
        } else { // ZIG-ZAG
          node = rotateRight(node->parent);
          node = rotateLeft(node->parent);
        }
        continue;
      }
      if (node->parent->parent->right == node->parent) { // ZIG-ZIG
        node->parent = rotateRight(node->parent->parent);
        node = rotateRight(node->parent);
      } else { // ZIG-ZAG
        node = rotateLeft(node->parent);
        node = rotateRight(node->parent);
      }
    }
  }
  
  Node<T>* rotateRight(Node<T>* node) {
    Node<T>* up = node->left;
    if (node->parent) {
      if (node->parent->left == node) {
        node->parent->left = up;
      } else {
        node->parent->right = up;
      }
    }
    up->parent = node->parent;
    node->left = up->right;
    up->right->parent = node;
    up->right = node;
    node->parent = up;
    return up;
  }

  Node<T>* rotateLeft(Node<T>* node) {
    Node<T>* up = node->right;
    if (node->parent) {
      if (node->parent->right == node) {
        node->parent->right = up;
      } else {
        node->parent->left = up;
      }
    }
    up->parent = node->parent;
    node->right = up->left;
    up->left->parent = node;
    up->left = node;
    node->parent= up;
    return up;
  }

  Node<T>* findMax(Node<T>* node) {
    if (node->right) {
      return findMax(node->right);
    }
    return node;
  }
};

int main() {
  return 0;
}
