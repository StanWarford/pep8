#ifndef LISTINGTRACEPANE_H
#define LISTINGTRACEPANE_H

#include <QtGui/QWidget>
#include <QTableWidgetItem>

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

    void updateListingTrace();
    // Post: the listing trace pane selection is updated

    void setDebuggingState(bool b);
    // Post: resume and single step buttons clickability is set to b
    // Also clears the selection

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

private:
    Ui::ListingTracePane *m_ui;

private slots:
    void updateIsCheckedTable(QTableWidgetItem *item);

};

#endif // LISTINGTRACEPANE_H
