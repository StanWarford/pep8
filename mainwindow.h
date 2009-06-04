#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "byteconverterdec.h"
#include "byteconverterhex.h"
#include "byteconverterbin.h"
#include "byteconverterchar.h"

// Left pane
#include "sourcecodepane.h"
#include "objectcodepane.h"
#include "assemblerlistingpane.h"
#include "listingtracepane.h"
#include "memorytracepane.h"

// Middle pane
#include "cpupane.h"
#include "inputpane.h"
#include "outputpane.h"
#include "terminalpane.h"

// Right pane
#include "memorydumppane.h"

// Dialog boxes
#include "redefinemnemonicsdialog.h"
#include "helpdialog.h"

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

    // Left pane
    SourceCodePane *sourceCodePane;
    ObjectCodePane *objectCodePane;
    AssemblerListingPane *assemblerListingPane;
    ListingTracePane *listingTracePane;
    MemoryTracePane *memoryTracePane;

    // Middle pane
    CpuPane *cpuPane;
    InputPane *inputPane;
    OutputPane *outputPane;
    TerminalPane *terminalPane;

    // Right pane
    MemoryDumpPane *memoryDumpPane;

    // Dialog boxes
    RedefineMnemonicsDialog *redefineMnemonicsDialog;
    HelpDialog *helpDialog;

    // Byte converter
    ByteConverterDec *byteConverterDec;
    ByteConverterHex *byteConverterHex;
    ByteConverterBin *byteConverterBin;
    ByteConverterChar *byteConverterChar;

    // Save methods
    bool save();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString curFile;

private slots:
    // File
    void on_actionFile_New_triggered();
    void on_actionFile_Open_triggered();
    void on_actionFile_Open_Recent_triggered();
    void on_actionFile_Save_Source_triggered();
    void on_actionFile_Save_Object_triggered();
    void on_actionFile_Save_Listing_triggered();
    bool on_actionFile_Save_Source_As_triggered();
    void on_actionFile_Save_Object_As_triggered();
    void on_actionFile_Save_Listing_As_triggered();
    void on_actionFile_Print_Source_triggered();
    void on_actionFile_Print_Object_triggered();
    void on_actionFile_Print_Listing_triggered();

    // Edit
    void on_actionEdit_Undo_triggered();
    void on_actionEdit_Redo_triggered();
    void on_actionEdit_Cut_triggered();
    void on_actionEdit_Copy_triggered();
    void on_actionEdit_Paste_triggered();
    void on_actionEdit_Font_triggered();
    void on_actionFormat_From_Listing_triggered();

    // Build
    void on_actionBuild_Assemble_triggered();
    void on_actionBuild_Load_triggered();
    void on_actionBuild_Execute_triggered();
    void on_actionBuild_Run_triggered();
    void on_actionBuild_Start_Debugging_triggered();
    void on_actionBuild_Remove_Error_Messages_triggered();

    // View
    void on_actionView_Code_Only_triggered();
    void on_actionView_Code_CPU_triggered();
    void on_actionView_Code_CPU_Memory_triggered();
    void on_actionView_Code_Memory_triggered();

    // System
    void on_actionSystem_Redefine_Mnemonics_triggered();
    void on_actionSystem_Assemble_Install_New_OS_triggered();
    void on_actionSystem_Reinstall_Default_OS_triggered();
    void on_actionSystem_Set_Execution_Limits_triggered();

    // Help
    void on_actionHelp_triggered();
    void on_actionHelp_Assembly_Language_triggered();
    void on_actionHelp_Machine_Language_triggered();
    void on_actionHelp_Break_Points_triggered();
    void on_actionHelp_Symbolic_Trace_triggered();
    void on_actionHelp_Byte_Converter_triggered();
    void on_actionHelp_Writing_Interrupt_Handlers_triggered();
    void on_actionHelp_Instruction_Set_triggered();
    void on_actionHelp_Dot_Commands_triggered();
    void on_actionHelp_Addressing_Modes_triggered();
    void on_actionHelp_Examples_triggered();
    void on_actionPep_8_Operating_System_triggered();
    void on_actionAbout_Pep8_triggered();
    void on_actionAbout_Qt_triggered();

    void helpCopyToSourceButtonClicked();

    // Byte converter
    void slotByteConverterDecEdited(const QString &);
    void slotByteConverterHexEdited(const QString &);
    void slotByteConverterBinEdited(const QString &);
    void slotByteConverterCharEdited(const QString &);

    /*
    These belong in the other panes
    // Trace
    void on_pepCpuSingleStepPushButton_clicked();
    void on_pepMemSingleStepPushButton_clicked();
    void on_pepCpuResumePushButton_clicked();
    void on_pepMemResumePushButton_clicked();
    void pepResumePushButtonClicked();

    // Memory pane
    void on_pepMemRefreshPushButton_clicked();

    // CPU stats
    void slotSaveTraceProgram(int);
    void slotSaveTraceTraps(int);
    void slotSaveTraceLoader(int);
*/

};

#endif // MAINWINDOW_H
