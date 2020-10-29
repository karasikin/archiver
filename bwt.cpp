#include "bwt.h"

#include <QDebug>

std::unique_ptr<QByteArray> BWT::encode(const QByteArray *string) {
    auto transformedString = std::make_unique<QByteArray>();
    auto buffer = QByteArray();
    auto stringSize = string->size();
    auto stringMatrix = QVector<QByteArray::const_iterator>();

    if(!string->isEmpty()) {
        createBuffer(buffer, string);
        createStringMatrix(stringMatrix, buffer, stringSize);
    }

    sortStringMatrix(stringMatrix, stringSize);

    for(const auto &it : stringMatrix) {
        *transformedString += *(it + stringSize - 1);
    }

    auto stringNumber = findPrimaryStringNumber(stringMatrix, string);
    pushIntIntoBlob(transformedString.get(), stringNumber);

    return transformedString;
}

std::unique_ptr<QByteArray> BWT::decode(const QByteArray *transformedString) {
    const int OFFSET = 4;

    auto primaryStringSize = transformedString->size() - OFFSET;
    auto primaryStringIndex = extractIntFromBlob(transformedString, primaryStringSize);

    auto primaryString = std::make_unique<QByteArray>();
    primaryString->resize(primaryStringSize);

    auto firstCountTable = QVector<QPair<char, int>>();           // { Символ : Количество таких же символов до текущего в строке }
    auto secondCountTable = QMap<char, int>();                    // { Символ : Количество "меньших" символов в строке

    createFirstAndSecondTable(firstCountTable, secondCountTable, transformedString, OFFSET);

    // Вычислям исходную строку

    auto index = primaryStringIndex;
    for(int i = transformedString->size() - 1 - OFFSET; i >= 0; --i) {
        (*primaryString)[i] = firstCountTable[index].first;
        index = firstCountTable[index].second + secondCountTable[firstCountTable[index].first];
    }

    return primaryString;
}

void BWT::createBuffer(QByteArray &buffer, const QByteArray *primaryString) {
    buffer.append(*primaryString);
    for(auto ch = primaryString->cbegin(); ch != primaryString->cend() - 1; ++ch) {
            buffer += *ch;
    }
}

void BWT::createStringMatrix(QVector<QByteArray::const_iterator> &stringMatrix, const QByteArray &buffer, int stringSize) {
    for(auto it = buffer.cbegin(); it != buffer.cend() - stringSize + 1; ++it) {
            stringMatrix.push_back(it);
    }
}

void BWT::sortStringMatrix(QVector<QByteArray::const_iterator> &stringMatrix, int stringSize) {
    auto comp = [stringSize](const auto &first, const auto &second) {
        for(int i = 0; i <= stringSize; ++i) {
            if(*(first + i) > *(second + i)) {
                return false;
            }
            if(*(first + i) < *(second + i)) {
                return true;
            }
        }

        return false;
    };

    std::sort(stringMatrix.begin(), stringMatrix.end(), comp);
}

int BWT::findPrimaryStringNumber(QVector<QByteArray::const_iterator> &stringMatrix, const QByteArray *primaryString) {
    auto stringNumber = int(0);
    for(int i = 0; i < stringMatrix.size(); ++i) {
        auto left = stringMatrix[i];
        auto right = primaryString->cbegin();
        auto isEqual = bool(true);

        while(right != primaryString->cend()) {
            if(*left++ != *right++) {
                isEqual = false;
                break;
            }
        }

        if(isEqual) {
            stringNumber = i;
            break;
        }
    }

    return stringNumber;
}

void BWT::pushIntIntoBlob(QByteArray *blob, int value) {
    blob->push_back(value >> 24);
    blob->push_back(value >> 16);
    blob->push_back(value >> 8);
    blob->push_back(value);
}

int BWT::extractIntFromBlob(const QByteArray *blob, int start) {
    int byteA = int(uchar((*blob)[start]));
    int byteB = int(uchar((*blob)[start + 1]));
    int byteC = int(uchar((*blob)[start + 2]));
    int byteD = int(uchar((*blob)[start + 3]));

    return (byteA << 24) + (byteB << 16) + (byteC << 8) + byteD;
}

void BWT::createFirstAndSecondTable(QVector<QPair<char, int>> &firstTable, QMap<char, int> &secondTable,
                                    const QByteArray *string, const int offset) {

    // Формируем firstCountTable

    for(auto ch = string->cbegin(); ch != string->cend() - offset; ++ch) {
        if(secondTable.contains(*ch)) {
            firstTable.push_back({*ch, secondTable[*ch]});
        } else {
            firstTable.push_back({*ch, 0});
        }

        secondTable[*ch]++;
    }

    // Формируем SecondCountTable

    int sum = 0;
    for(auto &item : secondTable) {
        int prevSum = sum;
        sum += item;
        item = prevSum;
    }
}
