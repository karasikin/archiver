#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDebug>

#include "fileworker.h"
#include "exception.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget),
      bwt(),
      mtf(),
      huffman(),
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
    currentWorkingFile = QFileDialog::getOpenFileName(this, "Выбор файла", "");
    ui->filenameLineEdit->setText(currentWorkingFile);
}

void Widget::onArchiveButton() {
    const QByteArray *input = nullptr;
    const QByteArray *bwtEncoded = nullptr;
    const QByteArray *mtfEncoded = nullptr;
    const QMap<char, int> *frequensy = nullptr;
    const QByteArray *huffmanEncoded = nullptr;

    try {
        input = FileWorker::readFromFile(currentWorkingFile);
        bwtEncoded = bwt.encode(*input);
        mtfEncoded = mtf.encode(*bwtEncoded);
        frequensy = huffman.frequencyAnalysis(*mtfEncoded);
        huffmanEncoded = huffman.encode(*mtfEncoded, frequensy);

        FileWorker::writeToFile(currentWorkingFile + extension, *huffmanEncoded);
        ui->informationTextEdit->append(QString("%1 >> %2").arg(currentWorkingFile)
                                        .arg(currentWorkingFile + extension));
        ui->informationTextEdit->append(QString("{%1 K >> %2 K}")
                                        .arg(input->size() / 1024).arg(huffmanEncoded->size() / 1024));
    } catch(std::exception &ex) {
        ui->informationTextEdit->append(ex.what());
    }

    delete huffmanEncoded;
    delete frequensy;
    delete mtfEncoded;
    delete bwtEncoded;
    delete input;
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
    const QByteArray *huffmanDecoded = nullptr;

    try {
        input = FileWorker::readFromFile(currentWorkingFile);
        huffmanDecoded = huffman.decode(input);
        mtfDecoded = mtf.decode(*huffmanDecoded);
        bwtDecoded = bwt.decode(*mtfDecoded);

        QString newName(currentWorkingFile);
        newName.remove(newName.size() - extension.size(), extension.size());
        newName.append(".uncomp");

        FileWorker::writeToFile(newName, *bwtDecoded);
        ui->informationTextEdit->append(QString("%1 >> %2").arg(currentWorkingFile).arg(newName));
    }  catch (std::exception &ex) {
        ui->informationTextEdit->append(ex.what());
    }

    delete huffmanDecoded;
    delete bwtDecoded;
    delete mtfDecoded;
    delete input;
}
