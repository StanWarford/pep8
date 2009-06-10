#ifndef TERMINALPANE_H
#define TERMINALPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class TerminalPane;
}

class TerminalPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TerminalPane)
public:
    explicit TerminalPane(QWidget *parent = 0);
    virtual ~TerminalPane();

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

private:
    Ui::TerminalPane *m_ui;
};

#endif // TERMINALPANE_H
