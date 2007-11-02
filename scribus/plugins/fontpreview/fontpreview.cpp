/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>

#include "fontpreview.h"
#include "scribus.h"
#include "prefsfile.h"
#include "commonstrings.h"
#include "prefsmanager.h"
#include "selection.h"
#include "sampleitem.h"
#include "util_icon.h"


FontPreview::FontPreview(QString fontName, QWidget* parent, ScribusDoc* doc)
	: QDialog(parent, 0)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(loadIcon("AppIcon.png"));
	m_Doc=doc;

	sampleItem = new SampleItem(m_Doc);

	languageChange();

	resetDisplayButton->setIcon(QIcon(loadIcon("u_undo16.png")));

	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	okIcon = loadIcon("ok.png");

	fontModel = new QStandardItemModel(0, 5, this);
	fontModel->setHeaderData(0, Qt::Horizontal, tr("Font Name"), Qt::DisplayRole);
	fontModel->setHeaderData(1, Qt::Horizontal, tr("Doc"), Qt::DisplayRole);
	fontModel->setHeaderData(2, Qt::Horizontal, tr("Type"), Qt::DisplayRole);
	fontModel->setHeaderData(3, Qt::Horizontal, tr("Subset"), Qt::DisplayRole);
	fontModel->setHeaderData(4, Qt::Horizontal, tr("Access"), Qt::DisplayRole);

	QList<QStandardItem *> fontRow;
	for (SCFontsIterator fontIter(PrefsManager::instance()->appPrefs.AvailFonts);
			fontIter.hasNext(); fontIter.next())
	{
		if (!fontIter.current().usable())
			continue;

		fontRow.clear();
		QStandardItem * col0 = new QStandardItem();
		QStandardItem * col1 = new QStandardItem();
		QStandardItem * col2 = new QStandardItem();
		QStandardItem * col3 = new QStandardItem();
		QStandardItem * col4 = new QStandardItem();

		col0->setEditable(false);
		col1->setEditable(false);
		col2->setEditable(false);
		col3->setEditable(false);
		col4->setEditable(false);

		col0->setText(fontIter.current().scName());

		if (m_Doc->UsedFonts.contains(fontIter.current().scName()))
			col1->setIcon(okIcon);

		ScFace::FontType type = fontIter.current().type();
		if (type == ScFace::OTF)
		{
			col2->setIcon(otfFont);
			col2->setText("OpenType");
		}
		else
			if (fontIter.current().subset())
				col3->setIcon(okIcon);

		if (type == ScFace::TYPE1) // type1
		{
			col2->setIcon(psFont);
			col2->setText("Type1");
		}

		if (type == ScFace::TTF)
		{
			col2->setIcon(ttfFont);
			col2->setText("TrueType");
		}

		QFileInfo fi(fontIter.current().fontFilePath());
		fi.absoluteFilePath().contains(QDir::homePath()) ?
				col4->setText(tr("User", "font preview")):
				col4->setText(tr("System", "font preview"));

		fontRow << col0 << col1 << col2 << col3 << col4;
		fontModel->appendRow(fontRow);
	}
// 	fontList->setModel(fontModel); // displaying is handled by proxyModel

	proxyModel = new QSortFilterProxyModel();
	proxyModel->setDynamicSortFilter(true);
	proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	proxyModel->setSourceModel(fontModel);
	proxyModel->setFilterKeyColumn(0);
	fontList->setModel(proxyModel);

	// scribus config
	defaultStr = tr("Woven silk pyjamas exchanged for blue quartz", "font preview");
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("fontpreview");
	sortColumn = prefs->getUInt("sortColumn", 0);
	fontModel->sort(sortColumn);
	xsize = prefs->getUInt("xsize", 640);
	ysize = prefs->getUInt("ysize", 480);
	sizeSpin->setValue(prefs->getUInt("fontSize", 18));
	QString ph = prefs->get("phrase", defaultStr);
	displayEdit->setText(ph);
	displayButton_clicked();
	resize(QSize(xsize, ysize).expandedTo(minimumSizeHint()));

	connect(displayButton, SIGNAL(clicked()), this, SLOT(displayButton_clicked()));
	connect(searchEdit, SIGNAL(textChanged(QString)), this, SLOT(searchEdit_textChanged(QString)));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));
	connect(resetDisplayButton, SIGNAL(clicked()), this, SLOT(resetDisplayButton_clicked()));
	connect(sizeSpin, SIGNAL(valueChanged(int)), this, SLOT(sizeSpin_valueChanged(int)));
	connect(fontList->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)),
			this, SLOT(fontList_currentChanged(const QModelIndex &, const QModelIndex &)));

	QString searchName;
	if (!fontName.isEmpty())
		searchName = fontName;
	else
	{
		Q_ASSERT(m_Doc!=0);
		if (m_Doc->m_Selection->count() != 0)
			searchName = m_Doc->currentStyle.charStyle().font().scName();
		else
			searchName = PrefsManager::instance()->appPrefs.toolSettings.defFont;
	}
	QList<QStandardItem *> found = fontModel->findItems(searchName);
	if (found.size() > 0)
	{
		fontList->scrollTo(found.at(0)->index(), QAbstractItemView::PositionAtCenter);
		fontList->selectRow(found.at(0)->index().row());
	}
	fontList->resizeColumnsToContents();
}

FontPreview::~FontPreview()
{
	prefs->set("sortColumn", fontList->horizontalHeader()->sortIndicatorSection());
	prefs->set("xsize", width());
	prefs->set("ysize", height());
	prefs->set("fontSize", sizeSpin->value());
	prefs->set("phrase", displayEdit->text());
	sampleItem->cleanupTemporary(); // just to be sure
}

void FontPreview::languageChange()
{
	cancelButton->setToolTip(tr("Leave preview", "font preview"));
	searchEdit->setToolTip("<qt>" + tr("Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.") + "</qt>");
	searchButton->setToolTip(tr("Start searching"));
	sizeSpin->setToolTip(tr("Size of the selected font"));
}

bool FontPreview::allowSample()
{
	if (fontModel->rowCount() != 0)
		return true;
	fontPreview->setText("No font selected");
	return false;
}

void FontPreview::paintSample()
{
	if (!allowSample())
		return;

	QString fontName(getCurrentFont());
	if (fontName.isNull())
		return;

	sampleItem->setFontSize(sizeSpin->value() * 10, true);
	sampleItem->setFont(fontName);
	QPixmap pixmap = sampleItem->getSample(fontPreview->maximumWidth(),
										   fontPreview->maximumHeight());
	fontPreview->clear();
	if (!pixmap.isNull())
		fontPreview->setPixmap(pixmap);
}

void FontPreview::searchEdit_textChanged(const QString &/*s*/)
{
	searchButton_clicked();
}

void FontPreview::searchButton_clicked()
{
	disconnect(fontList->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)),
			   this, SLOT(fontList_currentChanged(const QModelIndex &, const QModelIndex &)));
	QString s(searchEdit->text());
	if (s.isEmpty())
		fontList->setModel(fontModel);
	else
	{
		QRegExp regExp(QString("*%1*").arg(s), Qt::CaseInsensitive, QRegExp::Wildcard);
		proxyModel->setFilterRegExp(regExp);
		fontList->setModel(proxyModel);
	}
	fontList->resizeColumnsToContents();
	connect(fontList->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)),
			this, SLOT(fontList_currentChanged(const QModelIndex &, const QModelIndex &)));
}

QString FontPreview::getCurrentFont()
{
	QModelIndex ix(fontList->currentIndex());
	if (!ix.isValid())
		return QString();
	return fontModel->item(ix.row(), 0)->text();
}

void FontPreview::displayButton_clicked()
{
	sampleItem->setText(displayEdit->text());
	paintSample();
}

void FontPreview::cancelButton_clicked()
{
	reject();
}

void FontPreview::resetDisplayButton_clicked()
{
	displayEdit->setText(defaultStr);
	displayButton_clicked();
}

void FontPreview::sizeSpin_valueChanged( int )
{
	paintSample();
}

void FontPreview::fontList_currentChanged(const QModelIndex &, const QModelIndex &)
{
	paintSample();
}
