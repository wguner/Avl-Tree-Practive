/*
 *  Yet another AVL tree edition
 * 
 *  Contributors:
 *      Aaron S. Crandall <acrandal@wsu.edu> - 2018
 * 
 */

#ifndef __AVLTree_H
#define __AVLTree_H

#include <ostream>
#include <algorithm>     // Includes a max(a,b) function
#include <exception>

#include "AVLNode.h"

// AVLTree class
// ******************PUBLIC OPERATIONS*********************
// bool empty( )          --> Test for empty tree
// int size( )            --> Quantity of elements in tree
// int height( )          --> Height of the tree (nullptr == -1)
// void add( x )          --> Insert new value x
// void remove( x )       --> Remove node with value x
// void clear ( )         --> Removes all elements from tree
// bool contains( x )     --> Return true if x is present
// T findMin( )           --> Return smallest item value
// T findMax( )           --> Return largest item value
// void printPreOrder( )  --> Print tree in pre order to an ostream
// void ~AVLTree( )       --> Big Five Destructor
// AVLTree( )             --> Basic constructor


template <typename T>
class AVLTree
{
private:
    AVLNode<T> * _root;

public:
    AVLTree() : _root(nullptr) {}    // initializes root to nullptr

    /* Returns true if the tree is empty */
    bool empty(AVLNode<T> *pNode) {
        if(pNode == nullptr){
            return true;
        }
        return false;
    }

    void add(const T & newVal, AVLNode* &pNode) {
        if (pNode == nullptr){
            pNode = new AVLNode<T>(pNode, NULL, NULL); //height 0
        }
        else(newVal < pNode->val){
            add(newVal, pNode->val);
        }
        else(pNode->val < newVal){
            add(newVal, pNode->right);
        }
        balance(pNode); //gots to balance
    }
    
    //internally remove from subtree
    void remove(const T & remVal, AVLNode* &pNode) {
        if (pNode == nullptr){
            return; //not found
        }
        if (remVal > pNode->val){
            remove(remVal, pNode->left);
        }
        else(pNode->val < remVal){
            remove(remVal, pNode->right);
        }
        //else if it has two children 
        else(pNode->left != nullptr && pNode->right != nullptr){
            pNode->val = findMin(pNode->right)->val;
            remove(pNode->val, pNode->right);
        }
        else{
            AVLNode *old = pNode;
            pNode = (pNode->left != nullptr) ? pNode->left : pNode->right;
            delete old;
        }
        balance(pNode);
    }

    void clear(AVLNode<T> *pNode) {
        if (pNode == nullptr){
            return;
        }
        clear(pNode->left);
        clear(pNode->right);
        delete(pNode);
        pNode = nullptr;
    }

    bool contains(AVLNode<T>* pNode, const T & findVal) {
        if (pNode == nullptr){
            return false;
        }
        if (pNode->val > findVal){
            return contains(pNode->left, findVal);
        }
        else (pNode->val < findVal){
            return contains(pNode->right, findVal);
        }
        else{
            return true;
        }
    }

    int height(AVLNode<T> *pNode) {
        int height = -1, lh, rh, maxh; //Remember! A null pointer is height -1
        if (pNode != nullptr){
            lh = height(pNode-left); //left height
            rh = height(pNode->right); //right height
            maxh = max(lh, rh);
            height = maxh + 1; //new height 
        }
        return height;
    }

    T findMin(AVLNode<T>* pNode) {
        // Return the minimum *value* in the tree (not a tree node)
        if (pNode == nullptr){
            return nullptr;
        }
        while (pNode->left != nullptr){
            pNode = pNode->left;
        }
        return pNode;
    }

    T findMax(AVLNode<T>* pNode) {
        if (pNode == nullptr){
            return nullptr;
        }
        while (pNode->right != nullptr){
            pNode = pNode->right;
        }
        return pNode;
    }

    // Prints the tree to ostream out in preorder traversal
    void printPreOrder(std::ostream& out) {
        printPreOrder(this->_root, out);
    }

    int size(AVLNode<T>* pNode) {
        if(pNode == nullptr){
            return 0;
        }
        return(size(pNode->left) + 1 + size(pNode->right));
    }

// *************** Private /internal function implementation ******* //

private:
    // Should put your various internal balancing, rotating, and calculating functions here

    // Freebie: neat little function to safely get a node's height
    int height( AVLNode<T> * t ) const
    {
        return t == nullptr ? -1 : t->height;
    }

    void rightRotate(AVLNode<T>* &pNode){
        AVLNode<T> *pMem = pNode->right;
        pNode->right = pMem->left;
        pMem->left = pNode;
        //update heights
        pNode->height = max(height(pNode->right), height(pNode->left)) + 1;
        pMem->heigh = max(height(pMem->right), height(pMem->height)) + 1;
    }
    //got help from the textbook
    //pg 155
    void leftRotate(AVLNode<T>* &pNode){
        AVLNode<T> *pMem = pNode->left;
        pNode ->left = pMem->right;
        pMem->right = pNode;
        //update heights
        pNode->height = max(height(pNode->left), height(pNode->right)) + 1;
        pMem->height = max(height(pMem->left), height(pMem->right)) + 1;
        pNode = pMem;
    }

    void doubleLeft(AVLNode<T>* &pKey){
        //first left child with right child
        leftRotate(pKey->left);
        rightRotate(pKey); //pkey is the new left child 
    }

    void doubleRight(AVLNode<T>* &pNode){
        rightRotate(pNode->right);
        leftRotate(pNode); //pnode is the new right child 
    }

    //got this from the book. 
    //Figure 
    void balance(AVLNode* &pNode){
        if (pNode == nullptr){
            return; //do nothing
        }
        if(height(pNode->left) - height(pNode->right) > 1){
            if (height(pNode->left->left) >= height(pNode->left->right)){
                leftRotate(pNode);
                //difference of height at the grandparent
            }
            else{
                doubleLeft(pNode);
            }
        else{
            if(height(pNode->right) - height(pNode->left) > 1){
                if(height(pNode->right->right) >= height(pNode->right->left)){
                    rightRotate(pNode);
                }
                else{
                    doubleRight(pNode);
                }
            }
        }            
        }
        pNode->height = max(height(pNode->left), height(pNode->right)) + 1;
    }

    // Traverses the tree and prints in pre order
    // This is used a lot in the testing system, changing it would be troublesome
    void printPreOrder(AVLNode<T> * & t, std::ostream& out)
    {
        if( t != nullptr )
        {
            out << t->val << " ";
            printPreOrder(t->left, out);
            printPreOrder(t->right, out);
        }
    }



};




#endif
