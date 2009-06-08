#include <QMessageBox>
#include <QFileDialog>
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
    ui->middleSplitter->insertWidget(0, cpuPane); // Move cpu pane to index 0 (above I/O tabs).
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
    ui->horizontalSplitter->widget(0)->resize(QSize(800,1)); // Enlarge Code/Trace pane on left.
    ui->codeSplitter->widget(0)->resize(QSize(1, 800)); // Enlarge Source Code pane.
    ui->middleSplitter->widget(1)->resize(QSize(1, 600)); // Enlarge Input pane.

    // Dialog boxes setup
    redefineMnemonicsDialog = new RedefineMnemonicsDialog(this);
    redefineMnemonicsDialog->hide();

    helpDialog = new HelpDialog(this);
    QObject::connect(helpDialog, SIGNAL(clicked()), this, SLOT(helpCopyToSourceButtonClicked()));
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

// Save methods

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return on_actionFile_Save_Source_As_triggered();
    } else {
        return saveFile(curFile);
    }
}

void MainWindow::readSettings() {}
void MainWindow::writeSettings() {}

bool MainWindow::maybeSave()
{
    if (sourceCodePane->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "Pep/8",
                                   "The source code has been modified.\n"
                                   "Do you want to save your changes?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{

}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << sourceCodePane->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    sourceCodePane->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


// File MainWindow triggers
void MainWindow::on_actionFile_New_triggered()
{
    if (maybeSave()) {
        sourceCodePane->clearSourceCodePane();
        setCurrentFile("");
    }
}
void MainWindow::on_actionFile_Open_triggered()
{

}

void MainWindow::on_actionFile_Open_Recent_triggered()
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

bool MainWindow::on_actionFile_Save_Source_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
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

void MainWindow::on_actionEdit_Format_From_Listing_triggered()
{
    QStringList assemblerListingList = sourceCodePane->getAssemblerListingList();
    assemblerListingList.replaceInStrings(QRegExp("^............."), "");
    assemblerListingList.removeAll("");
    sourceCodePane->setSourceCodePaneText(assemblerListingList.join("\n"));
}

void MainWindow::on_actionEdit_Font_triggered()
{

}

// Build MainWindow triggers
void MainWindow::on_actionBuild_Assemble_triggered()
{
    Pep::burnCount = 0;
    if (sourceCodePane->assemble()) {
        if (Pep::burnCount > 0) {
            QString errorString = ";ERROR: .BURN not allowed in program unless installing OS.";
            sourceCodePane->appendMessageInSourceCodePaneAt(0, errorString, Qt::red);
            assemblerListingPane->clearAssemblerListing();
            objectCodePane->clearObjectCode();
            listingTracePane->clearListingTrace();
            ui->statusbar->showMessage("Assembly failed", 4000);
        }
        else {
            objectCodePane->setObjectCode(sourceCodePane->getObjectCode());
            assemblerListingPane->setAssemblerListing(sourceCodePane->getAssemblerListingList());
            listingTracePane->setListingTrace(sourceCodePane->getAssemblerListingList(), sourceCodePane->getHasCheckBox());
            ui->statusbar->showMessage("Assembly succeeded", 4000);
        }
    }
    else {
        assemblerListingPane->clearAssemblerListing();
        objectCodePane->clearObjectCode();
        listingTracePane->clearListingTrace();
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
    Pep::burnCount = 0;
    if (sourceCodePane->assemble()) {
        if (Pep::burnCount == 0) {
            QString errorString = ";ERROR: .BURN required to install OS.";
            sourceCodePane->appendMessageInSourceCodePaneAt(0, errorString, Qt::red);
            assemblerListingPane->clearAssemblerListing();
            objectCodePane->clearObjectCode();
            listingTracePane->clearListingTrace();
            ui->statusbar->showMessage("Assembly failed", 4000);
        }
        else if (Pep::burnCount > 1) {
            QString errorString = ";ERROR: Program contain more than one .BURN.";
            sourceCodePane->appendMessageInSourceCodePaneAt(0, errorString, Qt::red);
            assemblerListingPane->clearAssemblerListing();
            objectCodePane->clearObjectCode();
            listingTracePane->clearListingTrace();
            ui->statusbar->showMessage("Assembly failed", 4000);
        }
        else {
            // Adjust for .BURN
            int addressDelta = Pep::dotBurnArgument - Pep::byteCount + 1;
            QMutableMapIterator <QString, int> i(Pep::symbolTable);
            while (i.hasNext()) {
                i.next();
                if (Pep::adjustSymbolValueForBurn.value(i.key())) {
                    i.setValue(i.value() + addressDelta);
                }
            }
            sourceCodePane->adjustCodeList(addressDelta);
            Pep::romStartAddress += addressDelta;
            objectCodePane->setObjectCode(sourceCodePane->getObjectCode());
            assemblerListingPane->setAssemblerListing(sourceCodePane->getAssemblerListingList());
            listingTracePane->setListingTrace(sourceCodePane->getAssemblerListingList(), sourceCodePane->getHasCheckBox());
            sourceCodePane->installOS();
            ui->statusbar->showMessage("Assembly succeeded, OS installed", 4000);
        }
    }
    else {
        assemblerListingPane->clearAssemblerListing();
        objectCodePane->clearObjectCode();
        listingTracePane->clearListingTrace();
        ui->statusbar->showMessage("Assembly failed", 4000);
    }
}

void MainWindow::on_actionSystem_Reinstall_Default_OS_triggered()
{

}

void MainWindow::on_actionSystem_Set_Execution_Limits_triggered()
{

}


// Help MainWindow triggers
void MainWindow::on_actionHelp_triggered()
{
    helpDialog->show();
    helpDialog->helpClicked();
}

void MainWindow::on_actionHelp_Assembly_Language_triggered()
{
    helpDialog->show();
    helpDialog->assemblyLanguageClicked();
}

void MainWindow::on_actionHelp_Machine_Language_triggered()
{
    helpDialog->show();
    helpDialog->machineLanguageClicked();
}

void MainWindow::on_actionHelp_Break_Points_triggered()
{
    helpDialog->show();
    helpDialog->breakPointsClicked();
}

void MainWindow::on_actionHelp_Symbolic_Trace_triggered()
{
    helpDialog->show();
    helpDialog->symbolicTraceClicked();
}

void MainWindow::on_actionHelp_Byte_Converter_triggered()
{
    helpDialog->show();
    helpDialog->byteConverterClicked();
}

void MainWindow::on_actionHelp_Writing_Interrupt_Handlers_triggered()
{
    helpDialog->show();
    helpDialog->interruptHandlersClicked();
}

void MainWindow::on_actionHelp_Instruction_Set_triggered()
{
    helpDialog->show();
    helpDialog->instructionSetClicked();
}

void MainWindow::on_actionHelp_Dot_Commands_triggered()
{
    helpDialog->show();
    helpDialog->dotCommandsClicked();
}

void MainWindow::on_actionHelp_Addressing_Modes_triggered()
{
    helpDialog->show();
    helpDialog->addressingModesClicked();
}

void MainWindow::on_actionHelp_Examples_triggered()
{
    helpDialog->show();
    helpDialog->examplesClicked();
}

void MainWindow::on_actionPep_8_Operating_System_triggered()
{
    helpDialog->show();
    helpDialog->operatingSystemClicked();
}

void MainWindow::on_actionAbout_Pep8_triggered()
{

}

void MainWindow::helpCopyToSourceButtonClicked() {
    if (maybeSave()) {
        setCurrentFile("");
        sourceCodePane->setSourceCodePaneText(helpDialog->getLeftTextEditText());
        assemblerListingPane->clearAssemblerListing();
        objectCodePane->clearObjectCode();
        listingTracePane->clearListingTrace();
    }
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
