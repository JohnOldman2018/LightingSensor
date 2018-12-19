#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>

static QFile outFile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    outFile.setFileName("C:/Users/opilane/Desktop/build-LightingSensor-Desktop_Qt_5_11_2_MinGW_32bit-Debug/short.log");
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

void MainWindow::writeToFile(QString writeLine, QString fileName)
{
    QFile outFile(fileName);
    if (!outFile.open(QIODevice::Append | QIODevice::Text)){
        qDebug() << "Cannot write to file.";
    }
    outFile.write(qPrintable(writeLine + "\n"));
    outFile.close();
}

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
            if (i%1000==1){
                qDebug() << "Parsed" << i << "lines from input file";
            }
        }
        qDebug() << "Parsed all " << i << "lines from input file";
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
        qDebug() << "Lux :" << lux << "unixTimeSec" << unixTimeMilSec << "dt: " << qsTemp << "Current hour: " << dt.time().hour();
        int h = dt.time().hour();
        ui->plainTextEdit->appendPlainText("Time is: " + qsTemp);

    //    int countQSL(h);
        qsl.append(qsTemp);
        for (int i = 0; i < qsl.size(); ++i) {
            qDebug() << "QSL at "<<i << " : "<< qsl.at(i);
        }
        QString outStr = qsl.join(';');
        outStr.append("\n");
        outFile.write(qPrintable(outStr));
    }
}

static double countLighting;
int MainWindow::countQSL(int hour)
{
        countLighting = 0;

    ui->plainTextEdit->appendPlainText("Time is: " + hour);

}
