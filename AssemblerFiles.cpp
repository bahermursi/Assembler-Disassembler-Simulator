#include "AssemblerFiles.h"
#include "ui_AssemblerFiles.h"
#include "MainWindow.h"

AssemblerFiles::AssemblerFiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AssemblerFiles)
{
    ui->setupUi(this);
    QString windowTitle = "Assembler Files";
    setWindowTitle(windowTitle);
}

AssemblerFiles::~AssemblerFiles()
{
    delete ui;
}

void AssemblerFiles::getPointers(MainWindow *mainWindow, SimulateWindow *simulateWindow)
{
    this->mainWindow = mainWindow;
    this->simulateWindow = simulateWindow;
}

void AssemblerFiles::on_pushButton_clicked()
{
    if (ui->assemblyCodeFile->text() != "")
    {
        simulateWindow->getPointers(mainWindow);
        simulateWindow->getFiles(ui->assemblyCodeFile->text(), ui->textDumpFile->text(), ui->memoryDumpFile->text(), ui->registerUpdateFile->text());
        simulateWindow->show();
        close();
    }
}
void AssemblerFiles::on_textDumpFile_editingFinished()
{
    QString textDumpFile = "TextDump.bin";
    if (ui->textDumpFile->text() != textDumpFile && ui->textDumpFile->text() != "")
        ui->textDumpFileSetToDefault->hide();
    else
    {
        ui->textDumpFile->clear();
        ui->textDumpFile->setText(textDumpFile);
        ui->textDumpFileSetToDefault->show();
    }
}

void AssemblerFiles::on_memoryDumpFile_editingFinished()
{
    QString memoryDumpFile = "MemoryDump.bin";
    if (ui->memoryDumpFile->text() != memoryDumpFile && ui->memoryDumpFile->text() != "")
        ui->memoryDumpFileSetToDefault->hide();
    else
    {
        ui->memoryDumpFile->clear();
        ui->memoryDumpFile->setText(memoryDumpFile);
        ui->memoryDumpFileSetToDefault->show();
    }
}

void AssemblerFiles::on_registerUpdateFile_editingFinished()
{
    QString registerUpdateFile = "RegisterUpdate.txt";
    if (ui->registerUpdateFile->text() != registerUpdateFile && ui->registerUpdateFile->text() != "")
        ui->registerUpdateFileSetToDefault->hide();
    else
    {
        ui->registerUpdateFile->clear();
        ui->registerUpdateFile->setText(registerUpdateFile);
        ui->registerUpdateFileSetToDefault->show();
    }
}
