#include "bwt.h"

#include <QDebug>


QByteArray *BWT::encode(const QByteArray &string) const {
    auto transformedString = new QByteArray;                    // Преобразованная строка
    auto buffer = QByteArray(string);                           // В этой строке проводим преобразование
    auto stringSize = string.size();                            // Размер преобразованной строки
    auto stringMatrix = QVector<QByteArray::const_iterator>();  // Итераторы подстрок в буффере

    // Заполняем буффер и вектор итераторов

    if(!string.isEmpty()) {
        for(auto ch = string.cbegin(); ch != string.cend() - 1; ++ch) {
                buffer += *ch;
        }

        for(auto it = buffer.cbegin(); it != buffer.cend() - stringSize + 1; ++it) {
                stringMatrix.push_back(it);
        }
    }

    // Определяем функцию сравнения двух подстрок

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

    // Сортируем подстроки

    std::sort(stringMatrix.begin(), stringMatrix.end(), comp);

    // Формируем преобразованную строку

    for(const auto &it : stringMatrix) {
        *transformedString += *(it + stringSize - 1);
    }

    // Определяем номер строки

    auto stringNumber = int(0);
    for(int i = 0; i < stringMatrix.size(); ++i) {
        auto left = stringMatrix[i];
        auto right = string.cbegin();
        auto isEqual = bool(true);

        while(right != string.cend()) {
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

    transformedString->push_back(stringNumber >> 24);
    transformedString->push_back(stringNumber >> 16);
    transformedString->push_back(stringNumber >> 8);
    transformedString->push_back(stringNumber);

    return transformedString;
}

QByteArray *BWT::decode(const QByteArray &transformedString) const {
    const int OFFSET = 4;

    int primaryStringIndex =
            (int(uchar(*(transformedString.rbegin() + 3))) << 24) +
            (int(uchar(*(transformedString.rbegin() + 2))) << 16) +
            (int(uchar(*(transformedString.rbegin() + 1))) << 8) +
            (int(uchar(*(transformedString.rbegin()))));

    auto primaryString = new QByteArray(transformedString.size() - OFFSET, ' ');  // По-моему нельзя задать просто размер
    auto firstCountTable = QVector<QPair<char, int>>();                     // { Символ : Количество таких же символов до текущего в строке }
    auto secondCountTable = QMap<char, int>();                              // { Символ : Количество "меньших" символов в строке


    // Формируем firstCountTable

    for(auto ch = transformedString.cbegin(); ch != transformedString.cend() - OFFSET; ++ch) {
        if(secondCountTable.contains(*ch)) {
            firstCountTable.push_back({*ch, secondCountTable[*ch]});
        } else {
            firstCountTable.push_back({*ch, 0});
        }

        secondCountTable[*ch]++;
    }

    // Формируем SecondCountTable

    int sum = 0;
    for(auto &item : secondCountTable) {
        int prevSum = sum;
        sum += item;
        item = prevSum;
    }

    // Вычислям исходную строку

    auto index = primaryStringIndex;
    for(int i = transformedString.size() - 1 - OFFSET; i >= 0; --i) {
        (*primaryString)[i] = firstCountTable[index].first;
        index = firstCountTable[index].second + secondCountTable[firstCountTable[index].first];
    }

    return primaryString;
}
