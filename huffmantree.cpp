#include "huffmantree.h"

#include <QDebug>

HuffmanTree::~HuffmanTree() {
    removeTree(&root);
}

void HuffmanTree::insert(const QPair<QByteArray, int> &data) {
    insertHelper(data, &root);
}

HuffmanTree::DecodeIterator HuffmanTree::getDecodeIterator() const {
    return HuffmanTree::DecodeIterator(root);
}

const QMap<char, QVector<bool>> *HuffmanTree::getCodes() const {
    auto codes = new QMap<char, QVector<bool>>;

    getCodesHelper({}, codes, root);

    return codes;
}

void HuffmanTree::insertHelper(const QPair<QByteArray, int> &data, Node **root) {
    if(*root == nullptr) {
        *root = new Node(data);
        return;
    }

    QByteArray newNodeKey = data.first;
    QByteArray currentNodeKey = (*root)->data.first;

    // Если текущий элемент является подстрокой правого или левого потомка
    // То посылем его к соответствующему новому корню
    if((*root)->leftChild) {
        QByteArray leftChildNodeKey = (*root)->leftChild->data.first;
        if(leftChildNodeKey.contains(newNodeKey)) {
            insertHelper(data, &(*root)->leftChild);
            return;
        }
    }

    if((*root)->rightChild) {
        QByteArray rightChildNodeKey = (*root)->rightChild->data.first;
        if(rightChildNodeKey.contains(newNodeKey)) {
            insertHelper(data, &(*root)->rightChild);
            return;
        }
    }

    // Определяем куда добавлять листы (если это они)
    if(currentNodeKey.startsWith(newNodeKey))
        insertHelper(data, &(*root)->leftChild);
    else
        insertHelper(data, &(*root)->rightChild);
}

void HuffmanTree::getCodesHelper(QVector<bool> code, QMap<char, QVector<bool>> *codes, Node *root) const {
    if(!root) {
        return;
    }

    if(root->leftChild) {
        getCodesHelper(code + QVector<bool> {0}, codes, root->leftChild);
    }

    if(root->rightChild) {
        getCodesHelper(code + QVector<bool> {1}, codes, root->rightChild);
    }

    if(!root->leftChild && !root->rightChild) {
        (*codes)[root->data.first.front()] = code;
    }
}

void HuffmanTree::removeTree(Node **root) {
    if(*root == nullptr) {
        return;
    }

    if((*root)->leftChild) {
        removeTree(&(*root)->leftChild);
    }

    if((*root)->rightChild) {
        removeTree(&(*root)->rightChild);
    }

    if(!(*root)->leftChild && !(*root)->rightChild) {
        delete *root;
        *root = nullptr;
    }
}

/*                DecodeIterator
 *                  Реализация
 */

bool HuffmanTree::DecodeIterator::isLeaf() const {
    return !currentNode->leftChild && !currentNode->rightChild;
}

const QByteArray &HuffmanTree::DecodeIterator::getLabel() const {
    return currentNode->data.first;
}

void HuffmanTree::DecodeIterator::next(bool code) {
    if(!code && currentNode->leftChild) {
        currentNode = currentNode->leftChild;
        return;
    }

    if(code && currentNode->rightChild) {
        currentNode = currentNode->rightChild;
        return;
    }

    currentNode = rootOfTree;
}
