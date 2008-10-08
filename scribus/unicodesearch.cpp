/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFile>
#include <QHeaderView>
#include <QSortFilterProxyModel>
 
#include "unicodesearch.h"
#include "scpaths.h"


UnicodeChooseButton::UnicodeChooseButton(QWidget * parent)
	: QPushButton(parent),
	m_searchDialog(0)
{
	languageChange();
	setCheckable(true);

// 	m_cacheTimer = new QTimer(this);

	connect(this, SIGNAL(toggled(bool)),
			 this, SLOT(self_toggled(bool)));
// 	connect(m_cacheTimer, SIGNAL(timeout()),
// 			 this, SLOT(deleteSearchDialog()));
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


void UnicodeChooseButton::glyphSelected(const QString & hex)
{
	emit chosenUnicode(hex);
	emit toggled(false);
}

void UnicodeChooseButton::self_toggled(bool state)
{
	if (!m_searchDialog)
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		m_searchDialog = new UnicodeSearch();
		connect(m_searchDialog, SIGNAL(setVisibleState(bool)),
				 this, SLOT(setChecked(bool)));
		connect(m_searchDialog, SIGNAL(glyphSelected(const QString &)),
				 this, SLOT(glyphSelected(const QString &)));
		QApplication::restoreOverrideCursor();
	}

	if (state)
	{
		m_searchDialog->move(mapToGlobal(rect().bottomLeft()));
		m_searchDialog->show();
// 		m_cacheTimer->stop();
	}
	else
	{
		m_searchDialog->hide();
// 		m_cacheTimer->start(180000); // 3 minutes
	}
}

// void UnicodeChooseButton::deleteSearchDialog()
// {
// 	m_cacheTimer->stop();
// 	qDebug("void UnicodeChooseButton::deleteSearchDialog()");
// 	if (!m_searchDialog)
// 		return;
// 	qDebug("void UnicodeChooseButton::deleteSearchDialog() 1");
// 	delete m_searchDialog;
// 	m_searchDialog = 0;
// }

UnicodeSearchModel::UnicodeSearchModel(QObject * /*parent*/)
	: QAbstractTableModel()
{
	setObjectName("UnicodeSearchModel");

	QFile file(ScPaths::instance().shareDir() + "unicodenameslist.txt");
	if (file.open( QIODevice::ReadOnly ) )
	{
		QStringList list = QString(file.readAll()).split('\n', QString::SkipEmptyParts);
		file.close();

		QStringList line;
		UnicodeStruct uni;
		for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
		{
			line = (*it).split(':', QString::SkipEmptyParts);
			uni.description = line[1].toLower();
			uni.hex = line[0];
			m_unicode.append(uni);
		}
	}
	else
		qDebug("UnicodeSearchModel: error reading unicodes!");
}


UnicodeSearchModel::~UnicodeSearchModel()
{
}

int UnicodeSearchModel::rowCount(const QModelIndex & /*parent*/) const
{
	return m_unicode.count();
}

int UnicodeSearchModel::columnCount(const QModelIndex & /*parent*/) const
{
	return 2;
}

QVariant UnicodeSearchModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role == Qt::DisplayRole)
	{
		if (index.column() == 0)
			return m_unicode.at(index.row()).hex;
		else
			return m_unicode.at(index.row()).description;
	}
	return QVariant();
}

QString UnicodeSearchModel::hexData(const QModelIndex & index)
{
	return "0x" + m_unicode.at(index.row()).hex;
}

// QVariant UnicodeSearchModel::headerData(int section, Qt::Orientation orientation, int role) const
// {
// 	if (orientation == Qt::Vertical)
// 		return QVariant(); // no verticals
// 	if (role != Qt::DisplayRole)
// 		return QVariant();
// 
// 	if (section == 0)
// 		return "Hex";
// 	else
// 		return tr("Description");
// }



UnicodeSearch::UnicodeSearch( QWidget* parent)
	: QDialog( parent)
{
	setupUi(this);

	m_model = new UnicodeSearchModel(this);

	m_proxyModel = new QSortFilterProxyModel();
	m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_proxyModel->setSourceModel(m_model);
	m_proxyModel->setFilterKeyColumn(-1);
	m_proxyModel->sort(0);

	tableView->horizontalHeader()->hide();
	tableView->verticalHeader()->hide();
	tableView->setModel(m_proxyModel);
	tableView->resizeColumnsToContents();

	connect(searchEdit, SIGNAL(returnPressed()),
			this, SLOT(searchEdit_returnPressed()));
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex &)),
			 this, SLOT(itemChosen(const QModelIndex &)));
	connect(tableView, SIGNAL(activated(const QModelIndex &)),
			 this, SLOT(itemChosen(const QModelIndex &)));
}

UnicodeSearch::~UnicodeSearch()
{
// 	delete m_proxyModel;
// 	delete m_model;
}

void UnicodeSearch::itemChosen(const QModelIndex & index)
{
	emit glyphSelected(m_model->hexData(m_proxyModel->mapToSource(index)));
}

void UnicodeSearch::searchEdit_returnPressed()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QString s(searchEdit->text());
	if (s.isEmpty())
		m_proxyModel->setFilterRegExp(QRegExp("*",
											Qt::CaseInsensitive,
											QRegExp::Wildcard));
	else
	{
		QRegExp regExp(QString("*%1*").arg(s),
					   Qt::CaseInsensitive,
					   QRegExp::Wildcard);
		m_proxyModel->setFilterRegExp(regExp);
	}
	tableView->resizeColumnsToContents();
	tableView->setFocus(Qt::OtherFocusReason);
	tableView->selectRow(0);
	QApplication::restoreOverrideCursor();
}

void UnicodeSearch::hideEvent(QHideEvent * e)
{
	QDialog::hideEvent(e);
	emit setVisibleState(false);
}
