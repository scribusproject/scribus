#ifndef SHADEBUTTON_H
#define SHADEBUTTON_H

#include <qtoolbutton.h>
#include <q3popupmenu.h>
#include "scribusapi.h"

class SCRIBUS_API ShadeButton : public QToolButton
{

Q_OBJECT

public:
	ShadeButton(QWidget* parent);
	~ShadeButton() {};
	Q3PopupMenu *FillSh;
	int getValue();
	void setValue(int val);

private slots:
	virtual void setShade(int id);
};
#endif

