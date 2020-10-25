#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <QString>
#include <QPair>

class HuffmanTree;

/* Класс предназначен для кодирования СТРОКИ СИМВОЛОВ(пока!!!)
 * Кодами хаффмана. И расскодирования кодов Хаффмана в строку?
 */

/* НЕОБХОДИМО ПОДУМАТЬ: как записать таблицу частот символов в код */

class Huffman
{

public:
    Huffman(const QString &str);

    /* Считает количество различных символов в строке
     * Возможно стоит переписать для любой внешней строки
     * Возвращает QMap {символ : количество*/

    const QMap<QChar, int> *frequencyAnalysis();

    /* На основе частоты из frequency кодирует строку
     * Возвращает массив unsigned char (байтов) пригодных для записи в файл */

    const QVector<unsigned char> *encode(const QMap<QChar, int> *frequency);

    /* Функция должна раскодировать код из вестора code,
     * И вернуть раскодированную строку */

    const QString *decode(const QVector<unsigned char> *code) const;

private:
    /* Вспомогательная функция. Преобразует Map частот символов в стек элементов дерева. */

    QStack<QPair<QString, int>> *buildNodeStack(const QMap<QChar, int> *frequency) const;

    /* Достает из стека узлы дерева, добавляет их в дерево.
     * Из полученного дерева достает цепочки кодов для каждого символа.
     * Возвращает их в QMap<QChar, QVector<bool>>.
     * QVector<bool> играет роль bitset переменной длины */

    const QMap<QChar, QVector<bool>> *buildCodesMap(QStack<QPair<QString, int>> *nodeStack) const;

    /* Проходит по заданной строке, для каждого символа в строке находит код этого символа в codeMap
     * Уплотняет коды в unsigned char, записывает их в QVector<unsigned char>, после чего возвращает
     * этот вестор*/

    const QVector<unsigned char> *encodeString(const QMap<QChar, QVector<bool>> *codeMap,
                                               const QMap<QChar, int> *frequency) const;

    /* Извлекает из закодированной последовательности информацию о частотах символов,
     * и удаляет из закодированной последовательности эту информацию */

    const QMap<QChar, int> *decodeFrequency(QVector<unsigned char> *code) const;

    /* При помощи дерева tree находим символы, закодированные в векторе code */

    /* ВАЖНО!!!  Прочекать последний символ. Возможно надо добавить какой-то особый байт/байты,
     * для определения конца кода!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

    const QString *decodeString(const HuffmanTree &tree, QVector<unsigned char> *code) const;

private:
    QString string;
};

#endif // HUFFMAN_H
