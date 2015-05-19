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
#include "scribusdoc.h"
#include "commonstrings.h"


FontListModel::FontListModel(QObject * parent, ScribusDoc * doc)
	: QAbstractTableModel(parent),
	m_doc(doc),
	m_fonts(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts)
{
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	substFont = loadIcon("font_subst16.png");
	m_font_values = m_fonts.values();
    m_font_names = m_fonts.keys();
}

void FontListModel::setFonts(QList<QString> f)
{
	beginResetModel();
    m_font_names = f;
    m_font_values.clear();
    m_embedMethod.clear();
    for (int i = 0; i < m_font_names.length(); ++i)
    {
        ScFace font = m_fonts[m_font_names[i]];
        m_font_values.append(font);
        m_embedMethod.append(font.subset()? SubsetFont : font.embedPs()? EmbedFont : DontEmbed);
        
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
	return 10;
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
//            case FontListModel::FontFamily:
//                return tr("Family");
//            case FontListModel::FontStyle:
//                return tr("Style");
//            case FontListModel::FontVariant:
//                return tr("Variant");
            case FontListModel::FontType:
                return isDisplayRole? QString("a") : tr("Type");
            case FontListModel::FontFormat:
                return isDisplayRole? QString("b") : tr("Format");
            case FontListModel::FontEmbed:
                return isDisplayRole? QString("c") : tr("Embed in PostScript");
            case FontListModel::FontSubset:
                return isDisplayRole? QString("d") : tr("Subset");
            case FontListModel::FontOutline:
                return isDisplayRole? QString("e") : tr("Outline");
            case FontListModel::FontAccess:
                return isDisplayRole? QString("f") : tr("Access");
            case FontListModel::FontInDoc:
                return isDisplayRole? QString("g") : tr("Used in Doc");
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
//	ScFace font = m_fonts[m_fonts.keys().at(index.row())];
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
//			case FontListModel::FontFamily:
//				return font.family();
//			case FontListModel::FontStyle:
//				return font.style();
//			case FontListModel::FontVariant:
//				return font.variant();
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
                     || index.column() == FontListModel::FontEmbed
                     || index.column() == FontListModel::FontSubset
                     || index.column() == FontListModel::FontOutline
                     )
                    )
                {
                    return tr("Click to change the value");
                }
				return QVariant();
		};
	}

	
    
	if (role == Qt::CheckStateRole && index.column() == FontListModel::FontUsable)
		return !isLive() || (font.usable() ? Qt::Checked : Qt::Unchecked);
	if (role == Qt::CheckStateRole && index.column() == FontListModel::FontEmbed)
    {
        if (isLive())
            return (font.embedPs() ? Qt::Checked : Qt::Unchecked);
    else
        return (m_embedMethod[index.row()] >= EmbedFont ? Qt::Checked : Qt::Unchecked);
    }
	if (role == Qt::CheckStateRole && index.column() == FontListModel::FontSubset)
    {
        if (isLive())
            return (font.subset() ? Qt::Checked : Qt::Unchecked);
        else
            return (m_embedMethod[index.row()] >= SubsetFont ? Qt::Checked : Qt::Unchecked);
    }
    if (role == Qt::CheckStateRole && index.column() == FontListModel::FontOutline)
    {
        if (isLive())
            return (font.outline() ? Qt::Checked : Qt::Unchecked);
        else
            return (m_embedMethod[index.row()] >= OutlineFont ? Qt::Checked : Qt::Unchecked);
    }

	return QVariant();
}

Qt::ItemFlags FontListModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (!index.isValid())
		return QAbstractTableModel::flags(index);
	if (index.column() == FontListModel::FontUsable
		   || index.column() == FontListModel::FontEmbed
           || index.column() == FontListModel::FontOutline
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
        else if (idx.column() == FontListModel::FontEmbed)
            f.embedPs(!f.embedPs());
        else if (idx.column() == FontListModel::FontSubset)
            f.subset(!f.subset());
        else if (idx.column() == FontListModel::FontOutline)
            f.outline(!f.outline());
        else
            qDebug("FontListModel::setData() out of defined editable columns");
    }
    else
    {
        if (idx.column() == FontListModel::FontEmbed)
        {
            if (m_embedMethod[idx.row()] == EmbedFont)
                m_embedMethod[idx.row()] = DontEmbed;
            else
                m_embedMethod[idx.row()] = EmbedFont;
        }
        else if (idx.column() == FontListModel::FontSubset)
        {
            if (m_embedMethod[idx.row()] == SubsetFont)
                m_embedMethod[idx.row()] = EmbedFont;
            else
                m_embedMethod[idx.row()] = SubsetFont;
        }
        else if (idx.column() == FontListModel::FontOutline)
        {
            if (m_embedMethod[idx.row()] == OutlineFont)
                m_embedMethod[idx.row()] = SubsetFont;
            else
                m_embedMethod[idx.row()] = OutlineFont;
        }else
            qDebug("FontListModel::setData() out of defined editable columns");
    }
    emit dataChanged(index(idx.row(),FontListModel::FontEmbed), index(idx.row(),FontListModel::FontOutline));

	return true;
}

QString FontListModel::nameForIndex(const QModelIndex & idx)
{
	return m_font_names.at(idx.row());
}
