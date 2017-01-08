/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCheckBox>

#include "commonstrings.h"
#include "fontlistmodel.h"
#include "iconmanager.h"
#include "prefsmanager.h"
#include "scribusdoc.h"

FontListModel::FontListModel(QObject * parent, ScribusDoc * doc, bool includeDisabled)
	: QAbstractTableModel(parent),
	m_doc(doc),
	m_fonts(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts),
	m_includeDisabled(includeDisabled)
{
	ttfFont = IconManager::instance()->loadPixmap("font_truetype16.png");
	otfFont = IconManager::instance()->loadPixmap("font_otf16.png");
	psFont = IconManager::instance()->loadPixmap("font_type1_16.png");
	substFont = IconManager::instance()->loadPixmap("font_subst16.png");

	setFonts(m_fonts.keys());
}

void FontListModel::setFonts(QList<QString> f)
{
	beginResetModel();
	m_font_names.clear();
	m_font_values.clear();
	m_enabledFonts.clear();
	m_embedFlags.clear();
	for (int i = 0; i < f.length(); ++i)
	{
		ScFace &font = m_fonts[f[i]];
		if (font.usable() || m_includeDisabled)
		{
			m_font_names.append(f[i]);
			m_font_values.append(font);
			m_enabledFonts.append(font.usable());
			int embedFlags = 0;
			embedFlags |= font.embedPs() ? EmbedPS : 0;
			embedFlags |= font.subset()  ? SubsetPDF : 0;
			m_embedFlags.append(embedFlags);
		}
	}
	endResetModel();
}

int FontListModel::rowCount(const QModelIndex&) const
{
	return m_font_names.size();
}

int FontListModel::rowCount()
{
	return m_font_names.size();
}

int FontListModel::columnCount(const QModelIndex&) const
{
	return 9;
}

QVariant FontListModel::headerData(int section,
								   Qt::Orientation orientation,
								   int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant(); // no verticals
	// TODO for tooltips etc.
	
	bool isDisplayRole = (role == Qt::DisplayRole);
	switch(role)
	{
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
			switch (section)
		{
			case FontListModel::FontName:
				return tr("Font Name");
			case FontListModel::FontUsable:
				return isDisplayRole? QString("✓") : tr("Use Font");
			case FontListModel::FontType:
				return tr("Type");
			case FontListModel::FontFormat:
				return tr("Format");
			case FontListModel::FontSubset:
				return tr("Subset");
			case FontListModel::FontAccess:
				return tr("Access");
			case FontListModel::FontInDoc:
				return tr("Used in Doc");
			case FontListModel::FontFile:
				return tr("Path to Font File");
			default:
				return "Never should be shown";
		};
	
		default:
			// dummy return
			return QVariant();
	}
}

QVariant FontListModel::data(const QModelIndex & index,
							 int role) const
{
	ScFace font = m_font_values[index.row()];

	if (role == Qt::DecorationRole && index.column() == FontListModel::FontType)
	{
		switch (font.type())
		{
			case ScFace::TYPE0:
			case ScFace::TYPE1:
			case ScFace::TYPE3:
				return QVariant(psFont);
			case ScFace::TTF:
				return QVariant(ttfFont);
			case ScFace::OTF:
				return QVariant(otfFont);
			default:
				return QVariant();
		};
	}

	if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
	{
		switch (index.column())
		{
			case FontListModel::FontName:
				if (role == Qt::DisplayRole)
					return font.scName();
				return
					tr("Family") + ":\t" + font.family() + "\n"
					+ tr("Style") + ":\t" + font.style() + "\n"
					+ tr("Variant") + ":\t" + font.variant();
			case FontListModel::FontType:
			{
				if (role == Qt::DisplayRole)
					return QVariant();
				
				switch (font.type())
				{
					case ScFace::TYPE0:
					case ScFace::TYPE1:
					case ScFace::TYPE3:
						return "Type1";
					case ScFace::TTF:
						return "TrueType";
					case ScFace::CFF:
						return "CFF";
					case ScFace::OTF:
						return "OpenType";
					default:
						return tr("Unknown", "font type");
				};
			}
			case FontListModel::FontFormat:
			{
				switch (font.format())
				{
					case ScFace::PFA: return "PFA";
					case ScFace::PFB: return "PFB";
					case ScFace::TYPE2: return "TYPE2";
					case ScFace::TYPE42: return "TYPE42";
					case ScFace::SFNT: return "SFNT";
					case ScFace::TTCF: return "TTCF";
					default: return tr("Unknown", "font format");
				};
			}
			case FontListModel::FontAccess:
			{
				QFileInfo fi(font.fontFilePath());
				return fi.absoluteFilePath().contains(QDir::homePath()) ?
						tr("User", "font preview"):
						tr("System", "font preview");
			}
			case FontListModel::FontInDoc:
			{
				if (m_doc && m_doc->UsedFonts.contains(font.scName()))
					return CommonStrings::trYes;
				return QVariant();
			}
			case FontListModel::FontFile:
				return font.fontPath();
			case FontListModel::SortIndex:
				if (font.scName()[0] == QChar('.'))
					return font.scName().mid(1).toLower();
				return font.scName().toLower();
			default:
				if (role == Qt::ToolTipRole
					&&
					(index.column() == FontListModel::FontUsable
					 || index.column() == FontListModel::FontSubset
					 )
					)
				{
					return tr("Click to change the value");
				}
				return QVariant();
		};
	}
	
	if (role == Qt::CheckStateRole && index.column() == FontListModel::FontUsable)
	{
		if (isLive())
			return (font.usable() ? Qt::Checked : Qt::Unchecked);
		else
			return (m_enabledFonts[index.row()] ? Qt::Checked : Qt::Unchecked);
	}
	if (role == Qt::CheckStateRole && index.column() == FontListModel::FontSubset)
	{
		if (isLive())
			return (font.subset() ? Qt::Checked : Qt::Unchecked);
		else
			return ((m_embedFlags[index.row()] & SubsetPDF) ? Qt::Checked : Qt::Unchecked);
	}

	return QVariant();
}

Qt::ItemFlags FontListModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (!index.isValid())
		return QAbstractTableModel::flags(index);
	if (index.column() == FontListModel::FontUsable
		   || index.column() == FontListModel::FontSubset)
		return Qt::ItemIsUserCheckable | /*Qt::ItemIsEditable |*/ defaultFlags;
	else
		return defaultFlags;
}

bool FontListModel::setData(const QModelIndex & idx,
							const QVariant & value,
							int role)
{
	if (!idx.isValid() || role != Qt::CheckStateRole)
	{
		qDebug("FontListModel::setData() out of Qt::CheckStateRole role");
		return false;
	}

/*	ScFace f = m_fonts[m_fonts.keys().at(idx.row())];

	if (idx.column() == FontListModel::FontUsable)
		m_fonts[m_fonts.keys().at(idx.row())].usable(!f.usable());
	else if (idx.column() == FontListModel::FontEmbed)
		m_fonts[m_fonts.keys().at(idx.row())].embedPs(!f.embedPs());
	else if (idx.column() == FontListModel::FontSubset)
		m_fonts[m_fonts.keys().at(idx.row())].subset(!f.subset());
	else
		qDebug("FontListModel::setData() out of defined editable columns"); */

	ScFace f = m_font_values[idx.row()];

	if (isLive())
	{
		if (idx.column() == FontListModel::FontUsable)
			f.usable(!f.usable());
		else if (idx.column() == FontListModel::FontSubset)
			f.subset(!f.subset());
		else
			qDebug("FontListModel::setData() out of defined editable columns");
	}
	else
	{
		if (idx.column() == FontListModel::FontUsable && role == Qt::CheckStateRole)
		{
			Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());
			m_enabledFonts[idx.row()] = (checkState == Qt::Checked);
		}
		else if (idx.column() == FontListModel::FontSubset)
		{
			Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());
			m_embedFlags[idx.row()] &= ~SubsetPDF;
			m_embedFlags[idx.row()] |= (checkState == Qt::Checked) ? SubsetPDF : 0;
		}
		else
			qDebug("FontListModel::setData() out of defined editable columns");
	}

	if (idx.column() == FontListModel::FontUsable)
		emit dataChanged(index(idx.row(), FontListModel::FontUsable), index(idx.row(), FontListModel::FontUsable));
	else
		emit dataChanged(index(idx.row(), FontListModel::FontSubset), index(idx.row(), FontListModel::FontSubset));

	return true;
}

QString FontListModel::nameForIndex(const QModelIndex & idx)
{
	return m_font_names.at(idx.row());
}
