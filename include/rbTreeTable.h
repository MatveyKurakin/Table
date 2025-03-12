#pragma once
#include "binTreeTable.h"

template <typename TypeKey, typename TypeData>
class RbTreeTable : public BinTreeTable<TypeKey, TypeData> {
 private:
  inline bool HasGrandparent(Node<TypeKey, TypeData>* node) {
    if (node->parent == 0)
      return false;
    else if (node->parent->parent == 0)
      return false;
    return true;
  }

  inline bool HasUncle(Node<TypeKey, TypeData>* node) {
    if (!HasGrandparent(node)) return false;
    if (node->parent->parent->left == node->parent) {
      if (!HasRightChild(node->parent->parent)) return false;
    } else {
      if (!HasLeftChild(node->parent->parent)) return false;
    }
    return true;
  }

  Node<TypeKey, TypeData>* Uncle(Node<TypeKey, TypeData>* node) {
    if (node->parent->parent->left == node->parent) {
      if (HasRightChild(node->parent->parent)) return node->parent->parent->right;
    } else {
      if (HasLeftChild(node->parent->parent)) return node->parent->parent->left;
    }
  }

  Node<TypeKey, TypeData>* Grandparent(Node<TypeKey, TypeData>* node) { return node->parent->parent; }

  void Repaint(Node<TypeKey, TypeData>* node) {
    node->parent->colour = black;
    node->parent->parent->colour = red;
    Uncle(node)->colour = black;
    rebalance(node->parent->parent);
  }

  bool Check(Node<TypeKey, TypeData>* node) {
    if (node->colour == red) {
      if (HasParent(node)) {
        if (node->parent->colour == red) return true;
      }
      if (HasLeftChild(node)) {
        if (node->left->colour == red) return true;
      }
      if (HasRightChild(node)) {
        if (node->right->colour == red) return true;
      }
      return false;
    }
    return false;
  }

  void smallRight(Node<TypeKey, TypeData>* a) {
    Node<TypeKey, TypeData>* b = a->left;
    if (HasParent(a))
      if (a->parent->right == a)
        a->parent->right = b;
      else
        a->parent->left = b;
    else
      root = b;
    b->parent = a->parent;
    if (HasRightChild(b)) b->right->parent = a;
    a->left = b->right;
    b->right = a;
    a->parent = b;
  }
  void smallLeft(Node<TypeKey, TypeData>* a) {
    Node<TypeKey, TypeData>* b = a->right;
    if (HasParent(a))
      if (a->parent->right == a)
        a->parent->right = b;
      else
        a->parent->left = b;
    else
      root = b;
    b->parent = a->parent;
    if (HasLeftChild(b)) b->left->parent = a;
    a->right = b->left;
    b->left = a;
    a->parent = b;
  }
  void bigRight(Node<TypeKey, TypeData>* a) {
    smallRight(a);
    a->colour = red;
    a->parent->colour = black;
  }
  void bigLeft(Node<TypeKey, TypeData>* a) {
    smallLeft(a);
    a->colour = red;
    a->parent->colour = black;
  }

  void rebalance(Node<TypeKey, TypeData>* node) {
    if (node == root) {
      node->colour = black;
      return;
    }

    else if (!Check(node))
      return;

    if (HasUncle(node) && Uncle(node)->colour == red)
      Repaint(node);
    else {
      if (HasGrandparent(node) && node->parent->right == node && Grandparent(node)->left == node->parent) {
        smallLeft(node->parent);
        node = node->left;
      } else if (HasGrandparent(node) && node->parent->left == node && Grandparent(node)->right == node->parent) {
        smallRight(node->parent);
        node = node->right;
      }

      if (HasGrandparent(node)) {
        if (Grandparent(node)->left == node->parent) {
          if (node->parent->left == node)
            bigRight(Grandparent(node));
          else
            return;
        } else if ((Grandparent(node)->right == node->parent)) {
          if (node->parent->right == node)
            bigLeft(Grandparent(node));
          else
            return;
        }
      } else
        return;
    }
  }

 public:
  RbTreeTable() { root = 0; }

  RbTreeTable(const RbTreeTable& other) {
    if (other.root == 0) {
      root = 0;
    } else {
      root = new Node<TypeKey, TypeData>();
      root->parent = 0;
      root->left = 0;
      root->right = 0;
      root->colour = black;
      root->storage = other.root->storage;
      root->height = other.root->height;
      Node<TypeKey, TypeData>* n1 = root;
      Node<TypeKey, TypeData>* n2 = root;
      Node<TypeKey, TypeData>* other_n1 = other.root;
      Node<TypeKey, TypeData>* other_n2 = other.root;
      while (n2 != 0) {
        n1 = n2;
        other_n1 = other_n2;
        if (HasLeftChild(other_n1) && !HasLeftChild(n1)) {
          other_n2 = other_n1->left;
          n2 = new Node<TypeKey, TypeData>();
          n2->parent = n1;
          n2->left = 0;
          n2->right = 0;
          n2->storage = other_n2->storage;
          n2->height = other_n2->height;
          n2->colour = other_n2->colour;
          n1->left = n2;
        } else if (HasRightChild(other_n1) && !HasRightChild(n1)) {
          other_n2 = other_n1->right;
          n2 = new Node<TypeKey, TypeData>();
          n2->parent = n1;
          n2->left = 0;
          n2->right = 0;
          n2->storage = other_n2->storage;
          n2->height = other_n2->height;
          n2->colour = other_n2->colour;
          n1->right = n2;
        } else {
          n2 = n1->parent;
          other_n2 = other_n1->parent;
        }
      }
    }
  }

  BinTreeTable& operator=(const BinTreeTable& other) {
    if (&other != this) {
      if (root == 0) {
        if (other.root == 0) {
          root = 0;
        } else {
          root = new Node<TypeKey, TypeData>();
          root->parent = 0;
          root->left = 0;
          root->right = 0;
          root->colour = black;
          root->storage = other.root->storage;
          root->height = other.root->height;
          Node<TypeKey, TypeData>* n1 = root;
          Node<TypeKey, TypeData>* n2 = root;
          Node<TypeKey, TypeData>* other_n1 = other.root;
          Node<TypeKey, TypeData>* other_n2 = other.root;
          while (n2 != 0) {
            n1 = n2;
            other_n1 = other_n2;
            if (HasLeftChild(other_n1) && !HasLeftChild(n1)) {
              other_n2 = other_n1->left;
              n2 = new Node<TypeKey, TypeData>();
              n2->parent = n1;
              n2->left = 0;
              n2->right = 0;
              n2->storage = other_n2->storage;
              n2->height = other_n2->height;
              n2->colour = other_n2->colour;
              n1->left = n2;
            } else if (HasRightChild(other_n1) && !HasRightChild(n1)) {
              other_n2 = other_n1->right;
              n2 = new Node<TypeKey, TypeData>();
              n2->parent = n1;
              n2->left = 0;
              n2->right = 0;
              n2->storage = other_n2->storage;
              n2->height = other_n2->height;
              n2->colour = other_n2->colour;
              n1->right = n2;
            } else {
              n2 = n1->parent;
              other_n2 = other_n1->parent;
            }
          }
        }
      } else {
        if (other.root == 0) {
          Node<TypeKey, TypeData>* n1 = root;
          Node<TypeKey, TypeData>* n2 = root;
          while (n2 != NULL) {
            n1 = n2;
            if (n1->left != 0) {
              n2 = n1->left;
              n1->left = 0;
            } else if (n2->right != 0) {
              n2 = n1->right;
              n1->right = 0;
            } else {
              n2 = n1->parent;
              delete n1;
            }
          }
          delete n1;
          delete n2;
          root = 0;
        } else {
          root->storage = other.root->storage;
          Node<TypeKey, TypeData>* n1 = root;
          Node<TypeKey, TypeData>* n2 = root;
          Node<TypeKey, TypeData>* other_n1 = other.root;
          Node<TypeKey, TypeData>* other_n2 = other.root;
          while (n2 != 0) {
            n1 = n2;
            other_n1 = other_n2;
            if (HasLeftChild(other_n1) && !HasLeftChild(n1)) {
              other_n2 = other_n1->left;
              n2 = new Node<TypeKey, TypeData>();
              n2->parent = n1;
              n2->left = 0;
              n2->right = 0;
              n2->storage = other_n2->storage;
              n2->height = other_n2->height;
              n2->colour = other_n2->colour;
              n1->left = n2;
            } else if (HasRightChild(other_n1) && !HasRightChild(n1)) {
              other_n2 = other_n1->right;
              n2 = new Node<TypeKey, TypeData>();
              n2->parent = n1;
              n2->left = 0;
              n2->right = 0;
              n2->storage = other_n2->storage;
              n2->height = other_n2->height;
              n2->colour = other_n2->colour;
              n1->right = n2;
            } else if (!HasLeftChild(other_n1) && HasLeftChild(n1)) {
              Node<TypeKey, TypeData>* n_n1 = n1->left;
              Node<TypeKey, TypeData>* n_n2 = n1->left;
              while (n_n2 != NULL) {
                n_n1 = n_n2;
                if (n_n1->left != 0) {
                  n_n2 = n_n1->left;
                  n_n1->left = 0;
                } else if (n_n2->right != 0) {
                  n_n2 = n_n1->right;
                  n_n1->right = 0;
                } else {
                  n_n2 = n_n1->parent;
                  delete n_n1;
                }
              }
              delete n_n1;
              delete n_n2;
              n1->left = 0;
            } else if (!HasRightChild(other_n1) && HasRightChild(n1)) {
              Node<TypeKey, TypeData>* n_n1 = n1->right;
              Node<TypeKey, TypeData>* n_n2 = n1->right;
              while (n_n2 != NULL) {
                n_n1 = n_n2;
                if (n_n1->left != 0) {
                  n_n2 = n_n1->left;
                  n_n1->left = 0;
                } else if (n_n2->right != 0) {
                  n_n2 = n_n1->right;
                  n_n1->right = 0;
                } else {
                  n_n2 = n_n1->parent;
                  delete n_n1;
                }
              }
              delete n_n1;
              delete n_n2;
              n1->right = 0;
            } else {
              n2 = n1->parent;
              other_n2 = other_n1->parent;
            }
          }
        }
      }
    }
    return *this;
  }

  binTreeIterator<TypeKey, TypeData> insert(const TypeKey& key, const TypeData& d) {
    if (root == 0) {
      root = new Node<TypeKey, TypeData>();
      root->parent = 0;
      root->left = 0;
      root->right = 0;
      root->storage = make_pair(key, d);
      root->height = 0;
      root->colour = black;
      return binTreeIterator<TypeKey, TypeData>(*root, *this);
    } else {
      Node<TypeKey, TypeData>* n1 = root;
      Node<TypeKey, TypeData>* n2 = 0;
      while (n1 != 0 && n1->storage.first != key) {
        n2 = n1;
        n1->storage.first < key ? n1 = n1->right : n1 = n1->left;
      }
      if (n1 == 0) {
        n1 = new Node<TypeKey, TypeData>();
        n1->storage = make_pair(key, d);
        n1->left = 0;
        n1->right = 0;
        n1->parent = n2;
        n1->height = 0;
        n1->colour = red;
        if (n2->storage.first < n1->storage.first)
          n2->right = n1;
        else
          n2->left = n1;
        rebalance(n1);
      } else if (n1->storage.first == key) {
        n1->storage.second = d;
      }
      return binTreeIterator<TypeKey, TypeData>(*n1, *this);
    }
  }

  bool remove(const TypeKey& key) {
    Node<TypeKey, TypeData>* DeleteNode = root;
    while (DeleteNode != 0 && DeleteNode->storage.first != key)
      DeleteNode->storage.first < key ? DeleteNode = DeleteNode->right : DeleteNode = DeleteNode->left;
    if (DeleteNode == 0)
      return false;
    else if (DeleteNode->storage.first == key) {
      Node<TypeKey, TypeData>* tmp = 0;
      if (HasRightChild(DeleteNode)) {
        tmp = DeleteNode->right;
        while (HasLeftChild(tmp)) tmp = tmp->left;

        Node<TypeKey, TypeData>* saveNode = 0;
        if (DeleteNode->right == tmp) {
          if (HasLeftChild(DeleteNode)) {
            DeleteNode->left->parent = tmp;
          }
        } else {
          tmp->parent->left = tmp->right;
          if (HasRightChild(tmp)) {
            tmp->right->parent = tmp->parent;
            saveNode = tmp->right;
          }
          if (HasLeftChild(DeleteNode)) {
            DeleteNode->left->parent = tmp;
          }
          tmp->right = DeleteNode->right;
          if (HasRightChild(DeleteNode)) {
            DeleteNode->right->parent = tmp;
          }
        }
        if (DeleteNode == root) root = tmp;
        if (HasParent(DeleteNode)) {
          if (DeleteNode->parent->left == DeleteNode)
            DeleteNode->parent->left = tmp;
          else if (DeleteNode->parent->right == DeleteNode)
            DeleteNode->parent->right = tmp;
        }
        tmp->parent = DeleteNode->parent;
        tmp->left = DeleteNode->left;
        tmp->height = DeleteNode->height;
        tmp->colour = DeleteNode->colour;
        if (saveNode != 0)
          rebalance(saveNode);
        else
          rebalance(tmp);
        DeleteNode = 0;
      } else if (HasLeftChild(DeleteNode)) {
        tmp = DeleteNode->left;
        tmp->parent = DeleteNode->parent;
        if (HasParent(DeleteNode)) {
          if (DeleteNode->parent->left == DeleteNode)
            DeleteNode->parent->left = tmp;
          else if (DeleteNode->parent->right == DeleteNode)
            DeleteNode->parent->right = tmp;
        }
        tmp->height = DeleteNode->height;
        tmp->colour = DeleteNode->colour;
        if (DeleteNode == root) root = tmp;
        rebalance(tmp);
        DeleteNode = 0;
      } else {
        if (HasParent(DeleteNode) && DeleteNode->parent->right == DeleteNode)
          DeleteNode->parent->right = 0;
        else if (HasParent(DeleteNode) && DeleteNode->parent->left == DeleteNode)
          DeleteNode->parent->left = 0;
        if (DeleteNode == root) root = 0;
        DeleteNode = 0;
      }
      return true;
    }
  }

  int getHeight() {
    if (root == 0) return -1;
    int h = 0;
    int max_h = 0;
    Node<TypeKey, TypeData>* n = root;
    while (HasLeftChild(n)) {
      n = n->left;
      if (n->colour == black) h++;
    }
    if (h > max_h) max_h = h;
    while (true) {
      Node<TypeKey, TypeData>* it_node = n;
      if (HasRightChild(it_node)) {
        it_node = it_node->right;
        if (it_node->colour == black) h++;
        while (HasLeftChild(it_node)) {
          it_node = it_node->left;
          if (it_node->colour == black) h++;
        }
      } else {
        Node<TypeKey, TypeData>* save_node = it_node;
        while (HasParent(it_node) && it_node->parent->right == it_node) {
          if (it_node->colour == black) h--;
          it_node = it_node->parent;
        }
        if (!HasParent(it_node)) {
          it_node = save_node;
          break;
        } else if (it_node->parent->left == it_node) {
          if (it_node->colour == black) h--;
          it_node = it_node->parent;
        }
      }
      n = it_node;
      if (h > max_h) max_h = h;
    }
    return max_h;
  }
};
