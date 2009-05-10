#ifndef SOURCECODEPANE_H
#define SOURCECODEPANE_H

#include <QtGui/QWidget>
#include <QString>
#include <QList>

namespace Ui {
    class SourceCodePane;
}

class SourceCodePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(SourceCodePane)
public:
    explicit SourceCodePane(QWidget *parent = 0);
    virtual ~SourceCodePane();
    void assemble();
    QList<int> getObjectCode();
    QStringList getAssemblerListingList();
    QList<bool> getHasCheckBox();

private:
    Ui::SourceCodePane *m_ui;
    QList<int> objectCode;
    QStringList assemblerListingList;
    QList<bool> hasCheckBox;

};

#endif // SOURCECODEPANE_H
