#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <QPair>
#include <QString>

class HuffmanTree
{
    class Node {
    public:
        Node(const QPair<QByteArray, int> &data, Node *leftChild = nullptr, Node *rightChild = nullptr)
            : data(data),
              leftChild(leftChild),
              rightChild(rightChild)
        {}

    public:
        QPair<QByteArray, int> data;
        Node *leftChild;
        Node *rightChild;
    };

public:
    class DecodeIterator {
    public:
        DecodeIterator(Node *currentNode) : currentNode(currentNode), rootOfTree(currentNode) {}
        bool isLeaf() const;
        const QByteArray &getLabel() const;
        void next(bool code);

    private:
        Node *currentNode;
        Node *rootOfTree;
    };


public:
    HuffmanTree() : root(nullptr) {}
    ~HuffmanTree();

    void insert(const QPair<QByteArray, int> &data);
    DecodeIterator getDecodeIterator() const;
    const QMap<char, QVector<bool>> *getCodes() const;

private:
    void insertHelper(const QPair<QByteArray, int> &data, Node **root);
    void getCodesHelper(QVector<bool> code, QMap<char, QVector<bool>> *codes, Node *root) const;
    void removeTree(Node **root);

private:
    Node *root;

};

#endif // HUFFMANTREE_H
