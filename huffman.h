#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "byteconverter.h"

class HuffmanTree;
template <class K, class V> class QPair;

class Huffman : ByteConverter
{

public:

    std::unique_ptr<QByteArray> encode(std::unique_ptr<QByteArray> bytes) override;

    std::unique_ptr<QByteArray> decode(std::unique_ptr<QByteArray> bytes) override;

private:

    std::unique_ptr<QMap<char, int>> frequencyAnalysis(const QByteArray *bytes);

    std::unique_ptr<QStack<QPair<QByteArray, int>>> buildNodeStack(const QMap<char, int> *frequency);

    std::unique_ptr<QMap<char, QVector<bool>>> buildCodesMap(QStack<QPair<QByteArray, int>> *nodeStack);

    std::unique_ptr<QByteArray> encodeString(const QByteArray *bytes, const QMap<char, QVector<bool>> *codeMap,
                                               const QMap<char, int> *frequency);

    std::unique_ptr<QMap<char, int>> decodeFrequency(QByteArray *code);

    std::unique_ptr<QByteArray> decodeString(const HuffmanTree &tree, QByteArray *code);

};

#endif // HUFFMAN_H
