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

    void appendOutput(QString str);
    // Post: str is appended to the text edit

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: the last action in the text edit is undone

    void redo();
    // Post: the last undo in the text edit is redone

    void cut();
    // Not allowed in this pane (read only)

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void paste();
    // Not allowed in this pane (read only)

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

private:
    Ui::OutputPane *m_ui;
};

#endif // OUTPUTPANE_H
