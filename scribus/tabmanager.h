#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <qvariant.h>
#include <qdialog.h>
#include <qvaluelist.h>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class Tabruler;

class TabManager : public QDialog
{
	Q_OBJECT

public:
	TabManager( QWidget* parent, int dEin, QValueList<double> inTab, double wid);
	~TabManager() {};
	Tabruler* TabList;
	QPushButton* OKButton;
	QPushButton* CancelButton;
	QValueList<double> tmpTab;

public slots:
	void exitOK();

protected:
	QVBoxLayout* TabManagerLayout;
	QHBoxLayout* layout10;
};

#endif // TABMANAGER_H
