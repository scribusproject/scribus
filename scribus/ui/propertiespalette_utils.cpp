/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette_utils.h"

#include <QCloseEvent>
#include <QEvent>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QListView>
#include <QMenu>
#include <QObject>
#include <QRegExp>
#include <QToolTip>
#include <QValidator>
#include <QWidget>

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "commonstrings.h"
#include "sccolorengine.h"
#include "sccombobox.h"
#include "scraction.h"
#include "scribusdoc.h"
#include "selection.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"
#include "text/nlsconfig.h"

//using namespace std;


LineFormatValue::LineFormatValue() : m_Line(), m_doc(NULL), m_name() {};

LineFormatValue::LineFormatValue( const multiLine& line, ScribusDoc* doc, const QString name ) : m_Line(line), m_doc(doc), m_name(name) {};

LineFormatValue::LineFormatValue(const LineFormatValue& other)
{
	m_name = other.m_name;
	m_Line = other.m_Line;
	m_doc = other.m_doc;
}

LineFormatValue& LineFormatValue::operator= (const LineFormatValue& other)
{
	m_name = other.m_name;
	m_Line = other.m_Line;
	m_doc = other.m_doc;
	return *this;
}


void LineFormatItemDelegate::redraw(const QVariant& data) const  
{
	const LineFormatValue& item(data.value<LineFormatValue>());
	QColor tmpf;
	pmap->fill(Qt::white);
	QPainter p;
	p.begin(pmap.get());
	for (int its = item.m_Line.size()-1; its > -1; its--)
	{
		const ScColor& col = item.m_doc->PageColors[item.m_Line[its].Color];
		tmpf = ScColorEngine::getDisplayColor(col, item.m_doc, item.m_Line[its].Shade);
		QPen pen;
		QVector<double> m_array;
		if (item.m_Line[its].Dash == 1)
			pen.setStyle(Qt::SolidLine);
		else
		{
			getDashArray(item.m_Line[its].Dash, 1, m_array);
			pen.setDashPattern(m_array);
		}
		pen.setColor(tmpf);
		pen.setWidth(qMax(static_cast<int>(item.m_Line[its].Width), 1));
		pen.setCapStyle(static_cast<Qt::PenCapStyle>(item.m_Line[its].LineEnd));
		pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(item.m_Line[its].LineJoin));
		p.setPen(pen);
		p.drawLine(0, 18, 37, 18);
	}
	p.end();
}

QString LineFormatItemDelegate::text(const QVariant& data) const
{
	return data.toString();
}

NameWidget::NameWidget(QWidget* parent) : QLineEdit(parent)
{
	setObjectName("namewidget");
	QRegExp rx( "\\w+" );
	QValidator* validator = new QRegExpValidator( rx, this );
	setValidator( validator );
}

void NameWidget::focusOutEvent(QFocusEvent *e)
{
	emit Leaved();
	QLineEdit::focusOutEvent(e);
}

UserActionSniffer::UserActionSniffer(QObject* parent) : QObject (parent)
{

}

bool UserActionSniffer::eventFilter(QObject*, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
		emit actionStart();
	else if (e->type() == QEvent::MouseButtonRelease)
		emit actionEnd();
	else if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *k = dynamic_cast<QKeyEvent*>(e);
		if (k && !k->isAutoRepeat() && (k->key() == Qt::Key_Up || k->key() == Qt::Key_Down))
			emit actionStart();
	}
	else if (e->type() == QEvent::KeyRelease)
	{
		QKeyEvent *k = dynamic_cast<QKeyEvent*>(e);
		if (k && !k->isAutoRepeat() && (k->key() == Qt::Key_Up || k->key() == Qt::Key_Down))
			emit actionEnd();
	}
	return false;
}


