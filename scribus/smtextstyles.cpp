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
#include "scribus.h"
#include "prefsmanager.h"
#include "colorcombo.h"
#include "story.h"
#include "menumanager.h"
#include "fmitem.h"
#include "mpalette.h"
#include "smwidgets.h"

#include <qtabwidget.h>

SMParagraphStyle::SMParagraphStyle() : StyleItem(), pwidget_(0), doc_(0), selectionIsDirty_(false)
{
	pwidget_ = new SMPStyleWidget();
	Q_CHECK_PTR(pwidget_);
}

QTabWidget* SMParagraphStyle::widget()
{
	return pwidget_->tabWidget;
}

QString SMParagraphStyle::typeNamePlural()
{
	return tr("Paragraph Styles");
}

QString SMParagraphStyle::typeNameSingular()
{
	return tr("Paragrah Style");
}

void SMParagraphStyle::currentDoc(ScribusDoc *doc)
{
	doc_ = doc;
	if (doc_)
	{
		if (pwidget_)
		{
			pwidget_->cpage->fillLangCombo(doc_->scMW()->LangTransl, doc_->Language);
			pwidget_->cpage->fillColorCombo(doc_->PageColors);
			pwidget_->cpage->fontFace_->RebuildList(doc_);
		}
	}
	else
	{
		removeConnections();
		selection_.clear();
		tmpStyles_.clear();
	}
}

QValueList<StyleName> SMParagraphStyle::styles(bool reloadFromDoc)
{
	deleted_.clear();
	QValueList<StyleName> tmpList;

	if (!doc_)
		return tmpList; // no doc available

	if (reloadFromDoc)
		reloadTmpStyles();

	for (uint i = 5 /* err */; i < tmpStyles_.count(); ++i)
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

void SMParagraphStyle::reload()
{
	reloadTmpStyles();
}

void SMParagraphStyle::selected(const QStringList &styleNames)
{
	if (!doc_)
		return;

	selection_.clear();
	selectionIsDirty_ = false;
	removeConnections(); // we don't want to record changes during style setup
	if (styleNames.count() == 1)
	{
		QValueList<CharStyle> cstyles = getCharStyles();
		for (uint i = 0; i < tmpStyles_.count(); ++i)
		{
			if (tmpStyles_[i].displayName() == styleNames[0])
			{
				pwidget_->show(tmpStyles_[i], tmpStyles_, cstyles, doc_->unitIndex());
				selection_.append(&tmpStyles_[i]);
			}
		}
	}
	else // more than one item selected do the magic tricks here
	{
		
	}
	setupConnections();
}

QValueList<CharStyle> SMParagraphStyle::getCharStyles()
{
	QValueList<CharStyle> charStyles;
	if (!doc_)
		return charStyles; // no doc available

	StyleSet<CharStyle> &tmp = doc_->docCharStyles;
	for (uint i = 0; i < tmp.count(); ++i)
		charStyles.append(tmp[i]);
	return charStyles;
}

QString SMParagraphStyle::fromSelection() const
{
	QString lsName = QString::null;
	if (!doc_)
		return lsName; // no doc available

	for (uint i = 0; i < doc_->m_Selection->count(); ++i)
	{
		// wth is going on here
		PageItem *item = doc_->m_Selection->itemAt(i);
		ParagraphStyle pstyle(item->currentStyle());
		int index = pstyle.parent() ? 
			findParagraphStyle(doc_, *dynamic_cast<const ParagraphStyle*>(pstyle.parent())) : 0;

		QString tmpName = doc_->docParagraphStyles[index].name();

		if (lsName == QString::null && !tmpName.isEmpty() && tmpName != "")
		{
			lsName = tmpName;
		}
		else if (lsName != QString::null && !tmpName.isEmpty() && tmpName != "" && lsName != tmpName)
		{
			lsName = QString::null;
			break;
		}
	}
	return lsName;
}

void SMParagraphStyle::toSelection(const QString &styleName) const
{
	if (!doc_ || doc_->m_Selection->isEmpty())
		return; // nowhere to apply or no doc

	int index = findParagraphStyle(doc_, styleName);
	Q_ASSERT(index > -1);
	if (index < 0)
		return;

	doc_->currentStyle = doc_->docParagraphStyles[index];
	doc_->itemSelection_SetParagraphStyle(index);

	PageItem *currItem = doc_->m_Selection->itemAt(0);

	doc_->scMW()->setTBvals(currItem);
	doc_->scMW()->slotDocCh();
}

QString SMParagraphStyle::newStyle()
{
	if (!doc_)
		return QString::null;

	QString s = getUniqueName(tr("New Style"));
	// i suppose this is the default style to get the default attributes in
	tmpStyles_.append(ParagraphStyle(doc_->docParagraphStyles[0]));
	tmpStyles_.last().setName(s);
	return s;
}

QString SMParagraphStyle::newStyle(const QString &fromStyle)
{
	QString s = QString::null;

	for (uint i = 0; i < tmpStyles_.count(); ++i)
	{
		if (tmpStyles_[i].name() == fromStyle)
		{
			s = getUniqueName(tr("Clone of %1").arg(fromStyle));
			tmpStyles_.append(ParagraphStyle(tmpStyles_[i]));
			tmpStyles_.last().setName(s);
			break;
		}
	}

	return s;
}

// helper function to find a unique name to a new style or a clone
QString SMParagraphStyle::getUniqueName(const QString &name)
{
	int id = 0;
	bool done = false;
	QString s = name;

	while (!done)
	{
start:
		++id;
		for (uint i = 0; i < tmpStyles_.count(); ++i)
		{
			if (tmpStyles_[i].name() == s)
			{
				s = tr("%1 (%2)", "This for unique name when creating "
						"a new character style. %1 will be the name "
								"of the style and %2 will be a number forming "
								"a style name like: New Style (2)").arg(name).arg(id);
				goto start;
			}
		}
		done = true;
	}

	return s;
}

// copied over from ScMW::saveStyles() and removed all the commented bits
void SMParagraphStyle::apply()
{
	if (!doc_)
		return;

	QValueList<uint> ers;
	QString nn;
	bool ff;
	uint nr;
	ers.clear();
	ers.append(0);
	ers.append(1);
	ers.append(2);
	ers.append(3);
	ers.append(4);

	QMap<QString, QString> replacement;
	for (uint i = 0; i < deleted_.count(); ++i)
		replacement[deleted_[i].first] = deleted_[i].second;
	deleted_.clear();

	for (uint a=5; a<doc_->docParagraphStyles.count(); ++a)
	{
		ff = false;
		nn = doc_->docParagraphStyles[a].name();
		for (uint b=0; b<tmpStyles_.count(); ++b)
		{
			if (nn == tmpStyles_[b].name())
			{
				nr = b;
				ff = true;
				break;
			}
		}
		if (ff)
			ers.append(nr);
		else
		{
			for (uint b=0; b<tmpStyles_.count(); ++b)
			{
				if (doc_->docParagraphStyles[a].equiv(tmpStyles_[b]))
				{
					nr = b;
					ff = true;
					break;
				}
			}
			if (ff)
				ers.append(nr);
			else
			{
				if (replacement.count() != 0)
				{
					QString ne = replacement[nn];
					if (ne == QString::null)
						ers.append(0);
					else
					{
						for (uint b=0; b<tmpStyles_.count(); ++b)
						{
							if (ne == tmpStyles_[b].name())
							{
								nr = b;
								ff = true;
								break;
							}
						}
						if (ff)
							ers.append(nr);
						else
							ers.append(0);
					}
				}
				else
					ers.append(0);
			}
		}
	}

	if (doc_->scMW()->CurrStED != NULL)
	{
		if (doc_->scMW()->CurrStED->Editor->StyledText.count() != 0)
		{
			for (uint pa = 0; pa < doc_->scMW()->CurrStED->Editor->StyledText.count(); ++pa)
			{
				SEditor::ChList *chars;
				chars = doc_->scMW()->CurrStED->Editor->StyledText.at(pa);
				(*doc_->scMW()->CurrStED->Editor->ParagStyles.at(pa)) = ers[doc_->scMW()->CurrStED->Editor->ParagStyles[pa]];
			}
			doc_->scMW()->CurrStED->Editor->currentParaStyle = ers[doc_->scMW()->CurrStED->Editor->currentParaStyle];
		}
	}
	doc_->docParagraphStyles.redefine(tmpStyles_);
	if (doc_->scMW()->CurrStED != NULL)
	{
		if (doc_->scMW()->CurrStED->Editor->StyledText.count() != 0)
			doc_->scMW()->CurrStED->Editor->updateAll();
	}

	// TODO add some handy way to add used fonts to the used fonts menu
// 	PrefsManager *prefsManager = PrefsManager::instance();
// 	for (uint a=0; a<doc_->docParagraphStyles.count(); ++a)
// 	{
// 		if (!doc_->docParagraphStyles[a].charStyle().font().isNone())
// 		{
// 			QString nf = doc_->docParagraphStyles[a].charStyle().font().scName();
// 			if (!doc_->UsedFonts.contains(nf))
// 			{
// 				if (doc_->AddFont(nf)) //, prefsManager->appPrefs.AvailFonts[nf]->Font))
// 				{
// // 					int ff = FontMenu->insertItem(new FmItem(nf, prefsManager->appPrefs.AvailFonts[nf]));
// // 					FontID.insert(ff, prefsManager->appPrefs.AvailFonts[nf].scName());
// 				}
// 				else
// 					doc_->docParagraphStyles[a].charStyle().setFont((prefsManager->appPrefs.AvailFonts[doc_->toolSettings.defFont]));
// 			}
// 		}
// 	}

	doc_->scMW()->propertiesPalette->Spal->updateFormatList();
	doc_->scMW()->propertiesPalette->updateColorList();
	doc_->scMW()->propertiesPalette->updateCList();
// 	FIXME private access in ScMW
// 	doc_->scMW()->updateColorMenu();
	doc_->scMW()->view->DrawNew();
	doc_->changed();
}

void SMParagraphStyle::editMode(bool isOn)
{
	if (isOn)
		reloadTmpStyles();
}

void SMParagraphStyle::deleteStyles(const QValueList<RemoveItem> &removeList)
{
	for (uint i = 0; i < removeList.count(); ++i)
	{
		for (uint k = 0; k < selection_.count(); ++k)
		{
			if (selection_[k]->name() == removeList[i].first)
			{
				selection_.erase(selection_.at(k));
				break;
			}
		}
		for (uint j = 0; j < tmpStyles_.count(); ++j)
		{
			if (tmpStyles_[j].name() == removeList[i].first)
			{
				tmpStyles_.erase(tmpStyles_.at(j));
				break;
			}
		}
		deleted_.append(removeList[i]);
	}
}

void SMParagraphStyle::nameChanged(const QString &newName)
{
	if (selection_.count() != 1)
		return;

	QString oldName = selection_[0]->name();
	tmpStyles_.append(ParagraphStyle(*(selection_[0])));
	tmpStyles_.last().setName(newName);
	selection_.clear();
	selection_.append(&tmpStyles_.last());
	for (uint j = 0; j < tmpStyles_.count(); ++j)
	{
		if (tmpStyles_[j].name() == oldName)
		{
			tmpStyles_.erase(tmpStyles_.at(j));
			break;
		}
	}

	QValueList<RemoveItem>::iterator it;
	for (it = deleted_.begin(); it != deleted_.end(); ++it)
	{
		if ((*it).second == oldName)
		{
			oldName = (*it).first;
			deleted_.remove(it);
			break;
		}
	}

	deleted_.append(RemoveItem(oldName, newName));

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::languageChange()
{
	if (pwidget_)
	{
		pwidget_->languageChange();
		pwidget_->cpage->languageChange();
	}
}

void SMParagraphStyle::reloadTmpStyles()
{
	if (!doc_)
		return;

	selection_.clear();
	tmpStyles_.clear();
	StyleSet<ParagraphStyle> &tmp = doc_->docParagraphStyles;
	for (uint i = 0; i < tmp.count(); ++i)
		tmpStyles_.append(tmp[i]);
}

void SMParagraphStyle::setupConnections()
{
	if (!pwidget_)
		return;

	// paragraph attributes
	connect(pwidget_->lineSpacingMode_, SIGNAL(activated(int)), this, SLOT(slotLineSpacingMode(int)));
	connect(pwidget_->lineSpacing_, SIGNAL(valueChanged(int)), this, SLOT(slotLineSpacing()));
	connect(pwidget_->spaceAbove_, SIGNAL(valueChanged(int)), this, SLOT(slotSpaceAbove()));
	connect(pwidget_->spaceBelow_, SIGNAL(valueChanged(int)), this, SLOT(slotSpaceBelow()));
	connect(pwidget_->alignement_->TextL, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(pwidget_->alignement_->TextR, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(pwidget_->alignement_->TextC, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(pwidget_->alignement_->TextB, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(pwidget_->alignement_->TextF, SIGNAL(clicked()), this, SLOT(slotAlignment()));

	connect(pwidget_->dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap(bool)));
	connect(pwidget_->dropCapLines_, SIGNAL(valueChanged(int)), this, SLOT(slotDropCapLines(int)));
	connect(pwidget_->dropCapOffset_, SIGNAL(valueChanged(int)), this, SLOT(slotDropCapOffset()));

	connect(pwidget_->tabList_, SIGNAL(tabsChanged()), this, SLOT(slotTabRuler()));
	connect(pwidget_->tabList_, SIGNAL(leftIndentChanged(double)), this, SLOT(slotLeftIndent(double)));
	connect(pwidget_->tabList_, SIGNAL(rightIndentChanged(double)), this, SLOT(slotRightIndent(double)));
	connect(pwidget_->tabList_, SIGNAL(firstLineChanged(double)), this, SLOT(slotFirstLine(double)));

	connect(pwidget_->parentCombo, SIGNAL(activated(const QString&)),
			this, SLOT(slotParentChanged(const QString&)));

	// character attributes
	connect(pwidget_->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(pwidget_->cpage->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	connect(pwidget_->cpage->fillColor_, SIGNAL(highlighted(int)), this, SLOT(slotFillColor()));
	connect(pwidget_->cpage->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	connect(pwidget_->cpage->strokeColor_, SIGNAL(highlighted(int)), this, SLOT(slotStrokeColor()));
	connect(pwidget_->cpage->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	connect(pwidget_->cpage->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	connect(pwidget_->cpage->fontSize_, SIGNAL(valueChanged(int)), this, SLOT(slotFontSize()));
	connect(pwidget_->cpage->fontHScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleH()));
	connect(pwidget_->cpage->fontVScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleV()));
	connect(pwidget_->cpage->tracking_, SIGNAL(valueChanged(int)), this, SLOT(slotTracking()));
	connect(pwidget_->cpage->baselineOffset_, SIGNAL(valueChanged(int)), this, SLOT(slotBaselineOffset()));
	connect(pwidget_->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(pwidget_->cpage->parentCombo, SIGNAL(activated(const QString&)),
			this, SLOT(slotCharParentChanged(const QString&)));
}

void SMParagraphStyle::removeConnections()
{
	if (!pwidget_)
		return;

	disconnect(pwidget_->lineSpacingMode_, SIGNAL(activated(int)), this, SLOT(slotLineSpacingMode(int)));
	disconnect(pwidget_->lineSpacing_, SIGNAL(valueChanged(int)), this, SLOT(slotLineSpacing()));
	disconnect(pwidget_->spaceAbove_, SIGNAL(valueChanged(int)), this, SLOT(slotSpaceAbove()));
	disconnect(pwidget_->spaceBelow_, SIGNAL(valueChanged(int)), this, SLOT(slotSpaceBelow()));
	disconnect(pwidget_->alignement_->TextL, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(pwidget_->alignement_->TextR, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(pwidget_->alignement_->TextC, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(pwidget_->alignement_->TextB, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(pwidget_->alignement_->TextF, SIGNAL(clicked()), this, SLOT(slotAlignment()));

	disconnect(pwidget_->dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap(bool)));
	disconnect(pwidget_->dropCapLines_, SIGNAL(valueChanged(int)), this, SLOT(slotDropCapLines(int)));
	disconnect(pwidget_->dropCapOffset_, SIGNAL(valueChanged(int)), this, SLOT(slotDropCapOffset()));

	disconnect(pwidget_->parentCombo, SIGNAL(activated(const QString&)),
			this, SLOT(slotParentChanged(const QString&)));

	disconnect(pwidget_->tabList_, SIGNAL(tabsChanged()), this, SLOT(slotTabRuler()));
	disconnect(pwidget_->tabList_, SIGNAL(leftIndentChanged(double)), this, SLOT(slotLeftIndent(double)));
	disconnect(pwidget_->tabList_, SIGNAL(rightIndentChanged(double)), this, SLOT(slotRightIndent(double)));
	disconnect(pwidget_->tabList_, SIGNAL(firstLineChanged(double)), this, SLOT(slotFirstLine(double)));

	disconnect(pwidget_->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(pwidget_->cpage->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	disconnect(pwidget_->cpage->fillColor_, SIGNAL(highlighted(int)), this, SLOT(slotFillColor()));
	disconnect(pwidget_->cpage->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	disconnect(pwidget_->cpage->strokeColor_, SIGNAL(highlighted(int)), this, SLOT(slotStrokeColor()));
	disconnect(pwidget_->cpage->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	disconnect(pwidget_->cpage->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	disconnect(pwidget_->cpage->fontSize_, SIGNAL(valueChanged(int)), this, SLOT(slotFontSize()));
	disconnect(pwidget_->cpage->fontHScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleH()));
	disconnect(pwidget_->cpage->fontVScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleV()));
	disconnect(pwidget_->cpage->tracking_, SIGNAL(valueChanged(int)), this, SLOT(slotTracking()));
	disconnect(pwidget_->cpage->baselineOffset_, SIGNAL(valueChanged(int)), this, SLOT(slotBaselineOffset()));
	disconnect(pwidget_->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(pwidget_->cpage->parentCombo, SIGNAL(activated(const QString&)),
			this, SLOT(slotCharParentChanged(const QString&)));
}

void SMParagraphStyle::slotLineSpacingMode(int mode)
{
	ParagraphStyle::LineSpacingMode lsm;
	if (pwidget_->lineSpacingMode_->useParentValue())
		lsm = static_cast<ParagraphStyle::LineSpacingMode>(Style::NOVALUE);
	else
		lsm = static_cast<ParagraphStyle::LineSpacingMode>(mode);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setLineSpacingMode(lsm);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotLineSpacing()
{
	double a, b, value;
	int c;

	if (pwidget_->lineSpacing_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->lineSpacing_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setLineSpacing(value);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotSpaceAbove()
{
	double a, b, value;
	int c;

	if (pwidget_->spaceAbove_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->spaceAbove_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setGapBefore(value);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotSpaceBelow()
{
	double a, b, value;
	int c;

	if (pwidget_->spaceBelow_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->spaceBelow_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setGapAfter(value);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotAlignment()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setAlignment(pwidget_->alignement_->getStyle());

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotDropCap(bool isOn)
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setHasDropCap(isOn);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotDropCapLines(int lines)
{
	int l = pwidget_->dropCapLines_->useParentValue() ? Style::NOVALUE : lines;

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setDropCapLines(l);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotDropCapOffset()
{
	double a, b, value;
	int c;

	if (pwidget_->dropCapOffset_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->dropCapOffset_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setDropCapOffset(value);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotTabRuler()
{
	QValueList<ParagraphStyle::TabRecord> newTabs = pwidget_->tabList_->getTabVals();
	for (uint i = 0; i < selection_.count(); ++i)
	{
		QValueList<ParagraphStyle::TabRecord> &tabs = selection_[i]->tabValues();
		tabs.clear();
		for (uint i = 0; i < newTabs.count(); ++i)
			tabs.append(newTabs[i]);
	}

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotLeftIndent(double value)
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setLeftMargin(value);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotRightIndent(double value)
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setRightMargin(value);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFirstLine(double value)
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setFirstIndent(value);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFontSize()
{
	double a, b, value;
	int c;

	if (pwidget_->cpage->fontSize_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->cpage->fontSize_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setFontSize(qRound(value));

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotEffects(int e)
{
	double a, b, sxo, syo, olw, ulp, ulw, slp, slw;
	int c;

	pwidget_->cpage->effects_->ShadowVal->Xoffset->getValues(&a, &b, &c, &sxo);
	pwidget_->cpage->effects_->ShadowVal->Yoffset->getValues(&a, &b, &c, &syo);

	pwidget_->cpage->effects_->OutlineVal->LWidth->getValues(&a, &b, &c, &olw);

	pwidget_->cpage->effects_->UnderlineVal->LPos->getValues(&a, &b, &c, &ulp);
	pwidget_->cpage->effects_->UnderlineVal->LWidth->getValues(&a, &b, &c, &ulw);

	pwidget_->cpage->effects_->StrikeVal->LPos->getValues(&a, &b, &c, &slp);
	pwidget_->cpage->effects_->StrikeVal->LWidth->getValues(&a, &b, &c, &slw);

	for (uint i = 0; i < selection_.count(); ++i)
	{
		selection_[i]->charStyle().setEffects(static_cast<StyleFlag>(e));
		selection_[i]->charStyle().setShadowXOffset(qRound(sxo));
		selection_[i]->charStyle().setShadowYOffset(qRound(syo));
		selection_[i]->charStyle().setOutlineWidth(qRound(olw));
		selection_[i]->charStyle().setUnderlineOffset(qRound(ulp));
		selection_[i]->charStyle().setUnderlineWidth(qRound(ulw));
		selection_[i]->charStyle().setStrikethruOffset(qRound(slp));
		selection_[i]->charStyle().setStrikethruWidth(qRound(slw));
	}

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFillColor()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setFillColor(pwidget_->cpage->fillColor_->currentText());
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFillShade()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setFillShade(pwidget_->cpage->fillShade_->getValue());
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotStrokeColor()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setStrokeColor(pwidget_->cpage->strokeColor_->currentText());
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotStrokeShade()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setStrokeShade(pwidget_->cpage->strokeShade_->getValue());
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotLanguage()
{
	QMap<QString,QString>::Iterator it;
	QString language = doc_->Language;

	if (pwidget_->cpage->language_->useParentValue())
		language = CharStyle::NOLANG;
	else
	{
		for (it = doc_->scMW()->LangTransl.begin(); it != doc_->scMW()->LangTransl.end(); ++it)
		{
			if (it.data() == pwidget_->cpage->language_->currentText())
			{
				language = it.key();
				break;
			}
		}
	}

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setLanguage(language);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotScaleH()
{
	double a, b, value;
	int c;

	if (pwidget_->cpage->fontHScale_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->cpage->fontHScale_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setScaleH(qRound(value));
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotScaleV()
{
	double a, b, value;
	int c;

	if (pwidget_->cpage->fontVScale_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->cpage->fontVScale_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setScaleV(qRound(value));
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotTracking()
{
	double a, b, value;
	int c;

	if (pwidget_->cpage->tracking_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->cpage->tracking_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setTracking(qRound(value));
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotBaselineOffset()
{
	double a, b, value;
	int c;

	if (pwidget_->cpage->baselineOffset_->useParentValue())
		value = Style::NOVALUE;
	else
		pwidget_->cpage->baselineOffset_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setBaselineOffset(qRound(value));
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFont(QString s)
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->charStyle().setFont(PrefsManager::instance()->appPrefs.AvailFonts[s]);
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotParentChanged(const QString &parent)
{
	QStringList sel;
	ParagraphStyle *p = 0;

	if (parent != "" && !parent.isEmpty() && parent != QString::null)
		p = &(doc_->docParagraphStyles[doc_->docParagraphStyles.find(parent)]);

	for (uint i = 0; i < selection_.count(); ++i)
	{
		selection_[i]->eraseStyle(*selection_[i]); // reset everything to NOVALUE
		selection_[i]->setParent(p);
		selection_[i]->charStyle().setParent(&(p->charStyle()));
		sel << selection_[i]->name();
	}

	selected(sel);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotCharParentChanged(const QString &parent)
{ // TODO Multiple selection
	QStringList sel;
	const CharStyle *c = 0;
	bool setParent = true;

	int cItem = pwidget_->cpage->parentCombo->currentItem();

	if (selection_[0]->hasParent() && cItem == 1) // from parent
	{
		const Style *s = selection_[0]->parent();
		const ParagraphStyle *tmp = dynamic_cast<const ParagraphStyle*>(s);
		c = &(tmp->charStyle());
	}
	else if (parent != "" && !parent.isEmpty() && parent != QString::null)
		c = &(doc_->docCharStyles[doc_->docCharStyles.find(parent)]);
	else
		setParent = false;

	for (uint i = 0; i < selection_.count(); ++i)
	{
		if (setParent)
		{
			// reset everything to NOVALUE
			selection_[i]->charStyle().eraseCharStyle(selection_[i]->charStyle()); 
			selection_[i]->charStyle().setParent(c);
		}
		else
			selection_[i]->charStyle() = doc_->docParagraphStyles[0].charStyle();
		
		sel << selection_[i]->name();
	}

	selected(sel);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

SMParagraphStyle::~SMParagraphStyle()
{

}

/******************************************************************************/
/******************************************************************************/

SMCharacterStyle::SMCharacterStyle() : StyleItem(), widget_(0), page_(0), doc_(0), selectionIsDirty_(false)
{
	widget_ = new QTabWidget();
	Q_CHECK_PTR(widget_);
	page_ = new SMCStylePage();
	Q_CHECK_PTR(page_);
	widget_->addTab(page_, tr("Properties"));
}

QTabWidget* SMCharacterStyle::widget()
{
	return widget_;
}

QString SMCharacterStyle::typeNamePlural()
{
	return tr("Character Styles");
}

QString SMCharacterStyle::typeNameSingular()
{
	return tr("Character Style");
}

void SMCharacterStyle::currentDoc(ScribusDoc *doc)
{
	doc_ = doc;
	if (doc_)
	{
		if (page_)
		{
			page_->fillLangCombo(doc_->scMW()->LangTransl, doc_->Language);
			page_->fillColorCombo(doc_->PageColors);
			page_->fontFace_->RebuildList(doc_);
		}
	}
	else
	{
		removeConnections();
		selection_.clear();
		tmpStyles_.clear();
	}

}

QValueList<StyleName> SMCharacterStyle::styles(bool reloadFromDoc)
{
	QValueList<StyleName> tmpList;

	if (!doc_)
		return tmpList; // no doc available

	if (reloadFromDoc)
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

void SMCharacterStyle::reload()
{
	reloadTmpStyles();
}

void SMCharacterStyle::selected(const QStringList &styleNames)
{
	selection_.clear();
	selectionIsDirty_ = false;
	removeConnections();
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
	setupConnections();
}

QString SMCharacterStyle::fromSelection() const
{
	return QString::null;
}

void SMCharacterStyle::toSelection(const QString &styleName) const
{
	
}

QString SMCharacterStyle::newStyle()
{
	Q_ASSERT(doc_ && doc_->docParagraphStyles.count() > 0);

	QString s = getUniqueName(tr("New Style"));
	// fetching default values now from here
	tmpStyles_.append(CharStyle(doc_->docParagraphStyles[0].charStyle()));
	tmpStyles_.last().setName(s);
	return s;
}

QString SMCharacterStyle::newStyle(const QString &fromStyle)
{
	Q_ASSERT(doc_ && doc_->docParagraphStyles.count() > 0);

	QString s = QString::null;

	for (uint i = 0; i < tmpStyles_.count(); ++i)
	{
		if (tmpStyles_[i].name() == fromStyle)
		{
			s = getUniqueName(tr("Clone of %1").arg(fromStyle));
			tmpStyles_.append(CharStyle(tmpStyles_[i]));
			tmpStyles_.last().setName(s);
			break;
		}
	}

	return s;
}

QString SMCharacterStyle::getUniqueName(const QString &name)
{
	int id = 0;
	bool done = false;
	QString s = name;

	while (!done)
	{
start:
		++id;
		for (uint i = 0; i < tmpStyles_.count(); ++i)
		{
			if (tmpStyles_[i].name() == s)
			{
				s = tr("%1 (%2)", "This for unique name when creating "
						"a new character style. %1 will be the name "
								"of the style and %2 will be a number forming "
								"a style name like: New Style (2)").arg(name).arg(id);
				goto start;
			}
		}
		done = true;
	}

	return s;
}

void SMCharacterStyle::apply()
{
	if (!doc_)
		return;

	for (uint i = 0; i < deleted_.count(); ++i)
	{ // TODO apply also user defined replacement!!!
		int index = doc_->docCharStyles.find(deleted_[i].first);
		if (index > -1)
			doc_->docCharStyles.remove(static_cast<uint>(index));
	}
	deleted_.clear();
	doc_->docCharStyles.redefine(tmpStyles_);
}

void SMCharacterStyle::editMode(bool isOn)
{
	if (isOn)
		reloadTmpStyles();
}

void SMCharacterStyle::deleteStyles(const QValueList<RemoveItem> &removeList)
{
	for (uint i = 0; i < removeList.count(); ++i)
	{
		for (uint k = 0; k < selection_.count(); ++k)
		{
			if (selection_[k]->name() == removeList[i].first)
			{
				selection_.erase(selection_.at(k));
				break;
			}
		}
		for (uint j = 0; j < tmpStyles_.count(); ++j)
		{
			if (tmpStyles_[j].name() == removeList[i].first)
			{
				tmpStyles_.erase(tmpStyles_.at(j));
				break;
			}
		}
		deleted_ << removeList[i];
	}
}

void SMCharacterStyle::nameChanged(const QString &newName)
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setName(newName);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::languageChange()
{
	if (widget_ && page_)
	{
		widget_->setTabLabel(page_, tr("Properties"));
		page_->languageChange();
	}
}

void SMCharacterStyle::reloadTmpStyles()
{
	if (!doc_)
		return;

	selection_.clear();
	tmpStyles_.clear();
	StyleSet<CharStyle> &tmp = doc_->docCharStyles;
	for (uint i = 0; i < tmp.count(); ++i)
		tmpStyles_.append(tmp[i]);
}

void SMCharacterStyle::setupConnections()
{
	if (!page_)
		return;

	connect(page_->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(page_->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	connect(page_->fillColor_, SIGNAL(highlighted(int)), this, SLOT(slotFillColor()));
	connect(page_->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	connect(page_->strokeColor_, SIGNAL(highlighted(int)), this, SLOT(slotStrokeColor()));
	connect(page_->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	connect(page_->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	connect(page_->fontSize_, SIGNAL(valueChanged(int)), this, SLOT(slotFontSize()));
	connect(page_->fontHScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleH()));
	connect(page_->fontVScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleV()));
	connect(page_->tracking_, SIGNAL(valueChanged(int)), this, SLOT(slotTracking()));
	connect(page_->baselineOffset_, SIGNAL(valueChanged(int)), this, SLOT(slotBaselineOffset()));
	connect(page_->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(page_->parentCombo, SIGNAL(activated(const QString&)),
	        this, SLOT(slotParentChanged(const QString&)));
}

void SMCharacterStyle::removeConnections()
{
	if (!page_)
		return;

	disconnect(page_->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(page_->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	disconnect(page_->fillColor_, SIGNAL(highlighted(int)), this, SLOT(slotFillColor()));
	disconnect(page_->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	disconnect(page_->strokeColor_, SIGNAL(highlighted(int)), this, SLOT(slotStrokeColor()));
	disconnect(page_->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	disconnect(page_->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	disconnect(page_->fontSize_, SIGNAL(valueChanged(int)), this, SLOT(slotFontSize()));
	disconnect(page_->fontHScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleH()));
	disconnect(page_->fontVScale_, SIGNAL(valueChanged(int)), this, SLOT(slotScaleV()));
	disconnect(page_->tracking_, SIGNAL(valueChanged(int)), this, SLOT(slotTracking()));
	disconnect(page_->baselineOffset_, SIGNAL(valueChanged(int)), this, SLOT(slotBaselineOffset()));
	disconnect(page_->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(page_->parentCombo, SIGNAL(activated(const QString&)),
			this, SLOT(slotParentChanged(const QString&)));
}

void SMCharacterStyle::slotFontSize()
{
	double a, b, value;
	int c;

	if (page_->fontSize_->useParentValue())
		value = Style::NOVALUE;
	else
		page_->fontSize_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setFontSize(qRound(value));

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotEffects(int e)
{
	double a, b, sxo, syo, olw, ulp, ulw, slp, slw;
	int c;

	page_->effects_->ShadowVal->Xoffset->getValues(&a, &b, &c, &sxo);
	page_->effects_->ShadowVal->Yoffset->getValues(&a, &b, &c, &syo);

	page_->effects_->OutlineVal->LWidth->getValues(&a, &b, &c, &olw);

	page_->effects_->UnderlineVal->LPos->getValues(&a, &b, &c, &ulp);
	page_->effects_->UnderlineVal->LWidth->getValues(&a, &b, &c, &ulw);

	page_->effects_->StrikeVal->LPos->getValues(&a, &b, &c, &slp);
	page_->effects_->StrikeVal->LWidth->getValues(&a, &b, &c, &slw);

	for (uint i = 0; i < selection_.count(); ++i)
	{
		selection_[i]->setEffects(static_cast<StyleFlag>(e));
		selection_[i]->setShadowXOffset(qRound(sxo));
		selection_[i]->setShadowYOffset(qRound(syo));
		selection_[i]->setOutlineWidth(qRound(olw));
		selection_[i]->setUnderlineOffset(qRound(ulp));
		selection_[i]->setUnderlineWidth(qRound(ulw));
		selection_[i]->setStrikethruOffset(qRound(slp));
		selection_[i]->setStrikethruWidth(qRound(slw));
	}

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotFillColor()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setFillColor(page_->fillColor_->currentText());

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotFillShade()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setFillShade(page_->fillShade_->getValue());

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotStrokeColor()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setStrokeColor(page_->strokeColor_->currentText());

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotStrokeShade()
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setStrokeShade(page_->strokeShade_->getValue());

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotLanguage()
{
	QMap<QString,QString>::Iterator it;
	QString language = doc_->Language;

	if (page_->language_->useParentValue())
		language = CharStyle::NOLANG;
	else
	{
		for (it = doc_->scMW()->LangTransl.begin(); it != doc_->scMW()->LangTransl.end(); ++it)
		{
			if (it.data() == page_->language_->currentText())
			{
				language = it.key();
				break;
			}
		}
	}

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setLanguage(language);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotScaleH()
{
	double a, b, value;
	int c;

	if (page_->fontHScale_->useParentValue())
		value = Style::NOVALUE;
	else
		page_->fontHScale_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setScaleH(qRound(value));

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotScaleV()
{
	double a, b, value;
	int c;

	if (page_->fontVScale_->useParentValue())
		value = Style::NOVALUE;
	else
		page_->fontVScale_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setScaleV(qRound(value));

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotTracking()
{
	double a, b, value;
	int c;

	if (page_->tracking_->useParentValue())
		value = Style::NOVALUE;
	else
		page_->tracking_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setTracking(qRound(value));
	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotBaselineOffset()
{
	double a, b, value;
	int c;

	if (page_->baselineOffset_->useParentValue())
		value = Style::NOVALUE;
	else
		page_->baselineOffset_->getValues(&a, &b, &c, &value);

	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setBaselineOffset(qRound(value));

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotFont(QString s)
{
	for (uint i = 0; i < selection_.count(); ++i)
		selection_[i]->setFont(PrefsManager::instance()->appPrefs.AvailFonts[s]);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotParentChanged(const QString &parent)
{
	QStringList sel;
	CharStyle *c = 0;

	if (parent != "" && !parent.isEmpty() && parent != QString::null)
		c = &(doc_->docCharStyles[doc_->docCharStyles.find(parent)]);

	for (uint i = 0; i < selection_.count(); ++i)
	{
		// reset everything to NOVALUE
		selection_[i]->eraseCharStyle(*selection_[i]); 
		selection_[i]->setParent(c);
		sel << selection_[i]->name();
	}

	selected(sel);

	if (!selectionIsDirty_)
	{
		selectionIsDirty_ = true;
		emit selectionDirty();
	}
}

SMCharacterStyle::~SMCharacterStyle()
{
	delete page_;
	delete widget_;
	page_ = 0;
	widget_ = 0;
}

