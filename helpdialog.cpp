#include "helpdialog.h"
#include "ui_helpdialog.h"

#include <QDebug>
#include <QFile>

HelpDialog::HelpDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->helpTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(on_itemClicked(QTreeWidgetItem*,int)));
    QObject::connect(m_ui->helpTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(on_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    // Forward the helpCopyToSourceButton_clicked() signal from this to the main window
    QObject::connect(m_ui->helpCopyToSourceButton, SIGNAL(clicked()), this, SIGNAL(clicked()));

    m_ui->helpSplitter->widget(1)->hide();
}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}

QString resToString(QString fileName) {
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString inString = "";
    while (!in.atEnd()) {
        QString line = in.readLine();
        inString.append(line + "\n");
    }
    return inString;
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
        m_ui->helpSplitter->widget(1)->hide();
        if (!subCat) {                          // Writing Programs
            m_ui->helpTopWebView->load(QUrl("qrc:/help/writingprograms.html"));
        } else if (row == eMACHINE) {           // Writing Programs > Machine Language
            m_ui->helpTopWebView->load(QUrl("qrc:/help/machinelanguage.html"));
        } else if (row == eASSEMBLY) {          // Writing Programs > Assembly Language
            m_ui->helpTopWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
        }
        m_ui->helpTopWebView->show();
    } else if ((!subCat && row == eDEBUGGING) || parent == eDEBUGGING) {
        m_ui->helpSplitter->widget(1)->hide();
        if (!subCat) {                          // Debugging Programs
            m_ui->helpTopWebView->load(QUrl("qrc:/help/debuggingprograms.html"));
        } else if (row == eBREAK) {             // Debugging Programs > Break Points
            m_ui->helpTopWebView->load(QUrl("qrc:/help/breakpoints.html"));
        } else if (row == eSYMTRACE) {          // Debugging Programs > Symbolic Trace
            m_ui->helpTopWebView->load(QUrl("qrc:/help/symbolictrace.html"));
        } else if (row == eBYTECONVERTER) {     // Debugging Programs > Byte Converter
            m_ui->helpTopWebView->load(QUrl("qrc:/help/byteconverter.html"));
        }
        m_ui->helpTopWebView->show();
    } else if (!subCat && row == eINTERRUPT) {  // Writing Interrupt Handlers
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->load(QUrl("qrc:/help/interrupthandlers.html"));
        m_ui->helpTopWebView->show();
    } else if ((!subCat && row == eREFERENCE) || parent == eREFERENCE) {
        m_ui->helpSplitter->widget(1)->hide();
        if (!subCat) {                          // Pep/8 Reference
            m_ui->helpTopWebView->load(QUrl("qrc:/help/pep8reference.html"));
        } else if (row == eINSTRUCTION) {       // Pep/8 Reference > Instruction Set
            m_ui->helpTopWebView->load(QUrl("qrc:/help/instructionset.html"));
        } else if (row == eDOTCMD) {            // Pep/8 Reference > Dot Commands
            m_ui->helpTopWebView->load(QUrl("qrc:/help/dotcommands.html"));
        } else if (row == eADDRMODE) {          // Pep/8 Reference > Addressing Modes
            m_ui->helpTopWebView->load(QUrl("qrc:/help/addressingmodes.html"));
        }
        m_ui->helpTopWebView->show();
    } else if ((!subCat && row == eEXAMPLES) || parent == eEXAMPLES) {
        if (!subCat) {
            m_ui->helpSplitter->widget(1)->hide();
            m_ui->helpTopWebView->load(QUrl("qrc:/help/examples.html"));
//            m_ui->helpTopWebView->setHtml(
            m_ui->helpTopWebView->show();
        } else {
            m_ui->helpSplitter->widget(0)->hide();
            m_ui->helpSplitter->widget(1)->show();
            if (row == eFIG518) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0518.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0518.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG521) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0521.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0521.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG526) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0526.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0526.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG601) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0601.pep"));
                m_ui->helpRightTextEdit->hide();
            } else if (row == eFIG604) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0604.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0604.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG606) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0606.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0606.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG608) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0608.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0608.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG610) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0610.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0610.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG612) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0612.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0612.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG614) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0614.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0614.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG616) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0616.pep"));
                m_ui->helpRightTextEdit->hide();
            } else if (row == eFIG618) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0618.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0618.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG621) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0621.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0621.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG623) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0623.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0623.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG625) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0625.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0625.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG627) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0627.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0627.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG629) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0629.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0629.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG632) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0632.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0632.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG634) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0634.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0634.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG636) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0636.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0636.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG638) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0638.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0638.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG640) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0640.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0640.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG641) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0641.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0641.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG643) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0643.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0643.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG645) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0645.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0645.cpp"));
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG647) {
                m_ui->helpLeftTextEdit->setText(resToString(":/help/figures/fig0647.pep"));
                m_ui->helpRightTextEdit->setText(resToString(":/help/figures/fig0647.cpp"));
                m_ui->helpRightTextEdit->show();
            }
        }
    } else if (!subCat && row == eOS) {         // Pep/8 Operating System
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->load(QUrl("qrc:/help/pep8os.html"));
        m_ui->helpTopWebView->show();
    }
}

// Public functions called by main window help menu items:

void HelpDialog::on_menuItem_MachineLanguage_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/machinelanguage.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_AssemblyLanguage_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_BreakPoints_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/breakpoints.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_SymbolicTrace_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/symbolictrace.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_ByteConverter_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/byteconverter.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_InterruptHandlers_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/interrupthandlers.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_InstructionSet_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/instructionset.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_DotCommands_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/dotcommands.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_AddressingModes_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/addressingmodes.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_Examples_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/examples.html"));
    m_ui->helpTopWebView->show();
}

void HelpDialog::on_menuItem_OperatingSystem_clicked() {
    m_ui->helpSplitter->widget(1)->hide();
//    m_ui->helpTopWebView->setHtml("<html><body><pre>This is a test</pre><br>This is not a test</body></html>", QUrl("qrc:/"));
//    m_ui->helpTopWebView->setUrl(QUrl("qrc:/help/pep8os.html"));
    m_ui->helpTopWebView->show();
    m_ui->helpTopWebView->load(QUrl("http://homepage.mac.com/dimpfinator/pep8os.html"));
}

QString HelpDialog::getLeftTextEditText() {
    return m_ui->helpLeftTextEdit->toPlainText();
}
