/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pageitemattributes.h"
#include "commonstrings.h"

#include <QComboBox>
#include <QPushButton>
#include <QString>

#include "ui/sctablewidget.h"

PageItemAttributes::PageItemAttributes(QWidget* parent, bool modal) : QDialog(parent)
{
	setupUi(this);
	setModal(modal);

	attributesTable->setColumnWidth(0, 160);
	attributesTable->setColumnWidth(1, 130);
	attributesTable->setColumnWidth(2, 160);
	
	relationships << tr("None", "relationship") << tr("Relates To") << tr("Is Parent Of") << tr("Is Child Of");
	relationshipsData << "none" << "relation" << "parent" << "child";
	types << tr("None", "types") << tr("Boolean") << tr("Integer") << tr("Real Number") << tr("String");
	typesData << "none" << "boolean" << "integer" << "double" << "string";

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(attributesTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableItemChanged(int,int)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearEntries()));
	connect(copyButton, SIGNAL(clicked()), this, SLOT(copyEntry()));
}

void PageItemAttributes::setup(ObjAttrVector *pageItemAttrs, ObjAttrVector *docItemAttrs)
{
	localAttributes = *pageItemAttrs;
	localDocAttributes = *docItemAttrs;

	nameList.clear();
	nameList.append("");
	for (auto it = localDocAttributes.constBegin(); it != localDocAttributes.constEnd(); ++it)
		nameList.append(it->name);

	updateTable();
}

ObjAttrVector* PageItemAttributes::getNewAttributes()
{
	return &localAttributes;
}


void PageItemAttributes::tableItemChanged(int row, int col)
{
	switch (col)
	{
		case 0:
			{
				auto* qcti = dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
				if (qcti != nullptr)
					localAttributes[row].name = qcti->currentText();
			}
			break;
		case 1:
			{
				auto* qcti = dynamic_cast<QComboBox*>(attributesTable->cellWidget(row,col));
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
			auto* qcti = dynamic_cast<QComboBox*>(attributesTable->cellWidget(row, col));
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
			//AutoAddTo is not used once this gets to the page items
			/*
			{
				QComboTableItem* qcti = dynamic_cast<QComboTableItem*>(attributesTable->item(row, col));
				if (qcti != nullptr)
				{
					uint index = qcti->currentItem();
					if (index<autoAddToData.count())
						localAttributes[row].autoaddto = autoAddToData[index];
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
	blank.name = "";
	blank.relationship = CommonStrings::None;
	blank.autoaddto = "none";
	localAttributes.append(blank);
	updateTable();
}


void PageItemAttributes::deleteEntry()
{
	int currRow = attributesTable->currentRow();
	bool found = false;
	ObjAttrVector::Iterator it;
	int count = 0;
	for (it = localAttributes.begin(); it != localAttributes.end(); ++it)
	{
		if (count == currRow)
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


void PageItemAttributes::clearEntries()
{
	localAttributes.clear();
	updateTable();
}


void PageItemAttributes::copyEntry()
{
	int currRow = attributesTable->currentRow();
	bool found = false;
	ObjAttrVector::Iterator it;
	int count = 0;
	for (it = localAttributes.begin(); it != localAttributes.end(); ++it)
	{
		if (count == currRow)
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


void PageItemAttributes::updateTable()
{
	attributesTable->setRowCount(localAttributes.count());
	for (int row = 0; row < localAttributes.count(); ++row)
	{
		uint col = 0;
		ObjectAttribute& objAttr = localAttributes[row];

		//Name
		QComboBox *item1 = new QComboBox();
		item1->addItems(nameList);
		int listIndex = nameList.indexOf(objAttr.name);
		if (listIndex != -1)
			item1->setCurrentIndex(listIndex);
		else
		{
			item1->setCurrentIndex(0);
			item1->setItemText(0, objAttr.name);
		}
		item1->setEditable(true);
		attributesTable->setCellWidget(row, col++, item1);
		//Type
		QComboBox *item2 = new QComboBox();
		item2->addItems(types);

		listIndex = typesData.indexOf(objAttr.type);
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
		//Auto Add to not used here
		/*
		QComboBox *item7 = new QComboBox();
		item7->addItems(autoAddTo);
		attributesTable->setCellWidget(row, col++, item7);
		int index2 = autoAddToData.indexOf(objAttr.autoaddto);
		if (index2==-1)
		{
			objAttr.relationship="none";
			index2 = 0;
		}
		item7->setCurrentItem(index2);
		*/
		QTableWidgetItem* t = attributesTable->verticalHeaderItem(row);
		if (t != nullptr)
			t->setText(QString("%1").arg(row));
	}
	deleteButton->setEnabled(localAttributes.count() != 0);
	copyButton->setEnabled(localAttributes.count() != 0);
	clearButton->setEnabled(localAttributes.count() != 0);
}


void PageItemAttributes::okClicked()
{
	//Qt hack as we will lose data if the user hasn't left a cell
	//http://www.qtforum.org/thread.php?threadid=9078
	if (attributesTable->rowCount() > 0 && attributesTable->currentRow() != -1)
	{
		//Avoid selecting 0,0 if 0,0 is current cell
		int selectCol;
		if (attributesTable->currentColumn() != 0)
			selectCol = 0;
		else
			selectCol = 1;

		attributesTable->setCurrentCell(0, selectCol);
		tableItemChanged(attributesTable->currentRow(), attributesTable->currentColumn());
	}
	accept();
}

void PageItemAttributes::languageChange()
{
}
