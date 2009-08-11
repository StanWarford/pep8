#include <QFontDialog>
#include <QScrollBar>
#include "listingtracepane.h"
#include "ui_listingtracepane.h"
#include "sim.h"
#include "pep.h"

#include <QDebug>

ListingTracePane::ListingTracePane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ListingTracePane)
{
    m_ui->setupUi(this);

    m_ui->listingPepOsTraceTableWidget->hide();

    connect(m_ui->listingTraceTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(updateIsCheckedTable(QTableWidgetItem*)));
    connect(m_ui->listingPepOsTraceTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(updateIsCheckedTable(QTableWidgetItem*)));

    if (Pep::getSystem() != "Mac") {
        m_ui->listingTraceLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
        m_ui->listingPepOsTraceTableWidget->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
        m_ui->listingTraceTableWidget->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    }
}

ListingTracePane::~ListingTracePane()
{
    delete m_ui;
}

void ListingTracePane::setListingTrace(QStringList listingTraceList, QList<bool> hasCheckBox)
{
    // tableWidget depends on whether we are assembling the OS or a program
    QTableWidget *tableWidget;
    if (Pep::memAddrssToAssemblerListing == &Pep::memAddrssToAssemblerListingProg) {
        tableWidget = m_ui->listingTraceTableWidget;
    }
    else {
        tableWidget = m_ui->listingPepOsTraceTableWidget;
    }
    QTableWidgetItem *item;
    int numRows = listingTraceList.size();
    tableWidget->setRowCount(numRows);
    for (int i = 0; i < numRows; i++) {
        item = new QTableWidgetItem(listingTraceList[i]);
        tableWidget->setItem(i, 1, item);
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
        tableWidget->setItem(i, 0, item);
    }
    tableWidget->resizeColumnsToContents();
    m_ui->listingTraceTableWidget->width();
    tableWidget->resizeRowsToContents();
    if (Pep::memAddrssToAssemblerListing == &Pep::memAddrssToAssemblerListingProg) {
        programDocWidth = tableWidget->columnWidth(1);
    }
    else {
        osDocWidth = tableWidget->columnWidth(1);
    }
    resizeDocWidth();
    tableWidget->horizontalScrollBar()->setValue(tableWidget->horizontalScrollBar()->minimum());
}

void ListingTracePane::clearListingTrace()
{
    for (int i = 0; i < m_ui->listingTraceTableWidget->rowCount(); i++) {
        delete m_ui->listingTraceTableWidget->itemAt(i, 0);
        delete m_ui->listingTraceTableWidget->itemAt(i, 1);
    }
    m_ui->listingTraceTableWidget->setRowCount(0);
}

void ListingTracePane::updateListingTrace()
{
    // tableWidget depends on whether we are in the OS or a program
    QTableWidget *tableWidget;
    if (Sim::trapped) {
        tableWidget = m_ui->listingPepOsTraceTableWidget;
        m_ui->listingPepOsTraceTableWidget->show();
        m_ui->listingTraceTableWidget->hide();
    }
    else {
        tableWidget = m_ui->listingTraceTableWidget;
        m_ui->listingPepOsTraceTableWidget->hide();
        m_ui->listingTraceTableWidget->show();
    }

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        tableWidget->item(i, 1)->setBackgroundColor(Qt::white);
        tableWidget->item(i, 1)->setTextColor(Qt::black);
    }
    if (Pep::memAddrssToAssemblerListing->contains(Sim::programCounter)) {
        QTableWidgetItem *highlightedItem = tableWidget->item(Pep::memAddrssToAssemblerListing->value(Sim::programCounter), 1);
        highlightedItem->setBackgroundColor(QColor(56, 117, 215));
        highlightedItem->setTextColor(Qt::white);
        tableWidget->scrollToItem(highlightedItem);
    }
    tableWidget->horizontalScrollBar()->setValue(tableWidget->horizontalScrollBar()->minimum());
}

void ListingTracePane::setDebuggingState(bool b)
{
    QTableWidget *tableWidget;
    if (Sim::trapped) {
        tableWidget = m_ui->listingPepOsTraceTableWidget;
        m_ui->listingPepOsTraceTableWidget->show();
        m_ui->listingTraceTableWidget->hide();
    }
    else {
        tableWidget = m_ui->listingTraceTableWidget;
        m_ui->listingPepOsTraceTableWidget->hide();
        m_ui->listingTraceTableWidget->show();
    }

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        tableWidget->item(i, 1)->setBackgroundColor(Qt::white);
        tableWidget->item(i, 1)->setTextColor(Qt::black);
    }
    if (b && Pep::memAddrssToAssemblerListing->contains(Sim::programCounter)) {
        QTableWidgetItem *highlightedItem = tableWidget->item(Pep::memAddrssToAssemblerListing->value(Sim::programCounter), 1);
        highlightedItem->setBackgroundColor(QColor(56, 117, 215));
        highlightedItem->setTextColor(Qt::white);
        tableWidget->scrollToItem(highlightedItem);
    }
    tableWidget->horizontalScrollBar()->setValue(tableWidget->horizontalScrollBar()->minimum());
}

void ListingTracePane::showAssemblerListing()
{
    m_ui->listingPepOsTraceTableWidget->hide();
    m_ui->listingTraceTableWidget->show();
}

void ListingTracePane::highlightOnFocus()
{
    if (m_ui->listingTraceTableWidget->hasFocus() || m_ui->listingPepOsTraceTableWidget->hasFocus()) {
        m_ui->listingTraceLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->listingTraceLabel->setAutoFillBackground(false);
    }
}

bool ListingTracePane::hasFocus()
{
    return m_ui->listingTraceTableWidget->hasFocus();
}

void ListingTracePane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->listingTraceTableWidget->font()), this, "Set Listing Trace Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->listingTraceTableWidget->setFont(font);
    }
}

void ListingTracePane::resizeDocWidth()
{
    int column0 = m_ui->listingTraceTableWidget->columnWidth(0);
    if (m_ui->listingTraceTableWidget->width() > programDocWidth + column0 + 5) { // + 5 for breathing room
        m_ui->listingTraceTableWidget->setColumnWidth(1, m_ui->listingTraceTableWidget->viewport()->width() - column0);
    }
    else {
        m_ui->listingTraceTableWidget->setColumnWidth(1, programDocWidth);
    }
    if (m_ui->listingPepOsTraceTableWidget->width() > osDocWidth + column0 + 5) { // + 5 for breathing room
        m_ui->listingPepOsTraceTableWidget->setColumnWidth(1, m_ui->listingPepOsTraceTableWidget->viewport()->width() - column0);
    }
    else {
        m_ui->listingPepOsTraceTableWidget->setColumnWidth(1, osDocWidth);
    }
}

void ListingTracePane::updateIsCheckedTable(QTableWidgetItem *item)
{
    Pep::listingRowChecked->insert(item->row(), item->checkState());
}
