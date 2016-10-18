#include "SimulateWindow.h"
#include "ui_SimulateWindow.h"
#include "Simulator.h"
#include "AssemblyParser.h"
#include "GLOBALS.h"
#include "MainWindow.h"

SimulateWindow::SimulateWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulateWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textBrowser);
    QString windowTitle = "Simulate";
    setWindowTitle(windowTitle);
}

SimulateWindow::~SimulateWindow()
{
    delete ui;
}

void SimulateWindow::getPointers(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}

void SimulateWindow::getFiles(const QString assemblyCodeFile, const QString textDumpFile, const QString memoryDumpFile, const QString registerUpdateFile)
{
    ui->textBrowser->clear();
    Simulator simulator;
    simulator.memory.getPointers(ui->textBrowser);
    AssemblyParser assemblyParser(assemblyCodeFile.toStdString(), textDumpFile.toStdString(), memoryDumpFile.toStdString(), simulator.memory, simulator.program, ui->textBrowser);
    if (!TERMINATE)
        simulator.simulate(registerUpdateFile.toStdString(), ui->textBrowser);
}

void SimulateWindow::closeEvent(QCloseEvent *closeEvent)
{
    mainWindow->show();
}
