#ifndef SIMULATEWINDOW_H
#define SIMULATEWINDOW_H

#include <QMainWindow>

namespace Ui {
class SimulateWindow;
}

class MainWindow;

class SimulateWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulateWindow(QWidget *parent = 0);
    ~SimulateWindow();
    void getPointers(MainWindow *);
    void getFiles(const QString, const QString, const QString, const QString);

private:
    Ui::SimulateWindow *ui;
    MainWindow *mainWindow;
    void closeEvent(QCloseEvent *);
};

#endif // SIMULATEWINDOW_H
