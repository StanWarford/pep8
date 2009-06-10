#ifndef LISTINGTRACEPANE_H
#define LISTINGTRACEPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class ListingTracePane;
}

class ListingTracePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ListingTracePane)
public:
    explicit ListingTracePane(QWidget *parent = 0);
    virtual ~ListingTracePane();

    void setListingTrace(QStringList listingTraceList, QList<bool> hasCheckBox);
    // Post: Populate listingTraceTable from assemblerListingList and hasCheckBox

    void clearListingTrace();

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

private:
    Ui::ListingTracePane *m_ui;
};

#endif // LISTINGTRACEPANE_H
