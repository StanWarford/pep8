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

void AssemblerListingPane::setAssemblerListing(QStringList assemblerListingList) {
    m_ui->assemblerListingTextEdit->setText(assemblerListingList.join("\n"));
}
