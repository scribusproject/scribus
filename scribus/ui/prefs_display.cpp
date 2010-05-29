/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QApplication>
#include <QColor>
#include <QColorDialog>
#include <QDesktopWidget>
#include <QPainter>

#include "prefs_display.h"
#include "prefsstructs.h"
#include "scribusdoc.h"
#include "units.h"
#include "util_icon.h"

Prefs_Display::Prefs_Display(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	buttonRestoreDPI->setIcon(QIcon(loadIcon("screen.png")));

	connect(pageFillColorButton, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	connect(scratchSpaceColorButton, SIGNAL(clicked()), this, SLOT(changeScratchColor()));
	connect(frameSelectedColorButton, SIGNAL(clicked()), this, SLOT(changeFrameColor()));
	connect(frameColorButton, SIGNAL(clicked()), this, SLOT(changeNormFrameColor()));
	connect(frameGroupedColorButton, SIGNAL(clicked()), this, SLOT(changeGroupFrameColor()));
	connect(frameLinkedColorButton, SIGNAL(clicked()), this, SLOT(changeChainFrameColor()));
	connect(frameLockedColorButton, SIGNAL(clicked()), this, SLOT(changeLockFrameColor()));
	connect(frameAnnotationColorButton, SIGNAL(clicked()), this, SLOT(changeAnnotFrameColor()));
	connect(selectedPageBorderButton, SIGNAL(clicked()), this, SLOT(changePageBorderColor()));
	connect(textControlCharsButton, SIGNAL(clicked()), this, SLOT(changeControlCharsColor()));

	connect(buttonRestoreDPI, SIGNAL(clicked()), this, SLOT(restoreDisScale()));
	connect(adjustDisplaySlider, SIGNAL(valueChanged(int)), this, SLOT(setDisScale()));
	connect(rulerUnitComboBox, SIGNAL(activated(int)), this, SLOT(drawRuler()));
}

Prefs_Display::~Prefs_Display()
{
}

void Prefs_Display::languageChange()
{
	pageFillColorButton->setToolTip( "<qt>" + tr( "Color for paper (onscreen)" ) + "</qt>");
	showUnprintableAreaInMarginColorCheckBox->setToolTip( "<qt>" + tr( "Mask the area outside the margins in the margin color" ) + "</qt>" );
	showTextChainsCheckBox->setToolTip( "<qt>" + tr("Enable or disable  the display of linked frames.") + "</qt>");
	showControlCharsCheckBox->setToolTip( "<qt>" + tr("Display non-printing characters such as paragraph markers in text frames") + "</qt>");
	showFramesCheckBox->setToolTip( "<qt>" + tr("Turns the display of frames on or off") + "</qt>");
	showLayerIndicatorsCheckBox->setToolTip( "<qt>" + tr("Turns the display of layer indicators on or off") + "</qt>");
	showImagesCheckBox->setToolTip( "<qt>" + tr("Turns the display of images on or off") + "</qt>");
	showPageShadowCheckBox->setToolTip( "<qt>" + tr("Turns the page shadow on or off") + "</qt>");
	scratchSpaceLeftSpinBox->setToolTip( "<qt>" + tr( "Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	scratchSpaceRightSpinBox->setToolTip( "<qt>" + tr( "Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	scratchSpaceTopSpinBox->setToolTip( "<qt>" + tr( "Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	scratchSpaceBottomSpinBox->setToolTip( "<qt>" + tr( "Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	buttonRestoreDPI->setToolTip( "<qt>" + tr( "Set the default zoom level" )  + "</qt>");
	adjustDisplaySlider->setToolTip( "<qt>" + tr( "Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size" ) + "</qt>" );
}

void Prefs_Display::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	QString unitSuffix = unitGetSuffixFromIndex(docUnitIndex);

	showImagesCheckBox->setChecked(prefsData->guidesPrefs.showPic);
	showControlCharsCheckBox->setChecked(prefsData->guidesPrefs.showControls);
	showRulersCheckBox->setChecked(prefsData->guidesPrefs.rulersShown);
	showRulersRelativeToPageCheckBox->setChecked(prefsData->guidesPrefs.rulerMode);
	showTextChainsCheckBox->setChecked(prefsData->guidesPrefs.linkShown);
	showFramesCheckBox->setChecked(prefsData->guidesPrefs.framesShown);
	showLayerIndicatorsCheckBox->setChecked(prefsData->guidesPrefs.layerMarkersShown);
	showUnprintableAreaInMarginColorCheckBox->setChecked(prefsData->displayPrefs.marginColored);
	showBleedAreaCheckBox->setChecked(prefsData->guidesPrefs.showBleed);
	showPageShadowCheckBox->setChecked(prefsData->displayPrefs.showPageShadow);
	showVerifierWarningsOnCanvasCheckBox->setChecked(prefsData->displayPrefs.showVerifierWarningsOnCanvas);

	scratchSpaceLeftSpinBox->setMaximum(1000);
	scratchSpaceRightSpinBox->setMaximum(1000);
	scratchSpaceTopSpinBox->setMaximum(1000);
	scratchSpaceBottomSpinBox->setMaximum(1000);
	pageGapHorizontalSpinBox->setMaximum(1000);
	pageGapVerticalSpinBox->setMaximum(1000);
	scratchSpaceLeftSpinBox->setNewUnit(docUnitIndex);
	scratchSpaceRightSpinBox->setNewUnit(docUnitIndex);
	scratchSpaceTopSpinBox->setNewUnit(docUnitIndex);
	scratchSpaceBottomSpinBox->setNewUnit(docUnitIndex);
	pageGapHorizontalSpinBox->setNewUnit(docUnitIndex);
	pageGapVerticalSpinBox->setNewUnit(docUnitIndex);


	scratchSpaceLeftSpinBox->setValue(prefsData->displayPrefs.scratch.Left * unitRatio);
	scratchSpaceRightSpinBox->setValue(prefsData->displayPrefs.scratch.Right * unitRatio);
	scratchSpaceTopSpinBox->setValue(prefsData->displayPrefs.scratch.Top * unitRatio);
	scratchSpaceBottomSpinBox->setValue(prefsData->displayPrefs.scratch.Bottom * unitRatio);
	pageGapHorizontalSpinBox->setValue(prefsData->displayPrefs.pageGapHorizontal);
	pageGapVerticalSpinBox->setValue(prefsData->displayPrefs.pageGapVertical);

	QPixmap pm(100, 30);
	pm.fill(prefsData->displayPrefs.paperColor);
	colorPaper = prefsData->displayPrefs.paperColor;
	pageFillColorButton->setText( QString::null );
	pageFillColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.scratchColor);
	colorScratch = prefsData->displayPrefs.scratchColor;
	scratchSpaceColorButton->setText( QString::null );
	scratchSpaceColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.frameColor);
	colorFrame = prefsData->displayPrefs.frameColor;
	frameSelectedColorButton->setText( QString::null );
	frameSelectedColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.frameNormColor);
	colorFrameNorm = prefsData->displayPrefs.frameNormColor;
	frameColorButton->setText( QString::null );
	frameColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.frameGroupColor);
	colorFrameGroup = prefsData->displayPrefs.frameGroupColor;
	frameGroupedColorButton->setText( QString::null );
	frameGroupedColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.frameLinkColor);
	colorFrameLinked = prefsData->displayPrefs.frameLinkColor;
	frameLinkedColorButton->setText( QString::null );
	frameLinkedColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.frameLockColor);
	colorFrameLocked = prefsData->displayPrefs.frameLockColor;
	frameLockedColorButton->setText( QString::null );
	frameLockedColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.frameAnnotationColor);
	colorFrameAnnotation = prefsData->displayPrefs.frameAnnotationColor;
	frameAnnotationColorButton->setText( QString::null );
	frameAnnotationColorButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.pageBorderColor);
	colorPageBorder = prefsData->displayPrefs.pageBorderColor;
	selectedPageBorderButton->setText( QString::null );
	selectedPageBorderButton->setIcon(pm);

	pm.fill(prefsData->displayPrefs.controlCharColor);
	colorControlChars = prefsData->displayPrefs.controlCharColor;
	textControlCharsButton->setText( QString::null );
	textControlCharsButton->setIcon(pm);

	displayScale=prefsData->displayPrefs.displayScale;

	adjustDisplaySlider->setValue(qRound(100 * displayScale) - 150);
	displayDPI->setText(QString::number(qRound(displayScale*72.0))+ tr(" dpi"));
	rulerUnitComboBox->clear();
	rulerUnitComboBox->addItems(unitGetTextUnitList());
	rulerUnitComboBox->setCurrentIndex(docUnitIndex);
	drawRuler();

}

void Prefs_Display::drawRuler()
{
	int index = rulerUnitComboBox->currentIndex();
	double iter = unitRulerGetIter1FromIndex(index);
	double iter2 = unitRulerGetIter2FromIndex(index);
	double maxi;
	switch (index)
	{
		case 0:
			maxi = 200.0;
			break;
		case 1:
			maxi = iter2;
			break;
		case 2:
			maxi = 2 * iter2;
			break;
		case 3:
			maxi = 240.0;
			break;
		case 4:
			maxi = 240.0;
			break;
		default:
			iter = 10.0;
			iter2 = iter * 10.0;
			maxi = 200.0;
			break;
	}

	QPixmap pm(qMin(qMax(displaySizeRuler->width(), qRound(maxi)+30), qRound(maxi*displayScale+30)), 21);
	pm.fill();
	QPainter p;
	p.begin(&pm);
	p.drawLine(0, 19, pm.width(), 19);
	p.setBrush(Qt::black);
	p.setPen(Qt::black);
	p.scale(displayScale, 1.0);
	double xl;
	for (xl = 0; xl < maxi; xl += iter)
		p.drawLine(QPointF(xl, 13.0), QPointF(xl, 19.0));
	for (xl = 0; xl < maxi+10; xl += iter2)
	{
		p.drawLine(QPointF(xl, 6.0), QPointF(xl, 19.0));
		p.save();
		p.scale(1.0 / displayScale, 1.0);
		double val;
		switch (index)
		{
			case 2:
			case 4:
				val=xl/iter2;
				break;
			case 3:
			case 5:
				val=xl/iter;
				break;
			default:
				val=xl/iter*10;
				break;
		}
		p.drawText(static_cast<int>((xl+qRound(2/displayScale)) * displayScale), 12, QString::number(val));
		p.restore();
	}
	p.end();
	displaySizeRuler->setPixmap(pm);
}

void Prefs_Display::restoreDisScale()
{
	disconnect(adjustDisplaySlider, SIGNAL(valueChanged(int)), this, SLOT(setDisScale()));
	int dpi = qApp->desktop()->logicalDpiX();
	if ((dpi < 60) || (dpi > 250))
		dpi = 72;
	displayScale = dpi / 72.0;
	adjustDisplaySlider->setValue(qRound(100 * displayScale) - 150);
	drawRuler();
	displayDPI->setText(QString::number(qRound(displayScale*72.0))+ tr(" dpi"));
	connect(adjustDisplaySlider, SIGNAL(valueChanged(int)), this, SLOT(setDisScale()));
}

void Prefs_Display::setDisScale()
{
	displayScale = qMax((150.0 + adjustDisplaySlider->value()) / 100.0, 0.01);
	drawRuler();
	displayDPI->setText(QString::number(qRound(displayScale*72.0))+ tr(" dpi"));
}

void Prefs_Display::changePaperColor()
{
	QColor newColor(QColorDialog::getColor(colorPaper, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorPaper = newColor;
		pageFillColorButton->setIcon(pm);
	}
}

void Prefs_Display::changeScratchColor()
{
	QColor newColor(QColorDialog::getColor(colorScratch, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorScratch = newColor;
		scratchSpaceColorButton->setIcon(pm);
	}
}

void Prefs_Display::changeFrameColor()
{
	QColor newColor(QColorDialog::getColor(colorFrame, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorFrame = newColor;
		frameSelectedColorButton->setIcon(pm);
	}
}

void Prefs_Display::changeNormFrameColor()
{
	QColor newColor(QColorDialog::getColor(colorFrameNorm, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorFrameNorm = newColor;
		frameColorButton->setIcon(pm);
	}
}

void Prefs_Display::changeGroupFrameColor()
{
	QColor newColor(QColorDialog::getColor(colorFrameGroup, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorFrameGroup = newColor;
		frameGroupedColorButton->setIcon(pm);
	}
}

void Prefs_Display::changeChainFrameColor()
{
	QColor newColor(QColorDialog::getColor(colorFrameLinked, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorFrameLinked = newColor;
		frameLinkedColorButton->setIcon(pm);
	}
}

void Prefs_Display::changeLockFrameColor()
{
	QColor newColor(QColorDialog::getColor(colorFrameLocked, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorFrameLocked = newColor;
		frameLockedColorButton->setIcon(pm);
	}
}

void Prefs_Display::changeAnnotFrameColor()
{
	QColor newColor(QColorDialog::getColor(colorFrameAnnotation, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorFrameAnnotation = newColor;
		frameAnnotationColorButton->setIcon(pm);
	}
}

void Prefs_Display::changePageBorderColor()
{
	QColor newColor(QColorDialog::getColor(colorPageBorder, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorPageBorder = newColor;
		selectedPageBorderButton->setIcon(pm);
	}
}

void Prefs_Display::changeControlCharsColor()
{
	QColor newColor(QColorDialog::getColor(colorControlChars, this));
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		colorControlChars = newColor;
		textControlCharsButton->setIcon(pm);
	}
}

void Prefs_Display::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->guidesPrefs.showPic=showImagesCheckBox->isChecked();
	prefsData->guidesPrefs.showControls=showControlCharsCheckBox->isChecked();
	prefsData->guidesPrefs.rulersShown=showRulersCheckBox->isChecked();
	prefsData->guidesPrefs.rulerMode=showRulersRelativeToPageCheckBox->isChecked();
	prefsData->guidesPrefs.linkShown=showTextChainsCheckBox->isChecked();
	prefsData->guidesPrefs.framesShown=showFramesCheckBox->isChecked();
	prefsData->guidesPrefs.layerMarkersShown=showLayerIndicatorsCheckBox->isChecked();
	prefsData->displayPrefs.marginColored=showUnprintableAreaInMarginColorCheckBox->isChecked();
	prefsData->guidesPrefs.showBleed=showBleedAreaCheckBox->isChecked();
	prefsData->displayPrefs.showPageShadow=showPageShadowCheckBox->isChecked();
	prefsData->displayPrefs.showVerifierWarningsOnCanvas=showVerifierWarningsOnCanvasCheckBox->isChecked();
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	prefsData->displayPrefs.scratch.Left=scratchSpaceLeftSpinBox->value()/unitRatio;
	prefsData->displayPrefs.scratch.Right=scratchSpaceRightSpinBox->value()/unitRatio;
	prefsData->displayPrefs.scratch.Top=scratchSpaceTopSpinBox->value()/unitRatio;
	prefsData->displayPrefs.scratch.Bottom=scratchSpaceBottomSpinBox->value()/unitRatio;
	prefsData->displayPrefs.pageGapHorizontal=pageGapHorizontalSpinBox->value();
	prefsData->displayPrefs.pageGapVertical=pageGapVerticalSpinBox->value();

	prefsData->displayPrefs.paperColor=colorPaper;
	prefsData->displayPrefs.scratchColor=colorScratch;
	prefsData->displayPrefs.frameColor=colorFrame;
	prefsData->displayPrefs.frameNormColor=colorFrameNorm;
	prefsData->displayPrefs.frameGroupColor=colorFrameGroup;
	prefsData->displayPrefs.frameLinkColor=colorFrameLinked;
	prefsData->displayPrefs.frameLockColor=colorFrameLocked;
	prefsData->displayPrefs.frameAnnotationColor=colorFrameAnnotation;
	prefsData->displayPrefs.pageBorderColor=colorPageBorder;
	prefsData->displayPrefs.controlCharColor=colorControlChars;
	prefsData->displayPrefs.displayScale=displayScale;
}
