#include "gradientlistmodel.h"
//#include "gradientlistbox.h"
#include "commonstrings.h"
#include "scribusdoc.h"

GradientPixmapValue::GradientPixmapValue(const GradientPixmapValue& other)
				: m_gradient(other.m_gradient),
				  m_doc(other.m_doc),
				  m_name(other.m_name)
{

}

GradientPixmapValue& GradientPixmapValue::operator= (const GradientPixmapValue& other)
{
	m_gradient = other.m_gradient;
	m_doc = other.m_doc;
	m_name = other.m_name;
	return *this;
}

GradientPixmapValue::GradientPixmapValue(const VGradient& gradient, ScribusDoc* doc, const QString& gradientName)
				: m_gradient(gradient),
				  m_name(gradientName)
{
	m_doc = (doc) ? doc->guardedPtr() : nullptr;
}

GradientListModel::GradientListModel(QObject *parent)
			  : QAbstractItemModel(parent)
{

}

void GradientListModel::clear()
{
	beginResetModel();
	m_gradients.clear();
	endResetModel();
}

int GradientListModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 1;
}

QVariant GradientListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	const auto* pColorValue = static_cast<GradientPixmapValue*>(index.internalPointer());
	if (!pColorValue)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		if (pColorValue->m_name == CommonStrings::trCustom)
			return CommonStrings::trCustom;
		return pColorValue->m_name;
	}

	if (role == Qt::ToolTipRole)
	{
		const VGradient& gradient = pColorValue->m_gradient;
		if (pColorValue->m_name == CommonStrings::trCustom)
			return QVariant();

		switch(gradient.type())
		{
		case VGradient::diamond:
			return tr("Diamond");
			break;
		case VGradient::fourcolor:
			return tr("4-Color");
			break;
		case VGradient::freemesh:
			return tr("Free Mesh");
			break;
		case VGradient::linear:
			return tr("Linear");
			break;
		case VGradient::mesh:
			return tr("Mesh");
			break;
		case VGradient::radial:
			return tr("Radial");
			break;
		}

		return QVariant();
	}

	if (role == Qt::UserRole)
	{
		if (pColorValue->m_name == CommonStrings::trCustom)
			return CommonStrings::trCustom;
		return QVariant::fromValue(*pColorValue);
	}

	return QVariant();
}

Qt::ItemFlags GradientListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemFlags();

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return flags;
}

QModelIndex GradientListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	const auto* pColorValue = static_cast<GradientPixmapValue*>(parent.internalPointer());
	if (pColorValue)
		return QModelIndex();

	if (row < 0 || (row >= rowCount()) || (column != 0))
		return QModelIndex();

	const GradientPixmapValue& colorValue = m_gradients.at(row);
	return createIndex(row, column, const_cast<GradientPixmapValue*>(&colorValue));
}

void GradientListModel::insert(int i, const GradientPixmapValue& value)
{
	beginInsertRows(QModelIndex(), i, i);
	m_gradients.insert(i, value);
	endInsertRows();
}

QModelIndex GradientListModel::parent(const QModelIndex &/*child*/) const
{
	return QModelIndex();
}

bool GradientListModel::removeRow(int row, const QModelIndex& parent)
{
	if (row < 0 || row >= rowCount())
		return false;

	beginRemoveRows(parent, row, row);
	m_gradients.remove(row);
	endRemoveRows();

	return true;
}

bool GradientListModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (row < 0 || row >= rowCount())
		return false;
	if (count <= 0)
		return false;

	beginRemoveRows(parent, row, row + count - 1);
	m_gradients.remove(row, count);
	endRemoveRows();

	return true;
}

int GradientListModel::rowCount(const QModelIndex &parent) const
{
	if (m_gradients.count() == 0)
		return 0;

	const auto* pColorValue = static_cast<GradientPixmapValue*>(parent.internalPointer());
	if (pColorValue)
		return 0;

	return m_gradients.count();
}

void GradientListModel::setGradientList(const GradientList &gradientList)
{
	setGradientList(gradientList, m_isNoneColorShown);
}

void GradientListModel::setGradientList(const GradientList& gradientList, bool showNone)
{
	ScribusDoc* doc = gradientList.document();

	beginResetModel();

	m_isNoneColorShown = showNone;
	m_gradients.clear();
	m_gradients.reserve(gradientList.count());

	if (m_isNoneColorShown)
		m_gradients.append(GradientPixmapValue(VGradient(), nullptr, CommonStrings::trCustom));

	GradientList::const_iterator iter;
	for (iter = gradientList.begin(); iter != gradientList.end(); ++iter)
	{
		const QString& colorName = iter.key();
		const VGradient& gradient = iter.value();
		m_gradients.append(GradientPixmapValue(gradient, doc, colorName));
	}

	if (m_sortRule != SortByName)
	{
		if (m_sortRule == SortByType)
			std::sort(m_gradients.begin(), m_gradients.end(), compareGradientTypes);
	}

	endResetModel();
}

void GradientListModel::setShowNoneColor(bool showNone)
{
	if (m_isNoneColorShown == showNone)
		return;

	beginResetModel();
	m_isNoneColorShown = showNone;
	endResetModel();
}

void GradientListModel::setSortRule(SortRule sortRule)
{
	if (m_sortRule == sortRule)
		return;

	beginResetModel();

	m_sortRule = sortRule;
	if (m_sortRule == SortByType)
		std::sort(m_gradients.begin(), m_gradients.end(), compareGradientTypes);
	else
		std::sort(m_gradients.begin(), m_gradients.end(), compareGradientNames);

	endResetModel();
}

bool GradientListModel::compareGradientNames(const GradientPixmapValue& v1, const GradientPixmapValue& v2)
{
	if (v1.m_name == CommonStrings::trCustom)
		return true;
	if (v2.m_name == CommonStrings::trCustom)
		return false;

	return (v1.m_name < v2.m_name);
}

bool GradientListModel::compareGradientTypes(const GradientPixmapValue& v1, const GradientPixmapValue& v2)
{
	if (v1.m_name == CommonStrings::trCustom)
		return true;
	if (v2.m_name == CommonStrings::trCustom)
		return false;

	QString sortString1("%1-%2");
	QString sortString2("%1-%2");

	switch(v1.m_gradient.type())
	{
	case VGradient::linear:
		sortString1 = sortString1.arg("A", v1.m_name);
		break;
	case VGradient::radial:
		sortString1 = sortString1.arg("B", v1.m_name);
		break;
	case VGradient::diamond:
		sortString1 = sortString1.arg("C", v1.m_name);
		break;
	case VGradient::fourcolor:
		sortString1 = sortString1.arg("D", v1.m_name);
		break;
	case VGradient::freemesh:
		sortString1 = sortString1.arg("E", v1.m_name);
		break;
	case VGradient::mesh:
		sortString1 = sortString1.arg("F", v1.m_name);
		break;
	}

	switch(v2.m_gradient.type())
	{
	case VGradient::linear:
		sortString2 = sortString2.arg("A", v2.m_name);
		break;
	case VGradient::radial:
		sortString2 = sortString2.arg("B", v2.m_name);
		break;
	case VGradient::diamond:
		sortString2 = sortString2.arg("C", v2.m_name);
		break;
	case VGradient::fourcolor:
		sortString2 = sortString2.arg("D", v2.m_name);
		break;
	case VGradient::freemesh:
		sortString2 = sortString2.arg("E", v2.m_name);
		break;
	case VGradient::mesh:
		sortString2 = sortString2.arg("F", v2.m_name);
		break;
	}

	return (sortString1 < sortString2);
}
