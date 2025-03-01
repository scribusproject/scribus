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
	QColor colMargin = PrefsManager::instance().appPrefs.guidesPrefs.marginColor;
	QColor colBleed(Qt::gray);
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
		QRect rBleed;

		// Left Page
		if (i == 0 && count > 1)
		{
			rMargin.setLeft(rPage.left() + m_margins.right() * ratio);
			rMargin.setRight(rPage.right() - m_margins.left() * ratio);
			rBleed.setLeft(rPage.left() - m_bleeds.left() * ratio);
			rBleed.setRight(rPage.right());
		}
		// Right Page
		else if (i == 1 && count > 1)
		{
			rMargin.setLeft(rPage.left() + m_margins.left() * ratio);
			rMargin.setRight(rPage.right() - m_margins.right() * ratio);
			rBleed.setLeft(rPage.left());
			rBleed.setRight(rPage.right() + m_bleeds.right() * ratio);
		}
		// Single Page
		else
		{
			rMargin.setLeft(rPage.left() + m_margins.left() * ratio);
			rMargin.setRight(rPage.right() - m_margins.right() * ratio);
			rBleed.setLeft(rPage.left() - m_bleeds.left() * ratio);
			rBleed.setRight(rPage.right() + m_bleeds.right() * ratio);
		}

		rMargin.setTop(rPage.top() + m_margins.top() * ratio);
		rMargin.setBottom(rPage.bottom() - m_margins.bottom() * ratio);
		rBleed.setTop(rPage.top() - m_bleeds.top() * ratio);
		rBleed.setBottom(rPage.bottom() + m_bleeds.bottom() * ratio);

		// Draw Bleeds
		painter.setBrush( colPage );
		painter.setPen( QPen(colBleed, 1, Qt::DashLine) );
		painter.drawRect( rBleed );

		// Draw Pages
		painter.setBrush( Qt::NoBrush );
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
