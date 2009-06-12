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
    // Else: Do nothing.

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file



private:
    Ui::CpuPane *m_ui;
};

#endif // CPUPANE_H
