// myTree.h
// Chase Worsley
// COSC 2030, Prog 2
// Last Edited: 2-27-2024
// Data Structure for Prog2

#ifndef _MYBTREE_
#define _MYBTREE_
#include "treeNode.h"
#include <cctype>

/*
             Drawing for my own mental stability and for any clarification (i apologize for how convoluted my code may get)

                                                         ROOT
                                                          |
                                                        /   \
                                                       A     B 
                                                     /   \ /   \
                                                    A    B A    B 
                                                    ^      ^    ^---- This would spell 'BB'
                            This node would spell 'AA'     This node would spell 'BA'

    Pretend this is for all 26+1 for all letters of the alphabet and our good ol' apostrophe (caused me at least 3 seg faults)

    Note: there is now a "flag" called "isEOW" for each node that has been determined to be a word, for example, 'a' now has a flag that says its a word, so my .find() method will also look for this flag instead of isLeaf()
    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    EXAMPLE: Lets say we have a dictionary of four words: apple, apricot, app, and orange, the following diagram shows the tree visually:
    note that there are TECHNICALLY 27 children per node, but I only show non-null children here

                                                         ROOT
                                                          |
                                                        /   \
                                                       A     O
                                                       |     |
                                                       P     R
                                                      / \    |
          'P' is flagged as EOW (isEOW=true)-------> P   R   A
                                                     |   |   |
                                                     L   I   N
                                                     |   |   |
          'E' is flagged as EOW (isEOW=true)-------> E   C   G
                                                         |   |
                                                         O   E <-------- This 'E' is flagged as EOW, spelling "orange"
                                                         |
                                                         T <------- This 'T' is flagged as EOW, spelling "apricot"
    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

template <class T>
class myTree {
private:
    int size; // Size of tree, this is the number of nodes! not depth you silly goose (talking to myself)
    treeNode<T>* root; // Root of the whole tree
    void deleteRecur(treeNode<T>* node) { // A recursive function for the destructor, wanted to try recursives (hoping this works for very large datasets)
        if (node != nullptr) {
            if (node->isLeaf()) { // If node is a leaf, delete that thang
                delete node;
                node = nullptr;
            } else { // If not a leaf, destroy the children, Anakin Skywalker style
                for (int i = 0; i < alphLength; ++i) { 
                    deleteRecur(node->children[i]);
                }
            }
        }
    }
public:
    myTree() { // Ensures root is a nullptr, tree is size 0 (not counting root because it will contain no data)
        root = nullptr;
        size = 0;
    }
    ~myTree() { // Calls recursive function in private
        deleteRecur(root);
    }
    void insert(T item) { // Inserts words into tree
        if (root == nullptr) { // Creates new node if root/head does is empty (nullptr)
            root = new treeNode<T>();
        }
        treeNode<T>* current = root;
        for (char c : item) { // Iterates through each character
            if (c == '\'') { // Prevents segfaults, apostrophes are less than letters in ascii, therefore the 'childIndex' would be negative; you can't access a negative index
                c = int('z') + 1; // using z+1 as a pseudo apostrophe via ascii
            }
            int childIndex = c - int('a'); // index 0-26 responds to the following: [a, b, c, d, ... , z, apostrophe]
            if (current->children[childIndex] == nullptr) { // If current child being iterated over is null, make new node at said child. If childIndex = 0, we are looking for 'a', if there has been no 'a', add a new node for 'a'
                current->children[childIndex] = new treeNode<T>();
            }
            current = current->children[childIndex]; // Move to child of index, if index = 0, we move to the next 'a' node.
        }
        current->data = item; // The data of the new node is the word supplied
        current->isEOW = true; // Marks this word as a word (sounds stupid, useful for words like 'a', 'can', 'ho', etc...)
        size++; // Increments size
    }
    bool find(T item, long int& numCompares) { // Main find method, cross your fingers and hope it works!
        numCompares = 0; // Intializes number of comparisons for each word at 0
        if (root == nullptr) { // If tree is empty, nothing can be found, skadoodle
            return false;
        }
        treeNode<T>* current = root; // Start new traversal node
        for (char c : item) { // Iterate through each character in provided word
            if (c == '\'') { // If character is an apostrophe do the same ascii logic as insert, this whole thing is almost identical to insert
                c = int('z') + 1;
            } else if (std::isdigit(c)) { // Takes into account numbers in words, no dictionary word has a number so we can discard this word immeditaly, also caused me multiple segfaults ;-;
                return false;
            }
            int childIndex = c - int('a'); // ASCII to index in tree, look at .insert() for elaboration
            if (current->children[childIndex] == nullptr) { // If current child being looked at is empty, return false
                return false;
            }
            current = current->children[childIndex]; // Iterate through each child corresponding to the letter being iterated over, ex) For the word: "apple", go from 'a', to 'p'
            numCompares ++; // Increment compares
        }
        return current->isEOW; // If node ended on is verified to be a word (see insert method), return true
    }
    void remove() { // Recursion delete, didn't know what to do with this method tbh, so its the same as destructor
        deleteRecur(root); 
    }
    int getSize() { // Returns size
        return size;
    }
    bool isEmpty() { // Returns if empty
        if (size == 0) {
            return true;
        }
        return false;
    }
};


#endif