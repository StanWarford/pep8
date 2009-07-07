#include <QFontDialog>
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

    connect(m_ui->listingSingleStepPushButton, SIGNAL(clicked()), this, SLOT(singleStep()));

    connect(m_ui->listingResumePushButton, SIGNAL(clicked()), this, SLOT(resumeExecution()));

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

bool ListingTracePane::hasFocus()
{
    return m_ui->listingTraceTableWidget->hasFocus();
}

void ListingTracePane::undo()
{
    // does nothing with our current implementation
}

void ListingTracePane::redo()
{
    // does nothing with our current implementation
}

void ListingTracePane::cut()
{
    // does nothing with our current implementation
}

void ListingTracePane::copy()
{
    // does nothing with our current implementation
}

void ListingTracePane::paste()
{
    // does nothing with our current implementation
}

void ListingTracePane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->listingTraceTableWidget->font()), this, "Set Listing Trace Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->listingTraceTableWidget->setFont(font);
    }
}

void ListingTracePane::setButtonsDisabled(bool b)
{
    m_ui->listingResumePushButton->setDisabled(b);
    m_ui->listingSingleStepPushButton->setDisabled(b);
}

void ListingTracePane::beginExecution()
{
    for (int i = 0; i < m_ui->listingTraceTableWidget->rowCount(); i++) {
        m_ui->listingTraceTableWidget->item(i, 1)->setBackgroundColor(Qt::white);
        m_ui->listingTraceTableWidget->item(i, 1)->setTextColor(Qt::black);
    }
    if (Pep::memAddrssToAssemblerListing.contains(Sim::programCounter)) {
        int row = Pep::memAddrssToAssemblerListing.value(Sim::programCounter);
        m_ui->listingTraceTableWidget->item(row, 1)->setBackgroundColor(QColor(56, 117, 215));
        m_ui->listingTraceTableWidget->item(row, 1)->setTextColor(Qt::white);
    }
}

void ListingTracePane::singleStep()
{
    Sim::vonNeumannStep();
    emit updateCpuAndMemoryTrace();
    if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
        for (int i = 0; i < m_ui->listingTraceTableWidget->rowCount(); i++) {
            m_ui->listingTraceTableWidget->item(i, 1)->setBackgroundColor(Qt::white);
            m_ui->listingTraceTableWidget->item(i, 1)->setTextColor(Qt::black);
        }
        if (Pep::memAddrssToAssemblerListing.contains(Sim::programCounter)) {
            int row = Pep::memAddrssToAssemblerListing.value(Sim::programCounter);
            m_ui->listingTraceTableWidget->item(row, 1)->setBackgroundColor(QColor(56, 117, 215));
            m_ui->listingTraceTableWidget->item(row, 1)->setTextColor(Qt::white);
        }
    }
    else {
        m_ui->listingResumePushButton->setDisabled(true);
        m_ui->listingSingleStepPushButton->setDisabled(true);
        for (int i = 0; i < m_ui->listingTraceTableWidget->rowCount(); i++) {
            m_ui->listingTraceTableWidget->item(i, 1)->setBackgroundColor(Qt::white);
            m_ui->listingTraceTableWidget->item(i, 1)->setTextColor(Qt::black);
        }
        emit executionComplete();
    }
}

void ListingTracePane::resumeExecution()
{
    while (Sim::instructionSpecifier) {
        Sim::vonNeumannStep();
    }
}
