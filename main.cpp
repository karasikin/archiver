#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "fileworker.h"
#include "bwt.h"

int main(int argc, char *argv[]) {
    {
        auto uncompressed = FileWorker::readFromFile("../files/text.txt");
        auto h = Huffman(*uncompressed, 0x0, 0xff);
        auto frequency = h.frequencyAnalysis();
        auto compressed = h.encode(frequency);
        FileWorker::writeToFile("../files/arch.huff", *compressed);

        delete compressed;
        delete frequency;
        delete uncompressed;
    }

    {
        auto compressed = FileWorker::readFromFile("../files/arch.huff");
        auto h = Huffman(*compressed);
        auto uncompressed = h.decode(compressed);
        FileWorker::writeToFile("../files/uncompress.txt", *uncompressed);

        delete compressed;
        delete uncompressed;
    }
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
