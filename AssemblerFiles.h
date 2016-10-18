#ifndef ASSEMBLERFILES_H
#define ASSEMBLERFILES_H

#include "SimulateWindow.h"

#include <QDialog>

namespace Ui {
class AssemblerFiles;
}

class MainWindow;

class AssemblerFiles : public QDialog
{
    Q_OBJECT

public:
    explicit AssemblerFiles(QWidget *parent = 0);
    ~AssemblerFiles();
    void getPointers(MainWindow *, SimulateWindow *);

private slots:
    void on_pushButton_clicked();

    void on_textDumpFile_editingFinished();

    void on_memoryDumpFile_editingFinished();

    void on_registerUpdateFile_editingFinished();

private:
    Ui::AssemblerFiles *ui;
    MainWindow *mainWindow;
    SimulateWindow *simulateWindow;
};

#endif // ASSEMBLERFILES_H
