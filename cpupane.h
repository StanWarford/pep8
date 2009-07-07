#ifndef CPUPANE_H
#define CPUPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class CpuPane;
}

class CpuPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(CpuPane)
public:
    explicit CpuPane(QWidget *parent = 0);
    virtual ~CpuPane();

    void updateCpu();
    // Post: Updates CPU pane labels

    void runClicked();
    // Post: All of the debug checkboxes are unclicked

    void startDebuggingClicked();
    // Post: If no checkboxes are clicked, click the trace program checkbox

    void setExecutionState(bool b);
    // Post: if b is true, checkboxes are set to disabled, and vice versa

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: does nothing in this pane

    void redo();
    // Post: does nothing in this pane

    void cut();
    // Does nothing in our implementation of the cpu pane

    void copy();
    // Does nothing in our implementation of the cpu pane

    void paste();
    // Does nothing in our implementation of the cpu pane

private:
    Ui::CpuPane *m_ui;
};

#endif // CPUPANE_H
