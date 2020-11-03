#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDebug>

#include "archiver.h"
#include "bwt.h"
#include "mtf.h"
#include "huffman.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget),
      currentWorkingFile("")

{
    ui->setupUi(this);
    for(int i = 900; i > 0; i -= 100) {
        ui->comboBox->addItem(QString::number(i));
    }
    ui->comboBox->setCurrentIndex(0);

    ui->progressBar->reset();

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
    auto archiver = Archiver(currentWorkingFile, currentWorkingFile + COMPRESS_EXTENSION,
                             ui->comboBox->currentText().toInt() * 1024);
    connect(&archiver, &Archiver::startCompress, this, &Widget::onCompressStart);
    connect(&archiver, &Archiver::blockCompressed, this, &Widget::onBlockCompressed);

    archiver.addConvertingAlgorithms(new BWT());
    archiver.addConvertingAlgorithms(new MTF());
    archiver.addConvertingAlgorithms(new Huffman());
    if(archiver.compress()) {
        ui->informationTextEdit->append(QString("File %1 compressend > %2")
                                        .arg(currentWorkingFile)
                                        .arg(currentWorkingFile + COMPRESS_EXTENSION));
    }

    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->informationTextEdit->append(archiver.getMessage());
}

void Widget::onUnarchiveButton() {
    auto outputFileName = QString(currentWorkingFile);
    if(outputFileName.endsWith(COMPRESS_EXTENSION)) {
        outputFileName.remove(outputFileName.size() - COMPRESS_EXTENSION.size(), COMPRESS_EXTENSION.size());
    }

    auto archiver = Archiver(currentWorkingFile, outputFileName + UNCOMPRESS_EXTENSION);
    connect(&archiver, &Archiver::startUncompress, this, &Widget::onUncompressStart);
    connect(&archiver, &Archiver::blockUncompressed, this, &Widget::onBlockUncompressed);

    archiver.addConvertingAlgorithms(new Huffman());
    archiver.addConvertingAlgorithms(new MTF());
    archiver.addConvertingAlgorithms(new BWT());
    if(archiver.uncompress()) {
        ui->informationTextEdit->append(QString("File %1 uncompressend > %2")
                                        .arg(currentWorkingFile)
                                        .arg(outputFileName + UNCOMPRESS_EXTENSION));
    }

    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->informationTextEdit->append(archiver.getMessage());
}

void Widget::onCompressStart(int fileSize) {
    ui->progressBar->setRange(0, fileSize);
}

void Widget::onUncompressStart(int fileSize) {
    ui->progressBar->setRange(0, fileSize);
}

void Widget::onBlockCompressed(int blockSize) {
    ui->progressBar->setValue(ui->progressBar->value() + blockSize);
}

void Widget::onBlockUncompressed(int blockSize) {
    ui->progressBar->setValue(ui->progressBar->value() + blockSize);
}
