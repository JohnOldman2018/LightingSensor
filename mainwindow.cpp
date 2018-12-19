#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    QString qsTemp = ui->lineEdit->text();
    ui->plainTextEdit->appendPlainText(qsTemp);
    ui->lineEdit->clear();
    readFromFile(qsTemp);
}

void MainWindow::on_pushButton_pressed()
{
    on_lineEdit_returnPressed();
}

void MainWindow::MainWindow::writeToFile(QString writeLine, QString fileName)
{
    QFile outFile(fileName);
    if (!outFile.open(QIODevice::Append | QIODevice::Text)){
        qDebug() << "Cannot write to file.";
    }
    outFile.write(qPrintable(writeLine + "\n"));
    outFile.close();
}

void MainWindow::MainWindow::readFromFile(QString fileName)
{
    char buf[1024];
    QFile inputFile(fileName);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Can't open file" ;
    }
    while (!inputFile.atEnd()) {
        inputFile.readLine(buf, 1024);
        ui->plainTextEdit->appendPlainText(buf);
        writeToFile(buf, "short.log");
    }
}
