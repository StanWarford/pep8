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

    void clearCpu();
    // Post: The CPU pane labels are blanked

    void runClicked();
    // Post: All of the debug checkboxes are unclicked

    void startDebuggingClicked();
    // Post: If no checkboxes are clicked, click the trace program checkbox

    void setExecutionState(bool b);
    // Post: if b is true, checkboxes are set to disabled, and vice versa

    void runWithBatch();
    // Runs the simulator through with batch input

    void runWithTerminal();
    // Runs the simulator through with terminal i/o

    void resumeWithBatch();
    // Resumes the simulator with batch input

    void resumeWithTerminal();
    // Resumes the simulator with terminal input

    void singleStepWithBatch();
    // Single steps the simulator with batch input

    void singleStepWithTerminal();
    // Single steps the simulator with terminal input

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

signals:
    void resumeButtonClicked();
    void singleStepButtonClicked();
    void updateSimulationView();
    void executionComplete();
    void appendOutput(QString);
};

#endif // CPUPANE_H
