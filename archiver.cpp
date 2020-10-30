#include "archiver.h"

#include <QMap>
#include <QDebug>

#include "bwt.h"
#include "mtf.h"
#include "huffman.h"

Archiver::Archiver(const QString &inputFilename, const QString &outputFilename, int compressBlockSize)
    : inputFile(),
      outputFile()
{
    setCompressBlockSize(compressBlockSize);

    inputFile.setFileName(inputFilename);
    outputFile.setFileName(outputFilename);
}

bool Archiver::compress() {
    if(!openFiles()) {
        return false;
    }

    while(!inputFile.atEnd()) {
        auto input = readBlock(compressBlockSize);
        auto bwtEncoded = BWT::encode(input.get());
        auto mtfEnoded =MTF::encode(bwtEncoded.get());
        auto frequency = Huffman::frequencyAnalysis(mtfEnoded.get());
        auto huffmanEncoded = Huffman::encode(mtfEnoded.get(), frequency.get());

        writeEncodedBlockSize(huffmanEncoded->size());
        writeBlock(huffmanEncoded.get());
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

    while(!inputFile.atEnd()) {
        int blockSize = readEncodedBlockSize();
        auto input = readBlock(blockSize);
        auto huffmanDecoded = Huffman::decode(input.get());
        auto mtfDecoded = MTF::decode(huffmanDecoded.get());
        auto bwtDecoded = BWT::decode(mtfDecoded.get());

        writeBlock(bwtDecoded.get());
    }

    outputFile.flush();
    inputFile.close();
    outputFile.close();

    return true;
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
