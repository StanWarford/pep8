#ifndef MEMORYTRACEPANE_H
#define MEMORYTRACEPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class MemoryTracePane;
}

class MemoryTracePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MemoryTracePane)
public:
    explicit MemoryTracePane(QWidget *parent = 0);
    virtual ~MemoryTracePane();

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: does nothing in this pane

    void redo();
    // Post: does nothing in this pane

    void cut();
    // Does nothing in our implementation of the memory trace

    void copy();
    // Does nothing in our implementation of the memory trace

    void paste();
    // Does nothing in our implementation of the memory trace

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

private:
    Ui::MemoryTracePane *m_ui;

};

#endif // MEMORYTRACEPANE_H
