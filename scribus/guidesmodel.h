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


/*! \brief A model for guides lists.
It holds guides as a double QList (it will be expanded later because of RFEs)
and it handles its values editing too. The editor from GuidesDelegate
is used.
See Qt4 documentation for more info about its methods.
FIXME: unit conversions for dipslay vs. internal
\author Petr Vanek <petr@scribus.info>
*/
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

		void removeValues(const Guides & v);
		// qt4 api is "wrokarounded" with insertRow() and removeValues()
		// these removeRows() and insertRows() does not handle margin items correctly
// 		bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
// 		bool insertRows( int row, int count, const QModelIndex & parent = QModelIndex());
		void insertRow();

		//! \brief Set new values into the model.
		void setValues(Guides values);
		//! \brief Get values back to the app.
		Guides values();

		void unitChange(int docUnitIndex, int docUnitDecimals);
#if 0
		//! debug only
		void printValues();
#endif

	signals:
		/*! \brief Signal emmitted when the user finish the editation of one value.
		*/
		void valueChanged();

	private:
		Guides m_values;
		int m_docUnitIndex;
		int m_docUnitDecimals;
};

#endif
