#include "helpdialog.h"
#include "ui_helpdialog.h"

#include <QDebug>

HelpDialog::HelpDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->helpTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(on_itemClicked(QTreeWidgetItem*,int)));
}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}

void HelpDialog::on_itemClicked(QTreeWidgetItem *helpItem, int) {
    //    qDebug() << m_ui->helpTreeWidget->indexOfTopLevelItem(helpItem);
    qDebug() << "Is valid? : " << m_ui->helpTreeWidget->currentIndex().parent().isValid();
    qDebug() << "Parent row: " << m_ui->helpTreeWidget->currentIndex().parent().row();
    qDebug() << "Row: " << m_ui->helpTreeWidget->currentIndex().row();

//    m_ui->leftHelpWebView->load(QUrl("http://slashdot.org"));
    m_ui->rightHelpWebView->hide();
    m_ui->leftHelpWebView->load(QUrl("qrc:/help/fig0518.pep.txt"));
    m_ui->leftHelpWebView->show();


}
