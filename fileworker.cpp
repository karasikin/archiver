#include "fileworker.h"

#include <QString>
#include <QFile>
#include <QTextStream>

#include "exception.h"

const QString *FileWorker::readTextFromFile(const QString &filename) {
    auto str = new QString;

    if(!QFile::exists(filename)) {
        throw UnknownFileNameException(filename.toStdString());
    }

    QFile inputFile(filename);

    if(inputFile.open(QIODevice::ReadOnly)) {
        *str = { inputFile.readAll() };
        inputFile.close();
    } else {
        throw FileOpenErrorException(filename.toStdString());
    }

    return str;
}

const QVector<unsigned char> *FileWorker::readCodeFromFile(const QString &filename) {
    QVector<unsigned char> *code;

    if(!QFile::exists(filename)) {
        throw UnknownFileNameException(filename.toStdString());
    }

    QFile inputFile(filename);

    if(inputFile.open(QIODevice::ReadOnly)) {
        QByteArray inputData;
        inputData = inputFile.readAll();
        code = new QVector<unsigned char>{inputData.cbegin(), inputData.cend()};
        inputFile.close();
    } else {
        throw FileOpenErrorException(filename.toStdString());
    }

    return code;
}

void FileWorker::writeCodeToFile(const QString &filename, const QVector<unsigned char> *code) {
    QFile outputFile(filename);

    if(outputFile.open(QIODevice::WriteOnly)) {
        QByteArray outputData;
        for(const auto &item : *code) {
            outputData.push_back(item);
        }
        outputFile.write(outputData);
        outputFile.close();
    } else {
        throw FileOpenErrorException(filename.toStdString());
    }
}

void FileWorker::writeTextToFile(const QString &filename, const QString &text) {
    QFile output(filename);

    if(output.open(QIODevice::WriteOnly)) {
        QTextStream ts(&output);
        ts << text;
        output.close();
    } else {
        throw FileOpenErrorException(filename.toStdString());
    }
}
