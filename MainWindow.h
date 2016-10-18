#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SimulateWindow.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_assemblerButton_clicked();

private:
    Ui::MainWindow *ui;
    SimulateWindow simulateWindow;
};

#endif // MAINWINDOW_H
