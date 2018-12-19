#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QFile>

static QFile outFile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    outFile.setFileName("short.log"); //"C:/Qt/build-LightingSensor-Desktop_Qt_5_11_1_MinGW_32bit-Debug/short.log"
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

static int countLighting = 0;
static double sum = 0.0;
static int count = 0;
static double average = 0.0;
void MainWindow::readFromFile(QString fileName)
{
    QFile inputFile(fileName);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Can't open file" ;
    }
    while (!inputFile.atEnd()) {
        QString line;
        int i=0;
        while(!inputFile.atEnd()){
            line = inputFile.readLine();
            line = line.trimmed();
            process_line(line);
            i++;
        }
    }
}

void MainWindow::process_line(QString input)
{
    QStringList qsl = input.split(',');
    if(qsl.size()==3){
        double lux = qsl.at(0).toDouble();
        QString qsTemp = QString::number(lux,'f',6);
        qsl[0]=qsTemp;
        qint64 unixTimeMilSec = static_cast<qint64>(qsl.at(1).toLongLong());
        QDateTime dt =QDateTime::fromMSecsSinceEpoch(unixTimeMilSec);
        qsTemp = dt.toString();
        if (dt.time().hour() == countLighting){
            sum += lux; count++;
        }
        else {
            average = sum/count;
            countLighting = dt.time().hour();
            count = 0;
            sum = 0.0;
            qDebug() << "Average lighting level: " << average << "lux at " << dt.time().hour() << " o'clock on date:" + qsTemp;
            qsl.append(qsTemp);
            QString outStr = qsl.join(';');
            //outStr.append("\n");
            outFile.write(qPrintable(outStr));
            ui->plainTextEdit->appendPlainText(outStr);
        }
    }
}


