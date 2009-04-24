#include "memorytracepane.h"
#include "ui_memorytracepane.h"

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
