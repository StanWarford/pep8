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

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: does nothing in this pane

    void redo();
    // Post: does nothing in this pane

    void cut();
    // Does nothing in our implementation of the listing trace

    void copy();
    // Does nothing in our implementation of the listing trace

    void paste();
    // Does nothing in our implementation of the listing trace

private:
    Ui::ListingTracePane *m_ui;

private slots:
    void singleStep();

    void resumeExecution();

};

#endif // LISTINGTRACEPANE_H
