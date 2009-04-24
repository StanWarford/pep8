#include "memorydumppane.h"
#include "ui_memorydumppane.h"

MemoryDumpPane::MemoryDumpPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MemoryDumpPane)
{
    m_ui->setupUi(this);
}

MemoryDumpPane::~MemoryDumpPane()
{
    delete m_ui;
}
