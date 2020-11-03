#include "archiver.h"

#include <QDebug>

#include "byteconverter.h"
#include "exception.h"

Archiver::Archiver(const QString &inputFilename, const QString &outputFilename,
                   int compressBlockSize, QObject *parent)
    : QObject(parent),
      inputFile(),
      outputFile()
{
    setCompressBlockSize(compressBlockSize);

    inputFile.setFileName(inputFilename);
    outputFile.setFileName(outputFilename);
}

Archiver::~Archiver() {
    for(auto ptr : convertingAlgorithms) {
        delete ptr;
    }
}

bool Archiver::compress() {
    if(!openFiles()) {
        return false;
    }

    emit startCompress(inputFile.size());

    while(!inputFile.atEnd()) {
        auto currentBytes = readBlock(compressBlockSize);

        for(auto item : convertingAlgorithms) {
            currentBytes = item->encode(std::move(currentBytes));
        }

        writeEncodedBlockSize(currentBytes->size());
        writeBlock(currentBytes.get());

        emit blockCompressed(compressBlockSize);
    }

    outputFile.flush();
    inputFile.close();
    outputFile.close();

    return true;
}

bool Archiver::uncompress() {
    if(!openFiles()) {
        return false;
    }

    emit startUncompress(inputFile.size());

    while(!inputFile.atEnd()) {
        try {
            int blockSize = readEncodedBlockSize();
            if(blockSize <= 0) {
                throw BadCodeException();
            }

            auto currentBytes = readBlock(blockSize);

            for(auto item : convertingAlgorithms) {
                currentBytes = item->decode(std::move(currentBytes));
            }

            writeBlock(currentBytes.get());

            emit blockUncompressed(blockSize);

        } catch(std::exception &ex) {
            outputFile.flush();
            inputFile.close();
            outputFile.close();
            message = ex.what();

            return false;
        }
    }

    outputFile.flush();
    inputFile.close();
    outputFile.close();

    return true;
}

void Archiver::addConvertingAlgorithms(ByteConverter *convertingAlgorithm) {
    convertingAlgorithms.push_back(convertingAlgorithm);
}

QString Archiver::getMessage() const{
    return message;
}

bool Archiver::openFiles() {
    if(!QFile::exists(inputFile.fileName())) {
        message = QString("File %1 not extits").arg(inputFile.fileName());
        return false;
    }

    bool isInputFileOpen = inputFile.open(QIODevice::ReadOnly);
    if(!isInputFileOpen) {
        message = QString("File %1 not open").arg(inputFile.fileName());
        return false;
    }

    bool isOutputFileOpen = outputFile.open(QIODevice::WriteOnly);
    if(!isOutputFileOpen) {
        message= QString("File %1 not open").arg(outputFile.fileName());
        return false;
    }

    return true;
}

std::unique_ptr<QByteArray> Archiver::readBlock(int blockSize) {
    auto input = std::make_unique<QByteArray>();
    input->resize(blockSize);

    *input = inputFile.read(blockSize);
    return input;
}

void Archiver::writeBlock(const QByteArray *block) {
    outputFile.write(*block);
}

int Archiver::readEncodedBlockSize() {
    QByteArray sizeInt = inputFile.read(4);

    return (int(uchar(sizeInt[0])) << 24) + (int(uchar(sizeInt[1])) << 16) +
            (int(uchar(sizeInt[2])) << 8) + (int(uchar(sizeInt[3])));
}

void Archiver::writeEncodedBlockSize(int blockSize) {
    QByteArray sizeInt;
    sizeInt.push_back(blockSize >> 24);
    sizeInt.push_back(blockSize >> 16);
    sizeInt.push_back(blockSize >> 8);
    sizeInt.push_back(blockSize);

    outputFile.write(sizeInt);
}
