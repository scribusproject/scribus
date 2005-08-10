#ifndef SHADEBUTTON_H
#define SHADEBUTTON_H

#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include "scribusapi.h"

class SCRIBUS_API ShadeButton : public QToolButton
{

Q_OBJECT

public:
	ShadeButton(QWidget* parent);
	~ShadeButton() {};
	QPopupMenu *FillSh;
	int getValue();
	void setValue(int val);

private slots:
	virtual void setShade(int id);
};
#endif

