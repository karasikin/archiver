#include "huffman.h"

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QStack>
#include <QDebug>

#include "huffmantree.h"
#include "exception.h"

Huffman::Huffman(const QString &str)
{
    string += str + QChar(0xff, 0xff);          // Добавляем признак конца последовательности
}


const QMap<QChar, int> *Huffman::frequencyAnalysis() {
    auto frequency = new QMap<QChar, int>;
    for(const auto &ch : string)
        (*frequency)[ch]++;

    return frequency;
}

const QVector<unsigned char> *Huffman::encode(const QMap<QChar, int> *frequency) {
    auto nodeStack = buildNodeStack(frequency);
    auto codeMap = buildCodesMap(nodeStack);
    auto code = encodeString(codeMap, frequency);

    delete nodeStack;
    delete codeMap;

    return code;
}

const QString *Huffman::decode(const QVector<unsigned char> *code) const {
    // Наш код, который мы будем преобразовывать
    auto transformedCode = new QVector<unsigned char> {code->cbegin(), code->cend()};

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

QStack<QPair<QString, int>> *Huffman::buildNodeStack(const QMap<QChar, int> *frequency) const {
    auto comp = [](const int left, const int right) {
        return left < right;
    };

    QMap<QString, int> transformedFrequency;
    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        transformedFrequency[it.key()] = it.value();
    }

    auto nodeStack = new QStack<QPair<QString, int>>;

    for(auto item = transformedFrequency.cbegin(); item != transformedFrequency.cend(); ++item) {
        nodeStack->push({item.key(), item.value()});
    }

    while(transformedFrequency.size() > 1) {
        auto minFirst = std::min_element(transformedFrequency.cbegin(), transformedFrequency.cend(), comp);
        QString newKey = minFirst.key();
        int newValue = minFirst.value();
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

const QMap<QChar, QVector<bool>> *Huffman::buildCodesMap(QStack<QPair<QString, int>> *nodeStack) const {
    HuffmanTree tree;

    while(!nodeStack->empty()) {
        tree.insert(nodeStack->pop());
    }

    return tree.getCodes();
}

const QVector<unsigned char> *Huffman::encodeString(const QMap<QChar, QVector<bool>> *codeMap,
                                                    const QMap<QChar, int> *frequency) const {
    auto encoded = new QVector<unsigned char>;

    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        encoded->push_back(it.key().cell());
        encoded->push_back(it.key().row());
        encoded->push_back(uchar(it.value() >> 24));
        encoded->push_back(uchar(it.value() >> 16));
        encoded->push_back(uchar(it.value() >> 8));
        encoded->push_back(uchar(it.value()));
    }

    encoded->append({0x0, 0x0, 0x0, 0x0, 0x0, 0x0});  // Конец таблицы частот

    unsigned char byte = 0;
    int bit_count = 0;

    for(const auto &ch : string) {
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

const QMap<QChar, int> *Huffman::decodeFrequency(QVector<unsigned char> *code) const {
    const int RECORD_BYTE_SIZE = 6;
    auto frequency = new QMap<QChar, int>;

    while(true) {

        QVector<unsigned char> record(RECORD_BYTE_SIZE);
        for(int i = 0; i < RECORD_BYTE_SIZE; ++i) {
            if(code->isEmpty()) {
                delete frequency;
                throw BadCodeException();
            }

            record[i] = code->front();
            code->pop_front();
        }
        if(std::all_of(record.cbegin(), record.cend(), [](unsigned char item){ return item == 0; })) {
            break;
        }

        (*frequency)[{record[0], record[1]}] =
            (int(record[2]) << 24) + (int(record[3]) << 16) + (int(record[4]) << 8) + (int(record[5]));
    }

    return frequency;
}

const QString *Huffman::decodeString(const HuffmanTree &tree, QVector<unsigned char> *code) const {
    auto it = tree.getDecodeIterator();
    auto decodedString = new QString;

    for(auto item : *code) {
        for(int i = 0; i < 8; ++i) {
            if(it.isLeaf()) {
                if(it.getLabel() == QChar(0xff, 0xff)) {  // Конец закодированной последовательности
                    return decodedString;
                }

                *decodedString += it.getLabel();
                it.next(1);
            }
            it.next( (item << i) & 0x80);
        }
    }

    delete decodedString;

    throw BadCodeException();
}

