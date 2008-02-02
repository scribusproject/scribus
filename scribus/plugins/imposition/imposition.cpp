/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jun 2007
    copyright            : (C) 2007 by Mateusz Haligowski
    email                : halish@kofeina.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <iostream>
#include <sstream>
#include <QDebug>

#include "imposition.h"
#include "scribus.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "units.h"
#include "commonstrings.h"
#include "aligndistribute.h"
#include "guidemanagercore.h"
#include "undomanager.h"
#include "selection.h"
#include "scribusXml.h"
#include "desaxe/saxXML.h"
#include "serializer.h"
#include <QDebug>

Imposition::Imposition(QWidget* parent, ScribusDoc* doc) : QDialog(parent)
{
	UndoManager::instance()->setUndoEnabled(false);
	setupUi(this);
	setModal(true);

	languageChange();
	
	this->parent = parent;
	this->srcDoc = doc;
	
	//get units
	unitIndex = doc->unitIndex();
	unitRatio = unitGetRatioFromIndex(unitIndex);
	unitSuffix = unitGetSuffixFromIndex(unitIndex);
		
	//set units in page settings
	pageWidthBox->setSuffix(unitSuffix);
	pageHeightBox->setSuffix(unitSuffix);

	//fill Page Size combobox
	PageSize ps( tr(doc->m_pageSize.toLocal8Bit().constData()));
	pageSizeComboBox->addItems(ps.sizeTRList());
	pageSizeComboBox->addItem( CommonStrings::trCustomPageSize);
	
	QStringList pageSizes=ps.sizeList();
	int sizeIndex=pageSizes.indexOf(ps.nameTR());
	if (sizeIndex!=-1)
		pageSizeComboBox->setCurrentIndex(sizeIndex);
	else
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);

	pageOrientationComboBox->addItem( tr( "Portrait" ) );
	pageOrientationComboBox->addItem( tr( "Landscape" ) );

	//put page dimensions to boxes
	pageWidthBox->setValue(ps.width() * unitRatio);
	pageHeightBox->setValue(ps.height() * unitRatio);
	
	//fill grid pages boxes with pagelists
	for (int i = 1; i <= (int)(doc->Pages->count()); i++)
	{
		cbFront->addItem(QString::number(i));
		cbBack->addItem(QString::number(i));
	}
	
	//fill folds pages boxes with pagelists
	foldFrontPage->addItem(QString::number(1));
	foldBackPage->addItem(QString::number(1));
	
	for (int i = 1; i < (int)(srcDoc->Pages->count()); i++)
	{
		if (srcDoc->locationOfPage(i) == LeftPage)
		{
			foldFrontPage->addItem(QString::number(i+1));
			foldBackPage->addItem(QString::number(i+1));
		}
	}

	isEdited = true;
	isOK = verifyPage();
	btnGo->setEnabled(isOK);
	btnPreview->setEnabled(isOK);
	
	//connect slots and signals
	connect(pageSizeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPageSize(const QString &)));
		
	connect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	connect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	connect(chb2Sides, SIGNAL(stateChanged(int)), this, SLOT(change2SidesBox(int)));
	connect(foldIsBackSide, SIGNAL(stateChanged(int)), this, SLOT(changeFoldBackPage(int)));
	connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
	
	//document changing slots
	connect(impositionTabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));
	connect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(verifyPage()));
	connect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(verifyPage()));

	connect(cbFront, SIGNAL(currentIndexChanged(QString)), this, SLOT(verifyPage()));
	connect(cbBack, SIGNAL(currentIndexChanged(QString)), this, SLOT(verifyPage()));
	
	connect(cbLayout, SIGNAL(currentIndexChanged(QString)), this, SLOT(verifyPage()));
}


Imposition::~Imposition()
{
//	UndoManager::instance()->switchStack(targetDoc->DocName);

	UndoManager::instance()->setUndoEnabled(true);
}


void Imposition::languageChange()
{
	
}

void Imposition::setPageSize(const QString &size)
{
	disconnect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	disconnect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	
	PageSize* ps = new PageSize( tr(size.toLocal8Bit().constData()));
	
	pageWidthBox->setValue(ps->width() * unitRatio);
	pageHeightBox->setValue(ps->height() * unitRatio);
	
	delete ps;

	isEdited = true;
	isOK = verifyPage();
	btnGo->setEnabled(isOK);
	btnPreview->setEnabled(isOK);
	
	connect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	connect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
}

void Imposition::changeTab(int dest)
{
	isEdited = true;
	isOK = verifyPage();
	btnGo->setEnabled(isOK);
	btnPreview->setEnabled(isOK);
}

void Imposition::changeDocGrid()
{
	//get first copied page
	Page* src = srcDoc->Pages->at(cbFront->currentIndex());
	
	double realSrcWidth = src->width() + srcDoc->bleeds.Left + srcDoc->bleeds.Right;
	double realSrcHeight = src->height() + srcDoc->bleeds.Top + srcDoc->bleeds.Bottom;
	
	//check whether width fits
	if (isOK == true)
	{
		//count how many rows and cols will be needed
		int cols = (int)((targetDoc->pageWidth)/realSrcWidth); // how many columns do we have on page?
		int rows = (int)((targetDoc->pageHeight)/realSrcHeight); // how many rows do we have on page?
		
		//now count how many pages are needed and create them
		int countPages=0;
		countPages = (int)(boxCopies->value() / (cols * rows)) + 1;
		if ((boxCopies->value() % (cols * rows) ) == 0) countPages--;
		if (chb2Sides->checkState() == Qt::Checked) countPages = countPages * 2; //double pages!
		targetDoc->createNewDocPages(countPages);
		
		targetDoc->changeLayerName(0,srcDoc->layerName(0));
		for (int i = 1; i < srcDoc->layerCount(); i++)
		{
			targetDoc->addLayer(srcDoc->layerName(i));
		}
		
		// make guides
		for (int i = 0; i < countPages; i++)
		{
			Page* p = targetDoc->Pages->at(i);
			
			//vertical guides
			double vertDist = (p->width() - p->Margins.Left - p->Margins.Right - cols*src->width())/cols;
			
			p->guides.addVertical(p->Margins.Left + 0.5*vertDist, p->guides.Standard);
			p->guides.addVertical(p->width() - p->Margins.Right - 0.5*vertDist, p->guides.Standard);
			
			double left = p->Margins.Left + 0.5*vertDist;
			double right = p->width() - p->Margins.Right - 0.5*vertDist;
			
			for (int j = 0; j < cols; j++)
			{
				left = left + src->width() + vertDist;
				p->guides.addVertical(left, p->guides.Standard);
				
				right = right - src->width() - vertDist;
				p->guides.addVertical(right, p->guides.Standard);
			}
			
			//horizontal guides
			double horizDist = (p->height() - p->Margins.Top - p->Margins.Bottom - rows*src->height())/rows;
			
			p->guides.addHorizontal(p->Margins.Top + 0.5*horizDist, p->guides.Standard);
			p->guides.addHorizontal(p->height() - p->Margins.Bottom - 0.5*horizDist, p->guides.Standard);
			
			double top = p->Margins.Top + 0.5*horizDist;
			double bottom = p->height() - p->Margins.Bottom - 0.5*horizDist;
			
			for (int j = 0; j < rows; j++)
			{
				top = top + src->height() + horizDist;
				p->guides.addHorizontal(top, p->guides.Standard);
				
				bottom = bottom - src->height() - horizDist;
				p->guides.addHorizontal(bottom, p->guides.Standard);
			}
		}
		
		
		//copy the first page to the clipboard
		ScribusMainWindow* scMW = ScCore->primaryMainWindow();
		scMW->view->requestMode(modeNormal);
		Selection* s = new Selection(scMW);
		
		//select items to copy
		for (int i = 0; i < srcDoc->Items->count(); i++)
		{
			if (srcDoc->OnPage(srcDoc->Items->at(i)) == src->pageNr())
				s->addItem(srcDoc->Items->at(i),false);
		}
		
		//Now, as all the relevant items have been copied, move the selection to the clipboard
		
		// new version:
		std::ostringstream xmlString;
		SaxXML xmlStream(xmlString);
		Serializer::serializeObjects(*s, xmlStream);
		std::string xml(xmlString.str());
		QByteArray ba(QByteArray(xml.c_str(), xml.length()));
		
		int currow = 0;
		int curcol = 0;
		targetDoc->setCurrentPage(targetDoc->Pages->at(0));
		Page* cur = targetDoc->currentPage();
		
		//now, start placing
		for  (int j = 0; j < boxCopies->value(); j++)
		{
			
			Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
			targetDoc->moveGroup(
					cur->guides.vertical(curcol*2, cur->guides.Standard),
					cur->guides.horizontal(currow*2, cur->guides.Standard),
					true,
					&pastedObjects
				       );
					
			if ((curcol + 1) == cols)
			{
				curcol = 0;
				if ((currow + 1) == rows && (j+1 < boxCopies->value()))
				{
					currow = 0;
					if (chb2Sides->checkState() == Qt::Checked)
					{
						targetDoc->setCurrentPage(targetDoc->Pages->at(cur->pageNr()+2));
					}
					else
					{
						targetDoc->setCurrentPage(targetDoc->Pages->at(cur->pageNr()+1));
					}
					cur = targetDoc->currentPage();
				}
				else
				{
					currow++;
				}
			}
			else
			{
				curcol++;
			}
		}
		delete s;
						
		if (chb2Sides->checkState() == Qt::Checked)
		{
			
			s = new Selection(scMW);
			targetDoc->setCurrentPage(targetDoc->Pages->at(1));
			cur = targetDoc->currentPage();
			src = srcDoc->Pages->at(cbBack->currentIndex());
		
		//select items to copy
			for (int i = 0; i < srcDoc->Items->count(); i++)
			{
				if (srcDoc->OnPage(srcDoc->Items->at(i)) == src->pageNr())
					s->addItem(srcDoc->Items->at(i),false);
			}
		
		//Now, as all the relevant items have been copied, move the selection to the clipboard
		
		// new version:
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*s, xmlStream);
			std::string xml(xmlString.str());
			QByteArray ba(QByteArray(xml.c_str(), xml.length()));
			//start copying from the second page
			currow = 0;
			curcol = cols-1;
					
			for ( int j = 0; j < boxCopies->value(); j++ )
			{
				Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
				targetDoc->moveGroup(
						cur->guides.vertical(curcol*2, cur->guides.Standard),
						cur->guides.horizontal(currow*2, cur->guides.Standard),
						true,
						&pastedObjects
					       );
			
				if ( curcol == 0 )
				{
					curcol = cols - 1;
					if ( ( currow + 1 ) == rows && ( j+1 < boxCopies->value() ) )
					{
						currow = 0;
						targetDoc->setCurrentPage ( targetDoc->Pages->at ( cur->pageNr() +2 ) );
						cur = targetDoc->currentPage();
					}
					else
					{
						currow++;
					}
				}
				else
				{
					curcol--;
				}
			}
			delete s;
		}	
		
		view->Deselect();
		srcDoc->view()->Deselect();
		view->DrawNew();
		srcDoc->view()->DrawNew();
	}
}

QList<int>* Imposition::parsePages(QString pageslist)
{
	QList<int>* li = new QList<int>();
	
	QStringList spages = pageslist.split(",", QString::SkipEmptyParts);
	for (int i = 0; i < spages.count(); i++) 
	{
		if (spages.at(i).split("-", QString::SkipEmptyParts).count() == 1)
		{
			li->append(spages.at(i).toInt());
		}
		if (spages.at(i).split("-", QString::SkipEmptyParts).count() == 2)
		{
			QStringList range = spages.at(i).split("-", QString::SkipEmptyParts);
			for (int j = range.at(0).toInt(); j <= range.at(1).toInt(); j++)
				li->append(j);
			
		}
	}
	
	return li;
}

void Imposition::changeDocBooklet()
{
	if (isOK == true)
	{
		switch (cbLayout->currentText().toInt())
		{
			case 4:	this->booklet4p(parsePages(inpPages->text())); break;
			case 8: this->booklet8p(parsePages(inpPages->text())); break;
			case 16: this->booklet16p(parsePages(inpPages->text())); break;
		}
	}
}

void Imposition::booklet4p(QList<int>* pages)
{
	/*
	
	4 page imposition looks like this:
	       front:                     back:
	 --------------------       --------------------
	|         |          |     |         |          |
	|         |          |     |         |          |
	|         |          |     |         |          |
	|    4    |    1     |     |    2    |    3     |
	|         |          |     |         |          |
	|         |          |     |         |          |
	|         |          |     |         |          |
         --------------------       --------------------
	
	*/
	
	//fill the pages, so that it could be divided by for
	while ( (pages->count() % 4) != 0)
	{
		pages->append(0);
	}
	
	//create pages
	int targetSheets = (int)ceil(pages->count() / 4.0); //how many sheets do we have
	int targetPages = targetSheets * 2; //how many pages do we have
	
	targetDoc->createNewDocPages(targetPages);
	
	targetDoc->changeLayerName(0,srcDoc->layerName(0));
	for (int i = 1; i < srcDoc->layerCount(); i++)
	{
		targetDoc->addLayer(srcDoc->layerName(i));
	}
	
	//make guides
	for (int i = 0; i < targetDoc->Pages->count(); i++)
	{
		Page* p = targetDoc->Pages->at(i);
		
		//count the left guide:
		double guide_x = (p->width() - 2 * srcDoc->pageWidth)/2;
		p->guides.addVertical(guide_x, p->guides.Standard);
		
		//middle guide:
		guide_x += srcDoc->pageWidth;
		p->guides.addVertical(guide_x, p->guides.Standard);
		
		//and the right one:
		guide_x += srcDoc->pageWidth;
		p->guides.addVertical(guide_x, p->guides.Standard);
		
		//now, the top guide:
		double guide_y = (p->height() - srcDoc->pageHeight)/2;
		p->guides.addHorizontal(guide_y, p->guides.Standard);
		
		//and the bottom one:
		guide_y += srcDoc->pageHeight;
		p->guides.addHorizontal(guide_y, p->guides.Standard);
	}
	
	//start copying
	ScribusMainWindow* scMW = ScCore->primaryMainWindow();
	scMW->slotSelect();
	Selection* s = new Selection(scMW);
	
	//first, do the frontsides
	for (int i = 0; i < targetDoc->Pages->count(); i = i + 2)
	{
		targetDoc->setCurrentPage(targetDoc->Pages->at(i));
		
		//copy the page to the clipboard
		//right side
		//make selections
		for (int j = 0; j < srcDoc->Items->count(); j++)
		{
			if (srcDoc->OnPage(srcDoc->Items->at(j)) == (pages->at(i)-1))
			{
				s->addItem(srcDoc->Items->at(j),false);
			}
			
		}
		
		if (s->count() > 0)
		{
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*s, xmlStream);
			std::string xml(xmlString.str());
			QByteArray ba(QByteArray(xml.c_str(), xml.length()));
			
			//paste page from clipboard
			
			Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
			
			targetDoc->moveGroup(
					targetDoc->Pages->at(i)->guides.vertical(1, targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(i)->guides.horizontal(0, targetDoc->Pages->at(i)->guides.Standard),
					false,
					&pastedObjects
				);
			
			
			s->clear();
			
			for (int j = 0; j < targetDoc->layerCount(); j++)
			{
				//create group out of all the items in the layer
				Selection* gs = new Selection(scMW);
				
				for (int k = 0; k < pastedObjects.count(); k++)
				{
					if (pastedObjects.itemAt(k)->LayerNr == j) gs->addItem(pastedObjects.itemAt(k));
				}
				
				if (gs->count() > 0)
				{
					//create group
					targetDoc->itemSelection_GroupObjects(false, false, gs);
					
					//get group control
					signed int groupid = gs->itemAt(0)->Groups.at(0);
					
					PageItem* groupcontrol = NULL;
					for (int k = 0; k < targetDoc->Items->count(); k++)
					{
						if (
							targetDoc->Items->at(k)->Groups.count() > 0 &&
							targetDoc->Items->at(k)->Groups.at(0) == groupid &&
							targetDoc->Items->at(k)->isGroupControl
						   )
						{
							groupcontrol = targetDoc->Items->at(k);
							break;
						}
					}
					groupcontrol->SetRectFrame();
					
					double points[32] = {
					//left upper corner - left lower corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					
					//left lower corner - right lower corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
							
					100,
     					100,
	  				100,
       					100,

					//right lower corner - right upper corner
					100,
					100,
					100,
					100,
							
					0,
     					100,
	  				0,
       					100,
	    
					//right upper corner - left upper corner
					0,
					100,
					0,
					100,
					
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					};
					
					double groupX = groupcontrol->xPos();
					double groupY = groupcontrol->yPos() - targetDoc->Pages->at(i)->yOffset();
					
					double groupWidth = groupcontrol->width();
					double groupHeight = groupcontrol->height();
					
					qDebug() << groupX << groupY << groupWidth << groupHeight;
					
					for (int k = 0; k < 12; k += 2)
					{
						if  (points[k] < groupX)
						{
							points[k] = 0;
						}
						else if (points[k] - groupX > 100)
						{
							points[k] = 100;
						}
						else
						{
							points[k] = 100* (points[k] - groupX) / groupWidth;
						}
						
						if (points[k+1] < groupY)
						{
							points[k+1] = 0;
						}
						else if (points[k+1] - groupY > 100)
						{
							points[k+1] = 100;
						}
						else
						{
							points[k+1] = 100* (points[k+1] - groupY) / groupHeight;
						}
						
						qDebug() << "IMPOSITION: points[" << k << "] " << points[i] << "\n";
						qDebug() << "IMPOSITION: points[" << k+1 << "] " << points[i+1] << "\n";
					}
					
					for (int k = 28; k < 32; k += 2)
					{
						if  (points[k] < groupX)
						{
							points[k] = 0;
						}
						else if (points[k] - groupX > 100)
						{
							points[k] = 100;
						}
						else
						{
							points[k] = 100* (points[k] - groupX) / groupWidth;
						}
						
						if (points[k+1] < groupY)
						{
							points[k+1] = 0;
						}
						else if (points[k+1] - groupY > 100)
						{
							points[k+1] = 100;
						}
						else
						{
							points[k+1] = 100* (points[k+1] - groupY) / groupHeight;
						}
						
						qDebug() << "IMPOSITION: points[" << k << "] " << points[i] << "\n";
						qDebug() << "IMPOSITION: points[" << k+1 << "] " << points[i+1] << "\n";
					}

					groupcontrol->SetFrameShape(32,points);					
				}
			}
		
		}
		
		//left side
		for (int j = 0; j < srcDoc->Items->count(); j++)
		{
			if (srcDoc->OnPage(srcDoc->Items->at(j)) == (pages->at(pages->count()-i-1)-1))
			{
				s->addItem(srcDoc->Items->at(j),false);
			}
		
		}
		
		if (s->count() > 0)
		{
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*s, xmlStream);
			std::string xml(xmlString.str());
			QByteArray ba(QByteArray(xml.c_str(), xml.length()));
			
			//paste page from clipboard
			
			Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
			
			targetDoc->moveGroup(
					targetDoc->Pages->at(i)->guides.vertical(0, targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(i)->guides.horizontal(0, targetDoc->Pages->at(i)->guides.Standard),
					false,
					&pastedObjects
				       );
			s->clear();
			
			//clipping layerwise
			for (int j = 0; j < targetDoc->layerCount(); j++)
			{
				//create group out of all the items in the layer
				Selection* gs = new Selection(scMW);
				
				for (int k = 0; k < pastedObjects.count(); k++)
				{
					if (pastedObjects.itemAt(k)->LayerNr == j) gs->addItem(pastedObjects.itemAt(k));
				}
				
				if (gs->count() > 0)
				{
					//create group
					targetDoc->itemSelection_GroupObjects(false, false, gs);
					
					//get group control
					signed int groupid = gs->itemAt(0)->Groups.at(0);
					
					PageItem* groupcontrol = NULL;
					for (int k = 0; k < targetDoc->Items->count(); k++)
					{
						if (
							targetDoc->Items->at(k)->Groups.count() > 0 &&
							targetDoc->Items->at(k)->Groups.at(0) == groupid &&
							targetDoc->Items->at(k)->isGroupControl
						   )
						{
							groupcontrol = targetDoc->Items->at(k);
							break;
						}
					}
					groupcontrol->SetRectFrame();
					
					double points[32] = {
					//left upper corner - left lower corner
					0,
					0,
					0,
					0,
					
					0,
					100,
					0,
					100,
					
					//left lower corner - right lower corner
					0,
					100,
					0,
					100,
							
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),

					//right lower corner - right upper corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
							
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),

					//right upper corner - left upper corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					
					0,
					0,
					0,
					0
					};
					
					double groupX = groupcontrol->xPos();
					double groupY = groupcontrol->yPos() - targetDoc->Pages->at(i)->yOffset();
					
					double groupWidth = groupcontrol->width();
					double groupHeight = groupcontrol->height();
					
					qDebug() << groupX << groupY << groupWidth << groupHeight;
					
					for (int k = 13; k < 28; k += 2)
					{
						if  (points[k] < groupX)
						{
							points[k] = 0;
						}
						else if (points[k] - groupX > 100)
						{
							points[k] = 100;
						}
						else
						{
							points[k] = 100* (points[k] - groupX) / groupWidth;
						}
						
						if (points[k+1] < groupY)
						{
							points[k+1] = 0;
						}
						else if (points[k+1] - groupY > 100)
						{
							points[k+1] = 100;
						}
						else
						{
							points[k+1] = 100* (points[k+1] - groupY) / groupHeight;
						}
						
						qDebug() << "IMPOSITION: points[" << k << "] " << points[i] << "\n";
						qDebug() << "IMPOSITION: points[" << k+1 << "] " << points[i+1] << "\n";
					}
					
					groupcontrol->SetFrameShape(32,points);					
				}
			}
		}
	}
	
	//backsides
	for (int i = 1; i < targetDoc->Pages->count(); i = i + 2)
	{
		targetDoc->setCurrentPage(targetDoc->Pages->at(i));
		
		//copy the page to the clipboard
		//left side
		//make selections
		for (int j = 0; j < srcDoc->Items->count(); j++)
		{
			if (srcDoc->OnPage(srcDoc->Items->at(j)) == (pages->at(i)-1))
			{
				s->addItem(srcDoc->Items->at(j),false);
			}
			
		}
		
		if (s->count() > 0)
		{
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*s, xmlStream);
			std::string xml(xmlString.str());
			QByteArray ba(QByteArray(xml.c_str(), xml.length()));
			
			//paste page from clipboard
			
			Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
			
			targetDoc->moveGroup(
					targetDoc->Pages->at(i)->guides.vertical(0, targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(i)->guides.horizontal(0, targetDoc->Pages->at(i)->guides.Standard),
					false,
				       &pastedObjects
				       );
			s->clear();
			
			//clipping layerwise
			for (int j = 0; j < targetDoc->layerCount(); j++)
			{
				//create group out of all the items in the layer
				Selection* gs = new Selection(scMW);
				
				for (int k = 0; k < pastedObjects.count(); k++)
				{
					if (pastedObjects.itemAt(k)->LayerNr == j) gs->addItem(pastedObjects.itemAt(k));
				}
				
				if (gs->count() > 0)
				{
					//create group
					targetDoc->itemSelection_GroupObjects(false, false, gs);
					
					//get group control
					signed int groupid = gs->itemAt(0)->Groups.at(0);
					
					PageItem* groupcontrol = NULL;
					for (int k = 0; k < targetDoc->Items->count(); k++)
					{
						if (
							targetDoc->Items->at(k)->Groups.count() > 0 &&
							targetDoc->Items->at(k)->Groups.at(0) == groupid &&
							targetDoc->Items->at(k)->isGroupControl
						   )
						{
							groupcontrol = targetDoc->Items->at(k);
							break;
						}
					}
					groupcontrol->SetRectFrame();
					
					double points[32] = {
					//left upper corner - left lower corner
					0,
					0,
					0,
					0,
					
					0,
					100,
					0,
					100,
					
					//left lower corner - right lower corner
					0,
					100,
					0,
					100,
							
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),

					//right lower corner - right upper corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
							
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),

					//right upper corner - left upper corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					
					0,
					0,
					0,
					0
					};
					
					double groupX = groupcontrol->xPos();
					double groupY = groupcontrol->yPos() - targetDoc->Pages->at(i)->yOffset();
					
					double groupWidth = groupcontrol->width();
					double groupHeight = groupcontrol->height();
					
					qDebug() << groupX << groupY << groupWidth << groupHeight;
					
					for (int k = 13; k < 28; k += 2)
					{
						if  (points[k] < groupX)
						{
							points[k] = 0;
						}
						else if (points[k] - groupX > 100)
						{
							points[k] = 100;
						}
						else
						{
							points[k] = 100* (points[k] - groupX) / groupWidth;
						}
						
						if (points[k+1] < groupY)
						{
							points[k+1] = 0;
						}
						else if (points[k+1] - groupY > 100)
						{
							points[k+1] = 100;
						}
						else
						{
							points[k+1] = 100* (points[k+1] - groupY) / groupHeight;
						}
						
						qDebug() << "IMPOSITION: points[" << k << "] " << points[i] << "\n";
						qDebug() << "IMPOSITION: points[" << k+1 << "] " << points[i+1] << "\n";
					}
					
					groupcontrol->SetFrameShape(32,points);					
				}
			}
		}
		
		//right side
		for (int j = 0; j < srcDoc->Items->count(); j++)
		{
			if (srcDoc->OnPage(srcDoc->Items->at(j)) == (pages->at(pages->count()-i-1)-1))
			{
				s->addItem(srcDoc->Items->at(j),false);
			}
		
		}
		
		if (s->count() > 0)
		{
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*s, xmlStream);
			std::string xml(xmlString.str());
			QByteArray ba(QByteArray(xml.c_str(), xml.length()));
			
			//paste page from clipboard
			
			Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
			
			targetDoc->moveGroup(
					targetDoc->Pages->at(i)->guides.vertical(1, targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(i)->guides.horizontal(0, targetDoc->Pages->at(i)->guides.Standard),
					false,
					&pastedObjects
				       );
			s->clear();
			
			for (int j = 0; j < targetDoc->layerCount(); j++)
			{
				//create group out of all the items in the layer
				Selection* gs = new Selection(scMW);
				
				for (int k = 0; k < pastedObjects.count(); k++)
				{
					if (pastedObjects.itemAt(k)->LayerNr == j) gs->addItem(pastedObjects.itemAt(k));
				}
				
				if (gs->count() > 0)
				{
					//create group
					targetDoc->itemSelection_GroupObjects(false, false, gs);
					
					//get group control
					signed int groupid = gs->itemAt(0)->Groups.at(0);
					
					PageItem* groupcontrol = NULL;
					for (int k = 0; k < targetDoc->Items->count(); k++)
					{
						if (
							targetDoc->Items->at(k)->Groups.count() > 0 &&
							targetDoc->Items->at(k)->Groups.at(0) == groupid &&
							targetDoc->Items->at(k)->isGroupControl
						   )
						{
							groupcontrol = targetDoc->Items->at(k);
							break;
						}
					}
					groupcontrol->SetRectFrame();
					
					double points[32] = {
					//left upper corner - left lower corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					
					//left lower corner - right lower corner
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(1,targetDoc->Pages->at(i)->guides.Standard),
							
					100,
     					100,
	  				100,
       					100,

					//right lower corner - right upper corner
					100,
					100,
					100,
					100,
							
					0,
     					100,
	  				0,
       					100,
	    
					//right upper corner - left upper corner
					0,
					100,
					0,
					100,
					
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					targetDoc->Pages->at(0)->guides.vertical(1,targetDoc->Pages->at(0)->guides.Standard),
					targetDoc->Pages->at(0)->guides.horizontal(0,targetDoc->Pages->at(i)->guides.Standard),
					};
					
					double groupX = groupcontrol->xPos();
					double groupY = groupcontrol->yPos() - targetDoc->Pages->at(i)->yOffset();
					
					double groupWidth = groupcontrol->width();
					double groupHeight = groupcontrol->height();
					
					qDebug() << groupX << groupY << groupWidth << groupHeight;
					
					for (int k = 0; k < 12; k += 2)
					{
						if  (points[k] < groupX)
						{
							points[k] = 0;
						}
						else if (points[k] - groupX > 100)
						{
							points[k] = 100;
						}
						else
						{
							points[k] = 100* (points[k] - groupX) / groupWidth;
						}
						
						if (points[k+1] < groupY)
						{
							points[k+1] = 0;
						}
						else if (points[k+1] - groupY > 100)
						{
							points[k+1] = 100;
						}
						else
						{
							points[k+1] = 100* (points[k+1] - groupY) / groupHeight;
						}
						
						qDebug() << "IMPOSITION: points[" << k << "] " << points[i] << "\n";
						qDebug() << "IMPOSITION: points[" << k+1 << "] " << points[i+1] << "\n";
					}
					
					for (int k = 28; k < 32; k += 2)
					{
						if  (points[k] < groupX)
						{
							points[k] = 0;
						}
						else if (points[k] - groupX > 100)
						{
							points[k] = 100;
						}
						else
						{
							points[k] = 100* (points[k] - groupX) / groupWidth;
						}
						
						if (points[k+1] < groupY)
						{
							points[k+1] = 0;
						}
						else if (points[k+1] - groupY > 100)
						{
							points[k+1] = 100;
						}
						else
						{
							points[k+1] = 100* (points[k+1] - groupY) / groupHeight;
						}
						
						qDebug() << "IMPOSITION: points[" << k << "] " << points[i] << "\n";
						qDebug() << "IMPOSITION: points[" << k+1 << "] " << points[i+1] << "\n";
					}

					groupcontrol->SetFrameShape(32,points);					
				}
			}

		}
		
	}

}

void Imposition::booklet8p(QList<int>* pages)
{
	/*
	
	8 page imposition looks like this:
	       front:                     back:
         --------------------       --------------------
	|         |          |     |         |          |
	|         |          |     |         |          |
	|         |          |     |         |          |
	|    5    |    4     |     |    3    |    6     |
	|         |          |     |         |          |
	|         |          |     |         |          |
	|         |          |     |         |          |
        |--------------------|     |--------------------|
	|         |          |     |         |          |
	|         |          |     |         |          |
	|         |          |     |         |          |
	|    8    |    1     |     |    2    |    7     |
	|         |          |     |         |          |
	|         |          |     |         |          |
	|         |          |     |         |          |
	--------------------       --------------------
	
	*/
	
}

void Imposition::booklet16p(QList<int>* pages)
{
	/*
	16 page imposition looks like this:
	
	       front:
         -----------------------------------------
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	|    5    |    12    |    9    |    8     |
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	|--------------------|--------------------|
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	|    4    |    13    |    16   |    1     |
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	 -----------------------------------------
	
	       back:
	 -----------------------------------------
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	|    7    |    10    |    11   |    6     |
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	|--------------------|--------------------|
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	|    2    |    15    |    14   |    3     |
	|         |          |         |          |
	|         |          |         |          |
	|         |          |         |          |
	 -----------------------------------------
	*/

}

void Imposition::changeDocFold()
{
	if (isOK == true)
	{
		//create page
		if (foldIsBackSide->checkState() == Qt::Checked)
			targetDoc->createNewDocPages(2);
		else
			targetDoc->createNewDocPages(1);
		
		//copy layers
		for (int i = 1; i < srcDoc->layerCount(); i++)
		{
			targetDoc->addLayer(srcDoc->layerName(i));
		}
		
		targetDoc->setCurrentPage(targetDoc->Pages->at(0));
		Page* p = targetDoc->currentPage();
		
		int firstPage = foldFrontPage->currentText().toInt() - 1;
		int lastPage = 0;
		if (foldFrontPage->currentIndex() < (foldFrontPage->count()-1))
		{
			lastPage = foldFrontPage->itemText(foldFrontPage->currentIndex()+1).toInt() - 2;
		}
		else
		{
			lastPage = firstPage + srcDoc->currentPageLayout;
		}
		
		//make guides
		double allWidth = srcDoc->pageWidth * (srcDoc->currentPageLayout+1);
		double allHeight = srcDoc->pageHeight;
		
		double guide_x = (p->width() - allWidth)/2; //initial (left) guide
		p->guides.addVertical(guide_x, p->guides.Standard);
		
		for (int i = firstPage; i <= lastPage; i++)
		{
			guide_x += srcDoc->Pages->at(i)->width();
			p->guides.addVertical(guide_x,p->guides.Standard);
		}
		
		double guide_y = (p->height() - allHeight)/2;
		p->guides.addHorizontal(guide_y, p->guides.Standard);
		guide_y += allHeight;
		p->guides.addHorizontal(guide_y, p->guides.Standard);
		
		//do the copying
		ScribusMainWindow* scMW = ScCore->primaryMainWindow();
		scMW->view->requestMode(modeNormal);
		Selection* s = new Selection(scMW);
		
		//select items to copy for the first page
		for (int i = 0; i < srcDoc->Items->count(); i++)
		{
			if (	(srcDoc->OnPage(srcDoc->Items->at(i)) >= firstPage) &&
				(srcDoc->OnPage(srcDoc->Items->at(i)) <= lastPage)
			   )
				s->addItem(srcDoc->Items->at(i),false);
		}
		
		if (s->count() > 0)
		{
			//move the selection to the clipboard
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*s, xmlStream);
			std::string xml(xmlString.str());
			QByteArray ba(QByteArray(xml.c_str(), xml.length()));
			
			//paste
			Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
			targetDoc->moveGroup(
					p->guides.vertical(0, p->guides.Standard),
					p->guides.horizontal(0, p->guides.Standard),
					true,
					&pastedObjects
				);
		}
		
		if (foldIsBackSide->checkState() != Qt::Checked) return;
		
		//do the second page
		s->clear();
		firstPage = foldBackPage->currentText().toInt() - 1;
		if (foldBackPage->currentIndex() < (foldBackPage->count()-1))
		{
			lastPage = foldBackPage->itemText(foldBackPage->currentIndex()+1).toInt() - 2;
		}
		else
		{
			lastPage = firstPage + srcDoc->currentPageLayout;
		}
		
		targetDoc->setCurrentPage(targetDoc->Pages->at(1));
		p = targetDoc->currentPage();
		guide_x = (p->width() - allWidth)/2; //initial (left) guide
		p->guides.addVertical(guide_x, p->guides.Standard);
		
		for (int i = firstPage; i <= lastPage; i++)
		{
			guide_x += srcDoc->Pages->at(i)->width();
			p->guides.addVertical(guide_x,p->guides.Standard);
		}
		
		guide_y = (p->height() - allHeight)/2;
		p->guides.addHorizontal(guide_y, p->guides.Standard);
		guide_y += allHeight;
		p->guides.addHorizontal(guide_y, p->guides.Standard);
		
		for (int i = 0; i < srcDoc->Items->count(); i++)
		{
			if (	(srcDoc->OnPage(srcDoc->Items->at(i)) >= firstPage) &&
						      (srcDoc->OnPage(srcDoc->Items->at(i)) <= lastPage)
			   )
				s->addItem(srcDoc->Items->at(i),false);
		}
		
		if (s->count() > 0)
		{
			//move the selection to the clipboard
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*s, xmlStream);
			std::string xml(xmlString.str());
			QByteArray ba(QByteArray(xml.c_str(), xml.length()));
			
			//paste
			Selection pastedObjects = Serializer(*targetDoc).deserializeObjects(ba);
			targetDoc->moveGroup(
					p->guides.vertical(0, p->guides.Standard),
					p->guides.horizontal(0, p->guides.Standard),
					true,
					&pastedObjects
				       );
		}
	}
}

void Imposition::changePage()
{
	//create new ScribusDoc
	
	PageSize* t_ps = new PageSize(pageWidthBox->value()/unitRatio,pageHeightBox->value()/unitRatio);
	
	//...name...
	QString* t_name = new QString("imposition");
	
	//...margins...
	MarginStruct* t_margins = new MarginStruct(15,15,15,15);
	
	//...page setup
	DocPagesSetup* t_pagesetup = new DocPagesSetup(0,0,1,0,false,0,0);
	
	//... et voila!
	this->targetDoc = new ScribusDoc(*t_name,unitIndex,*t_ps,*t_margins,*t_pagesetup);
	
	this->w = new ScribusWin(ScCore->primaryMainWindow()->wsp, targetDoc);
	this-> view = new ScribusView(w, ScCore->primaryMainWindow(), targetDoc);
	targetDoc->setGUI(true,ScCore->primaryMainWindow(),view);
	
	//update
	switch (impositionTabWidget->currentIndex())
	{
		case 0:	changeDocGrid();
		break;
		case 1:	changeDocBooklet();
		break;
		case 2:	changeDocFold();
		break;
	}
	
}

bool Imposition::verifyPage()
{
	bool result = false;
	switch (impositionTabWidget->currentIndex())
	{
		case 0: 
			result = verifyGrid();
			break;
		case 1:
			result = verifyBooklet();
			break;
		case 2:
			result = verifyFold();
			break;
	}
	isEdited = true;
	btnGo->setEnabled(result);
	btnPreview->setEnabled(result);
	return result;
}

bool Imposition::verifyGrid()
{
	bool result = false;

	Page* src = srcDoc->Pages->at(cbFront->currentIndex());
	
//	double srcWidth = src->width();
//	double srcHeight = src->height();
	double realSrcWidth = src->width() + srcDoc->bleeds.Left + srcDoc->bleeds.Right;
	double realSrcHeight = src->height() + srcDoc->bleeds.Top + srcDoc->bleeds.Bottom;
	double width = pageWidthBox->value() / unitRatio;
	double height = pageHeightBox->value() / unitRatio;

	if ( 	(realSrcWidth < width) &&
		(realSrcHeight < height) )
	{
		result = true;
	}

	return result;
}

bool Imposition::verifyBooklet()
{
	return true;
}

bool Imposition::verifyFold()
{
	if (srcDoc->currentPageLayout == 0) {
		return false;
		
	}
	
	if (pageWidthBox->value() < (srcDoc->currentPageLayout + 1)*srcDoc->pageWidth ||
		   pageHeightBox->value() < srcDoc->pageHeight)
	{
		return false;
	}
	
	return true;
	
}

void Imposition::customizeSize()
{
	pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
}

void Imposition::change2SidesBox(int state)
{
	if (state == 0) 
		cbBack->setEnabled(false);
	else
		cbBack->setEnabled(true);
}

void Imposition::changeFoldBackPage(int state)
{
	if (state == 0)
		foldBackPage->setEnabled(false);
	else
 		foldBackPage->setEnabled(true);
}

void Imposition::accepted()
{
	if (isEdited == true) changePage();

	ScribusMainWindow* scMW = ScCore->primaryMainWindow();
			
	w->setMainWindow(scMW);
	
	w->setView(view);
	
	scMW->alignDistributePalette->setDoc(targetDoc);
	
	scMW->ActWin = w;
	
	targetDoc->WinHan = w;
	
	w->setCentralWidget(view);
	scMW->wsp->addWindow(w);
	
	scMW->HaveDoc++;
	scMW->HaveNewDoc();
	targetDoc->reformPages();
	
	w->show();
	view->show();
	scMW->newActWin(w);
	targetDoc->setCurrentPage(targetDoc->DocPages.at(0));
	view->slotDoZoom();
	view->GotoPage(0);

	connect(scMW->wsp, SIGNAL(windowActivated(QWidget *)), scMW, SLOT(newActWin(QWidget *)));
	connect(w, SIGNAL(AutoSaved()), scMW, SLOT(slotAutoSaved()));
	connect(UndoManager::instance(), SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	targetDoc->connectDocSignals();
	UndoManager::instance()->setUndoEnabled(true);
}
