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
#include "imposition.h"
#include "scribus.h"
#include "pagesize.h"
#include "units.h"
#include "commonstrings.h"
#include "aligndistribute.h"
#include "guidemanagercore.h"

Imposition::Imposition(QWidget* parent, ScribusDoc* doc)
  : QDialog(parent,"Imposition", true, 0)
{
	setupUi(this);

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
	PageSize ps(tr(doc->m_pageSize));
	pageSizeComboBox->insertStringList(ps.sizeTRList());
	pageSizeComboBox->insertItem( CommonStrings::trCustomPageSize);
	
	QStringList pageSizes=ps.sizeList();
	int sizeIndex=pageSizes.findIndex(ps.nameTR());
	if (sizeIndex!=-1)
		pageSizeComboBox->setCurrentItem(sizeIndex);
	else
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);

	pageOrientationComboBox->insertItem( tr( "Portrait" ) );
	pageOrientationComboBox->insertItem( tr( "Landscape" ) );

	//put page dimensions to boxes
	pageWidthBox->setValue(ps.width() * unitRatio);
	pageHeightBox->setValue(ps.height() * unitRatio);
	
	//fill grid pages boxes with pagelists
	for (int i = 1; i <= (int)(doc->Pages->count()); i++)
	{
		cbFront->insertItem(QString::number(i));
		cbBack->insertItem(QString::number(i));
	}	
	
	//create target document
	changePage();
	changeDocGrid();
	

	//connect slots and signals
	connect(pageSizeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPageSize(const QString &)));
		
 	connect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	connect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	connect(chb2Sides, SIGNAL(stateChanged(int)), this, SLOT(change2SidesBox(int)));
	connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
	
	//document changing slots
	connect(impositionTabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));
	connect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(changePage()));
	connect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(changePage()));
	
	connect(boxCopies, SIGNAL(valueChanged(int)), this, SLOT(changePage()));
	connect(chb2Sides, SIGNAL(stateChanged(int)), this, SLOT(changePage()));
	connect(cbFront, SIGNAL(currentIndexChanged(QString)), this, SLOT(changePage()));
	connect(cbBack, SIGNAL(currentIndexChanged(QString)), this, SLOT(changePage()));
	
	connect(cbSetting, SIGNAL(currentIndexChanged(QString)), this, SLOT(changePage()));
	
	

}


Imposition::~Imposition()
{

}

void Imposition::languageChange()
{
	
}

void Imposition::setPageSize(const QString &size)
{
	disconnect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	disconnect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	disconnect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(changePage())); //one is sufficient
	
	PageSize* ps = new PageSize(tr(size));
	
	pageWidthBox->setValue(ps->width() * unitRatio);
	pageHeightBox->setValue(ps->height() * unitRatio);
	
	delete ps;
	
	connect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(changePage()));
	connect(pageWidthBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
	connect(pageHeightBox, SIGNAL(valueChanged(double)), this, SLOT(customizeSize()));
}

void Imposition::changeTab(int dest)
{
	changePage();
}

void Imposition::changeDocGrid()
{
	//get copied page
	Page* src = srcDoc->Pages->at(cbFront->currentIndex());
	
	double srcWidth = src->width();
	double srcHeight = src->height();
	double realSrcWidth = src->width() + srcDoc->bleeds.Left + srcDoc->bleeds.Right;
	double realSrcHeight = src->height() + srcDoc->bleeds.Top + srcDoc->bleeds.Bottom;
	
	//check whether width fits
	if ( realSrcWidth < (targetDoc->pageWidth) )
	{
		//count how many rows and cols will be needed
		int cols = (int)((targetDoc->pageWidth)/realSrcWidth); // how many columns do we have on page?
		int rows = (int)((targetDoc->pageHeight)/realSrcHeight); // how many rows do we have on page?
		
		//now count how many pages are needed
		int countPages=0;
		countPages = (int)(boxCopies->value() / (cols * rows)) + 1;
		if ((boxCopies->value() % (cols * rows) ) == 0) countPages--;
		
		targetDoc->createNewDocPages(countPages);
		
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
		
		
		//start copying
		for (int i = 0; i < srcDoc->Items->count(); i++)
		{
			
			//determine if object is on the source page
			if (srcDoc->OnPage(srcDoc->Items->at(i)) != src->pageNr())
			{
				continue;
			}
			else
			{
				//copy the item to the buffer
				CopyPasteBuffer origBuffer;
				CopyPasteBuffer pasteBuffer;
				srcDoc->Items->at(i)->copyToCopyPasteBuffer(&origBuffer);
				srcDoc->Items->at(i)->copyToCopyPasteBuffer(&pasteBuffer);
				
				int currow = 0;
				int curcol = 0;
				targetDoc->setCurrentPage(targetDoc->Pages->at(0));
				Page* cur = targetDoc->currentPage();
				
				//and now, copy!
				for  (int j = 0; j < boxCopies->value(); j++)
				{
					printf("xOffset: %f ; yOffset: %f\n", cur->xOffset(), cur->yOffset());
					
					pasteBuffer.Xpos = cur->guides.vertical(curcol*2, cur->guides.Standard) + origBuffer.Xpos;
					pasteBuffer.Ypos = cur->guides.horizontal(currow*2, cur->guides.Standard) + origBuffer.Ypos + cur->yOffset()-20;
					
					view->PasteItem(&pasteBuffer, true, true, true);
					
					//printf("curcol: %d, currow: %d, rows: %d, cols: %d", curcol, currow, rows, cols);
					
					if ((curcol + 1) == cols)
					{
						curcol = 0;
						if ((currow + 1) == rows && (j+1 < boxCopies->value()))
						{
							currow = 0;
							targetDoc->setCurrentPage(targetDoc->Pages->at(cur->pageNr()+1));
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
			}
		}
		
		btnGo->setEnabled(true);
	}
	else
	{
		targetDoc = new ScribusDoc();
		btnGo->setEnabled(false);
	}
}

void Imposition::changeDocBooklet()
{
	//booklet positioning
}

void Imposition::changeDocFold()
{
	//fold positioning
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
	switch (impositionTabWidget->currentPageIndex())
	{
		case 0:	changeDocGrid();
		return;
		case 1:	changeDocBooklet();
		return;
		case 2:	changeDocFold();
		return;
	}
	
}

void Imposition::customizeSize()
{
	pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
}

void Imposition::change2SidesBox(int state)
{
	if (state == 0) 
		cbBack->setEnabled(false);
	else
		cbBack->setEnabled(true);
}

void Imposition::accepted()
{
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
	
	w->showMaximized();
	view->show();
}
