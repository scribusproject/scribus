/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


void TOCIndexPrefs::init()
{
	trStrNone=QT_TR_NOOP("None");
	strNone="None";
	trStrPNBeginning=QT_TR_NOOP("At the beginning");
	strPNBeginning="At the beginning";
	trStrPNEnd=QT_TR_NOOP("At the end");
	strPNEnd="At the end";
	trStrPNNotShown=QT_TR_NOOP("Not Shown");
	strPNNotShown="Not Shown";
	
	itemNumberPlacementComboBox->clear();
	itemNumberPlacementComboBox->insertItem(trStrPNEnd);	
	itemNumberPlacementComboBox->insertItem(trStrPNBeginning);
	itemNumberPlacementComboBox->insertItem(trStrPNNotShown);
	itemNumberPlacementComboBox->setCurrentText(trStrPNEnd);
}


void TOCIndexPrefs::destroy()
{

}

void TOCIndexPrefs::setup( ToCSetupVector* tocsetups, ScribusDoc *doc)
{
	localToCSetupVector=* tocsetups;
	currDoc=doc;
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
}

void TOCIndexPrefs::generatePageItemList()
{
	itemDestFrameComboBox->clear();
	itemDestFrameComboBox->insertItem(trStrNone);
	if (currDoc!=NULL)
	{
		for (uint d = 0; d < currDoc->DocItems.count(); ++d)
		{
			if (currDoc->DocItems.at(d)->itemType()==PageItem::TextFrame)
				itemDestFrameComboBox->insertItem(currDoc->DocItems.at(d)->itemName());
		}
	}
	else
		itemDestFrameComboBox->setEnabled(false);
}


void TOCIndexPrefs::setupItemAttrs( QStringList newNames )
{
	itemAttrComboBox->clear();
	itemAttrComboBox->insertItem(trStrNone);
	itemAttrComboBox->insertStringList(newNames);
}


void TOCIndexPrefs::selectToC( int numberSelected )
{
	uint num=numberSelected;
	if (localToCSetupVector.isEmpty())
		return;
	if (localToCSetupVector.count()<num)
		num=0;
	if (localToCSetupVector[num].itemAttrName==strNone)
		itemAttrComboBox->setCurrentText(trStrNone);
	else
		itemAttrComboBox->setCurrentText(localToCSetupVector[num].itemAttrName);
	
	if (localToCSetupVector[num].pageLocation==NotShown)
		itemNumberPlacementComboBox->setCurrentText(trStrPNNotShown);
	else
		if (localToCSetupVector[num].pageLocation==Beginning)
		itemNumberPlacementComboBox->setCurrentText(trStrPNBeginning);
	else
		itemNumberPlacementComboBox->setCurrentText(trStrPNEnd);

	if (currDoc!=NULL)
	{	
		if (localToCSetupVector[num].frameName==strNone)
			itemDestFrameComboBox->setCurrentText(trStrNone);
		else
			itemDestFrameComboBox->setCurrentText(localToCSetupVector[num].frameName);
			
		if (!paragraphStyleList.contains(localToCSetupVector[num].textStyle) || localToCSetupVector[num].textStyle==strNone)
			itemParagraphStyleComboBox->setCurrentText(trStrNone);
		else
			itemParagraphStyleComboBox->setCurrentText(localToCSetupVector[num].textStyle);
	}
	
	if (numberSelected>=0)
	{
		disconnect(tocNameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setToCName(const QString &)));
		tocNameLineEdit->setText(tocListBox->currentText());
		connect(tocNameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setToCName(const QString &)));
	}
}


void TOCIndexPrefs::addToC()
{
	bool found=false;
	QString newName=tocNameLineEdit->text();
	for(ToCSetupVector::Iterator it = localToCSetupVector.begin(); it!= localToCSetupVector.end(); ++it)
	{
		if ((*it).name==newName)
			found=true;
	}
	if (found || newName.isEmpty())
		newName=QString("Table of Contents %1").arg(localToCSetupVector.count()+1);
	ToCSetup newToCEntry;
	newToCEntry.name=newName;
	newToCEntry.itemAttrName=strNone;
	newToCEntry.frameName=strNone;
	newToCEntry.textStyle=strNone;
	newToCEntry.pageLocation=End;
	localToCSetupVector.append(newToCEntry);
	updateToCListBox();
	tocListBox->setCurrentItem(localToCSetupVector.count()-1);
	selectToC(localToCSetupVector.count()-1);
	enableGUIWidgets();
}


void TOCIndexPrefs::updateToCListBox()
{
	tocListBox->clear();
	for(ToCSetupVector::Iterator it = localToCSetupVector.begin(); it!= localToCSetupVector.end(); ++it)
		tocListBox->insertItem((*it).name);
}

void TOCIndexPrefs::updateParagraphStyleComboBox()
{
	paragraphStyleList.clear();
	paragraphStyleList.append(trStrNone);
	
	if(currDoc!=NULL && currDoc->docParagraphStyles.count()>5)
	{
		for (uint i = 5; i < currDoc->docParagraphStyles.count(); ++i)
			paragraphStyleList.append(currDoc->docParagraphStyles[i].Vname);
	}
	itemParagraphStyleComboBox->clear();
	itemParagraphStyleComboBox->insertStringList(paragraphStyleList);
}


void TOCIndexPrefs::enableGUIWidgets()
{
	bool enabled=(localToCSetupVector.count()>0);
	tocListBox->setEnabled(enabled);
	tocDeleteButton->setEnabled(enabled);
	itemAttrComboBox->setEnabled(enabled);
	itemNumberPlacementComboBox->setEnabled(enabled);
	bool haveDoc=enabled && currDoc!=NULL;
	itemDestFrameComboBox->setEnabled(haveDoc);
	itemParagraphStyleComboBox->setEnabled(haveDoc);
}


void TOCIndexPrefs::deleteToC()
{
	int numberSelected=tocListBox->currentItem();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it;
		for(it = localToCSetupVector.begin(); it!= localToCSetupVector.end(), i<numberSelected ; ++it, ++i)
			;
		localToCSetupVector.erase(it);
		updateToCListBox();
		enableGUIWidgets();
	}
}


void TOCIndexPrefs::itemAttributeSelected( const QString& itemAttributeName )
{
	int numberSelected=tocListBox->currentItem();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it;
		for(it = localToCSetupVector.begin(); it!= localToCSetupVector.end(), i<numberSelected ; ++it, ++i)
			;
		if (itemAttributeName==trStrNone)
			(*it).itemAttrName=strNone;
		else
			(*it).itemAttrName=itemAttributeName;
	}
}


void TOCIndexPrefs::itemFrameSelected( const QString& frameName )
{
	int numberSelected=tocListBox->currentItem();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it;
		for(it = localToCSetupVector.begin(); it!= localToCSetupVector.end(), i<numberSelected ; ++it, ++i)
			;
		if (frameName==trStrNone)
			(*it).frameName=strNone;
		else
			(*it).frameName=frameName;
	}

}


void TOCIndexPrefs::itemPageNumberPlacedSelected( const QString& pageLocation )
{
	int numberSelected=tocListBox->currentItem();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it;
		for(it = localToCSetupVector.begin(); it!= localToCSetupVector.end(), i<numberSelected ; ++it, ++i)
			;
		if (pageLocation==trStrPNBeginning || pageLocation==strPNBeginning)
			(*it).pageLocation=Beginning;
		else
		if (pageLocation==trStrPNEnd || pageLocation==strPNEnd)
			(*it).pageLocation=End;
		else
			(*it).pageLocation=NotShown;
	}
}


void TOCIndexPrefs::itemParagraphStyleSelected( const QString& itemStyle )
{
	int numberSelected=tocListBox->currentItem();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it;
		for(it = localToCSetupVector.begin(); it!= localToCSetupVector.end(), i<numberSelected ; ++it, ++i)
			;
		if (itemStyle==trStrNone)
			(*it).textStyle=strNone;
		else
			(*it).textStyle=itemStyle;
	}
}

ToCSetupVector* TOCIndexPrefs::getNewToCs()
{
	return &localToCSetupVector;
}


void TOCIndexPrefs::setToCName( const QString &newName )
{
	int numberSelected=tocListBox->currentItem();
	if (numberSelected!=-1)
	{
		tocListBox->changeItem(newName, numberSelected);
		int i=0;
		ToCSetupVector::Iterator it;
		for(it = localToCSetupVector.begin(); it!= localToCSetupVector.end(), i<numberSelected ; ++it, ++i)
			;
		(*it).name=newName;
	}
}
