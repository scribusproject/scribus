/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smtextstyles.h"
#include "smtextstyles.moc"
#include "smtextstylewidgets.h"
#include "util.h"
#include "mspinbox.h"
#include "alignselect.h"
#include "tabruler.h"
#include "fontcombo.h"
#include "styleselect.h"
#include "sccombobox.h"
#include "shadebutton.h"
#include "commonstrings.h"
#include "style.h"
#include "scribusdoc.h"
#include "selection.h"

#include <qtabwidget.h>



SMParagraphStyle::SMParagraphStyle() : StyleItem(), pwidget_(0), doc_(0)
{

}

QTabWidget* SMParagraphStyle::widget()
{
	if (!pwidget_)
	{
		pwidget_ = new SMPStyleWidget();
		Q_CHECK_PTR(pwidget_);
	}

	return pwidget_->tabWidget;
}

QString SMParagraphStyle::typeName()
{
	return tr("Paragraph Styles");
}

void SMParagraphStyle::currentDoc(ScribusDoc *doc)
{
	Q_ASSERT(doc);
	doc_ = doc;
}

QValueList<StyleName> SMParagraphStyle::styles()
{
	QValueList<StyleName> tmpList;

	if (!doc_)
		return tmpList; // no doc available

	reloadTmpStyles();

	for (uint i = 0; i < tmpStyles_.count(); ++i)
	{
		if (tmpStyles_[i].hasName())
		{
			QString styleName = tmpStyles_[i].name();
			QString parentName = QString::null;

			if (tmpStyles_[i].hasParent() && tmpStyles_[i].parent()->hasName())
				parentName = tmpStyles_[i].parent()->displayName();

			tmpList << StyleName(styleName, parentName);
		}
	}

	return tmpList;
}

void SMParagraphStyle::selected(const QStringList &styleNames)
{
	selection_.clear();
	if (styleNames.count() == 1)
	{
		QValueList<CharStyle> cstyles = getCharStyles();
		for (uint i = 0; i < tmpStyles_.count(); ++i)
		{
			if (tmpStyles_[i].displayName() == styleNames[i])
			{
				pwidget_->show(tmpStyles_[i], tmpStyles_, cstyles);
				selection_.append(&tmpStyles_[i]);
			}
		}
	}
	else // more than one item selected do the magic tricks here
	{
		
	}
}

QValueList<CharStyle> SMParagraphStyle::getCharStyles()
{
	QValueList<CharStyle> charStyles;
	StyleSet<CharStyle> &tmp = doc_->docCharStyles;
	for (uint i = 0; i < tmp.count(); ++i)
		charStyles.append(tmp[i]);
	return charStyles;
}

QString SMParagraphStyle::fromSelection() const
{
//	Q_ASSERT(doc_ && doc_->m_Selection);
//	QString lsName = QString::null;

//	for (uint i = 0; i < doc_->m_Selection->count(); ++i)
//	{
//		PageItem *item = doc_->m_Selection->itemAt(i);
//	}
	return QString::null;
}

void SMParagraphStyle::apply()
{
	Q_ASSERT(doc_);
	doc_->docParagraphStyles.redefine(tmpStyles_);
}

void SMParagraphStyle::editMode(bool isOn)
{
	if (isOn)
		reloadTmpStyles();
}

void SMParagraphStyle::deleteStyles(const QValueList<RemoveItem> &removeList)
{

}

void SMParagraphStyle::nameChanged(const QString &newName)
{

}

void SMParagraphStyle::reloadTmpStyles()
{
	selection_.clear();
	tmpStyles_.clear();
	StyleSet<ParagraphStyle> &tmp = doc_->docParagraphStyles;
	for (uint i = 0; i < tmp.count(); ++i)
		tmpStyles_.append(tmp[i]);
}

SMParagraphStyle::~SMParagraphStyle()
{

}

/******************************************************************************/
/******************************************************************************/

SMCharacterStyle::SMCharacterStyle() : StyleItem(), widget_(0), page_(0), doc_(0)
{

}

QTabWidget* SMCharacterStyle::widget()
{
	if (!widget_)
	{
		widget_ = new QTabWidget();
		Q_CHECK_PTR(widget_);
		page_ = new SMCStylePage();
		Q_CHECK_PTR(page_);
		widget_->addTab(page_, tr("Properties"));
	}

	return widget_;
}

QString SMCharacterStyle::typeName()
{
	return tr("Character Styles");
}

void SMCharacterStyle::currentDoc(ScribusDoc *doc)
{
	Q_ASSERT(doc);
	doc_ = doc;
}

QValueList<StyleName> SMCharacterStyle::styles()
{
	QValueList<StyleName> tmpList;

	if (!doc_)
		return tmpList; // no doc available

	reloadTmpStyles();

	for (uint i = 0; i < tmpStyles_.count(); ++i)
	{
		if (tmpStyles_[i].hasName())
		{
			QString styleName = tmpStyles_[i].name();
			QString parentName = QString::null;

			if (tmpStyles_[i].hasParent() && tmpStyles_[i].parent()->hasName())
				parentName = tmpStyles_[i].parent()->displayName();

			tmpList << StyleName(styleName, parentName);
		}
	}

	return tmpList;
}

void SMCharacterStyle::selected(const QStringList &styleNames)
{
	selection_.clear();
	if (styleNames.count() == 1)
	{
		for (uint i = 0; i < tmpStyles_.count(); ++i)
		{
			if (tmpStyles_[i].displayName() == styleNames[0])
			{
				page_->show(tmpStyles_[i], tmpStyles_);
				selection_.append(&tmpStyles_[i]);
				break;
			}
		}
			
	}
	else // more than one item selected do the magic tricks here
	{
		
	}
}

QString SMCharacterStyle::fromSelection() const
{
	return QString::null;
}

void SMCharacterStyle::apply()
{
	Q_ASSERT(doc_);
	doc_->docCharStyles.redefine(tmpStyles_);
}

void SMCharacterStyle::editMode(bool isOn)
{
	if (isOn)
		reloadTmpStyles();
}

void SMCharacterStyle::deleteStyles(const QValueList<RemoveItem> &removeList)
{

}

void SMCharacterStyle::nameChanged(const QString &newName)
{

}

void SMCharacterStyle::reloadTmpStyles()
{
	selection_.clear();
	tmpStyles_.clear();
	StyleSet<CharStyle> &tmp = doc_->docCharStyles;
	for (uint i = 0; i < tmp.count(); ++i)
		tmpStyles_.append(tmp[i]);
}

SMCharacterStyle::~SMCharacterStyle()
{
	delete page_;
	delete widget_;
	page_ = 0;
	widget_ = 0;
}

