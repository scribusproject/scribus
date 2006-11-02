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

extern QPixmap loadIcon(QString nam);


UnicodeChooseButton::UnicodeChooseButton(QWidget * parent, const char * name)
	: QWidget(parent, name),
	m_searchDialog(0)
{
	selectButton = new QPushButton(this, "selectButton");
	selectButton->setText(tr("Search first"));
	selectButton->setEnabled(false);
	searchButton = new QPushButton(this, "searchButton");
	searchButton->setPixmap(loadIcon("viewmag1.png"));
	searchButton->setToggleButton(true);
	searchButton->setMinimumHeight(selectButton->height());
	searchButton->setMinimumWidth(selectButton->height());

	QBoxLayout* l = new QHBoxLayout(this);
	l->addWidget(selectButton);
	l->addWidget(searchButton);
	
	m_searchDialog = new UnicodeSearch(this, "m_searchDialog", false);
	Q_CHECK_PTR(m_searchDialog);

	connect(searchButton, SIGNAL(toggled(bool)), this, SLOT(self_toggled(bool)));
	connect(selectButton, SIGNAL(clicked()), this, SLOT(selectButton_clicked()));
	connect(m_searchDialog, SIGNAL(setVisibleState(bool)), searchButton, SLOT(setOn(bool)));
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

void UnicodeChooseButton::selectButton_clicked()
{
	emit clicked();
}


UnicodeSearch::UnicodeSearch( QWidget* parent, const char* name, bool modal)
	: UnicodeSearchBase( parent, name, modal, WStyle_Customize | WStyle_NoBorder)
{
	if (!name)
		setName("UnicodeSearch");

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
	if (file.open( IO_ReadOnly ) )
	{
		QStringList list(QStringList::split('\n', file.readAll()));
		file.close();

		QStringList line;
		for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
		{
			line = QStringList::split(':', *it);
			m_unicodeMap[line[0]] = line[1];
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
		QListViewItem *item = new QListViewItem(unicodeList, "", it.key(), it.data());
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
		QListViewItem *item = new QListViewItem(unicodeList, "", it.key(), it.data());
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
