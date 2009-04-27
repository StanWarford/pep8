#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    // Left side setup
    sourceCodePane = new SourceCodePane(ui->codeSplitter);
    delete ui->SourceCodeWidgetPane;
    objectCodePane = new ObjectCodePane(ui->codeSplitter);
    delete ui->ObjectCodeWidgetPane;
    assemblerListingPane = new AssemblerListingPane(ui->leftSplitter);
    delete ui->AssemblerListingWidgetPane;
    memoryTracePane = new MemoryTracePane(ui->traceSplitter);
    delete ui->MemoryTraceWidgetPane;

    // Middle setup
    cpuPane = new CpuPane(ui->middleSplitter);
    ui->middleSplitter->insertWidget(0, cpuPane); //moves cpu widget to index 0 (above I/O tabs)
    delete ui->CpuWidgetPane;
    inputPane = new InputPane(ui->batchSplitter);
    delete ui->InputWidgetPane;
    outputPane = new OutputPane(ui->batchSplitter);
    delete ui->OutputWidgetPane;
    terminalPane = new TerminalPane(ui->terminalSplitter);
    delete ui->TerminalWidgetPane;

    // Right setup
    memoryDumpPane = new MemoryDumpPane(ui->horizontalSplitter);
    delete ui->MemoryDumpWidgetPane;

    // Dialog boxes setup
    redefineMnemonicsDialog = new RedefineMnemonicsDialog(this);
    redefineMnemonicsDialog->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// File MainWindow triggers
void MainWindow::on_actionFile_New_triggered()
{

}
void MainWindow::on_actionFile_Open_triggered()
{

}

void MainWindow::on_actionFile_Save_triggered()
{

}

void MainWindow::on_actionFile_Save_As_triggered()
{

}

void MainWindow::on_actionFile_Print_triggered()
{

}


// Edit MainWindow triggers
void MainWindow::on_actionEdit_Undo_triggered()
{

}

void MainWindow::on_actionEdit_Redo_triggered()
{

}

void MainWindow::on_actionEdit_Cut_triggered()
{

}

void MainWindow::on_actionEdit_Copy_triggered()
{

}

void MainWindow::on_actionEdit_Paste_triggered()
{

}

void MainWindow::on_actionEdit_Font_triggered()
{

}

// Build MainWindow triggers
void MainWindow::on_actionBuild_Assemble_triggered()
{

}

void MainWindow::on_actionBuild_Load_triggered()
{

}

void MainWindow::on_actionBuild_Execute_triggered()
{

}

void MainWindow::on_actionBuild_Run_triggered()
{

}

void MainWindow::on_actionBuild_Start_Debugging_triggered()
{

}

void MainWindow::on_actionBuild_Remove_Error_Messages_triggered()
{

}


// View MainWindow triggers
void MainWindow::on_actionView_Code_Only_triggered()
{

}

void MainWindow::on_actionView_Code_CPU_triggered()
{

}

void MainWindow::on_actionView_Code_CPU_Memory_triggered()
{

}

void MainWindow::on_actionView_Code_Memory_triggered()
{

}


// System MainWindow triggers
void MainWindow::on_actionSystem_Redefine_Mnemonics_triggered()
{
    redefineMnemonicsDialog->show();
}

void MainWindow::on_actionSystem_Assemble_Install_New_OS_triggered()
{

}

void MainWindow::on_actionSystem_Reinstall_Default_OS_triggered()
{

}

void MainWindow::on_actionSystem_Set_Execution_Limits_triggered()
{

}


// Help MainWindow triggers
void MainWindow::on_actionHelp_Assembly_Language_triggered()
{

}

void MainWindow::on_actionHelp_Machine_Language_triggered()
{

}

void MainWindow::on_actionHelp_Break_Points_triggered()
{

}

void MainWindow::on_actionHelp_Symbolic_Trace_triggered()
{

}

void MainWindow::on_actionHelp_Byte_Converter_triggered()
{

}

void MainWindow::on_actionHelp_Writing_Interrupt_Handlers_triggered()
{

}

void MainWindow::on_actionHelp_Instruction_Set_triggered()
{

}

void MainWindow::on_actionHelp_Dot_Commands_triggered()
{

}

void MainWindow::on_actionHelp_Addressing_Modes_triggered()
{

}

void MainWindow::on_actionHelp_Examples_triggered()
{

}

void MainWindow::on_actionAbout_Pep8_triggered()
{

}

void MainWindow::on_actionAbout_Qt_triggered()
{

}

