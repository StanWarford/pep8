#ifndef SOURCECODEPANE_H
#define SOURCECODEPANE_H

#include <QtGui/QWidget>
#include <QString>

namespace Ui {
    class SourceCodePane;
}

class SourceCodePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(SourceCodePane)
public:
    explicit SourceCodePane(QWidget *parent = 0);
    virtual ~SourceCodePane();
    QString getSourceCode();

private:
    Ui::SourceCodePane *m_ui;
};

#endif // SOURCECODEPANE_H
