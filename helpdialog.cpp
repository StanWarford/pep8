#include "helpdialog.h"
#include "ui_helpdialog.h"
#include "pep.h"

#include <QDebug>

HelpDialog::HelpDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

    // Forward signal to the on_currentItemChanged slot
    QObject::connect(m_ui->helpTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    // Forward the helpCopyToSourceButton_clicked() signal from this to the main window
    QObject::connect(m_ui->helpCopyToSourceButton, SIGNAL(clicked()), this, SIGNAL(clicked()));

    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTreeWidget->expandAll();

    this->selectItem("Machine Language");
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

void HelpDialog::selectItem(QString string) {
    QTreeWidgetItemIterator it(m_ui->helpTreeWidget);
    while (*it) {
        if ((*it)->text(0) == string) {
            (*it)->setSelected(true);
            m_ui->helpTreeWidget->setCurrentItem((*it));
        } else {
            (*it)->setSelected(false);
        }
        ++it;
    }
}

void HelpDialog::onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*) {
//    qDebug() << "Is valid? : " << m_ui->helpTreeWidget->currentIndex().parent().isValid();
//    qDebug() << "Parent row: " << m_ui->helpTreeWidget->currentIndex().parent().row();
//    qDebug() << "Row: " << m_ui->helpTreeWidget->currentIndex().row();

    // Is this a subcategory?
    bool isHelpSubCat = m_ui->helpTreeWidget->currentIndex().parent().isValid();
    // Parent row (if it has a parent, -1 else)
    int parentRow = m_ui->helpTreeWidget->currentIndex().parent().row();
    // Row (if it has a parent, this is the child row)
    int row = m_ui->helpTreeWidget->currentIndex().row();

//    qDebug() << "Selected: " << m_ui->helpTreeWidget->currentIndex();

    if ((!isHelpSubCat && row == eWRITING) || parentRow == eWRITING) {
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->show();
        if (!isHelpSubCat) {                          // Writing Programs
            m_ui->helpTopWebView->load(QUrl("qrc:/help/writingprograms.html"));
        } else if (row == eMACHINE) {           // Writing Programs > Machine Language
            m_ui->helpTopWebView->load(QUrl("qrc:/help/machinelanguage.html"));
        } else if (row == eASSEMBLY) {          // Writing Programs > Assembly Language
            m_ui->helpTopWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
        }
    } else if ((!isHelpSubCat && row == eDEBUGGING) || parentRow == eDEBUGGING) {
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->show();
        if (!isHelpSubCat) {                          // Debugging Programs
            m_ui->helpTopWebView->load(QUrl("qrc:/help/debuggingprograms.html"));
        } else if (row == eBREAK) {             // Debugging Programs > Break Points
            m_ui->helpTopWebView->load(QUrl("qrc:/help/breakpoints.html"));
        } else if (row == eSYMTRACE) {          // Debugging Programs > Symbolic Trace
            m_ui->helpTopWebView->load(QUrl("qrc:/help/symbolictrace.html"));
        } else if (row == eBYTECONVERTER) {     // Debugging Programs > Byte Converter
            m_ui->helpTopWebView->load(QUrl("qrc:/help/byteconverter.html"));
        }
    } else if (!isHelpSubCat && row == eINTERRUPT) {  // Writing Interrupt Handlers
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->show();
        m_ui->helpTopWebView->load(QUrl("qrc:/help/interrupthandlers.html"));
    } else if ((!isHelpSubCat && row == eREFERENCE) || parentRow == eREFERENCE) {
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->show();
        if (!isHelpSubCat) {                          // Pep/8 Reference
            m_ui->helpTopWebView->load(QUrl("qrc:/help/pep8reference.html"));
        } else if (row == eINSTRUCTION) {       // Pep/8 Reference > Instruction Set
            m_ui->helpTopWebView->load(QUrl("qrc:/help/instructionset.html"));
        } else if (row == eDOTCMD) {            // Pep/8 Reference > Dot Commands
            m_ui->helpTopWebView->load(QUrl("qrc:/help/dotcommands.html"));
        } else if (row == eADDRMODE) {          // Pep/8 Reference > Addressing Modes
            m_ui->helpTopWebView->load(QUrl("qrc:/help/addressingmodes.html"));
        }
    } else if ((!isHelpSubCat && row == eEXAMPLES) || parentRow == eEXAMPLES) {
        if (!isHelpSubCat) {
            m_ui->helpSplitter->widget(1)->hide();
            m_ui->helpTopWebView->show();
            m_ui->helpTopWebView->load(QUrl("qrc:/help/examples.html"));
//            m_ui->helpTopWebView->setHtml(
        } else {
            m_ui->helpSplitter->widget(0)->hide();
            m_ui->helpSplitter->widget(1)->show();
            if (row == eFIG518) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0518.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0518.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.18</b><code>  </code> The <code>cout</code> statement.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG521) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0521.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0521.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.21</b><code>  </code> The assignment statement with global variables.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG526) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0526.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0526.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.26</b><code>  </code> C++ constants.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG601) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0601.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.1</b><code>  </code> Stack-relative addressing.");
                m_ui->helpRightTextEdit->hide();
            } else if (row == eFIG604) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0604.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0604.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.4</b><code>  </code> Local variables.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG606) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0606.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0606.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.6</b><code>  </code> The <code>if</code> statement.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG608) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0608.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0608.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.8</b><code>  </code> The <code>if</code>/<code>else</code> statement.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG610) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0610.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0610.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.10</b><code>  </code> The <code>while</code> statement.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG612) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0612.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0612.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.12</b><code>  </code> The <code>do</code> statement.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG614) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0614.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0614.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.14</b><code>  </code> The <code>for</code> statement.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG616) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0616.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.16</b><code>  </code> A mystery program.");
                m_ui->helpRightTextEdit->hide();
            } else if (row == eFIG618) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0618.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0618.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.18</b><code>  </code> A procedure call with no parameters.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG621) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0621.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0621.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.21</b><code>  </code> Call-by-value parameters with global variables.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG623) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0623.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0623.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.23</b><code>  </code> Call-by-value parameters with local variables.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG625) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0625.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0625.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.25</b><code>  </code> A recursive nonvoid function.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG627) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0627.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0627.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.27</b><code>  </code> Call-by-reference parameters with global variables.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG629) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0629.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0629.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.29</b><code>  </code> Call-by-reference parameters with local variables.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG632) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0632.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0632.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.32</b><code>  </code> Translation of a boolean type.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG634) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0634.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0634.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.32</b><code>  </code> A global array.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG636) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0636.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0636.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.36</b><code>  </code> A local array.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG638) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0638.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0638.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.38</b><code>  </code> Passing a local array as a parameter.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG640) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0640.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0640.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.40</b><code>  </code> Translation of a <code>switch</code> statement.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG641) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0641.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0641.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.41</b><code>  </code> Translation of global pointers.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG643) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0643.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0643.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.43</b><code>  </code> Translation of local pointers.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG645) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0645.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0645.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.45</b><code>  </code> Translation of a structure.");
                m_ui->helpRightTextEdit->show();
            } else if (row == eFIG647) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0647.pep"));
                m_ui->helpRightTextEdit->setText(Pep::resToString(":/help/figures/fig0647.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.47</b><code>  </code> Translation of a linked list.");
                m_ui->helpRightTextEdit->show();
            }
        }
    } else if (!isHelpSubCat && row == eOS) {         // Pep/8 Operating System
        m_ui->helpSplitter->widget(0)->hide();
        m_ui->helpSplitter->widget(1)->show();
        m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/pep8os.pep"));
        m_ui->helpRightTextEdit->hide();
        m_ui->helpFigureLabel->setText("<b>Figures 8.2, 8.3, 8.6, 8.8, 8.10, 8.11</b><code>  </code> The Pep/8 operating system.");
    }
}

// Public functions called by main window help menu items:

void HelpDialog::machineLanguageClicked() {
    this->selectItem("Machine Language");
}

void HelpDialog::assemblyLanguageClicked() {
    this->selectItem("Assembly Language");
}

void HelpDialog::breakPointsClicked() {
    this->selectItem("Break Points");
}

void HelpDialog::symbolicTraceClicked() {
    this->selectItem("Symbolic Trace");
}

void HelpDialog::byteConverterClicked() {
    this->selectItem("Byte Converter");
}

void HelpDialog::interruptHandlersClicked() {
    this->selectItem("Writing Interrupt Handlers");
}

void HelpDialog::instructionSetClicked() {
    this->selectItem("Instruction Set");
}

void HelpDialog::dotCommandsClicked() {
    this->selectItem("Dot Commands");
}

void HelpDialog::addressingModesClicked() {
    this->selectItem("Addressing Modes");
}

void HelpDialog::examplesClicked() {
    this->selectItem("Examples");
}

void HelpDialog::operatingSystemClicked() {
    this->selectItem("Pep/8 Operating System");
}

// Helper Functions

QString HelpDialog::getLeftTextEditText() {
    return m_ui->helpLeftTextEdit->toPlainText();
}

// Gets called when the lifesaver button is pressed in the main window.
void HelpDialog::helpClicked() {
    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTopWebView->load(QUrl("qrc:/help/welcome.html"));
    m_ui->helpTopWebView->show();
}
