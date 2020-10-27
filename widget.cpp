#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>

#include "fileworker.h"
#include "exception.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget),
      bwt(),
      mtf(),
      currentWorkingFile("")

{
    ui->setupUi(this);

    connect(ui->selectButton, &QPushButton::clicked, this, &Widget::onSelectFileButton);
    connect(ui->archiveButton, &QPushButton::clicked, this, &Widget::onArchiveButton);
    connect(ui->unarchiveButton, &QPushButton::clicked, this, &Widget::onUnarchiveButton);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onSelectFileButton() {
    currentWorkingFile = QFileDialog::getOpenFileName(this, "Выбор файла", "/home");
    ui->filenameLineEdit->setText(currentWorkingFile);
}

void Widget::onArchiveButton() {
    const QByteArray *input = nullptr;
    const QByteArray *bwtEncoded = nullptr;
    const QByteArray *mtfEncoded = nullptr;

    try {
        input = FileWorker::readFromFile(currentWorkingFile);
        bwtEncoded = bwt.encode(*input);
        mtfEncoded = mtf.encode(*bwtEncoded);
        FileWorker::writeToFile(currentWorkingFile + extension, *mtfEncoded);
        ui->informationTextEdit->append(QString("%1 > %2").arg(currentWorkingFile).arg(currentWorkingFile + extension));
    } catch(std::exception &ex) {
        ui->informationTextEdit->append(ex.what());
    }

    delete input;
    delete bwtEncoded;
    delete mtfEncoded;
}

void Widget::onUnarchiveButton() {
    if(!currentWorkingFile.endsWith(extension)) {
        ui->informationTextEdit->append(QString("Error: file %1 does not have extension %2")
                                        .arg(currentWorkingFile).arg(extension));
        return;
    }

    const QByteArray *input = nullptr;
    const QByteArray *mtfDecoded = nullptr;
    const QByteArray *bwtDecoded = nullptr;

    try {
        input = FileWorker::readFromFile(currentWorkingFile);
        mtfDecoded = mtf.decode(*input);
        bwtDecoded = bwt.decode(*mtfDecoded);
        QString newName(currentWorkingFile);
        std::remove(newName.rbegin(), newName.rbegin() + extension.size(), newName);
        FileWorker::writeToFile(newName, *bwtDecoded);
    }  catch (std::exception &ex) {
        ui->informationTextEdit->append(ex.what());
    }

    delete input;
    delete mtfDecoded;
    delete bwtDecoded;
}
