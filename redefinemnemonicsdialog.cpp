#include "redefinemnemonicsdialog.h"
#include "ui_redefinemnemonicsdialog.h"

RedefineMnemonicsDialog::RedefineMnemonicsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::RedefineMnemonicsDialog)
{
    m_ui->setupUi(this);
}

RedefineMnemonicsDialog::~RedefineMnemonicsDialog()
{
    delete m_ui;
}
