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
    void clearObjectCode();

private:
    Ui::ObjectCodePane *m_ui;
};

#endif // OBJECTCODEPANE_H
