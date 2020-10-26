#ifndef BWT_H
#define BWT_H

class QByteArray;

class BWT
{

public:
    explicit BWT(const char end_of_string = char(0x00));

    QByteArray *getTransformedString(const QByteArray &string) const;
    QByteArray *getPrimaryString(const QByteArray &transformedStiring) const;

private:
    const char END_OF_STRING;

};

#endif // BWT_H
