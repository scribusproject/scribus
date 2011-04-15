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

#include "ui/createrange.h"
#include "customfdialog.h"
#include "scrspinbox.h"
#include "pageitem.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "units.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "scpaths.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QStringList>
#include <QTextEdit>
#include <QStackedWidget>

InsertAFrame::InsertAFrame(QWidget* parent, ScribusDoc *doc) :
	QDialog(parent),
	m_Doc(doc)
{
	setupUi(this);
	//Hide some unused items for now
// 	radioButtonTable->setShown(false);
// 	radioButtonShape->setShown(false);
// 	radioButtonPolygon->setShown(false);
	
	placementPagesRangeButton->setIcon(QIcon(loadIcon("ellipsis.png")));
	
	//set tab order
	QWidget::setTabOrder(radioButtonCustomPosition, xPosScrSpinBox);
	QWidget::setTabOrder(xPosScrSpinBox, yPosScrSpinBox);
	QWidget::setTabOrder(radioButtonCustomSize, widthScrSpinBox);
	QWidget::setTabOrder(widthScrSpinBox, heightScrSpinBox);
	QWidget::setTabOrder(textColumnCountSpinBox, textColumnGapScrSpinBox);
	
	typeButtonGroup = new QButtonGroup(typeGroupBox);
	typeButtonGroup->addButton(radioButtonTextFrame,0);
	typeButtonGroup->addButton(radioButtonImageFrame,1);
	pagePlacementButtonGroup = new QButtonGroup(pagePlacementGroupBox);
	pagePlacementButtonGroup->addButton(radioButtonCurrentPage,0);
	pagePlacementButtonGroup->addButton(radioButtonAllPages,1);
	pagePlacementButtonGroup->addButton(radioButtonRangeOfPages,2);
	framePositionButtonGroup = new QButtonGroup(framePositionGroupBox);
	framePositionButtonGroup->addButton(radioButtonTopLeftOfMargins,0);
	framePositionButtonGroup->addButton(radioButtonTopLeftOfPage,1);
	framePositionButtonGroup->addButton(radioButtonTopLeftOfBleed,2);
	framePositionButtonGroup->addButton(radioButtonCustomPosition,3);
	sizeButtonGroup = new QButtonGroup(sizeGroupBox);
	sizeButtonGroup->addButton(radioButtonPageMarginSize,0);
	sizeButtonGroup->addButton(radioButtonPageSize,1);
	sizeButtonGroup->addButton(radioButtonBleedSize,2);
	sizeButtonGroup->addButton(radioButtonImageSize,3);
	sizeButtonGroup->addButton(radioButtonCustomSize,4);
	radioButtonTextFrame->setChecked(true);
	radioButtonCurrentPage->setChecked(true);
	radioButtonTopLeftOfMargins->setChecked(true);
	radioButtonPageMarginSize->setChecked(true);
	slotSelectType(0);
	slotSelectPagePlacement(0);
	slotSelectPosition(0);
	slotSelectSize(0);
	
	int docUnitIndex = m_Doc ? m_Doc->unitIndex() : SC_PT;
	int decimals = unitGetPrecisionFromIndex(docUnitIndex);
	QString unitSuffix(unitGetSuffixFromIndex(docUnitIndex));
	
	xPosScrSpinBox->setNewUnit(docUnitIndex);
	yPosScrSpinBox->setNewUnit(docUnitIndex);
	widthScrSpinBox->setNewUnit(docUnitIndex);
	heightScrSpinBox->setNewUnit(docUnitIndex);
	xPosScrSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	yPosScrSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	widthScrSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	heightScrSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	textColumnGapScrSpinBox->setValues(0.0, 1000.0, decimals, 0.0);
	xPosScrSpinBox->setSuffix(unitSuffix);
	yPosScrSpinBox->setSuffix(unitSuffix);
	widthScrSpinBox->setSuffix(unitSuffix);
	heightScrSpinBox->setSuffix(unitSuffix);
	textColumnGapScrSpinBox->setSuffix(unitSuffix);

	sourceDocLineEdit->setText("");
	
	if (m_Doc!=0)
	{
		pageItemMap=m_Doc->getDocItemNames(PageItem::TextFrame);
		comboBoxLinkToExistingFrameName->addItems(pageItemMap.values());
	}
	if (comboBoxLinkToExistingFrameName->count()==0)
		checkBoxLinkToExistingFrame->setEnabled(false);
	
	connect(typeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotSelectType(int)));
	connect(pagePlacementButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotSelectPagePlacement(int)));
	connect(placementPagesRangeButton, SIGNAL(clicked()), this, SLOT(slotCreatePageNumberRange()));
	connect(framePositionButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotSelectPosition(int)));
	connect(sizeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotSelectSize(int)));
	connect(selectImageFileButton, SIGNAL(clicked()), this, SLOT(locateImageFile()));
	connect(selectDocFileButton, SIGNAL(clicked()), this, SLOT(locateDocFile()));
	connect(checkBoxLinkToExistingFrame, SIGNAL(stateChanged(int)), this, SLOT(slotLinkToExistingFrame(int)));
}

void InsertAFrame::slotSelectType( int id )
{
	checkBoxLinkCreatedTextFrames->setEnabled(id==0);
	radioButtonImageSize->setShown(id==1);
	switch (id)
	{
		case 0:
			typeTextEdit->setText( tr("<b>Insert a text frame</b><br/>A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable formats from plain text to OpenOffice.org.<br/>Your text may be edited and formatted on the page directly or in the Story Editor."));
			optionsStackedWidget->setCurrentIndex(0);
			break;
		case 1:
			typeTextEdit->setText( tr("<b>Insert an image frame</b><br/>An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, and posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette."));
			optionsStackedWidget->setCurrentIndex(1);
			break;
		case 2:
			typeTextEdit->setText("Insert a table");
			optionsStackedWidget->setCurrentIndex(2);
			break;
		case 3:
			typeTextEdit->setText("Insert a shape");
			optionsStackedWidget->setCurrentIndex(2);
			break;
		case 4:
			typeTextEdit->setText("Insert a polygon");
			optionsStackedWidget->setCurrentIndex(2);
			break;
		case 5:
			typeTextEdit->setText("Insert a line");
			optionsStackedWidget->setCurrentIndex(2);
			break;
		case 6:
			typeTextEdit->setText("Insert a bezier curve");
			optionsStackedWidget->setCurrentIndex(2);
			break;
	}
}

void InsertAFrame::slotSelectPagePlacement( int id )
{
	placementPagesLineEdit->setEnabled(id==2);
	placementPagesRangeButton->setEnabled(id==2);
	checkBoxLinkCreatedTextFrames->setEnabled(typeButtonGroup->checkedId()==0 && (id!=0));
}

void InsertAFrame::slotSelectPosition( int id )
{
	xPosScrSpinBox->setEnabled(id==3);
	yPosScrSpinBox->setEnabled(id==3);
}

void InsertAFrame::slotSelectSize( int id )
{
	widthScrSpinBox->setEnabled(id==4);
	heightScrSpinBox->setEnabled(id==4);
}

void InsertAFrame::getNewFrameProperties(InsertAFrameData &iafData)
{
	int type=typeButtonGroup->checkedId();
	iafData.source="";
	switch(type)
	{
		case 0:
			iafData.frameType=PageItem::TextFrame;
			iafData.source=QDir::fromNativeSeparators(sourceDocLineEdit->text());
			break;
		case 1:
			iafData.frameType=PageItem::ImageFrame;
			iafData.source=QDir::fromNativeSeparators(sourceImageLineEdit->text());
			break;
	}
	iafData.locationType=pagePlacementButtonGroup->checkedId();
	iafData.pageList=placementPagesLineEdit->text();
	iafData.positionType=framePositionButtonGroup->checkedId();
	iafData.sizeType=sizeButtonGroup->checkedId();
	int docUnitIndex = m_Doc->unitIndex();
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	iafData.x=xPosScrSpinBox->value() / unitRatio;
	iafData.y=yPosScrSpinBox->value() / unitRatio;
	iafData.width=widthScrSpinBox->value() / unitRatio;
	iafData.height=heightScrSpinBox->value() / unitRatio;
	iafData.impsetup=m_ImportSetup;
	iafData.columnCount=textColumnCountSpinBox->value();
	iafData.columnGap=textColumnGapScrSpinBox->value();
	iafData.linkTextFrames=checkBoxLinkCreatedTextFrames->isChecked();
	iafData.linkToExistingFrame=checkBoxLinkToExistingFrame->isChecked();
	iafData.linkToExistingFramePtr=NULL;
	if (comboBoxLinkToExistingFrameName->count()!=0)
	{
		QMapIterator<PageItem*, QString> i(pageItemMap);
		while (i.hasNext())
		{
			i.next();
			if (i.value()==comboBoxLinkToExistingFrameName->currentText())
				iafData.linkToExistingFramePtr=i.key();
		}
	}
}

void InsertAFrame::locateImageFile()
{
	QString formatD(FormatsManager::instance()->fileDialogFormatList(FormatsManager::IMAGESIMGFRAME));
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
	
	sourceImageLineEdit->setText(QDir::toNativeSeparators(fileName));
}

void InsertAFrame::locateDocFile()
{
	m_ImportSetup.runDialog=false;
	gtGetText* gt = new gtGetText(m_Doc);
	m_ImportSetup=gt->run();
	if (m_ImportSetup.runDialog)
		sourceDocLineEdit->setText(QDir::toNativeSeparators(m_ImportSetup.filename));
	delete gt;
}

void InsertAFrame::slotCreatePageNumberRange( )
{
	if (m_Doc!=0)
	{
		CreateRange cr(placementPagesLineEdit->text(), m_Doc->DocPages.count(), this);
		if (cr.exec())
		{
			CreateRangeData crData;
			cr.getCreateRangeData(crData);
			placementPagesLineEdit->setText(crData.pageRange);
			return;
		}
	}
	placementPagesLineEdit->setText(QString::null);
}

void InsertAFrame::slotLinkToExistingFrame(int state)
{
	comboBoxLinkToExistingFrameName->setEnabled(state==Qt::Checked);
	sourceDocLineEdit->setEnabled(state==Qt::Unchecked);
	selectDocFileButton->setEnabled(state==Qt::Unchecked);
}
