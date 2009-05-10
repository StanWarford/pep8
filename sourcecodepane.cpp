#include <QStringList>
#include "sourcecodepane.h"
#include "ui_sourcecodepane.h"

#include <QDebug>

SourceCodePane::SourceCodePane(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::SourceCodePane)
{
    m_ui->setupUi(this);
}

SourceCodePane::~SourceCodePane()
{
    delete m_ui;
}

void SourceCodePane::assemble()
{
    QString sourceCode = m_ui->pepSourceCodeTextEdit->toPlainText();
    assemblerListingList = sourceCode.split('\n');
    // UI test, if first letter on a line is 'y', insert text box in listing
    hasCheckBox.clear();
    for (int i = 0; i < assemblerListingList.size(); i++) {
        if (assemblerListingList[i].length() > 0 && assemblerListingList[i][0] == 'y') {
            hasCheckBox.append(true);
        }
        else {
            hasCheckBox.append(false);
        }
    }
    // Just some random numbers for now
    objectCode.clear();
    for (int i = 0; i < 40; i++) {
        objectCode << (10 * i) % 256;
    }
}

QList<int> SourceCodePane::getObjectCode() { return objectCode; }
QStringList SourceCodePane::getAssemblerListingList() { return assemblerListingList; }
QList<bool> SourceCodePane::getHasCheckBox() { return hasCheckBox; }
