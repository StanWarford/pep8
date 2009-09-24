// File: helpdialog.cpp
/*
    Pep8-1 is a virtual machine for writing machine language and assembly
    language programs.
    
    Copyright (C) 2009  J. Stanley Warford, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QClipboard>
#include "helpdialog.h"
#include "ui_helpdialog.h"
#include "pep.h"

HelpDialog::HelpDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

    connect(m_ui->helpTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this,
            SLOT(onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    // Forward the helpCopyToSourceButton_clicked() signal from this to the main window
    connect(m_ui->helpCopyToSourceButton, SIGNAL(clicked()), this, SIGNAL(clicked()));

    m_ui->helpSplitter->widget(1)->hide();
    m_ui->helpTreeWidget->expandAll();

    selectItem("Writing Programs");

    leftHighlighter = new PepHighlighter(m_ui->helpLeftTextEdit->document());
    rightCppHighlighter = new CppHighlighter(m_ui->helpRightCppTextEdit->document());
    rightPepHighlighter = new PepHighlighter(m_ui->helpRightPepTextEdit->document());

    m_ui->helpCopyToSourceButton->setFont(QFont(Pep::labelFont));
//    m_ui->helpTreeWidget->setFont(QFont(Pep::labelFont));

    m_ui->helpRightCppTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    m_ui->helpRightPepTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    m_ui->helpLeftTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}

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
        }
        else if (row == eMACHINE) {           // Writing Programs > Machine Language
            m_ui->helpTopWebView->load(QUrl("qrc:/help/machinelanguage.html"));
        }
        else if (row == eASSEMBLY) {          // Writing Programs > Assembly Language
            m_ui->helpTopWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
        }
    }
    else if (!isHelpSubCat && row == eDEBUGGING) {
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->show();
        m_ui->helpTopWebView->load(QUrl("qrc:/help/debuggingprograms.html"));
    }
    else if (!isHelpSubCat && row == eTRAP) {  // Writing Trap Handlers
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->show();
        m_ui->helpTopWebView->load(QUrl("qrc:/help/writingtraphandlers.html"));
    }
    else if ((!isHelpSubCat && row == eREFERENCE)) {
        m_ui->helpSplitter->widget(1)->hide();
        m_ui->helpTopWebView->show();
        m_ui->helpTopWebView->load(QUrl("qrc:/help/pep8reference.html"));
    }
    else if ((!isHelpSubCat && row == eEXAMPLES) || parentRow == eEXAMPLES) {
        if (!isHelpSubCat) {
            m_ui->helpSplitter->widget(1)->hide();
            m_ui->helpTopWebView->show();
            m_ui->helpTopWebView->load(QUrl("qrc:/help/examples.html"));
        }
        else {
            m_ui->helpSplitter->widget(0)->hide();
            m_ui->helpSplitter->widget(1)->show();
            m_ui->helpCopyToSourceButton->setText("Copy to Source");
            if (row == eFIG432) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0432.peph"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0432.pepb"));
                m_ui->helpFigureLabel->setText("<b>Figure 4.32</b><code>  </code> A machine language program to output the characters <code>Hi</code>.");
                m_ui->helpRightPepTextEdit->show();
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpCopyToSourceButton->setText("Copy to Object");

            }
            else if (row == eFIG434) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0434.peph"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0434.pepb"));
                m_ui->helpFigureLabel->setText("<b>Figure 4.34</b><code>  </code> A machine language program to input two characters and output them in reverse order.");
                m_ui->helpRightPepTextEdit->show();
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpCopyToSourceButton->setText("Copy to Object");
            }
            else if (row == eFIG435) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0435.peph"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0435.pepb"));
                m_ui->helpFigureLabel->setText("<b>Figure 4.35</b><code>  </code> A machine language program to add 5 and 3 and output the single-character result.");
                m_ui->helpRightPepTextEdit->show();
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpCopyToSourceButton->setText("Copy to Object");
            }
            else if (row == eFIG436) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0436.peph"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0436.pepb"));
                m_ui->helpFigureLabel->setText("<b>Figure 4.36</b><code>  </code> A machine language program that modifies itself. The add accumulator instruction changes to a subtract instruction.");
                m_ui->helpRightPepTextEdit->show();
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpCopyToSourceButton->setText("Copy to Object");
            }
            else if (row == eFIG503) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0503.pep"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0432.peph"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.3</b><code>  </code> An assembly-language program to output <code>Hi</code>. It is the assembly-language version of Figure 4.32.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG506) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0506.pep"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0434.peph"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.6</b><code>  </code> An assembly language program to input two characters and output them in reverse order. It is the assembly language version of Figure 4.34.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG507) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0507.pep"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0435.peph"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.7</b><code>  </code> An assembly language program to add 3 and 5 and output the single-character result. It is the assembly language version of Figure 4.35.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG510) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0510.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.10</b><code>  </code> A program to output <code>Hi</code> using immediate addressing.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG511) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0511.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.11</b><code>  </code> A program to input a decimal value, add 1 to it, and output the sum.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG512) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0512.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.12</b><code>  </code> A program identical to that of Figure 5.11 but with the <code>STRO</code> instruction.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG513) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0513.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.13</b><code>  </code> A nonsense program to illustrate the interpretation of bit patterns.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG514a) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0514a.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.14a</b><code>  </code> Two different source programs that produce the same object program and, therefore, the same output.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG514b) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0514b.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.14b</b><code>  </code> Two different source programs that produce the same object program and, therefore, the same output.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG515) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0515.pep"));
                m_ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0512.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.15</b><code>  </code> A program that adds 1 to a decimal value. It is identical to Figure 5.12 except that it uses symbols.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG516) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0516.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.16</b><code>  </code> A nonsense program that illustrates the underlying von Neumann nature of the machine.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG518) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0518.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0518.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.18</b><code>  </code> The <code>cout</code> statement.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG521) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0521.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0521.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.21</b><code>  </code> The assignment statement with global variables.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG526) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0526.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0526.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 5.26</b><code>  </code> C++ constants.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG601) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0601.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.1</b><code>  </code> Stack-relative addressing.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG604) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0604.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0604.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.4</b><code>  </code> Local variables.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG606) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0606.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0606.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.6</b><code>  </code> The <code>if</code> statement.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG608) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0608.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0608.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.8</b><code>  </code> The <code>if</code>/<code>else</code> statement.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG610) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0610.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0610.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.10</b><code>  </code> The <code>while</code> statement.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG612) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0612.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0612.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.12</b><code>  </code> The <code>do</code> statement.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG614) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0614.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0614.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.14</b><code>  </code> The <code>for</code> statement.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG616) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0616.pep"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.16</b><code>  </code> A mystery program.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG618) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0618.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0618.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.18</b><code>  </code> A procedure call with no parameters.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG621) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0621.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0621.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.21</b><code>  </code> Call-by-value parameters with global variables.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG623) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0623.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0623.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.23</b><code>  </code> Call-by-value parameters with local variables.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG625) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0625.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0625.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.25</b><code>  </code> A recursive nonvoid function.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG627) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0627.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0627.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.27</b><code>  </code> Call-by-reference parameters with global variables.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG629) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0629.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0629.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.29</b><code>  </code> Call-by-reference parameters with local variables.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG632) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0632.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0632.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.32</b><code>  </code> Translation of a boolean type.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG634) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0634.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0634.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.32</b><code>  </code> A global array.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG636) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0636.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0636.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.36</b><code>  </code> A local array.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG638) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0638.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0638.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.38</b><code>  </code> Passing a local array as a parameter.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG640) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0640.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0640.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.40</b><code>  </code> Translation of a <code>switch</code> statement.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG641) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0641.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0641.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.41</b><code>  </code> Translation of global pointers.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG643) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0643.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0643.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.43</b><code>  </code> Translation of local pointers.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG645) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0645.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0645.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.45</b><code>  </code> Translation of a structure.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG647) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0647.pep"));
                m_ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0647.cpp"));
                m_ui->helpFigureLabel->setText("<b>Figure 6.47</b><code>  </code> Translation of a linked list.");
                m_ui->helpRightCppTextEdit->show();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == eEXER804) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/exer0804.pep"));
                m_ui->helpFigureLabel->setText("<b>Exercise 8.4</b><code>  </code> An excercise for the <code>DECI</code> trap.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == ePROB829) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/prob0829.pep"));
                m_ui->helpFigureLabel->setText("<b>Problem 8.29</b><code>  </code> A test driver for the <code>STADI</code> instruction.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }
            else if (row == ePROB832) {
                m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/prob0832.pep"));
                m_ui->helpFigureLabel->setText("<b>Problem 8.32</b><code>  </code> A test driver for the <code>STACKADD</code> instruction.");
                m_ui->helpRightCppTextEdit->hide();
                m_ui->helpRightPepTextEdit->hide();
            }

        }
    }
    else if (!isHelpSubCat && row == eOS) {         // Pep/8 Operating System
        m_ui->helpCopyToSourceButton->setText("Copy to Source");
        m_ui->helpSplitter->widget(0)->hide();
        m_ui->helpSplitter->widget(1)->show();
        m_ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/pep8os.pep"));
        m_ui->helpRightCppTextEdit->hide();
        m_ui->helpRightPepTextEdit->hide();
        m_ui->helpFigureLabel->setText("<b>Figures 8.2, 8.3, 8.6, 8.8, 8.10, 8.11</b><code>  </code> The Pep/8 operating system.");
    }
}

// Public functions called by main window help menu items:

void HelpDialog::machineLanguageClicked()
{
    selectItem("Machine Language");
}

void HelpDialog::assemblyLanguageClicked()
{
    selectItem("Assembly Language");
}

void HelpDialog::debuggingProgramsClicked()
{
    selectItem("Debugging Programs");
}

void HelpDialog::writingTrapHandlersClicked()
{
    selectItem("Writing Trap Handlers");
}

void HelpDialog::pep8ReferenceClicked()
{
    selectItem("Pep/8 Reference");
}

void HelpDialog::examplesClicked()
{
    selectItem("Examples");
}

void HelpDialog::operatingSystemClicked()
{
    selectItem("Pep/8 Operating System");
}

// Helper Functions
QString HelpDialog::getCode(Enu::EPane &destPane, Enu::EPane &inputDest, QString &input)
{
    bool isHelpSubCat = m_ui->helpTreeWidget->currentIndex().parent().isValid();
    int row = m_ui->helpTreeWidget->currentIndex().row();
    if (!isHelpSubCat && row == eOS) {         // Pep/8 Operating System
        destPane = Enu::ESource;
        return m_ui->helpLeftTextEdit->toPlainText();
    }
    if (row == eFIG432) {
        destPane = Enu::EObject;
        return Pep::resToString(":/help/figures/fig0432.pepo");
    }
    else if (row == eFIG434) {
        destPane = Enu::EObject;
        input = "up";
        return Pep::resToString(":/help/figures/fig0434.pepo");
    }
    else if (row == eFIG435) {
        destPane = Enu::EObject;
        return Pep::resToString(":/help/figures/fig0435.pepo");
    }
    else if (row == eFIG436) {
        destPane = Enu::EObject;
        return Pep::resToString(":/help/figures/fig0436.pepo");
    }
    else if (row == eFIG506) {
        input = "up";
    }
    else if (row == eFIG511) {
        input = "-479";
    }
    else if (row == eFIG512) {
        input = "-479";
    }
    else if (row == eFIG515) {
        input = "-479";
    }
    else if (row == eFIG521) {
        input = "M 419";
    }
    else if (row == eFIG526 || row == eFIG604) {
        input = "68 84";
    }
    else if (row == eFIG606) {
        input = "-25";
    }
    else if (row == eFIG608) {
        input = "75";
    }
    else if (row == eFIG610) {
        input = "happy*";
    }
    else if (row == eFIG616) {
        input = "3 -15 25";
    }
    else if (row == eFIG621 || row == eFIG623) {
        input = "12  3 13 17 34 27 23 25 29 16 10 0 2";
    }
    else if (row == eFIG632) {
        input = "25";
    }
    else if (row == eFIG634) {
        input = "60 70 80 90";
    }
    else if (row == eFIG636) {
        input = "2 26 -3 9";
    }
    else if (row == eFIG638) {
        input = "5  40 50 60 70 80";
    }
    else if (row == eFIG645) {
        input = "bj 32 m";
    }
    else if (row == eFIG647) {
        input = "10 20 30 40 -9999";
    }
    else if (row == eFIG627 || row == eFIG629 || row == eFIG640) {
        inputDest = Enu::ETerminal;
    }
    else if (row == eEXER804) {
        input = "37";
    }
    destPane = Enu::ESource;
    return m_ui->helpLeftTextEdit->toPlainText();
}

bool HelpDialog::hasFocus()
{
    return m_ui->helpLeftTextEdit->hasFocus() || m_ui->helpRightCppTextEdit->hasFocus() || m_ui->helpTopWebView->hasFocus();
}

void HelpDialog::copy()
{
    if (m_ui->helpLeftTextEdit->hasFocus()) {
        m_ui->helpLeftTextEdit->copy();
    } else if (m_ui->helpRightCppTextEdit->hasFocus()) {
        m_ui->helpRightCppTextEdit->copy();
    } else if (m_ui->helpTopWebView->hasFocus()) {
        QApplication::clipboard()->setText(m_ui->helpTopWebView->selectedText());
    }
}

void HelpDialog::setCopyButtonDisabled(bool b)
{
    m_ui->helpCopyToSourceButton->setDisabled(b);
}
