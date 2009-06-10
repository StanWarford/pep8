#ifndef OUTPUTPANE_H
#define OUTPUTPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class OutputPane;
}

class OutputPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(OutputPane)
public:
    explicit OutputPane(QWidget *parent = 0);
    virtual ~OutputPane();

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

private:
    Ui::OutputPane *m_ui;
};

#endif // OUTPUTPANE_H
