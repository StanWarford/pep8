#ifndef ASSEMBLERLISTINGPANE_H
#define ASSEMBLERLISTINGPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class AssemblerListingPane;
}

class AssemblerListingPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(AssemblerListingPane)
public:
    explicit AssemblerListingPane(QWidget *parent = 0);
    virtual ~AssemblerListingPane();
    void setAssemblerListing(QStringList assemblerListingList);
    void clearAssemblerListing();

private:
    Ui::AssemblerListingPane *m_ui;
};

#endif // ASSEMBLERLISTINGPANE_H
