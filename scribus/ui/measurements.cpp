/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cmath>

#include "measurements.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "units.h"

using namespace std;


Measurements::Measurements( QWidget* parent ) : ScrPaletteBase( parent, "MeasurementsPalette", false, 0 )
{
	setupUi(this);

	mX1 = mY1 = 0;
	mX2 = mY2 = 0;
	mDX = mDY = 0;
	mLength = 0;

	const QString widthString="10000.0000";
	int textWidth = fontMetrics().width(widthString);
	x1Data->setMinimumSize(textWidth, 12);
	y1Data->setMinimumSize(textWidth, 12);
	x2Data->setMinimumSize(textWidth, 12);
	y2Data->setMinimumSize(textWidth, 12);
	dXData->setMinimumSize(textWidth, 12);
	dYData->setMinimumSize(textWidth, 12);
	angleData->setMinimumSize(textWidth, 12);
	lengthData->setMinimumSize(textWidth, 12);
	int maxUindex = unitGetMaxIndex() - 2;
	textWidth = fontMetrics().width("mmm")+12;
	for (int i = 0; i <= maxUindex; ++i)
		unitSwitch->addItem(unitGetStrFromIndex(i));
	int minHeight = unitSwitch->minimumSizeHint().height();
	unitSwitch->setMinimumSize(textWidth, minHeight);
	resize(minimumSizeHint());

	// Try to start with doc unit if possible
	ScribusView* view = qobject_cast<ScribusView*>(parent);
	if (view)
	{
		ScribusDoc* doc = view->Doc;
		if (doc)
		{
			int unitIndex = doc->unitIndex();
			unitSwitch->setCurrentIndex(unitIndex);
		}
	}
	setValues(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	
	connect(unitSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
}

void Measurements::setValues(double x1, double y1, double x2, double y2, double angle, double len)
{
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
	mDX = x2 - x1;
	mDY = y2 - y1;
	mLength = len;
	QString tmp;
	double rr = angle;
	if (angle < 0)
		rr = rr + 360;
	angleData->setText(tmp.setNum(fabs(rr), 'f', 2)+ trUtf8(" °"));
	unitChanged();
}

void Measurements::unitChanged()
{
	QString tmp;
	int uInd = unitSwitch->currentIndex();
	int uDec = unitGetDecimalsFromIndex(uInd);
	if (uDec == 0)
		uDec = 1; // Unlikely but avoid division by 0 if unitGetDecimalsFromIndex() returns 0
	int uPrec = unitGetPrecisionFromIndex(uInd);
	double uRatio = unitGetRatioFromIndex(uInd);
	QString uStr  = unitGetStrFromIndex(uInd);
	x1Data->setText(tmp.setNum(qRound(mX1 * uRatio * uDec) / static_cast<double>(uDec), 'f', uPrec) + " " + uStr);
	y1Data->setText(tmp.setNum(qRound(mY1 * uRatio * uDec) / static_cast<double>(uDec), 'f', uPrec) + " " + uStr);
	x2Data->setText(tmp.setNum(qRound(mX2 * uRatio * uDec) / static_cast<double>(uDec), 'f', uPrec) + " " + uStr);
	y2Data->setText(tmp.setNum(qRound(mY2 * uRatio * uDec) / static_cast<double>(uDec), 'f', uPrec) + " " + uStr);
	dXData->setText(tmp.setNum(qRound(mDX * uRatio * uDec) / static_cast<double>(uDec), 'f', uPrec) + " " + uStr);
	dYData->setText(tmp.setNum(qRound(mDY * uRatio * uDec) / static_cast<double>(uDec), 'f', uPrec) + " " + uStr);
	lengthData->setText(tmp.setNum(qRound(mLength * uRatio * uDec) / static_cast<double>(uDec), 'f', uPrec) + " " + uStr);
}

void Measurements::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void Measurements::languageChange()
{
	retranslateUi(this);
}
