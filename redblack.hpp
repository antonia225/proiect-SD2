#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include <iostream>
using namespace std;

class Node {
public:
    int data;
    char color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int data) {
        this->data = data;
        this->color = 'R';
        this->left = this->right = this->parent = nullptr;
    }
};

class Tree {
public:
    Node* root;

    Tree() {
        root = nullptr;
    }

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
            y->right->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void insert(int data) {
        Node* newNode = new Node(data);
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (newNode->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        newNode->parent = y;

        if (y == nullptr)
            root = newNode;
        else if (newNode->data < y->data)
            y->left = newNode;
        else
            y->right = newNode;

        insertFix(newNode);
    }

    void insertFix(Node* z) {
        while (z->parent != nullptr && z->parent->color == 'R') {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y != nullptr && y->color == 'R') {
                    z->parent->color = 'B';
                    y->color = 'B';
                    z->parent->parent->color = 'R';
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = 'B';
                    z->parent->parent->color = 'R';
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y != nullptr && y->color == 'R') {
                    z->parent->color = 'B';
                    y->color = 'B';
                    z->parent->parent->color = 'R';
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = 'B';
                    z->parent->parent->color = 'R';
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = 'B';
    }

    void deleteNode(int data) {
        Node* z = root;
        while (z != nullptr) {
            if (z->data == data)
                break;
            else if (data < z->data)
                z = z->left;
            else
                z = z->right;
        }

        if (z == nullptr) {
            cout << "Node not found!" << endl;
            return;
        }

        deleteFix(z);
    }

    void deleteFix(Node* z) {
        Node* y = z;
        Node* x;
        char yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                if (y->right != nullptr)
                    y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            if (y->left != nullptr)
                y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == 'B')
            deleteFixUp(x);
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;
    }

    Node* minimum(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    void deleteFixUp(Node* x) {
        while (x != root && (x == nullptr || x->color == 'B')) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w != nullptr && w->color == 'R') {
                    w->color = 'B';
                    x->parent->color = 'R';
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == 'B') &&
                    (w->right == nullptr || w->right->color == 'B')) {
                    w->color = 'R';
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == 'B') {
                        if (w->left != nullptr) w->left->color = 'B';
                        w->color = 'R';
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 'B';
                    if (w->right != nullptr) w->right->color = 'B';
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w != nullptr && w->color == 'R') {
                    w->color = 'B';
                    x->parent->color = 'R';
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == 'B') &&
                    (w->left == nullptr || w->left->color == 'B')) {
                    w->color = 'R';
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == 'B') {
                        if (w->right != nullptr) w->right->color = 'B';
                        w->color = 'R';
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 'B';
                    if (w->left != nullptr) w->left->color = 'B';
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        if (x != nullptr)
            x->color = 'B';
    }

    void inorder(Node* node) {
        if (node == nullptr) return;
        inorder(node->left);
        cout << node->data << "(" << node->color << ") ";
        inorder(node->right);
    }

    void display() {
        inorder(root);
        cout << endl;
    }

    bool search(int value) {
        return searchHelper(root, value) != nullptr;
    }

    Node* searchHelper(Node* node, int value) {
        if (node == nullptr || node->data == value)
            return node;
        if (value < node->data)
            return searchHelper(node->left, value);
        else
            return searchHelper(node->right, value);
    }

    int floor(int x) {
        Node* current = root;
        int result = -1;
        while (current) {
            if (current->data == x)
                return x;
            if (current->data < x) {
                result = current->data;
                current = current->right;
            } else {
                current = current->left;
            }
        }
        return result;
    }

    int ceiling(int x) {
        Node* current = root;
        int result = -1;
        while (current) {
            if (current->data == x)
                return x;
            if (current->data > x) {
                result = current->data;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return result;
    }

    void rangeQuery(int X, int Y) {
        rangeHelper(root, X, Y);
        cout << endl;
    }

    void rangeHelper(Node* node, int X, int Y) {
        if (!node) return;
        if (X < node->data)
            rangeHelper(node->left, X, Y);
        if (X <= node->data && node->data <= Y)
            cout << node->data << " ";
        if (node->data < Y)
            rangeHelper(node->right, X, Y);
    }

    void unionWith(Tree& other) {
        collectAndInsert(other.root);
    }

    void collectAndInsert(Node* node) {
        if (!node) return;
        collectAndInsert(node->left);
        if (!this->search(node->data))
            this->insert(node->data);
        collectAndInsert(node->right);
    }
};

#endif // RED_BLACK_TREE_HPP
