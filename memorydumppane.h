#ifndef MEMORYDUMPPANE_H
#define MEMORYDUMPPANE_H

#include <QtGui/QWidget>
#include <QScrollBar>

namespace Ui {
    class MemoryDumpPane;
}

class MemoryDumpPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MemoryDumpPane)
public:
    explicit MemoryDumpPane(QWidget *parent = 0);
    virtual ~MemoryDumpPane();

    void refreshMemory();
    int memDumpPaneWidth();

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: does nothing in this pane

    void redo();
    // Post: does nothing in this pane

private:
    Ui::MemoryDumpPane *m_ui;

private slots:
    void on_pepMemRefreshButton_clicked();
};

#endif // MEMORYDUMPPANE_H
