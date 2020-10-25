#include "bwt.h"

#include <QDebug>

BWT::BWT(const QChar end_of_string) : END_OF_STRING(end_of_string) {}

QString *BWT::getTransformedString(const QString &string) const {
    auto transformedString = new QString;                       // Преобразованная строка
    auto buffer = QString(string + END_OF_STRING);              // В этой строке проводим преобразование
    auto stringSize = string.size() + 1;                        // Размер преобразованной строки
    auto stringMatrix = QVector<QString::const_iterator>();     // Итераторы подстрок в буффере

    // Заполняем буффер и вектор итераторов

    if(!string.isEmpty()) {
        for(auto it = string.cbegin(); it != string.cend(); ++it) {
                buffer += *it;
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

    return transformedString;
}

QString *BWT::getPrimaryString(const QString &transformedString) const {
    auto primaryString = new QString(transformedString.size() - 1, ' ');  // По-моему нельзя задать просто размер
    auto firstCountTable = QVector<QPair<QChar, int>>();                  // { Символ : Количество таких же символов до текущего в строке }
    auto secondCountTable = QMap<QChar, int>();                           // { Символ : Количество "меньших" символов в строке


    // Формируем firstCountTable

    for(const auto ch : transformedString) {
        if(secondCountTable.contains(ch)) {
            firstCountTable.push_back({ch, secondCountTable[ch]});
        } else {
            firstCountTable.push_back({ch, 0});
        }

        secondCountTable[ch]++;
    }

    // Формируем SecondCountTable

    int sum = 0;
    for(auto &item : secondCountTable) {
        int prevSum = sum;
        sum += item;
        item = prevSum;
    }

    // Вычислям исходную строку

    auto index = firstCountTable.indexOf({END_OF_STRING, 0});

    for(int i = transformedString.size() - 2; i >= 0; --i) {
        index = firstCountTable[index].second + secondCountTable[firstCountTable[index].first];
        (*primaryString)[i] = firstCountTable[index].first;
    }

    return primaryString;
}
