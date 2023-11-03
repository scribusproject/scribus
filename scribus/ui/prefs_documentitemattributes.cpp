/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QComboBox>

#include "ui/prefs_documentitemattributes.h"
#include "prefsstructs.h"

Prefs_DocumentItemAttributes::Prefs_DocumentItemAttributes(QWidget* parent, ScribusDoc* /*doc*/)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Document Item Attributes");
	m_icon = "docattributes_16.png";

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

void Prefs_DocumentItemAttributes::languageChange()
{

}

void Prefs_DocumentItemAttributes::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	localAttributes = prefsData->itemAttrPrefs.defaultItemAttributes;
	updateTable();
}

void Prefs_DocumentItemAttributes::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->itemAttrPrefs.defaultItemAttributes = localAttributes;
}

void Prefs_DocumentItemAttributes::tableItemChanged( int row, int col )
{
	switch (col)
	{
	case 0:
		localAttributes[row].name = attributesTable->item(row, col)->text();
		break;
	case 1:
		{
			QComboBox* qcti = dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
			if (qcti != nullptr)
			{
				int index = qcti->currentIndex();
				if (index < typesData.count())
					localAttributes[row].type = typesData[index];
			}
		}
		break;
	case 2:
		localAttributes[row].value = attributesTable->item(row, col)->text();
		break;
	case 3:
		localAttributes[row].parameter = attributesTable->item(row, col)->text();
		break;
	case 4:
		{
			QComboBox* qcti = dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
			if (qcti != nullptr)
			{
				int index = qcti->currentIndex();
				if (index < relationshipsData.count())
					localAttributes[row].relationship = relationshipsData[index];
			}
		}
		break;
	case 5:
		localAttributes[row].relationshipto = attributesTable->item(row, col)->text();
		break;
	case 6:
		{
			QComboBox* qcti = dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
			if (qcti != nullptr)
			{
				int index = qcti->currentIndex();
				if (index < autoAddToData.count())
					localAttributes[row].autoaddto = autoAddToData[index];
			}
		}
		break;
	default:
		break;
	}
}


void Prefs_DocumentItemAttributes::addEntry()
{
	ObjectAttribute blank;
	blank.relationship = "none";
	blank.autoaddto = "none";
	blank.type = "none";
	localAttributes.append(blank);
	updateTable();
}


void Prefs_DocumentItemAttributes::updateTable()
{
	attributesTable->setRowCount(localAttributes.count());

	for (int row = 0; row < localAttributes.count(); ++row)
	{
		uint col = 0;
		ObjectAttribute& objAttr = localAttributes[row];

		//Name
		QTableWidgetItem *item1 = new QTableWidgetItem(objAttr.name);
		attributesTable->setItem(row, col++, item1);
		//Type
		QComboBox *item2 = new QComboBox();
		item2->addItems(types);

		int listIndex = typesData.indexOf(objAttr.type);
		if (listIndex == -1)
		{
			objAttr.type = "none";
			listIndex = 0;
		}
		item2->setCurrentIndex(listIndex);

		item2->setEditable(true);
		attributesTable->setCellWidget(row, col++, item2);
		//Default Value
		QTableWidgetItem *item3 = new QTableWidgetItem(objAttr.value);
		attributesTable->setItem(row, col++, item3);
		//Default Parameter
		QTableWidgetItem *item4 = new QTableWidgetItem(objAttr.parameter);
		attributesTable->setItem(row, col++, item4);
		//Relationship
		QComboBox *item5 = new QComboBox();
		item5->addItems(relationships);
		attributesTable->setCellWidget(row, col++, item5);
		int index = relationshipsData.indexOf(objAttr.relationship);
		if (index == -1)
		{
			objAttr.relationship = "none";
			index = 0;
		}
		item5->setCurrentIndex(index);
		//Relationship to
		QTableWidgetItem *item6 = new QTableWidgetItem(objAttr.relationshipto);
		attributesTable->setItem(row, col++, item6);
		//Auto Add to
		QComboBox *item7 = new QComboBox();
		item7->addItems(autoAddTo);
		attributesTable->setCellWidget(row, col++, item7);
		int index2 = autoAddToData.indexOf(objAttr.autoaddto);
		if (index2 == -1)
		{
			objAttr.relationship = "none";
			index2 = 0;
		}
		item7->setCurrentIndex(index2);

		QTableWidgetItem *t = attributesTable->verticalHeaderItem(row);
		if (t != nullptr)
			t->setText(QString("%1").arg(row));
	}
	deleteButton->setEnabled(localAttributes.count()!=0);
	copyButton->setEnabled(localAttributes.count()!=0);
	clearButton->setEnabled(localAttributes.count()!=0);
}


void Prefs_DocumentItemAttributes::deleteEntry()
{
	int currRow = attributesTable->currentRow();
	bool found = false;
	ObjAttrVector::Iterator it;
	int count = 0;
	for (it = localAttributes.begin(); it!= localAttributes.end(); ++it)
	{
		if(count == currRow)
		{
			found = true;
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


void Prefs_DocumentItemAttributes::clearEntries()
{
	localAttributes.clear();
	updateTable();
}


void Prefs_DocumentItemAttributes::copyEntry()
{
	int currRow = attributesTable->currentRow();
	bool found = false;
	ObjAttrVector::Iterator it;
	int count = 0;
	for (it = localAttributes.begin(); it != localAttributes.end(); ++it)
	{
		if(count == currRow)
		{
			found = true;
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

QStringList Prefs_DocumentItemAttributes::getDocAttributesNames()
{
	QStringList nameList;
	for (ObjAttrVector::Iterator it = localAttributes.begin(); it!= localAttributes.end(); ++it)
		nameList.append((*it).name);
	return nameList;
}
