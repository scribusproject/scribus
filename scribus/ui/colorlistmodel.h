/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef COLORLISTMODEL_H
#define COLORLISTMODEL_H

#include <QAbstractItemModel>
#include <QVector>

#include "sccolor.h"
#include "scguardedptr.h"
#include "scribusapi.h"

class ScribusDoc;

struct SCRIBUS_API ColorPixmapValue
{
	ScColor m_color;
	ScGuardedPtr<ScribusDoc> m_doc;
	QString m_name { "invalid" };

	ColorPixmapValue() = default;
	ColorPixmapValue(const ScColor& col, ScribusDoc* doc, const QString& colName);
	ColorPixmapValue(const ColorPixmapValue& other);
	ColorPixmapValue& operator= (const ColorPixmapValue& other);
};

Q_DECLARE_METATYPE(ColorPixmapValue)

class SCRIBUS_API ColorListModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	//! Constructor
	ColorListModel(QObject* parent = nullptr);

	enum SortRule
	{
		SortByName,
		SortByValues,
		SortByType
	};

	//! Returns the color list
	const QVector<ColorPixmapValue>& colors() const { return m_colors; }

	//! Returns if None color is shown as first item in the list
	bool isNoneColorShown() const { return m_isNoneColorShown; }

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

	//! Insert an item in the list at specified position
	void insert(int i, const ColorPixmapValue& value);

	//! Reimplement QAbstractItemModel parent()
	QModelIndex parent(const QModelIndex &child) const;

	//! Reimplement QAbstractItemModel removeRow()
	bool removeRow(int row, const QModelIndex& parent = QModelIndex());

	//! Reimplement QAbstractItemModel removeRowz()
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

	//! Reimplement QAbstractItemModel rowCount()
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	//! Set color list
	void setColorList(const ColorList& colorList);

	//! Set color list and None color option at same time
	void setColorList(const ColorList& colorList, bool showNone);

	//! Set if None color is shown as first item in the list
	void setShowNoneColor(bool showNone);

	//! Set how colors should be sorted
	void setSortRule(SortRule sortRule);

protected:
	//! If None color is shown as first item in the list
	bool m_isNoneColorShown { false };

	//! The color list
	QVector<ColorPixmapValue> m_colors;

	// SortRule m_sortRule
	SortRule m_sortRule { SortByName };

	static bool compareColorNames(const ColorPixmapValue& v1, const ColorPixmapValue& v2);
	static bool compareColorValues(const ColorPixmapValue& v1, const ColorPixmapValue& v2);
	static bool compareColorTypes(const ColorPixmapValue& v1, const ColorPixmapValue& v2);
};

#endif // COLORLISTMODEL_H