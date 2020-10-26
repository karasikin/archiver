#include "fileworker.h"

#include <QString>
#include <QFile>
#include <QTextStream>

#include "exception.h"

const QByteArray *FileWorker::readFromFile(const QString &filename) {
    auto bytes = new QByteArray;

    if(!QFile::exists(filename)) {
        delete bytes;
        throw UnknownFileNameException(filename.toStdString());
    }

    QFile inputFile(filename);

    if(inputFile.open(QIODevice::ReadOnly)) {
        *bytes = inputFile.readAll();
        inputFile.close();
    } else {
        delete bytes;
        throw FileOpenErrorException(filename.toStdString());
    }

    return bytes;
}

void FileWorker::writeToFile(const QString &filename, const QByteArray &bytes) {
    QFile outputFile(filename);

    if(outputFile.open(QIODevice::WriteOnly)) {
        outputFile.write(bytes);
        outputFile.close();
    } else {
        throw FileOpenErrorException(filename.toStdString());
    }
}
