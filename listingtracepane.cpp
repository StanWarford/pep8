#include "listingtracepane.h"
#include "ui_listingtracepane.h"

ListingTracePane::ListingTracePane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ListingTracePane)
{
    m_ui->setupUi(this);
}

ListingTracePane::~ListingTracePane()
{
    delete m_ui;
}

void ListingTracePane::setListingTrace(QStringList listingTraceList, QList<bool> hasCheckBox)
{
    QTableWidgetItem *item;
    int numRows = listingTraceList.size();
    m_ui->listingTraceTableWidget->setRowCount(numRows);
    for (int i = 0; i < numRows; i++) {
        item = new QTableWidgetItem(listingTraceList[i]);
        m_ui->listingTraceTableWidget->setItem(i, 1, item);
    }
    for (int i = 0; i < numRows; i++) {
        item = new QTableWidgetItem();
        if (hasCheckBox[i]) {
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
        }
        else {
            item->setFlags(Qt::NoItemFlags);
        }
        m_ui->listingTraceTableWidget->setItem(i, 0, item);
    }
    m_ui->listingTraceTableWidget->resizeColumnsToContents();
    m_ui->listingTraceTableWidget->resizeRowsToContents();
}

void ListingTracePane::clearListingTrace()
{
    for (int i = 0; i < m_ui->listingTraceTableWidget->rowCount(); i++) {
        delete m_ui->listingTraceTableWidget->itemAt(i, 0);
        delete m_ui->listingTraceTableWidget->itemAt(i, 1);
    }
    m_ui->listingTraceTableWidget->setRowCount(0);
}

void ListingTracePane::highlightOnFocus()
{
    if (m_ui->listingTraceTableWidget->hasFocus()) {
        m_ui->listingTraceLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->listingTraceLabel->setAutoFillBackground(false);
    }
}
