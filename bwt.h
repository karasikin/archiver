#ifndef BWT_H
#define BWT_H

#include <memory>

class QByteArray;

class BWT
{

public:

    std::unique_ptr<QByteArray> encode(const QByteArray *string) const;
    std::unique_ptr<QByteArray> decode(const QByteArray *transformedStiring) const;


};

#endif // BWT_H
