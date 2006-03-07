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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font size of non-text frame.","python error"));
		return NULL;
	}
	if (it->HasSel)
	{
		for (uint b = 0; b < it->itemText.count(); b++)
			if (it->itemText.at(b)->cselect)
				return PyFloat_FromDouble(static_cast<double>(it->itemText.at(b)->csize / 10.0));
		return NULL;
	}
	else
		return PyFloat_FromDouble(static_cast<long>(it->fontSize() / 10.0));
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font of non-text frame.","python error"));
		return NULL;
	}
	if (it->HasSel)
	{
		for (uint b = 0; b < it->itemText.count(); b++)
			if (it->itemText.at(b)->cselect)
				return PyString_FromString(it->itemText.at(b)->cfont->scName().utf8());
		return NULL;
	}
	else
		return PyString_FromString(it->font().utf8());
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text size of non-text frame.","python error"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i->itemText.count()));
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column count of non-text frame.","python error"));
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get line space of non-text frame.","python error"));
		return NULL;
	}
	return PyFloat_FromDouble(static_cast<double>(i->lineSpacing()));
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column gap of non-text frame.","python error"));
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error"));
		return NULL;
	}
	for (uint a = 0; a < it->itemText.count(); a++)
	{
		if (it->HasSel)
		{
			if (it->itemText.at(a)->cselect)
				text += it->itemText.at(a)->ch;
		}
		else
		{
			text += it->itemText.at(a)->ch;
		}
	}
	return PyString_FromString(text.utf8());
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error"));
		return NULL;
	}
	PageItem *is = NULL;
	// Scan backwards to find the first frame in a linked series
	while (it->BackBox != 0)
	{
		is = GetUniqueItem(it->BackBox->itemName());
		if (is == NULL)
		{
			// While GetUniqueItem has already set an exception, we'll
			// overwrite that with a more suitable one for this particular case.
			// Not making this translatable, since it shouldnt' be seen and it's more important for
			// us to be able to understand the error if it's reported.
			PyErr_SetString(ScribusException, QString("(System Error) Broken linked frame series when scanning back"));
			return NULL;
		}
		it = is;
	} // while
	///FIXME: What does this do? Could use a comment for explanation
	for (uint a = 0; a < it->itemText.count(); a++)
	{
		if (it->HasSel)
		{
			if (it->itemText.at(a)->cselect)
				text += it->itemText.at(a)->ch;
		}
		else
		{
			text += it->itemText.at(a)->ch;
		}
	} // for
	// Scan forward through linked frames and ... what?
	while (it->NextBox != 0)
	{
		is = GetUniqueItem(it->NextBox->itemName());
		if (is == NULL)
		{
			// While GetUniqueItem has already set an exception, we'll
			// overwrite that with a more suitable one for this particular case.
			// Not making this translatable, since it shouldnt' be seen and it's more important for
			// us to be able to understand the error if it's reported.
			PyErr_SetString(ScribusException, QString("(System Error) Broken linked frame series when scanning forward"));
			return NULL;
		}
		it = is;
		assert(it != NULL);
		for (uint a = 0; a < it->itemText.count(); a++)
		{
			if (it->HasSel)
			{
				if (it->itemText.at(a)->cselect)
					text += it->itemText.at(a)->ch;
			}
			else
			{
				text += it->itemText.at(a)->ch;
			}
		} // for
	} // while
	return PyString_FromString(text.utf8());
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text of non-text frame.","python error"));
		return NULL;
	}
	QString Daten = QString::fromUtf8(Text);
	PyMem_Free(Text);
	if (currItem->NextBox != 0)
	{
		PageItem *nextItem = currItem->NextBox;
		while (nextItem != 0)
		{
			for (ScText *itx = nextItem->itemText.first(); itx != 0; itx = nextItem->itemText.next())
			{
				if ((itx->ch == QChar(25)) && (itx->cembedded != 0))
				{
					ScMW->doc->FrameItems.remove(itx->cembedded);
					delete itx->cembedded;
				}
			}
			nextItem->itemText.clear();
			nextItem->CPos = 0;
			nextItem = nextItem->NextBox;
		}
	}
	for (ScText *itx = currItem->itemText.first(); itx != 0; itx = currItem->itemText.next())
	{
		if ((itx->ch == QChar(25)) && (itx->cembedded != 0))
		{
			ScMW->doc->FrameItems.remove(itx->cembedded);
			delete itx->cembedded;
		}
	}
	currItem->itemText.clear();
	currItem->CPos = 0;
	for (uint a = 0; a < ScMW->doc->FrameItems.count(); ++a)
	{
		ScMW->doc->FrameItems.at(a)->ItemNr = a;
	}
	for (uint a = 0; a < Daten.length(); ++a)
	{
		struct ScText *hg = new ScText;
		hg->ch = Daten.at(a);
		if (hg->ch == QChar(10))
			hg->ch = QChar(13);
		hg->cfont = (*ScMW->doc->AllFonts)[currItem->font()];
		hg->csize = currItem->fontSize();
		hg->ccolor = currItem->TxtFill;
		hg->cshade = currItem->ShTxtFill;
		hg->cstroke = currItem->TxtStroke;
		hg->cshade2 = currItem->ShTxtStroke;
		hg->cscale = currItem->TxtScale;
		hg->cscalev = currItem->TxtScaleV;
		hg->cbase = currItem->TxtBase;
		hg->cshadowx = currItem->TxtShadowX;
		hg->cshadowy = currItem->TxtShadowY;
		hg->coutline = currItem->TxtOutline;
		hg->cunderpos = currItem->TxtUnderPos;
		hg->cunderwidth = currItem->TxtUnderWidth;
		hg->cstrikepos = currItem->TxtStrikePos;
		hg->cstrikewidth = currItem->TxtStrikeWidth;
		hg->cextra = 0;
		hg->cselect = false;
		hg->cstyle = 0;
		hg->cab = ScMW->doc->currentParaStyle;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		hg->cembedded = 0;
		currItem->itemText.append(hg);
	}
	currItem->Dirty = false;
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot insert text into non-text frame.","python error"));
		return NULL;
	}
	QString Daten = QString::fromUtf8(Text);
	PyMem_Free(Text);
	if ((pos < -1) || (pos > static_cast<int>(it->itemText.count())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Insert index out of bounds.","python error"));
		return NULL;
	}
	if (pos == -1)
		pos = it->itemText.count();
	for (uint a = 0; a < Daten.length(); ++a)
	{
		struct ScText *hg = new ScText;
		hg->ch = Daten.at(Daten.length()-1-a);
		if (hg->ch == QChar(10))
			hg->ch = QChar(13);
		hg->cfont = (*ScMW->doc->AllFonts)[it->font()];
		hg->csize = it->fontSize();
		hg->ccolor = it->TxtFill;
		hg->cshade = it->ShTxtFill;
		hg->cstroke = it->TxtStroke;
		hg->cshade2 = it->ShTxtStroke;
		hg->cscale = it->TxtScale;
		hg->cscalev = it->TxtScaleV;
		hg->cbase = it->TxtBase;
		hg->cshadowx = it->TxtShadowX;
		hg->cshadowy = it->TxtShadowY;
		hg->coutline = it->TxtOutline;
		hg->cunderpos = it->TxtUnderPos;
		hg->cunderwidth = it->TxtUnderWidth;
		hg->cstrikepos = it->TxtStrikePos;
		hg->cstrikewidth = it->TxtStrikeWidth;
		hg->cextra = 0;
		hg->cselect = false;
		hg->cstyle = 0;
		hg->cab = ScMW->doc->currentParaStyle;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		hg->cembedded = 0;
		it->itemText.insert(pos, hg);
	}
	it->CPos = pos + Daten.length();
	it->Dirty = true;
	if (ScMW->doc->DoDrawing)
	{
		it->paintObj();
		it->Dirty = false;
	}
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Alignment out of range. Use one of the scribus.ALIGN* constants.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text alignment on a non-text frame.","python error"));
		return NULL;
	}
	int Apm = ScMW->doc->appMode;
	ScMW->doc->m_Selection->clear();
	ScMW->doc->m_Selection->addItem(i);
	if (i->HasSel)
		ScMW->doc->appMode = modeEdit;
	ScMW->setNewAbStyle(alignment);
	ScMW->doc->appMode = Apm;
	ScMW->view->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Font size out of bounds - must be 1 <= size <= 512.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;

	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font size on a non-text frame.","python error"));
		return NULL;
	}
	int Apm = ScMW->doc->appMode;
	ScMW->doc->m_Selection->clear();
	ScMW->doc->m_Selection->addItem(i);
	if (i->HasSel)
		ScMW->doc->appMode = modeEdit;
	ScMW->doc->chFSize(qRound(size * 10.0));
	ScMW->doc->appMode = Apm;
	ScMW->view->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font on a non-text frame.","python error"));
		return NULL;
	}
	if (PrefsManager::instance()->appPrefs.AvailFonts.find(QString::fromUtf8(Font)))
	{
		int Apm = ScMW->doc->appMode;
		ScMW->doc->m_Selection->clear();
		ScMW->doc->m_Selection->addItem(i);
		if (i->HasSel)
			ScMW->doc->appMode = modeEdit;
		ScMW->SetNewFont(QString::fromUtf8(Font));
		ScMW->doc->appMode = Apm;
		ScMW->view->Deselect();
	}
	else
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Font not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Line space out of bounds, must be >= 0.1.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set line spacing on a non-text frame.","python error"));
		return NULL;
	}
	i->setLineSpacing(w);
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Column gap out of bounds, must be positive.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set column gap on a non-text frame.","python error"));
		return NULL;
	}
	i->ColGap = ValueToPoint(w);
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Column count out of bounds, must be > 1.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (!i->asTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set number of columns on a non-text frame.","python error"));
		return NULL;
	}
	i->Cols = w;
	Py_INCREF(Py_None);
	return Py_None;
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
		selcount = it->itemText.count() - start;
		if (selcount < 0)
			// user passed start that's > text in the frame
			selcount = 0;
	}
	// cr 2005-01-18 fixed off-by-one with end bound that made selecting the last char impossible
	if ((start < 0) || ((start + selcount) > static_cast<int>(it->itemText.count())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Selection index out of bounds", "python error"));
		return NULL;
	}
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot select text in a non-text frame", "python error"));
		return NULL;
	}
	/* FIXME: not sure if we should make this check or not
	if (start > ende)
	{
		PyErr_SetString(PyExc_ValueError, QString("Selection start > selection end"));
		return NULL;
	}
	*/
	for (uint a = 0; a < it->itemText.count(); ++a)
		it->itemText.at(a)->cselect = false;
	if (selcount == 0)
	{
		it->HasSel = false;
		Py_INCREF(Py_None);
		return Py_None;
	}
	for (int aa = start; aa < (start + selcount); ++aa)
		it->itemText.at(aa)->cselect = true;
	it->HasSel = true;
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot delete text from a non-text frame.","python error"));
		return NULL;
	}
	if (it->HasSel)
		dynamic_cast<PageItem_TextFrame*>(it)->deleteSelectedTextFromFrame();
	else
	{
		for (ScText *itx = it->itemText.first(); itx != 0; itx = it->itemText.next())
		{
			if ((itx->ch == QChar(25)) && (itx->cembedded != 0))
			{
				ScMW->doc->FrameItems.remove(itx->cembedded);
				delete itx->cembedded;
			}
		}
		it->itemText.clear();
		it->CPos = 0;
		for (uint a = 0; a < ScMW->doc->FrameItems.count(); ++a)
		{
			ScMW->doc->FrameItems.at(a)->ItemNr = a;
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text fill on a non-text frame.","python error"));
		return NULL;
	}
	else
	{
		for (uint b = 0; b < it->itemText.count(); b++)
		{
			if (it->HasSel)
			{
				if (it->itemText.at(b)->cselect)
					it->itemText.at(b)->ccolor = QString::fromUtf8(Color);
			}
			else
				it->itemText.at(b)->ccolor = QString::fromUtf8(Color);
		}
		it->TxtFill = QString::fromUtf8(Color);
	}
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text stroke on a non-text frame.","python error"));
		return NULL;
	}
	else
	{
		for (uint b = 0; b < it->itemText.count(); b++)
		{
			if (it->HasSel)
			{
				if (it->itemText.at(b)->cselect)
					it->itemText.at(b)->cstroke = QString::fromUtf8(Color);
			}
			else
				it->itemText.at(b)->cstroke = QString::fromUtf8(Color);
		}
		it->TxtStroke = QString::fromUtf8(Color);
	}
	Py_INCREF(Py_None);
	return Py_None;
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
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if (!(it->asTextFrame()) && !(it->asPathText()))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text shade on a non-text frame.","python error"));
		return NULL;
	}
	else
	{
		for (uint b = 0; b < it->itemText.count(); ++b)
		{
			if (it->HasSel)
			{
				if (it->itemText.at(b)->cselect)
					it->itemText.at(b)->cshade = w;
			}
			else
				it->itemText.at(b)->cshade = w;
		}
	it->ShTxtFill = w;
	}
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only link text frames.","python error"));
		return NULL;
	}
	if (toitem->itemText.count())
	{
		PyErr_SetString(ScribusException, QObject::tr("Target frame must be empty.","python error"));
		return NULL;
	}
	if (toitem->NextBox != 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Target frame links to another frame.","python error"));
		return NULL;
	}
	if (toitem->BackBox != 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Target frame is linked to by another frame.","python error"));
		return NULL;
	}
	if (toitem == fromitem)
	{
		PyErr_SetString(ScribusException, QObject::tr("Source and target are the same object.","python error"));
		return NULL;
	}
	// references to the others boxes
	fromitem->NextBox = toitem;
	toitem->BackBox = fromitem;
	ScMW->view->DrawNew();
	// enable 'save icon' stuff
	ScMW->slotDocCh();
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot unlink a non-text frame.","python error"));
		return NULL;
	}
	// only linked
	if (item->BackBox == 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object is not a linked text frame, can't unlink.","python error"));
		return NULL;
	}
	if (item->NextBox == 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object the last frame in a series, can't unlink. Unlink the previous frame instead.","python error"));
		return NULL;
	}
	PageItem* nextbox = item->NextBox;
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
	item->BackBox->NextBox = 0;
	item->BackBox = 0;
	// enable 'save icon' stuff
	ScMW->slotDocCh();
	ScMW->view->DrawNew();
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot convert a non-text frame to outlines.","python error"));
		return NULL;
	}
	ScMW->view->Deselect(true);
	ScMW->view->SelectItemNr(item->ItemNr);
	ScMW->view->TextToPath();
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Only text frames can be checked for overflowing", "python error"));
		return NULL;
	}
	/* original solution
	if (item->itemText.count() > item->MaxChars)
	return PyBool_FromLong(static_cast<long>(true));
	return PyBool_FromLong(static_cast<long>(false)); */
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't set bookmark on a non-text frame", "python error"));
		return NULL;
	}
	if (i->isBookmark == toggle)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	if (toggle)
	{
		i->setIsAnnotation(false);
		ScMW->AddBookMark(i);
	}
	else
		ScMW->DelBookMark(i);
	i->isBookmark = toggle;
	Py_INCREF(Py_None);
	return Py_None;
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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't get info from a non-text frame", "python error"));
		return NULL;
	}
	if (i->isBookmark)
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}
