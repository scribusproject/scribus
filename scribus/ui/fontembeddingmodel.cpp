/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "fontembeddingmodel.h"

#include "commonstrings.h"
#include "pdfoptions.h"

FontEmbeddingModel::FontEmbeddingModel(QObject *parent)
	          : QAbstractItemModel(parent)
{
	
	m_enabledVec << true << true << true;
}

void FontEmbeddingModel::clear()
{
	qDebug() << "FontEmbeddingModel: this model cannot be cleared";
}

int FontEmbeddingModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 1;
}

QVariant FontEmbeddingModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	bool* pEnabled = static_cast<bool*>(index.internalPointer());
	if (!pEnabled)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		int row = index.row();
		if (row == 0)
			return tr("Embed or Subset");
		if (row == 1)
			return tr("Outline All Fonts");
		if (row == 2)
			return tr("Do Not Embed");
		return QVariant();
	}

	return QVariant();
}

Qt::ItemFlags FontEmbeddingModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flags = Qt::NoItemFlags;
	if (m_enabledVec[index.row()])
		flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return flags;
}

QModelIndex FontEmbeddingModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	bool* pEnabled = static_cast<bool*>(parent.internalPointer());
	if (pEnabled)
		return QModelIndex();

	if (row < 0 || (row >= rowCount()) || (column != 0))
		return QModelIndex();

	const bool& modeEnabled = m_enabledVec.at(row);
	return createIndex(row, column, const_cast<bool*>(&modeEnabled));
}

QModelIndex FontEmbeddingModel::parent(const QModelIndex &/*child*/) const
{
	return QModelIndex();
}

bool FontEmbeddingModel::removeRow(int row, const QModelIndex& parent)
{
	qDebug() << "FontEmbeddingModel: this model cannot have rows removed";
	return false;
}

bool FontEmbeddingModel::removeRows(int row, int count, const QModelIndex& parent)
{
	qDebug() << "FontEmbeddingModel: this model cannot have rows removed";
	return false;
}

int FontEmbeddingModel::rowCount(const QModelIndex &parent) const
{
	if (m_enabledVec.count() == 0)
		return 0;

	bool* pEnabled = static_cast<bool*>(parent.internalPointer());
	if (pEnabled)
		return 0;

	return m_enabledVec.count();
}

void FontEmbeddingModel::setNoFontEmbeddingEnabled(bool enabled)
{
	//beginResetModel();
	m_enabledVec[2] = enabled;
	//endResetModel();
}
