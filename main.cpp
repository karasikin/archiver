#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "bwt.h"
#include "mtf.h"
#include "archiver.h"

int main(int argc, char *argv[]) {

//    Archiver archiverComp("../files/text.txt", "../files/code.dat", 20 * 1024);
//    qDebug() << archiverComp.compress();

//    Archiver archiverUncomp("../files/code.dat", "../files/text.uncomp");
//    qDebug() << archiverUncomp.uncompress();

        Archiver archiverComp("../files/text.txt", "../files/text.txt.code", 900 * 1024);
        qDebug() << archiverComp.compress();

        Archiver archiverUncomp("../files/text.txt.code", "../files/text.txt.uncode");
        qDebug() << archiverUncomp.uncompress();

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
