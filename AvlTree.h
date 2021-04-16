/**
 * @Author: jy1lnz
 * @Description:
 * @Date: 2021/4/15
**/


#ifndef ALGORITHM_AVLTREE_H
#define ALGORITHM_AVLTREE_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>

struct AvlNode
{
    int data;
    int height;
    AvlNode * leftNode;
    AvlNode * rightNode;
    explicit AvlNode(int _data) : data(_data), height(1), leftNode(nullptr), rightNode(nullptr){}
};

class AvlTree
{
public:
    typedef AvlNode * AvlNodePtr;
    AvlTree::AvlNodePtr add(int value);
    int remove(int value);
    void print();

    AvlTree() : root(nullptr), size(0) {}

private:
    static AvlNodePtr rightRotate(AvlNodePtr node);
    static AvlNodePtr leftRotate(AvlNodePtr node);
    static int getHeight(AvlNodePtr node);
    static int getBalanceFactor(AvlNodePtr node);
    /*
        get value in tree node
    */
    AvlNodePtr getNode(AvlNodePtr node, int value);
    static AvlNodePtr removeNodeMin(AvlNodePtr node, AvlNodePtr &ans);
    AvlNodePtr remove(AvlNodePtr node, int value);
    AvlNodePtr add(AvlNodePtr node, int value);
    static AvlNodePtr balance(AvlNodePtr node);

private:
    AvlNodePtr root;
    int size;
};

void AvlTree::print()
{
    if (root == nullptr) return;
    std::queue<std::pair<AvlNodePtr, int>> que;
    que.push(std::make_pair(root, 1));
    int per = 1;
    while(!que.empty())
    {
        auto pa = que.front();
        que.pop();
        if (pa.second != per) {std::cout << "\n"; per = pa.second;}
        if (pa.first)
            std::cout << pa.first->data << "  ";
        else
            std::cout << -1 << "  ";
        if (pa.first)
        {
            que.push(std::make_pair(pa.first->leftNode, pa.second+1));
            que.push(std::make_pair(pa.first->rightNode, pa.second+1));
        }
    }
    std::cout << "\n";
}

AvlTree::AvlNodePtr AvlTree::getNode(AvlNodePtr node, int value)
{
    if (node == nullptr) return nullptr;
    if (value < node->data) return getNode(node->leftNode, value);
    if (value > node->data) return getNode(node->rightNode, value);
    return node;
}

AvlTree::AvlNodePtr AvlTree::removeNodeMin(AvlNodePtr node, AvlNodePtr &ans)
{
    if (node->leftNode == nullptr)
    {
        ans = node;
        return node->rightNode;
    }
    node->leftNode = removeNodeMin(node->leftNode, ans);
    return balance(node);
}

AvlTree::AvlNodePtr AvlTree::balance(AvlNodePtr node)
{
    node->height = std::max(getHeight(node->leftNode), getHeight(node->rightNode))+1;
    int balanceFactor = getHeight(node->leftNode)-getHeight(node->rightNode);

    if (balanceFactor > 1 && getBalanceFactor(node->leftNode) >= 0)
    {
        return rightRotate(node);
    }

    if (balanceFactor < -1 && getBalanceFactor(node->rightNode) <= 0)
    {
        return leftRotate(node);
    }

    if (balanceFactor > 1 && getBalanceFactor(node->leftNode) < 0)
    {
        node->leftNode = leftRotate(node->leftNode);
        return rightRotate(node);
    }

    if (balanceFactor < -1 && getBalanceFactor(node->rightNode) > 0)
    {
        node->rightNode = rightRotate(node->rightNode);
        return leftRotate(node);
    }
    return node;
}

AvlTree::AvlNodePtr AvlTree::add(int value)
{
    root = add(root, value);
    return root;
}

AvlTree::AvlNodePtr AvlTree::add(AvlNodePtr node, int value)
{
    if (node == nullptr)
    {
        ++size;
        return new AvlNode(value);
    }
    if (value < node->data)
    {
        node->leftNode = add(node->leftNode, value);
    }
    else if (value > node->data)
    {
        node->rightNode = add(node->rightNode, value);
    }
    else
    {
        // exist
        node->data = value;
        return node;
    }

    return balance(node);
}

int AvlTree::remove(int value)
{
    AvlNodePtr node = getNode(root, value);
    if (node == nullptr)
    {
        return -1;
    }
    root = remove(root, value);
    return 0;
}

AvlTree::AvlNodePtr AvlTree::remove(AvlNodePtr node, int value)
{
    if (node == nullptr) return nullptr;
    AvlNodePtr retNode = nullptr;
    if (value < node->data)
    {
        node->leftNode = remove(node->leftNode, value);
        retNode = node;
    }
    else if(value > node->data)
    {
        node->rightNode = remove(node->rightNode, value);
        retNode = node;
    }
    else
    {
        if (node->leftNode == nullptr)
        {
            retNode = node->rightNode;
            node->rightNode = nullptr;
            size--;
            delete node;
        }
        else if (node->rightNode == nullptr)
        {
            retNode = node->leftNode;
            node->leftNode = nullptr;
            size--;
            delete node;
        }
        else
        {
            AvlNodePtr successor;
            AvlNodePtr nodeRight = removeNodeMin(node->rightNode, successor);
            successor->leftNode = node->leftNode;
            successor->rightNode = node->rightNode;
            delete node;
            retNode = successor;
        }
    }

    if (retNode == nullptr) return nullptr;
    return balance(retNode);
}

int AvlTree::getHeight(AvlNodePtr node)
{
    if (node == nullptr) return 0;
    return node->height;
}

int AvlTree::getBalanceFactor(AvlNodePtr node)
{
    if (node == nullptr) return 0;
    return getHeight(node->leftNode)-getHeight(node->rightNode);
}

AvlTree::AvlNodePtr AvlTree::rightRotate(AvlNodePtr node)
{
    AvlNodePtr LN = node->leftNode;
    AvlNodePtr LRN = LN->rightNode;

    LN->rightNode = node;
    node->leftNode = LRN;

    node->height = std::max(getHeight(node->leftNode), getHeight(node->rightNode))+1;
    LN->height = std::max(getHeight(LN->leftNode), getHeight(LN->rightNode))+1;

    return LN;
}

AvlTree::AvlNodePtr AvlTree::leftRotate(AvlNodePtr node)
{
    AvlNodePtr RN = node->rightNode;
    AvlNodePtr RLN = RN->leftNode;

    RN->leftNode = node;
    node->rightNode = RLN;

    node->height = std::max(getHeight(node->leftNode), getHeight(node->rightNode))+1;
    RN->height = std::max(getHeight(RN->leftNode), getHeight(RN->rightNode))+1;

    return RN;
}

#endif //ALGORITHM_AVLTREE_H
