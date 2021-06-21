/*
INSTRUCTIONS
In this assignment, it is required that you fill out areas under comments labeled as "TODO" appropriately based on the accompanying directions. 
You are also required to follow any directions accompanying comments such as "NOTE".
You can add/modify code anywhere, with the exception of the provided "main" (which we will use for testing).
You can use the constants RED and BLACK, instead of the ints 0 and 1, when appropriate.
*/

#include <iostream>
#include <math.h> // for asserting height
#include <queue>

using namespace std;

#define RED 0
#define BLACK 1

template <class T>
class RBT;

// swapColor swaps the color from red to black and vice versa
int swapColor(int c) {
    return (c==0)?1:0;
}

template <class T>
class RBTNode {
    RBTNode<T> *parent, *left, *right;
    T data;
    int color;

public:
    RBTNode(T data)
            : data(data),
              color(RED),
              parent(nullptr),
              left(nullptr),
              right(nullptr){}

    friend class RBT<T>;
    void prettyPrint(int indent) const;

    template <class T1>
    friend void swapColor(RBTNode<T1> *);
    template <class T1>
    friend int getColor(RBTNode<T1> *);


    int height() const;
};

template <class T>
int RBTNode<T>::height() const {
    int left_h = 0;
    if (left != nullptr) {
        left_h = left->height();
    }
    int right_h = 0;
    if (right != nullptr) {
        right_h = right->height();
    }
    return 1 + max(left_h, right_h);
}

template <class T>
void RBTNode<T>::prettyPrint(int indent) const {
    if (right != nullptr) {
        right->prettyPrint(indent + 1);
    }
    int margin = indent * 2;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "DATA: " << data << endl;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "COLOR: " << (color == RED ? "RED" : "BLACK") << endl;
    if (left != nullptr) {
        left->prettyPrint(indent + 1);
    }
}

template <class T>
void swapColor(RBTNode<T> *node) {
    if (node != nullptr) {
        node->color = swapColor(node->color);
    }
}

// getColor handles null nodes
template <class T>
int getColor(RBTNode<T> *node) {
    if (node != nullptr) {
        return node->color;
    }
    return BLACK;
}

template <class T>
class RBT {
    RBTNode<T> *root;
    void singleCCR(RBTNode<T> *&point);
    void doubleCR(RBTNode<T> *&point);
    void singleCR(RBTNode<T> *&point);
    void doubleCCR(RBTNode<T> *&point);

public:
    RBT() : root(nullptr){}
    void insert(const T &);
    void insert(const T &, RBTNode<T> *&point, RBTNode<T> *parent);
    void prettyPrint() const { root->prettyPrint(0); }

    int height() const { return root->height(); }
};

template <class T>
void RBT<T>::doubleCCR(RBTNode<T> *&point) {
    singleCR(point->right);
    singleCCR(point);
}

template <class T>
void RBT<T>::doubleCR(RBTNode<T> *&point) {
    singleCCR(point->left);
    singleCR(point);
}

template <class T>
void RBT<T>::singleCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;  //x
    RBTNode<T> *parent = point->left; //y = x.left

    // TODO: ADD ROTATION CODE HERE -- OK DONE
    // RE-ORDER RELATIONS
    // turn parent's right subtree into gp's left subtree
    grandparent->left = parent->right; // x.left = y.right
    if (parent->right != nullptr)
        parent->right->parent = grandparent;
    parent->parent = grandparent->parent;
    if (grandparent->parent == nullptr)
        root = parent;
    else if(grandparent == grandparent->parent->left)
        grandparent->parent->left = parent;
    else
        grandparent->parent->right = parent;
    parent->right = grandparent;
    grandparent->parent = parent;

    //Swap color of parent and grandparent
    swapColor(parent);
    swapColor(grandparent);

}

template <class T>
void RBT<T>::singleCCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;
    RBTNode<T> *parent = point->right;
    // TODO: ADD ROTATION CODE HERE -- OK DONE
    grandparent->right = parent->left;
    if (parent->left != nullptr)
        parent->left->parent = grandparent;
    parent->parent = grandparent->parent;
    if (grandparent->parent == nullptr)
        root = parent;
    else if(grandparent == grandparent->parent->left)
        grandparent->parent->left = parent;
    else
        grandparent->parent->right = parent;
    parent->left = grandparent;
    grandparent->parent = parent;

    //Swap color of parent and grandparent
    swapColor(parent);
    swapColor(grandparent);

}

template <class T>
void RBT<T>::insert(const T &toInsert, RBTNode<T> *&point, RBTNode<T> *parent) {
    RBTNode<T>* point_copy;
    if (point == nullptr) {               // leaf location is found so insert node
        point = new RBTNode<T>(toInsert); // modifies the pointer itself since *point
        // is passed by reference
        point->parent = parent;

        RBTNode<T> *curr_node = point; // curr_node will be set appropriately when walking up the tree
        // TODO: ADD RBT RULES HERE -- OK DONE

        // we derive spatial relationship between grandparent and grandchild by comparing curr_node to point
        // point cannot be used to climb tree because prettyPrint uses it, so we make a copy
        RBTNode<T>* point_copy = point;
        while (point_copy != root) {
            if (curr_node->parent != nullptr)
                curr_node = curr_node->parent;
            if (curr_node->parent != nullptr)
                curr_node = curr_node->parent;
            if (curr_node->parent == nullptr)
                root = curr_node;

            // if new node is outside - single rotation (2 cases)
            // case 1: left/left
            if (curr_node->left != nullptr && curr_node->left->left != nullptr && point_copy == curr_node->left->left) {
                while (getColor(curr_node->left) == RED && getColor(curr_node->left->left) == RED) {
                    if (getColor(curr_node->right) == BLACK)  // uncle is black: single rotation CR
                        singleCR(curr_node);
                    else { // uncle is red: flip uncle and parent to black and grandparent to red
                        swapColor(curr_node->right);
                        swapColor(curr_node->left);
                        swapColor(curr_node);
                    }
                }
            }
                // case 2: right/right
            else if (curr_node->right != nullptr && curr_node->right->right != nullptr &&
                     point_copy == curr_node->right->right) {
                while (getColor(curr_node->right) == RED && getColor(curr_node->right->right) == RED) {
                    if (getColor(curr_node->left) == BLACK) // uncle is black: single rotation CCR
                        singleCCR(curr_node);
                    else {
                        swapColor(curr_node->right);
                        swapColor(curr_node->left);
                        swapColor(curr_node);
                    }
                }
            }
                // if new node is inside - double rotation (2 cases)
                // case 3: left/right
            else if (curr_node->left != nullptr && curr_node->left->right != nullptr &&
                     point_copy == curr_node->left->right) {
                while (getColor(curr_node->left) == RED && getColor(curr_node->left->right) == RED) {
                    if (getColor(curr_node->right) == BLACK)
                        doubleCR(curr_node);
                    else {
                        swapColor(curr_node->right);
                        swapColor(curr_node->left);
                        swapColor(curr_node);
                    }
                }
            }
                // case 4: right/left
            else if (curr_node->right != nullptr && curr_node->right->left != nullptr &&
                     point_copy == curr_node->right->left) {
                while (getColor(curr_node->right) == RED && getColor(curr_node->right->left) == RED) {
                    if (getColor(curr_node->left) == BLACK)
                        doubleCCR(curr_node);
                    else {
                        swapColor(curr_node->right);
                        swapColor(curr_node->left);
                        swapColor(curr_node);
                    }
                }
            }
            if (root->color == RED)
                swapColor(root);

            if (point_copy->parent != nullptr) {
                point_copy = point_copy->parent;
            }

            if (point_copy->parent != nullptr) {
                point_copy = point_copy->parent;
            }
        }

    } else if (toInsert < point->data) { // recurse down the tree to left to find correct leaf location
        insert(toInsert, point->left, point);
    } else { // recurse down the tree to right to find correct leaf location
        insert(toInsert, point->right, point);
    }
}

template <class T>
void RBT<T>::insert(const T &toInsert) {
    insert(toInsert, root, nullptr);
}

// NOTE: DO NOT MODIFY THE MAIN FUNCTION BELOW
int main() {
    RBT<int> b;
    int count = 10;
    for (int i = 0; i < count; i++) {
        b.insert(i);
    }

    b.prettyPrint();
    /* EXPECTED OUTPUT:
                                                                    Data: 9
                                                                    COLOR: RED
                                                    Data: 8
                                                    COLOR: BLACK
                                    Data: 7
                                    COLOR: RED
                                                    Data: 6
                                                    COLOR: BLACK
                    Data: 5
                    COLOR: BLACK
                                    Data: 4
                                    COLOR: BLACK
    Data: 3
    COLOR: BLACK
                                    Data: 2
                                    COLOR: BLACK
                    Data: 1
                    COLOR: BLACK
                                    Data: 0
                                    COLOR: BLACK
    */

    // TEST
    // the below tests the validity of the height of the RBT
    // if the assertion fails, then your tree does not properly self-balance
    int height = b.height();
    assert(height <= 2 * log2(count));
}