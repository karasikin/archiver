#ifndef FILEWORKER_H
#define FILEWORKER_H

class QString;
class QByteArray;

class FileWorker
{

public:
    static const QByteArray *readFromFile(const QString &filename);
    static void writeToFile(const QString &filename, const QByteArray &bytes);

private:

};

#endif // FILEWORKER_H
