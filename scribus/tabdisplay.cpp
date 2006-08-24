/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <qtooltip.h>
#include <qgroupbox.h>
#include <qcolordialog.h>
#include <qpainter.h>

#include "tabdisplay.h"
#include "tabdisplay.moc"
#include "mspinbox.h"
#include "prefsmanager.h"
#include "units.h"


TabDisplay::TabDisplay(QWidget* parent, const char* name)
	: TabDisplayBase(parent, name, 0)
{
	QToolTip::add( backColor, "<qt>" + tr( "Color for paper" ) + "</qt>");
	QToolTip::add( checkUnprintable, "<qt>" + tr( "Mask the area outside the margins in the margin color" ) + "</qt>" );
	QToolTip::add( checkLink, "<qt>" + tr("Enable or disable  the display of linked frames.") + "</qt>");
	QToolTip::add( checkControl, "<qt>" + tr("Display non-printing characters such as paragraph markers in text frames") + "</qt>");
	QToolTip::add( checkFrame, "<qt>" + tr("Turns the display of frames on or off") + "</qt>");
	QToolTip::add( checkLayerM, "<qt>" + tr("Turns the display of layer indicators on or off") + "</qt>");
	QToolTip::add( checkPictures, "<qt>" + tr("Turns the display of pictures on or off") + "</qt>");
	QToolTip::add( leftScratch, "<qt>" + tr( "Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	QToolTip::add( rightScratch, "<qt>" + tr( "Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	QToolTip::add( topScratch, "<qt>" + tr( "Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	QToolTip::add( bottomScratch, "<qt>" + tr( "Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	QToolTip::add( CaliText, "<qt>" + tr( "Set the default zoom level" )  + "</qt>");
	QToolTip::add( CaliGroup, "<qt>" + tr( "Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size" ) + "</qt>" );

	connect(CaliSlider, SIGNAL(valueChanged(int)), this, SLOT(setDisScale()));
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
}

void TabDisplay::restoreDefaults(struct ApplicationPrefs *prefsData, struct guidesPrefs *guidesSettings)
{
	docUnitIndex = prefsData->docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);

	DisScale = prefsData->DisScale;
	int decimals = unitGetPrecisionFromIndex(docUnitIndex);
	QString unitSuffix = unitGetSuffixFromIndex(docUnitIndex);
	QPixmap pm5(54, 14);
	pm5.fill(prefsData->DpapColor);
	colorPaper = prefsData->DpapColor;
	backColor->setPixmap(pm5);
	backColor->setText( QString::null );
	checkUnprintable->setChecked( prefsData->marginColored );
	checkPictures->setChecked(guidesSettings->showPic);
	checkLink->setChecked(guidesSettings->linkShown);
	checkControl->setChecked(guidesSettings->showControls);
	checkFrame->setChecked(guidesSettings->framesShown);
	checkLayerM->setChecked(guidesSettings->layerMarkersShown);
	checkRuler->setChecked(guidesSettings->rulerMode);
	topScratch->setDecimals( decimals );
	topScratch->setMaxValue(1000);
	topScratch->setValue(prefsData->ScratchTop * unitRatio);
	leftScratch->setDecimals( decimals );
	leftScratch->setMaxValue(1000);
	leftScratch->setValue(prefsData->ScratchLeft * unitRatio);
	bottomScratch->setDecimals( decimals );
	bottomScratch->setMaxValue(1000);
	bottomScratch->setValue(prefsData->ScratchBottom * unitRatio);
	rightScratch->setDecimals( decimals );
	rightScratch->setMaxValue(1000);
	rightScratch->setValue(prefsData->ScratchRight * unitRatio);
	topScratch->setSuffix(unitSuffix);
	bottomScratch->setSuffix(unitSuffix);
	leftScratch->setSuffix(unitSuffix);
	rightScratch->setSuffix(unitSuffix);
	gapHorizontal->setSuffix( unitSuffix );
	gapHorizontal->setDecimals( decimals );
	gapHorizontal->setMaxValue(1000);
	gapHorizontal->setValue(prefsData->pageSets[prefsData->FacingPages].GapHorizontal * unitRatio);
	gapVertical->setSuffix( unitSuffix );
	gapVertical->setDecimals( decimals );
	gapVertical->setMaxValue(1000);
	gapVertical->setValue(prefsData->pageSets[prefsData->FacingPages].GapVertical * unitRatio);
	drawRuler();
	//CaliSlider->setValue(static_cast<int>(100 * DisScale)-100);
	setDisScale();
}

void TabDisplay::unitChange(QString unit, int docUnitIx, int decimals, double invUnitConversion)
{
	double oldMin, oldMax, val;
	int decimalsOld;
	docUnitIndex = docUnitIx;

	topScratch->setSuffix(unit);
	bottomScratch->setSuffix(unit);
	leftScratch->setSuffix(unit);
	rightScratch->setSuffix(unit);
	gapVertical->setSuffix( unit );
	gapHorizontal->setSuffix( unit );

	topScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	topScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	bottomScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	bottomScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	leftScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	leftScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	rightScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	rightScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	gapVertical->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	gapVertical->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	gapHorizontal->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	gapHorizontal->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	drawRuler();
}
void TabDisplay::changePaperColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorPaper, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorPaper = neu;
		backColor->setPixmap(pm);
	}
}

void TabDisplay::setDisScale()
{
	DisScale = QMAX((100.0 + CaliSlider->value()) / 100.0, 0.01);
	drawRuler();
	CaliAnz->setText(QString::number(DisScale*100, 'f', 2)+" %");
}

void TabDisplay::drawRuler()
{
	double xl, iter, iter2, maxi;
	switch (docUnitIndex)
	{
		case 0:
			iter = 10.0;
			iter2 = iter * 10.0;
			maxi = 200.0;
			break;
		case 1:
			iter = (10.0 / 25.4) * 72.0;
			iter2 = iter * 10.0;
			maxi = iter2;
			break;
		case 2:
			iter = 18.0;
			iter2 = 72.0;
			maxi = 2 * iter2;
			break;
		case 3:
			iter = 12.0;
			iter2 = 120.0;
			maxi = 240.0;
			break;
		case 4:
			iter = 12.0;
			iter2 = 120.0;
			maxi = 240.0;
			break;
		default:
			iter = 10.0;
			iter2 = iter * 10.0;
			maxi = 200.0;
			break;
	}

	QPixmap pm(static_cast<int>(maxi*DisScale+30), 21);
	pm.fill();
	QPainter p;
	p.begin(&pm);
	p.drawLine(0, 19, width(), 19);
	p.setBrush(black);
	p.setPen(black);
	p.scale(DisScale, 1.0);
	for (xl = 0; xl < maxi; xl += iter)
		p.drawLine(static_cast<int>(xl), 13, static_cast<int>(xl), 19);
	for (xl = 0; xl < maxi+10; xl += iter2)
	{
		p.drawLine(static_cast<int>(xl), 6, static_cast<int>(xl), 19);
		p.save();
		p.scale(1.0 / DisScale, 1.0);
		switch (docUnitIndex)
		{
			case 2:
				p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12,
						   QString::number(xl / iter2));
				break;
			case 3:
				p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12,
						   QString::number(xl / iter));
				break;
			default:
				p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12,
						   QString::number(xl / iter * 10));
				break;
		}
		p.restore();
	}
	p.end();
	CaliRuler->setPixmap(pm);
}

void TabDisplay::hideReform()
{
	CaliGroup->hide();
}
