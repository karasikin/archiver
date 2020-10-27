#ifndef MTF_H
#define MTF_H

class QByteArray;

// Нуждается в тестах..........................

class MTF
{

public:
    QByteArray *encode(const QByteArray &bytes) const;
    QByteArray *decode(const QByteArray &transformedBytes) const;

private:

};

#endif // MTF_H
