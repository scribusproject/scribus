#ifndef PAGESELECTOR_H
#define PAGESELECTOR_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qvalidator.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QLabel;
class QPushButton;

class PageSelector : public QWidget
{
	Q_OBJECT

public:
	PageSelector( QWidget* parent, int maxPg );
	~PageSelector() {};

	QPushButton* Start;
	QPushButton* Back;
	QLabel* Label1;
	QComboBox* PageCombo;
	QLabel* Label2;
	QPushButton* Forward;
	QPushButton* Last;
	QIntValidator *v;
	int LastPG;
	int APage;

public slots:
	virtual void GotoPg(int);
	virtual void setMaxValue(int);

private slots:
	virtual void GotoPgE(int);
	virtual void ToStart();
	virtual void ToEnd();
	virtual void goBk();
	virtual void goFw();

protected:
	QHBoxLayout* PageSelectorLayout;

signals:
	void GotoPage(int);

};

#endif // PAGESELECTOR_H
