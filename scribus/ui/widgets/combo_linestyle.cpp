#include "combo_linestyle.h"

#include <QStylePainter>

#include "sccolorengine.h"
#include "util.h"
#include "ui/propertiespalette_utils.h"
#include "util_gui.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

ComboLineStyle::ComboLineStyle(QWidget *parent) : QComboBox(parent)
{
	m_list = new QListWidget();
	m_list->setViewMode(QListWidget::ListMode);

	this->setModel(m_list->model());
	this->setView(m_list);
	this->setIconSize(m_size);

	languageChange();
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

void ComboLineStyle::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

void ComboLineStyle::setIconSize(QSize size)
{
	m_size = size;
	QComboBox::setIconSize(m_size);
	m_list->setIconSize(m_size);
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void ComboLineStyle::updateLineStyles()
{
	QSignalBlocker sig1(this);
	QSignalBlocker sig2(m_list);

	int index = this->currentIndex();

	m_list->clear();
	if (m_doc == nullptr)
		return;

	QHash<QString,MultiLine>::Iterator it;
	for (it = m_doc->docLineStyles.begin(); it != m_doc->docLineStyles.end(); ++it)
	{
		LineStyleItem *lsi = new LineStyleItem(m_doc, it.value(), it.key());
//		lsi->setSizeHint(QSize(lsi->sizeHint().width(), 48));
		QIcon ico;
		ico.addPixmap(renderPixmap(lsi->data(Qt::UserRole).value<LineStyleValue>()));
		lsi->setIcon(ico);
		m_list->addItem( lsi );
	}

	m_list->sortItems();
	m_list->insertItem( 0, tr("No Style"));
	m_list->item(0)->setIcon(QIcon(renderEmptyPattern(m_size)));

	index = (m_list->count() >= 0 && index < m_list->count()) ? index : 0;
	this->setCurrentIndex(index);
}

void ComboLineStyle::languageChange()
{
	if (m_list->count() > 0)
		m_list->item(0)->setText( tr("No Style") );

	this->setToolTip( tr("Line style of current object"));
}

QPixmap ComboLineStyle::renderPixmap(LineStyleValue item)
{
	QPixmap pixmap(m_size);
	pixmap.fill(Qt::white);

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
