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

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: the last action in the text edit is undone

    void redo();
    // Post: the last undo in the text edit is redone

    bool isUndoable;
    // The undoability of the text edit is stored here

    bool isRedoable;
    // The redoability of the text edit is stored here

    void cut();
    // Post: selected text in the text edit is cut to the clipboard

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void paste();
    // Post: selected text in the clipboard is pasted to the text edit

    void setReadOnly(bool b);
    // Post: the text edit's read only attribute is set to b

private:
    Ui::InputPane *m_ui;

private slots:
    void setUndoability(bool b);
    void setRedoability(bool b);

signals:
    void undoAvailable(bool);
    void redoAvailable(bool);

};

#endif // INPUTPANE_H
