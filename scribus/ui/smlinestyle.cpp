/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>
#include <QRegExp>

#include "colorcombo.h"
#include "smlinestyle.h"
#include "smlinestylewidget.h"
#include "scribus.h"
#include "propertiespalette.h"
#include "propertiespalette_line.h"
#include "scribusdoc.h"

#include "selection.h"
#include "sccolorengine.h"
#include "util_color.h"
#include "util.h"


SMLineStyle::SMLineStyle()
{
	
}

QTabWidget* SMLineStyle::widget()
{
	if (!m_twidget)
	{
		m_twidget = new QTabWidget();
		m_widget = new SMLineStyleWidget();
		m_twidget->addTab(m_widget, tr("Properties"));
		unitChange();
		connect(m_widget->lineStyles, SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentLineChanged(int)));
	}
	return m_twidget;
}

QString SMLineStyle::typeNamePlural()
{
	return tr("Line Styles");
}

QString SMLineStyle::typeNameSingular()
{
	return tr("Line Style");
}

void SMLineStyle::setCurrentDoc(ScribusDoc *doc)
{
	m_doc = doc;
	if (m_doc)
	{
		m_tmpLines = m_doc->docLineStyles;
		m_selection.clear();
		if (m_widget)
		{
			if (m_unitRatio != m_doc->unitRatio())
				unitChange();
		}
	}
	else
	{
		m_tmpLines.clear();
		m_selection.clear();
	}
}

QList<StyleName> SMLineStyle::styles(bool reloadFromDoc)
{
	if (m_doc && reloadFromDoc) {
		m_tmpLines = m_doc->docLineStyles;
	}

	QList<StyleName> tmpList;
	for (auto it = m_tmpLines.cbegin(); it != m_tmpLines.cend(); ++it)
		tmpList << StyleName(it.key(), QString());
	std::sort(tmpList.begin(), tmpList.end(), sortingQPairOfStrings);

	return tmpList;
}

void SMLineStyle::reload()
{
	if (!m_doc)
		return;

	m_selection.clear();
	m_tmpLines = m_doc->docLineStyles;
}

void SMLineStyle::selected(const QStringList &styleNames)
{
	m_selection.clear();
	m_selectionIsDirty = false;

	removeConnections();

	if (styleNames.count() > 1)
		setMultiSelection(styleNames);
	else if (styleNames.count() == 1)
		setSelection(styleNames[0]);

	updatePreview();
	setupConnections();
}

void SMLineStyle::setSelection(const QString& styleName)
{
	if (!m_doc)
		return;

	if (!m_tmpLines.contains(styleName))
		return; // something's wrong here

	m_selection.clear();
	m_selection[styleName] = &m_tmpLines[styleName];
	m_widget->showStyle(m_tmpLines[styleName], m_doc->PageColors);
	m_currentLine = 0;
}

void SMLineStyle::setMultiSelection(const QStringList& styles)
{
	m_selection.clear();
	for (int i = 0; i < styles.count(); ++i)
	{
		if (!m_tmpLines.contains(styles[i]))
			continue;
		m_selection[styles[i]] = &m_tmpLines[styles[i]];
	}
	m_currentLine = 0;
	// todo do the tricks for clever showing of multiple styles
}

QString SMLineStyle::fromSelection() const
{
	QString lsName;
	if (!m_doc)
		return lsName;

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
	{
		const PageItem *item = m_doc->m_Selection->itemAt(i);
		QString tmpName = item->customLineStyle();
		if (lsName.isNull() && !tmpName.isEmpty() && tmpName != "")
		{
			lsName = item->customLineStyle();
		}
		else if (!lsName.isNull() && !tmpName.isEmpty() && tmpName != "" && lsName != tmpName)
		{
			lsName.clear();
			break;
		}
	}
	return lsName;
}

void SMLineStyle::toSelection(const QString &styleName) const
{
	if (!m_doc)
		return;

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
	{
		if (styleName.isNull())
			m_doc->m_Selection->itemAt(i)->setCustomLineStyle("");
		else
			m_doc->m_Selection->itemAt(i)->setCustomLineStyle(styleName);

		m_doc->m_Selection->itemAt(i)->update();
	}
}

QString SMLineStyle::newStyle()
{
	if (!m_doc)
		return QString();
	struct SingleLine sl;
	sl.Color = m_doc->itemToolPrefs().lineColor;
	sl.Shade = m_doc->itemToolPrefs().lineColorShade;
	sl.Dash = Qt::SolidLine;//Docu->itemToolPrefs.;
	sl.LineEnd = Qt::FlatCap;//Docu->itemToolPrefs.;
	sl.LineJoin = Qt::MiterJoin;//Docu->itemToolPrefs.;
	sl.Width = m_doc->itemToolPrefs().lineWidth;
	multiLine ml;
	ml.push_back(sl);
	QString name = getUniqueName( tr("New Style"));
	m_tmpLines[name] = ml;
	return name;
}

QString SMLineStyle::newStyle(const QString &fromStyle)
{
	Q_ASSERT(m_tmpLines.contains(fromStyle));

	multiLine ml(m_tmpLines[fromStyle]);
	QString name = getUniqueName(fromStyle);
	m_tmpLines[name] = ml;
	return name;
}

QString SMLineStyle::getUniqueName(const QString &name)
{
	// Unfortunately we have to copy the logic from StyleSet::generateUniqueCopyName
	if (!m_tmpLines.contains(name))
		return name;

	QString newName(name);

	// Search the string for (number) at the end and capture
	// both the number and the text leading up to it sans brackets.
	//     Copy of fred (5)
	//     ^^^^^^^^^^^^  ^   (where ^ means captured)
	static QRegExp rx("^(.*)\\s+\\((\\d+)\\)$");
	int numMatches = rx.lastIndexIn(name);
	// Add a (number) suffix to the end of the name. We start at the
	// old suffix's value if there was one, or at 2 if there was not.
	int suffixNum = 1;
	QString prefix(newName);
	if (numMatches != -1)
	{
		// Already had a suffix; use the name w/o suffix for prefix and
		// grab the old suffix value as a starting point.
		QStringList matches = rx.capturedTexts();
		prefix = matches[1];
		suffixNum = matches[2].toInt();
	}
	// Keep on incrementing the suffix 'till we find a free name
	do
	{
		suffixNum ++;
		newName = prefix + " (" + QString::number(suffixNum) + ")";
	}
	while (m_tmpLines.contains(newName));

	return newName;
}

void SMLineStyle::apply()
{
	if (!m_doc)
		return;

	PageItem* ite;
	m_doc->docLineStyles = m_tmpLines;
	QMap<QString, QString> replacement;
	for (int i = 0; i < m_deleted.count(); ++i)
	{
		if (m_deleted[i].first == m_deleted[i].second)
			continue;
		replacement[m_deleted[i].first] = m_deleted[i].second;
	}

	m_deleted.clear();

	for (int d = 0; d < m_doc->DocItems.count(); ++d)
	{
		ite = m_doc->DocItems.at(d);
		if (!ite->NamedLStyle.isEmpty())
		{
			if (!m_doc->docLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = replacement[ite->NamedLStyle];
		}
	}
	for (int d1 = 0; d1 < m_doc->MasterItems.count(); ++d1)
	{
		ite = m_doc->MasterItems.at(d1);
		if (!ite->NamedLStyle.isEmpty())
		{
			if (!m_doc->docLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = replacement[ite->NamedLStyle];
		}
	}
	for (auto it = m_doc->FrameItems.begin(); it != m_doc->FrameItems.end(); ++it)
	{
		ite = it.value();
		if (!ite->NamedLStyle.isEmpty())
		{
			if (!m_doc->docLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = replacement[ite->NamedLStyle];
		}
	}
	m_doc->changed();
	m_doc->scMW()->requestUpdate(reqLineStylesUpdate);
	// Better not call DrawNew() here, as this will cause several unnecessary calls
	// m_doc->view()->DrawNew();
	m_selectionIsDirty = false;
}

bool SMLineStyle::isDefaultStyle(const QString &stylename) const
{
	return false;//we have no default line styles yet
// 	Q_ASSERT(m_tmpLines.contains(stylename));
// 	return m_tmpLines[stylename].isDefaultStyle();
}

void SMLineStyle::setDefaultStyle(bool ids)
{
	/* we don't have default line styles yet
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	QMap<QString, multiLine*>::iterator it;
	for (it = m_selection.begin(); it != m_selection.end(); ++it)
		(*it)->setDefaultStyle(ids);
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
	*/
}

QString SMLineStyle::shortcut(const QString &stylename) const
{
	QString s;
	QHash<QString, multiLine>::ConstIterator it = m_tmpLines.find(stylename);
	if (it != m_tmpLines.end())
		s = it.value().shortcut;
	return s;
}

void SMLineStyle::setShortcut(const QString &shortcut)
{
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	for (auto it = m_selection.begin(); it != m_selection.end(); ++it)
		(*it)->shortcut = shortcut;

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::deleteStyles(const QList<RemoveItem> &removeList)
{
	for (int i = 0; i < removeList.count(); ++i)
	{
		m_selection.erase(m_selection.find(removeList[i].first));
		m_tmpLines.erase(m_tmpLines.find(removeList[i].first));
		m_deleted.append(removeList[i]);
	}
}

void SMLineStyle::nameChanged(const QString &newName)
{
	if (m_selection.count() != 1)
	{
		qDebug() << QString("SMLineStyle::nameChanged #selection=%1").arg(m_selection.count());
		return;
	}
	QString oldName = m_selection.begin().key();
	multiLine *tmpLine = m_selection.begin().value();
	multiLine newLine(*tmpLine);
	
	m_selection.clear();
	m_tmpLines.remove(oldName);

	m_tmpLines.insert(newName, newLine);
	m_selection[newName] = &m_tmpLines[newName];

	for (auto it = m_deleted.begin(); it != m_deleted.end(); ++it)
	{
		if (it->second == oldName)
		{
			oldName = (*it).first;
			m_deleted.erase(it);
			break;
		}
	}

	if (oldName != newName)
		m_deleted.append(RemoveItem(oldName, newName));
}

void SMLineStyle::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
}

void SMLineStyle::languageChange()
{
	if (m_twidget && m_widget)
	{
		m_twidget->addTab(m_widget, tr("Properties"));
	}
}

void SMLineStyle::unitChange()
{
	m_unitRatio = m_doc->unitRatio();
	if (m_widget)
		m_widget->unitChange(m_doc->unitIndex());
}

void SMLineStyle::setupConnections()
{
	connect(m_widget->addButton, SIGNAL(clicked()), this, SLOT(slotAddLine()));
	connect(m_widget->removeButton, SIGNAL(clicked()), this, SLOT(slotDeleteLine()));
	connect(m_widget->endCombo, SIGNAL(activated(int)), this, SLOT(slotSetEnd(int)));
	connect(m_widget->joinCombo, SIGNAL(activated(int)), this, SLOT(slotSetJoin(int)));
	connect(m_widget->colorCombo, SIGNAL(activated(const QString&)), this, SLOT(slotColor(const QString&)));
	connect(m_widget->dashCombo, SIGNAL(activated(int)), this, SLOT(slotLineStyle(int)));
	connect(m_widget->shadeBox, SIGNAL(valueChanged(int)), this, SLOT(slotShade(int)));
	connect(m_widget->lineWidth, SIGNAL(valueChanged(double)), this, SLOT(slotLineWidth()));
}

void SMLineStyle::removeConnections()
{
	disconnect(m_widget->addButton, SIGNAL(clicked()), this, SLOT(slotAddLine()));
	disconnect(m_widget->removeButton, SIGNAL(clicked()), this, SLOT(slotDeleteLine()));
	disconnect(m_widget->endCombo, SIGNAL(activated(int)), this, SLOT(slotSetEnd(int)));
	disconnect(m_widget->joinCombo, SIGNAL(activated(int)), this, SLOT(slotSetJoin(int)));
	disconnect(m_widget->colorCombo, SIGNAL(activated(const QString&)), this, SLOT(slotColor(const QString&)));
	disconnect(m_widget->dashCombo, SIGNAL(activated(int)), this, SLOT(slotLineStyle(int)));
	disconnect(m_widget->shadeBox, SIGNAL(valueChanged(int)), this, SLOT(slotShade(int)));
	disconnect(m_widget->lineWidth, SIGNAL(valueChanged(double)), this, SLOT(slotLineWidth()));
}

void SMLineStyle::slotLineStyle(int i)
{
	Q_ASSERT(m_currentLine >= 0);
	if (m_currentLine < 0)
		return;

	for (auto it = m_selection.begin(); it != m_selection.end(); ++it)
	{
		multiLine *tmp = it.value();
		(*tmp)[m_currentLine].Dash = i + 1;
	}

	updateSList();
	updatePreview();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::slotSetEnd(int i)
{
	Qt::PenCapStyle c = Qt::FlatCap;
	switch (i)
	{
		case 0:
			c = Qt::FlatCap;
			break;
		case 1:
			c = Qt::SquareCap;
			break;
		case 2:
			c = Qt::RoundCap;
			break;
	}

	for (auto it = m_selection.begin(); it != m_selection.end(); ++it)
	{
		multiLine *tmp = it.value();
		(*tmp)[m_currentLine].LineEnd = static_cast<int>(c);
	}

	updateSList();
	updatePreview();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::slotSetJoin(int i)
{
	Q_ASSERT(m_currentLine >= 0);
	if (m_currentLine < 0)
		return;

	Qt::PenJoinStyle c = Qt::MiterJoin;
	switch (i)
	{
		case 0:
			c = Qt::MiterJoin;
			break;
		case 1:
			c = Qt::BevelJoin;
			break;
		case 2:
			c = Qt::RoundJoin;
			break;
	}

	for (auto it = m_selection.begin(); it != m_selection.end(); ++it)
	{
		multiLine *tmp = it.value();
		(*tmp)[m_currentLine].LineJoin = static_cast<int>(c);
	}

	updateSList();
	updatePreview();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::slotColor(const QString &s)
{
	for (auto it = m_selection.begin(); it != m_selection.end(); ++it)
	{
		multiLine *tmp = it.value();
		(*tmp)[m_currentLine].Color = s;
	}

	updateSList();
	updatePreview();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::slotShade(int i)
{
	for (auto it = m_selection.begin(); it != m_selection.end(); ++it)
	{
		multiLine *tmp = it.value();
		(*tmp)[m_currentLine].Shade = i;
	}

	updateSList();
	updatePreview();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::slotLineWidth()
{
	double unitRatio = m_widget->lineWidth->unitRatio();

	for (auto it = m_selection.begin(); it != m_selection.end(); ++it)
	{
		multiLine *tmp = it.value();
		(*tmp)[m_currentLine].Width = m_widget->lineWidth->value() / unitRatio;
	}

	updatePreview();
	resort();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::slotAddLine()
{
	if (!m_doc || m_selection.count() != 1)
		return;

	multiLine *tmpLine = m_selection.begin().value();
	struct SingleLine sl;
	sl.Color = (*tmpLine)[m_currentLine].Color;
	sl.Shade = (*tmpLine)[m_currentLine].Shade;
	sl.Dash = (*tmpLine)[m_currentLine].Dash;
	sl.LineEnd = (*tmpLine)[m_currentLine].LineEnd;
	sl.LineJoin = (*tmpLine)[m_currentLine].LineJoin;
	sl.Width = (*tmpLine)[m_currentLine].Width;

	int cc = 0;
	bool fo = false;
	for (auto it2 = tmpLine->begin(); it2 != tmpLine->end(); ++it2)
	{
		if (sl.Width < it2->Width)
		{
			tmpLine->insert(it2, sl);
			fo = true;
			break;
		}
		cc++;
	}
	if (!fo)
		tmpLine->push_back(sl);
	m_currentLine = cc;
	rebuildList();
	m_widget->showStyle(*tmpLine, m_doc->PageColors, cc);
	updatePreview();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::rebuildList()
{
	QPixmap * pm2;
	QString tmp, tmp2;
	
	int decimals = m_widget->lineWidth->decimals();
	double  unitRatio = m_widget->lineWidth->unitRatio();
	QString unitSuffix = m_widget->lineWidth->suffix();

	m_widget->lineStyles->clear();

	const multiLine *tmpLine = m_selection.begin().value();
	for (auto it = tmpLine->begin(); it != tmpLine->end(); ++it)
	{
		pm2 = getWidePixmap(calcFarbe(it->Color, it->Shade));
		tmp2 = " "+ tmp.setNum(it->Width * unitRatio, 'f', decimals) + unitSuffix + " ";
		if (it->Dash < 6)
			tmp2 += CommonStrings::translatePenStyleName(static_cast<Qt::PenStyle>(it->Dash)) + " ";
		m_widget->lineStyles->addItem(new QListWidgetItem(*pm2, tmp2, m_widget->lineStyles));
	}
}

void SMLineStyle::slotDeleteLine()
{
	if (!m_doc || m_selection.count() != 1)
		return;

	multiLine *tmpLine = m_selection.begin().value();
	if (tmpLine->size() == 1)
		return;

	int cc = 0;
	for (multiLine::iterator it3 = tmpLine->begin(); it3 != tmpLine->end(); ++it3)
	{
		if (cc == m_currentLine)
		{
			tmpLine->erase(it3);
			break;
		}
		cc++;
	}

	m_currentLine = 0;
	rebuildList();
	m_widget->showStyle(*tmpLine, m_doc->PageColors);
	updatePreview();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMLineStyle::updateSList()
{
	if (m_selection.count() < 1)
		return;

	Q_ASSERT(m_currentLine >= 0);
	if  (m_currentLine < 0)
		return;

	const multiLine *tmpLine = m_selection.begin().value();
	const SingleLine& singleLine = tmpLine->at(m_currentLine);

	int decimals = m_widget->lineWidth->decimals();
	double  unitRatio = m_widget->lineWidth->unitRatio();
	QString unitSuffix = m_widget->lineWidth->suffix();
	
	const QPixmap* pm = getWidePixmap(calcFarbe(singleLine.Color, singleLine.Shade));
	QString tmp = " " + QString::number(singleLine.Width * unitRatio, 'f', decimals) + unitSuffix + " ";
	if (singleLine.Dash < 6)
		tmp += CommonStrings::translatePenStyleName(static_cast<Qt::PenStyle>(singleLine.Dash)) + " ";
	if (m_widget->lineStyles->count() == 1)  // to avoid Bug in Qt-3.1.2
	{
		m_widget->lineStyles->clear();
		m_widget->lineStyles->addItem(new QListWidgetItem(*pm, tmp, m_widget->lineStyles));
	}
	else
	{
		QListWidgetItem *lwItem = m_widget->lineStyles->item(m_currentLine);
		lwItem->setIcon(*pm);
		lwItem->setText(tmp);
	}
}

void SMLineStyle::updatePreview()
{
	if (m_selection.count() < 1)
		return;
	
	QPixmap pm(200, 37);
	pm.fill(Qt::white);
	QPainter p;
	p.begin(&pm);

	multiLine *tmpLine = m_selection.begin().value();
	for (int it = tmpLine->size()-1; it > -1; it--)
	{
		QPen pen;
		const SingleLine& singleLine = tmpLine->at(it);
		if (singleLine.Dash == 1)
			pen.setStyle(Qt::SolidLine);
		else
		{
			QVector<double> dashArray;
			getDashArray(singleLine.Dash, 1, dashArray);
			pen.setDashPattern(dashArray);
		}
		pen.setColor(calcFarbe(singleLine.Color, singleLine.Shade));
		pen.setWidth(qMax(static_cast<int>(singleLine.Width), 1));
		pen.setCapStyle(static_cast<Qt::PenCapStyle>(singleLine.LineEnd));
		pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(singleLine.LineJoin));
		p.setPen(pen);
		p.drawLine(17, 18, 183, 18);
	}
	p.end();
	m_widget->previewLabel->setPixmap(pm);
}

QColor SMLineStyle::calcFarbe(const QString &name, int shade)
{
	QColor tmpf;
	if (!m_doc)
		return tmpf;
	const ScColor& color = m_doc->PageColors[name];
	tmpf = ScColorEngine::getDisplayColor(color, m_doc, shade);
	return tmpf;
}

void SMLineStyle::slotCurrentLineChanged(int i)
{
	if (i < 0)
		return;
	m_currentLine = i;
}

void SMLineStyle::resort()
{
	if (!m_doc || m_selection.count() != 1)
		return;

	int cc = 0;
	struct SingleLine sl;
	multiLine *tmpLine = m_selection.begin().value();
	sl.Color = (*tmpLine)[m_currentLine].Color;
	sl.Shade = (*tmpLine)[m_currentLine].Shade;
	sl.Dash = (*tmpLine)[m_currentLine].Dash;
	sl.LineEnd = (*tmpLine)[m_currentLine].LineEnd;
	sl.LineJoin = (*tmpLine)[m_currentLine].LineJoin;
	sl.Width = (*tmpLine)[m_currentLine].Width;

	for (auto it3 = tmpLine->begin(); it3 != tmpLine->end(); ++it3)
	{
		if (cc == m_currentLine)
		{
			tmpLine->erase(it3);
			break;
		}
		cc++;
	}
	cc = 0;

	bool fo = false;
	for (auto it2 = tmpLine->begin(); it2 != tmpLine->end(); ++it2)
	{
		if (sl.Width < it2->Width)
		{
			tmpLine->insert(it2, sl);
			fo = true;
			break;
		}
		cc++;
	}
	if (!fo)
		tmpLine->push_back(sl);
	m_currentLine = cc;
	rebuildList();
	m_widget->showStyle(*tmpLine, m_doc->PageColors, cc);
	updatePreview();
}

SMLineStyle::~SMLineStyle()
{
	delete m_widget;
	delete m_twidget;
	m_widget = nullptr;
	m_twidget = nullptr;
}

