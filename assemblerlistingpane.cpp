#include "assemblerlistingpane.h"
#include "ui_assemblerlistingpane.h"

AssemblerListingPane::AssemblerListingPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::AssemblerListingPane)
{
    m_ui->setupUi(this);
}

AssemblerListingPane::~AssemblerListingPane()
{
    delete m_ui;
}

void AssemblerListingPane::setAssemblerListing(QStringList assemblerListingList, QList<bool> hasCheckBox)
{
    int numRows = assemblerListingList.size();
    m_ui->assemblerListingTable->setRowCount(numRows);
    for (int i = 0; i < numRows; i++) {
        m_ui->assemblerListingTable->setItem(i, 1, new QTableWidgetItem(assemblerListingList[i]));
    }
    for (int i = 0; i < numRows; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        if (hasCheckBox[i]) {
            item->setFlags(Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
        }
        else {
            item->setFlags(Qt::NoItemFlags);
        }
        m_ui->assemblerListingTable->setItem(i, 0, item);
    }
    m_ui->assemblerListingTable->resizeColumnsToContents();
    m_ui->assemblerListingTable->setColumnWidth(0, m_ui->assemblerListingTable->columnWidth(0) + 5);
    m_ui->assemblerListingTable->setColumnWidth(1, m_ui->assemblerListingTable->columnWidth(1) + 200);
    m_ui->assemblerListingTable->resizeRowsToContents();




    // QTableWidgetItem * QTableWidget::itemFromIndex ( const QModelIndex & index ) const   [protected]


    /*

        State::mainWindow->pepAssemblerListingTable->setRowCount( listingTxtList.size() );

        QString listingRow;
        int rowCount = State::mainWindow->pepAssemblerListingTable->rowCount();
        QTableWidgetItem* item1;
        for ( int row = 0; row < rowCount; row++ ) {
            item1 = new QTableWidgetItem();
            listingRow = listingTxtList[row];
            if (( listingRow != NULL &&
                  !listingRow.startsWith( "Addr" ) &&
                  !listingRow.startsWith( "Symbol" ) &&
                  !listingRow.startsWith( " " ) &&
                  !listingRow.startsWith( "\t" ) &&
                  !listingRow.startsWith( "\n" ) &&
                  !listingRow.startsWith( "No errors" ) &&
                  !listingRow.startsWith( ";" )) &&

                (
                    ((listingRow.size() > 13) && !(listingRow.at( 13 ) == '.')) ||
                    ((listingRow.size() > 22) && !(listingRow.at( 22 ) == '.'))
                    )
                )

            {
                QTableWidgetItem* chkBoxItem = new QTableWidgetItem();
                chkBoxItem->setCheckState(Qt::Unchecked);
                State::mainWindow->pepAssemblerListingTable->setItem(row, 0, chkBoxItem);
            }
            State::mainWindow->pepAssemblerListingTable->setItem(row, 1, new QTableWidgetItem(listingRow));
        }
        State::mainWindow->pepAssemblerListingTable->resizeColumnsToContents();
        State::mainWindow->pepAssemblerListingTable->setColumnWidth(0, State::mainWindow->pepAssemblerListingTable->columnWidth(0) + 5);
        State::mainWindow->pepAssemblerListingTable->setColumnWidth(1, State::mainWindow->pepAssemblerListingTable->columnWidth(1) + 200);
        State::mainWindow->pepAssemblerListingTable->resizeRowsToContents();


      */

}
