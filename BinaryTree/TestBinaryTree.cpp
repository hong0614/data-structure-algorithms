#include <iostream>
#include "BinaryTree.h"
using namespace std;

//Test program
int main()
{

    /*
    -------------------------------- BinarySearchTree --------------------------------
    */
    BinarySearchTree<int> BST_1;
    int NUMS_1 = 400000;
    const int GAP_1 = 3711;
    int i_1;

    cout << "Checking... (no more output means success)" << endl;

    for (i_1 = GAP_1; i_1 != 0; i_1 = (i_1 + GAP_1) % NUMS_1)
        BST_1.insert(i_1);

    for (i_1 = 1; i_1 < NUMS_1; i_1 += 2)
        BST_1.remove(i_1);

    if (NUMS_1 < 40)
        BST_1.printTree();
    if (BST_1.findMin() != 2 || BST_1.findMax() != NUMS_1 - 2)
        cout << "FindMin or FindMax error!" << endl;

    for (i_1 = 2; i_1 < NUMS_1; i_1 += 2)
        if (!BST_1.contains(i_1))
            cout << "Find error1!" << endl;

    for (i_1 = 1; i_1 < NUMS_1; i_1 += 2)
    {
        if (BST_1.contains(i_1))
            cout << "Find error2!" << endl;
    }

    BinarySearchTree<int> BST_2;
    BST_2 = BST_1;

    for (i_1 = 2; i_1 < NUMS_1; i_1 += 2)
        if (!BST_2.contains(i_1))
            cout << "Find error1!" << endl;

    for (i_1 = 1; i_1 < NUMS_1; i_1 += 2)
    {
        if (BST_2.contains(i_1))
            cout << "Find error2!" << endl;
    }

    cout << "Finished testing for Binary Search Tree" << endl;

    /*
    -------------------------------- AVLTree --------------------------------
    */

    AvlTree<int> AVL_1;
    int NUMS_2 = 2000000;
    const int GAP_2 = 37;
    int i_2;

    cout << "Checking... (no more output means success)" << endl;

    for (i_2 = GAP_2; i_2 != 0; i_2 = (i_2 + GAP_2) % NUMS_2)
        AVL_1.insert(i_2);
    AVL_1.remove(0);
    for (i_2 = 1; i_2 < NUMS_2; i_2 += 2)
        AVL_1.remove(i_2);

    if (NUMS_2 < 40)
        AVL_1.printTree();
    if (AVL_1.findMin() != 2 || AVL_1.findMax() != NUMS_2 - 2)
        cout << "FindMin or FindMax error!" << endl;

    for (i_2 = 2; i_2 < NUMS_2; i_2 += 2)
        if (!AVL_1.contains(i_2))
            cout << "Find error1!" << endl;

    for (i_2 = 1; i_2 < NUMS_2; i_2 += 2)
    {
        if (AVL_1.contains(i_2))
            cout << "Find error2!" << endl;
    }

    AvlTree<int> AVL_2;
    AVL_2 = AVL_1;

    for (i_2 = 2; i_2 < NUMS_2; i_2 += 2)
        if (!AVL_2.contains(i_2))
            cout << "Find error1!" << endl;

    for (i_2 = 1; i_2 < NUMS_2; i_2 += 2)
    {
        if (AVL_2.contains(i_2))
            cout << "Find error2!" << endl;
    }

    cout << "End of test... AVL Tree" << endl;

    /*
    -------------------------------- RedBlackTree --------------------------------
    */
    const int NEG_INF = -9999;
    RedBlackTree<int> RBT_1{ NEG_INF };
    int NUMS_3 = 400000;
    const int GAP_3 = 37;
    int i_3;

    cout << "Checking... (no more output means success)" << endl;

    for (i_3 = GAP_3; i_3 != 0; i_3 = (i_3 + GAP_3) % NUMS_3)
        RBT_1.insert(i_3);

    if (NUMS_3 < 40)
        RBT_1.printTree();
    if (RBT_1.findMin() != 1 || RBT_1.findMax() != NUMS_3 - 1)
        cout << "FindMin or FindMax error!" << endl;

    for (i_3 = 1; i_3 < NUMS_3; ++i_3)
        if (!RBT_1.contains(i_3))
            cout << "Find error1!" << endl;
    if (RBT_1.contains(0))
        cout << "Oops!" << endl;


    RedBlackTree<int> RBT_2{ NEG_INF };
    RBT_2 = RBT_1;

    for (i_3 = 1; i_3 < NUMS_3; ++i_3)
        if (!RBT_2.contains(i_3))
            cout << "Find error1!" << endl;
    if (RBT_2.contains(0))
        cout << "Oops!" << endl;

    cout << "Test complete for Red Black Tree" << endl;

    /*
    -------------------------------- SplayTree --------------------------------
    */

    SplayTree<int> t;
    int NUMS_4 = 3000;
    const int GAP_4 = 37;
    int i_4;

    cout << "Checking... (no more output means success)" << endl;

    for (i_4 = GAP_4; i_4 != 0; i_4 = (i_4 + GAP_4) % NUMS_4)
        t.insert(i_4);

    for (i_4 = 1; i_4 < NUMS_4; i_4 += 2)
        t.remove(i_4);

    if (NUMS_4 < 40)
        t.printTree();
    if (t.findMin() != 2 || t.findMax() != NUMS_4 - 2)
        cout << "FindMin or FindMax error!" << endl;

    for (i_4 = 2; i_4 < NUMS_4; i_4 += 2)
        if (!t.contains(i_4))
            cout << "Find error1!" << endl;

    for (i_4 = 1; i_4 < NUMS_4; i_4 += 2)
    {
        if (t.contains(i_4))
            cout << "Find error2!" << endl;
    }

    SplayTree<int> t2;
    t2 = t;

    for (i_4 = 2; i_4 < NUMS_4; i_4 += 2)
        if (!t2.contains(i_4))
            cout << "Find error1!" << endl;

    for (i_4 = 1; i_4 < NUMS_4; i_4 += 2)
    {
        if (t2.contains(i_4))
            cout << "Find error2!" << endl;
    }

    cout << "Test completed for SplayTree." << endl;
    return 0;
}
