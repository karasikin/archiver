#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <memory>
#include <QString>
#include <QPair>

class HuffmanTree;

/* Класс предназначен для кодирования СТРОКИ СИМВОЛОВ(пока!!!)
 * Кодами хаффмана. И расскодирования кодов Хаффмана в строку?
 */


class Huffman
{

public:

    /* Считает количество различных символов в строке
     * Возможно стоит переписать для любой внешней строки
     * Возвращает QMap {символ : количество*/

    std::unique_ptr<QMap<char, int>> frequencyAnalysis(const QByteArray *bytes);

    /* На основе частоты из frequency кодирует строку
     * Возвращает массив unsigned char (байтов) пригодных для записи в файл */

    std::unique_ptr<QByteArray> encode(const QByteArray *bytes, const QMap<char, int> *frequency);

    /* Функция должна раскодировать код из вестора code,
     * И вернуть раскодированную строку */

    std::unique_ptr<QByteArray> decode(const QByteArray *code) const;

private:
    /* Вспомогательная функция. Преобразует Map частот символов в стек элементов дерева. */

    std::unique_ptr<QStack<QPair<QByteArray, int>>> buildNodeStack(const QMap<char, int> *frequency) const;

    /* Достает из стека узлы дерева, добавляет их в дерево.
     * Из полученного дерева достает цепочки кодов для каждого символа.
     * Возвращает их в QMap<QChar, QVector<bool>>.
     * QVector<bool> играет роль bitset переменной длины */

    std::unique_ptr<QMap<char, QVector<bool>>> buildCodesMap(QStack<QPair<QByteArray, int>> *nodeStack) const;

    /* Проходит по заданной строке, для каждого символа в строке находит код этого символа в codeMap
     * Уплотняет коды в unsigned char, записывает их в QVector<unsigned char>, после чего возвращает
     * этот вестор*/

    std::unique_ptr<QByteArray> encodeString(const QByteArray *bytes, const QMap<char, QVector<bool>> *codeMap,
                                               const QMap<char, int> *frequency) const;

    /* Извлекает из закодированной последовательности информацию о частотах символов,
     * и удаляет из закодированной последовательности эту информацию */

    std::unique_ptr<QMap<char, int>> decodeFrequency(QByteArray *code) const;

    /* При помощи дерева tree находим символы, закодированные в векторе code */

    /* ВАЖНО!!!  Прочекать последний символ. Возможно надо добавить какой-то особый байт/байты,
     * для определения конца кода!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

    std::unique_ptr<QByteArray> decodeString(const HuffmanTree &tree, QByteArray *code) const;

};

#endif // HUFFMAN_H
