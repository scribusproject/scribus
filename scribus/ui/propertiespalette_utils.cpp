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

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "commonstrings.h"
#include "sccolorengine.h"
#include "scraction.h"
#include "scribusdoc.h"
#include "selection.h"
#include "units.h"
#include "util.h"
#include "util_math.h"

//using namespace std;

LineStyleValue::LineStyleValue(const multiLine& line, ScribusDoc* doc, const QString& name) :
	m_Line(line),
	m_doc(doc),
	m_name(name)
{};

LineStyleValue::LineStyleValue(const LineStyleValue& other)
{
	m_name = other.m_name;
	m_Line = other.m_Line;
	m_doc = other.m_doc;
}

LineStyleValue& LineStyleValue::operator= (const LineStyleValue& other)
{
	m_name = other.m_name;
	m_Line = other.m_Line;
	m_doc = other.m_doc;
	return *this;
}


void LineStyleItemDelegate::redraw(const QVariant& data) const  
{
	const LineStyleValue& item(data.value<LineStyleValue>());
	pmap->fill(Qt::white);
	QPainter p;
	p.begin(pmap.data());
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
		p.drawLine(0, 18, 37, 18);
	}
	p.end();
}

QString LineStyleItemDelegate::text(const QVariant& data) const
{
	return data.toString();
}

NameWidget::NameWidget(QWidget* parent) : QLineEdit(parent)
{
	setObjectName("namewidget");
	QRegExp rx( "[\\w()]+" );
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


