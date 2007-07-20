/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GUIDESMODEL_H
#define GUIDESMODEL_H

#include <QAbstractTableModel>
#include "scribusstructs.h"


class GuidesModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		GuidesModel(QObject * parent = 0);
		~GuidesModel();

		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;

		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

		Qt::ItemFlags flags(const QModelIndex & index) const;

		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
		bool insertRows( int row, int count, const QModelIndex & parent = QModelIndex());
		void insertRow();

		void setValues(Guides values);
		Guides values();
		// debug
		void printValues();

	signals:
		void drawGuides();

	private:
		Guides m_values;
};

#endif
