#ifndef GRADIENTLISTMODEL_H
#define GRADIENTLISTMODEL_H


#include <QAbstractItemModel>
#include <QVector>

#include "scguardedptr.h"
#include "scribusapi.h"
#include "vgradient.h"

class ScribusDoc;

struct SCRIBUS_API GradientPixmapValue
{
	VGradient m_gradient;
	ScGuardedPtr<ScribusDoc> m_doc;
	QString m_name { "invalid" };

	GradientPixmapValue() = default;
	GradientPixmapValue(const VGradient& gradient, ScribusDoc* doc, const QString& gradientName);
	GradientPixmapValue(const GradientPixmapValue& other);
	GradientPixmapValue& operator= (const GradientPixmapValue& other);
};

Q_DECLARE_METATYPE(GradientPixmapValue)

class SCRIBUS_API GradientListModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	//! Constructor
	GradientListModel(QObject* parent = nullptr);

	enum SortRule
	{
		SortByName,
		SortByType
	};

	//! Returns the color list
	const QVector<GradientPixmapValue>& gradients() const { return m_gradients; }

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
	void insert(int i, const GradientPixmapValue& value);

	//! Reimplement QAbstractItemModel parent()
	QModelIndex parent(const QModelIndex &child) const;

	//! Reimplement QAbstractItemModel removeRow()
	bool removeRow(int row, const QModelIndex& parent = QModelIndex());

	//! Reimplement QAbstractItemModel removeRowz()
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

	//! Reimplement QAbstractItemModel rowCount()
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	//! Set color list
	void setGradientList(const GradientList& gradientList);

	//! Set color list and None color option at same time
	void setGradientList(const GradientList &gradientList, bool showNone);

	//! Set if None color is shown as first item in the list
	void setShowNoneColor(bool showNone);

	//! Set how colors should be sorted
	void setSortRule(SortRule sortRule);

protected:
	//! If None color is shown as first item in the list
	bool m_isNoneColorShown { false };

	//! The color list
	QVector<GradientPixmapValue> m_gradients;

	// SortRule m_sortRule
	SortRule m_sortRule { SortByName };

	static bool compareGradientNames(const GradientPixmapValue& v1, const GradientPixmapValue& v2);
	static bool compareGradientTypes(const GradientPixmapValue& v1, const GradientPixmapValue& v2);
};

#endif // GRADIENTLISTMODEL_H
