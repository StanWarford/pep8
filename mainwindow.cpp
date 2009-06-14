#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pep.h"
#include "sim.h"

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

    // Dialog boxes setup
    redefineMnemonicsDialog = new RedefineMnemonicsDialog(this);

    helpDialog = new HelpDialog(this);
    connect(helpDialog, SIGNAL(clicked()), this, SLOT(helpCopyToSourceButtonClicked()));

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

    // Adjust initial configuration
    ui->horizontalSplitter->widget(2)->hide();
    ui->horizontalSplitter->widget(0)->resize(QSize(800,1)); // Enlarge Code/Trace pane on left.
    ui->codeSplitter->widget(0)->resize(QSize(1, 800)); // Enlarge Source Code pane.
    ui->middleSplitter->widget(1)->resize(QSize(1, 600)); // Enlarge Input pane.
    ui->horizontalSplitter->widget(2)->resize(QSize(memoryDumpPane->memDumpPaneWidth(), 1));

    // Save system setup
    readSettings();

    // Mac title bar
    setUnifiedTitleAndToolBarOnMac(true);

    // Focus highlighting
    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(highlightLabel(QWidget*, QWidget*)));
    // Undo/redo highlight thing
    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(actionsDeActivate(QWidget*,QWidget*)));

    // Recent files
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    separatorAct = ui->menu_File->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        ui->menu_File->addAction(recentFileActs[i]);
    }
    updateRecentFileActions();

    // Testing updateCpu
    Sim::vonNeumannStep();

    cpuPane->updateCpu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Protected closeEvent
 void MainWindow::closeEvent(QCloseEvent *event)
 {
     if (maybeSaveSource() || maybeSaveObject()) {
         writeSettings();
         event->accept();
     } else {
         event->ignore();
     }
 }

// Save methods

bool MainWindow::saveSource()
{
    if (curSourceFile.isEmpty()) {
        return on_actionFile_Save_Source_As_triggered();
    } else {
        return saveFileSource(curSourceFile);
    }
}

bool MainWindow::saveObject()
{
    if (curObjectFile.isEmpty()) {
        return on_actionFile_Save_Object_As_triggered();
    } else {
        return saveFileObject(curObjectFile);
    }
}

void MainWindow::readSettings()
{
    QSettings settings("Pep/8", "Dialog");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("Pep/8", "Dialog");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSaveSource()
{
    if (sourceCodePane->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "Pep/8",
                                   "The source code has been modified.\n"
                                   "Do you want to save your changes?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveSource();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::maybeSaveObject()
{
    if (objectCodePane->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "Pep/8",
                                   "The object code has been modified.\n"
                                   "Do you want to save your changes?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveObject();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QRegExp rx("*.pepo");
    QString pane = "";
    if (rx.exactMatch(fileName)) {
        // Set object code pane text
        objectCodePane->setObjectCodePaneText(in.readAll());
        pane = "Object";
    } else {
        // Set source code pane text
        sourceCodePane->setSourceCodePaneText(in.readAll());
        pane = "Source";
    }
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName, pane);
    statusBar()->showMessage(tr("File loaded"), 4000);
}

bool MainWindow::saveFileSource(const QString &fileName)
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

    setCurrentFile(fileName, "Source");
    statusBar()->showMessage("Source saved", 4000);
    return true;
}

bool MainWindow::saveFileObject(const QString &fileName) // Copied and pasted, change.
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
    out << objectCodePane->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName, "Object");
    statusBar()->showMessage("Object code saved", 4000);
    return true;
}

bool MainWindow::saveFileListing(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Pep/8"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << assemblerListingPane->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName, "Listing");
    statusBar()->showMessage("Assembler listing saved", 4000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName, QString pane)
{
    if (pane == "Source") {
        curSourceFile = fileName;
        sourceCodePane->setModifiedFalse();
    } else if (pane == "Object") {
        curObjectFile = fileName;
        objectCodePane->setModifiedFalse();
    } else if (pane == "Listing") {
        curListingFile = fileName;
    }

    QString shownName;
    if (pane == "Source") {
        if (curSourceFile.isEmpty()) {
            shownName = "untitled.pep";
        } else {
            shownName = strippedName(curSourceFile);
        }
        sourceCodePane->setCurrentFile(shownName);
        if (curListingFile.isEmpty()) {
            if (curSourceFile.isEmpty()) {
                curListingFile = "untitled.pepl";
            }
            else {
                QString temp = curSourceFile;
                if (temp.endsWith(".pep", Qt::CaseInsensitive) || temp.endsWith(".txt", Qt::CaseInsensitive)) {
                    temp.chop(4);
                }
                temp.append(".pepl");
                curListingFile = temp;
            }
        }
        if (curObjectFile.isEmpty()) {
            // Set curObjectFile name here.
            // Things to watch out for:
            // 1) We don't want to overwrite a file by the same name as the curSourceFile sans extension by accident
            // 2) We want all dialogs to open to the same directory
        }
    } else if (pane == "Object") {
        if (curObjectFile.isEmpty()) {
            shownName = "untitled.pepo";
        } else {
            shownName = strippedName(curObjectFile);
        }
        objectCodePane->setCurrentFile(shownName);
    }

    // For recent files:
    if (pane != "Listing") {
        QSettings settings("Pep/8", "Recent Files");
        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
            files.removeLast();

        settings.setValue("recentFileList", files);

        foreach (QWidget *widget, QApplication::topLevelWidgets()) {
            MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
            if (mainWin)
                mainWin->updateRecentFileActions();
        }
    }
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

// Recent files:
void MainWindow::updateRecentFileActions()
{
    QSettings settings("Pep/8", "Recent Files");
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}


// File MainWindow triggers
void MainWindow::on_actionFile_New_triggered()
{
    if (maybeSaveSource()) {
        sourceCodePane->clearSourceCode();
        setCurrentFile("", "Source");
    }
}
void MainWindow::on_actionFile_Open_triggered()
{
    if (maybeSaveSource()) {
        QString fileName = QFileDialog::getOpenFileName(
                this,
                "Open text file",
                "",
                "Text files (*.txt *.pep *.pepo)");
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::on_actionFile_Save_Source_triggered()
{
    if (curSourceFile.isEmpty()) {
        return on_actionFile_Save_Source_As_triggered();
    } else {
        return saveFileSource(curSourceFile);
    }
}

bool MainWindow::on_actionFile_Save_Object_triggered()
{
    if (curObjectFile.isEmpty()) {
        return on_actionFile_Save_Object_As_triggered();
    } else {
        return saveFileObject(curObjectFile);
    }
}

bool MainWindow::on_actionFile_Save_Source_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,
            "Save Source Code",
            curSourceFile.isEmpty() ? "untitled.pep" : curSourceFile,
            "Pep8 Source (*.pep *.txt)");
    if (fileName.isEmpty())
        return false;

    return saveFileSource(fileName);
}

bool MainWindow::on_actionFile_Save_Object_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,
            "Save Object Code",
            curObjectFile.isEmpty() ? "untitled.pepo" : curObjectFile,
            "Pep8 Object (*.pepo *.txt)");
    if (fileName.isEmpty())
        return false;

    return saveFileObject(fileName);
}

bool MainWindow::on_actionFile_Save_Listing_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,
            "Save Assembler Listing",
            curListingFile.isEmpty() ? "untitled.pepl" : curListingFile,
            "Pep8 Listing (*.pepl)");
    if (fileName.isEmpty()) {
        return false;
    }
    return saveFileListing(fileName);
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
    if (sourceCodePane->hasFocus()) {
        sourceCodePane->undo();
    }
    else if (objectCodePane->hasFocus()) {
        objectCodePane->undo();
    }
    else if (assemblerListingPane->hasFocus()) {
        assemblerListingPane->undo();
    }
    else if (listingTracePane->hasFocus()) {
        listingTracePane->undo();
    }
    else if (memoryTracePane->hasFocus()) {
        memoryTracePane->undo();
    }
    else if (cpuPane->hasFocus()) {
        cpuPane->undo();
    }
    else if (inputPane->hasFocus()) {
        inputPane->undo();
    }
    else if (outputPane->hasFocus()) {
        outputPane->undo();
    }
    else if (terminalPane->hasFocus()) {
        terminalPane->undo();
    }
    else if (memoryDumpPane->hasFocus()) {
        memoryDumpPane->undo();
    }
}

void MainWindow::on_actionEdit_Redo_triggered()
{
    if (sourceCodePane->hasFocus()) {
        sourceCodePane->redo();
    }
    else if (objectCodePane->hasFocus()) {
        objectCodePane->redo();
    }
    else if (assemblerListingPane->hasFocus()) {
        assemblerListingPane->redo();
    }
    else if (listingTracePane->hasFocus()) {
        listingTracePane->redo();
    }
    else if (memoryTracePane->hasFocus()) {
        memoryTracePane->redo();
    }
    else if (cpuPane->hasFocus()) {
        cpuPane->redo();
    }
    else if (inputPane->hasFocus()) {
        inputPane->redo();
    }
    else if (outputPane->hasFocus()) {
        outputPane->redo();
    }
    else if (terminalPane->hasFocus()) {
        terminalPane->redo();
    }
    else if (memoryDumpPane->hasFocus()) {
        memoryDumpPane->redo();
    }
}

void MainWindow::on_actionEdit_Cut_triggered()
{
    if (sourceCodePane->hasFocus()) {
        sourceCodePane->cut();
    }
    else if (objectCodePane->hasFocus()) {
        objectCodePane->cut();
    }
    else if (assemblerListingPane->hasFocus()) {
        assemblerListingPane->cut();
    }
    else if (listingTracePane->hasFocus()) {
        listingTracePane->cut();
    }
    else if (memoryTracePane->hasFocus()) {
        memoryTracePane->cut();
    }
    else if (cpuPane->hasFocus()) {
        cpuPane->cut();
    }
    else if (inputPane->hasFocus()) {
        inputPane->cut();
    }
    else if (outputPane->hasFocus()) {
        outputPane->cut();
    }
    else if (terminalPane->hasFocus()) {
        terminalPane->cut();
    }
    else if (memoryDumpPane->hasFocus()) {
        memoryDumpPane->cut();
    }
}

void MainWindow::on_actionEdit_Copy_triggered()
{
    if (sourceCodePane->hasFocus()) {
        sourceCodePane->copy();
    }
    else if (objectCodePane->hasFocus()) {
        objectCodePane->copy();
    }
    else if (assemblerListingPane->hasFocus()) {
        assemblerListingPane->copy();
    }
    else if (listingTracePane->hasFocus()) {
        listingTracePane->copy();
    }
    else if (memoryTracePane->hasFocus()) {
        memoryTracePane->copy();
    }
    else if (cpuPane->hasFocus()) {
        cpuPane->copy();
    }
    else if (inputPane->hasFocus()) {
        inputPane->copy();
    }
    else if (outputPane->hasFocus()) {
        outputPane->copy();
    }
    else if (terminalPane->hasFocus()) {
        terminalPane->copy();
    }
    else if (memoryDumpPane->hasFocus()) {
        memoryDumpPane->copy();
    }
}

void MainWindow::on_actionEdit_Paste_triggered()
{
    if (sourceCodePane->hasFocus()) {
        sourceCodePane->paste();
    }
    else if (objectCodePane->hasFocus()) {
        objectCodePane->paste();
    }
    else if (assemblerListingPane->hasFocus()) {
        assemblerListingPane->paste();
    }
    else if (listingTracePane->hasFocus()) {
        listingTracePane->paste();
    }
    else if (memoryTracePane->hasFocus()) {
        memoryTracePane->paste();
    }
    else if (cpuPane->hasFocus()) {
        cpuPane->paste();
    }
    else if (inputPane->hasFocus()) {
        inputPane->paste();
    }
    else if (outputPane->hasFocus()) {
        outputPane->paste();
    }
    else if (terminalPane->hasFocus()) {
        terminalPane->paste();
    }
    else if (memoryDumpPane->hasFocus()) {
        memoryDumpPane->paste();
    }
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
    QList<int> objectCodeList;
    if (objectCodePane->getObjectCode(objectCodeList)) {
        Sim::loadMem(objectCodeList);
        ui->statusbar->showMessage("Load succeeded", 4000);
    }
    else {
        ui->statusbar->showMessage("Load failed", 4000);
    }
}

void MainWindow::on_actionBuild_Execute_triggered()
{

}

void MainWindow::on_actionBuild_Run_triggered()
{
    cpuPane->runClicked();
}

void MainWindow::on_actionBuild_Start_Debugging_triggered()
{
    cpuPane->startDebuggingClicked();
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
    memoryDumpPane->refreshMemory();
    ui->horizontalSplitter->widget(0)->show();
    ui->horizontalSplitter->widget(1)->show();
    ui->horizontalSplitter->widget(2)->show();
}

void MainWindow::on_actionView_Code_Tab_triggered()
{
    ui->pepCodeTraceTab->setCurrentIndex(0);
}

void MainWindow::on_actionView_Trace_Tab_triggered()
{
    ui->pepCodeTraceTab->setCurrentIndex(1);
}

void MainWindow::on_actionView_Batch_I_O_Tab_triggered()
{
    ui->pepInputOutputTab->setCurrentIndex(0);
    if (ui->horizontalSplitter->widget(1)->isHidden()) {
        on_actionView_Code_CPU_triggered();
    }
}

void MainWindow::on_actionView_Terminal_Tab_triggered()
{
    ui->pepInputOutputTab->setCurrentIndex(1);
    if (ui->horizontalSplitter->widget(1)->isHidden()) {
        on_actionView_Code_CPU_triggered();
    }
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
    if (helpDialog->isHidden()) {
        helpDialog->setHidden(false);
    }
    else {
        helpDialog->raise();
        helpDialog->activateWindow();
    }
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

void MainWindow::helpCopyToSourceButtonClicked()
{
    if (maybeSaveSource()) {
        setCurrentFile("untitled.pep", "Source");
        sourceCodePane->setSourceCodePaneText(helpDialog->getLeftTextEditText());
        assemblerListingPane->clearAssemblerListing();
        objectCodePane->clearObjectCode();
        listingTracePane->clearListingTrace();
        statusBar()->showMessage("Copied to source", 4000);
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{

}

void MainWindow::slotByteConverterDecEdited(const QString &str)
{
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

// Focus Coloring
void MainWindow::highlightLabel(QWidget *, QWidget *)
{
    sourceCodePane->highlightOnFocus();
    objectCodePane->highlightOnFocus();
    assemblerListingPane->highlightOnFocus();
    listingTracePane->highlightOnFocus();
    memoryTracePane->highlightOnFocus();
    cpuPane->highlightOnFocus();
    inputPane->highlightOnFocus();
    outputPane->highlightOnFocus();
    terminalPane->highlightOnFocus();
    memoryDumpPane->highlightOnFocus();
}

// Activates and deactivates undo/redo/cut/copy/paste actions contextually
void MainWindow::actionsDeActivate(QWidget *, QWidget *)
{
    if (sourceCodePane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(false);
        ui->actionEdit_Redo->setDisabled(false);
        ui->actionEdit_Cut->setDisabled(false);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(false);
    }
    else if (objectCodePane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(false);
        ui->actionEdit_Redo->setDisabled(false);
        ui->actionEdit_Cut->setDisabled(false);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(false);
    }
    else if (assemblerListingPane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(true);
    }
    else if (listingTracePane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(true);
        ui->actionEdit_Paste->setDisabled(true);

    }
    else if (memoryTracePane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(true);
        ui->actionEdit_Paste->setDisabled(true);

    }
    else if (cpuPane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(true);
        ui->actionEdit_Paste->setDisabled(true);
    }
    else if (inputPane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(false);
        ui->actionEdit_Redo->setDisabled(false);
        ui->actionEdit_Cut->setDisabled(false);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(false);
    }
    else if (outputPane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(true);
    }
    else if (terminalPane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(false);
        ui->actionEdit_Redo->setDisabled(false);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(false);
    }
    else if (memoryDumpPane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(true);
    }
}

// Recent files
void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}
