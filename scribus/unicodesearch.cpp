/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFile>
#include <QHeaderView>
 
#include "unicodesearch.h"
#include "scpaths.h"


UnicodeChooseButton::UnicodeChooseButton(QWidget * parent)
	: QPushButton(parent),
	m_searchDialog(0)
{
	languageChange();
	setCheckable(true);

	m_searchDialog = new UnicodeSearch(this);
	Q_CHECK_PTR(m_searchDialog);

	connect(this, SIGNAL(toggled(bool)), this, SLOT(self_toggled(bool)));
	connect(m_searchDialog, SIGNAL(setVisibleState(bool)), this, SLOT(setChecked(bool)));
	// listview user inputs
	connect(m_searchDialog->unicodeList, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
			this, SLOT(unicodeList_chosen(QTableWidgetItem *)));
	connect(m_searchDialog->unicodeList, SIGNAL(cellDoubleClicked(int, int)),
			this, SLOT(unicodeList_chosen(int, int)));
}

void UnicodeChooseButton::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void UnicodeChooseButton::languageChange()
{
// 	setText(tr("&Search"));
}


void UnicodeChooseButton::unicodeList_chosen(QTableWidgetItem *item)
{
	int r=item->row();
	QTableWidgetItem *item2 = m_searchDialog->unicodeList->item(r,0);
	emit chosenUnicode(item2->text());
	emit toggled(false);
}

void UnicodeChooseButton::unicodeList_chosen(int row, int column)
{
	unicodeList_chosen(m_searchDialog->unicodeList->item(row, 0));
}

void UnicodeChooseButton::self_toggled(bool state)
{
	if (state)
	{
		m_searchDialog->move(mapToGlobal(rect().bottomLeft()));
		m_searchDialog->show();
		m_searchDialog->checkForUpdate();
	}
	else
		m_searchDialog->hide();
}

UnicodeSearch::UnicodeSearch( QWidget* parent)
	: QDialog( parent),
	m_zoom(0)
{
	setupUi(this);

	unicodeList->horizontalHeader()->hide();
	unicodeList->verticalHeader()->hide();

	connect(searchEdit, SIGNAL(returnPressed()), this, SLOT(searchEdit_returnPressed()));
}

void UnicodeSearch::checkForUpdate()
{
	if (m_unicodeMap.count()==0)
	{
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
		readUnicodeMap();
		query();
		qApp->restoreOverrideCursor();
	}
}

void UnicodeSearch::readUnicodeMap()
{
	m_unicodeMap.clear();

	QFile file(ScPaths::instance().shareDir() + "unicodenameslist.txt");
	if (file.open( QIODevice::ReadOnly ) )
	{
		QStringList list = QString(file.readAll()).split('\n', QString::SkipEmptyParts);
		file.close();

		QStringList line;
		for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
		{
			line = (*it).split(':', QString::SkipEmptyParts);
			m_unicodeMap[line[0]] = line[1].toLower();
		}
	}
	else
		qDebug("UnicodeSearch: error reading unicodes!");
}

void UnicodeSearch::query()
{
	unicodeList->clear();
	unicodeList->setColumnCount(2);
	unicodeList->setRowCount(m_unicodeMap.count());
	QMap<QString,QString>::Iterator it;
	int r=0;
	for (it = m_unicodeMap.begin(); it != m_unicodeMap.end(); ++it)
	{
		QTableWidgetItem *item = new QTableWidgetItem(it.key());
		QTableWidgetItem *item2 = new QTableWidgetItem(it.value());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		unicodeList->setItem(r,0,item);
		unicodeList->setItem(r,1,item2);
		delete unicodeList->takeVerticalHeaderItem(r);
		++r;
	}
	unicodeList->resizeColumnsToContents();
}

void UnicodeSearch::query(QString filter)
{
	if (filter.isNull())
		return;

	unicodeList->clear();
	unicodeList->setColumnCount(2);
	unicodeList->setRowCount(m_unicodeMap.count());
	QMap<QString,QString>::Iterator it;
	int r=0;
	for (it = m_unicodeMap.begin(); it != m_unicodeMap.end(); ++it)
	{
		if (!it.key().contains(filter, Qt::CaseInsensitive) && !it.value().contains(filter, Qt::CaseInsensitive))
			continue;
		QTableWidgetItem *item = new QTableWidgetItem(it.key());
		QTableWidgetItem *item2 = new QTableWidgetItem(it.value());
		unicodeList->setItem(r,0,item);
		unicodeList->setItem(r,1,item2);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		delete unicodeList->takeVerticalHeaderItem(r);
		++r;
	}
	unicodeList->resizeColumnsToContents();
}

void UnicodeSearch::searchEdit_returnPressed()
{
	if (searchEdit->text().length() == 0)
		query();
	query(searchEdit->text());
}

void UnicodeSearch::hideEvent(QHideEvent * e)
{
	QDialog::hideEvent(e);
	emit setVisibleState(false);
}
