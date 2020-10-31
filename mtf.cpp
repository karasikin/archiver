#include "mtf.h"

#include <QList>
#include <QDebug>

std::unique_ptr<QByteArray> MTF::encode(std::unique_ptr<QByteArray> bytes) {
    auto transformedBytes = std::make_unique<QByteArray>();
    auto catalog = QList<char>();

    for(auto i = CHAR_MIN; i <= CHAR_MAX; ++i) {
        catalog.push_back(i);
    }

    for(const auto ch : *bytes) {
        int index = catalog.indexOf(ch);
        transformedBytes->push_back(index);
        catalog.move(index, 0);
    }

    return transformedBytes;
}

std::unique_ptr<QByteArray> MTF::decode(std::unique_ptr<QByteArray> bytes) {
    auto primaryBytes = std::make_unique<QByteArray>();
    auto catalog = QList<char>();

    for(auto i = CHAR_MIN; i <= CHAR_MAX; ++i) {
        catalog.push_back(i);
    }

    for(const auto ch : *bytes) {
        primaryBytes->push_back(catalog[uchar(ch)]);
        catalog.move(uchar(ch), 0);
    }

    return primaryBytes;
}
