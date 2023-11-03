/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : June 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   Scribus is free software; you can redistribute it and/or modify       *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef ALIGNDISTRIBUTEPALETTE_H
#define ALIGNDISTRIBUTEPALETTE_H

#include "ui_aligndistribute.h"

#include <QVariant>
#include <QPixmap>
#include <QDialog>
#include <QLabel>
#include <QList>

class QEvent;

#include "scribusapi.h"
#include "scribusdoc.h"
#include "ui/scdockpalette.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QSpacerItem;
class QToolButton;
class ScrSpinBox;
class ScribusMainWindow;
class ScribusView;
class UndoManager;

struct AlignObjs;


/*! \brief Align/Distribute palette.
*/
class SCRIBUS_API AlignDistributePalette : public ScDockPalette, Ui::AlignDistribute
{
	Q_OBJECT

public:
	AlignDistributePalette( QWidget* parent = nullptr, const char* name = 0);
	~AlignDistributePalette() = default;

	virtual void setDoc( ScribusDoc* newDoc );
	void unitChange();
	
public slots:
	void setGuide(int, qreal);
	void localeChange();

protected:
	ScribusView *currView { nullptr };

	void changeEvent(QEvent *e) override;
	
protected slots:
	virtual void iconSetChange();
	void languageChange();
	
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
	void distributeDistH(bool usingDistance = false);
	void distributeDistValH();
	void distributeDistAcrossPage();
	void distributeDistAcrossMargins();
	void distributeRight();
	void distributeBottom();
	void distributeCenterH();
	void distributeDistV(bool usingDistance = false);
	void distributeDistValV();
	void distributeDistDownPage();
	void distributeDistDownMargins();
	void distributeLeft();
	void distributeCenterV();
	void distributeTop();
	void swapLeft();
	void swapRight();
	
	void alignToChanged(int);
	void alignMethodChanged(int);

private:
	void init();
	void enableGuideButtons();

	UndoManager *undoManager { nullptr };
	ScribusDoc::AlignTo currAlignTo { ScribusDoc::alignFirst };
	ScribusDoc::AlignMethod currAlignMethod { ScribusDoc::alignByMoving };
	QList<AlignObjs> *alignObjects { nullptr };
	ScribusDoc *currDoc { nullptr };
	qreal unitRatio { 1.0 };
	int guideDirection { -1 };
	qreal guidePosition { 0.0 };
	QString guideInfoText;
	QString guideInfoTextNone;
	
signals:
	void documentChanged();

};

#endif // ALIGNDISTRIBUTEPALETTE_H
