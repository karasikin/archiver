#ifndef MTF_H
#define MTF_H

#include <memory>

class QByteArray;

// Нуждается в тестах..........................

class MTF
{

public:
    std::unique_ptr<QByteArray> encode(const QByteArray *bytes) const;
    std::unique_ptr<QByteArray> decode(const QByteArray *transformedBytes) const;

private:

};

#endif // MTF_H
