#ifndef BWT_H
#define BWT_H

#include <memory>

class QByteArray;

class BWT
{

public:

    static std::unique_ptr<QByteArray> encode(const QByteArray *string);
    static std::unique_ptr<QByteArray> decode(const QByteArray *transformedStiring);

private:

    static void createBuffer(QByteArray *buffer, const QByteArray *primaryString);

};

#endif // BWT_H
