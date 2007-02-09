/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qfile.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qcursor.h>

#include "unicodesearch.h"
#include "unicodesearch.moc"
#include "scpaths.h"
// #include "fonts/scfontmetrics.h"
#include "charzoom.h"


extern QPixmap loadIcon(QString nam);


UnicodeChooseButton::UnicodeChooseButton(QWidget * parent, const char * name)
	: QPushButton(parent, name),
	m_searchDialog(0)
{
	setText("&Search");
	setToggleButton(true);

	m_searchDialog = new UnicodeSearch(this, "m_searchDialog", false);
	Q_CHECK_PTR(m_searchDialog);

	connect(this, SIGNAL(toggled(bool)), this, SLOT(self_toggled(bool)));
	connect(m_searchDialog, SIGNAL(setVisibleState(bool)), this, SLOT(setOn(bool)));
	//
	// listview user inputs
	connect(m_searchDialog->unicodeList, SIGNAL(doubleClicked(QListViewItem *, const QPoint &, int)), this, SLOT(unicodeList_chosen(QListViewItem *)));
	connect(m_searchDialog->unicodeList, SIGNAL(returnPressed(QListViewItem *)), this, SLOT(unicodeList_chosen(QListViewItem *)));
	connect(m_searchDialog->unicodeList, SIGNAL(spacePressed(QListViewItem *)), this, SLOT(unicodeList_chosen(QListViewItem *)));
}

void UnicodeChooseButton::unicodeList_chosen(QListViewItem *item)
{
	emit chosenUnicode(item->text(0));
	emit toggled(false);
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

UnicodeSearch::UnicodeSearch( QWidget* parent, const char* name, bool modal)
	: UnicodeSearchBase( parent, name, modal), // WStyle_Customize | WStyle_NoBorder),
	m_zoom(0)
{
	if (!name)
		setName("UnicodeSearch");

	connect(searchEdit, SIGNAL(returnPressed()), this, SLOT(searchEdit_returnPressed()));
	connect(unicodeList, SIGNAL(mouseButtonPressed(int, QListViewItem*, const QPoint&, int)), this, SLOT(unicodeList_mouseButtonPressed(int, QListViewItem*, const QPoint&, int)));
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
	if (file.open( IO_ReadOnly ) )
	{
		QStringList list(QStringList::split('\n', file.readAll()));
		file.close();

		QStringList line;
		for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
		{
			line = QStringList::split(':', *it);
			m_unicodeMap[line[0]] = line[1].lower();
		}
	}
	else
		qDebug("UnicodeSearch: error reading unicodes!");
}

void UnicodeSearch::query()
{
	unicodeList->clear();
	QMap<QString,QString>::Iterator it;
	for (it = m_unicodeMap.begin(); it != m_unicodeMap.end(); ++it)
	{
		QListViewItem *item = new QListViewItem(unicodeList, it.key(), it.data());
		unicodeList->insertItem(item);
	}
}

void UnicodeSearch::query(QString filter)
{
	if (filter.isNull())
		return;

	unicodeList->clear();

	QMap<QString,QString>::Iterator it;
	for (it = m_unicodeMap.begin(); it != m_unicodeMap.end(); ++it)
	{
		if (!it.key().contains(filter, false) && !it.data().contains(filter, false))
			continue;
		QListViewItem *item = new QListViewItem(unicodeList, it.key(), it.data());
		unicodeList->insertItem(item);
	}
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

void UnicodeSearch::unicodeList_mouseButtonPressed(int button, QListViewItem* item, const QPoint& point, int)
{
	if (!item)
		return;
	// It must go 1st to delete the existing dialog on click
	if (m_zoom)
	{
		delete m_zoom;
		m_zoom = 0;
	}
	if (button == RightButton && !m_zoom)
	{
		bool ok;
		int val = item->text(0).toInt(&ok, 16);
		if (!ok)
			return;
		m_zoom = new CharZoom(this, val, m_font);
		m_zoom->move(point.x()-2, point.y()-2);
		m_zoom->show();
	}
}
