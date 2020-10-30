#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDebug>

#include "archiver.h"

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
//    auto archiver = Archiver(currentWorkingFile);
//    archiver.compress();
//    ui->informationTextEdit->append(archiver.getMessage());
}

void Widget::onUnarchiveButton() {
//    auto archiver = Archiver(currentWorkingFile);
//    archiver.uncompress();
//    ui->informationTextEdit->append(archiver.getMessage());
}
