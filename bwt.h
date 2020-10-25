#ifndef BWT_H
#define BWT_H

#include <QString>
#include <QVector>

class BWT
{

public:
    explicit BWT(const QChar end_of_string = QChar(0x00, 0x00));

    QString *getTransformedString(const QString &string) const;
    QString *getPrimaryString(const QString &transformedString) const;

private:
    const QChar END_OF_STRING;

};

#endif // BWT_H
