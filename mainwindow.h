#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;
    void writeToFile(QString writeLine, QString fileName);
    void readFromFile(QString fileName);
};

#endif // MAINWINDOW_H
