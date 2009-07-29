#ifndef CPUPANE_H
#define CPUPANE_H

#include <QtGui/QWidget>
#include "enu.h"

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

    void setDebugState(bool b);
    // Post: if b is true, checkboxes are set to disabled, and vice versa

    void setButtonsEnabled(bool b);
    // Post: if b is true, buttons are enabled, and vice versa

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

    void trapLookahead();
    // Looks ahead to the next instruction to determine if we are trapping

    void resumeThroughTrap();
    // Resumes through the trap to continue tracing the program

    void interruptExecution();
    // Post: interruptExecutionFlag is set to true

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    Enu::EWaiting waitingState();
    // Post: Returns the waiting state of the simulation (for terminal io)

private:
    Ui::CpuPane *m_ui;

    bool interruptExecutionFlag; // Used to interrupt execution by the user

    Enu::EWaiting waiting; // Used to store terminal IO waiting for input state

signals:
    void resumeButtonClicked();
    void singleStepButtonClicked();
    void updateSimulationView();
    void executionComplete();
    void appendOutput(QString);
    void vonNeumannStepped();
    void waitingForInput();
};

#endif // CPUPANE_H
