#ifndef MEMORYDUMPPANE_H
#define MEMORYDUMPPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class MemoryDumpPane;
}

class MemoryDumpPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MemoryDumpPane)
public:
    explicit MemoryDumpPane(QWidget *parent = 0);
    virtual ~MemoryDumpPane();

private:
    Ui::MemoryDumpPane *m_ui;
};

#endif // MEMORYDUMPPANE_H
