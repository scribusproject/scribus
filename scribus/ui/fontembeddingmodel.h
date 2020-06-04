/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTEMBEDDINGMODEL_H
#define FONTEMBEDDINGMODEL_H

#include <QAbstractItemModel>
#include <QVector>

#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API FontEmbeddingModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	//! Constructor
	FontEmbeddingModel(QObject* parent = nullptr);

	//! Remove all colors from list;
	void clear();

	//! Reimplement QAbstractItemModel columnCount()
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	//! Reimplement QAbstractItemModel data()
	QVariant data(const QModelIndex &index, int role) const;

	//! Reimplement QAbstractItemModel flags()
	Qt::ItemFlags flags(const QModelIndex &index) const;

	//! Reimplement QAbstractItemModel index()
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

	//! Reimplement QAbstractItemModel parent()
	QModelIndex parent(const QModelIndex &child) const;

	//! Reimplement QAbstractItemModel removeRow()
	bool removeRow(int row, const QModelIndex& parent = QModelIndex());

	//! Reimplement QAbstractItemModel removeRowz()
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

	//! Reimplement QAbstractItemModel rowCount()
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	
	//! Allow or disallow no font embedding mode
	void setNoFontEmbeddingEnabled(bool enabled);

protected:
	//! The color list
	QVector<bool> m_enabledVec;
};

#endif // FONTEMBEDDINGMODEL_H
