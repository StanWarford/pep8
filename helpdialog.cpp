#include "helpdialog.h"
#include "ui_helpdialog.h"

//#include <QDebug>

HelpDialog::HelpDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->helpTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(on_itemClicked(QTreeWidgetItem*,int)));
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
};

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
            m_ui->leftHelpWebView->show();
        } else if (row == eMACHINE) {           // Writing Programs > Machine Language
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/machinelanguage.html"));
            m_ui->leftHelpWebView->show();
        } else if (row == eASSEMBLY) {          // Writing Programs > Assembly Language
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
            m_ui->leftHelpWebView->show();
        }
    } else if ((!subCat && row == eDEBUGGING) || parent == eDEBUGGING) {
        m_ui->rightHelpWebView->hide();
        if (!subCat) {                          // Debugging Programs
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/debuggingprograms.html"));
            m_ui->leftHelpWebView->show();
        } else if (row == eBREAK) {             // Debugging Programs > Break Points
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
            m_ui->leftHelpWebView->show();
        } else if (row == eSYMTRACE) {          // Debugging Programs > Symbolic Trace
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
            m_ui->leftHelpWebView->show();
        } else if (row == eBYTECONVERTER) {     // Debugging Programs > Byte Converter
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
            m_ui->leftHelpWebView->show();
        }
    } else if (!subCat && row == eINTERRUPT) {  // Writing Interrupt Handlers
        m_ui->rightHelpWebView->hide();
        m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
        m_ui->leftHelpWebView->show();
    } else if ((!subCat && row == eREFERENCE) || parent == eREFERENCE) {
        m_ui->rightHelpWebView->hide();
        if (!subCat) {                          // Pep/8 Reference
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
            m_ui->leftHelpWebView->show();
        } else if (row == eINSTRUCTION) {       // Pep/8 Reference > Instruction Set
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
            m_ui->leftHelpWebView->show();
        } else if (row == eDOTCMD) {            // Pep/8 Reference > Dot Commands
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
            m_ui->leftHelpWebView->show();
        } else if (row == eADDRMODE) {          // Pep/8 Reference > Addressing Modes
            m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
            m_ui->leftHelpWebView->show();
        }
    } else if (!subCat && row == eEXAMPLES) {   // Examples. Need to expand this a lot.
        m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
        m_ui->rightHelpWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
        m_ui->leftHelpWebView->show();
        m_ui->rightHelpWebView->show();
    } else if (!subCat && row == eOS) {         // Pep/8 Operating System
        m_ui->rightHelpWebView->hide();
        m_ui->leftHelpWebView->load(QUrl("qrc:/help/writingprograms.html"));
        m_ui->leftHelpWebView->show();
    }
}

//Writing Programs
//	Assembly Language
//	Machine Language
//Debugging Programs
//	Break Points
//	Symbolic Trace
//	Byte Converter
//Writing Interrupt Handlers
//Pep/8 Reference
//	Instruction Set
//	Dot Commands
//	Addressing Modes
//Examples
//Pep/8 OS
