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


/*! \brief A model for editing manual guide positions.
Positions are stored in document points and displayed relative to the ruler.
\author Petr Vanek <petr@scribus.info>
*/
class GuidesModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		GuidesModel(QObject * parent = nullptr);
		~GuidesModel();

		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;

		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

		Qt::ItemFlags flags(const QModelIndex & index) const;

		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		void removeValues(const Guides & guides);
		int insertRow();
		void discardPendingRow();

		//! \brief Set new values into the model.
		void setValues(const Guides& values);
		//! \brief Get values back to the app.
		const Guides& values() const;

		void unitChange(int docUnitIndex, int docUnitDecimals, double offset);

	signals:
		/*! \brief Signal emitted when the user finishes editing one value.
		*/
		void valueChanged(double oldPosition, double newPosition);
		void guideAdded(double position);

	private:
		Guides m_values;
		int m_pendingRow {-1};
		int m_docUnitIndex {0};
		int m_docUnitDecimals {0};
		double m_rule {0.0};
};

#endif
