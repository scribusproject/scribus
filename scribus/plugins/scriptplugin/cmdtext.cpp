/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdtext.h"

#include "appmodes.h"
#include "cmdutil.h"
#include "hyphenator.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "util.h"


template<typename T>
class ApplyCharstyleHelper
{
	PageItem* m_item;
	T m_value;

public:
	ApplyCharstyleHelper(PageItem* item, T v) : m_item(item), m_value(v) {}

	void apply(void (CharStyle::*f)(T), int p, int len)
	{
		CharStyle cs;
		(cs.*f)(m_value);
		if (m_item->HasSel)
		{
			int max = qMax(p + len, m_item->itemText.length());
			for (int i = p; i < max; i++)
			{
				if (m_item->itemText.selected(i))
					m_item->itemText.applyCharStyle(i, 1, cs);
			}
		}
		else
		{
			m_item->itemText.applyCharStyle(p, len, cs);
		}
	}

};

PyObject *scribus_getfontsize(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font size of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->HasSel)
	{
		for (int i = 0; i < item->itemText.length(); i++)
			if (item->itemText.selected(i))
				return PyFloat_FromDouble(static_cast<double>(item->itemText.charStyle(i).fontSize() / 10.0));
		return nullptr;
	}
	return PyFloat_FromDouble(static_cast<double>(item->currentCharStyle().fontSize() / 10.0));
}

PyObject *scribus_getfont(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->HasSel)
	{
		for (int i = 0; i < item->itemText.length(); i++)
			if (item->itemText.selected(i))
				return PyUnicode_FromString(item->itemText.charStyle(i).font().scName().toUtf8());
		return nullptr;
	}
	return PyUnicode_FromString(item->currentCharStyle().font().scName().toUtf8());
}

PyObject *scribus_gettextcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
    if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text color of non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->HasSel)
	{
		for (int i = 0; i < item->itemText.length(); ++i)
		{
			if (item->itemText.selected(i))
				return PyUnicode_FromString(item->itemText.charStyle(i).fillColor().toUtf8());
		}
        return nullptr;
	}
    return PyUnicode_FromString(item->currentCharStyle().fillColor().toUtf8());
}

PyObject *scribus_gettextshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text color of non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->HasSel)
	{
		for (int i = 0; i < item->itemText.length(); ++i)
		{
			if (item->itemText.selected(i))
				return PyLong_FromLong(item->itemText.charStyle(i).fillShade());
		}
		return nullptr;
	}
	return PyLong_FromLong(item->currentCharStyle().fillShade());
}

PyObject *scribus_gettextlength(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text size of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(item->itemText.length()));
}

PyObject *scribus_gettextlines(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get number of lines of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(item->textLayout.lines()));
}

PyObject *scribus_gettextverticalalignment(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get vertical alignment of non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(item->verticalAlignment()));
}

PyObject *scribus_getcolumns(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column count of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(item->m_columns));
}

PyObject *scribus_getcolumngap(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column gap of non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyFloat_FromDouble(PointToValue(static_cast<double>(item->m_columnGap)));
}

PyObject *scribus_getfontfeatures(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get fontfeatures of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->HasSel)
	{
		for (int i = 0; i < item->itemText.length(); i++)
			if (item->itemText.selected(i))
				return PyUnicode_FromString(item->itemText.charStyle(i).fontFeatures().toUtf8());
		return nullptr;
	}
	return PyUnicode_FromString(item->currentCharStyle().fontFeatures().toUtf8());
}

PyObject *scribus_getfirstlinkedframe(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get linked frames of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	return PyUnicode_FromString(item->firstInChain()->itemName().toUtf8());
}

PyObject *scribus_getlastlinkedframe(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get linked frames of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	return PyUnicode_FromString(item->lastInChain()->itemName().toUtf8());
}

PyObject *scribus_getnextlinkedframe(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get linked frames of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	if (item->nextInChain() != nullptr)
		return PyUnicode_FromString(item->nextInChain()->itemName().toUtf8());

	Py_RETURN_NONE;
}

PyObject *scribus_getprevlinkedframe(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get linked frames of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	if (item->prevInChain() != nullptr)
		return PyUnicode_FromString(item->prevInChain()->itemName().toUtf8());

	Py_RETURN_NONE;
}

PyObject *scribus_getfirstlineoffset(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get first line offset of non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(item->firstLineOffset()));
}

PyObject *scribus_getlinespacing(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get line space of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyFloat_FromDouble(static_cast<double>(item->currentStyle().lineSpacing()));
}

PyObject *scribus_getlinespacingmode(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get line spacing mode of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyFloat_FromDouble(static_cast<double>(item->currentStyle().lineSpacingMode()));
}

PyObject *scribus_gettextdistances(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text distances of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return Py_BuildValue("(dddd)",
	        PointToValue(item->textToFrameDistLeft()),
	        PointToValue(item->textToFrameDistRight()),
	        PointToValue(item->textToFrameDistTop()),
	        PointToValue(item->textToFrameDistBottom()));
}

PyObject *scribus_getframetext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	
	const StoryText& story = item->itemText;
	QString text;
	text.reserve(story.hasSelection() ? story.selectionLength() : story.length());
	for (int i = item->firstInFrame(); i <= item->lastInFrame(); ++i)
	{
		if (item->HasSel)
		{
			if (story.selected(i))
				text += story.text(i);
		}
		else
		{
			text += story.text(i);
		}
	}
	return PyUnicode_FromString(text.toUtf8());
}

PyObject *scribus_getalltext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	// collect all chars from a storytext
	const StoryText& story = item->itemText;
	QString text;
	text.reserve(story.hasSelection() ? story.selectionLength() : story.length());
	for (int i = 0; i < story.length(); i++)
	{
		if (item->HasSel)
		{
			if (story.selected(i))
				text += story.text(i);
		}
		else
		{
			text += story.text(i);
		}
	}
	return PyUnicode_FromString(text.toUtf8());
}

PyObject *scribus_settext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Text;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Text, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *currItem = GetUniqueItem(QString::fromUtf8(Name));
	if (currItem == nullptr)
		return nullptr;
	if (!(currItem->isTextFrame()) && !(currItem->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text of non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString userText = QString::fromUtf8(Text);
	userText.replace("\r\n", SpecialChars::PARSEP);
	userText.replace(QChar('\n') , SpecialChars::PARSEP);
	PyMem_Free(Text);
	currItem->itemText.clear();
	currItem->itemText.insertChars(0, userText);
	currItem->invalidateLayout();

	Py_RETURN_NONE;
}

PyObject *scribus_inserttext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Text;
	int pos;
	if (!PyArg_ParseTuple(args, "esi|es", "utf-8", &Text, &pos, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot insert text into non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString textData = QString::fromUtf8(Text);
	textData.replace("\r\n", SpecialChars::PARSEP);
	textData.replace(QChar('\n') , SpecialChars::PARSEP);
	PyMem_Free(Text);
	if ((pos < -1) || (pos > static_cast<int>(item->itemText.length())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Insert index out of bounds.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (pos == -1)
		pos = item->itemText.length();
	item->itemText.insertChars(pos, textData, true);
	item->invalidateLayout();

	Py_RETURN_NONE;
}

PyObject *scribus_inserthtmltext(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	char *file;

	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &file, "utf-8", &name)) {
		return nullptr;
	}

	if (!checkHaveDocument()) {
		return nullptr;
	}

	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr) {
		return nullptr;
	}

	if (!(item->isTextFrame()) && !(item->isPathText())) {
		PyErr_SetString(WrongFrameTypeError,
				QObject::tr("Cannot insert text into non-text frame.",
					"python error").toLocal8Bit().constData());
		return nullptr;
	}

	QString fileName = QString::fromUtf8(file);

	gtGetText gt(ScCore->primaryMainWindow()->doc);
	gt.launchImporter(-1, fileName, false, QString("utf-8"), false, true, item);

	// FIXME: PyMem_Free() - are any needed??
	Py_RETURN_NONE;
}

PyObject *scribus_layouttext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame() && !item->isPathText())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot layout text of a non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	item->layout();

	Py_RETURN_NONE;
}

PyObject *scribus_layouttextchain(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame() && !item->isPathText())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot layout text chain for a non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}

	if (item->isPathText())
	{
		item->layout();
		Py_RETURN_NONE;
	}
	
	PageItem* firstFrame = item->firstInChain();
	PageItem* nextFrame = firstFrame;
	while (nextFrame)
	{
		nextFrame->layout();
		nextFrame = nextFrame->nextInChain();
	}

	Py_RETURN_NONE;
}

PyObject *scribus_settextalignment(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int alignment;
	if (!PyArg_ParseTuple(args, "i|es", &alignment, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if ((alignment > 4) || (alignment < 0))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Alignment out of range. Use one of the scribus.ALIGN_* constants.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text alignment on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	
	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode  = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetAlignment(alignment, &tmpSelection);
	doc->appMode = oldAppMode;

	Py_RETURN_NONE;
}

PyObject *scribus_setdirection(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int direction;
	if (!PyArg_ParseTuple(args, "i|es", &direction, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if ((direction > 1) || (direction < 0))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("direction out of range. Use one of the scribus.DIRECTION* constants.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text direction on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetDirection(direction, &tmpSelection);
	doc->appMode = oldAppMode;

	Py_RETURN_NONE;
}

PyObject *scribus_setfontsize(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double size;
	if (!PyArg_ParseTuple(args, "d|es", &size, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if ((size > 512) || (size < 1))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Font size out of bounds - must be 1 <= size <= 512.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;

	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font size on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetFontSize(qRound(size * 10.0), &tmpSelection);
	doc->appMode = oldAppMode;

	Py_RETURN_NONE;
}

PyObject *scribus_setfontfeatures(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *fontfeature = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &fontfeature, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;

	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font feature on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetFontFeatures(QString::fromUtf8(fontfeature), &tmpSelection);
	doc->appMode = oldAppMode;

	Py_RETURN_NONE;
}

PyObject *scribus_setfont(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Font = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Font, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (!PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.contains(QString::fromUtf8(Font)))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Font not found.", "python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetFont(QString::fromUtf8(Font), &tmpSelection);
	doc->appMode = oldAppMode;

	Py_RETURN_NONE;
}

PyObject *scribus_setfirstlineoffset(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int offset;
	if (!PyArg_ParseTuple(args, "i|es", &offset, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (offset < 0 || offset > (int) FLOPBaselineGrid)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("First line offset out of bounds, Use one of the scribus.FLOP_* constants.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set first line offset on a non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	item->setFirstLineOffset((FirstLineOffsetPolicy) offset);
	item->update();

	Py_RETURN_NONE;
}

PyObject *scribus_setlinespacing(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double w;
	if (!PyArg_ParseTuple(args, "d|es", &w, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (w < 0.1)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Line space out of bounds, must be >= 0.1.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set line spacing on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetLineSpacing(w, &tmpSelection);
	doc->appMode = oldAppMode;

	Py_RETURN_NONE;
}

PyObject *scribus_setlinespacingmode(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (w < 0 || w > 3) // Use constants?
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Line space mode invalid, must be 0, 1 or 2","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set line spacing mode on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetLineSpacingMode(w, &tmpSelection);
	doc->appMode = oldAppMode;
		
	Py_RETURN_NONE;
}

PyObject *scribus_settextdistances(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double l,r,t,b;
	if (!PyArg_ParseTuple(args, "dddd|es", &l, &r, &t, &b, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (l < 0.0 || r < 0.0 || t < 0.0 || b < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Text distances out of bounds, must be positive.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text distances on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	item->setTextToFrameDist(ValueToPoint(l), ValueToPoint(r), ValueToPoint(t), ValueToPoint(b));

	Py_RETURN_NONE;
}

PyObject *scribus_setcolumngap(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double w;
	if (!PyArg_ParseTuple(args, "d|es", &w, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (w < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Column gap out of bounds, must be positive.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set column gap on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	item->m_columnGap = ValueToPoint(w);

	Py_RETURN_NONE;
}

PyObject *scribus_setcolumns(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (w < 1)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Column count out of bounds, must be > 1.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set number of columns on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	item->m_columns = w;

	Py_RETURN_NONE;
}

PyObject *scribus_settextverticalalignment(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int alignment;
	if (!PyArg_ParseTuple(args, "i|es", &alignment, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (alignment < 0 || alignment > 2)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Vertical alignment out of bounds, Use one of the scribus.ALIGNV_* constants.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set vertical alignment on a non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	item->setVerticalAlignment(alignment);
	item->update();

	Py_RETURN_NONE;
}

PyObject *scribus_selectframetext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int start, selcount;
	if (!PyArg_ParseTuple(args, "ii|es", &start, &selcount, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;

	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot select text in a non-text frame", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (selcount < -1)
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Count must be positive, 0 or -1", "python error").toLocal8Bit().constData());
		return nullptr;
	}

	if (start < 0 || (selcount > 0 && ((item->lastInFrame() == -1) || (selcount + start > item->lastInFrame() - item->firstInFrame() + 1))))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Selection index out of bounds", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	start += item->firstInFrame();
	if (selcount == -1)
		selcount = item->lastInFrame() + 1 - start;
	item->itemText.deselectAll();
	if (selcount == 0)
	{
		item->HasSel = false;
		Py_RETURN_NONE;
	}
	item->itemText.select(start, selcount, true);
	item->HasSel = true;

	Py_RETURN_NONE;
}

PyObject *scribus_selecttext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int start, selcount;
	if (!PyArg_ParseTuple(args, "ii|es", &start, &selcount, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (selcount == -1)
	{
		// user wants to select all after the start point -- CR
		selcount = item->itemText.length() - start;
		if (selcount < 0)
			// user passed start that's > text in the frame
			selcount = 0;
	}
	// cr 2005-01-18 fixed off-by-one with end bound that made selecting the last char impossible
	if ((start < 0) || ((start + selcount) > static_cast<int>(item->itemText.length())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Selection index out of bounds", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (!(item->isTextFrame()) && !(item->isPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot select text in a non-text frame", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	/* FIXME: not sure if we should make this check or not
	if (start > ende)
	{
		PyErr_SetString(PyExc_ValueError, QString("Selection start > selection end").toLocal8Bit().constData());
		return nullptr;
	}
	*/
	item->itemText.deselectAll();
	if (selcount == 0)
	{
		item->HasSel = false;
		Py_RETURN_NONE;
	}
	item->itemText.select(start, selcount, true);
	item->HasSel = true;

	Py_RETURN_NONE;
}

PyObject *scribus_deletetext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame() && !item->isPathText())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot delete text from a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem_TextFrame* tf_item = item->asTextFrame();
	if (tf_item)
	{
		if (tf_item->HasSel)
			tf_item->deleteSelectedTextFromFrame();
		else
			tf_item->itemText.clear();
	}
	else
	{
		//Path text cannot have selected text, :( FIXME
		if (item->isPathText())
			item->itemText.clear();
	}
	Py_RETURN_NONE;
}

PyObject *scribus_settextfill(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Color;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Color, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame() && !item->isPathText())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text fill on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ApplyCharstyleHelper<QString>(item, QString::fromUtf8(Color)).apply(&CharStyle::setFillColor, 0, item->itemText.length());
	Py_RETURN_NONE;
}

PyObject *scribus_settextstroke(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Color;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Color, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame() && !item->isPathText())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text stroke on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ApplyCharstyleHelper<QString>(item, QString::fromUtf8(Color)).apply(&CharStyle::setStrokeColor, 0, item->itemText.length());
	Py_RETURN_NONE;
}


PyObject *scribus_settextscalingh(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double sc;
	if (!PyArg_ParseTuple(args, "d|es", &sc, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (sc < 10)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Character scaling out of bounds, must be >= 10","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set character scaling on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetScaleH(qRound(sc * 10), &tmpSelection);
	doc->appMode = oldAppMode;
		
	Py_RETURN_NONE;
}


PyObject *scribus_settextscalingv(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double sc;
	if (!PyArg_ParseTuple(args, "d|es", &sc, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (sc < 10)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Character scaling out of bounds, must be >= 10","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set character scaling on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int oldAppMode = ScCore->primaryMainWindow()->doc->appMode;

	Selection tmpSelection(nullptr, false);
	tmpSelection.addItem(item);
	if (item->HasSel)
		doc->appMode = modeEdit;
	doc->itemSelection_SetScaleV(qRound(sc * 10), &tmpSelection);
	doc->appMode = oldAppMode;
		
	Py_RETURN_NONE;
}


PyObject *scribus_settextshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if ((w < 0) || (w > 100))
		Py_RETURN_NONE;

	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame() && !item->isPathText())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text shade on a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ApplyCharstyleHelper<double>(item, w).apply(&CharStyle::setFillShade, 0, item->itemText.length());
	Py_RETURN_NONE;
}

PyObject *scribus_linktextframes(PyObject* /* self */, PyObject* args)
{
	char *name1;
	char *name2;

	if (!PyArg_ParseTuple(args, "eses", "utf-8", &name1, "utf-8", &name2))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *fromItem = GetUniqueItem(QString::fromUtf8(name1));
	if (fromItem == nullptr)
		return nullptr;
	PageItem *toItem = GetUniqueItem(QString::fromUtf8(name2));
	if (toItem == nullptr)
		return nullptr;
	if (!(fromItem->isTextFrame()) || !(toItem->isTextFrame()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only link text frames.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (fromItem->nextInChain() != nullptr)
	{
		PyErr_SetString(ScribusException, QObject::tr("Source frame is linked to another frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (toItem->prevInChain() != nullptr)
	{
		PyErr_SetString(ScribusException, QObject::tr("Target frame is linked to by another frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (toItem == fromItem)
	{
		PyErr_SetString(ScribusException, QObject::tr("Source and target are the same object.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	// references to the others boxes
	fromItem->link(toItem);
	ScCore->primaryMainWindow()->view->DrawNew();
	// enable 'save icon' stuff
	ScCore->primaryMainWindow()->slotDocCh();

	Py_RETURN_NONE;
}

PyObject *scribus_unlinktextframes(PyObject* /* self */, PyObject* args)
{
	char *name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot unlink a non-text frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	// only linked
	if (item->prevInChain() == nullptr)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object is not a linked text frame, can't unlink.","python error").toLocal8Bit().constData());
		return nullptr;
	}
/*	if (item->NextBox == 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object the last frame in a series, can't unlink. Unlink the previous frame instead.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	*/
/*	PageItem* nextbox = item->NextBox;

	while (nextbox != 0)
	{
		uint a = nextbox->itemText.count();
		for (uint s=0; s<a; ++s)
			item->itemText.append(nextbox->itemText.take(0));
		nextbox = nextbox->NextBox;
	} // while
	uint a2 = item->itemText.count();
	for (uint s = 0; s < a2; ++s)
		item->BackBox->itemText.append(item->itemText.take(0));
*/
	item->prevInChain()->unlink();
	// enable 'save icon' stuff
	ScCore->primaryMainWindow()->slotDocCh();
	ScCore->primaryMainWindow()->view->DrawNew();

	Py_RETURN_NONE;
}

/*
 * Convert the selected text frame to outlines.
 *
 * 2004-09-07 (Craig Ringer)
 * 2004-09-14 pv frame type, optional frame name param
 */
PyObject *scribus_outlinetext(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot convert a non-text frame to outlines.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->invalid)
		item->layout();
	ScCore->primaryMainWindow()->view->deselectItems(true);
	ScCore->primaryMainWindow()->view->selectItem(item);
	ScCore->primaryMainWindow()->view->TextToPath();

	Py_RETURN_NONE;
}

PyObject *scribus_istextoverflowing(PyObject * self, PyObject* args, PyObject* kw)
{
	int nolinks = 0;
	char *name = const_cast<char*>("");
	char *kwargs[] = {const_cast<char*>("name"), const_cast<char*>("nolinks"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "|esi", kwargs, "utf-8", &name, &nolinks))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Only text frames can be checked for overflowing", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	/* original solution
	if (item->itemText.count() > item->MaxChars)
	return PyBool_FromLong(static_cast<long>(true));
	return PyBool_FromLong(static_cast<long>(false)); */
	/*
	 uint firstFrame = 0;
	if (nolinks)
		firstFrame = item->itemText.count();
	uint chars = item->itemText.count();
	uint maxchars = item->MaxChars;
	while (item->NextBox != 0) {
		item = item->NextBox;
		chars += item->itemText.count();
		maxchars += item->MaxChars;
	}
	// no overrun
	if (nolinks)
		return PyLong_FromLong(maxchars - firstFrame);

	if (maxchars > chars)
		return PyLong_FromLong(0);
	// number of overrunning letters
	return PyLong_FromLong(static_cast<long>(chars - maxchars));
	 */
	// refresh overflow information
	item->invalidateLayout();
	item->layout();
	return PyLong_FromLong(static_cast<long>(item->frameOverflows()));
}

/*
 * Does hyphenation on the given text frame.
 * 08.12.2007: Joachim Neu
 */
PyObject *scribus_hyphenatetext(PyObject*, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only hyphenate text frame", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->doc->docHyphenator->slotHyphenate(item);
	return PyBool_FromLong(1);
}

/*
 * Does dehyphenation on the given text frame.
 * 13.12.2007: Joachim Neu
 */
PyObject *scribus_dehyphenatetext(PyObject*, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only dehyphenate text frame", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->doc->docHyphenator->slotDeHyphenate(item);
	return PyBool_FromLong(1);
}

PyObject *scribus_setpdfbookmark(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	bool toggle;
	if (!PyArg_ParseTuple(args, "b|es", &toggle, "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't set bookmark on a non-text frame", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->isBookmark == toggle)
		Py_RETURN_NONE;
	if (toggle)
	{
		item->setIsAnnotation(false);
		ScCore->primaryMainWindow()->AddBookMark(item);
	}
	else
		ScCore->primaryMainWindow()->DelBookMark(item);
	item->isBookmark = toggle;

	Py_RETURN_NONE;
}

PyObject *scribus_ispdfbookmark(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't get info from a non-text frame", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (item->isBookmark)
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdtextdocwarnings()
{
	QStringList s;
	s << scribus_dehyphenatetext__doc__
	  << scribus_deletetext__doc__
	  << scribus_getalltext__doc__
	  << scribus_getcolumngap__doc__
	  << scribus_getcolumns__doc__
	  << scribus_getfirstlineoffset__doc__
	  << scribus_getfirstlinkedframe__doc__
	  << scribus_getfont__doc__
	  << scribus_getfontfeatures__doc__
	  << scribus_getfontsize__doc__
	  << scribus_getframetext__doc__
	  << scribus_getlastlinkedframe__doc__
	  << scribus_getlinespacing__doc__
	  << scribus_getnextlinkedframe__doc__
	  << scribus_getprevlinkedframe__doc__
	  << scribus_gettext__doc__ // Deprecated
	  << scribus_gettextcolor__doc__
	  << scribus_gettextdistances__doc__
	  << scribus_gettextlength__doc__
	  << scribus_gettextlines__doc__
	  << scribus_gettextshade__doc__
	  << scribus_gettextverticalalignment__doc__
	  << scribus_hyphenatetext__doc__
	  << scribus_inserthtmltext__doc__
	  << scribus_inserttext__doc__
	  << scribus_ispdfbookmark__doc__
	  << scribus_istextoverflowing__doc__
	  << scribus_layouttext__doc__
	  << scribus_layouttextchain__doc__
	  << scribus_linktextframes__doc__
	  << scribus_outlinetext__doc__
	  << scribus_selectframetext__doc__
	  << scribus_selecttext__doc__
	  << scribus_setcolumngap__doc__
	  << scribus_setcolumns__doc__
	  << scribus_setdirection__doc__
	  << scribus_setfirstlineoffset__doc__
	  << scribus_setfont__doc__
	  << scribus_setfontfeatures__doc__
	  << scribus_setfontsize__doc__
	  << scribus_setlinespacing__doc__
	  << scribus_setlinespacingmode__doc__
	  << scribus_setpdfbookmark__doc__
	  << scribus_settextdistances__doc__
	  << scribus_settext__doc__
	  << scribus_settextalignment__doc__
	  << scribus_settextfill__doc__
	  << scribus_settextscalingh__doc__
	  << scribus_settextscalingv__doc__
	  << scribus_settextshade__doc__
	  << scribus_settextstroke__doc__
	  << scribus_settextverticalalignment__doc__
	  << scribus_tracetext__doc__
	  << scribus_unlinktextframes__doc__;
}
