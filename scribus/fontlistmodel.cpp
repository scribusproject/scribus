/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCheckBox>
#include "fontlistmodel.h"
#include "prefsmanager.h"
#include "util_icon.h"


FontListModel::FontListModel(QObject * parent)
	: QAbstractTableModel(parent),
	m_fonts(PrefsManager::instance()->appPrefs.AvailFonts)
{
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	substFont = loadIcon("font_subst16.png");
	m_font_keys = m_fonts.keys();
}

void FontListModel::setFonts(SCFonts f)
{
	m_fonts = f;
	m_font_keys = m_fonts.keys();
	reset();
}

int FontListModel::rowCount(const QModelIndex&) const
{
	return m_fonts.size();
}

int FontListModel::rowCount()
{
	return m_fonts.size();
}

int FontListModel::columnCount(const QModelIndex&) const
{
	return 12;
}

QVariant FontListModel::headerData(int section,
								   Qt::Orientation orientation,
								   int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant(); // no verticals
	// TODO for tooltips etc.
	if (role != Qt::DisplayRole)
		return QVariant();

	switch (section)
	{
		case FontListModel::FontName:
			return tr("Font Name");
		case FontListModel::FontUsable:
			return tr("Use Font");
		case FontListModel::FontFamily:
			return tr("Family");
		case FontListModel::FontStyle:
			return tr("Style");
		case FontListModel::FontVariant:
			return tr("Variant");
		case FontListModel::FontType:
			return tr("Type");
		case FontListModel::FontFormat:
			return tr("Format");
		case FontListModel::FontEmbed:
			return tr("Embed in PostScript");
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
	
	// dummy return
	return QVariant();
}

QVariant FontListModel::data(const QModelIndex & index,
							 int role) const
{
//	ScFace font = m_fonts[m_fonts.keys().at(index.row())];
	ScFace font = m_fonts[m_font_keys.at(index.row())];

	if (role == Qt::DecorationRole
		   && index.column() == FontListModel::FontType)
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

	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
			case FontListModel::FontName:
				return font.scName();
			case FontListModel::FontFamily:
				return font.family();
			case FontListModel::FontStyle:
				return font.style();
			case FontListModel::FontVariant:
				return font.variant();
			case FontListModel::FontType:
			{
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
// 				if (m_Doc->UsedFonts.contains(fontIter.current().scName()))
// 					col1->setIcon(okIcon);
				return "TODO";
			}
			case FontListModel::FontFile:
				return font.fontFilePath();
			default:
				return QVariant();
		};
	}

	if (role == Qt::ToolTipRole
		   &&
		   (index.column() == FontListModel::FontUsable
		   || index.column() == FontListModel::FontEmbed
		   || index.column() == FontListModel::FontSubset
		   )
	   )
	{
		return tr("Click to change the value");
	}

	if (role == Qt::CheckStateRole
		   && index.column() == FontListModel::FontUsable)
		return font.usable();
	if (role == Qt::CheckStateRole
		   && index.column() == FontListModel::FontEmbed)
		return font.embedPs();
	if (role == Qt::CheckStateRole
		   && index.column() == FontListModel::FontSubset)
		return font.subset();

	return QVariant();
}

Qt::ItemFlags FontListModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (!index.isValid())
		return QAbstractTableModel::flags(index);
	if (index.column() == FontListModel::FontUsable 
		   || index.column() == FontListModel::FontEmbed
		   || index.column() == FontListModel::FontSubset)
		return Qt::ItemIsUserCheckable | /*Qt::ItemIsEditable |*/ defaultFlags;
	else
		return defaultFlags;
}

bool FontListModel::setData(const QModelIndex & index,
							const QVariant & value,
							int role)
{
	if (!index.isValid() || role != Qt::CheckStateRole)
	{
		qDebug("FontListModel::setData() out of Qt::CheckStateRole role");
		return false;
	}

/*	ScFace f = m_fonts[m_fonts.keys().at(index.row())];

	if (index.column() == FontListModel::FontUsable)
		m_fonts[m_fonts.keys().at(index.row())].usable(!f.usable());
	else if (index.column() == FontListModel::FontEmbed)
		m_fonts[m_fonts.keys().at(index.row())].embedPs(!f.embedPs());
	else if (index.column() == FontListModel::FontSubset)
		m_fonts[m_fonts.keys().at(index.row())].subset(!f.subset());
	else
		qDebug("FontListModel::setData() out of defined editable columns"); */
	
	ScFace f = m_fonts[m_font_keys.at(index.row())];

	if (index.column() == FontListModel::FontUsable)
		m_fonts[m_font_keys.at(index.row())].usable(!f.usable());
	else if (index.column() == FontListModel::FontEmbed)
		m_fonts[m_font_keys.at(index.row())].embedPs(!f.embedPs());
	else if (index.column() == FontListModel::FontSubset)
		m_fonts[m_font_keys.at(index.row())].subset(!f.subset());
	else
		qDebug("FontListModel::setData() out of defined editable columns");

	emit dataChanged(index, index);

	return true;
}

//TODO searching
QString FontListModel::nameForIndex(const QModelIndex & index)
{
//	return m_fonts.keys().at(index.row());
	return m_font_keys.at(index.row());
}
