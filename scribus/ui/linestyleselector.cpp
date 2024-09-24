#include "linestyleselector.h"
#include "scribus.h"
#include "sccolorengine.h"
#include "ui/delegates/sclistitemdelegate.h"
#include "util.h"
#include "ui/propertiespalette_utils.h"
#include "ui/stylemanager.h"
#include "util_gui.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

LineStyleSelector::LineStyleSelector(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	//lineStyles->setItemDelegate(new LineStyleItemDelegate());
//	tnailGrid->setIconSize(QSize(ICONSIZE, ICONSIZE));
//	tnailGrid->setGridSize(QSize(220, 80));
//	tnailGrid->setResizeMode(QListView::Adjust);
//	tnailGrid->setItemDelegate(new ScListItemDelegate(QSize(ICONSIZE,ICONSIZE), ScListItemDelegate::Right));

	lineStyles->setViewMode(QListWidget::ListMode);
	lineStyles->setIconSize(m_size);
//	lineStyles->setGridSize(QSize(76,64));
//	lineStyles->setSpacing(4);
	lineStyles->setItemDelegate(new ScListItemDelegate(QListWidget::ListMode, m_size, ScListItemDelegate::Right, ScListItemDelegate::Simple));
//	lineStyles->addItem( "No Style" );
//	lineStyles->setCurrentItem(lineStyles->item(0));

	languageChange();

	connect(lineStyles, SIGNAL(itemClicked(QListWidgetItem*)), this, SIGNAL(itemClicked(QListWidgetItem*)));
	connect(buttonNewLine, &QToolButton::clicked, this, &LineStyleSelector::newLine);
}

LineStyleSelector::~LineStyleSelector()
{
	delete lineStyles->itemDelegate();
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

void LineStyleSelector::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

QSize LineStyleSelector::iconSize()
{
	return m_size;
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void LineStyleSelector::updateLineStyles()
{
	lineStyles->blockSignals(true);
	lineStyles->clear();
	if (m_doc != nullptr)
	{
		QHash<QString,MultiLine>::Iterator it;
		for (it = m_doc->docLineStyles.begin(); it != m_doc->docLineStyles.end(); ++it)
		{
			LineStyleItem *lsi = new LineStyleItem(m_doc, it.value(), it.key());
//			lsi->setSizeHint(QSize(lsi->sizeHint().width(), 48));
			QIcon ico;
			ico.addPixmap(renderPixmap(lsi->data(Qt::UserRole).value<LineStyleValue>()));
			lsi->setIcon(ico);
			lineStyles->addItem( lsi );
		}

		lineStyles->sortItems();
		lineStyles->insertItem( 0, tr("No Style"));
		lineStyles->item(0)->setIcon(QIcon(renderEmptyPattern(m_size, devicePixelRatio())));

		if (lineStyles->currentItem())
			lineStyles->currentItem()->setSelected(false);
	}
	lineStyles->blockSignals(false);
}

QPixmap LineStyleSelector::renderPixmap(LineStyleValue item)
{
	QPixmap pixmap(m_size);
	pixmap.fill(Qt::white);

	//const LineStyleValue& item(lsi->data(Qt::UserRole).value<LineStyleValue>());

	QPainter p(&pixmap);
	for (int i = item.m_Line.size() - 1; i > -1; i--)
	{
		const SingleLine& sLine = item.m_Line.at(i);
		const ScColor& col = item.m_doc->PageColors[sLine.Color];
		QColor tmpf = ScColorEngine::getDisplayColor(col, item.m_doc, sLine.Shade);
		QPen pen;
		QVector<double> m_array;
		if (sLine.Dash == 1)
			pen.setStyle(Qt::SolidLine);
		else
		{
			getDashArray(sLine.Dash, 1, m_array);
			pen.setDashPattern(m_array);
		}
		pen.setColor(tmpf);
		pen.setWidth(qMax(static_cast<int>(sLine.Width), 1));
		pen.setCapStyle(static_cast<Qt::PenCapStyle>(sLine.LineEnd));
		pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(sLine.LineJoin));
		p.setPen(pen);
		p.drawLine(0, m_size.height() / 2, m_size.width(), m_size.height() / 2);
	}
	p.end();

	return pixmap;

}

/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */

void LineStyleSelector::languageChange()
{
	if(lineStyles->count() > 0)
		lineStyles->item(0)->setText( tr("No Style") );

	lineStyles->setToolTip( tr("Line style of current object"));
}

void LineStyleSelector::newLine()
{
	if(!m_doc)
		return;

	m_doc->scMW()->styleMgr()->setPaletteShown(true);
}

