#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "fileworker.h"
#include "bwt.h"

int main(int argc, char *argv[])
{
    auto text = QByteArray("Мама мыла рыму, рама мыла маму, маму мыла рама");
    Huffman h(text);

    auto frequency = h.frequencyAnalysis();

    std::for_each(text.cbegin(), text.cend(), [](const auto &item){
        auto debug = qDebug();
        debug << Qt::hex << int(item);
    });

    for(auto it = frequency->cbegin(); it != frequency->cend(); ++it) {
        auto debug = qDebug();
        debug << Qt::hex << int(it.key()) << ":" << it.value();
    }

    auto code = h.encode(frequency);

    qDebug() << "--------------------------------------------";

    std::for_each(code->cbegin(), code->cend(), [](const auto &item){
        auto debug = qDebug();
        debug << Qt::hex << int(item);
    });

    auto decodedText = h.decode(code);

    qDebug() << "--------------------------------------------";

    std::for_each(decodedText->cbegin(), decodedText->cend(), [](const auto &item){
        auto debug = qDebug();
        debug << Qt::hex << int(item);
    });

    qDebug() << (text == *decodedText);

    delete frequency;
    delete code;
    delete decodedText;


//    auto text = FileWorker::readTextFromFile("../files/text_slojny.txt");
//    qDebug() << *text;
//    qDebug() << "----------------------------------";

//    auto bwt = BWT();
//    auto bwtString = bwt.getTransformedString(*text);

//    qDebug() << *bwtString;
//    qDebug() << "----------------------------------";

//    //bwt.setString(*bwtString);
//    auto primaryString = bwt.getPrimaryString(*bwtString);

//    qDebug() << *primaryString;

//    delete bwtString;
//    delete primaryString;


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
