#ifndef MEMORYDUMPPANE_H
#define MEMORYDUMPPANE_H

#include <QtGui/QWidget>
#include <QScrollBar>
#include <QSet>

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
    // Post: the entire memory pane is refreshed

    void refreshMemoryLines(int firstByte, int lastByte);
    // Post: The memory dump is refresed from the line containing startByte to the line
    // containing endByte. Called by load().

    void highlightMemory(bool b);
    // Post: Everything is unhighlighted. If b, current program counter is highlighted.

    void cacheModifiedBytes();
    // Post: Changed bytes from Sim are added to the QSet modifiedBytes

    void updateMemory();
    // Post: Memory displays are updated using the changedMemoryAddrss qlist in sim

    void scrollToTop();
    // Post: Memory dump is scrolled to the top left corner

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

    int memoryDumpWidth();
    // Post: the width of the memory dump text edit document is returned

    QSize sizeHint();

private:
    Ui::MemoryDumpPane *m_ui;

    void highlightByte(int memAddr, QColor foreground, QColor background);

    QList<int> highlightedData;
    QSet<int> modifiedBytes;
    QList<int> bytesWrittenLastStep;

private slots:
    void on_pepMemRefreshButton_clicked();
};

#endif // MEMORYDUMPPANE_H
