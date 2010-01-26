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
        ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    connect(ui->helpTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this,
            SLOT(onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    // Forward the helpCopyToSourceButton_clicked() signal from this to the main window
    connect(ui->helpCopyToSourceButton, SIGNAL(clicked()), this, SIGNAL(clicked()));

    ui->helpSplitter->widget(1)->hide();
    ui->helpTreeWidget->expandAll();

    selectItem("Writing Programs");

    leftHighlighter = new PepHighlighter(ui->helpLeftTextEdit->document());
    rightCppHighlighter = new CppHighlighter(ui->helpRightCppTextEdit->document());
    rightPepHighlighter = new PepHighlighter(ui->helpRightPepTextEdit->document());

    ui->helpCopyToSourceButton->setFont(QFont(Pep::labelFont));
    if (Pep::getSystem() == "Linux") {
        ui->helpTreeWidget->setFont(QFont(Pep::labelFont, 8));
    }
    else if (Pep::getSystem() == "Windows") {
        ui->helpTreeWidget->setFont(QFont(Pep::labelFont, 8)); // I don't know if this is the proper font size.
    }

    ui->helpRightCppTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    ui->helpRightPepTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    ui->helpLeftTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::selectItem(QString string) {
    QTreeWidgetItemIterator it(ui->helpTreeWidget);
    while (*it) {
        if ((*it)->text(0) == string) {
            (*it)->setSelected(true);
            ui->helpTreeWidget->setCurrentItem((*it));
        } else {
            (*it)->setSelected(false);
        }
        ++it;
    }
}

void HelpDialog::onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*) {
    // Is this a subcategory?
    bool isHelpSubCat = ui->helpTreeWidget->currentIndex().parent().isValid();
    // Parent row (if it has a parent, -1 else)
    int parentRow = ui->helpTreeWidget->currentIndex().parent().row();
    // Row (if it has a parent, this is the child row)
    int row = ui->helpTreeWidget->currentIndex().row();

//    qDebug() << "Selected: " << ui->helpTreeWidget->currentIndex();

    if ((!isHelpSubCat && row == eWRITING) || parentRow == eWRITING) {
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        if (!isHelpSubCat) {                          // Writing Programs
            ui->helpTopWebView->load(QUrl("qrc:/help/writingprograms.html"));
        }
        else if (row == eMACHINE) {           // Writing Programs > Machine Language
            ui->helpTopWebView->load(QUrl("qrc:/help/machinelanguage.html"));
        }
        else if (row == eASSEMBLY) {          // Writing Programs > Assembly Language
            ui->helpTopWebView->load(QUrl("qrc:/help/assemblylanguage.html"));
        }
    }
    else if (!isHelpSubCat && row == eDEBUGGING) {
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/debuggingprograms.html"));
    }
    else if (!isHelpSubCat && row == eTRAP) {  // Writing Trap Handlers
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/writingtraphandlers.html"));
    }
    else if ((!isHelpSubCat && row == eREFERENCE)) {
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/pep8reference.html"));
    }
    else if ((!isHelpSubCat && row == eEXAMPLES) || parentRow == eEXAMPLES) {
        if (!isHelpSubCat) {
            ui->helpSplitter->widget(1)->hide();
            ui->helpTopWebView->show();
            ui->helpTopWebView->load(QUrl("qrc:/help/examples.html"));
        }
        else {
            ui->helpSplitter->widget(0)->hide();
            ui->helpSplitter->widget(1)->show();
            ui->helpCopyToSourceButton->setText("Copy to Source");
            if (row == eFIG432) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0432.peph"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0432.pepb"));
                ui->helpFigureLabel->setText("<b>Figure 4.32</b><code>  </code> A machine language program to output the characters <code>Hi</code>.");
                ui->helpRightPepTextEdit->show();
                ui->helpRightCppTextEdit->hide();
                ui->helpCopyToSourceButton->setText("Copy to Object");

            }
            else if (row == eFIG434) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0434.peph"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0434.pepb"));
                ui->helpFigureLabel->setText("<b>Figure 4.34</b><code>  </code> A machine language program to input two characters and output them in reverse order.");
                ui->helpRightPepTextEdit->show();
                ui->helpRightCppTextEdit->hide();
                ui->helpCopyToSourceButton->setText("Copy to Object");
            }
            else if (row == eFIG435) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0435.peph"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0435.pepb"));
                ui->helpFigureLabel->setText("<b>Figure 4.35</b><code>  </code> A machine language program to add 5 and 3 and output the single-character result.");
                ui->helpRightPepTextEdit->show();
                ui->helpRightCppTextEdit->hide();
                ui->helpCopyToSourceButton->setText("Copy to Object");
            }
            else if (row == eFIG436) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0436.peph"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0436.pepb"));
                ui->helpFigureLabel->setText("<b>Figure 4.36</b><code>  </code> A machine language program that modifies itself. The add accumulator instruction changes to a subtract instruction.");
                ui->helpRightPepTextEdit->show();
                ui->helpRightCppTextEdit->hide();
                ui->helpCopyToSourceButton->setText("Copy to Object");
            }
            else if (row == eFIG503) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0503.pep"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0432.peph"));
                ui->helpFigureLabel->setText("<b>Figure 5.3</b><code>  </code> An assembly-language program to output <code>Hi</code>. It is the assembly-language version of Figure 4.32.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG506) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0506.pep"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0434.peph"));
                ui->helpFigureLabel->setText("<b>Figure 5.6</b><code>  </code> An assembly language program to input two characters and output them in reverse order. It is the assembly language version of Figure 4.34.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG507) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0507.pep"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0435.peph"));
                ui->helpFigureLabel->setText("<b>Figure 5.7</b><code>  </code> An assembly language program to add 3 and 5 and output the single-character result. It is the assembly language version of Figure 4.35.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG510) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0510.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.10</b><code>  </code> A program to output <code>Hi</code> using immediate addressing.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG511) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0511.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.11</b><code>  </code> A program to input a decimal value, add 1 to it, and output the sum.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG512) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0512.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.12</b><code>  </code> A program identical to that of Figure 5.11 but with the <code>STRO</code> instruction.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG513) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0513.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.13</b><code>  </code> A nonsense program to illustrate the interpretation of bit patterns.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG514a) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0514a.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.14a</b><code>  </code> Two different source programs that produce the same object program and, therefore, the same output.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG514b) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0514b.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.14b</b><code>  </code> Two different source programs that produce the same object program and, therefore, the same output.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG515) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0515.pep"));
                ui->helpRightPepTextEdit->setText(Pep::resToString(":/help/figures/fig0512.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.15</b><code>  </code> A program that adds 1 to a decimal value. It is identical to Figure 5.12 except that it uses symbols.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->show();
            }
            else if (row == eFIG516) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0516.pep"));
                ui->helpFigureLabel->setText("<b>Figure 5.16</b><code>  </code> A nonsense program that illustrates the underlying von Neumann nature of the machine.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG518) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0518.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0518.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 5.18</b><code>  </code> The <code>cout</code> statement.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG521) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0521.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0521.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 5.21</b><code>  </code> The assignment statement with global variables.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG526) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0526.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0526.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 5.26</b><code>  </code> C++ constants.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG601) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0601.pep"));
                ui->helpFigureLabel->setText("<b>Figure 6.1</b><code>  </code> Stack-relative addressing.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG604) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0604.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0604.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.4</b><code>  </code> Local variables.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG606) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0606.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0606.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.6</b><code>  </code> The <code>if</code> statement.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG608) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0608.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0608.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.8</b><code>  </code> The <code>if</code>/<code>else</code> statement.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG610) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0610.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0610.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.10</b><code>  </code> The <code>while</code> statement.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG612) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0612.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0612.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.12</b><code>  </code> The <code>do</code> statement.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG614) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0614.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0614.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.14</b><code>  </code> The <code>for</code> statement.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG616) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0616.pep"));
                ui->helpFigureLabel->setText("<b>Figure 6.16</b><code>  </code> A mystery program.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG618) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0618.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0618.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.18</b><code>  </code> A procedure call with no parameters.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG621) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0621.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0621.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.21</b><code>  </code> Call-by-value parameters with global variables.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG623) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0623.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0623.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.23</b><code>  </code> Call-by-value parameters with local variables.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG625) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0625.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0625.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.25</b><code>  </code> A recursive nonvoid function.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG627) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0627.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0627.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.27</b><code>  </code> Call-by-reference parameters with global variables.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG629) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0629.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0629.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.29</b><code>  </code> Call-by-reference parameters with local variables.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG632) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0632.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0632.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.32</b><code>  </code> Translation of a boolean type.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG634) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0634.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0634.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.32</b><code>  </code> A global array.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG636) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0636.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0636.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.36</b><code>  </code> A local array.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG638) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0638.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0638.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.38</b><code>  </code> Passing a local array as a parameter.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG640) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0640.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0640.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.40</b><code>  </code> Translation of a <code>switch</code> statement.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG641) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0641.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0641.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.41</b><code>  </code> Translation of global pointers.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG643) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0643.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0643.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.43</b><code>  </code> Translation of local pointers.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG645) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0645.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0645.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.45</b><code>  </code> Translation of a structure.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eFIG647) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/fig0647.pep"));
                ui->helpRightCppTextEdit->setText(Pep::resToString(":/help/figures/fig0647.cpp"));
                ui->helpFigureLabel->setText("<b>Figure 6.47</b><code>  </code> Translation of a linked list.");
                ui->helpRightCppTextEdit->show();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == eEXER804) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/exer0804.pep"));
                ui->helpFigureLabel->setText("<b>Exercise 8.4</b><code>  </code> An excercise for the <code>DECI</code> trap.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == ePROB829) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/prob0829.pep"));
                ui->helpFigureLabel->setText("<b>Problem 8.29</b><code>  </code> A test driver for the <code>STADI</code> instruction.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }
            else if (row == ePROB832) {
                ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/prob0832.pep"));
                ui->helpFigureLabel->setText("<b>Problem 8.32</b><code>  </code> A test driver for the <code>STACKADD</code> instruction.");
                ui->helpRightCppTextEdit->hide();
                ui->helpRightPepTextEdit->hide();
            }

        }
    }
    else if (!isHelpSubCat && row == eOS) {         // Pep/8 Operating System
        ui->helpCopyToSourceButton->setText("Copy to Source");
        ui->helpSplitter->widget(0)->hide();
        ui->helpSplitter->widget(1)->show();
        ui->helpLeftTextEdit->setText(Pep::resToString(":/help/figures/pep8os.pep"));
        ui->helpRightCppTextEdit->hide();
        ui->helpRightPepTextEdit->hide();
        ui->helpFigureLabel->setText("<b>Figures 8.2, 8.3, 8.6, 8.8, 8.10, 8.11</b><code>  </code> The Pep/8 operating system.");
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
    bool isHelpSubCat = ui->helpTreeWidget->currentIndex().parent().isValid();
    int row = ui->helpTreeWidget->currentIndex().row();
    if (!isHelpSubCat && row == eOS) {         // Pep/8 Operating System
        destPane = Enu::ESource;
        return ui->helpLeftTextEdit->toPlainText();
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
    return ui->helpLeftTextEdit->toPlainText();
}

bool HelpDialog::hasFocus()
{
    return ui->helpLeftTextEdit->hasFocus() || ui->helpRightCppTextEdit->hasFocus() || ui->helpTopWebView->hasFocus();
}

void HelpDialog::copy()
{
    if (ui->helpLeftTextEdit->hasFocus()) {
        ui->helpLeftTextEdit->copy();
    } else if (ui->helpRightCppTextEdit->hasFocus()) {
        ui->helpRightCppTextEdit->copy();
    } else if (ui->helpTopWebView->hasFocus()) {
        QApplication::clipboard()->setText(ui->helpTopWebView->selectedText());
    }
}

void HelpDialog::setCopyButtonDisabled(bool b)
{
    ui->helpCopyToSourceButton->setDisabled(b);
}
