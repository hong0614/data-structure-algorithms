#include<iostream>
#include<algorithm>
using namespace std;

//"dsexceptions.h"
class UnderflowException { };
class IllegalArgumentException { };
class ArrayIndexOutOfBoundsException { };
class IteratorOutOfBoundsException { };
class IteratorMismatchException { };
class IteratorUninitializedException { };

template<typename Comparable>
class BinaryTree
{
protected:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode* left;
        BinaryNode* right;

        BinaryNode(const Comparable& theElement, BinaryNode* lt, BinaryNode* rt)
            : element{ theElement }, left{ lt }, right{ rt } { }

        BinaryNode(Comparable&& theElement, BinaryNode* lt, BinaryNode* rt)
            : element{ std::move(theElement) }, left{ lt }, right{ rt } { }
    };
    BinaryNode* root;

public:
    BinaryTree() : root{ nullptr }
    {
    }

    BinaryTree(const BinaryTree& rhs) : root{ nullptr }
    {
        root = clone(rhs.root);
    }

    BinaryTree(BinaryTree&& rhs) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }


    virtual ~BinaryTree() {};

    BinaryTree& operator=(const BinaryTree& rhs)
    {
        BinaryTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }


    BinaryTree& operator=(BinaryTree&& rhs)
    {
        std::swap(root, rhs.root);
        return *this;
    }

    virtual const Comparable& findMin() const = 0;

    virtual const Comparable& findMax() const = 0;

    virtual bool contains(const Comparable& x) const = 0;

    virtual bool isEmpty() const
    {
        return root == nullptr;
    }

    virtual void printTree(ostream& out = cout) const = 0;

    virtual void makeEmpty() = 0;

    virtual void insert(const Comparable& x) = 0;

    virtual void insert(Comparable&& x) = 0;

    virtual void remove(const Comparable& x) = 0;

};

template<typename Comparable>
class BinarySearchTree : public BinaryTree<Comparable>
{
private:
    using BinaryNode = typename BinaryTree<Comparable>::BinaryNode;
    BinaryNode* root;

    void insert(const Comparable& x, BinaryNode*& t)
    {
        if (t == nullptr)
            t = new BinaryNode{ x, nullptr, nullptr };
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);
        else
            ;  // Duplicate; do nothing
    }


    void insert(Comparable&& x, BinaryNode*& t)
    {
        if (t == nullptr)
            t = new BinaryNode{ std::move(x), nullptr, nullptr };
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (t->element < x)
            insert(std::move(x), t->right);
        else
            ;  // Duplicate; do nothing
    }


    void remove(const Comparable& x, BinaryNode*& t)
    {
        if (t == nullptr)
            return;   // Item not found; do nothing
        if (x < t->element)
            remove(x, t->left);
        else if (t->element < x)
            remove(x, t->right);
        else if (t->left != nullptr && t->right != nullptr) // Two children
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else
        {
            BinaryNode* oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }
    }

    BinaryNode* findMin(BinaryNode* t) const
    {
        if (t == nullptr)
            return nullptr;
        if (t->left == nullptr)
            return t;
        return findMin(t->left);
    }


    BinaryNode* findMax(BinaryNode* t) const
    {
        if (t != nullptr)
            while (t->right != nullptr)
                t = t->right;
        return t;
    }

    bool contains(const Comparable& x, BinaryNode* t) const
    {
        if (t == nullptr)
            return false;
        else if (x < t->element)
            return contains(x, t->left);
        else if (t->element < x)
            return contains(x, t->right);
        else
            return true;    // Match
    }

    void makeEmpty(BinaryNode*& t)
    {
        if (t != nullptr)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }


    void printTree(BinaryNode* t, ostream& out) const
    {
        if (t != nullptr)
        {
            printTree(t->left, out);
            out << t->element << endl;
            printTree(t->right, out);
        }
    }

    BinaryNode* clone(BinaryNode* t) const
    {
        if (t == nullptr)
            return nullptr;
        else
            return new BinaryNode{ t->element, clone(t->left), clone(t->right) };
    }

public:
    BinarySearchTree() : root{ nullptr }
    {
    }


    BinarySearchTree(const BinarySearchTree& rhs) : root{ nullptr }
    {
        root = clone(rhs.root);
    }


    BinarySearchTree(BinarySearchTree&& rhs) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }


    ~BinarySearchTree()
    {
        makeEmpty();
    }

    BinarySearchTree& operator=(const BinarySearchTree& rhs)
    {
        BinarySearchTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    BinarySearchTree& operator=(BinarySearchTree&& rhs)
    {
        std::swap(root, rhs.root);
        return *this;
    }

    bool isEmpty() const override
    {
        return root == nullptr;
    }

    const Comparable& findMin() const override 
    {
        if (this->isEmpty())
            throw UnderflowException{ };
        return findMin(this->root)->element;
    }

    const Comparable& findMax() const override 
    {
        if (this->isEmpty())
            throw UnderflowException{ };
        return findMax(this->root)->element;
    }

    bool contains(const Comparable& x) const override 
    {
        return contains(x, this->root);
    }

    void makeEmpty() override 
    {
        makeEmpty(root);
    }

    void insert(const Comparable& x) override 
    {
        insert(x, this->root);
    }

    void insert(Comparable&& x) override 
    {
        insert(std::move(x), this->root);
    }

    void remove(const Comparable& x) override 
    {
        remove(x, this->root);
    }

    void printTree(ostream& out = cout) const override
    {
        if (isEmpty())
            out << "Empty tree" << endl;
        else
            printTree(root, out);
    }
};

template<typename Comparable>
class AvlTree :public BinaryTree<Comparable>
{
private:
    using BinaryNode = typename BinaryTree<Comparable>::BinaryNode;
    struct AvlNode 
    {
        Comparable element;
        AvlNode* left;
        AvlNode* right;
        int       height;

        AvlNode(const Comparable& ele, AvlNode* lt, AvlNode* rt, int h = 0)
            : element{ ele }, left{ lt }, right{ rt }, height{ h } { }

        AvlNode(Comparable&& ele, AvlNode* lt, AvlNode* rt, int h = 0)
            : element{ std::move(ele) }, left{ lt }, right{ rt }, height{ h } { }
    };

    AvlNode* root;

    void insert(const Comparable& x, AvlNode*& t)
    {
        if (t == nullptr)
            t = new AvlNode{ x, nullptr, nullptr };
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);

        balance(t);
    }

    void insert(Comparable&& x, AvlNode*& t)
    {
        if (t == nullptr)
            t = new AvlNode{ std::move(x), nullptr, nullptr };
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (t->element < x)
            insert(std::move(x), t->right);

        balance(t);
    }

    void remove(const Comparable& x, AvlNode*& t)
    {
        if (t == nullptr)
            return;   // Item not found; do nothing

        if (x < t->element)
            remove(x, t->left);
        else if (t->element < x)
            remove(x, t->right);
        else if (t->left != nullptr && t->right != nullptr) // Two children
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else
        {
            AvlNode* oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }

        balance(t);
    }

    static const int ALLOWED_IMBALANCE = 1;

    // Assume t is balanced or within one of being balanced
    void balance(AvlNode*& t)
    {
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);
        else
            if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
                if (height(t->right->right) >= height(t->right->left))
                    rotateWithRightChild(t);
                else
                    doubleWithRightChild(t);

        t->height = max(height(t->left), height(t->right)) + 1;
    }

    AvlNode* findMin(AvlNode* t) const
    {
        if (t == nullptr)
            return nullptr;
        if (t->left == nullptr)
            return t;
        return findMin(t->left);
    }

    AvlNode* findMax(AvlNode* t) const
    {
        if (t != nullptr)
            while (t->right != nullptr)
                t = t->right;
        return t;
    }

    bool contains(const Comparable& x, AvlNode* t) const
    {
        if (t == nullptr)
            return false;
        else if (x < t->element)
            return contains(x, t->left);
        else if (t->element < x)
            return contains(x, t->right);
        else
            return true;    // Match
    }

    void makeEmpty(AvlNode*& t)
    {
        if (t != nullptr)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

    void printTree(AvlNode* t) const
    {
        if (t != nullptr)
        {
            printTree(t->left);
            cout << t->element << endl;
            printTree(t->right);
        }
    }


    AvlNode* clone(AvlNode* t) const
    {
        if (t == nullptr)
            return nullptr;
        else
            return new AvlNode{ t->element, clone(t->left), clone(t->right), t->height };
    }

    int height(AvlNode* t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }

    void rotateWithLeftChild(AvlNode*& k2)
    {
        AvlNode* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    void rotateWithRightChild(AvlNode*& k1)
    {
        AvlNode* k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }

    void doubleWithLeftChild(AvlNode*& k3)
    {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void doubleWithRightChild(AvlNode*& k1)
    {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }

public:
    AvlTree() : root{ nullptr }
    { }

    AvlTree(const AvlTree& rhs) : root{ nullptr }
    {
        root = clone(rhs.root);
    }

    AvlTree(AvlTree&& rhs) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }

    ~AvlTree()
    {
        makeEmpty();
    }

    AvlTree& operator=(const AvlTree& rhs)
    {
        AvlTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    AvlTree& operator=(AvlTree&& rhs)
    {
        std::swap(root, rhs.root);

        return *this;
    }

    const Comparable& findMin() const override
    {
        if (isEmpty())
            throw UnderflowException{ };
        return findMin(root)->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable& findMax() const override
    {
        if (isEmpty())
            throw UnderflowException{ };
        return findMax(root)->element;
    }

    bool contains(const Comparable& x) const override
    {
        return contains(x, root);
    }

    bool isEmpty() const override
    {
        return root == nullptr;
    }

    void printTree(ostream& out = cout) const override
    {
        if (isEmpty())
            cout << "Empty tree" << endl;
        else
            printTree(root);
    }

    void makeEmpty() override
    {
        makeEmpty(root);
    }

    void insert(const Comparable& x) override
    {
        insert(x, root);
    }

    void insert(Comparable&& x) override
    {
        insert(std::move(x), root);
    }

    void remove(const Comparable& x) override
    {
        remove(x, root);
    }

};

template<typename Comparable>
class RedBlackTree :public BinaryTree<Comparable>
{
private:
    using BinaryNode = typename BinaryTree<Comparable>::BinaryNode;
    enum { RED, BLACK };
    struct RedBlackNode
    {
        Comparable    element;
        RedBlackNode* left;
        RedBlackNode* right;
        int           color;

        RedBlackNode(const Comparable& theElement = Comparable{ },
            RedBlackNode* lt = nullptr, RedBlackNode* rt = nullptr,
            int c = BLACK)
            : element{ theElement }, left{ lt }, right{ rt }, color{ c } { }

        RedBlackNode(Comparable&& theElement, RedBlackNode* lt = nullptr,
            RedBlackNode* rt = nullptr, int c = BLACK)
            : element{ std::move(theElement) }, left{ lt }, right{ rt }, color{ c } { }
    };

    RedBlackNode* header;   // The tree header (contains negInf)
    RedBlackNode* nullNode;

    // Used in insert routine and its helpers (logically static)
    RedBlackNode* current;
    RedBlackNode* parent;
    RedBlackNode* grand;
    RedBlackNode* great;

    // Usual recursive stuff
    void reclaimMemory(RedBlackNode* t)
    {
        if (t != t->left)
        {
            reclaimMemory(t->left);
            reclaimMemory(t->right);
            delete t;
        }
    }

    void printTree(RedBlackNode* t, ostream& out) const
    {
        if (t != t->left)
        {
            printTree(t->left,out);
            cout << t->element << endl;
            printTree(t->right,out);
        }
    }

    RedBlackNode* clone(RedBlackNode* t) const
    {
        if (t == t->left)  // Cannot test against nullNode!!!
            return nullNode;
        else
            return new RedBlackNode{ t->element, clone(t->left),
                                     clone(t->right), t->color };
    }

    // Red-black tree manipulations
/**
 * Internal routine that is called during an insertion if a node has two red
 * children. Performs flip and rotations. item is the item being inserted.
 */
    void handleReorient(const Comparable& item)
    {
        // Do the color flip
        current->color = RED;
        current->left->color = BLACK;
        current->right->color = BLACK;

        if (parent->color == RED)   // Have to rotate
        {
            grand->color = RED;
            if (item < grand->element != item < parent->element)
                parent = rotate(item, grand);  // Start dbl rotate
            current = rotate(item, great);
            current->color = BLACK;
        }
        header->right->color = BLACK; // Make root black
    }

    /**
     * Internal routine that performs a single or double rotation.
     * Because the result is attached to the parent, there are four cases.
     * Called by handleReorient.
     * item is the item in handleReorient.
     * theParent is the parent of the root of the rotated subtree.
     * Return the root of the rotated subtree.
     */
    RedBlackNode* rotate(const Comparable& item, RedBlackNode* theParent)
    {
        if (item < theParent->element)
        {
            item < theParent->left->element ?
                rotateWithLeftChild(theParent->left) :  // LL
                rotateWithRightChild(theParent->left);  // LR
            return theParent->left;
        }
        else
        {
            item < theParent->right->element ?
                rotateWithLeftChild(theParent->right) :  // RL
                rotateWithRightChild(theParent->right);  // RR
            return theParent->right;
        }
    }

    void rotateWithLeftChild(RedBlackNode*& k2)
    {
        RedBlackNode* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2 = k1;
    }

    void rotateWithRightChild(RedBlackNode*& k1)
    {
        RedBlackNode* k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1 = k2;
    }

public:
    explicit RedBlackTree(const Comparable& negInf)
    {
        nullNode = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;

        header = new RedBlackNode{ negInf };
        header->left = header->right = nullNode;
    }

    RedBlackTree(const RedBlackTree& rhs)
    {
        nullNode = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;

        header = new RedBlackNode{ rhs.header->element };
        header->left = nullNode;
        header->right = clone(rhs.header->right);
    }

    RedBlackTree(RedBlackTree&& rhs)
        : nullNode{ rhs.nullNode }, header{ rhs.header }
    {
        rhs.nullNode = nullptr;
        rhs.header = nullptr;
    }

    ~RedBlackTree()
    {
        makeEmpty();
        delete nullNode;
        delete header;
    }

    /**
     * Deep copy.
     */
    RedBlackTree& operator=(const RedBlackTree& rhs)
    {
        RedBlackTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    /**
     * Move.
     */
    RedBlackTree& operator=(RedBlackTree&& rhs)
    {
        std::swap(header, rhs.header);
        std::swap(nullNode, rhs.nullNode);

        return *this;
    }

    const Comparable& findMin() const override
    {
        if (isEmpty())
            throw UnderflowException{ };

        RedBlackNode* itr = header->right;

        while (itr->left != nullNode)
            itr = itr->left;

        return itr->element;
    }

    const Comparable& findMax() const override
    {
        if (isEmpty())
            throw UnderflowException{ };

        RedBlackNode* itr = header->right;

        while (itr->right != nullNode)
            itr = itr->right;

        return itr->element;
    }

    bool contains(const Comparable& x) const override
    {
        nullNode->element = x;
        RedBlackNode* curr = header->right;

        for (; ; )
        {
            if (x < curr->element)
                curr = curr->left;
            else if (curr->element < x)
                curr = curr->right;
            else
                return curr != nullNode;
        }
    }

    bool isEmpty() const override
    {
        return header->right == nullNode;
    }

    void printTree(ostream& out = cout) const override
    {
        if (header->right == nullNode)
            cout << "Empty tree" << endl;
        else
            printTree(header->right,out);
    }

    void makeEmpty() override
    {
        if (header == nullptr)
            return;

        reclaimMemory(header->right);
        header->right = nullNode;
    }

    /**
     * Insert item x into the tree. Does nothing if x already present.
     */
    void insert(const Comparable& x) override
    {
        current = parent = grand = header;
        nullNode->element = x;

        while (current->element != x)
        {
            great = grand; grand = parent; parent = current;
            current = x < current->element ? current->left : current->right;

            // Check if two red children; fix if so
            if (current->left->color == RED && current->right->color == RED)
                handleReorient(x);
        }

        // Insertion fails if already present
        if (current != nullNode)
            return;
        current = new RedBlackNode{ x, nullNode, nullNode };

        // Attach to parent
        if (x < parent->element)
            parent->left = current;
        else
            parent->right = current;
        handleReorient(x);
    }

    void insert(Comparable&& x) override
    {
        cout << "called: RedBlackTree<Comparable>::insert( Comparable && x )\n";
        current = parent = grand = header;
        nullNode->element = move(x);  // 使用 std::move 将 x 转为右值

        while (current->element != nullNode->element)
        {
            great = grand; grand = parent; parent = current;
            current = nullNode->element < current->element ?
                current->left : current->right;

            // Check if two red children; fix if so
            if (current->left->color == RED && current->right->color == RED)
                handleReorient(nullNode->element);
        }

        // Insertion fails if already present
        if (current != nullNode)
            return;
        current = new RedBlackNode{ move(nullNode->element), nullNode, nullNode };
        // 使用 std::move 将 nullNode->element 转为右值
// Attach to parent
        if (x < parent->element)
            parent->left = current;
        else
            parent->right = current;
        handleReorient(nullNode->element);
    }

    void remove(const Comparable& x) override
    {
        cout << "Sorry, remove unimplemented; " << x <<
            " still present" << endl;
    }

};

template<typename Comparable>
class SplayTree :public BinaryTree<Comparable>
{
private:
    struct BinaryNode
    {
        Comparable  element;
        BinaryNode* left;
        BinaryNode* right;

        BinaryNode() : left{ nullptr }, right{ nullptr } { }

        BinaryNode(const Comparable& theElement, BinaryNode* lt, BinaryNode* rt)
            : element{ theElement }, left{ lt }, right{ rt } { }
    };
    //using BinaryNode = typename BinaryTree<Comparable>::BinaryNode;
    BinaryNode *root;
    BinaryNode *nullNode;

    void reclaimMemory(BinaryNode* t)
    {
        if (t != t->left)
        {
            reclaimMemory(t->left);
            reclaimMemory(t->right);
            delete t;
        }
    }

    void printTree(BinaryNode* t, ostream& out) const
    {
        if (t != t->left)
        {
            printTree(t->left,out);
            cout << t->element << endl;
            printTree(t->right,out);
        }
    }

    BinaryNode* clone(BinaryNode* t) const
    {
        if (t == t->left)  // Cannot test against nullNode!!!
            return nullNode;
        else
            return new BinaryNode{ t->element, clone(t->left), clone(t->right) };
    }

    void rotateWithLeftChild(BinaryNode*& k2)
    {
        BinaryNode* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2 = k1;
    }

    void rotateWithRightChild(BinaryNode*& k1)
    {
        BinaryNode* k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1 = k2;
    }

    void splay(const Comparable& x, BinaryNode*& t)
    {
        BinaryNode* leftTreeMax, * rightTreeMin;
        static BinaryNode header;

        header.left = header.right = nullNode;
        leftTreeMax = rightTreeMin = &header;

        nullNode->element = x;   // Guarantee a match

        for (; ; )
            if (x < t->element)
            {
                if (x < t->left->element)
                    rotateWithLeftChild(t);
                if (t->left == nullNode)
                    break;
                // Link Right
                rightTreeMin->left = t;
                rightTreeMin = t;
                t = t->left;
            }
            else if (t->element < x)
            {
                if (t->right->element < x)
                    rotateWithRightChild(t);
                if (t->right == nullNode)
                    break;
                // Link Left
                leftTreeMax->right = t;
                leftTreeMax = t;
                t = t->right;
            }
            else
                break;

        leftTreeMax->right = t->left;
        rightTreeMin->left = t->right;
        t->left = header.right;
        t->right = header.left;
    }


 public:
     SplayTree()
     {
         nullNode = new BinaryNode;
         nullNode->left = nullNode->right = nullNode;
         root = nullNode;
     }

     SplayTree(const SplayTree& rhs)
     {
         nullNode = new BinaryNode;
         nullNode->left = nullNode->right = nullNode;
         root = clone(rhs.root);
     }

     SplayTree(SplayTree&& rhs) : root{ rhs.root }, nullNode{ rhs.nullNode }
     {
         rhs.root = nullptr;
         rhs.nullNode = nullptr;
     }

     ~SplayTree()
     {
         makeEmpty();
         delete nullNode;
     }

     SplayTree& operator=(const SplayTree& rhs)
     {
         SplayTree copy = rhs;
         std::swap(*this, copy);
         return *this;
     }

     SplayTree& operator=(SplayTree&& rhs)
     {
         std::swap(root, rhs.root);
         std::swap(nullNode, rhs.nullNode);

         return *this;
     }

     const Comparable& findMin()
     {
         if (isEmpty())
             throw UnderflowException{ };

         BinaryNode* ptr = root;

         while (ptr->left != nullNode)
             ptr = ptr->left;

         splay(ptr->element, root);
         return ptr->element;
     }


     const Comparable& findMax()
     {
         if (isEmpty())
             throw UnderflowException{ };

         BinaryNode* ptr = root;

         while (ptr->right != nullNode)
             ptr = ptr->right;

         splay(ptr->element, root);
         return ptr->element;
     }


     bool contains(const Comparable& x)
     {
         if (isEmpty())
             return false;
         splay(x, root);
         return root->element == x;
     }

     bool isEmpty() const override
     {
         return root == nullNode;
     }

     void printTree(ostream& out = cout) const override
     {
         if (isEmpty())
             cout << "Empty tree" << endl;
         else
             printTree(root,out);
     }

     void makeEmpty() override
     {
         /******************************
          * Comment this out, because it is prone to excessive
          * recursion on degenerate trees. Use alternate algorithm.

             reclaimMemory( root );
             root = nullNode;
          *******************************/
         while (!isEmpty())
         {
             findMax();        // Splay max item to root
             remove(root->element);
         }
     }

     void insert(const Comparable& x) override
     {
         static BinaryNode* newNode = nullptr;

         if (newNode == nullptr)
             newNode = new BinaryNode;
         newNode->element = x;

         if (root == nullNode)
         {
             newNode->left = newNode->right = nullNode;
             root = newNode;
         }
         else
         {
             splay(x, root);
             if (x < root->element)
             {
                 newNode->left = root->left;
                 newNode->right = root;
                 root->left = nullNode;
                 root = newNode;
             }
             else
                 if (root->element < x)
                 {
                     newNode->right = root->right;
                     newNode->left = root;
                     root->right = nullNode;
                     root = newNode;
                 }
                 else
                     return;
         }
         newNode = nullptr;   // So next insert will call new
     }

     void insert(Comparable&& x) override
     {
         cout << "called: SplayTree<Comparable>::insert( Comparable && x )\n";
         static BinaryNode* newNode = nullptr;

         if (newNode == nullptr)
             newNode = new BinaryNode;
         newNode->element = std::move(x); // 使用 std::move 将 x 转为右值  

         if (root == nullNode)
         {
             newNode->left = newNode->right = nullNode;
             root = newNode;
         }
         else
         {
             splay(newNode->element, root);
             if (newNode->element < root->element)
             {
                 newNode->left = root->left;
                 newNode->right = root;
                 root->left = nullNode;
                 root = newNode;
             }
             else
                 if (root->element < newNode->element)
                 {
                     newNode->right = root->right;
                     newNode->left = root;
                     root->right = nullNode;
                     root = newNode;
                 }
                 else
                     return;
         }
         newNode = nullptr;   // So next insert will call new
     }

     void remove(const Comparable& x) override
     {
         // If x is found, it will be splayed to the root by contains
         if (!contains(x))
             return;   // Item not found; do nothing

         BinaryNode* newTree;

         if (root->left == nullNode)
             newTree = root->right;
         else
         {
             // Find the maximum in the left subtree
             // Splay it to the root; and then attach right child
             newTree = root->left;
             splay(x, newTree);
             newTree->right = root->right;
         }
         delete root;
         root = newTree;
     }

     const Comparable& findMin() const override
     {
         return findMin();
     }

     const Comparable& findMax() const override
     {
         return findMax();
     }

     bool contains(const Comparable& x) const override
     {
         return contains(x);
     }
};
