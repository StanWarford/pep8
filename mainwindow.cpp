#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pep.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);

    // Left pane setup
    sourceCodePane = new SourceCodePane(ui->codeSplitter);
    delete ui->SourceCodeWidgetPane;
    objectCodePane = new ObjectCodePane(ui->codeSplitter);
    delete ui->ObjectCodeWidgetPane;
    assemblerListingPane = new AssemblerListingPane(ui->codeSplitter);
    delete ui->AssemblerListingWidgetPane;
    listingTracePane = new ListingTracePane(ui->traceSplitter);
    delete ui->ListingTraceWidgetPane;
    memoryTracePane = new MemoryTracePane(ui->traceSplitter);
    delete ui->MemoryTraceWidgetPane;
    ui->pepCodeTraceTab->setCurrentIndex(0);

    // Middle pane setup
    cpuPane = new CpuPane(ui->middleSplitter);
    ui->middleSplitter->insertWidget(0, cpuPane); //moves cpu widget to index 0 (above I/O tabs)
    delete ui->CpuWidgetPane;
    inputPane = new InputPane(ui->batchSplitter);
    delete ui->InputWidgetPane;
    outputPane = new OutputPane(ui->batchSplitter);
    delete ui->OutputWidgetPane;
    terminalPane = new TerminalPane(ui->terminalSplitter);
    delete ui->TerminalWidgetPane;
    ui->pepInputOutputTab->setCurrentIndex(0);

    // Right pane setup
    memoryDumpPane = new MemoryDumpPane(ui->horizontalSplitter);
    delete ui->MemoryDumpWidgetPane;

    // Adjust initial configuration
    ui->horizontalSplitter->widget(2)->hide();
    ui->horizontalSplitter->widget(0)->resize(QSize(500,1));
    ui->middleSplitter->widget(1)->resize(QSize(1, 400));

    // Dialog boxes setup
    redefineMnemonicsDialog = new RedefineMnemonicsDialog(this);
    redefineMnemonicsDialog->hide();

    helpDialog = new HelpDialog(this);
    helpDialog->hide();

    // Byte converter setup
    byteConverterDec = new ByteConverterDec();
    ui->byteConverterToolBar->addWidget(byteConverterDec);
    byteConverterHex = new ByteConverterHex();
    ui->byteConverterToolBar->addWidget(byteConverterHex);
    byteConverterBin = new ByteConverterBin();
    ui->byteConverterToolBar->addWidget(byteConverterBin);
    byteConverterChar = new ByteConverterChar();
    ui->byteConverterToolBar->addWidget(byteConverterChar);
    QObject::connect(byteConverterDec, SIGNAL(textEdited(const QString &)), this,
                     SLOT(slotByteConverterDecEdited(const QString &)));
    QObject::connect(byteConverterHex, SIGNAL(textEdited(const QString &)), this,
                     SLOT(slotByteConverterHexEdited(const QString &)));
    QObject::connect(byteConverterBin, SIGNAL(textEdited(const QString &)), this,
                     SLOT(slotByteConverterBinEdited(const QString &)));
    QObject::connect(byteConverterChar, SIGNAL(textEdited(const QString &)), this,
                     SLOT(slotByteConverterCharEdited(const QString &)));

    // Pep tables setup
    Pep::initEnumMnemonMaps();
    Pep::initAddrModesMap();
    Pep::initMnemonicMaps();
    Pep::initDecoderTables();
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

void MainWindow::on_actionFile_Save_Source_triggered()
{

}

void MainWindow::on_actionFile_Save_Object_triggered()
{

}

void MainWindow::on_actionFile_Save_Listing_triggered()
{

}

void MainWindow::on_actionFile_Save_Source_As_triggered()
{

}

void MainWindow::on_actionFile_Save_Object_As_triggered()
{

}

void MainWindow::on_actionFile_Save_Listing_As_triggered()
{

}

void MainWindow::on_actionFile_Print_Source_triggered()
{

}

void MainWindow::on_actionFile_Print_Object_triggered()
{

}

void MainWindow::on_actionFile_Print_Listing_triggered()
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
    if (sourceCodePane->assemble()) {
        ui->statusbar->showMessage("Assembly succeeded", 4000);
        objectCodePane->setObjectCode(sourceCodePane->getObjectCode());
        assemblerListingPane->setAssemblerListing(sourceCodePane->getAssemblerListingList());
        listingTracePane->setListingTrace(sourceCodePane->getAssemblerListingList(), sourceCodePane->getHasCheckBox());
    }
    else {
        ui->statusbar->showMessage("Assembly failed", 4000);
    }

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
    sourceCodePane->removeErrorMessages();
}


// View MainWindow triggers
void MainWindow::on_actionView_Code_Only_triggered()
{
    ui->horizontalSplitter->widget(0)->show();
    ui->horizontalSplitter->widget(1)->hide();
    ui->horizontalSplitter->widget(2)->hide();
}

void MainWindow::on_actionView_Code_CPU_triggered()
{
    ui->horizontalSplitter->widget(0)->show();
    ui->horizontalSplitter->widget(1)->show();
    ui->horizontalSplitter->widget(2)->hide();

}

void MainWindow::on_actionView_Code_CPU_Memory_triggered()
{
    ui->horizontalSplitter->widget(0)->show();
    ui->horizontalSplitter->widget(1)->show();
    ui->horizontalSplitter->widget(2)->show();
}

void MainWindow::on_actionView_Code_Memory_triggered()
{
    ui->horizontalSplitter->widget(0)->show();
    ui->horizontalSplitter->widget(1)->hide();
    ui->horizontalSplitter->widget(2)->show();
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
    helpDialog->show();
    helpDialog->on_menuItem_AssemblyLanguage_clicked();
}

void MainWindow::on_actionHelp_Machine_Language_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_MachineLanguage_clicked();
}

void MainWindow::on_actionHelp_Break_Points_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_BreakPoints_clicked();
}

void MainWindow::on_actionHelp_Symbolic_Trace_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_SymbolicTrace_clicked();
}

void MainWindow::on_actionHelp_Byte_Converter_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_ByteConverter_clicked();
}

void MainWindow::on_actionHelp_Writing_Interrupt_Handlers_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_InterruptHandlers_clicked();
}

void MainWindow::on_actionHelp_Instruction_Set_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_InstructionSet_clicked();
}

void MainWindow::on_actionHelp_Dot_Commands_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_DotCommands_clicked();
}

void MainWindow::on_actionHelp_Addressing_Modes_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_AddressingModes_clicked();
}

void MainWindow::on_actionHelp_Examples_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_Examples_clicked();
}

void MainWindow::on_actionAbout_Pep8_triggered()
{
    helpDialog->show();
    helpDialog->on_menuItem_OperatingSystem_clicked();
}

void MainWindow::on_actionAbout_Qt_triggered() {}

void MainWindow::slotByteConverterDecEdited(const QString &str) {
    if (str.length() > 0) {
        bool ok;
        int data = str.toInt(&ok, 10);
        byteConverterHex->setValue(data);
        byteConverterBin->setValue(data);
        byteConverterChar->setValue(data);
    }
}

void MainWindow::slotByteConverterHexEdited(const QString &str)
{
    if (str.length() >= 2) {
        if (str.startsWith("0x")) {
            QString hexPart = str;
            hexPart.remove(0, 2);
            if (hexPart.length() > 0) {
                bool ok;
                int data = hexPart.toInt(&ok, 16);
                byteConverterDec->setValue(data);
                byteConverterBin->setValue(data);
                byteConverterChar->setValue(data);
            }
            else {
                // Exactly "0x" remains, so do nothing
            }
        }
        else {
            // Prefix "0x" was mangled
            byteConverterHex->setValue(-1);
        }
    }
    else {
        // Prefix "0x" was shortened
        byteConverterHex->setValue(-1);
    }
}

void MainWindow::slotByteConverterBinEdited(const QString &str)
{
    if (str.length() > 0) {
        bool ok;
        int data = str.toInt(&ok, 2);
        byteConverterDec->setValue(data);
        byteConverterHex->setValue(data);
        byteConverterChar->setValue(data);
    }
}

void MainWindow::slotByteConverterCharEdited(const QString &str)
{
    if (str.length() > 0) {
        int data = (int)str[0].toAscii();
        byteConverterDec->setValue(data);
        byteConverterHex->setValue(data);
        byteConverterBin->setValue(data);
    }
}
