#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "fileworker.h"
#include "bwt.h"

int main(int argc, char *argv[])
{
    auto text = FileWorker::readTextFromFile("../files/text_slojny.txt");
    qDebug() << *text;
    qDebug() << "----------------------------------";

    auto bwt = BWT();
    auto bwtString = bwt.getTransformedString(*text);

    qDebug() << *bwtString;
    qDebug() << "----------------------------------";

    //bwt.setString(*bwtString);
    auto primaryString = bwt.getPrimaryString(*bwtString);

    qDebug() << *primaryString;

    delete bwtString;
    delete primaryString;


//    Huffman h(*text);
//    delete text;

//    auto frequency = h.frequencyAnalysis();

//    qDebug() << Qt::endl << *frequency << Qt::endl;

//    auto code = h.encode(frequency);
//    FileWorker::writeCodeToFile("../files/code.haff", code);

//    try {
//        auto encoded = FileWorker::readCodeFromFile("../files/code.haff");
//        auto decoded = h.decode(encoded);
//        delete encoded;

//        qDebug() << *decoded;
//        FileWorker::writeTextToFile("../files/decoded.txt", *decoded);

//        delete decoded;

//    }  catch (char const *ex) {
//        qDebug() << ex;
//    }

//    delete code;
//    delete frequency;


    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
