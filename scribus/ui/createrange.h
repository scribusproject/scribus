/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CREATERANGE
#define CREATERANGE

#include "scribusapi.h"
#include "ui_createrange.h"

#include <QString>
#include <QWidget>
struct CreateRangeData;

class SCRIBUS_API CreateRange : public QDialog, Ui::CreateRange
{
	Q_OBJECT
	public:
		CreateRange(QString currText, int pageCount, QWidget* parent, Qt::WFlags fl=0);
		~CreateRange();
		void getCreateRangeData(CreateRangeData&);
		
	protected slots:
		void selectRangeType(QWidget *);
		void basicAddToRange();
		void basicDelFromRange();
		void basicMoveUp();
		void basicMoveDown();
		void basicSelectRangeTypeConsec();
		void basicSelectRangeTypeComma();
		void basicSelectRangeTypeEven();
		void basicSelectRangeTypeOdd();
		void basicSelectRangeType(int);
		void advSpinChange(int);
		
	protected:
		int m_PageCount;
		int m_RangeType;
		int m_BasicRangeType;
		QString m_PageString;
};

#endif
