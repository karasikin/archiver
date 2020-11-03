#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "bwt.h"
#include "mtf.h"
#include "archiver.h"

int main(int argc, char *argv[]) {

//    Archiver archiverComp("../files/text.txt", "../files/code.dat", 900 * 1024);
//    archiverComp.addConvertingAlgorithms(new BWT());
//    archiverComp.addConvertingAlgorithms(new MTF());
//    archiverComp.addConvertingAlgorithms(new Huffman());
//    if(!archiverComp.compress()) {
//        qDebug() << archiverComp.getMessage();
//    }

//    Archiver archiverUncomp("../files/text.txt", "../files/text.uncomp");
//    archiverUncomp.addConvertingAlgorithms(new BWT());
//    archiverUncomp.addConvertingAlgorithms(new MTF());
//    archiverUncomp.addConvertingAlgorithms(new Huffman());
//    if(!archiverUncomp.uncompress()) {
//        qDebug() << archiverUncomp.getMessage();
//    }

//    Archiver archiverComp("../files/test/C++.tar", "../files/test/C++.tar.code", 900 * 1024);
//    archiverComp.addConvertingAlgorithms(new BWT());
//    archiverComp.addConvertingAlgorithms(new MTF());
//    archiverComp.addConvertingAlgorithms(new Huffman());
//    qDebug() << archiverComp.compress();

//    Archiver archiverUncomp("../files/test/C++.tar.code", "../files/test/C++.uncode.tar");
//    archiverUncomp.addConvertingAlgorithms(new BWT());
//    archiverUncomp.addConvertingAlgorithms(new MTF());
//    archiverUncomp.addConvertingAlgorithms(new Huffman());
//    qDebug() << archiverUncomp.uncompress();



//        Archiver archiverComp("../files/test/C++.tar", "../files/test/C++.tar.code", 900 * 1024);
//        qDebug() << archiverComp.compress();

//        Archiver archiverUncomp("../files/test/C++.tar.code", "../files/test/C++.uncode.tar");
//        qDebug() << archiverUncomp.uncompress();

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
