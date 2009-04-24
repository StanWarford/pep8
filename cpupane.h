#ifndef CPUPANE_H
#define CPUPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class CpuPane;
}

class CpuPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(CpuPane)
public:
    explicit CpuPane(QWidget *parent = 0);
    virtual ~CpuPane();

private:
    Ui::CpuPane *m_ui;
};

#endif // CPUPANE_H
