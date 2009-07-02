#include <QScrollBar>
#include "assemblerlistingpane.h"
#include "ui_assemblerlistingpane.h"
#include "Pep.h"

AssemblerListingPane::AssemblerListingPane(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::AssemblerListingPane)
{
    m_ui->setupUi(this);
}

AssemblerListingPane::~AssemblerListingPane()
{
    delete m_ui;
}

void AssemblerListingPane::setAssemblerListing(QStringList assemblerListingList) {
    clearAssemblerListing();
    m_ui->assemblerListingTextEdit->append("-------------------------------------------------------------------------------");
    m_ui->assemblerListingTextEdit->append("      Object");
    m_ui->assemblerListingTextEdit->append("Addr  code   Symbol   Mnemon  Operand     Comment");
    m_ui->assemblerListingTextEdit->append("-------------------------------------------------------------------------------");
    m_ui->assemblerListingTextEdit->append(assemblerListingList.join("\n"));
    m_ui->assemblerListingTextEdit->append("-------------------------------------------------------------------------------");
    if (Pep::symbolTable.size() > 0) {
        m_ui->assemblerListingTextEdit->append("");
        m_ui->assemblerListingTextEdit->append("");
        m_ui->assemblerListingTextEdit->append("Symbol table");
        m_ui->assemblerListingTextEdit->append("--------------------------------------");
        m_ui->assemblerListingTextEdit->append("Symbol    Value        Symbol    Value");
        m_ui->assemblerListingTextEdit->append("--------------------------------------");
        QMapIterator<QString, int> i(Pep::symbolTable);
        QString symbolTableLine = "";
        QString hexString;
        while (i.hasNext()) {
            i.next();
            hexString = QString("%1").arg(i.value(), 4, 16, QLatin1Char('0')).toUpper();
            if (symbolTableLine.length() == 0) {
                symbolTableLine = QString("%1%2").arg(i.key(), -10).arg(hexString, -13);
            }
            else {
                symbolTableLine.append(QString("%1%2").arg(i.key(), -10).arg(hexString, -4));
                m_ui->assemblerListingTextEdit->append(symbolTableLine);
                symbolTableLine = "";
            }
        }
        if (symbolTableLine.length() > 0) {
            m_ui->assemblerListingTextEdit->append(symbolTableLine);
        }
        m_ui->assemblerListingTextEdit->append("--------------------------------------");
    }
    m_ui->assemblerListingTextEdit->verticalScrollBar()->setValue(m_ui->assemblerListingTextEdit->verticalScrollBar()->minimum());
}

void AssemblerListingPane::clearAssemblerListing()
{
    m_ui->assemblerListingTextEdit->clear();
}

bool AssemblerListingPane::isModified()
{
    return m_ui->assemblerListingTextEdit->document()->isModified();
}

QString AssemblerListingPane::toPlainText()
{
    return m_ui->assemblerListingTextEdit->toPlainText();
}

void AssemblerListingPane::highlightOnFocus()
{
    if (m_ui->assemblerListingTextEdit->hasFocus()) {
        m_ui->assemblerListingLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->assemblerListingLabel->setAutoFillBackground(false);
    }
}

bool AssemblerListingPane::hasFocus()
{
    return m_ui->assemblerListingTextEdit->hasFocus();
}

void AssemblerListingPane::undo()
{
    // does nothing with our current implementation
}

void AssemblerListingPane::redo()
{
    // does nothing with our current implementation
}

void AssemblerListingPane::cut()
{
    // does nothing with our current implementation
}

void AssemblerListingPane::copy()
{
    m_ui->assemblerListingTextEdit->copy();
}

void AssemblerListingPane::paste()
{
    // does nothing with our current implementation
}

