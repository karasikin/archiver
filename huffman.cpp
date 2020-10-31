#include "huffman.h"

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QStack>
#include <QDebug>

#include "huffmantree.h"
#include "exception.h"



std::unique_ptr<QByteArray> Huffman::encode(std::unique_ptr<QByteArray> bytes) {
    auto frequency = frequencyAnalysis(bytes.get());
    auto nodeStack = buildNodeStack(frequency.get());
    auto codeMap = buildCodesMap(nodeStack.get());
    auto code = encodeString(bytes.get(), codeMap.get(), frequency.get());

    return code;
}

std::unique_ptr<QByteArray> Huffman::decode(std::unique_ptr<QByteArray> bytes) {
    //auto transformedCode = std::make_unique<QByteArray>(bytes.get());
    //auto frequency = decodeFrequency(transformedCode.get());
    auto frequency = decodeFrequency(bytes.get());
    auto nodeStack = buildNodeStack(frequency.get());

    HuffmanTree codeTree;
    while(!nodeStack->empty()) {
        codeTree.insert(nodeStack->pop());
    }

    //auto decoded = decodeString(codeTree, transformedCode.get());
    auto decoded = decodeString(codeTree, bytes.get());

    return decoded;
}

std::unique_ptr<QMap<char, int>> Huffman::frequencyAnalysis(const QByteArray *bytes) {
    auto frequency = std::make_unique<QMap<char, int>>();
    for(const auto &ch : *bytes)
        (*frequency)[ch]++;

    return frequency;
}

std::unique_ptr<QStack<QPair<QByteArray, int>>> Huffman::buildNodeStack(const QMap<char, int> *frequency) {
    QMap<QByteArray, int> transformedFrequency;
    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        transformedFrequency[QByteArray(1, it.key())] = it.value();
    }

    auto nodeStack = std::make_unique<QStack<QPair<QByteArray, int>>>();

    for(auto item = transformedFrequency.cbegin(); item != transformedFrequency.cend(); ++item) {
        nodeStack->push({item.key(), item.value()});
    }

    auto comp = [](const auto &left, const auto &right) {
        return left < right;
    };

    while(transformedFrequency.size() > 1) {
        auto minFirst = std::min_element(transformedFrequency.cbegin(), transformedFrequency.cend(), comp);
        auto newKey = minFirst.key();
        auto newValue = minFirst.value();
        transformedFrequency.remove(minFirst.key());

        auto minSecond = std::min_element(transformedFrequency.cbegin(), transformedFrequency.cend(), comp);
        newKey += minSecond.key();
        newValue += minSecond.value();
        transformedFrequency.remove(minSecond.key());

        transformedFrequency[newKey] = newValue;
        nodeStack->push({newKey, newValue});
    }

    return nodeStack;
}

std::unique_ptr<QMap<char, QVector<bool>>> Huffman::buildCodesMap(QStack<QPair<QByteArray, int>> *nodeStack) {
    HuffmanTree tree;

    while(!nodeStack->empty()) {
        tree.insert(nodeStack->pop());
    }

    return tree.getCodes();
}

std::unique_ptr<QByteArray> Huffman::encodeString(const QByteArray *bytes,
                                        const QMap<char, QVector<bool>> *codeMap,
                                        const QMap<char, int> *frequency)
{
    auto encoded = std::make_unique<QByteArray>();

    pushIntIntoBlob(encoded.get(), frequency->size());

    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        encoded->push_back(it.key());
        pushIntIntoBlob(encoded.get(), it.value());
    }

    pushIntIntoBlob(encoded.get(), bytes->size());

    unsigned char byte = 0;
    int bit_count = 0;

    for(const auto &ch : *bytes) {
        const auto &code = (*codeMap)[ch];
        for(const auto &bit : code) {
            if(bit_count == 8) {
                encoded->push_back(byte);
                byte = 0;
                bit_count = 0;
            }

            ++bit_count;
            byte = byte << 1;
            if(bit) {
                byte |= 0x01;
            }
        }
    }

    byte <<= (8 - bit_count);
    encoded->push_back(byte);

    return encoded;
}

//void Huffman::pushIntIntoBlob(QByteArray *blob, int value) {
//    blob->push_back(value >> 24);
//    blob->push_back(value >> 16);
//    blob->push_back(value >> 8);
//    blob->push_back(value);
//}

//int Huffman::extractIntFromBlob(const QByteArray *blob, int start) {
//    int byteA = int(uchar((*blob)[start]));
//    int byteB = int(uchar((*blob)[start + 1]));
//    int byteC = int(uchar((*blob)[start + 2]));
//    int byteD = int(uchar((*blob)[start + 3]));

//    return (byteA << 24) + (byteB << 16) + (byteC << 8) + byteD;
//}

std::unique_ptr<QMap<char, int>>Huffman::decodeFrequency(QByteArray *code) {
    const int RECORD_BYTE_SIZE = 5;
    auto frequency = std::make_unique<QMap<char, int>>();
    int currentCodeByte = 0;

    int frequencySize = extractIntFromBlob(code, 0);
    currentCodeByte += 4;

    if(frequencySize < 0 || frequencySize >= code->size() - 4) {
        throw BadCodeException();
    }

    while(frequencySize--) {
        QByteArray record;
        for(int i = 0; i < RECORD_BYTE_SIZE; ++i) {
            record.push_back((*code)[currentCodeByte++]);
        }

        (*frequency)[record[0]] = extractIntFromBlob(&record, 1);
    }

    code->remove(0, currentCodeByte);

    return frequency;
}

std::unique_ptr<QByteArray> Huffman::decodeString(const HuffmanTree &tree, QByteArray *code) {
    auto it = tree.getDecodeIterator();
    auto decodedString = std::make_unique<QByteArray>();

    int byteCount = extractIntFromBlob(code, 0);
    code->remove(0, 4);

    if(!byteCount) {
        return decodedString;
    }

    for(auto item : *code) {
        for(int i = 0; i < 8; ++i) {

            it.next((item << i) & 0x80);

            if(it.isLeaf()) {

                decodedString->append(it.getLabel());
                it.next(1);

                if(! --byteCount) {
                    return decodedString;
                }
            }

        }
    }

    throw BadCodeException();
}
