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

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

    void setDebuggingState(bool b);
    // Post: resume and single step buttons clickability is set to b
    // Also clears the selection

    void runWithBatch();
    // Runs the simulator through with batch input

    void runWithTerminal();
    // Runs the simulator through with terminal i/o

private:
    Ui::ListingTracePane *m_ui;

private slots:
    void singleStep();
    void resumeExecution();

signals:
    void updateCpuAndMemoryTrace();
    void executionComplete();
    void appendOutput(QString);
};

#endif // LISTINGTRACEPANE_H
