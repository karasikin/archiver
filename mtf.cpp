#include "mtf.h"

#include <QByteArray>
#include <QList>
#include <QDebug>

QByteArray *MTF::encode(const QByteArray &bytes) const {
    auto transformedBytes = new QByteArray;
    auto catalog = QList<char>();

    for(auto i = CHAR_MIN; i <= CHAR_MAX; ++i) {
        catalog.push_back(i);
    }

    for(const auto ch : bytes) {
        int index = catalog.indexOf(ch);
        transformedBytes->push_back(index);
        catalog.move(index, 0);
    }

    return transformedBytes;
}

QByteArray *MTF::decode(const QByteArray &transformedBytes) const {
    auto primaryBytes = new QByteArray;
    auto catalog = QList<char>();

    for(auto i = CHAR_MIN; i < CHAR_MAX; ++i) {
        catalog.push_back(i);
    }

    for(const auto ch : transformedBytes) {
        primaryBytes->push_back(catalog[uchar(ch)]);
        catalog.move(uchar(ch), 0);
    }

    return primaryBytes;
}
