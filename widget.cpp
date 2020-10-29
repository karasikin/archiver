#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDebug>

#include "fileworker.h"
#include "exception.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget),
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
    try {
        // Сжимаем
        auto input = FileWorker::readFromFile(currentWorkingFile);
        auto bwtEncoded = BWT::encode(input.get());
        auto mtfEncoded = MTF::encode(bwtEncoded.get());
        auto frequensy = Huffman::frequencyAnalysis(mtfEncoded.get());
        auto huffmanEncoded = Huffman::encode(mtfEncoded.get(), frequensy.get());

        // Пишем в файл
        FileWorker::writeToFile(currentWorkingFile + COMPRESS_EXTENSION, huffmanEncoded.get());

        // Выводим информацию для пользователя
        ui->informationTextEdit->append(QString("%1 >> %2").arg(currentWorkingFile)
                                        .arg(currentWorkingFile + COMPRESS_EXTENSION));
        ui->informationTextEdit->append(QString("{%1 K >> %2 K}")
                                        .arg(input->size() / 1024).arg(huffmanEncoded->size() / 1024));
    } catch(std::exception &ex) {
        ui->informationTextEdit->append(ex.what());
    }
}

void Widget::onUnarchiveButton() {
    if(!currentWorkingFile.endsWith(COMPRESS_EXTENSION)) {
        ui->informationTextEdit->append(QString("Error: file %1 does not have extension %2")
                                        .arg(currentWorkingFile).arg(COMPRESS_EXTENSION));
        return;
    }

    try {
        // Разархивируем
        auto input = FileWorker::readFromFile(currentWorkingFile);
        auto huffmanDecoded = Huffman::decode(input.get());
        auto mtfDecoded = MTF::decode(huffmanDecoded.get());
        auto bwtDecoded = BWT::decode(mtfDecoded.get());

        // Составляем имя выходного файла
        QString newName(currentWorkingFile);
        newName.remove(newName.size() - COMPRESS_EXTENSION.size(), COMPRESS_EXTENSION.size());
        newName.append(".uncomp");

        // Пишем в файл
        FileWorker::writeToFile(newName, bwtDecoded.get());

        // Выводим информацию для пользователей
        ui->informationTextEdit->append(QString("%1 >> %2").arg(currentWorkingFile).arg(newName));
    }  catch (std::exception &ex) {
        ui->informationTextEdit->append(ex.what());
    }
}
