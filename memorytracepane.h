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

private:
    Ui::MemoryTracePane *m_ui;
};

#endif // MEMORYTRACEPANE_H
