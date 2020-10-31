#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

#include <memory>
#include <QByteArray>


class ByteConverter {

public:

    ByteConverter() {}
    virtual ~ByteConverter() {}

    virtual std::unique_ptr<QByteArray> encode(std::unique_ptr<QByteArray> bytes) = 0;
    virtual std::unique_ptr<QByteArray> decode(std::unique_ptr<QByteArray> bytes) = 0;

    void pushIntIntoBlob(QByteArray *blob, int value) {
        blob->push_back(value >> 24);
        blob->push_back(value >> 16);
        blob->push_back(value >> 8);
        blob->push_back(value);
    }

    int extractIntFromBlob(const QByteArray *blob, int start) {
        int byteA = int(uchar((*blob)[start]));
        int byteB = int(uchar((*blob)[start + 1]));
        int byteC = int(uchar((*blob)[start + 2]));
        int byteD = int(uchar((*blob)[start + 3]));

        return (byteA << 24) + (byteB << 16) + (byteC << 8) + byteD;
    }
};


#endif // BYTECONVERTER_H
