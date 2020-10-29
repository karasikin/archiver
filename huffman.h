#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <memory>
#include <QString>
#include <QPair>

class HuffmanTree;


class Huffman
{

public:

    static std::unique_ptr<QMap<char, int>> frequencyAnalysis(const QByteArray *bytes);

    static std::unique_ptr<QByteArray> encode(const QByteArray *bytes, const QMap<char, int> *frequency);

    static std::unique_ptr<QByteArray> decode(const QByteArray *code);

private:

    static std::unique_ptr<QStack<QPair<QByteArray, int>>> buildNodeStack(const QMap<char, int> *frequency);

    static std::unique_ptr<QMap<char, QVector<bool>>> buildCodesMap(QStack<QPair<QByteArray, int>> *nodeStack);

    static std::unique_ptr<QByteArray> encodeString(const QByteArray *bytes, const QMap<char, QVector<bool>> *codeMap,
                                               const QMap<char, int> *frequency);

    static void pushIntIntoBlob(QByteArray *blob, int value);

    static int extractIntFromBlob(const QByteArray *blob, int start);


    static std::unique_ptr<QMap<char, int>> decodeFrequency(QByteArray *code);

    static std::unique_ptr<QByteArray> decodeString(const HuffmanTree &tree, QByteArray *code);

};

#endif // HUFFMAN_H
