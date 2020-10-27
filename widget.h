#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "bwt.h"
#include "mtf.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void onSelectFileButton();
    void onArchiveButton();
    void onUnarchiveButton();

private:
    Ui::Widget *ui;

    BWT bwt;
    MTF mtf;

    QString currentWorkingFile;
    const QString extension = ".ibzip2";
};
#endif // WIDGET_H
