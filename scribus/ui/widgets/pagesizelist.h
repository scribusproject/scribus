/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGESIZELIST_H
#define PAGESIZELIST_H

#include <QListView>
#include <QScrollBar>
#include <QStandardItemModel>

#include "pagesize.h"
#include "scribusapi.h"

class SCRIBUS_API PageSizeList : public QListView
{
	Q_OBJECT

public:

	enum SortMode {
		NameAsc = 0,
		NameDesc = 1,
		DimensionAsc = 2,
		DimensionDesc = 3
	};

	enum ItemData {
		SizeLabel = Qt::UserRole,
		Unit = Qt::UserRole + 1,
		Category = Qt::UserRole + 2,
		Name = Qt::UserRole + 3,
		Dimension = Qt::UserRole + 4,
	};

	PageSizeList(QWidget* parent);
	~PageSizeList() = default;

	void setFormat(QString format);
	const QString& format() const { return m_name; };

	void setOrientation(int orientation);
	int orientation() const { return m_orientation; };

	void setCategory(PageSizeInfo::Category category);
	PageSizeInfo::Category category() const { return m_category; };

	void setSortMode(SortMode sortMode);
	SortMode sortMode() const { return m_sortMode; };

	void setValues(QString format, int orientation, PageSizeInfo::Category category, SortMode sortMode);

	void updateGeometries() override;

private:
	QString m_name {PageSize::defaultSizesList().at(1)};
	int m_orientation {0};
	PageSizeInfo::Category m_category {PageSizeInfo::Preferred};
	SortMode m_sortMode {SortMode::NameAsc};
	QStandardItemModel* m_model { nullptr };

	QIcon sizePreview(QSize iconSize, QSize pageSize) const;
	void loadPageSizes(QString name, int orientation, PageSizeInfo::Category category);
};


#endif // PAGESIZELIST_H
