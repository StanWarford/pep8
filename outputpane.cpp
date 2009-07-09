#include <QFontDialog>
#include "outputpane.h"
#include "ui_outputpane.h"

OutputPane::OutputPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::OutputPane)
{
    m_ui->setupUi(this);
}

OutputPane::~OutputPane()
{
    delete m_ui;
}

void OutputPane::appendOutput(QString str)
{
    m_ui->pepOutputTextEdit->setText(m_ui->pepOutputTextEdit->toPlainText().append(str));
}

void OutputPane::clearOutput()
{
    m_ui->pepOutputTextEdit->clear();
}

void OutputPane::highlightOnFocus()
{
    if (m_ui->pepOutputTextEdit->hasFocus()) {
        m_ui->pepOutputLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepOutputLabel->setAutoFillBackground(false);
    }
}

bool OutputPane::hasFocus()
{
    return m_ui->pepOutputTextEdit->hasFocus();
}

void OutputPane::undo()
{
    m_ui->pepOutputTextEdit->undo();
}

void OutputPane::redo()
{
    m_ui->pepOutputTextEdit->redo();
}

void OutputPane::cut()
{
    // not allowed, read only
}

void OutputPane::copy()
{
    m_ui->pepOutputTextEdit->copy();
}

void OutputPane::paste()
{
    // not allowed, read only
}

void OutputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepOutputTextEdit->font()), this, "Set Output Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepOutputTextEdit->setFont(font);
    }
}
