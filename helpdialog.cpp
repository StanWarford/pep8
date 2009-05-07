#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

// QTreeWidget: Convenience class
// QTreeWidgetItem: Convenience class

//     QTreeWidgetItem *cities = new QTreeWidgetItem(helpTreeView);
//     cities->setText(0, tr("Cities"));
//     QTreeWidgetItem *osloItem = new QTreeWidgetItem(cities);
//     osloItem->setText(0, tr("Oslo"));
//     osloItem->setText(1, tr("Yes"));

}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}
