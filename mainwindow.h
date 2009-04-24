#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

// Left
#include "sourcecodepane.h"
#include "objectcodepane.h"
#include "assemblerlistingpane.h"
#include "memorytracepane.h"

// Middle
#include "cpupane.h"
#include "inputpane.h"
#include "outputpane.h"
#include "terminalpane.h"

// Right
#include "memorydumppane.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;
    // Left
    SourceCodePane *sourceCodePane;
    ObjectCodePane *objectCodePane;
    AssemblerListingPane *assemblerListingPane;
    MemoryTracePane *memoryTracePane;

    // Middle
    CpuPane *cpuPane;
    InputPane *inputPane;
    OutputPane *outputPane;
    TerminalPane *terminalPane;

    // Right
    MemoryDumpPane *memoryDumpPane;
};

#endif // MAINWINDOW_H
