#include "fileworker.h"

#include <QString>
#include <QFile>
#include <QTextStream>

#include "exception.h"

std::unique_ptr<QByteArray> FileWorker::readFromFile(const QString &filename) {
    auto bytes = std::make_unique<QByteArray>();

    if(!QFile::exists(filename)) {
        throw UnknownFileNameException(filename.toStdString());
    }

    QFile inputFile(filename);

    if(inputFile.open(QIODevice::ReadOnly)) {
        *bytes = inputFile.readAll();
        inputFile.close();
    } else {
        throw FileOpenErrorException(filename.toStdString());
    }

    return bytes;
}

void FileWorker::writeToFile(const QString &filename, const QByteArray *bytes) {
    QFile outputFile(filename);

    if(outputFile.open(QIODevice::WriteOnly)) {
        outputFile.write(*bytes);
        outputFile.close();
    } else {
        throw FileOpenErrorException(filename.toStdString());
    }
}
