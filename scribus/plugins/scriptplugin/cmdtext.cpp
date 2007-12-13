/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdtext.h"
#include "cmdutil.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include "selection.h"
#include "util.h"
#include "scribuscore.h"
#include "hyphenator.h"

PyObject *scribus_getfontsize(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font size of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (it->HasSel)
	{
		for (int b = 0; b < it->itemText.length(); b++)
			if (it->itemText.selected(b))
				return PyFloat_FromDouble(static_cast<double>(it->itemText.charStyle(b).fontSize() / 10.0));
		return NULL;
	}
	else
		return PyFloat_FromDouble(static_cast<long>(it->currentCharStyle().fontSize() / 10.0));
}

PyObject *scribus_getfont(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (it->HasSel)
	{
		for (int b = 0; b < it->itemText.length(); b++)
			if (it->itemText.selected(b))
				return PyString_FromString(it->itemText.charStyle(b).font().scName().toUtf8());
		return NULL;
	}
	else
		return PyString_FromString(it->currentCharStyle().font().scName().toUtf8());
}

PyObject *scribus_gettextsize(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!(i->asTextFrame()) && !(i->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text size of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i->itemText.length()));
}

PyObject *scribus_gettextlines(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!(i->asTextFrame()) && !(i->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get number of lines of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i->itemText.lines()));
}

PyObject *scribus_getcolumns(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column count of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i->Cols));
}

PyObject *scribus_getlinespace(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get line space of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyFloat_FromDouble(static_cast<double>(i->currentStyle().lineSpacing()));
}

PyObject *scribus_getcolumngap(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column gap of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyFloat_FromDouble(PointToValue(static_cast<double>(i->ColGap)));
}

PyObject *scribus_getframetext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	QString text = "";
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	for (int a = it->firstInFrame(); a < it->lastInFrame(); a++)
	{
		if (it->HasSel)
		{
			if (it->itemText.selected(a))
				text += it->itemText.text(a);
		}
		else
		{
			text += it->itemText.text(a);
		}
	}
	return PyString_FromString(text.toUtf8());
}

PyObject *scribus_gettext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	QString text = "";
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}

	// collect all chars from a storytext
	for (int a = 0; a < it->itemText.length(); a++)
	{
		if (it->HasSel)
		{
			if (it->itemText.selected(a))
				text += it->itemText.text(a);
		}
		else
		{
			text += it->itemText.text(a);
		}
	} // for
	return PyString_FromString(text.toUtf8());
}

PyObject *scribus_setboxtext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Text;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Text, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *currItem = GetUniqueItem(QString::fromUtf8(Name));
	if (currItem == NULL)
		return NULL;
	if (!(currItem->asTextFrame()) && !(currItem->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text of non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString Daten = QString::fromUtf8(Text);
	PyMem_Free(Text);
	currItem->itemText.clear();
	currItem->CPos = 0;
	for (int a = 0; a < ScCore->primaryMainWindow()->doc->FrameItems.count(); ++a)
	{
		ScCore->primaryMainWindow()->doc->FrameItems.at(a)->ItemNr = a;
	}
	currItem->itemText.insertChars(0, Daten);
	currItem->Dirty = false;
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_inserttext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Text;
	int pos;
	if (!PyArg_ParseTuple(args, "esi|es", "utf-8", &Text, &pos, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot insert text into non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString Daten = QString::fromUtf8(Text);
	PyMem_Free(Text);
	if ((pos < -1) || (pos > static_cast<int>(it->itemText.length())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Insert index out of bounds.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (pos == -1)
		pos = it->itemText.length();
	it->itemText.insertChars(pos, Daten);
	it->CPos = pos + Daten.length();
	it->Dirty = true;
	if (ScCore->primaryMainWindow()->doc->DoDrawing)
	{
// FIXME adapt to Qt-4 painting style
//		it->paintObj();
		it->Dirty = false;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setalign(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int alignment;
	if (!PyArg_ParseTuple(args, "i|es", &alignment, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((alignment > 4) || (alignment < 0))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Alignment out of range. Use one of the scribus.ALIGN* constants.","python error").toLocal8Bit().constData());
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text alignment on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
	if (i->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->setNewAlignment(alignment);
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setfontsize(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double size;
	if (!PyArg_ParseTuple(args, "d|es", &size, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((size > 512) || (size < 1))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Font size out of bounds - must be 1 <= size <= 512.","python error").toLocal8Bit().constData());
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;

	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font size on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
	if (i->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->doc->itemSelection_SetFontSize(qRound(size * 10.0));
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setfont(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Font = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Font, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!(i->asTextFrame()) && !(i->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (PrefsManager::instance()->appPrefs.AvailFonts.contains(QString::fromUtf8(Font)))
	{
		int Apm = ScCore->primaryMainWindow()->doc->appMode;
		ScCore->primaryMainWindow()->doc->m_Selection->clear();
		ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
		if (i->HasSel)
			ScCore->primaryMainWindow()->doc->appMode = modeEdit;
		ScCore->primaryMainWindow()->SetNewFont(QString::fromUtf8(Font));
		ScCore->primaryMainWindow()->doc->appMode = Apm;
		ScCore->primaryMainWindow()->view->Deselect();
	}
	else
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Font not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setlinespace(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double w;
	if (!PyArg_ParseTuple(args, "d|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 0.1)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Line space out of bounds, must be >= 0.1.","python error").toLocal8Bit().constData());
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set line spacing on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	i->setLineSpacing(w);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setcolumngap(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double w;
	if (!PyArg_ParseTuple(args, "d|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Column gap out of bounds, must be positive.","python error").toLocal8Bit().constData());
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set column gap on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	i->ColGap = ValueToPoint(w);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setcolumns(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 1)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Column count out of bounds, must be > 1.","python error").toLocal8Bit().constData());
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set number of columns on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	i->Cols = w;
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_selecttext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int start, selcount;
	if (!PyArg_ParseTuple(args, "ii|es", &start, &selcount, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (selcount == -1)
	{
		// user wants to select all after the start point -- CR
		selcount = it->itemText.length() - start;
		if (selcount < 0)
			// user passed start that's > text in the frame
			selcount = 0;
	}
	// cr 2005-01-18 fixed off-by-one with end bound that made selecting the last char impossible
	if ((start < 0) || ((start + selcount) > static_cast<int>(it->itemText.length())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Selection index out of bounds", "python error").toLocal8Bit().constData());
		return NULL;
	}
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot select text in a non-text frame", "python error").toLocal8Bit().constData());
		return NULL;
	}
	/* FIXME: not sure if we should make this check or not
	if (start > ende)
	{
		PyErr_SetString(PyExc_ValueError, QString("Selection start > selection end").toLocal8Bit().constData());
		return NULL;
	}
	*/
	it->itemText.deselectAll();
	if (selcount == 0)
	{
		it->HasSel = false;
//		Py_INCREF(Py_None);
//		return Py_None;
		Py_RETURN_NONE;
	}
	it->itemText.select(start, selcount, true);
	it->HasSel = true;
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_deletetext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot delete text from a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (it->HasSel)
		dynamic_cast<PageItem_TextFrame*>(it)->deleteSelectedTextFromFrame();
	else
	{
		it->itemText.clear();
		it->CPos = 0;
		for (int a = 0; a < ScCore->primaryMainWindow()->doc->FrameItems.count(); ++a)
		{
			ScCore->primaryMainWindow()->doc->FrameItems.at(a)->ItemNr = a;
		}
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_settextfill(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Color;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Color, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text fill on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	else
	{
		for (int b = 0; b < it->itemText.length(); b++)
		{
			//FIXME: doc method
			if (it->HasSel)
			{
				if (it->itemText.selected(b))
					it->itemText.item(b)->setFillColor(QString::fromUtf8(Color));
			}
			else
				it->itemText.item(b)->setFillColor(QString::fromUtf8(Color));
		}
//		it->TxtFill = QString::fromUtf8(Color);
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_settextstroke(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Color;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Color, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && (it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text stroke on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	else
	{
		for (int b = 0; b < it->itemText.length(); b++)
		{
			//FIXME:NLS use document method for this
			if (it->HasSel)
			{
				if (it->itemText.selected(b))
					it->itemText.item(b)->setStrokeColor(QString::fromUtf8(Color));
			}
			else
				it->itemText.item(b)->setStrokeColor(QString::fromUtf8(Color));
		}
//		it->TxtStroke = QString::fromUtf8(Color);
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_settextshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
//		Py_INCREF(Py_None);
//		return Py_None;
		Py_RETURN_NONE;
	}
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text shade on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	else
	{
		//FIXME:NLS use document method for that
		for (int b = 0; b < it->itemText.length(); ++b)
		{
			if (it->HasSel)
			{
				if (it->itemText.selected(b))
					it->itemText.item(b)->setFillShade(w);
			}
			else
				it->itemText.item(b)->setFillShade(w);
		}
//	it->ShTxtFill = w;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_linktextframes(PyObject* /* self */, PyObject* args)
{
	char *name1;
	char *name2;

	if (!PyArg_ParseTuple(args, "eses", "utf-8", &name1, "utf-8", &name2))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *fromitem = GetUniqueItem(QString::fromUtf8(name1));
	if (fromitem == NULL)
		return NULL;
	PageItem *toitem = GetUniqueItem(QString::fromUtf8(name2));
	if (toitem == NULL)
		return NULL;
	if (!(fromitem->asTextFrame()) || !(toitem->asTextFrame()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only link text frames.","python error").toLocal8Bit().constData());
		return NULL;
	}
/*	if (toitem->itemText.length() > 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Target frame must be empty.","python error").toLocal8Bit().constData());
		return NULL;
	}*/
	if (toitem->nextInChain() != 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Target frame links to another frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (toitem->prevInChain() != 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Target frame is linked to by another frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (toitem == fromitem)
	{
		PyErr_SetString(ScribusException, QObject::tr("Source and target are the same object.","python error").toLocal8Bit().constData());
		return NULL;
	}
	// references to the others boxes
	fromitem->link(toitem);
	ScCore->primaryMainWindow()->view->DrawNew();
	// enable 'save icon' stuff
	ScCore->primaryMainWindow()->slotDocCh();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_unlinktextframes(PyObject* /* self */, PyObject* args)
{
	char *name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (!item->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot unlink a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	// only linked
	if (item->prevInChain() == 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object is not a linked text frame, can't unlink.","python error").toLocal8Bit().constData());
		return NULL;
	}
/*	if (item->NextBox == 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object the last frame in a series, can't unlink. Unlink the previous frame instead.","python error").toLocal8Bit().constData());
		return NULL;
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
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*
 * Convert the selected text frame to outlines.
 *
 * 2004-09-07 (Craig Ringer)
 * 2004-09-14 pv frame type, optional frame name param
 */
PyObject *scribus_tracetext(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (!item->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot convert a non-text frame to outlines.","python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->view->Deselect(true);
	ScCore->primaryMainWindow()->view->SelectItemNr(item->ItemNr);
	ScCore->primaryMainWindow()->view->TextToPath();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_istextoverflowing(PyObject * self, PyObject* args, PyObject* kw)
{
	char *name = const_cast<char*>("");
	bool nolinks = false;
	char *kwargs[] = {const_cast<char*>("name"), const_cast<char*>("nolinks"), NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "|esi", kwargs, "utf-8", &name, &nolinks))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (!item->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Only text frames can be checked for overflowing", "python error").toLocal8Bit().constData());
		return NULL;
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
		return PyInt_FromLong(maxchars - firstFrame);

	if (maxchars > chars)
		return PyInt_FromLong(0);
	// number of overrunning letters
	return PyInt_FromLong(static_cast<long>(chars - maxchars));
	 */
	// refresh overflow information
	item->invalidateLayout();
	item->layout();
	return PyInt_FromLong(static_cast<long>(item->frameOverflows()));
}

/*
 * Does hyphenation on the given text frame.
 * 08.12.2007: Joachim Neu
 */
PyObject *scribus_hyphenatetext(PyObject*, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only hyphenate text frame", "python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->doc->docHyphenator->slotHyphenate(i);
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
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only dehyphenate text frame", "python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->doc->docHyphenator->slotDeHyphenate(i);
	return PyBool_FromLong(1);
}

PyObject *scribus_setpdfbookmark(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	bool toggle;
	if (!PyArg_ParseTuple(args, "b|es", &toggle, "utf-8", &name))
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't set bookmark on a non-text frame", "python error").toLocal8Bit().constData());
		return NULL;
	}
	if (i->isBookmark == toggle)
	{
//		Py_INCREF(Py_None);
//		return Py_None;
		Py_RETURN_NONE;
	}
	if (toggle)
	{
		i->setIsAnnotation(false);
		ScCore->primaryMainWindow()->AddBookMark(i);
	}
	else
		ScCore->primaryMainWindow()->DelBookMark(i);
	i->isBookmark = toggle;
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_ispdfbookmark(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == NULL)
		return NULL;
	if (i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't get info from a non-text frame", "python error").toLocal8Bit().constData());
		return NULL;
	}
	if (i->isBookmark)
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdtextdocwarnings()
{
    QStringList s;
    s << scribus_getfontsize__doc__    << scribus_getfont__doc__
	  << scribus_gettextlines__doc__   << scribus_gettextsize__doc__
	  << scribus_getframetext__doc__   << scribus_gettext__doc__
	  << scribus_getlinespace__doc__   << scribus_getcolumngap__doc__
	  << scribus_getcolumns__doc__     << scribus_setboxtext__doc__
	  << scribus_inserttext__doc__     << scribus_setfont__doc__
	  << scribus_setfontsize__doc__    << scribus_setlinespace__doc__
	  << scribus_setcolumngap__doc__   << scribus_setcolumns__doc__
	  << scribus_setalign__doc__       << scribus_selecttext__doc__
	  << scribus_deletetext__doc__     << scribus_settextfill__doc__
	  << scribus_settextstroke__doc__  << scribus_settextshade__doc__
	  << scribus_linktextframes__doc__ << scribus_unlinktextframes__doc__
	  << scribus_tracetext__doc__      << scribus_istextoverflowing__doc__
	  << scribus_setpdfbookmark__doc__ << scribus_ispdfbookmark__doc__
	<< scribus_hyphenatetext__doc__ << scribus_dehyphenatetext__doc__;
}
