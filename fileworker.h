#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <memory>

class QString;
class QByteArray;

class FileWorker
{

public:
    static std::unique_ptr<QByteArray> readFromFile(const QString &filename);
    static void writeToFile(const QString &filename, const QByteArray *bytes);

private:

};

#endif // FILEWORKER_H
