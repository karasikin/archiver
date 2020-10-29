#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "bwt.h"
#include "mtf.h"
#include "huffman.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    const QString COMPRESS_EXTENSION = ".ibzip2";
    const QString UNCOMPRESS_EXTENSION = ".uncomp";

public slots:
    void onSelectFileButton();
    void onArchiveButton();
    void onUnarchiveButton();

private:
    Ui::Widget *ui;

    QString currentWorkingFile;

};
#endif // WIDGET_H
