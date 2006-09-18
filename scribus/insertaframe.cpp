/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
//
//
// Author: Craig Bradney <cbradney@zip.com.au>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "insertaframe.h"
#include "insertaframe.moc"

#include "customfdialog.h"
#include "mspinbox.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "util.h"

#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtextedit.h>
#include <qwidgetstack.h>

InsertAFrame::InsertAFrame(QWidget* parent, ScribusDoc *doc) :
	InsertAFrameBase(parent),
	m_Doc(doc)
{
	//Hide some unused items for now
	radioButtonTable->setShown(false);
	radioButtonShape->setShown(false);
	radioButtonPolygon->setShown(false);
	
	//set tab order
	QWidget::setTabOrder(radioButtonCustomPosition, xPosMSpinBox);
	QWidget::setTabOrder(xPosMSpinBox, yPosMSpinBox);
	QWidget::setTabOrder(radioButtonCustomSize, widthMSpinBox);
	QWidget::setTabOrder(widthMSpinBox, heightMSpinBox);
	QWidget::setTabOrder(textColumnCountSpinBox, textColumnGapMSpinBox);
	
	typeButtonGroup->setButton(0);
	pagePlacementButtonGroup->setButton(0);
	framePositionButtonGroup->setButton(0);
	sizeButtonGroup->setButton(0);
	slotSelectType(0);
	slotSelectPagePlacement(0);
	slotSelectPosition(0);
	slotSelectSize(0);
	
	int docUnitIndex = m_Doc->unitIndex();
// 	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);
	QString unitSuffix(unitGetSuffixFromIndex(docUnitIndex));
	xPosMSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	yPosMSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	widthMSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	heightMSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	textColumnGapMSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	xPosMSpinBox->setSuffix(unitSuffix);
	yPosMSpinBox->setSuffix(unitSuffix);
	widthMSpinBox->setSuffix(unitSuffix);
	heightMSpinBox->setSuffix(unitSuffix);
	textColumnGapMSpinBox->setSuffix(unitSuffix);

	sourceDocLineEdit->setText("");
 	connect(typeButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectType(int)));
 	connect(pagePlacementButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectPagePlacement(int)));
 	connect(framePositionButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectPosition(int)));
 	connect(sizeButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectSize(int)));
 	connect(selectImageFileButton, SIGNAL(clicked()), this, SLOT(locateImageFile()));
 	connect(selectDocFileButton, SIGNAL(clicked()), this, SLOT(locateDocFile()));
}

void InsertAFrame::slotSelectType( int id )
{
	checkBoxLinkCreatedTextFrames->setEnabled(id==0);
	switch (id)
	{
		case 0:
			typeTextEdit->setText("<b>Insert a text frame</b><br/>A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable format from plain text to OpenOffice.org.<br/>Your text may be edited and formatted on the page directly or in the simple Story Editor.");
			optionsWidgetStack->raiseWidget(2);
			break;
		case 1:
			typeTextEdit->setText("<b>Insert an image frame</b><br/>An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette.");
			optionsWidgetStack->raiseWidget(0);
			break;
		case 2:
			typeTextEdit->setText("Insert a table");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 3:
			typeTextEdit->setText("Insert a shape");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 4:
			typeTextEdit->setText("Insert a polygon");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 5:
			typeTextEdit->setText("Insert a line");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 6:
			typeTextEdit->setText("Insert a bezier curve");
			optionsWidgetStack->raiseWidget(1);
			break;
	}
}

void InsertAFrame::slotSelectPagePlacement( int id )
{
	placementPagesLineEdit->setEnabled(id==1);
	checkBoxLinkCreatedTextFrames->setEnabled(id==1);
}

void InsertAFrame::slotSelectPosition( int id )
{
	xPosMSpinBox->setEnabled(id==2);
	yPosMSpinBox->setEnabled(id==2);
}

void InsertAFrame::slotSelectSize( int id )
{
	widthMSpinBox->setEnabled(id==2);
	heightMSpinBox->setEnabled(id==2);
}

void InsertAFrame::getNewFrameProperties(insertAFrameData &iafData)
{
	int type=typeButtonGroup->selectedId();
	iafData.source="";
	switch(type)
	{
		case 0:
			iafData.frameType=PageItem::TextFrame;
			iafData.source=sourceDocLineEdit->text();
			break;
		case 1:
			iafData.frameType=PageItem::ImageFrame;
			iafData.source=sourceImageLineEdit->text();
			break;
		case 2:
// 			iafData.frameType=PageItem::
			break;
		case 3:
			iafData.frameType=PageItem::Polygon;
			break;
		case 4:
			iafData.frameType=PageItem::Polygon;
			break;
	}
	iafData.locationType=pagePlacementButtonGroup->selectedId();
	iafData.pageList=placementPagesLineEdit->text();
	iafData.positionType=framePositionButtonGroup->selectedId();
	iafData.sizeType=sizeButtonGroup->selectedId();
	iafData.x=xPosMSpinBox->value();
	iafData.y=yPosMSpinBox->value();
	iafData.width=widthMSpinBox->value();
	iafData.height=heightMSpinBox->value();
	iafData.impsetup=m_ImportSetup;
	iafData.columnCount=textColumnCountSpinBox->value();
	iafData.columnGap=textColumnGapMSpinBox->value();
	iafData.linkTextFrames=checkBoxLinkCreatedTextFrames->isChecked();
}

void InsertAFrame::locateImageFile()
{
	QString formatD(setupImageFormats());
	QString docDir = ".";
	PrefsManager* prefsManager=PrefsManager::instance();
	QString prefsDocDir(prefsManager->documentDir());
	if (!prefsDocDir.isEmpty())
		docDir = prefsManager->prefsFile->getContext("dirs")->get("images", prefsDocDir);
	else
		docDir = prefsManager->prefsFile->getContext("dirs")->get("images", ".");
		
	QString fileName("");
	CustomFDialog dia(this, docDir, tr("Open"), formatD, fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	
	sourceImageLineEdit->setText(fileName);
}

void InsertAFrame::locateDocFile()
{
	m_ImportSetup.runDialog=false;
	gtGetText* gt = new gtGetText(m_Doc);
	m_ImportSetup=gt->run();
	if (m_ImportSetup.runDialog)
		sourceDocLineEdit->setText(m_ImportSetup.filename);
	delete gt;
}
