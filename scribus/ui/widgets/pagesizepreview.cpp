#include "pagesizepreview.h"
#include "prefsmanager.h"

#include <QFontMetrics>

PageSizePreview::PageSizePreview(QWidget *parent) : QWidget{parent}
{
	setMinimumSize(120, 120);
}

void PageSizePreview::paintEvent(QPaintEvent *event)
{
	QColor colPage = PrefsManager::instance().appPrefs.displayPrefs.paperColor;
	QColor colBackground = PrefsManager::instance().appPrefs.displayPrefs.scratchColor;
	QColor colMargin(Qt::blue);
	QColor colFrame(Qt::black);
	QColor colLabel(Qt::white);
	QColor colLabelBackground(0, 0, 0, 128);

	int count = m_layout + 1;
	int space = 8;
	QRect saveArea(rect().adjusted(space, space, -space, -space));
	double ratio = qMin( saveArea.height() / m_height, saveArea.width() / (m_width * count) );
	double pageWidth = m_width * ratio;
	double pageHeight = m_height * ratio;
	QSize pageSize(pageWidth, pageHeight);
	QRect rectPaper(0, 0, pageSize.width() * count, pageSize.height());
	rectPaper.setLeft( space + (saveArea.width() - rectPaper.width()) / 2 );
	rectPaper.setTop( space + (saveArea.height() - rectPaper.height()) / 2 );

	QPainter painter(this);
	painter.fillRect( rect(), QBrush(colBackground) );

	for (int i = 0; i < count; i++)
	{
		QRect rPage(rectPaper.x() + pageSize.width() * i , rectPaper.y(), pageSize.width(), pageSize.height());
		QRect rMargin;

		// Left Page
		if (i == 0 && count > 1)
		{
			rMargin.setLeft(rPage.left() + m_margins.right() * ratio);
			rMargin.setRight(rPage.right() - m_margins.left() * ratio);	
		}
		// Right and Single Page
		else
		{
			rMargin.setLeft(rPage.left() + m_margins.left() * ratio);
			rMargin.setRight(rPage.right() - m_margins.right() * ratio);
		}

		rMargin.setTop(rPage.top() + m_margins.top() * ratio);
		rMargin.setBottom(rPage.bottom() - m_margins.bottom() * ratio);


		// Draw Pages
		painter.setBrush( colPage );
		painter.setPen( QPen(colFrame) );
		painter.drawRect( rPage );

		// Draw Margins
		painter.setBrush( Qt::NoBrush );
		painter.setPen( QPen(colMargin) );
		painter.drawRect( rMargin );

		painter.setPen( QPen(colFrame) );
		if (m_firstPage == i && count > 1)
			painter.drawText(rPage, Qt::AlignCenter | Qt::AlignVCenter, QString::number(1));

	}

	// Draw label
	painter.setBackgroundMode(Qt::OpaqueMode);
	painter.setBackground(colLabelBackground);
	painter.setPen(QPen(colLabel));
	painter.drawText(saveArea, Qt::AlignLeft | Qt::AlignBottom |Qt::TextWordWrap, " " + m_name + " ");
}
