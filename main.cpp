#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "huffman.h"
#include "fileworker.h"
#include "bwt.h"
#include "mtf.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
