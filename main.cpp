#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "fileworker.h"
#include "bwt.h"

int main(int argc, char *argv[]) {
//    {
//        auto uncompressed = FileWorker::readFromFile("../files/text.txt");
//        auto h = Huffman(*uncompressed);
//        auto frequency = h.frequencyAnalysis();
//        auto compressed = h.encode(frequency);
//        FileWorker::writeToFile("../files/arch.huff", *compressed);

//        delete compressed;
//        delete frequency;
//        delete uncompressed;
//    }

//    {
//        auto compressed = FileWorker::readFromFile("../files/arch.huff");
//        auto h = Huffman(*compressed);
//        auto uncompressed = h.decode(compressed);
//        FileWorker::writeToFile("../files/uncompress.txt", *uncompressed);

//        delete compressed;
//        delete uncompressed;
//    }

    auto bwt = BWT();
    auto bytes = FileWorker::readFromFile("../files/text.txt");
    auto transformed = bwt.getTransformedString(*bytes);
    FileWorker::writeToFile("../files/code.bwt", *transformed);

    auto loadedTransformed = FileWorker::readFromFile("../files/code.bwt");

    qDebug() << bytes;
    qDebug() << "----------------------------------";
    qDebug() << *loadedTransformed;

    auto primary = bwt.getPrimaryString(*loadedTransformed);
    FileWorker::writeToFile("../files/decoded.txt", *primary);

    qDebug() << "----------------------------------";
    qDebug() << *primary;

    delete loadedTransformed;
    delete bytes;
    delete primary;
    delete transformed;

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
