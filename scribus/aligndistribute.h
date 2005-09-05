/***************************************************************************
	begin                : June 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScApp program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef ALIGNDISTRIBUTEPALETTE_H
#define ALIGNDISTRIBUTEPALETTE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

#include "scribusapi.h"
#include "scribusstructs.h"
#include "scribusview.h"
#include "scrpalettebase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLabel;
class QLineEdit;
class QComboBox;
class QToolButton;
class MSpinBox;

class ScribusApp;
class ScribusDoc;
class UndoManager;

class SCRIBUS_API AlignDistributePalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	typedef enum {First, Last, Page, Margins, Guide, Selection } AlignTo;

	AlignDistributePalette( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~AlignDistributePalette();

	virtual void setView( ScribusView * newView );
	void unitChange();
	
public slots:
	void setGuide(int, double);

protected:
	ScribusView *currView;
	
	
	QGroupBox* alignGroupBox;
	QLabel* alignRelativeToLabel;
	QComboBox* alignRelativeToCombo;
	QToolButton* alignLeftOutToolButton;
	QToolButton* alignRightOutToolButton;
	QToolButton* alignBottomInToolButton;
	QToolButton* alignRightInToolButton;
	QToolButton* alignBottomOutToolButton;
	QToolButton* alignCenterHorToolButton;
	QToolButton* alignLeftInToolButton;
	QToolButton* alignCenterVerToolButton;
	QToolButton* alignTopOutToolButton;
	QToolButton* alignTopInToolButton;
	QLabel *alignGuideLabel;
	QLineEdit* alignGuideLineEdit;
	QGroupBox* distributeGroupBox;
	QToolButton* distributeDistHToolButton;
	QToolButton* distributeDistValueHToolButton;
	QToolButton* distributeRightToolButton;
	QToolButton* distributeBottomToolButton;
	QToolButton* distributeCenterHToolButton;
	QToolButton* distributeDistVToolButton;
	QToolButton* distributeDistValueVToolButton;
	QToolButton* distributeLeftToolButton;
	QToolButton* distributeCenterVToolButton;
	QToolButton* distributeTopToolButton;
	QLabel* distributeDistLabel;
	MSpinBox* distributeDistMSpinBox;

	QVBoxLayout* AlignDistributePaletteLayout;
	QGridLayout* alignGroupBoxLayout;
	QHBoxLayout* alignGuideLayout;
	QSpacerItem* alignGuideLeftSpacer;
	QSpacerItem* alignGuideRightSpacer;

	QHBoxLayout* layout11;
	QHBoxLayout* layout14;
	QSpacerItem* spacer15;
	QSpacerItem* spacer16;
	QGridLayout* layout2;
	QGridLayout* distributeGroupBoxLayout;
	QHBoxLayout* layout4;
	QSpacerItem* distributeLeftSpacer;
	QSpacerItem* distributeRightSpacer;
	QGridLayout* layout1;
	QHBoxLayout* distanceLayout;
	QSpacerItem* distributeDistLeftSpacer;
	QSpacerItem* distributeDistRightSpacer;

protected slots:
	virtual void languageChange();
	
	void alignLeftOut();
	void alignRightOut();
	void alignBottomIn();
	void alignRightIn();
	void alignBottomOut();
	void alignCenterHor();
	void alignLeftIn();
	void alignCenterVer();
	void alignTopOut();
	void alignTopIn();
	void distributeDistH();
	void distributeDistValH();
	void distributeRight();
	void distributeBottom();
	void distributeCenterH();
	void distributeDistV();
	void distributeDistValV();
	void distributeLeft();
	void distributeCenterV();
	void distributeTop();
	
	void alignToChanged(int);

private:
	void init();
	bool startAlign();
	void endAlign();
	UndoManager *undoManager;
	ScribusApp* ScApp;
	AlignTo currAlignTo;
	QValueList<AlignObjs> *alignObjects;
	uint alignObjectsCount;
	ScribusDoc *currDoc;
	double unitRatio;
	bool usingDistance;
	int guideDirection;
	double guidePosition;
	QString guideInfoText, guideInfoTextNone;
	void enableGuideButtons();
	double widthForPageLocation();
	
signals:
	void documentChanged();

};

#endif // ALIGNDISTRIBUTEPALETTE_H
