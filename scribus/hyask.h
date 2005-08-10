#ifndef HYASK_H
#define HYASK_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

#include "scribusapi.h"

class SCRIBUS_API WortEdit : public QLineEdit
{
    Q_OBJECT
public:
	WortEdit(QWidget* parent);
	~WortEdit() {};
	void keyPressEvent(QKeyEvent *k);
};

class SCRIBUS_API HyAsk : public QDialog
{ 
    Q_OBJECT

public:
    HyAsk( QWidget* parent, QString HWort );
    ~HyAsk() {};

    WortEdit* Wort;
    QPushButton* OK;
    QPushButton* Skip;
    QPushButton* Cancel;
	int xpos;
	int ypos;

public slots:
	void Check();
	void DoSkip();
	void accept();
	void reject();

protected:
    QVBoxLayout* HyAskLayout;
    QHBoxLayout* Layout1;
};

#endif // HYASK_H
