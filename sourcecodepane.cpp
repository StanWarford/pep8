#include "sourcecodepane.h"
#include "ui_sourcecodepane.h"

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

void SourceCodePane::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
