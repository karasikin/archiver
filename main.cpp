#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "fileworker.h"
#include "bwt.h"
#include "mtf.h"

int main(int argc, char *argv[]) {
//    auto bwt = BWT();
//    auto mtf = MTF();

//    auto input = QByteArray("mama myla ramy. Ramu myla mama.");

//    // encode

//    auto bwtCode = bwt.getTransformedString(input);
//    auto mtfCode = mtf.encode(*bwtCode);

//    {
//        auto debug = qDebug();
//        for(auto ch : *mtfCode) {
//            debug << Qt::hex << uchar(ch) << " ";
//        }
//    }

//    // decode

//    auto mtfEncoded = mtf.decode(*mtfCode);
//    auto bwtEncoded = bwt.getPrimaryString(*mtfEncoded);

//    qDebug() << *bwtEncoded;

//    delete mtfEncoded;
//    delete bwtEncoded;
//    delete bwtCode;
//    delete mtfCode;



//    auto mtf = MTF();
//    auto bytes = FileWorker::readFromFile("../files/text.txt");
//    auto transformed = mtf.encode(*bytes);
//    FileWorker::writeToFile("../files/code.mtf", *transformed);

//    {
//    auto deb = qDebug();
//    for(const auto ch : *transformed) {
//        deb << Qt::hex << int(ch) << " ";
//    }
//    }

//    auto compressed = FileWorker::readFromFile("../files/code.mtf");
//    auto primary = mtf.decode(*transformed);
//    FileWorker::writeToFile("../files/uncompressed.txt", *primary);


//    delete compressed;
//    delete bytes;
//    delete primary;
//    delete transformed;


try{
    {
        auto uncompressed = FileWorker::readFromFile("../files/text.txt");
        auto h = Huffman();
        auto frequency = h.frequencyAnalysis(*uncompressed);
        auto compressed = h.encode(*uncompressed, frequency);
        FileWorker::writeToFile("../files/arch.huff", *compressed);

        qDebug() << *uncompressed;

        auto debug = qDebug();
        for(auto byte : *compressed) {
            debug << Qt::hex << int(byte) << " ";
        }

        delete compressed;
        delete frequency;
        delete uncompressed;
    }

    {
        auto compressed = FileWorker::readFromFile("../files/arch.huff");
        auto h = Huffman();
        auto uncompressed = h.decode(compressed);
        FileWorker::writeToFile("../files/uncompress.txt", *uncompressed);

        qDebug() << *uncompressed;

        delete compressed;
        delete uncompressed;
    }
} catch(std::exception &ex) {
        qDebug() << ex.what();
} catch(...) {
        qDebug() << "что-то";
    }

//    auto bwt = BWT();
//    auto bytes = FileWorker::readFromFile("../files/text.txt");
//    auto transformed = bwt.getTransformedString(*bytes);
//    FileWorker::writeToFile("../files/code.bwt", *transformed);

//    auto loadedTransformed = FileWorker::readFromFile("../files/code.bwt");

//    qDebug() << bytes;
//    qDebug() << "----------------------------------";
//    qDebug() << *loadedTransformed;

//    auto primary = bwt.getPrimaryString(*loadedTransformed);
//    FileWorker::writeToFile("../files/decoded.txt", *primary);

//    qDebug() << "----------------------------------";
//    qDebug() << *primary;

//    delete loadedTransformed;
//    delete bytes;
//    delete primary;
//    delete transformed;

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
