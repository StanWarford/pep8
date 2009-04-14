#ifndef _SOURCECODEWINDOW
#define _SOURCECODEWINDOW

#include "ui_SourceCodeWindow.h"

class SourceCodeWindow : public QWidget, private Ui::SourceCodeWindowWidget
{
Q_OBJECT

private:
	
public:
    SourceCodeWindow(QWidget *parent = 0);
	bool isEmpty();
	
////////////////////////////////// Edit Menu Functions////////////////////////////////  
	void editUndo();
	void editRedo();
	void editCut();
	void editCopy();
	void editPaste();
	void editFont();
	QString text();
private slots:
	// None
signals:
	// None

public slots:
	void update(QString);
	void clear();
	void removeErrorMessages();
signals:
	// None
	
protected:
    bool eventFilter( QObject *dist, QEvent *event);
};
 
#endif
