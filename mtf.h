#ifndef MTF_H
#define MTF_H

#include <memory>

class QByteArray;

// Нуждается в тестах..........................

class MTF
{

public:
    static std::unique_ptr<QByteArray> encode(const QByteArray *bytes);
    static std::unique_ptr<QByteArray> decode(const QByteArray *transformedBytes);

private:

};

#endif // MTF_H
