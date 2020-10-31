#ifndef BWT_H
#define BWT_H

#include "byteconverter.h"

template <class K, class V> class QPair;

class BWT : public ByteConverter {

public:
    BWT() : ByteConverter() {}

    std::unique_ptr<QByteArray> encode(std::unique_ptr<QByteArray> bytes) override;
    std::unique_ptr<QByteArray> decode(std::unique_ptr<QByteArray> bytes) override;

private:

    void createBuffer(QByteArray &buffer, const QByteArray *primaryString);
    void createStringMatrix(QVector<QByteArray::const_iterator> &stringMatrix, const QByteArray &buffer, int stringSize);
    void sortStringMatrix(QVector<QByteArray::const_iterator> &stringMatrix, int stringSize);
    int findPrimaryStringNumber(QVector<QByteArray::const_iterator> &stringMatrix, const QByteArray *primaryString);
    void createFirstAndSecondTable(QVector<QPair<char, int>> &firstTable, QMap<char, int> &secondTable,
                                          const QByteArray *string, const int offset);

};

#endif // BWT_H
