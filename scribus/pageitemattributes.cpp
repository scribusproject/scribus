/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pageitemattributes.h"

#include <qstring.h>
#include <qtable.h>
#include <qpushbutton.h>

PageItemAttributes::PageItemAttributes( QWidget* parent, const char* name, bool modal, WFlags fl )
	: PageItemAttributesBase(parent, name, modal, fl)
{
	relationships << tr("None") << tr("Relates To") << tr("Is Parent Of") << tr("Is Child Of");
	relationshipsData << "none" << "relation" << "parent" << "child";
}

PageItemAttributes::~PageItemAttributes()
{
}

void PageItemAttributes::setup(ObjAttrVector *pageItemAttrs, ObjAttrVector *docItemAttrs)
{
	localAttributes=*pageItemAttrs;
	localDocAttributes=*docItemAttrs;
		
	nameList.clear();
	nameList.append("");
	for(ObjAttrVector::Iterator it = localDocAttributes.begin(); it!= localDocAttributes.end(); ++it)
		nameList.append((*it).name);
	
	updateTable();
}

ObjAttrVector* PageItemAttributes::getNewAttributes()
{
	return &localAttributes;
}


void PageItemAttributes::tableItemChanged( int row, int col )
{
	switch (col)
	{
		case 0:
			localAttributes[row].name=attributesTable->text(row, col);
			break;
		case 1:
			localAttributes[row].type=attributesTable->text(row, col);
			break;
		case 2:
			localAttributes[row].value=attributesTable->text(row, col);
			break;
		case 3:
			localAttributes[row].parameter=attributesTable->text(row, col);
			break;
		case 4:
		{
			QComboTableItem* qcti=dynamic_cast<QComboTableItem*>(attributesTable->item(row,col));
			if (qcti!=NULL)
			{
				uint index=qcti->currentItem();
				if (index<relationshipsData.count())
					localAttributes[row].relationship=relationshipsData[index];
			}
		}
		break;
		case 5:
			localAttributes[row].relationshipto=attributesTable->text(row, col);
			break;
		case 6:
			//AutoAddTo is not used once this gets to the page items
			/*
			{
				QComboTableItem* qcti=dynamic_cast<QComboTableItem*>(attributesTable->item(row,col));
				if (qcti!=NULL)
				{
					uint index=qcti->currentItem();
					if (index<autoAddToData.count())
						localAttributes[row].autoaddto=autoAddToData[index];
				}
			}
			*/
			break;
		default:
			break;
	}
}


void PageItemAttributes::addEntry()
{
	ObjectAttribute blank;
	blank.name="";
	blank.relationship="None";
	blank.autoaddto="none";
	localAttributes.append(blank);
	updateTable();
}


void PageItemAttributes::deleteEntry()
{
	int currRow=attributesTable->currentRow();
	bool found=false;
	ObjAttrVector::Iterator it;
	int count=0;
	for(it = localAttributes.begin(); it!= localAttributes.end(); ++it)
	{
		if(count==currRow)
		{
			found=true;
			break;
		}
		++count;
	}
	if (found)
	{
		localAttributes.erase(it);
		updateTable();
	}
}


void PageItemAttributes::clearEntries()
{
	localAttributes.clear();
	updateTable();
}


void PageItemAttributes::copyEntry()
{
	int currRow=attributesTable->currentRow();
	bool found=false;
	ObjAttrVector::Iterator it;
	int count=0;
	for(it = localAttributes.begin(); it!= localAttributes.end(); ++it)
	{
		if(count==currRow)
		{
			found=true;
			break;
		}
		++count;
	}
	if (found)
	{
		localAttributes.append((*it));
		updateTable();
	}
}


void PageItemAttributes::updateTable()
{
	attributesTable->setNumRows(localAttributes.count());
	int row=0;
	for(ObjAttrVector::Iterator it = localAttributes.begin(); it!= localAttributes.end(); ++it)
	{
		uint i=0;
		//Name
		QComboTableItem *item1 = new QComboTableItem(attributesTable, nameList, true);
		if (nameList.contains((*it).name))
			item1->setCurrentItem((*it).name);
		else
			item1->setCurrentItem("");
		attributesTable->setItem(row, i++, item1);
		//Type
		QTableItem *item2 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).type);
		attributesTable->setItem(row, i++, item2);
		//Default Value
		QTableItem *item3 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).value);
		attributesTable->setItem(row, i++, item3);
		//Default Parameter
		QTableItem *item4 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).parameter);
		attributesTable->setItem(row, i++, item4);
		//Relationship
		QComboTableItem *item5 = new QComboTableItem(attributesTable, relationships);
		attributesTable->setItem(row, i++, item5);
		int index=relationshipsData.findIndex((*it).relationship);
		if (index==-1)
		{
			(*it).relationship="none";
			index=0;
		}
		item5->setCurrentItem(index);
		//Relationship to
		QTableItem *item6 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).relationshipto);
		attributesTable->setItem(row, i++, item6);
		//Auto Add to not used here
		/*
		QComboTableItem *item7 = new QComboTableItem(attributesTable, autoAddTo);
		attributesTable->setItem(row, i++, item7);
		index=autoAddToData.findIndex((*it).autoaddto);
		if (index==-1)
		{
			(*it).autoaddto="none";
			index=0;
		}
		item7->setCurrentItem(index);		
		*/
		
		attributesTable->verticalHeader()->setLabel(row, QString("%1").arg(row));
		row++;
	}
	deleteButton->setEnabled(localAttributes.count()!=0);
	copyButton->setEnabled(localAttributes.count()!=0);
	clearButton->setEnabled(localAttributes.count()!=0);
}


void PageItemAttributes::okClicked()
{
	//Qt hack as we will lose data if the user hasnt left a cell
	//http://www.qtforum.org/thread.php?threadid=9078
	if (attributesTable->numRows()>0 && attributesTable->currentRow()!=-1)
	{
		//Avoid selecting 0,0 if 0,0 is current cell
		int selectCol;
		if(attributesTable->currentColumn()!=0)
			selectCol=0;
		else
			selectCol=1;
			
		attributesTable->setCurrentCell(0,selectCol);
		tableItemChanged(attributesTable->currentRow(), attributesTable->currentColumn());
	}
	accept();
}

void PageItemAttributes::languageChange()
{
}

#include "pageitemattributes.moc"
