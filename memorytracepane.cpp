#include <QFontDialog>
#include "memorytracepane.h"
#include "ui_memorytracepane.h"
#include "sim.h"

MemoryTracePane::MemoryTracePane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MemoryTracePane)
{
    m_ui->setupUi(this);

}

MemoryTracePane::~MemoryTracePane()
{
    delete m_ui;
}

void MemoryTracePane::highlightOnFocus()
{
    if (m_ui->pepStackTraceGraphicsView->hasFocus()) {
        m_ui->pepMemoryTraceLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepMemoryTraceLabel->setAutoFillBackground(false);
    }
}

bool MemoryTracePane::hasFocus()
{
    return m_ui->pepStackTraceGraphicsView->hasFocus();
}

void MemoryTracePane::undo()
{
    // does nothing with our current implementation
}

void MemoryTracePane::redo()
{
    // does nothing with our current implementation
}

void MemoryTracePane::cut()
{
    // does nothing with our current implementation
}

void MemoryTracePane::copy()
{
    // does nothing with our current implementation
}

void MemoryTracePane::paste()
{
    // does nothing with our current implementation
}

void MemoryTracePane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepStackTraceGraphicsView->font()), this, "Set Object Code Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepStackTraceGraphicsView->setFont(font);
    }
}
