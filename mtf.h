#ifndef MTF_H
#define MTF_H

#include "byteconverter.h"

class MTF : public ByteConverter {

public:
    MTF() : ByteConverter() {}

    std::unique_ptr<QByteArray> encode(std::unique_ptr<QByteArray> bytes) override;
    std::unique_ptr<QByteArray> decode(std::unique_ptr<QByteArray> bytes) override;

private:

};

#endif // MTF_H
