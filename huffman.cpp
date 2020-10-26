#include "huffman.h"

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QStack>
#include <QDebug>

#include "huffmantree.h"
#include "exception.h"

Huffman::Huffman(const QByteArray &uncompressedBytes, const char end_of_string, const char end_of_frequency)
    : END_OF_STRING(end_of_string),
      END_OF_FREQUENCY(end_of_frequency),
      uncompressedBytes(uncompressedBytes + end_of_string)
{}


const QMap<char, int> *Huffman::frequencyAnalysis() {
    auto frequency = new QMap<char, int>;
    for(const auto &ch : uncompressedBytes)
        (*frequency)[ch]++;

    return frequency;
}

const QByteArray *Huffman::encode(const QMap<char, int> *frequency) {
    auto nodeStack = buildNodeStack(frequency);
    auto codeMap = buildCodesMap(nodeStack);
    auto code = encodeString(codeMap, frequency);

    delete nodeStack;
    delete codeMap;

    return code;
}

const QByteArray *Huffman::decode(const QByteArray *code) const {
    // Наш код, который мы будем преобразовывать
    auto transformedCode = new QByteArray(*code);

    // Извлекаем частоты символов
    auto frequency = decodeFrequency(transformedCode);

    // Получаем стек узлов кодового дерева
    auto nodeStack = buildNodeStack(frequency);

    delete frequency;


    HuffmanTree codeTree;
    while(!nodeStack->empty()) {
        codeTree.insert(nodeStack->pop());
    }

    delete nodeStack;


    auto decoded = decodeString(codeTree, transformedCode);

    delete transformedCode;


    return decoded;
}

QStack<QPair<QByteArray, int>> *Huffman::buildNodeStack(const QMap<char, int> *frequency) const {
    QMap<QByteArray, int> transformedFrequency;
    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        transformedFrequency[{1, it.key()}] = it.value();
    }

    auto nodeStack = new QStack<QPair<QByteArray, int>>;

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

const QMap<char, QVector<bool>> *Huffman::buildCodesMap(QStack<QPair<QByteArray, int>> *nodeStack) const {
    HuffmanTree tree;

    while(!nodeStack->empty()) {
        tree.insert(nodeStack->pop());
    }

    return tree.getCodes();
}

const QByteArray *Huffman::encodeString(const QMap<char, QVector<bool>> *codeMap,
                                                    const QMap<char, int> *frequency) const {
    auto encoded = new QByteArray;

    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        encoded->push_back(it.key());
        encoded->push_back(it.value() >> 24);
        encoded->push_back(it.value() >> 16);
        encoded->push_back(it.value() >> 8);
        encoded->push_back(it.value());
    }

    encoded->append(QByteArray(5, END_OF_FREQUENCY));  // Конец таблицы частот

    unsigned char byte = 0;
    int bit_count = 0;

    for(const auto &ch : uncompressedBytes) {
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

const QMap<char, int> *Huffman::decodeFrequency(QByteArray *code) const {
    const int RECORD_BYTE_SIZE = 5;
    auto frequency = new QMap<char, int>;

    while(true) {
        QByteArray record;
        for(int i = 0; i < RECORD_BYTE_SIZE; ++i) {
            if(code->isEmpty()) {
                delete frequency;
                throw BadCodeException();
            }

            record.push_back(code->front());
            code->remove(0, 1);    // Очень плохо переписать!!!!!!!!!
        }
        if(std::all_of(record.cbegin(), record.cend(), [this](const auto item){ return item == END_OF_FREQUENCY; })) {
            break;
        }

        (*frequency)[record[0]] =
            (int(uchar(record[1])) << 24) + (int(uchar(record[2])) << 16) + (int(uchar(record[3])) << 8) + int(uchar(record[4]));
    }

    return frequency;
}

const QByteArray *Huffman::decodeString(const HuffmanTree &tree, QByteArray *code) const {
    auto it = tree.getDecodeIterator();
    auto decodedString = new QByteArray;

    for(auto item : *code) {
        for(int i = 0; i < 8; ++i) {
            if(it.isLeaf()) {
                if(it.getLabel() == QByteArray(1, END_OF_STRING)) {  // Конец закодированной последовательности
                    return decodedString;
                }

                decodedString->append(it.getLabel());
                it.next(1);
            }
            it.next((item << i) & 0x80);
        }
    }

    delete decodedString;

    throw BadCodeException();
}

