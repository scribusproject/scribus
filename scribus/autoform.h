#ifndef AUTOFORM_H
#define AUTOFORM_H

#include <qwidget.h>
#include <qtoolbutton.h>
//Added by qt3to4:
#include <QPixmap>
#include <QGridLayout>
#include <Q3PopupMenu>
#include "scribusapi.h"
class QGridLayout;
class Q3PopupMenu;
class QPixmap;
class AutoformButtonGroup;


class SCRIBUS_API Autoforms : public QToolButton
{
    Q_OBJECT

public:
	Autoforms( QWidget* parent );
	~Autoforms() {};
	QPixmap getIconPixmap(int nr);
	Q3PopupMenu* Men;

public slots:
	void selForm(int a);

signals:
	void FormSel(int, int, double *);

protected:
	QGridLayout* buttonGroup1Layout;
	AutoformButtonGroup* buttonGroup1;
};

#endif

