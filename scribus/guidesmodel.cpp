/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "guidesmodel.h"


GuidesModel::GuidesModel(QObject * /*parent*/)
	: QAbstractTableModel()
{
	// debug
// 	m_values << 1.0 << 10.1 << 6.3 << 4.1;
// 	qSort(m_values);
}


GuidesModel::~GuidesModel()
{
}

int GuidesModel::rowCount(const QModelIndex & /*parent*/) const
{
	return m_values.count();
}

int GuidesModel::columnCount(const QModelIndex & /*parent*/) const
{
	// for future RFEs - color per guide etc.
	return 1;
}

QVariant GuidesModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		QString s;
		return QVariant(s.setNum(m_values.at(index.row()), 'f', 6));
	}
	if (role == Qt::BackgroundColorRole && m_values.at(index.row()) == 0.0)
		return QVariant(Qt::red);
	return QVariant();
// 	return QAbstractTableModel::data(index, role);
}

bool GuidesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role != Qt::EditRole || !index.isValid())
		return false;
	bool ok;
	double newVal = value.toDouble(&ok);
	if (!ok)
		return false;
	m_values[index.row()] = newVal;
	qSort(m_values);
	emit dataChanged(index, index);
	emit valueChanged();
	return true;
}

Qt::ItemFlags GuidesModel::flags(const QModelIndex & /*index*/) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

QVariant GuidesModel::headerData(int /*section*/, Qt::Orientation orientation, int /*role*/) const
{
	if (orientation == Qt::Horizontal)
		return "Value";
	return "";
}

bool GuidesModel::removeRows(int row, int count, const QModelIndex & parent)
{
	if(count <= 0 || row < 0 || (row + count) > rowCount(parent))
		return false;

	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
		m_values.removeAt(row);
	endRemoveRows();
// 	emit drawGuides();
	return true;
}

bool GuidesModel::insertRows( int row, int count, const QModelIndex & parent)
{
	beginInsertRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
		m_values.insert(row + count, 0.0);
	endInsertRows();
// 	emit drawGuides();
	return true;
}

void GuidesModel::insertRow()
{
	insertRows(rowCount(), 1);
}

void GuidesModel::setValues(Guides values)
{
	m_values = values;
	reset();
}

Guides GuidesModel::values()
{
	return m_values;
}

// debug
#include <QtDebug>
void GuidesModel::printValues()
{
	qDebug() << "GuidesModel dump start";
	for (int i = 0; i < m_values.count(); ++i)
		qDebug() << "GuidesModel dump: " << m_values[i];
	qDebug() << "GuidesModel end of dump";
}
