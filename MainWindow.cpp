#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AssemblerFiles.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString windowTitle = "MIPS Assembler, Disassembler, and Simulator";
    setWindowTitle(windowTitle);
    setFixedSize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_assemblerButton_clicked()
{
    hide();
    AssemblerFiles assemblerFiles;
    assemblerFiles.getPointers(this, &simulateWindow);
    assemblerFiles.setModal(true);
    assemblerFiles.exec();
}
