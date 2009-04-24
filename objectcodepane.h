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

private:
    Ui::ObjectCodePane *m_ui;
};

#endif // OBJECTCODEPANE_H
