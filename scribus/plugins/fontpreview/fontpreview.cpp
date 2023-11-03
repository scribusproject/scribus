/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QSortFilterProxyModel>
#include <QHeaderView>

#include "fontpreview.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "selection.h"
#include "sampleitem.h"
#include "fontlistmodel.h"
#include "iconmanager.h"


FontPreview::FontPreview(const QString& fontName, QWidget* parent, ScribusDoc* doc)
	: QDialog(parent, Qt::WindowFlags()),
	  m_Doc(doc)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));

	m_sampleItem = new SampleItem();
	m_sampleItem->setDevicePixelRatio(devicePixelRatioF());

	languageChange();

	m_fontModel = new FontListModel(this, m_Doc, false);

	m_proxyModel = new QSortFilterProxyModel();
	m_proxyModel->setDynamicSortFilter(true);
	m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_proxyModel->setSourceModel(m_fontModel);
	m_proxyModel->setFilterKeyColumn(1);
	m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	fontList->setModel(m_proxyModel);

	// scribus config
	m_defaultStr = tr("Woven silk pyjamas exchanged for blue quartz", "font preview");
	m_prefs = PrefsManager::instance().prefsFile->getPluginContext("fontpreview");
	uint srt = m_prefs->getUInt("sortColumn", 0);
	bool extend = m_prefs->getBool("extendedView", false);
	extendedCheckBox->setChecked(extend);

	auto srtOrder = (Qt::SortOrder) m_prefs->getUInt("sortColumnOrder", 0);
	m_proxyModel->sort(srt, srtOrder);

	fontList->horizontalHeader()->setSortIndicatorShown(true);
	fontList->horizontalHeader()->setSortIndicator(srt, srtOrder);

	m_xsize = m_prefs->getUInt("xsize", 640);
	m_ysize = m_prefs->getUInt("ysize", 480);
	sizeSpin->setValue(m_prefs->getUInt("fontSize", 18));

	QString ph = m_prefs->get("phrase", m_defaultStr);
	displayEdit->setText(ph);

	displayButton_clicked();
	resize(QSize(m_xsize, m_ysize).expandedTo(minimumSizeHint()));

	setExtendedView(extend);

	QString searchName;
	if (!fontName.isEmpty())
		searchName = fontName;
	else
	{
		Q_ASSERT(m_Doc != nullptr);
		if (m_Doc->m_Selection->count() != 0)
			searchName = m_Doc->currentStyle.charStyle().font().scName();
		else
			searchName = PrefsManager::instance().appPrefs.itemToolPrefs.textFont;
	}
	QModelIndexList found = m_fontModel->match(m_fontModel->index(0, 0),
											   Qt::DisplayRole, searchName,
											   1,
											   Qt::MatchContains | Qt::MatchWrap);
	if (!found.empty())
	{
		fontList->scrollTo(found.at(0), QAbstractItemView::PositionAtCenter);
		fontList->selectRow(found.at(0).row());
	}

	fontList->resizeColumnsToContents();

	connect(displayButton, SIGNAL(clicked()), this, SLOT(displayButton_clicked()));
	connect(searchEdit, SIGNAL(textChanged(QString)), this, SLOT(searchEdit_textChanged(QString)));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));
	connect(resetDisplayButton, SIGNAL(clicked()), this, SLOT(resetDisplayButton_clicked()));
	connect(sizeSpin, SIGNAL(valueChanged(int)), this, SLOT(sizeSpin_valueChanged(int)));
	connect(fontList->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)), this, SLOT(fontList_currentChanged(const QModelIndex&, const QModelIndex&)));
	connect(extendedCheckBox, SIGNAL(clicked(bool)), this, SLOT(setExtendedView(bool)));
}

FontPreview::~FontPreview()
{
	m_prefs->set("sortColumn", fontList->horizontalHeader()->sortIndicatorSection());
	m_prefs->set("sortColumnOrder", fontList->horizontalHeader()->sortIndicatorOrder());
	m_prefs->set("xsize", width());
	m_prefs->set("ysize", height());
	m_prefs->set("fontSize", sizeSpin->value());
	m_prefs->set("phrase", displayEdit->text());
	m_prefs->set("extendedView", extendedCheckBox->isChecked());
}

void FontPreview::languageChange()
{
	cancelButton->setToolTip(tr("Leave preview", "font preview"));
	searchEdit->setToolTip("<qt>" + tr("Typing the text here provides quick searching in the font names. Searching is case insensitive. The given text is taken as substring.") + "</qt>");
	sizeSpin->setToolTip(tr("Size of the selected font"));
}

void FontPreview::showEvent(QShowEvent * event)
{
	paintSample();
	QDialog::showEvent(event);
}

void FontPreview::resizeEvent(QResizeEvent * event)
{
	paintSample();
	QDialog::resizeEvent(event);
}

bool FontPreview::allowSample()
{
	if (m_fontModel->rowCount() != 0)
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

	m_sampleItem->setFontSize(sizeSpin->value() * 10, true);
	m_sampleItem->setFont(fontName);
	QPixmap pixmap = m_sampleItem->getSample(fontPreview->width(),
										     fontPreview->height());
	fontPreview->clear();
	if (!pixmap.isNull())
		fontPreview->setPixmap(pixmap);
}

void FontPreview::searchEdit_textChanged(const QString &/*s*/)
{
	fontList->blockSignals(true);
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
	fontList->resizeColumnsToContents();
	fontList->blockSignals(false);
}

QString FontPreview::getCurrentFont()
{
	QModelIndex ix(fontList->currentIndex());
	if (!ix.isValid())
		return QString();
	return m_fontModel->nameForIndex(m_proxyModel->mapToSource(fontList->currentIndex()));
}

void FontPreview::displayButton_clicked()
{
	m_sampleItem->setText(displayEdit->text());
	paintSample();
}

void FontPreview::cancelButton_clicked()
{
	reject();
}

void FontPreview::resetDisplayButton_clicked()
{
	displayEdit->setText(m_defaultStr);
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

void FontPreview::setExtendedView(bool state)
{
	for (int i=0; i < fontList->model()->columnCount(); ++i)
	{
		if (i == 1)
			continue;
		fontList->setColumnHidden(i, !state);
	}
}
