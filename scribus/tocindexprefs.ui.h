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
	trNone=QT_TR_NOOP("None");
	none="None";
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
	itemDestFrameComboBox->insertItem(trNone);
	if (currDoc!=NULL)
	{
		if (currDoc->DocItems.count()>0)
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
	itemAttrComboBox->insertItem(trNone);
	itemAttrComboBox->insertStringList(newNames);	
	selectToC(0);
}


void TOCIndexPrefs::selectToC( int numberSelected )
{
	uint num=numberSelected;
	if (localToCSetupVector.isEmpty())
		return;
	if (localToCSetupVector.count()<num)
		num=0;
	if (localToCSetupVector[num].itemAttrName==none)
		itemAttrComboBox->setCurrentText(trNone);
	else
		itemAttrComboBox->setCurrentText(localToCSetupVector[num].itemAttrName);
	if (localToCSetupVector[num].frameName==none)
		itemDestFrameComboBox->setCurrentText(trNone);
	else
		itemDestFrameComboBox->setCurrentText(localToCSetupVector[num].frameName);
	if (localToCSetupVector[num].style==none)
		itemStyleComboBox->setCurrentText(trNone);
	else
		itemStyleComboBox->setCurrentText(localToCSetupVector[num].style);
}


void TOCIndexPrefs::addToC()
{
	ToCSetup newToCEntry;
	newToCEntry.name=QString("New Table of Contents %1").arg(localToCSetupVector.count()+1);
	newToCEntry.itemAttrName=none;
	newToCEntry.frameName=none;
	newToCEntry.style=none;
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
	{
		tocListBox->insertItem((*it).name);
	}
}


void TOCIndexPrefs::enableGUIWidgets()
{
	bool enabled=(localToCSetupVector.count()>0);
	tocListBox->setEnabled(enabled);
	tocDeleteButton->setEnabled(enabled);
	itemAttrComboBox->setEnabled(enabled);
	itemDestFrameComboBox->setEnabled(enabled);
	itemStyleComboBox->setEnabled(enabled);
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
		if (itemAttributeName==trNone)
			(*it).itemAttrName=none;
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
		if (frameName==trNone)
			(*it).frameName=none;
		else
			(*it).frameName=frameName;
	}

}


void TOCIndexPrefs::itemStyleSelected( const QString& itemStyle )
{
	int numberSelected=tocListBox->currentItem();
	if (numberSelected>=0)
	{
		int i=0;
		ToCSetupVector::Iterator it;
		for(it = localToCSetupVector.begin(); it!= localToCSetupVector.end(), i<numberSelected ; ++it, ++i)
			;
		if (itemStyle==trNone)
			(*it).style=none;
		else
			(*it).style=itemStyle;
	}

}


ToCSetupVector* TOCIndexPrefs::getNewToCs()
{
	return &localToCSetupVector;
}
