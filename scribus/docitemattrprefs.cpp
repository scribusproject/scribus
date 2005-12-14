#include "docitemattrprefs.h"

#include <qstring.h>
#include <qtable.h>
#include <qpushbutton.h>

DocumentItemAttributes::DocumentItemAttributes(QWidget* parent)
	: DocumentItemAttributesBase(parent)
{
	relationships << tr("None") << tr("Relates To") << tr("Is Parent Of") << tr("Is Child Of");
	relationshipsData << "none" << "relation" << "parent" << "child";
	autoAddTo << tr("None") << tr("Text Frames") << tr("Image Frames");
	autoAddToData << "none" << "textframes" << "imageframes";
	types << tr("None") << tr("Boolean") << tr("Integer") << "Real Number" << tr("String");
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
			QComboTableItem* qcti=dynamic_cast<QComboTableItem*>(attributesTable->item(row,col));
			if (qcti!=NULL)
			{
				uint index=qcti->currentItem();
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
		{
			QComboTableItem* qcti=dynamic_cast<QComboTableItem*>(attributesTable->item(row,col));
			if (qcti!=NULL)
			{
				uint index=qcti->currentItem();
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
		QTableItem *item1 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).name);
		attributesTable->setItem(row, i++, item1);
		//Type
		QComboTableItem *item2 = new QComboTableItem(attributesTable, types);
		attributesTable->setItem(row, i++, item2);
		int index=typesData.findIndex((*it).type);
		if (index==-1)
		{
			(*it).type="none";
			index=0;
		}
		item2->setCurrentItem(index);
		//Default Value
		QTableItem *item3 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).value);
		attributesTable->setItem(row, i++, item3);
		//Default Parameter
		QTableItem *item4 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).parameter);
		attributesTable->setItem(row, i++, item4);
		//Relationship
		QComboTableItem *item5 = new QComboTableItem(attributesTable, relationships);
		attributesTable->setItem(row, i++, item5);
		index=relationshipsData.findIndex((*it).relationship);
		if (index==-1)
		{
			(*it).relationship="none";
			index=0;
		}
		item5->setCurrentItem(index);
		//Relationship to
		QTableItem *item6 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).relationshipto);
		attributesTable->setItem(row, i++, item6);
		//Auto Add to
		QComboTableItem *item7 = new QComboTableItem(attributesTable, autoAddTo);
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
