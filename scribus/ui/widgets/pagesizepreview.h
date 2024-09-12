#ifndef PAGESIZEPREVIEW_H
#define PAGESIZEPREVIEW_H

#include "margins.h"
#include "pagesize.h"
#include <QWidget>

class PageSizePreview : public QWidget
{
	Q_OBJECT
public:
	explicit PageSizePreview(QWidget *parent = nullptr);

	void setPageHeight(double height) { m_height = height; update(); };
	void setPageWidth(double width) { m_width = width; update(); };
	void setMargins(MarginStruct margins) { m_margins = margins; update(); };
	void setBleeds(MarginStruct bleeds) { m_bleeds = bleeds; update(); };
	void setPageName(QString name) {
		PageSize ps(name);
		m_name = ps.nameTR();
		update();
	};
	void setLayout(int layout) { m_layout = layout; update(); };
	void setFirstPage(int firstPage) { m_firstPage = firstPage; update(); };
	void setPage(double height, double width, MarginStruct margins, MarginStruct bleeds, QString name, int layout, int firstPage)
	{
		PageSize ps(name);

		m_height = height;
		m_width = width;
		m_margins = margins;
		m_bleeds = bleeds;
		m_name = ps.nameTR();
		m_layout = layout;
		m_firstPage = firstPage;
		update();
	}

private:
	MarginStruct m_margins;
	MarginStruct m_bleeds;
	double m_height;
	double m_width;
	QString m_name;
	int m_layout;
	int m_firstPage;

	void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // PAGESIZEPREVIEW_H
