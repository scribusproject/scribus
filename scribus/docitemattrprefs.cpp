/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "docitemattrprefs.h"

#include <QString>
#include <QPushButton>
#include <QComboBox>

DocumentItemAttributes::DocumentItemAttributes(  QWidget* parent, Qt::WFlags fl  )
	: QWidget(parent, fl)
{
	setupUi(this);
	relationships << tr("None", "relationship") << tr("Relates To") << tr("Is Parent Of") << tr("Is Child Of");
	relationshipsData << "none" << "relation" << "parent" << "child";
	autoAddTo << tr("None", "auto add") << tr("Text Frames") << tr("Image Frames");
	autoAddToData << "none" << "textframes" << "imageframes";
	types << tr("None", "types") << tr("Boolean") << tr("Integer") << tr("Real Number") << tr("String");
	typesData << "none" << "boolean" << "integer" << "double" << "string";

	connect(attributesTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableItemChanged(int,int)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearEntries()));
	connect(copyButton, SIGNAL(clicked()), this, SLOT(copyEntry()));
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
		localAttributes[row].name=attributesTable->item(row, col)->text();
		break;
	case 1:
		{
			QComboBox* qcti=dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
			if (qcti!=NULL)
			{
				int index=qcti->currentIndex();
				if (index<typesData.count())
					localAttributes[row].type=typesData[index];
			}
		}
		break;
	case 2:
		localAttributes[row].value=attributesTable->item(row, col)->text();
		break;
	case 3:
		localAttributes[row].parameter=attributesTable->item(row, col)->text();
		break;
	case 4:
		{
			QComboBox* qcti=dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
			if (qcti!=NULL)
			{
				int index=qcti->currentIndex();
				if (index<relationshipsData.count())
					localAttributes[row].relationship=relationshipsData[index];
			}
		}
		break;
	case 5:
		localAttributes[row].relationshipto=attributesTable->item(row, col)->text();
		break;
	case 6:
		{
			QComboBox* qcti=dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
			if (qcti!=NULL)
			{
				int index=qcti->currentIndex();
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
	attributesTable->setRowCount(localAttributes.count());
	int row=0;
	for(ObjAttrVector::Iterator it = localAttributes.begin(); it!= localAttributes.end(); ++it)
	{
		uint i=0;
		//Name
		QTableWidgetItem *item1 = new QTableWidgetItem((*it).name);
		attributesTable->setItem(row, i++, item1);
		//Type
		QComboBox *item2 = new QComboBox();
		item2->addItems(types);
/* <<7636
		int listIndex=types.indexOf((*it).type);
		if (listIndex!=-1)
			item2->setCurrentIndex(listIndex);
		else
		{
			item2->setCurrentIndex(0);
			item2->setItemText(0,(*it).type);
		}
 >>7636 */
		int listIndex=typesData.indexOf((*it).type);
		if (listIndex==-1)
		{
			(*it).type="none";
			listIndex=0;
		}
		item2->setCurrentIndex(listIndex);

		item2->setEditable(true);
		attributesTable->setCellWidget(row, i++, item2);
		//Default Value
		QTableWidgetItem *item3 = new QTableWidgetItem((*it).value);
		attributesTable->setItem(row, i++, item3);
		//Default Parameter
		QTableWidgetItem *item4 = new QTableWidgetItem((*it).parameter);
		attributesTable->setItem(row, i++, item4);
		//Relationship
		QComboBox *item5 = new QComboBox();
		item5->addItems(relationships);
		attributesTable->setCellWidget(row, i++, item5);
		int index=relationshipsData.indexOf((*it).relationship);
		if (index==-1)
		{
			(*it).relationship="none";
			index=0;
		}
		item5->setCurrentIndex(index);
		//Relationship to
		QTableWidgetItem *item6 = new QTableWidgetItem((*it).relationshipto);
		attributesTable->setItem(row, i++, item6);
		//Auto Add to
		QComboBox *item7 = new QComboBox();
		item7->addItems(autoAddTo);
		attributesTable->setCellWidget(row, i++, item7);
		int index2=autoAddToData.indexOf((*it).autoaddto);
		if (index2==-1)
		{
			(*it).relationship="none";
			index2=0;
		}
		item7->setCurrentIndex(index2);

		QTableWidgetItem *t=attributesTable->verticalHeaderItem(row);
 		if (t!=NULL)
			t->setText(QString("%1").arg(row));
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
