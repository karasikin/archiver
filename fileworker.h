#ifndef FILEWORKER_H
#define FILEWORKER_H

class QString;
template <class T> class QVector;

class FileWorker
{

public:
    static const QString *readTextFromFile(const QString &filename);
    static const QVector<unsigned char> *readCodeFromFile(const QString &filename);
    static void writeCodeToFile(const QString &filename, const QVector<unsigned char> *code);
    static void writeTextToFile(const QString &filename, const QString &text);

private:

};

#endif // FILEWORKER_H
