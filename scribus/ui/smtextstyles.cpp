/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>
#include <QMessageBox>
#include <QTabWidget>

#include "alignselect.h"
#include "colorcombo.h"
#include "commonstrings.h"
#include "fontcombo.h"
#include "langmgr.h"
#include "ui/scmwmenumanager.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "propertiespalette_text.h"
#include "sccombobox.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "selection.h"
#include "shadebutton.h"
#include "smalignselect.h"
#include "smcolorcombo.h"
#include "smcstylewidget.h"
#include "smpstylewidget.h"
#include "smsccombobox.h"
#include "smshadebutton.h"
#include "smspinbox.h"
#include "smtabruler.h"
#include "smtextstyles.h"
#include "storyeditor.h"
#include "style.h"
#include "styleselect.h"
#include "tabruler.h"
#include "units.h"
#include "util.h"


SMParagraphStyle::SMParagraphStyle(StyleSet<CharStyle> *cstyles) : StyleItem(),
m_pwidget(0), m_doc(0), m_selectionIsDirty(false), m_unitRatio(1.0), m_cstyles(cstyles)
{
	Q_ASSERT(m_cstyles);
	m_pwidget = new SMPStyleWidget(m_doc, m_cstyles);
	Q_CHECK_PTR(m_pwidget);
}

QTabWidget* SMParagraphStyle::widget()
{
	return m_pwidget->tabWidget;
}

QString SMParagraphStyle::typeNamePlural()
{
	return tr("Paragraph Styles");
}

QString SMParagraphStyle::typeNameSingular()
{
	return tr("Paragraph Style");
}

void SMParagraphStyle::setCurrentDoc(ScribusDoc *doc)
{
	m_doc = doc;
	if (m_doc)
	{
		if (m_pwidget)
		{
			QStringList languageList;
			LanguageManager::instance()->fillInstalledHyphStringList(&languageList);
			m_pwidget->cpage->fillLangComboFromList(languageList);
			m_pwidget->cpage->fillColorCombo(m_doc->PageColors);
			m_pwidget->cpage->fontFace_->RebuildList(m_doc);
			if (m_unitRatio != m_doc->unitRatio())
				unitChange();
		}
	}
	else
	{
		removeConnections();
		m_selection.clear();
		m_tmpStyles.clear();
		m_deleted.clear();
	}
	if (m_pwidget)
		m_pwidget->setDoc(doc);
}

StyleSet<ParagraphStyle>* SMParagraphStyle::tmpStyles()
{
	return &m_tmpStyles;
}

QList<StyleName> SMParagraphStyle::styles(bool reloadFromDoc)
{
	QList<StyleName> tmpList;

	if (!m_doc)
		return tmpList; // no doc available

	if (reloadFromDoc)
	{
		m_deleted.clear();
		reloadTmpStyles();
	}

	for (int i = 0; i < m_tmpStyles.count(); ++i)
	{
		if (m_tmpStyles[i].hasName())
		{
			QString styleName(m_tmpStyles[i].displayName());
			QString parentName(QString::null);

			if (m_tmpStyles[i].hasParent())
			{
				const Style* parentStyle = m_tmpStyles[i].parentStyle();
				if (parentStyle)
					parentName = parentStyle->displayName();
			}

			tmpList << StyleName(styleName, parentName);
		}
	}
	qSort(tmpList.begin(), tmpList.end(), sortingQPairOfStrings);

	return tmpList;
}

void SMParagraphStyle::reload()
{
	reloadTmpStyles();
}

void SMParagraphStyle::selected(const QStringList &styleNames)
{
	if (!m_doc)
		return;

	m_selection.clear();
	m_selectionIsDirty = false;
	removeConnections(); // we don't want to record changes during style setup

	m_tmpStyles.invalidate();

	QList<ParagraphStyle> pstyles; // get saved styles
	QList<CharStyle> cstyles;
	for (int i = 0; i < m_tmpStyles.count(); ++i)
		pstyles << m_tmpStyles[i];
	for (int i = 0; i < m_cstyles->count(); ++i)
		cstyles << (*m_cstyles)[i];

	int index;
	for (int i = 0; i < styleNames.count(); ++i)
	{
		index = m_tmpStyles.find(styleNames[i]);
		//FIXME: #7133: Use .isDefaultStyle() instead here rather than relying on tr text comparison
		if (index<0 && styleNames[i]==CommonStrings::trDefaultParagraphStyle)
			index = m_tmpStyles.find(CommonStrings::DefaultParagraphStyle);
		if (index > -1)
			m_selection.append(&m_tmpStyles[index]);
	}

	m_pwidget->show(m_selection, pstyles, cstyles, m_doc->unitIndex(), PrefsManager::instance()->appPrefs.hyphPrefs.Language);

	setupConnections();
}

QList<CharStyle> SMParagraphStyle::getCharStyles()
{
	QList<CharStyle> charStyles;
	if (!m_doc)
		return charStyles; // no doc available

	const StyleSet<CharStyle> &tmp(m_doc->charStyles());
	for (int i = 0; i < tmp.count(); ++i)
		charStyles.append(tmp[i]);
	return charStyles;
}

QString SMParagraphStyle::fromSelection() const
{
	QString lsName(QString::null);
	if (!m_doc)
		return lsName; // no doc available

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
	{
		// wth is going on here
		PageItem *item = m_doc->m_Selection->itemAt(i);

		QString tmpName = item->itemText.defaultStyle().parent();

		if (lsName.isNull() && !tmpName.isEmpty() && tmpName != "")
		{
			lsName = tmpName;
		}
		else if (!lsName.isNull() && !tmpName.isEmpty() && tmpName != "" && lsName != tmpName)
		{
			lsName = QString::null;
			break;
		}
	}
	return lsName;
}

void SMParagraphStyle::toSelection(const QString &styleName) const
{
	if (!m_doc)
		return; // nowhere to apply or no doc

	QString realName = styleName;
	int styleIndex = m_tmpStyles.find(styleName);
	if (styleIndex < 0 && (styleName == CommonStrings::trDefaultParagraphStyle))
	{
		styleIndex = m_tmpStyles.find(CommonStrings::DefaultParagraphStyle);
		if (styleIndex >= 0)
			realName = CommonStrings::DefaultParagraphStyle;
	}
	if (styleIndex >= 0)
	{
		m_doc->itemSelection_SetNamedParagraphStyle(realName);
	}
}

QString SMParagraphStyle::newStyle()
{
	if (!m_doc)
		return QString::null;

	QString s(getUniqueName( tr("New Style")));
	ParagraphStyle p;
	p.setDefaultStyle(false);
	p.setName(s);
	p.charStyle().setLanguage(m_doc->hyphLanguage());
	m_tmpStyles.create(p);
	return s;
}

QString SMParagraphStyle::newStyle(const QString &fromStyle)
{
	//#7179, do our name switch yet again to handle this properly for default styles
	//FIXME: use isDefaultStyle somehow
	QString copiedStyleName(fromStyle);
	if (fromStyle==CommonStrings::trDefaultParagraphStyle)
		copiedStyleName=CommonStrings::DefaultParagraphStyle;

	Q_ASSERT(m_tmpStyles.resolve(copiedStyleName));
	if (!m_tmpStyles.resolve(copiedStyleName))
		return QString::null;

	//Copy the style with the original name
	QString s(getUniqueName( tr("Clone of %1").arg(fromStyle)));
	ParagraphStyle p(m_tmpStyles.get(copiedStyleName));
	p.setDefaultStyle(false);
	p.setName(s);
	p.setShortcut(QString::null); // do not clone the sc
	m_tmpStyles.create(p);

	return s;
}

// helper function to find a unique name to a new style or a clone
QString SMParagraphStyle::getUniqueName(const QString &name)
{
	int id = 0;
	bool done = false;
	QString s(name);

	while (!done)
	{
start:
		++id;
		for (int i = 0; i < m_tmpStyles.count(); ++i)
		{
			if (m_tmpStyles[i].name() == s)
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

void SMParagraphStyle::apply()
{
	if (!m_doc)
		return;
	QMap<QString, QString> replacement;
	for (int i = 0; i < m_deleted.count(); ++i)
	{
		if (m_deleted[i].first == m_deleted[i].second)
			continue;
		replacement[m_deleted[i].first] = m_deleted[i].second;
	}
	m_doc->redefineStyles(m_tmpStyles, false);
	m_doc->replaceStyles(replacement);

	m_deleted.clear(); // deletion done at this point

	m_doc->scMW()->requestUpdate(reqTextStylesUpdate);
	// Better not call DrawNew() here, as this will cause several unnecessary calls
	// m_doc->view()->DrawNew();
	m_doc->changed();
}

void SMParagraphStyle::editMode(bool isOn)
{
	if (isOn)
		reloadTmpStyles();
}

bool SMParagraphStyle::isDefaultStyle(const QString &stylename) const
{
	int index = m_tmpStyles.find(stylename);
	bool b=false;
	if (index > -1)
		b = m_tmpStyles[index].isDefaultStyle();
	else
	{
		if (CommonStrings::trDefaultParagraphStyle==stylename)
		{
			index = m_tmpStyles.find(CommonStrings::DefaultParagraphStyle);
			if (index > -1)
				b = m_tmpStyles[index].isDefaultStyle();
		}
	}
	return b;
}

void SMParagraphStyle::setDefaultStyle(bool ids)
{
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	m_selection[0]->setDefaultStyle(ids);
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

QString SMParagraphStyle::shortcut(const QString &stylename) const
{
	QString s(QString::null);

	int index = m_tmpStyles.find(stylename);
	if (index > -1)
		s = m_tmpStyles[index].shortcut();
	else
	{
		//FIXME: Use isDefaultStyle somehow
		if (CommonStrings::trDefaultParagraphStyle==stylename)
		{
			index = m_tmpStyles.find(CommonStrings::DefaultParagraphStyle);
			if (index > -1)
				s = m_tmpStyles[index].shortcut();
		}
	}

	return s;
}

void SMParagraphStyle::setShortcut(const QString &shortcut)
{
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	m_selection[0]->setShortcut(shortcut);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::deleteStyles(const QList<RemoveItem> &removeList)
{
	for (int i = 0; i < removeList.count(); ++i)
	{
		for (int k = 0; k < m_selection.count(); ++k)
		{
			if (m_selection[k]->name() == removeList[i].first)
			{
				m_selection.removeAt(k);
				break;
			}
		}

		int index = m_tmpStyles.find(removeList[i].first);
		if (index > -1)
			m_tmpStyles.remove(index);

		m_deleted.append(removeList[i]);
	}
}

void SMParagraphStyle::nameChanged(const QString &newName)
{
	if (m_selection.count() != 1)
		return;

	QString oldName(m_selection[0]->name());
	ParagraphStyle p(*m_selection[0]);
	p.setName(newName);
	m_tmpStyles.create(p);
	m_selection.clear();
	m_selection.append(&m_tmpStyles[m_tmpStyles.find(newName)]);
	for (int j = 0; j < m_tmpStyles.count(); ++j)
	{
		int index = m_tmpStyles.find(oldName);
		if (index > -1)
		{
			m_tmpStyles.remove(index);
			break;
		}
	}

	for (int j = 0; j < m_tmpStyles.count(); ++j)
	{
		if (m_tmpStyles[j].parent() == oldName)
			m_tmpStyles[j].setParent(newName);
	}

	QList<RemoveItem>::iterator it;
	for (it = m_deleted.begin(); it != m_deleted.end(); ++it)
	{
		if (it->second == oldName)
		{
			oldName = (*it).first;
			m_deleted.erase(it);
			break;
		}
	}

	if (oldName != newName)
		m_deleted.append(RemoveItem(oldName, newName));

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::languageChange()
{
	if (m_pwidget)
	{
		m_pwidget->languageChange();
		m_pwidget->cpage->languageChange();
	}
}

void SMParagraphStyle::unitChange()
{
	double oldRatio = m_unitRatio;
	m_unitRatio = m_doc->unitRatio();
	m_pwidget->unitChange(oldRatio, m_unitRatio, m_doc->unitIndex());
}

void SMParagraphStyle::reloadTmpStyles()
{
	if (!m_doc)
		return;

	m_selection.clear();
	m_tmpStyles.clear();
	m_deleted.clear();
	m_tmpStyles.redefine(m_doc->paragraphStyles(), true);
	Q_ASSERT(m_tmpStyles.count() > 0);
	m_tmpStyles[0].charStyle().setContext(m_cstyles);
}

void SMParagraphStyle::setupConnections()
{
	if (!m_pwidget)
		return;

	// paragraph attributes
	connect(m_pwidget->lineSpacingMode, SIGNAL(activated(int)), this, SLOT(slotLineSpacingMode(int)));
	connect(m_pwidget->lineSpacing, SIGNAL(valueChanged(double)), this, SLOT(slotLineSpacing()));
	connect(m_pwidget->spaceAbove, SIGNAL(valueChanged(double)), this, SLOT(slotSpaceAbove()));
	connect(m_pwidget->spaceBelow, SIGNAL(valueChanged(double)), this, SLOT(slotSpaceBelow()));
	connect(m_pwidget->alignement->TextL, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(m_pwidget->alignement->TextR, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(m_pwidget->alignement->TextC, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(m_pwidget->alignement->TextB, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(m_pwidget->alignement->TextF, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	connect(m_pwidget->alignement->parentButton, SIGNAL(clicked()), this, SLOT(slotAlignment()));
//	connect(m_pwidget->optMarginCombo, SIGNAL(activated(int)), this, SLOT(slotOpticalMargin(int)));
	connect(m_pwidget->optMarginRadioNone, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	connect(m_pwidget->optMarginRadioLeft, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	connect(m_pwidget->optMarginRadioRight, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	connect(m_pwidget->optMarginRadioBoth, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	connect(m_pwidget, SIGNAL(useParentOptMargins()), this, SLOT(slotParentOpticalMargin()));
	
	connect(m_pwidget->minSpaceSpin, SIGNAL(valueChanged(double)),this,SLOT(slotMinSpace()));
	connect(m_pwidget->minGlyphExtSpin, SIGNAL(valueChanged(double)),this,SLOT(slotMinGlyphExt()));
	connect(m_pwidget->maxGlyphExtSpin, SIGNAL(valueChanged(double)),this,SLOT(slotMaxGlyphExt()));

	connect(m_pwidget, SIGNAL(useParentParaEffects()), this, SLOT(slotParentParaEffects()));
	connect(m_pwidget->dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap(bool)));
	connect(m_pwidget->dropCapLines, SIGNAL(valueChanged(int)), this, SLOT(slotDropCapLines(int)));
	connect(m_pwidget->parEffectOffset, SIGNAL(valueChanged(double)), this, SLOT(slotParEffectOffset()));
	connect(m_pwidget->parEffectIndentBox, SIGNAL(toggled(bool)), this, SLOT(slotParEffectIndent(bool)));
	connect(m_pwidget->parEffectCharStyleCombo, SIGNAL(activated(const QString&)), this, SLOT(slotParEffectCharStyle(const QString&)));

	connect(m_pwidget->bulletBox, SIGNAL(toggled(bool)), this, SLOT(slotBullet(bool)));
	connect(m_pwidget->bulletStrEdit, SIGNAL(editTextChanged(QString)), this, SLOT(slotBulletStr(QString)));
	connect(m_pwidget->numBox, SIGNAL(toggled(bool)), this, SLOT(slotNumeration(bool)));
	connect(m_pwidget->numComboBox, SIGNAL(activated(QString)), this, SLOT(slotNumName(QString)));
	connect(m_pwidget->numLevelSpin, SIGNAL(valueChanged(int)), this, SLOT(slotNumLevel(int)));
	connect(m_pwidget->numFormatCombo, SIGNAL(activated(int)), this, SLOT(slotNumFormat(int)));
	connect(m_pwidget->numStartSpin, SIGNAL(valueChanged(int)), this, SLOT(slotNumStart(int)));
	connect(m_pwidget->numRestartCombo, SIGNAL(activated(int)), this, SLOT(slotNumRestart(int)));
	connect(m_pwidget->numRestartOtherBox, SIGNAL(toggled(bool)), this, SLOT(slotNumOther(bool)));
	connect(m_pwidget->numRestartHigherBox, SIGNAL(toggled(bool)), this, SLOT(slotNumHigher(bool)));
	connect(m_pwidget->numPrefix, SIGNAL(textChanged(QString)), this, SLOT(slotNumPrefix(QString)));
	connect(m_pwidget->numSuffix, SIGNAL(textChanged(QString)), this, SLOT(slotNumSuffix(QString)));
	connect(m_pwidget->numNewLineEdit, SIGNAL(editingFinished()), this, SLOT(slotNumNew()));
	connect(m_pwidget->numNewLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSelectionDirty()));

	connect(m_pwidget->keepLinesStart, SIGNAL(valueChanged(int)), this, SLOT(handleKeepLinesStart()));
	connect(m_pwidget->keepLinesEnd, SIGNAL(valueChanged(int)), this, SLOT(handleKeepLinesEnd()));
	connect(m_pwidget->keepTogether, SIGNAL(stateChanged(int)), this, SLOT(handleKeepTogether()));
	connect(m_pwidget->keepWithNext, SIGNAL(stateChanged(int)), this, SLOT(handleKeepWithNext()));

	connect(m_pwidget->tabList, SIGNAL(tabsChanged()), this, SLOT(slotTabRuler()));
	connect(m_pwidget->tabList, SIGNAL(mouseReleased()), this, SLOT(slotTabRuler()));
	connect(m_pwidget->tabList->left_, SIGNAL(valueChanged(double)), this, SLOT(slotLeftIndent()));
	connect(m_pwidget->tabList->right_, SIGNAL(valueChanged(double)), this, SLOT(slotRightIndent()));
	connect(m_pwidget->tabList->first_, SIGNAL(valueChanged(double)), this, SLOT(slotFirstLine()));

	connect(m_pwidget->parentCombo, SIGNAL(activated(const QString&)), this, SLOT(slotParentChanged(const QString&)));

	// character attributes
	connect(m_pwidget->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(m_pwidget->cpage->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	connect(m_pwidget->cpage->effects_->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	connect(m_pwidget->cpage->effects_->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	connect(m_pwidget->cpage->effects_->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	connect(m_pwidget->cpage->effects_->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	connect(m_pwidget->cpage->effects_->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	connect(m_pwidget->cpage->effects_->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	connect(m_pwidget->cpage->effects_->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	connect(m_pwidget->cpage->fillColor_, SIGNAL(activated(const QString&)), this, SLOT(slotFillColor()));
	connect(m_pwidget->cpage->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	connect(m_pwidget->cpage->strokeColor_, SIGNAL(activated(const QString&)), this, SLOT(slotStrokeColor()));
	connect(m_pwidget->cpage->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	connect(m_pwidget->cpage->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	connect(m_pwidget->cpage->fontSize_, SIGNAL(valueChanged(double)), this, SLOT(slotFontSize()));
	connect(m_pwidget->cpage->fontHScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleH()));
	connect(m_pwidget->cpage->fontVScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleV()));
	connect(m_pwidget->cpage->tracking_, SIGNAL(valueChanged(double)), this, SLOT(slotTracking()));
	connect(m_pwidget->cpage->widthSpaceSpin, SIGNAL(valueChanged(double)), this, SLOT(slotWordTracking()));
	connect(m_pwidget->cpage->baselineOffset_, SIGNAL(valueChanged(double)), this, SLOT(slotBaselineOffset()));
	connect(m_pwidget->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(m_pwidget->cpage->parentCombo, SIGNAL(activated(const QString&)), this, SLOT(slotCharParentChanged(const QString&)));
}

void SMParagraphStyle::removeConnections()
{
	if (!m_pwidget)
		return;

	disconnect(m_pwidget->lineSpacingMode, SIGNAL(activated(int)), this, SLOT(slotLineSpacingMode(int)));
	disconnect(m_pwidget->lineSpacing, SIGNAL(valueChanged(double)), this, SLOT(slotLineSpacing()));
	disconnect(m_pwidget->spaceAbove, SIGNAL(valueChanged(double)), this, SLOT(slotSpaceAbove()));
	disconnect(m_pwidget->spaceBelow, SIGNAL(valueChanged(double)), this, SLOT(slotSpaceBelow()));
	disconnect(m_pwidget->alignement->TextL, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(m_pwidget->alignement->TextR, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(m_pwidget->alignement->TextC, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(m_pwidget->alignement->TextB, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(m_pwidget->alignement->TextF, SIGNAL(clicked()), this, SLOT(slotAlignment()));
	disconnect(m_pwidget->alignement->parentButton, SIGNAL(clicked()), this, SLOT(slotAlignment()));
//	disconnect(m_pwidget->optMarginCombo, SIGNAL(activated(int)), this, SLOT(slotOpticalMargin(int)));
	disconnect(m_pwidget->optMarginRadioNone, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	disconnect(m_pwidget->optMarginRadioLeft, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	disconnect(m_pwidget->optMarginRadioRight, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	disconnect(m_pwidget->optMarginRadioBoth, SIGNAL(clicked()), this, SLOT(slotOpticalMarginSelector()));
	
	disconnect(m_pwidget->minSpaceSpin, SIGNAL(valueChanged(double)),this,SLOT(slotMinSpace()));
	disconnect(m_pwidget->minGlyphExtSpin, SIGNAL(valueChanged(double)),this,SLOT(slotMinGlyphExt()));
	disconnect(m_pwidget->maxGlyphExtSpin, SIGNAL(valueChanged(double)),this,SLOT(slotMaxGlyphExt()));
	
	disconnect(m_pwidget, SIGNAL(useParentParaEffects()), this, SLOT(slotParentParaEffects()));
	disconnect(m_pwidget->dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap(bool)));
	disconnect(m_pwidget->dropCapLines, SIGNAL(valueChanged(int)), this, SLOT(slotDropCapLines(int)));
	disconnect(m_pwidget->parEffectOffset, SIGNAL(valueChanged(double)), this, SLOT(slotParEffectOffset()));
	disconnect(m_pwidget->parEffectIndentBox, SIGNAL(toggled(bool)), this, SLOT(slotParEffectIndent(bool)));
	disconnect(m_pwidget->parEffectCharStyleCombo, SIGNAL(activated(const QString&)), this, SLOT(slotParEffectCharStyle(const QString&)));

	disconnect(m_pwidget->bulletBox, SIGNAL(toggled(bool)), this, SLOT(slotBullet(bool)));
	disconnect(m_pwidget->bulletStrEdit, SIGNAL(editTextChanged(QString)), this, SLOT(slotBulletStr(QString)));
	disconnect(m_pwidget->numBox, SIGNAL(toggled(bool)), this, SLOT(slotNumeration(bool)));
	disconnect(m_pwidget->numComboBox, SIGNAL(activated(QString)), this, SLOT(slotNumName(QString)));
	disconnect(m_pwidget->numFormatCombo, SIGNAL(activated(int)), this, SLOT(slotNumFormat(int)));
	disconnect(m_pwidget->numLevelSpin, SIGNAL(valueChanged(int)), this, SLOT(slotNumLevel(int)));
	disconnect(m_pwidget->numStartSpin, SIGNAL(valueChanged(int)), this, SLOT(slotNumStart(int)));
	disconnect(m_pwidget->numRestartCombo, SIGNAL(activated(int)), this, SLOT(slotNumRestart(int)));
	disconnect(m_pwidget->numRestartOtherBox, SIGNAL(toggled(bool)), this, SLOT(slotNumOther(bool)));
	disconnect(m_pwidget->numRestartHigherBox, SIGNAL(toggled(bool)), this, SLOT(slotNumHigher(bool)));
	disconnect(m_pwidget->numPrefix, SIGNAL(textChanged(QString)), this, SLOT(slotNumPrefix(QString)));
	disconnect(m_pwidget->numSuffix, SIGNAL(textChanged(QString)), this, SLOT(slotNumSuffix(QString)));
	disconnect(m_pwidget->numNewLineEdit, SIGNAL(editingFinished()), this, SLOT(slotNumNew()));
	disconnect(m_pwidget->numNewLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSelectionDirty()));
	
	disconnect(m_pwidget->parentCombo, SIGNAL(activated(const QString&)), this, SLOT(slotParentChanged(const QString&)));

	disconnect(m_pwidget->keepLinesStart, SIGNAL(valueChanged(int)), this, SLOT(handleKeepLinesStart()));
	disconnect(m_pwidget->keepLinesEnd, SIGNAL(valueChanged(int)), this, SLOT(handleKeepLinesEnd()));
	disconnect(m_pwidget->keepTogether, SIGNAL(stateChanged(int)), this, SLOT(handleKeepTogether()));
	disconnect(m_pwidget->keepWithNext, SIGNAL(stateChanged(int)), this, SLOT(handleKeepWithNext()));

	disconnect(m_pwidget->tabList, SIGNAL(tabsChanged()), this, SLOT(slotTabRuler()));
	disconnect(m_pwidget->tabList->left_, SIGNAL(valueChanged(double)), this, SLOT(slotLeftIndent()));
	disconnect(m_pwidget->tabList->right_, SIGNAL(valueChanged(double)), this, SLOT(slotRightIndent()));
	disconnect(m_pwidget->tabList->first_, SIGNAL(valueChanged(double)), this, SLOT(slotFirstLine()));

	disconnect(m_pwidget->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(m_pwidget->cpage->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	disconnect(m_pwidget->cpage->effects_->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	disconnect(m_pwidget->cpage->effects_->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	disconnect(m_pwidget->cpage->effects_->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	disconnect(m_pwidget->cpage->effects_->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	disconnect(m_pwidget->cpage->effects_->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	disconnect(m_pwidget->cpage->effects_->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	disconnect(m_pwidget->cpage->effects_->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(slotEffectProperties()));
	disconnect(m_pwidget->cpage->fillColor_, SIGNAL(activated(const QString&)), this, SLOT(slotFillColor()));
	disconnect(m_pwidget->cpage->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	disconnect(m_pwidget->cpage->strokeColor_, SIGNAL(activated(const QString&)), this, SLOT(slotStrokeColor()));
	disconnect(m_pwidget->cpage->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	disconnect(m_pwidget->cpage->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	disconnect(m_pwidget->cpage->fontSize_, SIGNAL(valueChanged(double)), this, SLOT(slotFontSize()));
	disconnect(m_pwidget->cpage->fontHScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleH()));
	disconnect(m_pwidget->cpage->fontVScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleV()));
	disconnect(m_pwidget->cpage->tracking_, SIGNAL(valueChanged(double)), this, SLOT(slotTracking()));
	disconnect(m_pwidget->cpage->widthSpaceSpin, SIGNAL(valueChanged(double)), this, SLOT(slotWordTracking()));
	disconnect(m_pwidget->cpage->baselineOffset_, SIGNAL(valueChanged(double)), this, SLOT(slotBaselineOffset()));
	disconnect(m_pwidget->cpage->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(m_pwidget->cpage->parentCombo, SIGNAL(activated(const QString&)), this, SLOT(slotCharParentChanged(const QString&)));
}

void SMParagraphStyle::slotLineSpacingMode(int mode)
{
	ParagraphStyle::LineSpacingMode lsm = static_cast<ParagraphStyle::LineSpacingMode>(mode);

	if (m_pwidget->lineSpacingMode->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetLineSpacingMode();
	else
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setLineSpacingMode(lsm);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotLineSpacing()
{
	if (m_pwidget->lineSpacing->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetLineSpacing();
	else 
	{
		double a, b, value;
		int c;

		m_pwidget->lineSpacing->getValues(&a, &b, &c, &value);
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setLineSpacing(value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotSpaceAbove()
{
	if (m_pwidget->spaceAbove->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetGapBefore();
	else 
	{
		double a, b, value;
		int c;

		m_pwidget->spaceAbove->getValues(&a, &b, &c, &value);
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setGapBefore(value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotSpaceBelow()
{
	if (m_pwidget->spaceBelow->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetGapAfter();
	else 
	{
		double a, b, value;
		int c;
		
		m_pwidget->spaceBelow->getValues(&a, &b, &c, &value);
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setGapAfter(value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotAlignment()
{
	ParagraphStyle::AlignmentType style = static_cast<ParagraphStyle::AlignmentType>(m_pwidget->alignement->getStyle());
	if (m_pwidget->alignement->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetAlignment();
	else 
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setAlignment(style);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotOpticalMargin(int i)
{
//	ParagraphStyle::OpticalMarginType omt( static_cast<ParagraphStyle::OpticalMarginType>(m_pwidget->optMarginCombo->itemData(i).toInt()));
//	if (m_pwidget->optMarginCombo->useParentValue())
//		for (int i = 0; i < m_selection.count(); ++i)
//			m_selection[i]->resetOpticalMargins();
//	else
//		for (int i = 0; i < m_selection.count(); ++i)
//			m_selection[i]->setOpticalMargins(omt);
//
//	if (!m_selectionIsDirty)
//	{
//		m_selectionIsDirty = true;
//		emit selectionDirty();
//	}
}

void SMParagraphStyle::slotOpticalMarginSelector()
{
	int omt(ParagraphStyle::OM_None);
	if (false)//(m_pwidget->optMarginCombo->useParentValue())
	{
	}
	else
	{
// 		if (m_pwidget->optMarginRadioNone->isChecked()) omt = aragraphStyle::OM_None;
		if (m_pwidget->optMarginRadioRight->isChecked()) omt = ParagraphStyle::OM_RightHangingPunct;
		else if (m_pwidget->optMarginRadioLeft->isChecked()) omt = ParagraphStyle::OM_LeftHangingPunct;
		else if (m_pwidget->optMarginRadioBoth->isChecked()) omt = ParagraphStyle::OM_Default;

		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setOpticalMargins(omt);
	}
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotParentOpticalMargin()
{
	for (int i = 0; i < m_selection.count(); ++i)
		m_selection[i]->resetOpticalMargins();

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotMinSpace()
{
	if (m_pwidget->minSpaceSpin->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetMinWordTracking();
	else 
	{
		double ms(m_pwidget->minSpaceSpin->getValue(SC_PERCENT));
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setMinWordTracking(ms / 100.0);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotMinGlyphExt()
{
	if (m_pwidget->minGlyphExtSpin->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetMinGlyphExtension();
	else 
	{
		double mge(m_pwidget->minGlyphExtSpin->getValue(SC_PERCENT));
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setMinGlyphExtension(mge / 100.0);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotMaxGlyphExt()
{
	if (m_pwidget->maxGlyphExtSpin->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetMaxGlyphExtension();
	else 
	{
		double mge(m_pwidget->maxGlyphExtSpin->getValue(SC_PERCENT));
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setMaxGlyphExtension(mge / 100.0);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotDropCap(bool isOn)
{
	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->setHasDropCap(isOn);
		if (isOn)
		{
			m_selection[i]->setHasBullet(false);
			m_selection[i]->setHasNum(false);
		}
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotParentParaEffects()
{
	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->resetHasDropCap();
		m_selection[i]->resetHasBullet();
		m_selection[i]->resetHasNum();
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotDropCapLines(int lines)
{
	if (m_pwidget->dropCapLines->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetDropCapLines();
	else		
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setDropCapLines(lines);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotParEffectOffset()
{
	if (m_pwidget->parEffectOffset->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetParEffectOffset();
	else 
	{
		double a, b, value;
		int c;

		m_pwidget->parEffectOffset->getValues(&a, &b, &c, &value);
		value = value / m_unitRatio;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setParEffectOffset(value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotParEffectIndent(bool isOn)
{
	if (m_pwidget->parEffectIndentBox->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetParEffectIndent();
	else 
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setParEffectIndent(isOn);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotParEffectCharStyle(const QString& name)
{
	if (m_pwidget->parEffectCharStyleCombo->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetPeCharStyleName();
	else
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setPeCharStyleName(name);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotBullet(bool isOn)
{
	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->setHasBullet(isOn);
		if (isOn)
		{
			m_selection[i]->setBulletStr(m_pwidget->bulletStrEdit->currentText());
			m_selection[i]->setHasDropCap(false);
			m_selection[i]->setHasNum(false);
		}
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotBulletStr(const QString &str)
{
	QString bstr(str);
	if (bstr.isEmpty())
	{
		bstr = m_pwidget->bulletStrEdit->itemText(0);
		m_pwidget->bulletStrEdit->setEditText(bstr);
	}
	for (int i = 0; i < m_selection.count(); ++i)
		m_selection[i]->setBulletStr(bstr);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumeration(bool isOn)
{
	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->setHasNum(isOn);
		if (isOn)
		{
			m_selection[i]->setHasDropCap(false);
			m_selection[i]->setHasBullet(false);
		}
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumName(const QString &str)
{
	QString bstr(str);
	if (!str.isEmpty())
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setNumName(bstr);
		m_pwidget->numComboBox->setCurrentItem(m_pwidget->numComboBox->findText(m_selection[0]->numName()));
		m_pwidget->numLevelSpin->setValue(m_selection[0]->numLevel()+1);
		NumStruct * numS = m_doc->numerations.value(m_selection[0]->numName());
		if (numS)
			m_pwidget->numLevelSpin->setMaximum(numS->m_counters.count()+1);
		else
			m_pwidget->numLevelSpin->setMaximum(1);
		m_doc->flag_NumUpdateRequest = true;
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumNew()
{
	QString newName = m_pwidget->numNewLineEdit->text();
	if (!newName.isEmpty())
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setNumName(newName);
		m_doc->flag_NumUpdateRequest = true;
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotSelectionDirty()
{
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumFormat(int numFormat)
{
	if (m_pwidget->numFormatCombo->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetNumFormat();
	else
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setNumFormat(numFormat);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumLevel(int level)
{
	if (m_pwidget->numLevelSpin->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetNumLevel();
	else
		for (int i = 0; i < m_selection.count(); ++i)
		{
			m_selection[i]->setNumLevel(level-1);
		}
	
	if (level == 0)
		slotNumHigher(false);
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumPrefix(const QString &str)
{
	for (int i = 0; i < m_selection.count(); ++i)
		m_selection[i]->setNumPrefix(str);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumSuffix(const QString &str)
{
	for (int i = 0; i < m_selection.count(); ++i)
		m_selection[i]->setNumSuffix(str);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumStart(int start)
{
	if (m_pwidget->numStartSpin->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetNumStart();
	else
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setNumStart(start);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumRestart(int restart)
{
	if (m_pwidget->numRestartCombo->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetNumRestart();
	else
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setNumRestart(restart);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumOther(bool isOn)
{
	if (m_pwidget->numRestartOtherBox->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetNumOther();
	else 
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setNumOther(isOn);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotNumHigher(bool isOn)
{
	if (m_pwidget->numRestartHigherBox->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetNumHigher();
	else 
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setNumHigher(isOn);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}


void SMParagraphStyle::handleKeepLinesStart()
{
	if (m_pwidget->keepLinesStart->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetKeepLinesStart();
	else 
	{
		int value = m_pwidget->keepLinesStart->value();
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setKeepLinesStart (value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::handleKeepLinesEnd()
{
	if (m_pwidget->keepLinesEnd->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetKeepLinesEnd();
	else 
	{
		int value = m_pwidget->keepLinesEnd->value();
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setKeepLinesEnd (value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::handleKeepTogether()
{
	if (m_pwidget->keepTogether->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetKeepTogether();
	else 
	{
		bool value = m_pwidget->keepTogether->isChecked();
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setKeepTogether (value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::handleKeepWithNext()
{
	if (m_pwidget->keepWithNext->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetKeepWithNext();
	else 
	{
		bool value = m_pwidget->keepWithNext->isChecked();
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setKeepWithNext (value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotTabRuler()
{
	if (m_pwidget->tabList->useParentTabs())
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetTabValues();
	}
	else
	{
		QList<ParagraphStyle::TabRecord> newTabs = m_pwidget->tabList->getTabVals();
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setTabValues(newTabs);
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotLeftIndent()
{
	if (m_pwidget->tabList->useParentLeftIndent())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetLeftMargin();
	else 
	{
		double a, b, value;
		int c;

		m_pwidget->tabList->left_->getValues(&a, &b, &c, &value);
		value = value / m_unitRatio;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setLeftMargin(value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotRightIndent()
{
	if (m_pwidget->tabList->useParentRightIndent())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetRightMargin();
	else 
	{
		double a, b, value;
		int c;

		m_pwidget->tabList->right_->getValues(&a, &b, &c, &value);
		value = value / m_unitRatio;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setRightMargin(value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFirstLine()
{
	if (m_pwidget->tabList->useParentFirstLine())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetFirstIndent();
	else 
	{
		double a, b, value;
		int c;
		
		m_pwidget->tabList->first_->getValues(&a, &b, &c, &value);
		value = value / m_unitRatio;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setFirstIndent(value);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFontSize()
{
	if (m_pwidget->cpage->fontSize_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetFontSize();
	else
	{
		double a, b, value;
		int c;
		
		m_pwidget->cpage->fontSize_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setFontSize(qRound(value));
	}


	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotEffects(int e)
{
	StyleFlag s = ScStyle_None;
	if (m_pwidget->cpage->effects_->useParentValue())
	{
		for (int i = 0; i < m_selection.count(); ++i)
		{
			m_selection[i]->charStyle().resetFeatures();
			m_selection[i]->charStyle().resetShadowXOffset();
			m_selection[i]->charStyle().resetShadowYOffset();
			m_selection[i]->charStyle().resetOutlineWidth();
			m_selection[i]->charStyle().resetUnderlineOffset();
			m_selection[i]->charStyle().resetUnderlineWidth();
			m_selection[i]->charStyle().resetStrikethruOffset();
			m_selection[i]->charStyle().resetStrikethruWidth();
		}
	}
	else
	{
		double a, b, sxo, syo, olw, ulp, ulw, slp, slw;
		int c;
		
		s = static_cast<StyleFlag>(e);
		m_pwidget->cpage->effects_->ShadowVal->Xoffset->getValues(&a, &b, &c, &sxo);
		sxo *= 10;
		m_pwidget->cpage->effects_->ShadowVal->Yoffset->getValues(&a, &b, &c, &syo);
		syo *= 10;

		m_pwidget->cpage->effects_->OutlineVal->LWidth->getValues(&a, &b, &c, &olw);
		olw *= 10;

		m_pwidget->cpage->effects_->UnderlineVal->LPos->getValues(&a, &b, &c, &ulp);
		ulp *= 10;
		m_pwidget->cpage->effects_->UnderlineVal->LWidth->getValues(&a, &b, &c, &ulw);
		ulw *= 10;

		m_pwidget->cpage->effects_->StrikeVal->LPos->getValues(&a, &b, &c, &slp);
		slp *= 10;
		m_pwidget->cpage->effects_->StrikeVal->LWidth->getValues(&a, &b, &c, &slw);
		slw *= 10;

		for (int i = 0; i < m_selection.count(); ++i)
		{
			QStringList feList = s.featureList();
			feList.removeAll(CharStyle::INHERIT);
			m_selection[i]->charStyle().setFeatures(feList);
//			m_selection[i]->charStyle().setFeatures(s.featureList());
			m_selection[i]->charStyle().setShadowXOffset(qRound(sxo));
			m_selection[i]->charStyle().setShadowYOffset(qRound(syo));
			m_selection[i]->charStyle().setOutlineWidth(qRound(olw));
			m_selection[i]->charStyle().setUnderlineOffset(qRound(ulp));
			m_selection[i]->charStyle().setUnderlineWidth(qRound(ulw));
			m_selection[i]->charStyle().setStrikethruOffset(qRound(slp));
			m_selection[i]->charStyle().setStrikethruWidth(qRound(slw));
		}
	}


	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotEffectProperties()
{
	double a, b, sxo, syo, olw, ulp, ulw, slp, slw;
	int c;

	m_pwidget->cpage->effects_->ShadowVal->Xoffset->getValues(&a, &b, &c, &sxo);
	sxo *= 10;
	m_pwidget->cpage->effects_->ShadowVal->Yoffset->getValues(&a, &b, &c, &syo);
	syo *= 10;

	m_pwidget->cpage->effects_->OutlineVal->LWidth->getValues(&a, &b, &c, &olw);
	olw *= 10;

	m_pwidget->cpage->effects_->UnderlineVal->LPos->getValues(&a, &b, &c, &ulp);
	ulp *= 10;
	m_pwidget->cpage->effects_->UnderlineVal->LWidth->getValues(&a, &b, &c, &ulw);
	ulw *= 10;

	m_pwidget->cpage->effects_->StrikeVal->LPos->getValues(&a, &b, &c, &slp);
	slp *= 10;
	m_pwidget->cpage->effects_->StrikeVal->LWidth->getValues(&a, &b, &c, &slw);
	slw *= 10;
	
	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->charStyle().setShadowXOffset(qRound(sxo));
		m_selection[i]->charStyle().setShadowYOffset(qRound(syo));
		m_selection[i]->charStyle().setOutlineWidth(qRound(olw));
		m_selection[i]->charStyle().setUnderlineOffset(qRound(ulp));
		m_selection[i]->charStyle().setUnderlineWidth(qRound(ulw));
		m_selection[i]->charStyle().setStrikethruOffset(qRound(slp));
		m_selection[i]->charStyle().setStrikethruWidth(qRound(slw));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFillColor()
{
	if (m_pwidget->cpage->fillColor_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetFillColor();
	else {
		QString col( m_pwidget->cpage->fillColor_->currentText());
		if (col == CommonStrings::tr_NoneColor)
			col = CommonStrings::None;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setFillColor(col);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFillShade()
{
	if (m_pwidget->cpage->fillShade_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetFillShade();
	else {
		int fs = m_pwidget->cpage->fillShade_->getValue();

		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setFillShade(fs);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotStrokeColor()
{
	if (m_pwidget->cpage->strokeColor_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetStrokeColor();
	else {
		QString col(m_pwidget->cpage->strokeColor_->currentText());
		if (col == CommonStrings::tr_NoneColor)
			col = CommonStrings::None;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setStrokeColor(col);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotStrokeShade()
{
	if (m_pwidget->cpage->strokeShade_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetStrokeShade();
	else 
	{
		int ss = m_pwidget->cpage->strokeShade_->getValue();

		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setStrokeShade(ss);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotLanguage()
{
	QMap<QString,QString>::Iterator it;
	QString language = m_doc->paragraphStyle("").charStyle().language();

	if (m_pwidget->cpage->language_->useParentValue())
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetLanguage();
	}
	else
	{
		QString la=LanguageManager::instance()->getAbbrevFromLang(m_pwidget->cpage->language_->currentText(), true, false);
		if (!la.isEmpty())
			language=la;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setLanguage(language);
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotScaleH()
{
	if (m_pwidget->cpage->fontHScale_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetScaleH();
	else
	{
		double a, b, value;
		int c;
		m_pwidget->cpage->fontHScale_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setScaleH(qRound(value));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotScaleV()
{
	if (m_pwidget->cpage->fontVScale_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetScaleV();
	else
	{
		double a, b, value;
		int c;
		m_pwidget->cpage->fontVScale_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setScaleV(qRound(value));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotTracking()
{
	if (m_pwidget->cpage->tracking_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetTracking();
	else
	{
		double a, b, value;
		int c;
		m_pwidget->cpage->tracking_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setTracking(qRound(value));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotWordTracking()
{
	if (m_pwidget->cpage->widthSpaceSpin->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetWordTracking();
	else
	{
		double a, b, value;
		int c;
		m_pwidget->cpage->widthSpaceSpin->getValues(&a, &b, &c, &value);
		value = value / 100.0;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setWordTracking(value);
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotBaselineOffset()
{
	if (m_pwidget->cpage->baselineOffset_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetBaselineOffset();
	else
	{
		double a, b, value;
		int c;	
		m_pwidget->cpage->baselineOffset_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setBaselineOffset(qRound(value));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotFont(QString s)
{
	if (m_pwidget->cpage->fontFace_->useParentFont())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().resetFont();
	else {
		ScFace sf = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[s];
		
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->charStyle().setFont(sf);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotParentChanged(const QString &parent)
{
	Q_ASSERT(!parent.isNull());

	bool  loop = false, parentLoop = false;
	const Style* parentStyle = (!parent.isEmpty()) ? m_tmpStyles.resolve(parent) : NULL;
	QStringList sel;

	for (int i = 0; i < m_selection.count(); ++i)
	{
		loop = false;
		// Check if setting parent won't create a loop
		const Style* pStyle = parentStyle;
		while (pStyle)
		{
			if (pStyle->hasParent() && (pStyle->parent() == m_selection[i]->name()))
			{
				loop = parentLoop = true;
				break;
			}
			pStyle = pStyle->hasParent() ? pStyle->parentStyle() : NULL;
		}
		if (!loop)
		{
			m_selection[i]->erase(); // reset everything to NOVALUE
			m_selection[i]->setParent(parent);
			m_selection[i]->charStyle().setParent("");
		}
		sel << m_selection[i]->name();
	}

	if (parentLoop)
		QMessageBox::warning(this->widget(), CommonStrings::trWarning, tr("Setting that style as parent would create an infinite loop."), CommonStrings::tr_OK);

	selected(sel);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMParagraphStyle::slotCharParentChanged(const QString &parent)
{
	Q_ASSERT(parent != QString::null);

	QStringList sel;

	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->charStyle().erase();
		if (parent != QString::null)
			m_selection[i]->charStyle().setParent(parent);

		sel << m_selection[i]->name();
	}

	selected(sel);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

SMParagraphStyle::~SMParagraphStyle()
{
	delete m_pwidget;
	m_pwidget = 0;
}

/******************************************************************************/
/******************************************************************************/

SMCharacterStyle::SMCharacterStyle() : StyleItem(), m_widget(0), m_page(0), m_doc(0), m_selectionIsDirty(false)
{
	m_widget = new QTabWidget();
	Q_CHECK_PTR(m_widget);
	m_widget->setContentsMargins(5, 5, 5, 5);//CB the SMCStylePage parent has a 0 value to fit properly onto the pstyle page, so add it here
	m_page = new SMCStyleWidget();
	Q_CHECK_PTR(m_page);
	m_widget->addTab(m_page, tr("Properties"));
}

QTabWidget* SMCharacterStyle::widget()
{
	return m_widget;
}

QString SMCharacterStyle::typeNamePlural()
{
	return tr("Character Styles");
}

QString SMCharacterStyle::typeNameSingular()
{
	return tr("Character Style");
}

void SMCharacterStyle::setCurrentDoc(ScribusDoc *doc)
{
	m_doc = doc;
	if (m_doc)
	{
		if (m_page)
		{
			QStringList languageList;
			LanguageManager::instance()->fillInstalledHyphStringList(&languageList);
			m_page->fillLangComboFromList(languageList);
			m_page->fillColorCombo(m_doc->PageColors);
			m_page->fontFace_->RebuildList(m_doc);
		}
	}
	else
	{
		removeConnections();
		m_selection.clear();
		m_tmpStyles.clear();
	}

}

StyleSet<CharStyle>* SMCharacterStyle::tmpStyles()
{
	return &m_tmpStyles;
}

QList<StyleName> SMCharacterStyle::styles(bool reloadFromDoc)
{
	QList<StyleName> tmpList;

	if (!m_doc)
		return tmpList; // no doc available

	if (reloadFromDoc)
		reloadTmpStyles();

	for (int i = 0; i < m_tmpStyles.count(); ++i)
	{
		if (m_tmpStyles[i].hasName())
		{
			QString styleName(m_tmpStyles[i].displayName());
			QString parentName(QString::null);

			if (m_tmpStyles[i].hasParent())
			{
				const Style* parentStyle = m_tmpStyles[i].parentStyle();
				if (parentStyle)
					parentName = parentStyle->displayName();
			}

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
	m_selection.clear();
	m_selectionIsDirty = false;
	removeConnections();
	QList<CharStyle> cstyles;

	m_tmpStyles.invalidate();

	for (int i = 0; i < m_tmpStyles.count(); ++i)
		cstyles << m_tmpStyles[i];

	for (int i = 0; i < styleNames.count(); ++i)
	{
		int index = m_tmpStyles.find(styleNames[i]);
		//FIXME: #7133: Use .isDefaultStyle() instead here rather than relying on tr text comparison
		if (index<0 && styleNames[i]==CommonStrings::trDefaultCharacterStyle)
			index = m_tmpStyles.find(CommonStrings::DefaultCharacterStyle);
		if (index > -1)
			m_selection.append(&m_tmpStyles[index]);

	}
	m_page->show(m_selection, cstyles, PrefsManager::instance()->appPrefs.hyphPrefs.Language, m_doc->unitIndex());
	setupConnections();
}

QString SMCharacterStyle::fromSelection() const
{
	QString lsName(QString::null);
	if (!m_doc)
		return lsName; // no doc available

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
	{
		// wth is going on here
		PageItem *item = m_doc->m_Selection->itemAt(i);

		QString tmpName = item->itemText.defaultStyle().charStyle().parent();

		if (lsName.isNull() && !tmpName.isEmpty() && tmpName != "")
		{
			lsName = tmpName;
		}
		else if (!lsName.isNull() && !tmpName.isEmpty() && tmpName != "" && lsName != tmpName)
		{
			lsName = QString::null;
			break;
		}
	}
	return lsName;
}

void SMCharacterStyle::toSelection(const QString &styleName) const
{
	if (!m_doc)
		return; // nowhere to apply or no doc

	QString realName = styleName;
	int styleIndex = m_tmpStyles.find(styleName);
	if (styleIndex < 0 && (styleName == CommonStrings::trDefaultCharacterStyle))
	{
		styleIndex = m_tmpStyles.find(CommonStrings::DefaultCharacterStyle);
		if (styleIndex >= 0)
			realName = CommonStrings::DefaultCharacterStyle;
	}
	if (styleIndex >= 0)
	{
		m_doc->itemSelection_SetNamedCharStyle(realName);
	}
}

QString SMCharacterStyle::newStyle()
{
	Q_ASSERT(m_doc && m_doc->paragraphStyles().count() > 0);

	QString s = getUniqueName( tr("New Style"));
	CharStyle c;
	c.setDefaultStyle(false);
	c.setName(s);
	// #7360  - rather here than in CharStyle constructor as we have a pointer to doc.
	c.setLanguage(m_doc->hyphLanguage());
	m_tmpStyles.create(c);
	return s;
}

QString SMCharacterStyle::newStyle(const QString &fromStyle)
{
	//#7179, do our name switch yet again to handle this properly for default styles
	//FIXME: use isDefaultStyle somehow
	QString copiedStyleName(fromStyle);
	if (fromStyle==CommonStrings::trDefaultCharacterStyle)
		copiedStyleName=CommonStrings::DefaultCharacterStyle;

	Q_ASSERT(m_tmpStyles.resolve(copiedStyleName));
	if (!m_tmpStyles.resolve(copiedStyleName))
		return QString::null;
	//Copy the style with the original name
	QString s(getUniqueName( tr("Clone of %1").arg(fromStyle)));
	CharStyle c(m_tmpStyles.get(copiedStyleName));
	c.setDefaultStyle(false);
	c.setName(s);
	c.setShortcut(QString::null);
	m_tmpStyles.create(c);

	return s;
}

QString SMCharacterStyle::getUniqueName(const QString &name)
{
	int id = 0;
	bool done = false;
	QString s(name);

	while (!done)
	{
start:
		++id;
		for (int i = 0; i < m_tmpStyles.count(); ++i)
		{
			if (m_tmpStyles[i].name() == s)
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
	if (!m_doc)
		return;

	QMap<QString, QString> replacement;
	for (int i = 0; i < m_deleted.count(); ++i)
	{
		if (m_deleted[i].first == m_deleted[i].second)
			continue;
		replacement[m_deleted[i].first] = m_deleted[i].second;
	}

	m_doc->redefineCharStyles(m_tmpStyles, false);
	m_doc->replaceCharStyles(replacement);

	m_deleted.clear(); // deletion done at this point

	m_doc->scMW()->requestUpdate(reqTextStylesUpdate);
	// Better not call DrawNew() here, as this will cause several unnecessary calls
	// m_doc->view()->DrawNew();
	m_doc->changed();
}

void SMCharacterStyle::editMode(bool isOn)
{
	if (isOn)
		reloadTmpStyles();
}

bool SMCharacterStyle::isDefaultStyle(const QString &stylename) const
{
	int index = m_tmpStyles.find(stylename);
	bool b=false;
	if (index > -1)
		b = m_tmpStyles[index].isDefaultStyle();
	else
	{
		if (CommonStrings::trDefaultCharacterStyle==stylename)
		{
			index = m_tmpStyles.find(CommonStrings::DefaultCharacterStyle);
			if (index > -1)
				b = m_tmpStyles[index].isDefaultStyle();
		}
	}
	return b;
}

void SMCharacterStyle::setDefaultStyle(bool ids)
{
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	m_selection[0]->setDefaultStyle(ids);
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

QString SMCharacterStyle::shortcut(const QString &stylename) const
{
	QString s(QString::null);
	int index = m_tmpStyles.find(stylename);
	if (index > -1)
		s = m_tmpStyles[index].shortcut();
	else
	{
		//FIXME: Use isDefaultStyle somehow
		if (CommonStrings::trDefaultCharacterStyle==stylename)
		{
			index = m_tmpStyles.find(CommonStrings::DefaultCharacterStyle);
			if (index > -1)
				s = m_tmpStyles[index].shortcut();
		}
	}
	return s;
}

void SMCharacterStyle::setShortcut(const QString &shortcut)
{
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	m_selection[0]->setShortcut(shortcut);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::deleteStyles(const QList<RemoveItem> &removeList)
{
	for (int i = 0; i < removeList.count(); ++i)
	{
		for (int k = 0; k < m_selection.count(); ++k)
		{
			if (m_selection[k]->name() == removeList[i].first)
			{
				m_selection.removeAt(k);
				break;
			}
		}

		int index = m_tmpStyles.find(removeList[i].first);
		if (index > -1)
			m_tmpStyles.remove(index);
		m_deleted << removeList[i];
	}
}

void SMCharacterStyle::nameChanged(const QString &newName)
{
// 	for (int i = 0; i < m_selection.count(); ++i)
// 		m_selection[i]->setName(newName);

	QString oldName(m_selection[0]->name());
	CharStyle c(*m_selection[0]);
	c.setName(newName);
	m_tmpStyles.create(c);
	m_selection.clear();
	m_selection.append(&m_tmpStyles[m_tmpStyles.find(newName)]);
	for (int j = 0; j < m_tmpStyles.count(); ++j)
	{
		int index = m_tmpStyles.find(oldName);
		if (index > -1)
		{
			m_tmpStyles.remove(index);
			break;
		}
	}

	for (int j = 0; j < m_tmpStyles.count(); ++j)
	{
		if (m_tmpStyles[j].parent() == oldName)
			m_tmpStyles[j].setParent(newName);
	}

	QList<RemoveItem>::iterator it;
	for (it = m_deleted.begin(); it != m_deleted.end(); ++it)
	{
		if (it->second == oldName)
		{
			oldName = (*it).first;
			m_deleted.erase(it);
			break;
		}
	}

	if (oldName != newName)
		m_deleted.append(RemoveItem(oldName, newName));

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::languageChange()
{
	if (m_widget && m_page)
	{
		m_widget->setTabText(m_widget->indexOf(m_page), tr("Properties"));
		m_page->languageChange();
	}
}

void SMCharacterStyle::unitChange()
{

}

void SMCharacterStyle::reloadTmpStyles()
{
	if (!m_doc)
		return;

	m_selection.clear();
	m_tmpStyles.clear();
	m_tmpStyles.redefine(m_doc->charStyles(), true);
}

void SMCharacterStyle::setupConnections()
{
	if (!m_page)
		return;

	connect(m_page->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(m_page->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	connect(m_page->effects_->ShadowVal->Xoffset, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	connect(m_page->effects_->ShadowVal->Yoffset, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	connect(m_page->effects_->OutlineVal->LWidth, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	connect(m_page->effects_->UnderlineVal->LPos, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	connect(m_page->effects_->UnderlineVal->LWidth, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	connect(m_page->effects_->StrikeVal->LPos, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	connect(m_page->effects_->StrikeVal->LWidth, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	connect(m_page->fillColor_, SIGNAL(activated(const QString&)), this, SLOT(slotFillColor()));
	connect(m_page->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	connect(m_page->strokeColor_, SIGNAL(activated(const QString&)), this, SLOT(slotStrokeColor()));
	connect(m_page->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	connect(m_page->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	connect(m_page->fontSize_, SIGNAL(valueChanged(double)), this, SLOT(slotFontSize()));
	connect(m_page->fontHScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleH()));
	connect(m_page->fontVScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleV()));
	connect(m_page->tracking_, SIGNAL(valueChanged(double)), this, SLOT(slotTracking()));
	connect(m_page->widthSpaceSpin, SIGNAL(valueChanged(double)), this, SLOT(slotWordTracking()));
	connect(m_page->baselineOffset_, SIGNAL(valueChanged(double)), this, SLOT(slotBaselineOffset()));
	connect(m_page->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	connect(m_page->parentCombo, SIGNAL(activated(const QString&)),
	        this, SLOT(slotParentChanged(const QString&)));
}

void SMCharacterStyle::removeConnections()
{
	if (!m_page)
		return;

	disconnect(m_page->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(m_page->effects_, SIGNAL(State(int)), this, SLOT(slotEffects(int)));
	disconnect(m_page->effects_->ShadowVal->Xoffset, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	disconnect(m_page->effects_->ShadowVal->Yoffset, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	disconnect(m_page->effects_->OutlineVal->LWidth, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	disconnect(m_page->effects_->UnderlineVal->LPos, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	disconnect(m_page->effects_->UnderlineVal->LWidth, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	disconnect(m_page->effects_->StrikeVal->LPos, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	disconnect(m_page->effects_->StrikeVal->LWidth, SIGNAL(valueChanged(double)),
			   this, SLOT(slotEffectProperties()));
	disconnect(m_page->fillColor_, SIGNAL(activated(const QString&)), this, SLOT(slotFillColor()));
	disconnect(m_page->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	disconnect(m_page->strokeColor_, SIGNAL(activated(const QString&)), this, SLOT(slotStrokeColor()));
	disconnect(m_page->strokeShade_, SIGNAL(clicked()), this, SLOT(slotStrokeShade()));
	disconnect(m_page->language_, SIGNAL(activated(int)), this, SLOT(slotLanguage()));
	disconnect(m_page->fontSize_, SIGNAL(valueChanged(double)), this, SLOT(slotFontSize()));
	disconnect(m_page->fontHScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleH()));
	disconnect(m_page->fontVScale_, SIGNAL(valueChanged(double)), this, SLOT(slotScaleV()));
	disconnect(m_page->tracking_, SIGNAL(valueChanged(double)), this, SLOT(slotTracking()));
	disconnect(m_page->widthSpaceSpin, SIGNAL(valueChanged(double)), this, SLOT(slotWordTracking()));
	disconnect(m_page->baselineOffset_, SIGNAL(valueChanged(double)), this, SLOT(slotBaselineOffset()));
	disconnect(m_page->fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotFont(QString)));
	disconnect(m_page->parentCombo, SIGNAL(activated(const QString&)),
			this, SLOT(slotParentChanged(const QString&)));
}

void SMCharacterStyle::slotFontSize()
{
	if (m_page->fontSize_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetFontSize();
	else
	{
		double a, b, value;
		int c;

		m_page->fontSize_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setFontSize(qRound(value));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotEffects(int e)
{
	StyleFlag s = ScStyle_None;
	if (m_page->effects_->useParentValue())
	{
		for (int i = 0; i < m_selection.count(); ++i)
		{
			m_selection[i]->resetFeatures();
			m_selection[i]->resetShadowXOffset();
			m_selection[i]->resetShadowYOffset();
			m_selection[i]->resetOutlineWidth();
			m_selection[i]->resetUnderlineOffset();
			m_selection[i]->resetUnderlineWidth();
			m_selection[i]->resetStrikethruOffset();
			m_selection[i]->resetStrikethruWidth();
		}
	}
	else
	{
		s = static_cast<StyleFlag>(e);
		double a, b, sxo, syo, olw, ulp, ulw, slp, slw;
		int c;

		m_page->effects_->ShadowVal->Xoffset->getValues(&a, &b, &c, &sxo);
		sxo *= 10;
		m_page->effects_->ShadowVal->Yoffset->getValues(&a, &b, &c, &syo);
		syo *= 10;

		m_page->effects_->OutlineVal->LWidth->getValues(&a, &b, &c, &olw);
		olw *= 10;

		m_page->effects_->UnderlineVal->LPos->getValues(&a, &b, &c, &ulp);
		ulp *= 10;
		m_page->effects_->UnderlineVal->LWidth->getValues(&a, &b, &c, &ulw);
		ulw *= 10;

		m_page->effects_->StrikeVal->LPos->getValues(&a, &b, &c, &slp);
		slp *= 10;
		m_page->effects_->StrikeVal->LWidth->getValues(&a, &b, &c, &slw);
		slw *= 10;

		for (int i = 0; i < m_selection.count(); ++i)
		{
			QStringList feList = s.featureList();
			feList.removeAll(CharStyle::INHERIT);
			m_selection[i]->setFeatures(feList);
//			m_selection[i]->setFeatures(s.featureList());
			m_selection[i]->setShadowXOffset(qRound(sxo));
			m_selection[i]->setShadowYOffset(qRound(syo));
			m_selection[i]->setOutlineWidth(qRound(olw));
			m_selection[i]->setUnderlineOffset(qRound(ulp));
			m_selection[i]->setUnderlineWidth(qRound(ulw));
			m_selection[i]->setStrikethruOffset(qRound(slp));
			m_selection[i]->setStrikethruWidth(qRound(slw));
		}
	}


	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotEffectProperties()
{
	double a, b, sxo, syo, olw, ulp, ulw, slp, slw;
	int c;

	m_page->effects_->ShadowVal->Xoffset->getValues(&a, &b, &c, &sxo);
	sxo *= 10;
	m_page->effects_->ShadowVal->Yoffset->getValues(&a, &b, &c, &syo);
	syo *= 10;

	m_page->effects_->OutlineVal->LWidth->getValues(&a, &b, &c, &olw);
	olw *= 10;

	m_page->effects_->UnderlineVal->LPos->getValues(&a, &b, &c, &ulp);
	ulp *= 10;
	m_page->effects_->UnderlineVal->LWidth->getValues(&a, &b, &c, &ulw);
	ulw *= 10;

	m_page->effects_->StrikeVal->LPos->getValues(&a, &b, &c, &slp);
	slp *= 10;
	m_page->effects_->StrikeVal->LWidth->getValues(&a, &b, &c, &slw);
	slw *= 10;
	
	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->setShadowXOffset(qRound(sxo));
		m_selection[i]->setShadowYOffset(qRound(syo));
		m_selection[i]->setOutlineWidth(qRound(olw));
		m_selection[i]->setUnderlineOffset(qRound(ulp));
		m_selection[i]->setUnderlineWidth(qRound(ulw));
		m_selection[i]->setStrikethruOffset(qRound(slp));
		m_selection[i]->setStrikethruWidth(qRound(slw));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotFillColor()
{
	if (m_page->fillColor_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetFillColor();
	else {		
		QString col(m_page->fillColor_->currentText());
		if (col == CommonStrings::tr_NoneColor)
			col = CommonStrings::None;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setFillColor(col);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotFillShade()
{
	if (m_page->fillShade_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetFillShade();
	else {
		int fs = m_page->fillShade_->getValue();

		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setFillShade(fs);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotStrokeColor()
{
	if (m_page->strokeColor_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetStrokeColor();
	else {
		QString col(m_page->strokeColor_->currentText());
		if (col == CommonStrings::tr_NoneColor)
			col = CommonStrings::None;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setStrokeColor(col);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotStrokeShade()
{
	if (m_page->strokeShade_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetStrokeShade();
	else {
		int ss = m_page->strokeShade_->getValue();
		
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setStrokeShade(ss);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotLanguage()
{
	QMap<QString,QString>::Iterator it;
	QString language = m_doc->paragraphStyle("").charStyle().language();

	if (m_page->language_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetLanguage();
	else
	{
		QString tl(LanguageManager::instance()->getAbbrevFromLang(m_page->language_->currentText(), true));
		if (!tl.isEmpty())
			language=tl;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setLanguage(language);
	}


	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotScaleH()
{
	if (m_page->fontHScale_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetScaleH();
	else
	{
		double a, b, value;
		int c;

		m_page->fontHScale_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setScaleH(qRound(value));
	}


	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotScaleV()
{
	if (m_page->fontVScale_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetScaleV();
	else
	{
		double a, b, value;
		int c;

		m_page->fontVScale_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setScaleV(qRound(value));
	}


	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotTracking()
{
	if (m_page->tracking_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetTracking();
	else
	{
		double a, b, value;
		int c;

		m_page->tracking_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setTracking(qRound(value));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotWordTracking()
{
	if (m_page->widthSpaceSpin->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetWordTracking();
	else
	{
		double a, b, value;
		int c;

		m_page->widthSpaceSpin->getValues(&a, &b, &c, &value);
		value = value / 100.0;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setWordTracking(value);
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotBaselineOffset()
{
	if (m_page->baselineOffset_->useParentValue())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetBaselineOffset();
	else
	{
		double a, b, value;
		int c;
		
		m_page->baselineOffset_->getValues(&a, &b, &c, &value);
		value = value * 10;
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setBaselineOffset(qRound(value));
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotFont(QString s)
{
	ScFace sf;
	if (m_page->fontFace_->useParentFont())
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetFont();
	else {
		sf = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[s];

		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setFont(sf);
	}
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCharacterStyle::slotParentChanged(const QString &parent)
{
	Q_ASSERT(!parent.isNull());

	bool  loop = false, parentLoop = false;
	const Style* parentStyle = (!parent.isEmpty()) ? m_tmpStyles.resolve(parent) : NULL;
	QStringList  sel;

	for (int i = 0; i < m_selection.count(); ++i)
	{
		loop = false;
		// Check if setting parent won't create a loop
		const Style* pStyle = parentStyle;
		while (pStyle)
		{
			if (pStyle->hasParent() && (pStyle->parent() == m_selection[i]->name()))
			{
				loop = parentLoop = true;
				break;
			}
			pStyle = pStyle->hasParent() ? pStyle->parentStyle() : NULL;
		}
		if (!loop)
		{
			m_selection[i]->erase();
			m_selection[i]->setParent(parent);
		}
		sel << m_selection[i]->name();
	}

	if (parentLoop)
		QMessageBox::warning(this->widget(), CommonStrings::trWarning, tr("Setting that style as parent would create an infinite loop."), CommonStrings::tr_OK);

	selected(sel);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

SMCharacterStyle::~SMCharacterStyle()
{
	delete m_page;
	delete m_widget;
	m_page = 0;
	m_widget = 0;
}

