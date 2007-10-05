/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "createrange.h"

#include "usertaskstructs.h"

#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QString>
#include <QTabWidget>

CreateRange::CreateRange(QString currText, int pageCount, QWidget* parent, Qt::WFlags fl )
	: QDialog(parent, fl),
	m_PageCount(pageCount),
	m_RangeType(0),
	m_BasicRangeType(0)
{
	setupUi(this);
	pageCountValueLabel->setText(QString("%1").arg(pageCount));
	basicConsecutiveFromSpinBox->setMinimum(1);
	basicConsecutiveToSpinBox->setMinimum(1);
	basicConsecutiveFromSpinBox->setMaximum(pageCount);
	basicConsecutiveToSpinBox->setMaximum(pageCount);
	basicSelectRangeType(m_BasicRangeType);
	advPageGroupSizeSpinBox->setMaximum(pageCount);
	if (m_PageCount==1)
		basicEvenRadioButton->setShown(false);
	if (currText.length()>0)
		basicRangeListBox->addItem(currText);
	// signals and slots connections
	connect(tabWidget, SIGNAL(currentChanged(QWidget*)), this, SLOT(selectRangeType(QWidget*)));
	connect(basicRangeAddButton, SIGNAL(clicked()), this, SLOT(basicAddToRange()));
	connect(basicRangeDelButton, SIGNAL(clicked()), this, SLOT(basicDelFromRange()));
	connect(basicConsecutiveRadioButton, SIGNAL(clicked()), this, SLOT(basicSelectRangeTypeConsec()));
	connect(basicCommaSepRadioButton, SIGNAL(clicked()), this, SLOT(basicSelectRangeTypeComma()));
	connect(basicEvenRadioButton, SIGNAL(clicked()), this, SLOT(basicSelectRangeTypeEven()));
	connect(basicOddRadioButton, SIGNAL(clicked()), this, SLOT(basicSelectRangeTypeOdd()));
	connect(basicRangeUpButton, SIGNAL(clicked()), this, SLOT(basicMoveUp()));
	connect(basicRangeDownButton, SIGNAL(clicked()), this, SLOT(basicMoveDown()));
	connect(advPageGroupSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(advSpinChange(int)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	advPageGroupSizeSpinBox->setValue(4);
}

CreateRange::~CreateRange()
{
}

void CreateRange::getCreateRangeData(CreateRangeData& crData)
{
	crData.pageRange="";
	//First tab selected
	if (m_RangeType==0)
	{
		uint c=basicRangeListBox->count();
		if (c==0)
		{
			basicAddToRange();
			c=basicRangeListBox->count();
		}
		for (uint i=0;i<c;++i)
		{
			if (i!=0 && i<c)
				crData.pageRange+=",";
			crData.pageRange+=basicRangeListBox->item(i)->text();
		}
	}
	else
		crData.pageRange=m_PageString;
}

void CreateRange::basicAddToRange( )
{
	QString newEntry;
	switch (m_BasicRangeType)
	{
		case 0:
			{
				int from=basicConsecutiveFromSpinBox->value();
				int to=basicConsecutiveToSpinBox->value();
				if (from==to)
					newEntry=QString("%1").arg(from);
				else
					newEntry=QString("%1-%2").arg(from).arg(to);
			}
			break;
		case 1:
			newEntry=basicCommaSepLineEdit->text();
			break;
		case 2:
			{
				QString numbers;
				for (int i=2; i<=m_PageCount; i+=2)
				{
					if (i!=2)
						numbers+=",";
					numbers+=QString("%1").arg(i);
				}
				newEntry=numbers;
			}
			break;
		case 3:
			{
				QString numbers;
				for (int i=1; i<=m_PageCount; i+=2)
				{
					if (i!=1)
						numbers+=",";
					numbers+=QString("%1").arg(i);
				}
				newEntry=numbers;
			}
			break;
	}
	if (newEntry.length()!=0)
		basicRangeListBox->addItem(newEntry);
}

void CreateRange::basicDelFromRange()
{
	delete basicRangeListBox->takeItem(basicRangeListBox->currentRow());
}

void CreateRange::basicSelectRangeTypeConsec()
{
	basicSelectRangeType(0);
}

void CreateRange::basicSelectRangeTypeComma()
{
	basicSelectRangeType(1);
}

void CreateRange::basicSelectRangeTypeEven()
{
	basicSelectRangeType(2);
}

void CreateRange::basicSelectRangeTypeOdd()
{
	basicSelectRangeType(3);
}

void CreateRange::basicSelectRangeType(int i)
{
	m_BasicRangeType=i;
	bool basicRangeTypeIsConsecutive=(i==0);
	basicConsecutiveFromLabel->setEnabled(basicRangeTypeIsConsecutive);
	basicConsecutiveFromSpinBox->setEnabled(basicRangeTypeIsConsecutive);
	basicConsecutiveToLabel->setEnabled(basicRangeTypeIsConsecutive);
	basicConsecutiveToSpinBox->setEnabled(basicRangeTypeIsConsecutive);
	basicCommaSepLineEdit->setEnabled(!basicRangeTypeIsConsecutive);
}

void CreateRange::selectRangeType(QWidget *)
{
	m_RangeType=tabWidget->currentIndex();
}


void CreateRange::basicMoveUp()
{
	int index=basicRangeListBox->currentRow();
	if (index==-1 || index==0)
		return;
	basicRangeListBox->clearSelection();
	QListWidgetItem * clbi = basicRangeListBox->takeItem(index);
	basicRangeListBox->insertItem(qMax(0, index-1), clbi);
	basicRangeListBox->setCurrentItem(clbi);
}

void CreateRange::basicMoveDown()
{
	int index=basicRangeListBox->currentRow();
	if (index==-1 || index==static_cast<int>(basicRangeListBox->count())-1)
		return;
	basicRangeListBox->clearSelection();
	QListWidgetItem * clbi = basicRangeListBox->takeItem(index);
	basicRangeListBox->insertItem(index+1, clbi);
	basicRangeListBox->setCurrentItem(clbi);
}


void CreateRange::advSpinChange(int /*v*/)
{
	m_PageString="";
	int mp1=m_PageCount+1;
	//locked at 4 for now.
	if (m_PageCount % 4 == 0)
	{
	//28,1,2,27, 26,3,4,25
		for (int i=m_PageCount;i>m_PageCount/2;i-=2)
		{
			if (i!=m_PageCount)
				m_PageString+=",";
			m_PageString+=QString("%1").arg(i);
			m_PageString+=QString(",%1").arg(mp1-i);
			m_PageString+=QString(",%1").arg(mp1-i+1);
			m_PageString+=QString(",%1").arg(i-1);
		}
	}
	QString tmp(m_PageString);
	tmp.truncate(20);
	advSampleOrderExampleLabel->setText(tmp+"...");
}
