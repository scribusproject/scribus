#ifndef HYASK_H
#define HYASK_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

class WortEdit : public QLineEdit
{
    Q_OBJECT
public:
	WortEdit(QWidget* parent);
	~WortEdit() {};
	void keyPressEvent(QKeyEvent *k);
};

class HyAsk : public QDialog
{ 
    Q_OBJECT

public:
    HyAsk( QWidget* parent, QString HWort );
    ~HyAsk() {};

    WortEdit* Wort;
    QPushButton* OK;
    QPushButton* Skip;
    QPushButton* Cancel;

public slots:
		void Check();
		void DoSkip();

protected:
    QVBoxLayout* HyAskLayout;
    QHBoxLayout* Layout1;
};

#endif // HYASK_H
