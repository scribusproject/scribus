/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_flop.h"

#include "scribusdoc.h"
#include "units.h"
#include "util_icon.h"

PropertyWidget_Flop::PropertyWidget_Flop(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );
	
	flopRealHeight->setChecked(true);

	languageChange();
}

void PropertyWidget_Flop::setDoc(ScribusDoc *d)
{
	if(d == (ScribusDoc*) m_doc)
		return;

	m_doc  = d;

	if (m_doc.isNull())
		return;
}

void PropertyWidget_Flop::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Flop::languageChange()
{
	flopRealHeight->setText( tr("Maximum Ascent") );
	flopFontAscent->setText( tr("Font Ascent") );
	flopLineSpacing->setText( tr("Line Spacing") );
	
	flopRealHeight->setToolTip( "<qt>" + tr("Set the height of the first line of the text frame to use the tallest height of the included characters") + "</qt>" );
	flopFontAscent->setToolTip( "<qt>" + tr("Set the height of the first line of text frame to use the full ascent of the font(s) in use") + "</qt>" );
	flopLineSpacing->setToolTip( "<qt>" + tr("Set the height of the first line of the text frame to the specified line height") + "</qt>" );
}