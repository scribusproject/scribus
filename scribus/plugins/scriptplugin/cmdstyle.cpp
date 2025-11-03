/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
02.01.2008: Joachim Neu - joachim_neu@web.de - http://www.joachim-neu.de
*/
#include "cmdstyle.h"
#include "cmdutil.h"

#include <limits>

#include <QBuffer>
#include <QList>
#include <QPixmap>

#include "pyesstring.h"
#include "scribuscore.h"
#include "styles/paragraphstyle.h"
#include "styles/charstyle.h"
#include "ui/stylemanager.h"

/*! 02.01.2007 - 05.01.2007 : Joachim Neu : Create a paragraph style.
			Special thanks go to avox for helping me! */
PyObject *scribus_createparagraphstyle(PyObject* /* self */, PyObject* args, PyObject* keywords)
{
	//TODO - new paragraph properties for bullets and numbering
	char* keywordargs[] = {
			const_cast<char*>("name"),
			const_cast<char*>("linespacingmode"),
			const_cast<char*>("linespacing"),
			const_cast<char*>("alignment"),
			const_cast<char*>("leftmargin"),
			const_cast<char*>("rightmargin"),
			const_cast<char*>("gapbefore"),
			const_cast<char*>("gapafter"),
			const_cast<char*>("firstindent"),
			const_cast<char*>("hasdropcap"),
			const_cast<char*>("dropcaplines"),
			const_cast<char*>("dropcapoffset"),
			const_cast<char*>("charstyle"),
			const_cast<char*>("bullet"),
			const_cast<char*>("tabs"),
			const_cast<char*>("unit"),
			nullptr};
	PyESString name;
	PyESString charStyle;
	PyESString bullet;
	int lineSpacingMode = 0, alignment = 0, dropCapLines = 2, hasDropCap = 0, unit = SC_PT;
	double lineSpacing = 15.0, leftMargin = 0.0, rightMargin = 0.0;
	double gapBefore = 0.0, gapAfter = 0.0, firstIndent = 0.0, peOffset = 0;
	PyObject *tabDefinitions = nullptr;
	if (!PyArg_ParseTupleAndKeywords(args, keywords, "es|ididddddiidesesOi",
		 keywordargs, "utf-8", name.ptr(), &lineSpacingMode, &lineSpacing, &alignment,
		&leftMargin, &rightMargin, &gapBefore, &gapAfter, &firstIndent,
		&hasDropCap, &dropCapLines, &peOffset, "utf-8", charStyle.ptr(),
		"utf-8", bullet.ptr(), &tabDefinitions, &unit))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty paragraph style name.","python error").toUtf8().constData());
		return nullptr;
	}
	
	if ((hasDropCap != 0) && (dropCapLines <= 1) )
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("hasdropcap is true but dropcaplines value is invalid","python error").toUtf8().constData());
		return nullptr;
	}
	
	if (bullet.length() > 0 && (hasDropCap  != 0))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("hasdropcap and bullet are not allowed to be specified together.","python error").toUtf8().constData());
		return nullptr;
	}

	ParagraphStyle tmpParagraphStyle;
	tmpParagraphStyle.setName(name.c_str());
	tmpParagraphStyle.setLineSpacingMode((ParagraphStyle::LineSpacingMode) lineSpacingMode);
	tmpParagraphStyle.setLineSpacing(lineSpacing);
	tmpParagraphStyle.setAlignment((ParagraphStyle::AlignmentType) alignment);
	tmpParagraphStyle.setLeftMargin(leftMargin);
	tmpParagraphStyle.setFirstIndent(firstIndent);
	tmpParagraphStyle.setRightMargin(rightMargin);
	tmpParagraphStyle.setGapBefore(gapBefore);
	tmpParagraphStyle.setGapAfter(gapAfter);
	
	if (hasDropCap != 0)
	{
		tmpParagraphStyle.setDropCapLines(dropCapLines);
		tmpParagraphStyle.setHasDropCap(true);
		tmpParagraphStyle.setHasBullet(false);
		tmpParagraphStyle.setHasNum(false);
	}
	else
	{
		tmpParagraphStyle.setHasDropCap(false);
	}
	
	if (bullet.length() > 0)
	{
		tmpParagraphStyle.setBulletStr(QString::fromUtf8(bullet.c_str()));
		tmpParagraphStyle.setHasDropCap(false);
		tmpParagraphStyle.setHasBullet(true);
		tmpParagraphStyle.setHasNum(false);
	}
	else
	{
		tmpParagraphStyle.setHasBullet(false);
	}

	tmpParagraphStyle.setParEffectOffset(peOffset);
	tmpParagraphStyle.charStyle().setParent(charStyle.c_str());

	if (tabDefinitions != nullptr)
	{
		Py_ssize_t n = PyList_Size(tabDefinitions);
		for (Py_ssize_t i = 0; i < n; i++)
		{
			PyObject* tabDefinition = PyList_GetItem(tabDefinitions, i);
			Py_ssize_t size = PyTuple_Check(tabDefinition) ? PyTuple_Size(tabDefinition) : 1;
			PyObject* tabPositionDefinition = PyTuple_Check(tabDefinition) ? PyTuple_GetItem(tabDefinition, 0) : tabDefinition;

			float tabPosition = 0.0;
			if (!PyArg_Parse(tabPositionDefinition, "f", &tabPosition))
			{
				PyErr_SetString(PyExc_TypeError, QObject::tr("invalid tab-position specified.","python error").toUtf8().constData());
				return nullptr;
			}
			tabPosition = value2pts(tabPosition, unit);

			int tabType = 0;
			if (size >= 2) {
				PyArg_Parse(PyTuple_GetItem(tabDefinition, 1), "i", &tabType);
			}

			char *fillChar = const_cast<char*>("");
			if (size == 3) {
				PyArg_Parse(PyTuple_GetItem(tabDefinition, 2), "es", "utf-8", &fillChar);
			}

			QString fillCharString = QString::fromUtf8(fillChar).trimmed();

			ParagraphStyle::TabRecord tr;
			tr.tabFillChar = (fillCharString.size() > 0) ? fillCharString.at(0) : QChar();
			tr.tabPosition = static_cast<qreal>(tabPosition);
			tr.tabType = tabType;
			tmpParagraphStyle.appendTabValue(tr);
		}
	}

	StyleSet<ParagraphStyle> tmpStyleSet;
	tmpStyleSet.create(tmpParagraphStyle);
	ScCore->primaryMainWindow()->doc->redefineStyles(tmpStyleSet, false);
	// PV - refresh the Style Manager window.
	// I thought that this can work but it doesn't:
	// ScCore->primaryMainWindow()->styleMgr()->reloadStyleView();
	// So the brute force setDoc is called...
	ScCore->primaryMainWindow()->styleMgr()->setDoc(ScCore->primaryMainWindow()->doc);

	Py_RETURN_NONE;
}

/*! 03.01.2007 - 05.01.2007 : Joachim Neu : Create a char style.
			Special thanks go to avox for helping me! */
PyObject *scribus_createcharstyle(PyObject* /* self */, PyObject* args, PyObject* keywords)
{
	char* keywordargs[] = {
		const_cast<char*>("name"),
		const_cast<char*>("font"),
		const_cast<char*>("fontsize"),
		const_cast<char*>("features"),
		const_cast<char*>("fillcolor"),
		const_cast<char*>("fillshade"),
		const_cast<char*>("strokecolor"),
		const_cast<char*>("strokeshade"),
		const_cast<char*>("bgcolor"),
		const_cast<char*>("baselineoffset"),
		const_cast<char*>("shadowxoffset"),
		const_cast<char*>("shadowyoffset"),
		const_cast<char*>("outlinewidth"),
		const_cast<char*>("underlineoffset"),
		const_cast<char*>("underlinewidth"),
		const_cast<char*>("strikethruoffset"),
		const_cast<char*>("strikethruwidth"),
		const_cast<char*>("scaleh"),
		const_cast<char*>("scalev"),
		const_cast<char*>("tracking"),
		const_cast<char*>("language"),
		const_cast<char*>("fontfeatures"),
		nullptr};

	if (!checkHaveDocument())
		return nullptr;

	constexpr double dbl_min = -std::numeric_limits<double>::max();

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const StyleSet<CharStyle>& charStyles = ScCore->primaryMainWindow()->doc->charStyles();
	const CharStyle* defaultStyle = charStyles.getDefault();

	PyESString name;
	PyESString font;
	PyESString features;
	PyESString fillColor;
	PyESString bgColor;
	PyESString fontFeatures;
	PyESString strokeColor;
	PyESString language;
	double fontSize = -1;
	double fillShade = -1, strokeShade = -1;
	double scaleH = -1, scaleV = -1;
	double baselineOffset = dbl_min;
	double shadowXOffset  = dbl_min, shadowYOffset = dbl_min;
	double outlineWidth = dbl_min, underlineOffset = dbl_min, underlineWidth = dbl_min;
	double strikethruOffset = dbl_min, strikethruWidth = dbl_min;
	double tracking = dbl_min;
	
	if (!PyArg_ParseTupleAndKeywords(args, keywords, "es|esdesesdesdsdddddddddddeses", keywordargs,
									"utf-8", name.ptr(), "utf-8", font.ptr(), &fontSize, "utf-8", features.ptr(),
									"utf-8", fillColor.ptr(), &fillShade, "utf-8", strokeColor.ptr(), &strokeShade, bgColor.ptr(), &baselineOffset, &shadowXOffset,
									&shadowYOffset, &outlineWidth, &underlineOffset, &underlineWidth, &strikethruOffset, &strikethruWidth,
									&scaleH, &scaleV, &tracking, "utf-8", language.ptr(), "utf-8", fontFeatures.ptr()))
		return nullptr;
	
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty char style name.","python error").toUtf8().constData());
		return nullptr;
	}

	QString realFont(font.c_str());
	if (!realFont.isEmpty())
	{
		if (!currentDoc->AllFonts->contains(realFont))
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("Specified font is not available.", "python error").toUtf8().constData());
			return nullptr;
		}
	}

	const ColorList& docColors = currentDoc->PageColors;
	QString qFillColor(fillColor.c_str());
	QString qStrokeColor(strokeColor.defaulted("Black"));
	QString qBgColor(bgColor.c_str());
	if (!qFillColor.isEmpty())
	{
		if ((qFillColor != CommonStrings::None) && (!docColors.contains(qFillColor)))
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("Specified fill color is not available in document.", "python error").toUtf8().constData());
			return nullptr;
		}
	}
	if (!qStrokeColor.isEmpty())
	{
		if ((qStrokeColor != CommonStrings::None) && (!docColors.contains(qStrokeColor)))
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("Specified stroke color is not available in document.", "python error").toUtf8().constData());
			return nullptr;
		}
	}
	if (!qBgColor.isEmpty())
	{
		if ((qBgColor != CommonStrings::None) && (!docColors.contains(qBgColor)))
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("Specified background color is not available in document.", "python error").toUtf8().constData());
			return nullptr;
		}
	}

	if (fillShade >= 0)
		fillShade = qMax(0.0, qMin(fillShade, 1.0));
	if (strokeShade >= 0)
		strokeShade = qMax(0.0, qMin(strokeShade, 1.0));
	QStringList featuresList = QString(features.defaulted("inherit")).split(',', Qt::SkipEmptyParts);
	QString qFontFeatures = QString::fromUtf8(fontFeatures.c_str());
	QString qLanguage(language.c_str());

	CharStyle tmpCharStyle;
	tmpCharStyle.setName(name.c_str());
	if (!realFont.isEmpty())
		tmpCharStyle.setFont((*currentDoc->AllFonts)[realFont]);
	if (fontSize > 0)
		tmpCharStyle.setFontSize(fontSize * 10);
	if (qFontFeatures != defaultStyle->fontFeatures())
		tmpCharStyle.setFontFeatures(qFontFeatures);
	if (featuresList != defaultStyle->features())
		tmpCharStyle.setFeatures(featuresList);
	if (!qFillColor.isEmpty())
		tmpCharStyle.setFillColor(qFillColor);
	if (!qBgColor.isEmpty())
		tmpCharStyle.setBackColor(qBgColor);
	if (fillShade >= 0)
		tmpCharStyle.setFillShade(fillShade * 100);
	if (!qStrokeColor.isEmpty())
		tmpCharStyle.setStrokeColor(qStrokeColor);
	if (strokeShade >= 0)
		tmpCharStyle.setStrokeShade(strokeShade * 100);
	if (baselineOffset >= 0)
		tmpCharStyle.setBaselineOffset(baselineOffset);
	if (shadowXOffset != dbl_min)
		tmpCharStyle.setShadowXOffset(shadowXOffset);
	if (shadowYOffset != dbl_min)
		tmpCharStyle.setShadowYOffset(shadowYOffset);
	if (outlineWidth >= 0)
		tmpCharStyle.setOutlineWidth(outlineWidth);
	if (underlineOffset >= 0)
		tmpCharStyle.setUnderlineOffset(underlineOffset);
	if (underlineWidth >= 0)
		tmpCharStyle.setUnderlineWidth(underlineWidth);
	if (strikethruOffset >= 0)
		tmpCharStyle.setStrikethruOffset(strikethruOffset);
	if (strikethruWidth >= 0)
		tmpCharStyle.setStrikethruWidth(strikethruWidth);
	if (scaleH > 0)
		tmpCharStyle.setScaleH(scaleH * 1000);
	if (scaleV > 0)
		tmpCharStyle.setScaleV(scaleV * 1000);
	if (tracking != dbl_min)
		tmpCharStyle.setTracking(tracking);
	if (!qLanguage.isEmpty())
		tmpCharStyle.setLanguage(qLanguage);

	StyleSet<CharStyle> tmpStyleSet;
	tmpStyleSet.create(tmpCharStyle);
	currentDoc->redefineCharStyles(tmpStyleSet, false);
	// PV - refresh the Style Manager window.
	// I thought that this can work but it doesn't:
	// ScCore->primaryMainWindow()->styleMgr()->reloadStyleView();
	// So the brute force setDoc is called...
	ScCore->primaryMainWindow()->styleMgr()->setDoc(ScCore->primaryMainWindow()->doc);

	Py_RETURN_NONE;
}

PyObject *scribus_createcustomlinestyle(PyObject * /* self */, PyObject* args)
{
	PyESString name;
	PyObject *obj;

	if (!PyArg_ParseTuple(args, "esO", "utf-8", name.ptr(), &obj))
		return nullptr;

	if (!PyList_Check(obj)) {
		PyErr_SetString(PyExc_TypeError, "'style' must be list.");
		return nullptr;
	}

	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	MultiLine ml;
	const ColorList& docColors = currentDoc->PageColors;

	for (Py_ssize_t i = 0; i < PyList_Size(obj); i++)
	{
		PyObject *line = PyList_GetItem(obj, i);
		if (!PyDict_Check(line))
		{
			PyErr_SetString(PyExc_TypeError, "elements of list must be Dictionary.");
			return nullptr;
		}
		struct SingleLine sl;
		PyObject *val;

		val = PyDict_GetItemString(line, "Color");
		if (val)
			sl.Color = PyUnicode_asQString(val);
		else 
			sl.Color = currentDoc->itemToolPrefs().lineColor;

		val = PyDict_GetItemString(line, "Dash");
		if (val)
			sl.Dash = PyLong_AsLong(val);
		else 
			sl.Dash = Qt::SolidLine;

		val = PyDict_GetItemString(line, "LineEnd");
		if (val)
			sl.LineEnd = PyLong_AsLong(val);
		else 
			sl.LineEnd = Qt::FlatCap;

		val = PyDict_GetItemString(line, "LineJoin");
		if (val)
			sl.LineJoin = PyLong_AsLong(val);
		else 
			sl.LineJoin = Qt::MiterJoin;

		val = PyDict_GetItemString(line, "Shade");
		if (val)
			sl.Shade = PyLong_AsLong(val);
		else 
			sl.Shade = currentDoc->itemToolPrefs().lineColorShade;

		val = PyDict_GetItemString(line, "Width");
		if (val)
			sl.Width = PyFloat_AsDouble(val);
		else 
			sl.Width = currentDoc->itemToolPrefs().lineWidth;

		val = PyDict_GetItemString(line, "Shortcut");
		if (val)
			ml.shortcut = PyUnicode_asQString(val);
		else 
			ml.shortcut = "";

		if (!docColors.contains(sl.Color))
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("Specified color is not available in document.", "python error").toUtf8().constData());
			return nullptr;
		}
		ml.push_back(sl);
	}
	if (!ml.empty())
		currentDoc->docLineStyles[name.c_str()] = ml;
	Py_RETURN_NONE;
}

/*
 * Craig Ringer, 2004-09-09
 * Enumerate all known paragraph styles
 */
PyObject *scribus_getparagraphstyles(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const auto& paragraphStyles = ScCore->primaryMainWindow()->doc->paragraphStyles();

	PyObject *styleList = PyList_New(0);
	for (int i = 0; i < paragraphStyles.count(); ++i)
	{
		const QString& paraStyleName = paragraphStyles[i].name();
		if (PyList_Append(styleList, PyUnicode_FromString(paraStyleName.toUtf8())))
		{
			// An exception will have already been set by PyList_Append apparently.
			return nullptr;
		}
	}
	return styleList;
}

/*
 * Enumerate all known character styles
 */
PyObject *scribus_getcharstyles(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const auto& charStyles = ScCore->primaryMainWindow()->doc->charStyles();

	PyObject *charStyleList = PyList_New(0);
	for (int i = 0; i < charStyles.count(); ++i)
	{
		const QString& charStyleName = charStyles[i].name();
		if (PyList_Append(charStyleList, PyUnicode_FromString(charStyleName.toUtf8())))
		{
			// An exception will have already been set by PyList_Append apparently.
			return nullptr;
		}
	}
	return charStyleList;
}

/*
 * Enumerate all known line styles
 */
PyObject *scribus_getlinestyles(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const auto& lineStyles = ScCore->primaryMainWindow()->doc->lineStyles();

	PyObject *lineStyleList = PyList_New(0);
	auto itEnd = lineStyles.constEnd();
	for (auto it = lineStyles.constBegin(); it != itEnd; ++it)
	{
		const QString& lineStyleName = it.key();
		if (PyList_Append(lineStyleList, PyUnicode_FromString(lineStyleName.toUtf8())))
		{
			// An exception will have already been set by PyList_Append apparently.
			return nullptr;
		}
	}
	return lineStyleList;
}

/*
 * Enumerate all known cell styles
 */
PyObject *scribus_getcellstyles(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const auto& cellStyles = ScCore->primaryMainWindow()->doc->cellStyles();

	PyObject *styleList = PyList_New(0);
	for (int i = 0; i < cellStyles.count(); ++i)
	{
		const QString& cellStyleName = cellStyles[i].name();
		if (PyList_Append(styleList, PyUnicode_FromString(cellStyleName.toUtf8())))
		{
			// An exception will have already been set by PyList_Append apparently.
			return nullptr;
		}
	}
	return styleList;
}

/*
 * Enumerate all known table styles
 */
PyObject *scribus_gettablestyles(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const auto& tableStyles = ScCore->primaryMainWindow()->doc->tableStyles();

	PyObject *styleList = PyList_New(0);
	for (int i = 0; i < tableStyles.count(); ++i)
	{
		const QString& tableStyleName = tableStyles[i].name();
		if (PyList_Append(styleList, PyUnicode_FromString(tableStyleName.toUtf8())))
		{
			// An exception will have already been set by PyList_Append apparently.
			return nullptr;
		}
	}
	return styleList;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdstyledocwarnings()
{
	QStringList s;
	s  << scribus_createcharstyle__doc__
	   << scribus_createcustomlinestyle__doc__
	   << scribus_createparagraphstyle__doc__
	   << scribus_getallstyles__doc__
	   << scribus_getcellstyles__doc__
	   << scribus_getcharstyles__doc__
	   << scribus_getlinestyles__doc__
	   << scribus_getparagraphstyles__doc__
	   << scribus_gettablestyles__doc__;
}
