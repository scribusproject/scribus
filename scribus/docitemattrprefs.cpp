/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "docitemattrprefs.h"
//#include "docitemattrprefs.moc"

#include <qstring.h>
#include <q3table.h>
#include <qpushbutton.h>

DocumentItemAttributes::DocumentItemAttributes(  QWidget* parent, const char* name, Qt::WFlags fl  )
	: QWidget(parent, name, fl)
{
	setupUi(this);
	relationships << tr("None", "relationship") << tr("Relates To") << tr("Is Parent Of") << tr("Is Child Of");
	relationshipsData << "none" << "relation" << "parent" << "child";
	autoAddTo << tr("None", "auto add") << tr("Text Frames") << tr("Image Frames");
	autoAddToData << "none" << "textframes" << "imageframes";
	types << tr("None", "types") << tr("Boolean") << tr("Integer") << tr("Real Number") << tr("String");
	typesData << "none" << "boolean" << "integer" << "double" << "string";
}

DocumentItemAttributes::~DocumentItemAttributes()
{
}

void DocumentItemAttributes::setup(ObjAttrVector *docItemAttrs)
{
	localAttributes=*docItemAttrs;
	updateTable();
}

ObjAttrVector* DocumentItemAttributes::getNewAttributes()
{
	return &localAttributes;
}


void DocumentItemAttributes::tableItemChanged( int row, int col )
{
	switch (col)
	{
	case 0:
		localAttributes[row].name=attributesTable->text(row, col);
		break;
	case 1:
		{
			Q3ComboTableItem* qcti=dynamic_cast<Q3ComboTableItem*>(attributesTable->item(row,col));
			if (qcti!=NULL)
			{
				int index=qcti->currentItem();
				if (index<typesData.count())
					localAttributes[row].type=typesData[index];
			}
		}
		break;
	case 2:
		localAttributes[row].value=attributesTable->text(row, col);
		break;
	case 3:
		localAttributes[row].parameter=attributesTable->text(row, col);
		break;
	case 4:
		{
			Q3ComboTableItem* qcti=dynamic_cast<Q3ComboTableItem*>(attributesTable->item(row,col));
			if (qcti!=NULL)
			{
				int index=qcti->currentItem();
				if (index<relationshipsData.count())
					localAttributes[row].relationship=relationshipsData[index];
			}
		}
		break;
	case 5:
		localAttributes[row].relationshipto=attributesTable->text(row, col);
		break;
	case 6:
		{
			Q3ComboTableItem* qcti=dynamic_cast<Q3ComboTableItem*>(attributesTable->item(row,col));
			if (qcti!=NULL)
			{
				int index=qcti->currentItem();
				if (index<autoAddToData.count())
					localAttributes[row].autoaddto=autoAddToData[index];
			}
		}
		break;
	default:
		break;
	}
}


void DocumentItemAttributes::addEntry()
{
	ObjectAttribute blank;
	blank.relationship="none";
	blank.autoaddto="none";
	blank.type="none";
	localAttributes.append(blank);
	updateTable();
}


void DocumentItemAttributes::updateTable()
{
	attributesTable->setNumRows(localAttributes.count());
	int row=0;
	for(ObjAttrVector::Iterator it = localAttributes.begin(); it!= localAttributes.end(); ++it)
	{
		uint i=0;
		//Name
		Q3TableItem *item1 = new Q3TableItem(attributesTable, Q3TableItem::WhenCurrent, (*it).name);
		attributesTable->setItem(row, i++, item1);
		//Type
		Q3ComboTableItem *item2 = new Q3ComboTableItem(attributesTable, types);
		attributesTable->setItem(row, i++, item2);
		int index=typesData.findIndex((*it).type);
		if (index==-1)
		{
			(*it).type="none";
			index=0;
		}
		item2->setCurrentItem(index);
		//Default Value
		Q3TableItem *item3 = new Q3TableItem(attributesTable, Q3TableItem::WhenCurrent, (*it).value);
		attributesTable->setItem(row, i++, item3);
		//Default Parameter
		Q3TableItem *item4 = new Q3TableItem(attributesTable, Q3TableItem::WhenCurrent, (*it).parameter);
		attributesTable->setItem(row, i++, item4);
		//Relationship
		Q3ComboTableItem *item5 = new Q3ComboTableItem(attributesTable, relationships);
		attributesTable->setItem(row, i++, item5);
		index=relationshipsData.findIndex((*it).relationship);
		if (index==-1)
		{
			(*it).relationship="none";
			index=0;
		}
		item5->setCurrentItem(index);
		//Relationship to
		Q3TableItem *item6 = new Q3TableItem(attributesTable, Q3TableItem::WhenCurrent, (*it).relationshipto);
		attributesTable->setItem(row, i++, item6);
		//Auto Add to
		Q3ComboTableItem *item7 = new Q3ComboTableItem(attributesTable, autoAddTo);
		attributesTable->setItem(row, i++, item7);
		index=autoAddToData.findIndex((*it).autoaddto);
		if (index==-1)
		{
			(*it).autoaddto="none";
			index=0;
		}
		item7->setCurrentItem(index);

		attributesTable->verticalHeader()->setLabel(row, QString("%1").arg(row));
		row++;
	}
	deleteButton->setEnabled(localAttributes.count()!=0);
	copyButton->setEnabled(localAttributes.count()!=0);
	clearButton->setEnabled(localAttributes.count()!=0);
}


void DocumentItemAttributes::deleteEntry()
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


void DocumentItemAttributes::clearEntries()
{
	localAttributes.clear();
	updateTable();
}


void DocumentItemAttributes::copyEntry()
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

QStringList DocumentItemAttributes::getDocAttributesNames()
{
	QStringList nameList;
	for(ObjAttrVector::Iterator it = localAttributes.begin(); it!= localAttributes.end(); ++it)
		nameList.append((*it).name);
	return nameList;
}
