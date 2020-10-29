#include "huffman.h"

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QStack>
#include <QDebug>

#include "huffmantree.h"
#include "exception.h"

std::unique_ptr<QMap<char, int>> Huffman::frequencyAnalysis(const QByteArray *bytes) {
    auto frequency = std::make_unique<QMap<char, int>>();
    for(const auto &ch : *bytes)
        (*frequency)[ch]++;

    return frequency;
}

std::unique_ptr<QByteArray> Huffman::encode(const QByteArray *bytes, const QMap<char, int> *frequency) {
    auto nodeStack = buildNodeStack(frequency);
    auto codeMap = buildCodesMap(nodeStack.get());
    auto code = encodeString(bytes, codeMap.get(), frequency);

    return code;
}

std::unique_ptr<QByteArray> Huffman::decode(const QByteArray *code) const {
    // Наш код, который мы будем преобразовывать
    auto transformedCode = std::make_unique<QByteArray>(*code);

    // Извлекаем частоты символов
    auto frequency = decodeFrequency(transformedCode.get());

    // Получаем стек узлов кодового дерева
    auto nodeStack = buildNodeStack(frequency.get());

    HuffmanTree codeTree;
    while(!nodeStack->empty()) {
        codeTree.insert(nodeStack->pop());
    }
    auto decoded = decodeString(codeTree, transformedCode.get());

    return decoded;
}

std::unique_ptr<QStack<QPair<QByteArray, int>>> Huffman::buildNodeStack(const QMap<char, int> *frequency) const {
    QMap<QByteArray, int> transformedFrequency;
    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        transformedFrequency[{1, it.key()}] = it.value();
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

std::unique_ptr<QMap<char, QVector<bool>>> Huffman::buildCodesMap(QStack<QPair<QByteArray, int>> *nodeStack) const {
    HuffmanTree tree;

    while(!nodeStack->empty()) {
        tree.insert(nodeStack->pop());
    }

    return tree.getCodes();
}

std::unique_ptr<QByteArray> Huffman::encodeString(const QByteArray *bytes,
                                        const QMap<char, QVector<bool>> *codeMap,
                                        const QMap<char, int> *frequency) const
{
    auto encoded = std::make_unique<QByteArray>();


    // Логически делится на две функциии реализовать!!!!!!!!!!!!!!!!!!!!

    // Пишем размер таблицы частот
    int frequencySize = frequency->size();
    encoded->push_back(frequencySize >> 24);
    encoded->push_back(frequencySize >> 16);
    encoded->push_back(frequencySize >> 8);
    encoded->push_back(frequencySize);

    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        encoded->push_back(it.key());
        encoded->push_back(it.value() >> 24);
        encoded->push_back(it.value() >> 16);
        encoded->push_back(it.value() >> 8);
        encoded->push_back(it.value());
    }

    // Пишем размер текста
    int byteCount = bytes->size();
    encoded->push_back(byteCount >> 24);
    encoded->push_back(byteCount >> 16);
    encoded->push_back(byteCount >> 8);
    encoded->push_back(byteCount);

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

std::unique_ptr<QMap<char, int>>Huffman::decodeFrequency(QByteArray *code) const {
    const int RECORD_BYTE_SIZE = 5;
    auto frequency = std::make_unique<QMap<char, int>>();

    int frequencySize = (int(uchar((*code)[0])) << 24) + (int(uchar((*code)[1])) << 16) +
            (int(uchar((*code)[2])) << 8) + (int(uchar((*code)[3])));

    code->remove(0, 4);

    while(frequencySize--) {
        QByteArray record;
        for(int i = 0; i < RECORD_BYTE_SIZE; ++i) {
            if(code->isEmpty()) {
                throw BadCodeException();
            }

            record.push_back(code->front());
            code->remove(0, 1);    // Очень плохо переписать!!!!!!!!!
        }

        (*frequency)[record[0]] = (int(uchar(record[1])) << 24) +
                (int(uchar(record[2])) << 16) + (int(uchar(record[3])) << 8) + int(uchar(record[4]));
    }

    return frequency;
}

std::unique_ptr<QByteArray> Huffman::decodeString(const HuffmanTree &tree, QByteArray *code) const {
    auto it = tree.getDecodeIterator();
    auto decodedString = std::make_unique<QByteArray>();

    int byteCount = (int(uchar((*code)[0])) << 24) + (int(uchar((*code)[1])) << 16) +
            (int(uchar((*code)[2])) << 8) + (int(uchar((*code)[3])));
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

