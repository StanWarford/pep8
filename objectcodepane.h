#ifndef OBJECTCODEPANE_H
#define OBJECTCODEPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class ObjectCodePane;
}

class ObjectCodePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ObjectCodePane)
public:
    explicit ObjectCodePane(QWidget *parent = 0);
    virtual ~ObjectCodePane();

    void setObjectCode(QList<int> objectCode);
    // Post Sets text in source code pane.

    void setObjectCodePaneText(QString string);
    // Post: Sets text in source code pane to string.

    bool getObjectCode(QList<int> &objectCodeList);
    // Pre: The object code pane contains object code
    // Post: If the object code is syntactically correct, true is returned, and
    // &objectCodeList contains the object code, one byte per integer.
    // Otherwise, false is returned.

    void clearObjectCode();
    // Post: Clears the source code pane

    bool isModified();
    // Post: Returns true if the source code pane has been modified

    void setModifiedFalse();
    // Post: Sets isModified of the source code pane to false

    QString toPlainText();
    // Post: Contents of the source code pane are returned

    void setCurrentFile(QString string);
    // Post: Title bar of the window is set to "Object Code - 'string'"

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

private:
    Ui::ObjectCodePane *m_ui;

private slots:
    void setLabelToModified(bool modified);
    void setUndoability(bool b);
    void setRedoability(bool b);
};

#endif // OBJECTCODEPANE_H
