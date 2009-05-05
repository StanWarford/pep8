#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);
}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}
