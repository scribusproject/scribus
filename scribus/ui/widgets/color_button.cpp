#include "color_button.h"

#include <QPainter>
#include <QPainterPath>
#include "util_gui.h"
#include "sccolorengine.h"
#include "popup_menu.h"
#include "ui/colorlistbox.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */
ColorButton::ColorButton(QWidget *parent) : QToolButton(parent)
{
	setFixedSize(26,26);

	m_colorName = CommonStrings::tr_NoneColor;

	colorList = new ColorListBox(this);
	colorList->setPixmapType(ColorListBox::fancyPixmap);

	setPopupMode(QToolButton::InstantPopup);
	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	PopupMenu *m_menu = new PopupMenu(colorList);
	setMenu(m_menu);

	connect(colorList, &ColorListBox::currentTextChanged, this, &ColorButton::setCurrentColor);
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

bool ColorButton::hasDot()
{
	return m_hasDot;
}

QSize ColorButton::backgroundDotSize()
{
	int smallestSize = qMin(height(), width());

	return QSize(smallestSize, smallestSize);
}

QSize ColorButton::foregroundDotSize()
{
	int smallestSize = qMin(height()/2, width()/2);

	return QSize(smallestSize, smallestSize);
}

QString ColorButton::currentColor() const
{
	QString colorName(m_colorName);
	if (colorName == CommonStrings::tr_NoneColor)
		colorName = CommonStrings::None;

	return colorName;
}

void ColorButton::setColors(const ColorList &list, bool insertNone)
{
	ColorListModel* clm = qobject_cast<ColorListModel*>(colorList->model());
	if (!clm)
		return;

	clm->setColorList(list, insertNone);
}

void ColorButton::setCurrentColor(QString colorName)
{
	if (!m_doc)
		return;

	m_colorName = colorName;

	if (m_colorName == CommonStrings::tr_NoneColor || m_colorName == CommonStrings::None)
	{
		m_colorName = CommonStrings::tr_NoneColor;
		setBackground( renderEmptyPattern(this->size()));
	}
	else
	{
		ScColor sColor(0, 0, 0);

		if(m_doc->PageColors.contains(m_colorName))
			sColor = m_doc->PageColors.value(m_colorName);

		QColor qColor = ScColorEngine::getDisplayColor(sColor, m_doc);

		int smallSide = backgroundDotSize().width();
		setBackground( renderSplitColor(QSize(smallSide, smallSide), qColor ));
	}

	// update color list
	QSignalBlocker sig(colorList);
	colorList->setCurrentColor(m_colorName);

	emit colorChanged();
}

QBrush ColorButton::background()
{
	if (isEnabled())
		return m_background;
	else
		return QBrush(palette().color(QPalette::Window));
}

QBrush ColorButton::foreground()
{
	if (isEnabled())
		return m_foreground;
	else
		return QBrush(palette().color(QPalette::Window));
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void ColorButton::renderFill(QPainter *painter, QPointF center, qreal radius, QBrush background)
{
	renderCircularHandle(painter, center, radius, background);
}

/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */
void ColorButton::setBackground(QBrush background)
{
	m_background = background;
	update();
}

void ColorButton::setForeground(QBrush foreground)
{
	m_foreground = foreground;
	if(m_hasDot)
		update();
}

void ColorButton::setHasDot(bool enabled)
{
	m_hasDot = enabled;
	update();
}

void ColorButton::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void ColorButton::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QSize bSize = backgroundDotSize();
	QSize fSize = foregroundDotSize();
	QPainterPath mask;

	int inset = 1;

	// Draw Background Dot
	QRectF bDot(rect().center() - QPointF(fSize.width() - inset - 0.5, fSize.height() - inset - 0.5), QSizeF(bSize.width() - inset, bSize.height() - inset));
	mask.addEllipse(bDot.adjusted(inset, inset, -inset, -inset));
	painter.setClipPath(mask);
	renderCheckerPattern(&painter, mask.boundingRect());
	painter.setClipping(false);

	renderFill(&painter, bDot.center(), bSize.width() - inset, background());

	// Draw Foreground Dot
	if (m_hasDot)
	{
		mask.clear();
		QRectF fDot(rect().center().x(), rect().center().y(), fSize.width(), fSize.height());
		mask.addEllipse(fDot.adjusted(inset, inset, -inset, -inset));
		painter.setClipPath(mask);
		renderCheckerPattern(&painter, mask.boundingRect());
		painter.setClipping(false);

		renderFill(&painter, fDot.center(), fSize.width() - inset, foreground());
	}

	painter.end();
}

