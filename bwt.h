#ifndef BWT_H
#define BWT_H

class QByteArray;

class BWT
{

public:

    QByteArray *encode(const QByteArray &string) const;
    QByteArray *decode(const QByteArray &transformedStiring) const;


};

#endif // BWT_H
