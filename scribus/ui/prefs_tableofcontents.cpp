/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_tableofcontents.h"
#include "pagestructs.h"
#include "prefsstructs.h"
#include "page.h"
#include "scribusdoc.h"
#include "commonstrings.h"
#include "util.h"

Prefs_TableOfContents::Prefs_TableOfContents(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	m_Doc(doc)
{
	setupUi(this);
	languageChange();
	itemDestFrameComboBox->setMaximumWidth(fontMetrics().width( "This is a very long Name" ));
	itemAttrComboBox->setMaximumWidth(fontMetrics().width( "This is a very long Name" ));
	itemNumberPlacementComboBox->setMaximumWidth(fontMetrics().width( "This is a very long Name" ));
	itemParagraphStyleComboBox->setMaximumWidth(fontMetrics().width( "This is a very long Name" ));

	// signals and slots connections
	//do not connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	connect( tocAddButton, SIGNAL( clicked() ), this, SLOT( addToC() ) );
	connect( tocDeleteButton, SIGNAL( clicked() ), this, SLOT( deleteToC() ) );
	connect( itemAttrComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemAttributeSelected(const QString&) ) );
	connect( itemDestFrameComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemFrameSelected(const QString&) ) );
	connect( itemParagraphStyleComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemParagraphStyleSelected(const QString&) ) );
	connect( itemNumberPlacementComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemPageNumberPlacedSelected(const QString&) ) );
	connect( tocNameLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( setToCName(const QString&) ) );
	connect( itemListNonPrintingCheckBox, SIGNAL( toggled(bool) ), this, SLOT( nonPrintingFramesSelected(bool) ) );
	setCurrentComboItem(itemNumberPlacementComboBox, trStrPNEnd);
	numSelected=999;
}

Prefs_TableOfContents::~Prefs_TableOfContents()
{
}


void Prefs_TableOfContents::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void Prefs_TableOfContents::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	localToCSetupVector=prefsData->tocPrefs.defaultToCSetups;
	generatePageItemList();
	bool enabled=(localToCSetupVector.count()>0);
	if (enabled)
	{
		updateToCListBox();
		updateParagraphStyleComboBox();
		tocListBox->setCurrentItem(0);
		selectToC(0);
	}
	else
		tocListBox->clear();
	enableGUIWidgets();
	connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
}

void Prefs_TableOfContents::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->tocPrefs.defaultToCSetups = localToCSetupVector;
}

void Prefs_TableOfContents::languageChange()
{
	trStrPNBeginning=tr("Beginning");
	strPNBeginning="Beginning";
	trStrPNEnd=tr("End");
	strPNEnd="End";
	trStrPNNotShown=tr("Not Shown");
	strPNNotShown="Not Shown";

	disconnect( itemNumberPlacementComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemPageNumberPlacedSelected(const QString&) ) );
	int i=itemNumberPlacementComboBox->currentIndex();
	itemNumberPlacementComboBox->clear();
	itemNumberPlacementComboBox->addItem(trStrPNEnd);
	itemNumberPlacementComboBox->addItem(trStrPNBeginning);
	itemNumberPlacementComboBox->addItem(trStrPNNotShown);
	itemNumberPlacementComboBox->setCurrentIndex(i);
	connect( itemNumberPlacementComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemPageNumberPlacedSelected(const QString&) ) );
}

void Prefs_TableOfContents::destroy()
{

}

void Prefs_TableOfContents::generatePageItemList()
{
	itemDestFrameComboBox->clear();
	itemDestFrameComboBox->addItem(CommonStrings::tr_None);
	if (m_Doc!=NULL)
	{
		QList<PageItem*> allItems;
		for (int a = 0; a < m_Doc->DocItems.count(); ++a)
		{
			PageItem *currItem = m_Doc->DocItems.at(a);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (currItem->itemType() == PageItem::TextFrame)
					itemDestFrameComboBox->addItem(currItem->itemName());
			}
			allItems.clear();
		}
	}
	else
		itemDestFrameComboBox->setEnabled(false);
}

void Prefs_TableOfContents::setupItemAttrs( QStringList newNames )
{
	disconnect( itemAttrComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemAttributeSelected(const QString&) ) );
	itemAttrComboBox->clear();
	itemAttrComboBox->addItem(CommonStrings::tr_None);
	itemAttrComboBox->addItems(newNames);
	if (numSelected!=999 && numSelected!=-1)
	{
		if (localToCSetupVector[numSelected].itemAttrName==CommonStrings::None)
			setCurrentComboItem(itemAttrComboBox, CommonStrings::tr_None);
		else
			setCurrentComboItem(itemAttrComboBox, localToCSetupVector[numSelected].itemAttrName);
	}
	connect( itemAttrComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemAttributeSelected(const QString&) ) );
}

void Prefs_TableOfContents::selectToC( int numberSelected )
{
	numSelected=numberSelected;
	if (numSelected < 0)
		return;
	if (localToCSetupVector.isEmpty())
		return;
	if (localToCSetupVector.count()<numSelected)
		numSelected=0;
	disconnect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	disconnect( itemAttrComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemAttributeSelected(const QString&) ) );
	disconnect( itemDestFrameComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemFrameSelected(const QString&) ) );
	disconnect( itemParagraphStyleComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemParagraphStyleSelected(const QString&) ) );
	disconnect( itemNumberPlacementComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemPageNumberPlacedSelected(const QString&) ) );
	disconnect( tocNameLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( setToCName(const QString&) ) );
	disconnect( itemListNonPrintingCheckBox, SIGNAL( toggled(bool) ), this, SLOT( nonPrintingFramesSelected(bool) ) );
	if (localToCSetupVector[numSelected].itemAttrName==CommonStrings::None)
		setCurrentComboItem(itemAttrComboBox, CommonStrings::tr_None);
	else
		setCurrentComboItem(itemAttrComboBox, localToCSetupVector[numSelected].itemAttrName);
	if (localToCSetupVector[numSelected].pageLocation==NotShown)
		setCurrentComboItem(itemNumberPlacementComboBox, trStrPNNotShown);
	else
		if (localToCSetupVector[numSelected].pageLocation==Beginning)
		setCurrentComboItem(itemNumberPlacementComboBox, trStrPNBeginning);
	else
		setCurrentComboItem(itemNumberPlacementComboBox, trStrPNEnd);

	itemListNonPrintingCheckBox->setChecked(localToCSetupVector[numSelected].listNonPrintingFrames);
	if (m_Doc!=NULL)
	{
		if (localToCSetupVector[numSelected].frameName==CommonStrings::None)
			setCurrentComboItem(itemDestFrameComboBox, CommonStrings::tr_None);
		else
			setCurrentComboItem(itemDestFrameComboBox, localToCSetupVector[numSelected].frameName);

		if (itemParagraphStyleComboBox->count()>0)
		{
			if (!paragraphStyleList.contains(localToCSetupVector[numSelected].textStyle) || localToCSetupVector[numSelected].textStyle==CommonStrings::None)
				setCurrentComboItem(itemParagraphStyleComboBox, CommonStrings::tr_None);
			else
				setCurrentComboItem(itemParagraphStyleComboBox, localToCSetupVector[numSelected].textStyle);
		}
	}

	if (tocListBox->currentItem())
		tocNameLineEdit->setText(tocListBox->currentItem()->text());

	connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	connect( itemAttrComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemAttributeSelected(const QString&) ) );
	connect( itemDestFrameComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemFrameSelected(const QString&) ) );
	connect( itemParagraphStyleComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemParagraphStyleSelected(const QString&) ) );
	connect( itemNumberPlacementComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemPageNumberPlacedSelected(const QString&) ) );
	connect( tocNameLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( setToCName(const QString&) ) );
	connect( itemListNonPrintingCheckBox, SIGNAL( toggled(bool) ), this, SLOT( nonPrintingFramesSelected(bool) ) );
}


void Prefs_TableOfContents::addToC()
{
	bool found=false;
	QString newName=tocNameLineEdit->text();
	for(ToCSetupVector::Iterator it = localToCSetupVector.begin(); it!= localToCSetupVector.end(); ++it)
	{
		if ((*it).name==newName)
			found=true;
	}
	if (found || newName.isEmpty())
		newName=tr("Table of Contents %1").arg(localToCSetupVector.count()+1);
	ToCSetup newToCEntry;
	newToCEntry.name=newName;
	newToCEntry.itemAttrName=CommonStrings::None;
	newToCEntry.frameName=CommonStrings::None;
	newToCEntry.textStyle=CommonStrings::None;
	newToCEntry.pageLocation=End;
	newToCEntry.listNonPrintingFrames=false;
	localToCSetupVector.append(newToCEntry);
	disconnect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	updateToCListBox();
	if (localToCSetupVector.count()==1) //reinit parastyles if we are adding the first TOC
		updateParagraphStyleComboBox();
	tocListBox->setCurrentRow(localToCSetupVector.count()-1);
	selectToC(localToCSetupVector.count()-1);
	enableGUIWidgets();
	connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
}


void Prefs_TableOfContents::updateToCListBox()
{
	tocListBox->clear();
	QStringList sl;
	for(ToCSetupVector::Iterator it = localToCSetupVector.begin(); it!= localToCSetupVector.end(); ++it)
		sl << (*it).name;
	tocListBox->insertItems(0, sl);
}

void Prefs_TableOfContents::updateParagraphStyleComboBox()
{
	paragraphStyleList.clear();
	paragraphStyleList.append(CommonStrings::tr_None);

	if(m_Doc!=NULL) // && m_Doc->docParagraphStyles.count()>5)
	{
		for (int i = 0; i < m_Doc->paragraphStyles().count(); ++i)
			paragraphStyleList.append(m_Doc->paragraphStyles()[i].name());
	}
	itemParagraphStyleComboBox->clear();
	itemParagraphStyleComboBox->addItems(paragraphStyleList);
}


void Prefs_TableOfContents::enableGUIWidgets()
{
	bool enabled=(localToCSetupVector.count()>0);
	tocListBox->setEnabled(enabled);
	tocDeleteButton->setEnabled(enabled);
	itemAttrComboBox->setEnabled(enabled);
	itemNumberPlacementComboBox->setEnabled(enabled);
	bool haveDoc=enabled && m_Doc!=NULL;
	itemDestFrameComboBox->setEnabled(haveDoc);
	itemParagraphStyleComboBox->setEnabled(haveDoc);
}


void Prefs_TableOfContents::deleteToC()
{
	int numberSelected=tocListBox->currentRow();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it=localToCSetupVector.begin();
		while (it!= localToCSetupVector.end() && i<numberSelected)
		{
			++it;
			++i;
		}
		localToCSetupVector.erase(it);
		updateToCListBox();
		enableGUIWidgets();
	}
}


void Prefs_TableOfContents::itemAttributeSelected( const QString& itemAttributeName )
{
	int numberSelected=tocListBox->currentRow();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it=localToCSetupVector.begin();
		while (it!= localToCSetupVector.end() && i<numberSelected)
		{
			++it;
			++i;
		}
		if (itemAttributeName==CommonStrings::tr_None)
			(*it).itemAttrName=CommonStrings::None;
		else
			(*it).itemAttrName=itemAttributeName;
	}
}


void Prefs_TableOfContents::itemFrameSelected( const QString& frameName )
{
	int numberSelected=tocListBox->currentRow();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it=localToCSetupVector.begin();
		while (it!= localToCSetupVector.end() && i<numberSelected)
		{
			++it;
			++i;
		}
		if (frameName==CommonStrings::tr_None)
			(*it).frameName=CommonStrings::None;
		else
			(*it).frameName=frameName;
	}

}


void Prefs_TableOfContents::itemPageNumberPlacedSelected( const QString& pageLocation )
{
	int numberSelected=tocListBox->currentRow();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it=localToCSetupVector.begin();
		while (it!= localToCSetupVector.end() && i<numberSelected)
		{
			++it;
			++i;
		}
		if (pageLocation==trStrPNBeginning || pageLocation==strPNBeginning)
			(*it).pageLocation=Beginning;
		else
		if (pageLocation==trStrPNEnd || pageLocation==strPNEnd)
			(*it).pageLocation=End;
		else
			(*it).pageLocation=NotShown;
	}
}


void Prefs_TableOfContents::itemParagraphStyleSelected( const QString& itemStyle )
{
	int numberSelected=tocListBox->currentRow();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it=localToCSetupVector.begin();
		while (it!= localToCSetupVector.end() && i<numberSelected)
		{
			++it;
			++i;
		}
		if (itemStyle==CommonStrings::tr_None)
			(*it).textStyle=CommonStrings::None;
		else
			(*it).textStyle=itemStyle;
	}
}

void Prefs_TableOfContents::setToCName( const QString &newName )
{
	int numberSelected=tocListBox->currentRow();
	if (numberSelected!=-1)
	{
		tocListBox->item(numberSelected)->setText(newName);
		int i=0;
		ToCSetupVector::Iterator it=localToCSetupVector.begin();
		while (it!= localToCSetupVector.end() && i<numberSelected)
		{
			++it;
			++i;
		}
		(*it).name=newName;
	}
}


void Prefs_TableOfContents::nonPrintingFramesSelected( bool showNonPrinting )
{
	int numberSelected=tocListBox->currentRow();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it=localToCSetupVector.begin();
		while (it!= localToCSetupVector.end() && i<numberSelected)
		{
			++it;
			++i;
		}
		(*it).listNonPrintingFrames=showNonPrinting;
	}
}

