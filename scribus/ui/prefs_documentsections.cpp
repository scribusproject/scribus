/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>

#include "prefs_documentsections.h"
#include "commonstrings.h"
#include "prefsstructs.h"
#include "scribusdoc.h"
#include "ui/scmessagebox.h"

Prefs_DocumentSections::Prefs_DocumentSections(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	m_doc(doc), m_maxPageIndex(0)
{
	setupUi(this);
	languageChange();
	connect(sectionsTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableItemChanged(int,int)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
}

Prefs_DocumentSections::~Prefs_DocumentSections()
{
}

void Prefs_DocumentSections::languageChange()
{
}

void Prefs_DocumentSections::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	m_localSections=prefsData->docSectionMap;
	m_maxPageIndex=m_doc->DocPages.count()-1;
	m_styles.clear();
	m_styles = getFormatList();
	m_styles << CommonStrings::tr_None;
//	styles << tr("1, 2, 3, ...") << tr("i, ii, iii, ...") << tr("I, II, III, ...") << tr("a, b, c, ...") << tr("A, B, C, ...") << tr("*") << tr("CJK") << CommonStrings::tr_None;

	updateTable();
}

void Prefs_DocumentSections::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->docSectionMap=m_localSections;
}

void Prefs_DocumentSections::updateTable()
{
	sectionsTable->setRowCount(m_localSections.count());
	int row=0;
	for(DocumentSectionMap::Iterator it = m_localSections.begin(); it!= m_localSections.end(); ++it)
	{
		uint i=0;
		//Name
		QTableWidgetItem *item0 = new QTableWidgetItem((*it).name);
		sectionsTable->setItem(row, i++, item0);
		//Active
		QTableWidgetItem *item1 = new QTableWidgetItem();;
		item1->setCheckState((*it).active ? Qt::Checked : Qt::Unchecked);
		sectionsTable->setItem(row, i++, item1);
		//Reversed
		QTableWidgetItem *item2 = new QTableWidgetItem();
		item2->setCheckState((*it).reversed ? Qt::Checked : Qt::Unchecked);
		sectionsTable->setItem(row, i++, item2);
		//FromIndex
		QTableWidgetItem *item3 = new QTableWidgetItem(QString::number((*it).fromindex+1));
		sectionsTable->setItem(row, i++, item3);
		//ToIndex
		QTableWidgetItem *item4 = new QTableWidgetItem(QString::number((*it).toindex+1));
		sectionsTable->setItem(row, i++, item4);
		//Style
		QComboBox *item5 = new QComboBox();
		item5->addItems(m_styles);
		sectionsTable->setCellWidget(row, i++, item5);
		if ((*it).type==Type_None)
			item5->setCurrentIndex(m_styles.count()-1);
		else
			item5->setCurrentIndex((*it).type);
		//Start Page Number
		QTableWidgetItem *item6 = new QTableWidgetItem(QString::number((*it).sectionstartindex));
		sectionsTable->setItem(row, i++, item6);
		//End Page Number
		/*
		QTableItem *item7 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).sectionstartindex + (*it).toindex - (*it).fromindex));
		item7->setEnabled(false);
		sectionsTable->setItem(row, i++, item7);
		*/
		//Field Width
		QTableWidgetItem *item7 = new QTableWidgetItem(QString::number((*it).pageNumberWidth));
		sectionsTable->setItem(row, i++, item7);
		//Fill Char
		QTableWidgetItem *item8 = new QTableWidgetItem(QString((*it).pageNumberFillChar));
		sectionsTable->setItem(row, i++, item8);
		//
		QTableWidgetItem *t=sectionsTable->verticalHeaderItem(row);
		if (t!=NULL)
			t->setText(QString("%1").arg(row));
		row++;
	}
	deleteButton->setEnabled(m_localSections.count()>1);
}

void Prefs_DocumentSections::tableItemChanged( int row, int col )
{
	bool outOfRange=false;
	uint newDocPageSpec;
	switch (col)
	{
	case 0:
		m_localSections[row].name=sectionsTable->item(row, col)->text();
		break;
	case 1:
		m_localSections[row].active=(sectionsTable->item(row, col)->checkState()==Qt::Checked);
		break;
	case 2:
		m_localSections[row].reversed=(sectionsTable->item(row, col)->checkState()==Qt::Checked);
		break;
	case 3:
	case 4:
		// Validate to/from page specification before conversion to an index
		//!!!	There is still a problem here if m_maxPageIndex == MAX_UINT ;)
		newDocPageSpec=sectionsTable->item(row, col)->text().toUInt();
		if (newDocPageSpec==0)
		{
			newDocPageSpec=1;
			outOfRange=true;
		}
		else
		if (newDocPageSpec>m_maxPageIndex+1)
		{
			newDocPageSpec=m_maxPageIndex+1;
			outOfRange=true;
		}
		// Now, since newDocPageSpec >= 1, convert to index
		--newDocPageSpec;
		if (col==3)
			m_localSections[row].fromindex=newDocPageSpec;
		else
			m_localSections[row].toindex=newDocPageSpec;
		break;
	case 5:
		{
			QComboBox* qcti=dynamic_cast<QComboBox*>(sectionsTable->cellWidget(row,col));
			if (qcti!=NULL)
			{
				int index=qcti->currentIndex();
				if (index<m_styles.count()-1)
					m_localSections[row].type=(NumFormat)index;
				else
					if (index==m_styles.count()-1)
						m_localSections[row].type=Type_None;
			}
		}
		break;
	case 6:
		m_localSections[row].sectionstartindex = sectionsTable->item(row, col)->text().toUInt();;
		break;
	case 7:
		m_localSections[row].pageNumberWidth = sectionsTable->item(row, col)->text().toInt();
		break;
	case 8:
		{
			QString ch=sectionsTable->item(row, col)->text();
			if (ch.length()>0)
				m_localSections[row].pageNumberFillChar=sectionsTable->item(row, col)->text().at(0);
			else
				m_localSections[row].pageNumberFillChar=QChar();
		}
		break;
	default:
		break;
	}

	if (outOfRange)
	{
		updateTable();
		ScMessageBox::warning(parentWidget(), tr("Page Number Out Of Bounds"),"<qt>"+ tr("The value you have entered is outside the range of page numbers in the current document (%1-%2).").arg(1).arg(m_maxPageIndex+1)+"</qt>");
	}
}

void Prefs_DocumentSections::addEntry()
{
	int currRow=sectionsTable->currentRow();
	bool found=false;
	DocumentSectionMap::Iterator it = m_localSections.begin();
	int count=0;
	for(; it!= m_localSections.end(); ++it)
	{
		if(count==currRow)
		{
			found=true;
			break;
		}
		++count;
	}
	if (!found) //End of map, just append
	{
		struct DocumentSection blank;
		uint count=m_localSections.count();
		blank.number=count;
		blank.name=QString::number(count);
		blank.fromindex=m_maxPageIndex+1;
		blank.toindex=m_maxPageIndex+1;
		blank.type=Type_1_2_3;
		blank.sectionstartindex=1;
		blank.reversed=false;
		blank.active=true;
		m_localSections.insert(count, blank);
	}
	else
	{
		//Now, copy to a temp map
		DocumentSectionMap tempSections(m_localSections);
		m_localSections.clear();
		//Copy the temp map entries over. When we find the number of the current row, also insert a new entry.
		uint i=0;
		for(DocumentSectionMap::Iterator it2 = tempSections.begin(); it2!= tempSections.end(); ++it2)
		{
			it2.value().number=i;
			m_localSections.insert(i, it2.value());

			if ((*it).number==i)
			{
				struct DocumentSection blank;
				blank.number=++i;
				blank.name=QString::number(i);
				blank.fromindex=(*it).toindex+1+1;
				blank.toindex=(*it).toindex+2+1;
				blank.type=Type_1_2_3;
				blank.sectionstartindex=1;
				blank.reversed=false;
				blank.active=true;
				m_localSections.insert(i, blank);
			}
			++i;
		}
	}
	updateTable();
}

void Prefs_DocumentSections::deleteEntry()
{
	int currRow=sectionsTable->currentRow();
	if (currRow==0 && m_localSections.count()==1)
		return;
	bool found=false;
	DocumentSectionMap::Iterator it = m_localSections.begin();
	int count=0;
	for(; it!= m_localSections.end(); ++it)
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
		//If we arent at the start, copy the toindex of the current item
		//to the toindex of the previous item
		if (it!=m_localSections.begin())
		{
			DocumentSectionMap::Iterator it2(it);
			(*--it2).toindex=(*it).toindex;
		}
		//Delete the currently selected entry
		m_localSections.erase(it);
		//Now, copy to a temp map and reinsert with consecutive keys again
		DocumentSectionMap tempSections(m_localSections);
		m_localSections.clear();
		uint i=0;
		it = tempSections.begin();
		for(; it!= tempSections.end(); ++it)
		{
			it.value().number=i;
			m_localSections.insert(i++, it.value());
		}
		int newCount=m_localSections.count();
		//int preIndex=qMax(currentIndex-1, 0);
		m_localSections[0].fromindex=0;
		m_localSections[newCount-1].toindex = m_maxPageIndex;
		updateTable();
	}
}
