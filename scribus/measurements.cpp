/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "measurements.h"
#include <cmath>
#include "units.h"

using namespace std;


Measurements::Measurements( QWidget* parent ) : ScrPaletteBase( parent, "MeasurementsPalette", false, 0 )
{
	setupUi(this);
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
	{
		unitSwitchX1->addItem(unitGetStrFromIndex(i));
		unitSwitchY1->addItem(unitGetStrFromIndex(i));
		unitSwitchX2->addItem(unitGetStrFromIndex(i));
		unitSwitchY2->addItem(unitGetStrFromIndex(i));
		unitSwitchLength->addItem(unitGetStrFromIndex(i));
		unitSwitchDX->addItem(unitGetStrFromIndex(i));
		unitSwitchDY->addItem(unitGetStrFromIndex(i));
		unitSwitchX1->setMinimumSize(textWidth, 12);
		unitSwitchY1->setMinimumSize(textWidth, 12);
		unitSwitchX2->setMinimumSize(textWidth, 12);
		unitSwitchY2->setMinimumSize(textWidth, 12);
		unitSwitchLength->setMinimumSize(textWidth, 12);
		unitSwitchDX->setMinimumSize(textWidth, 12);
		unitSwitchDY->setMinimumSize(textWidth, 12);
	}
	resize(minimumSizeHint());
	connect(unitSwitchX1, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
	connect(unitSwitchY1, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
	connect(unitSwitchX2, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
	connect(unitSwitchY1, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
	connect(unitSwitchDX, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
	connect(unitSwitchDY, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
	connect(unitSwitchLength, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged()));
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
	int uInd = unitSwitchX1->currentIndex();
	x1Data->setText(tmp.setNum(qRound(mX1*unitGetRatioFromIndex(uInd)*unitGetDecimalsFromIndex(uInd)) / static_cast<double>(unitGetDecimalsFromIndex(uInd)), 'f', unitGetPrecisionFromIndex(uInd)));
	uInd = unitSwitchY1->currentIndex();
	y1Data->setText(tmp.setNum(qRound(mY1*unitGetRatioFromIndex(uInd)*unitGetDecimalsFromIndex(uInd)) / static_cast<double>(unitGetDecimalsFromIndex(uInd)), 'f', unitGetPrecisionFromIndex(uInd)));
	uInd = unitSwitchX1->currentIndex();
	x2Data->setText(tmp.setNum(qRound(mX2*unitGetRatioFromIndex(uInd)*unitGetDecimalsFromIndex(uInd)) / static_cast<double>(unitGetDecimalsFromIndex(uInd)), 'f', unitGetPrecisionFromIndex(uInd)));
	uInd = unitSwitchY2->currentIndex();
	y2Data->setText(tmp.setNum(qRound(mY2*unitGetRatioFromIndex(uInd)*unitGetDecimalsFromIndex(uInd)) / static_cast<double>(unitGetDecimalsFromIndex(uInd)), 'f', unitGetPrecisionFromIndex(uInd)));
	uInd = unitSwitchDX->currentIndex();
	dXData->setText(tmp.setNum(qRound(mDX*unitGetRatioFromIndex(uInd)*unitGetDecimalsFromIndex(uInd)) / static_cast<double>(unitGetDecimalsFromIndex(uInd)), 'f', unitGetPrecisionFromIndex(uInd)));
	uInd = unitSwitchDY->currentIndex();
	dYData->setText(tmp.setNum(qRound(mDY*unitGetRatioFromIndex(uInd)*unitGetDecimalsFromIndex(uInd)) / static_cast<double>(unitGetDecimalsFromIndex(uInd)), 'f', unitGetPrecisionFromIndex(uInd)));
	uInd = unitSwitchLength->currentIndex();
	lengthData->setText(tmp.setNum(qRound(mLength*unitGetRatioFromIndex(uInd)*unitGetDecimalsFromIndex(uInd)) / static_cast<double>(unitGetDecimalsFromIndex(uInd)), 'f', unitGetPrecisionFromIndex(uInd)));
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
