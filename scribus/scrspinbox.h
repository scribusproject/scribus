/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Craig Bradney, cbradney@zip.com.au                                    *
 ***************************************************************************/

#ifndef SCRSPINBOX
#define SCRSPINBOX

#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QWidget>

#include "fparser.h"

//!brief ScrSpinBox is a Qt4 replacement for our old ScrSpinBox using Qt3
class ScrSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
	public:
		ScrSpinBox(QWidget *parent, int unitIndex=0);
		ScrSpinBox(double minValue, double maxValue, QWidget *pa, int unitIndex=0);
		~ScrSpinBox();
		
		//overridden members
		double valueFromText ( const QString & text ) const;
		QValidator::State validate ( QString & input, int & pos ) const;
		
		//custom
		void init(int unitIndex);
		void setNewUnit(int unitIndex);
		double getValue(int unitIndex=0);
		//implemented for backwards compatibility, perhaps we can remove this
		void setLineStepM(int val);
		
	public slots:
		void setValues(double min, double max, int deci, double val);
		void getValues(double *min, double *max, int *deci, double *val);
	
	protected:
		void setParameters( int s );
		uint m_unitIndex;
		bool eventFilter ( QObject * watched, QEvent * event );
		
	protected slots:
		void textChanged();

};
#endif
