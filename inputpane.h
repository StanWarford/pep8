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

    QString toPlainText();
    // Post: input text edit text is returned

    void setText(QString input);
    // Post: the input pane text edit text is set to string

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: the last action in the text edit is undone

    void redo();
    // Post: the last undo in the text edit is redone

    bool isUndoable();
    // Returns the undoability of the text edit

    bool isRedoable();
    // Returns the redoability of the text edit

    void cut();
    // Post: selected text in the text edit is cut to the clipboard

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void paste();
    // Post: selected text in the clipboard is pasted to the text edit

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

    void setReadOnly(bool b);
    // Post: the text edit's read only attribute is set to b

private:
    Ui::InputPane *m_ui;

    bool eventFilter(QObject *, QEvent *event);
    QString tab(int curLinePos);

signals:
    void undoAvailable(bool);
    void redoAvailable(bool);

};

#endif // INPUTPANE_H
