/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDFVERSIONMODEL_H
#define PDFVERSIONMODEL_H

#include <QAbstractItemModel>
#include <QVector>

#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API PdfVersionModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	//! Constructor
	PdfVersionModel(QObject* parent = nullptr);
	
	enum PdfVersionItem
	{
		ItemPDF_13 = 0,
		ItemPDF_14 = 1,
		ItemPDF_15 = 2,
		ItemPDF_16 = 3,
		ItemPDFX_1a = 4,
		ItemPDFX_3  = 5,
		ItemPDFX_4  = 6
	};

	//! Reimplement QAbstractItemModel columnCount()
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	//! Reimplement QAbstractItemModel data()
	QVariant data(const QModelIndex &index, int role) const override;

	//! Reimplement QAbstractItemModel flags()
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	//! Reimplement QAbstractItemModel index()
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

	//! Reimplement QAbstractItemModel parent()
	QModelIndex parent(const QModelIndex &child) const override;

	//! Reimplement QAbstractItemModel removeRow()
	bool removeRow(int row, const QModelIndex& parent = QModelIndex());

	//! Reimplement QAbstractItemModel removeRowz()
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

	//! Reimplement QAbstractItemModel rowCount()
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	
	//! Allow or disallow no font embedding mode
	void setPdfXEnabled(bool enabled);

protected:
	//! The enabled/disabled flags
	QVector<bool> m_enabledVec;
};

#endif // PDFVERSIONMODEL_H
