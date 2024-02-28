// treeNode.h
// Chase Worsley
// COSC 2030, Prog 2
// Last Edited: 2-27-2024
// Nodes for Tree for Prog2

// This was inspired by my attempt at the linked list problem, psuedo-but-also-not-psuedo-alphabetizes the dictionary, see myTree.h for further explanation/breakdown

#ifndef _MYTREENODE_
#define _MYTREENODE_

#define alphLength 27 // Alphabet length + apostrophe, this means 27 children for each node, one for each letter and the apostrophe

template <class T>
class treeNode {
public:
    T data;
    treeNode<T>* children[alphLength]; // Array of nodes for children (each letter)
    bool isEOW; // Is end of word, used for smaller words like 'ho' which are components of other large ones, needed a lot
    treeNode() {
        for (int i = 0; i < alphLength; i++) { // Ensures all children are nullptr's, segfaults are no fun :(
            children[i] = nullptr;
        }
        isEOW = false; // Ensures that only nodes that have been determined to be words are correctly assigned true, otherwise false
    }
    ~treeNode() { // Destroys all children (Warning: don't use on actual children)
        for (int i = 0; i < alphLength; i++) { 
            if (children[i] != nullptr){
                delete children[i];
            } 
        }
    }
    treeNode(T newData) { // Makes new data the data of the node, again ensures all children are nullptr's, i have had too many segfaults to not put this in here
        data = newData;
        for (int i = 0; i < alphLength; i++) {
            children[i] = nullptr;
        }
    }
    bool isLeaf() { // Just returns if the node is a leaf (childless)
        for (int i = 0; i < alphLength; i++) {
            if (children[i] != nullptr) { // If ANY child is not a nullptr, returns false
                return false;
            }
        }
        return true;
    }
};

#endif