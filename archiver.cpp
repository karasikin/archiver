#include "archiver.h"

#include <QMap>

#include "fileworker.h"
#include "bwt.h"
#include "mtf.h"
#include "huffman.h"

bool Archiver::compress() {
    message.clear();

    try {
        // Сжимаем
        auto input = FileWorker::readFromFile(inputFilename);
        auto bwtEncoded = BWT::encode(input.get());
        auto mtfEncoded = MTF::encode(bwtEncoded.get());
        auto frequensy = Huffman::frequencyAnalysis(mtfEncoded.get());
        auto huffmanEncoded = Huffman::encode(mtfEncoded.get(), frequensy.get());

        // Пишем в файл
        FileWorker::writeToFile(inputFilename + COMPRESS_EXTENSION, huffmanEncoded.get());

        // Выводим информацию для пользователя
        message.append(QString("%1 >> %2").arg(inputFilename)
                                        .arg(inputFilename + COMPRESS_EXTENSION));
        message.append(QString("{%1 K >> %2 K}")
                                        .arg(input->size() / 1024).arg(huffmanEncoded->size() / 1024));
    } catch(std::exception &ex) {
        message.append(ex.what());
        return false;
    }

    return true;
}

bool Archiver::uncompress() {
    message.clear();

    if(!inputFilename.endsWith(COMPRESS_EXTENSION)) {
        message.append(QString("Error: file %1 does not have extension %2")
                                        .arg(inputFilename).arg(COMPRESS_EXTENSION));
        return false;
    }

    try {
        // Разархивируем
        auto input = FileWorker::readFromFile(inputFilename);
        auto huffmanDecoded = Huffman::decode(input.get());
        auto mtfDecoded = MTF::decode(huffmanDecoded.get());
        auto bwtDecoded = BWT::decode(mtfDecoded.get());

        // Составляем имя выходного файла
        QString newName(inputFilename);
        newName.remove(newName.size() - COMPRESS_EXTENSION.size(), COMPRESS_EXTENSION.size());
        newName.append(".uncomp");

        // Пишем в файл
        FileWorker::writeToFile(newName, bwtDecoded.get());

        // Выводим информацию для пользователей
        message.append(QString("%1 >> %2").arg(inputFilename).arg(newName));
    }  catch (std::exception &ex) {
        message.append(ex.what());
        return false;
    }

    return true;
}

QString Archiver::getMessage() const{
    return message;
}
