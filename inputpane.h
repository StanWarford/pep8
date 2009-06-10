#ifndef INPUTPANE_H
#define INPUTPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class InputPane;
}

class InputPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(InputPane)
public:
    explicit InputPane(QWidget *parent = 0);
    virtual ~InputPane();

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

private:
    Ui::InputPane *m_ui;
};

#endif // INPUTPANE_H
