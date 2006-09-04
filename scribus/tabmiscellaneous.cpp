/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qcolordialog.h>
#include <qtooltip.h>
#include <qpushbutton.h>

#include "tabmiscellaneous.h"
#include "tabmiscellaneous.moc"
#include "scribusapp.h"
extern ScribusQApp* ScQApp;

TabMiscellaneous::TabMiscellaneous(QWidget* parent, const char* name)
	: TabMiscellaneousBase(parent, name, 0)
{
	QToolTip::add( buttonFrameSelected, "<qt>" + tr( "Selection color for frames" ) + "</qt>");
	connect(buttonFrameSelected, SIGNAL(clicked()), this, SLOT(changeFrameColor()));
	connect(buttonFrameNormal, SIGNAL(clicked()), this, SLOT(changeNormFrameColor()));
	connect(buttonFrameGrouped, SIGNAL(clicked()), this, SLOT(changeGroupFrameColor()));
	connect(buttonFrameLinked, SIGNAL(clicked()), this, SLOT(changeChainFrameColor()));
	connect(buttonFrameLocked, SIGNAL(clicked()), this, SLOT(changeLockFrameColor()));
	connect(buttonFrameAnnotation, SIGNAL(clicked()), this, SLOT(changeAnnotFrameColor()));
	connect(buttonSelectedPage, SIGNAL(clicked()), this, SLOT(changePageBorderColor()));
	connect(buttonControlChars, SIGNAL(clicked()), this, SLOT(changeControlCharsColor()));
}

void TabMiscellaneous::changeFrameColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorFrame, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorFrame = neu;
		buttonFrameSelected->setPixmap(pm);
	}
}

void TabMiscellaneous::changeNormFrameColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorFrameNorm, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorFrameNorm = neu;
		buttonFrameNormal->setPixmap(pm);
	}
}

void TabMiscellaneous::changeGroupFrameColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorFrameGroup, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorFrameGroup = neu;
		buttonFrameGrouped->setPixmap(pm);
	}
}

void TabMiscellaneous::changeChainFrameColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorFrameLinked, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorFrameLinked = neu;
		buttonFrameLinked->setPixmap(pm);
	}
}

void TabMiscellaneous::changeLockFrameColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorFrameLocked, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorFrameLocked = neu;
		buttonFrameLocked->setPixmap(pm);
	}
}

void TabMiscellaneous::changeAnnotFrameColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorFrameAnnotation, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorFrameAnnotation = neu;
		buttonFrameAnnotation->setPixmap(pm);
	}
}

void TabMiscellaneous::changePageBorderColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorPageBorder, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorPageBorder = neu;
		buttonSelectedPage->setPixmap(pm);
	}
}

void TabMiscellaneous::changeControlCharsColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorControlChars, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorControlChars = neu;
		buttonControlChars->setPixmap(pm);
	}
}

void TabMiscellaneous::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	AskForSubs->setChecked(prefsData->askBeforeSubstituite);
	stylePreview->setChecked(prefsData->haveStylePreview);
	startUpDialog->setChecked(prefsData->showStartupDialog);
	useStandardLI->setChecked(prefsData->useStandardLI);
	paragraphsLI->setValue(prefsData->paragraphsLI);
	showSplashCheckBox->setChecked( !ScQApp->neverSplashExists() );
	QPixmap pm(54, 14);
	pm.fill(prefsData->DFrameColor);
	colorFrame = prefsData->DFrameColor;
	buttonFrameSelected->setText( QString::null );
	buttonFrameSelected->setPixmap(pm);
	pm.fill(prefsData->DFrameNormColor);
	colorFrameNorm = prefsData->DFrameNormColor;
	buttonFrameNormal->setText( QString::null );
	buttonFrameNormal->setPixmap(pm);
	pm.fill(prefsData->DFrameGroupColor);
	colorFrameGroup = prefsData->DFrameGroupColor;
	buttonFrameGrouped->setText( QString::null );
	buttonFrameGrouped->setPixmap(pm);
	pm.fill(prefsData->DFrameLinkColor);
	colorFrameLinked = prefsData->DFrameLinkColor;
	buttonFrameLinked->setText( QString::null );
	buttonFrameLinked->setPixmap(pm);
	pm.fill(prefsData->DFrameLockColor);
	colorFrameLocked = prefsData->DFrameLockColor;
	buttonFrameLocked->setText( QString::null );
	buttonFrameLocked->setPixmap(pm);
	pm.fill(prefsData->DFrameAnnotationColor);
	colorFrameAnnotation = prefsData->DFrameAnnotationColor;
	buttonFrameAnnotation->setText( QString::null );
	buttonFrameAnnotation->setPixmap(pm);
	pm.fill(prefsData->DPageBorderColor);
	colorPageBorder = prefsData->DPageBorderColor;
	buttonSelectedPage->setText( QString::null );
	buttonSelectedPage->setPixmap(pm);
	pm.fill(prefsData->DControlCharColor);
	colorControlChars = prefsData->DControlCharColor;
	buttonControlChars->setText( QString::null );
	buttonControlChars->setPixmap(pm);
}
