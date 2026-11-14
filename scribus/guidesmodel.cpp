/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <algorithm>
#include <QLocale>

#include "guidesmodel.h"
#include "units.h"


GuidesModel::GuidesModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

GuidesModel::~GuidesModel() = default;

int GuidesModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : m_values.count();
}

int GuidesModel::columnCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : 1;
}

QVariant GuidesModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid() || index.model() != this || index.column() != 0 || index.row() < 0 || index.row() >= m_values.count())
		return QVariant();
	// Display is rounded to the document's precision; editing uses the value.
	if (role == Qt::DisplayRole)
	{
		QLocale l;
		return QVariant(l.toString(pts2value(m_values.at(index.row()) - m_rule, m_docUnitIndex), 'f', m_docUnitDecimals) + unitGetSuffixFromIndex(m_docUnitIndex));
	}
	if (role == Qt::EditRole)
		return pts2value(m_values.at(index.row()) - m_rule, m_docUnitIndex);
	if (role == Qt::UserRole)
		return m_values.at(index.row());

	return QVariant();
}

bool GuidesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role != Qt::EditRole || !index.isValid() || index.model() != this || index.column() != 0 || index.row() < 0 || index.row() >= m_values.count())
		return false;
	bool ok;
	double newVal = value.toDouble(&ok);
	if (!ok)
		return false;

	const int oldRow = index.row();
	const double oldPos = m_values.at(oldRow);
	const double newPos = value2pts(newVal, m_docUnitIndex) + m_rule;
	const bool pending = (oldRow == m_pendingRow);
	// Commit a new row even at zero; unchanged existing guides must be no-ops.
	if (!pending)
	{
		QLocale locale;
		const double shownValue = locale.toDouble(locale.toString(pts2value(oldPos - m_rule, m_docUnitIndex), 'f', m_docUnitDecimals));
		if (oldPos == newPos || shownValue == newVal)
			return true;
	}

	// Ignore the pending row itself when checking for duplicate positions.
	const bool occupied = m_values.contains(newPos) && (!pending || newPos != oldPos || m_values.count(newPos) > 1);
	if (occupied)
	{
		beginRemoveRows(QModelIndex(), oldRow, oldRow);
		m_values.removeAt(oldRow);
		if (pending)
			m_pendingRow = -1;
		endRemoveRows();
	}
	else
	{
		int newRow = 0;
		for (int i = 0; i < m_values.count(); ++i)
		{
			if (i != oldRow && m_values.at(i) < newPos)
				++newRow;
		}
		if (newRow != oldRow)
		{
			const int destination = newRow > oldRow ? newRow + 1 : newRow;
			beginMoveRows(QModelIndex(), oldRow, oldRow, QModelIndex(), destination);
			m_values.removeAt(oldRow);
			m_values.insert(newRow, newPos);
			endMoveRows();
		}
		else
			m_values[oldRow] = newPos;
		if (pending)
			m_pendingRow = -1;
		const QModelIndex changed = this->index(newRow, 0);
		emit dataChanged(changed, changed);
	}
	if (pending)
	{
		if (!occupied)
			emit guideAdded(newPos);
	}
	else
		emit valueChanged(oldPos, newPos);
	return true;
}

Qt::ItemFlags GuidesModel::flags(const QModelIndex &index) const
{
	if (!index.isValid() || index.model() != this || index.column() != 0 || index.row() < 0 || index.row() >= m_values.count())
		return Qt::NoItemFlags;
	return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

QVariant GuidesModel::headerData(int /*section*/, Qt::Orientation orientation, int /*role*/) const
{
	if (orientation == Qt::Horizontal)
		return "Value";
	return "";
}

void GuidesModel::removeValues(const Guides & guides)
{
	if (guides.isEmpty())
		return;
	// Keep an unfinished editor row (and its index) when removing other guides.
	for (int row = m_values.count() - 1; row >= 0; --row)
	{
		if (row == m_pendingRow || !guides.contains(m_values.at(row)))
			continue;
		beginRemoveRows(QModelIndex(), row, row);
		m_values.removeAt(row);
		if (m_pendingRow > row)
			--m_pendingRow;
		endRemoveRows();
	}
}

int GuidesModel::insertRow()
{
	if (m_pendingRow >= 0)
		return m_pendingRow;
	// A temporary editor row starts at ruler-relative zero, not a committed guide.
	beginInsertRows(QModelIndex(), 0, 0);
	m_values.prepend(m_rule);
	m_pendingRow = 0;
	endInsertRows();
	return m_pendingRow;
}

void GuidesModel::discardPendingRow()
{
	if (m_pendingRow < 0)
		return;
	beginRemoveRows(QModelIndex(), m_pendingRow, m_pendingRow);
	m_values.removeAt(m_pendingRow);
	m_pendingRow = -1;
	endRemoveRows();
}

void GuidesModel::setValues(const Guides& values)
{
	beginResetModel();
	m_values = values;
	std::sort(m_values.begin(), m_values.end());
	m_pendingRow = -1;
	endResetModel();
}

const Guides& GuidesModel::values() const
{
	return m_values;
}

void GuidesModel::unitChange(int docUnitIndex, int docUnitDecimals,double offset)
{
	beginResetModel();
	m_rule = offset;
	m_docUnitIndex = docUnitIndex;
	m_docUnitDecimals = docUnitDecimals;
	endResetModel();
}
