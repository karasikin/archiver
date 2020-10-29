#ifndef BWT_H
#define BWT_H

#include <memory>
#include <QByteArray>

template <class K, class V> class QPair;

class BWT
{

public:

    static std::unique_ptr<QByteArray> encode(const QByteArray *string);
    static std::unique_ptr<QByteArray> decode(const QByteArray *transformedStiring);

private:

    static void createBuffer(QByteArray &buffer, const QByteArray *primaryString);
    static void createStringMatrix(QVector<QByteArray::const_iterator> &stringMatrix, const QByteArray &buffer, int stringSize);
    static void sortStringMatrix(QVector<QByteArray::const_iterator> &stringMatrix, int stringSize);
    static int findPrimaryStringNumber(QVector<QByteArray::const_iterator> &stringMatrix, const QByteArray *primaryString);
    static void pushIntIntoBlob(QByteArray *blob, int value);
    static int extractIntFromBlob(const QByteArray *blob, int start);
    static void createFirstAndSecondTable(QVector<QPair<char, int>> &firstTable, QMap<char, int> &secondTable,
                                          const QByteArray *string, const int offset);

};

#endif // BWT_H
