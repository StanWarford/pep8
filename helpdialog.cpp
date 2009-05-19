#include "helpdialog.h"
#include "ui_helpdialog.h"

#include <QDebug>

HelpDialog::HelpDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->helpTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(on_itemClicked(QTreeWidgetItem*,int)));
//    QObject::connect(m_ui->helpTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(on_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}

enum Row {
    eWRITING = 0,
    eDEBUGGING = 1,
    eINTERRUPT = 2,
    eREFERENCE = 3,
    eEXAMPLES = 4,
    eOS = 5,

    eASSEMBLY = 1,
    eMACHINE = 0,

    eBREAK = 0,
    eSYMTRACE = 1,
    eBYTECONVERTER = 2,

    eINSTRUCTION = 0,
    eDOTCMD = 1,
    eADDRMODE = 2,

    eFIG518 = 0,
    eFIG521 = 1,
    eFIG526 = 2,
    eFIG601 = 3,
    eFIG604 = 4,
    eFIG606 = 5,
    eFIG608 = 6,
    eFIG610 = 7,
    eFIG612 = 8,
    eFIG614 = 9,
    eFIG616 = 10,
    eFIG618 = 11,
    eFIG621 = 12,
    eFIG623 = 13,
    eFIG625 = 14,
    eFIG627 = 15,
    eFIG629 = 16,
    eFIG632 = 17,
    eFIG634 = 18,
    eFIG636 = 19,
    eFIG638 = 20,
    eFIG640 = 21,
    eFIG641 = 22,
    eFIG643 = 23,
    eFIG645 = 24,
    eFIG647 = 25,

};



void HelpDialog::on_currentItemChanged(QTreeWidgetItem* item,QTreeWidgetItem*) {
    this->on_itemClicked(item,0);
}

void HelpDialog::on_itemClicked(QTreeWidgetItem*,int) {
//    qDebug() << "Is valid? : " << m_ui->helpTreeWidget->currentIndex().parent().isValid();
//    qDebug() << "Parent row: " << m_ui->helpTreeWidget->currentIndex().parent().row();
//    qDebug() << "Row: " << m_ui->helpTreeWidget->currentIndex().row();

    // Is this a subcategory?
    bool subCat = m_ui->helpTreeWidget->currentIndex().parent().isValid();
    // Parent row (if it has a parent, -1 else)
    int parent = m_ui->helpTreeWidget->currentIndex().parent().row();
    // Row (if it has a parent, this is the child row)
    int row = m_ui->helpTreeWidget->currentIndex().row();

    if ((!subCat && row == eWRITING) || parent == eWRITING) {
        m_ui->rightHelpWebView->hide();
        if (!subCat) {                          // Writing Programs
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
        } else if (row == eMACHINE) {           // Writing Programs > Machine Language
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/machinelanguage.html"));
        } else if (row == eASSEMBLY) {          // Writing Programs > Assembly Language
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
        }
    } else if ((!subCat && row == eDEBUGGING) || parent == eDEBUGGING) {
        m_ui->rightHelpWebView->hide();
        if (!subCat) {                          // Debugging Programs
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/debuggingprograms.html"));
        } else if (row == eBREAK) {             // Debugging Programs > Break Points
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/breakpoints.html"));
        } else if (row == eSYMTRACE) {          // Debugging Programs > Symbolic Trace
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/symbolictrace.html"));
        } else if (row == eBYTECONVERTER) {     // Debugging Programs > Byte Converter
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/byteconverter.html"));
        }
        m_ui->leftHelpWebView->show();
    } else if (!subCat && row == eINTERRUPT) {  // Writing Interrupt Handlers
        m_ui->rightHelpWebView->hide();
        m_ui->leftHelpWebView->load(QUrl("qrc:/help/interrupthandlers.html"));
    } else if ((!subCat && row == eREFERENCE) || parent == eREFERENCE) {
        m_ui->rightHelpWebView->hide();
        if (!subCat) {                          // Pep/8 Reference
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/pep8reference.html"));
        } else if (row == eINSTRUCTION) {       // Pep/8 Reference > Instruction Set
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/instructionset.html"));
        } else if (row == eDOTCMD) {            // Pep/8 Reference > Dot Commands
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/dotcommands.html"));
        } else if (row == eADDRMODE) {          // Pep/8 Reference > Addressing Modes
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/addressingmodes.html"));
        }
    } else if ((!subCat && row == eEXAMPLES) || parent == eEXAMPLES) {
        if (!subCat) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/examples.html"));
            m_ui->rightHelpWebView->hide();
        } else if (row == eFIG518) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0518.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0518.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG521) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0521.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0521.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG526) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0526.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0526.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG601) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0601.pep.html"));
            m_ui->rightHelpWebView->hide();
        } else if (row == eFIG604) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0604.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0604.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG606) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0606.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0606.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG608) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0608.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0608.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG610) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0610.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0610.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG612) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0612.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0612.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG614) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0614.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0614.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG616) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0616.pep.html"));
            m_ui->rightHelpWebView->hide();
        } else if (row == eFIG618) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0618.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0618.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG621) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0621.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0621.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG623) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0623.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0623.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG625) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0625.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0625.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG627) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0627.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0627.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG629) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0629.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0629.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG632) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0632.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0632.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG634) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0634.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0634.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG636) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0618.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0618.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG638) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0621.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0621.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG640) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0640.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0640.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG641) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0641.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0641.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG643) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0643.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0643.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG645) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0645.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0645.cpp.html"));
            m_ui->rightHelpWebView->show();
        } else if (row == eFIG647) {
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/figures/fig0647.pep.html"));
            m_ui->rightHelpWebView->load(QUrl("qrc:/help/figures/fig0647.cpp.html"));
            m_ui->rightHelpWebView->show();
        }
    } else if (!subCat && row == eOS) {         // Pep/8 Operating System
        m_ui->rightHelpWebView->hide();
        m_ui->leftHelpWebView->load(QUrl("qrc:/help/pep8os.html"));
    }
    m_ui->leftHelpWebView->show();
}

void HelpDialog::on_menuItem_WritingPrograms_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
    m_ui->leftHelpWebView->show();
}

void HelpDialog::on_menuItem_MachineLanguage_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/machinelanguage.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_AssemblyLanguage_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_DebuggingPrograms_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/debuggingprograms.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_BreakPoints_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/breakpoints.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_SymbolicTrace_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/symbolictrace.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_ByteConverter_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/byteconverter.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_InterruptHandlers_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/interrupthandlers.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_Reference_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/pep8reference.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_InstructionSet_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/instructionset.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_DotCommands_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/dotcommands.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_AddressingModes_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/addressingmodes.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_Examples_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/examples.html"));
    m_ui->leftHelpWebView->show();
}
void HelpDialog::on_menuItem_OperatingSystem_clicked() {
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/pep8os.html"));
    m_ui->leftHelpWebView->show();
}
