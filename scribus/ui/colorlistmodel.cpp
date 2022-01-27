/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "colorlistmodel.h"

#include "colorlistbox.h"
#include "commonstrings.h"

ColorPixmapValue::ColorPixmapValue(const ColorPixmapValue& other)
	            : m_color(other.m_color),
	              m_doc(other.m_doc),
	              m_name(other.m_name)
{

}

ColorPixmapValue& ColorPixmapValue::operator= (const ColorPixmapValue& other)
{
	m_color = other.m_color;
	m_doc = other.m_doc;
	m_name = other.m_name;
	return *this;
}

ColorPixmapValue::ColorPixmapValue(const ScColor& col, ScribusDoc* doc, const QString& colName)
                : m_color(col),
	              m_name(colName)
{
	m_doc = (doc) ? doc->guardedPtr() : nullptr;
}

ColorListModel::ColorListModel(QObject *parent)
	          : QAbstractItemModel(parent)
{

}

void ColorListModel::clear()
{
	beginResetModel();
	m_colors.clear();
	endResetModel();
}

int ColorListModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 1;
}

QVariant ColorListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	const auto* pColorValue = static_cast<ColorPixmapValue*>(index.internalPointer());
	if (!pColorValue)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		if (pColorValue->m_name == CommonStrings::None)
			return CommonStrings::tr_NoneColor;
		return pColorValue->m_name;
	}

	if (role == Qt::ToolTipRole)
	{
		const ScColor& color = pColorValue->m_color;
		if (pColorValue->m_name == CommonStrings::None)
			return QVariant();
		if (color.getColorModel() == colorModelRGB)
		{
			int r, g, b;
			color.getRawRGBColor(&r, &g, &b);
			return tr("R: %1 G: %2 B: %3").arg(r).arg(g).arg(b);
		}
		if (color.getColorModel() == colorModelCMYK)
		{
			double c, m, y, k;
			color.getCMYK(&c, &m, &y, &k);
			return tr("C: %1% M: %2% Y: %3% K: %4%").arg(c * 100, 0, 'f', 2).arg(m * 100, 0, 'f', 2).arg(y * 100, 0, 'f', 2).arg(k * 100, 0, 'f', 2);
		}
		if (color.getColorModel() == colorModelLab)
		{
			double L, a, b;
			color.getLab(&L, &a, &b);
			return tr("L: %1 a: %2 b: %3").arg(L, 0, 'f', 2).arg(a, 0, 'f', 2).arg(b, 0, 'f', 2);
		}
		return QVariant();
	}

	if (role == Qt::UserRole)
	{
		if (pColorValue->m_name == CommonStrings::None)
			return CommonStrings::None;
		return QVariant::fromValue(*pColorValue);
	}

	return QVariant();
}

Qt::ItemFlags ColorListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemFlags();

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return flags;
}

QModelIndex ColorListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	const auto* pColorValue = static_cast<ColorPixmapValue*>(parent.internalPointer());
	if (pColorValue)
		return QModelIndex();

	if (row < 0 || (row >= rowCount()) || (column != 0))
		return QModelIndex();

	const ColorPixmapValue& colorValue = m_colors.at(row);
	return createIndex(row, column, const_cast<ColorPixmapValue*>(&colorValue));
}

void ColorListModel::insert(int i, const ColorPixmapValue& value)
{
	beginInsertRows(QModelIndex(), i, i);
	m_colors.insert(i, value);
	endInsertRows();
}

QModelIndex ColorListModel::parent(const QModelIndex &/*child*/) const
{
	return QModelIndex();
}

bool ColorListModel::removeRow(int row, const QModelIndex& parent)
{
	if (row < 0 || row >= rowCount())
		return false;

	beginRemoveRows(parent, row, row);
	m_colors.remove(row);
	endRemoveRows();

	return true;
}

bool ColorListModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (row < 0 || row >= rowCount())
		return false;
	if (count <= 0)
		return false;

	beginRemoveRows(parent, row, row + count - 1);
	m_colors.remove(row, count);
	endRemoveRows();

	return true;
}

int ColorListModel::rowCount(const QModelIndex &parent) const
{
	if (m_colors.count() == 0)
		return 0;

	const auto* pColorValue = static_cast<ColorPixmapValue*>(parent.internalPointer());
	if (pColorValue)
		return 0;

	return m_colors.count();
}

void ColorListModel::setColorList(const ColorList& colorList)
{
	setColorList(colorList, m_isNoneColorShown);
}

void ColorListModel::setColorList(const ColorList& colorList, bool showNone)
{
	ScribusDoc* doc = colorList.document();

	beginResetModel();

	m_isNoneColorShown = showNone;
	m_colors.clear();
	m_colors.reserve(colorList.count());

	if (m_isNoneColorShown)
		m_colors.append(ColorPixmapValue(ScColor(), nullptr, CommonStrings::None));

	ColorList::const_iterator iter;
	for (iter = colorList.begin(); iter != colorList.end(); ++iter)
	{
		const QString& colorName = iter.key();
		const ScColor& color = iter.value();
		m_colors.append(ColorPixmapValue(color, doc, colorName));
	}

	if (m_sortRule != SortByName)
	{
		if (m_sortRule == SortByValues)
			std::sort(m_colors.begin(), m_colors.end(), compareColorValues);
		else if (m_sortRule == SortByType)
			std::sort(m_colors.begin(), m_colors.end(), compareColorTypes);
	}

	endResetModel();
}

void ColorListModel::setShowNoneColor(bool showNone)
{
	if (m_isNoneColorShown == showNone)
		return;

	beginResetModel();
	m_isNoneColorShown = showNone;
	endResetModel();
}

void ColorListModel::setSortRule(SortRule sortRule)
{
	if (m_sortRule == sortRule)
		return;

	beginResetModel();

	m_sortRule = sortRule;
	if (m_sortRule == SortByValues)
		std::sort(m_colors.begin(), m_colors.end(), compareColorValues);
	else if (m_sortRule == SortByType)
		std::sort(m_colors.begin(), m_colors.end(), compareColorTypes);
	else
		std::sort(m_colors.begin(), m_colors.end(), compareColorNames);

	endResetModel();
}

bool ColorListModel::compareColorNames(const ColorPixmapValue& v1, const ColorPixmapValue& v2)
{
	if (v1.m_name == CommonStrings::None || v1.m_name == CommonStrings::tr_None)
		return true;
	if (v2.m_name == CommonStrings::None || v2.m_name == CommonStrings::tr_None)
		return false;

	return (v1.m_name < v2.m_name);
}

bool ColorListModel::compareColorValues(const ColorPixmapValue& v1, const ColorPixmapValue& v2)
{
	if (v1.m_name == CommonStrings::None || v1.m_name == CommonStrings::tr_None)
		return true;
	if (v2.m_name == CommonStrings::None || v2.m_name == CommonStrings::tr_None)
		return false;

	QColor c1 = v1.m_color.getRawRGBColor();
	QColor c2 = v2.m_color.getRawRGBColor();

	QString sortString1 = QString("%1-%2-%3-%4").arg(c1.hue(), 3, 10, QChar('0')).arg(c1.saturation(), 3, 10, QChar('0')).arg(c1.value(), 3, 10, QChar('0')).arg(v1.m_name);
	QString sortString2 = QString("%1-%2-%3-%4").arg(c2.hue(), 3, 10, QChar('0')).arg(c2.saturation(), 3, 10, QChar('0')).arg(c2.value(), 3, 10, QChar('0')).arg(v2.m_name);
	return (sortString1 < sortString2);
}

bool ColorListModel::compareColorTypes(const ColorPixmapValue& v1, const ColorPixmapValue& v2)
{
	if (v1.m_name == CommonStrings::None || v1.m_name == CommonStrings::tr_None)
		return true;
	if (v2.m_name == CommonStrings::None || v2.m_name == CommonStrings::tr_None)
		return false;

	QString sortString1("%1-%2");
	QString sortString2("%1-%2");

	if (v1.m_color.isRegistrationColor())
		sortString1 = sortString1.arg("A", v1.m_name);
	else if (v1.m_color.isSpotColor())
		sortString1 = sortString1.arg("B", v1.m_name);
	else if (v1.m_color.getColorModel() == colorModelCMYK)
		sortString1 = sortString1.arg("C", v1.m_name);
	else
		sortString1 = sortString1.arg("D", v1.m_name);

	if (v2.m_color.isRegistrationColor())
		sortString2 = sortString2.arg("A", v2.m_name);
	else if (v2.m_color.isSpotColor())
		sortString2 = sortString2.arg("B", v2.m_name);
	else if (v2.m_color.getColorModel() == colorModelCMYK)
		sortString2 = sortString2.arg("C", v2.m_name);
	else
		sortString2 = sortString2.arg("D", v2.m_name);

	return (sortString1 < sortString2);
}
