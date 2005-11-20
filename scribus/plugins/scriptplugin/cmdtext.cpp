/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "cmdtext.h"
#include "cmdutil.h"

PyObject *scribus_getfontsize(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font size of non-text frame.","python error"));
		return NULL;
	}
	if (it->HasSel)
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
			if (it->Ptext.at(b)->cselect)
				return PyFloat_FromDouble(static_cast<double>(it->Ptext.at(b)->csize / 10.0));
		return NULL;
	}
	else
		return PyFloat_FromDouble(static_cast<long>(it->ISize / 10.0));
}

PyObject *scribus_getfont(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get font of non-text frame.","python error"));
		return NULL;
	}
	if (it->HasSel)
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
			if (it->Ptext.at(b)->cselect)
				return PyString_FromString(it->Ptext.at(b)->cfont.utf8());
		return NULL;
	}
	else
		return PyString_FromString(it->IFont.utf8());
}

PyObject *scribus_gettextsize(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if ((i->PType != FRAME_TEXT) && (i->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text size of non-text frame.","python error"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i->Ptext.count()));
}

PyObject *scribus_getcolumns(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column count of non-text frame.","python error"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i->Cols));
}

PyObject *scribus_getlinespace(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get line space of non-text frame.","python error"));
		return NULL;
	}
	return PyFloat_FromDouble(static_cast<double>(i->LineSp));
}

PyObject *scribus_getcolumngap(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get column gap of non-text frame.","python error"));
		return NULL;
	}
	return PyFloat_FromDouble(PointToValue(static_cast<double>(i->ColGap)));
}

PyObject *scribus_getframetext(PyObject */*self*/, PyObject* args)
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
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error"));
		return NULL;
	}
	for (uint a = 0; a < it->Ptext.count(); a++)
	{
		if (it->HasSel)
		{
			if (it->Ptext.at(a)->cselect)
				text += it->Ptext.at(a)->ch;
		}
		else
		{
			text += it->Ptext.at(a)->ch;
		}
	}
	return PyString_FromString(text.utf8());
}

PyObject *scribus_gettext(PyObject */*self*/, PyObject* args)
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
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get text of non-text frame.","python error"));
		return NULL;
	}
	PageItem *is = NULL;
	// Scan backwards to find the first frame in a linked series
	while (it->BackBox != 0)
	{
		is = GetUniqueItem(it->BackBox->AnName);
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
	for (uint a = 0; a < it->Ptext.count(); a++)
	{
		if (it->HasSel)
		{
			if (it->Ptext.at(a)->cselect)
				text += it->Ptext.at(a)->ch;
		}
		else
		{
			text += it->Ptext.at(a)->ch;
		}
	} // for
	// Scan forward through linked frames and ... what?
	while (it->NextBox != 0)
	{
		is = GetUniqueItem(it->NextBox->AnName);
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
		for (uint a = 0; a < it->Ptext.count(); a++)
		{
			if (it->HasSel)
			{
				if (it->Ptext.at(a)->cselect)
					text += it->Ptext.at(a)->ch;
			}
			else
			{
				text += it->Ptext.at(a)->ch;
			}
		} // for
	} // while
	return PyString_FromString(text.utf8());
}

PyObject *scribus_setboxtext(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Text;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Text, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text of non-text frame.","python error"));
		return NULL;
	}
	QString Daten = QString::fromUtf8(Text);
	PyMem_Free(Text);
	if (it->NextBox != 0)
	{
		PageItem *nb = it->NextBox;
		while (nb != 0)
		{
			nb->Ptext.clear();
			nb->CPos = 0;
			nb = nb->NextBox;
		}
	}
	it->Ptext.clear();
	it->CPos = 0;
	for (uint a = 0; a < Daten.length(); ++a)
	{
		struct Pti *hg = new Pti;
		hg->ch = Daten.at(a);
		if (hg->ch == QChar(10))
			hg->ch = QChar(13);
		hg->cfont = it->IFont;
		hg->csize = it->ISize;
		hg->ccolor = it->TxtFill;
		hg->cshade = it->ShTxtFill;
		hg->cstroke = it->TxtStroke;
		hg->cshade2 = it->ShTxtStroke;
		hg->cscale = it->TxtScale;
		hg->cextra = 0;
		hg->cselect = false;
		hg->cstyle = 0;
		hg->cab = Carrier->doc->CurrentABStil;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		it->Ptext.append(hg);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_inserttext(PyObject */*self*/, PyObject* args)
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
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot insert text into non-text frame.","python error"));
		return NULL;
	}
	QString Daten = QString::fromUtf8(Text);
	PyMem_Free(Text);
	if ((pos < -1) || (pos > static_cast<int>(it->Ptext.count())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Insert index out of bounds.","python error"));
		return NULL;
	}
	if (pos == -1)
		pos = it->Ptext.count();
	for (uint a = 0; a < Daten.length(); ++a)
	{
		struct Pti *hg = new Pti;
		hg->ch = Daten.at(Daten.length()-1-a);
		if (hg->ch == QChar(10))
			hg->ch = QChar(13);
		hg->cfont = it->IFont;
		hg->csize = it->ISize;
		hg->ccolor = it->TxtFill;
		hg->cshade = it->ShTxtFill;
		hg->cstroke = it->TxtStroke;
		hg->cshade2 = it->ShTxtStroke;
		hg->cscale = it->TxtScale;
		hg->cextra = 0;
		hg->cselect = false;
		hg->cstyle = 0;
		hg->cab = Carrier->doc->CurrentABStil;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		it->Ptext.insert(pos, hg);
	}
	it->CPos = pos + Daten.length();
	it->paintObj();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setalign(PyObject */*self*/, PyObject* args)
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
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text alignment on a non-text frame.","python error"));
		return NULL;
	}
	int Apm = Carrier->doc->AppMode;
	i->OwnPage->SelItem.clear();
	i->OwnPage->SelItem.append(i);
	if (i->HasSel)
		Carrier->doc->AppMode = 7;
	Carrier->setNewAbStyle(alignment);
	Carrier->doc->AppMode = Apm;
	i->OwnPage->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfontsize(PyObject */*self*/, PyObject* args)
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

	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font size on a non-text frame.","python error"));
		return NULL;
	}
	int Apm = Carrier->doc->AppMode;
	i->OwnPage->SelItem.clear();
	i->OwnPage->SelItem.append(i);
	if (i->HasSel)
		Carrier->doc->AppMode = 7;
	i->OwnPage->chFSize(qRound(size * 10.0));
	Carrier->doc->AppMode = Apm;
	i->OwnPage->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfont(PyObject */*self*/, PyObject* args)
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
	if ((i->PType != FRAME_TEXT) && (i->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set font on a non-text frame.","python error"));
		return NULL;
	}
	if (Carrier->Prefs.AvailFonts.find(QString::fromUtf8(Font)))
	{
		int Apm = Carrier->doc->AppMode;
		i->OwnPage->SelItem.clear();
		i->OwnPage->SelItem.append(i);
		if (i->HasSel)
			Carrier->doc->AppMode = 7;
		Carrier->SetNewFont(QString::fromUtf8(Font));
		Carrier->doc->AppMode = Apm;
		i->OwnPage->Deselect();
	}
	else
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Font not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinespace(PyObject */*self*/, PyObject* args)
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
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set line spacing on a non-text frame.","python error"));
		return NULL;
	}
	i->LineSp = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setcolumngap(PyObject */*self*/, PyObject* args)
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
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set column gap on a non-text frame.","python error"));
		return NULL;
	}
	i->ColGap = ValueToPoint(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setcolumns(PyObject */*self*/, PyObject* args)
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
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set number of columns on a non-text frame.","python error"));
		return NULL;
	}
	i->Cols = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_selecttext(PyObject */*self*/, PyObject* args)
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
		selcount = it->Ptext.count() - start;
		if (selcount < 0)
			// user passed start that's > text in the frame
			selcount = 0;
	}
	// cr 2005-01-18 fixed off-by-one with end bound that made selecting the last char impossible
	if ((start < 0) || ((start + selcount) > static_cast<int>(it->Ptext.count())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Selection index out of bounds", "python error"));
		return NULL;
	}
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot select text in a non-text frame", "python error"));
		return NULL;
	}
	/* FIXME: not sure if we should make this check or not
	if (start > selcount)
	{
		PyErr_SetString(PyExc_ValueError, QString("Selection start > selection end"));
		return NULL;
	}
	*/
	for (uint a = 0; a < it->Ptext.count(); ++a)
		it->Ptext.at(a)->cselect = false;
	if (selcount == 0)
	{
		it->HasSel = false;
		Py_INCREF(Py_None);
		return Py_None;
	}
	for (int aa = start; aa < (start + selcount); ++aa)
		it->Ptext.at(aa)->cselect = true;
	it->HasSel = true;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_deletetext(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot delete text from a non-text frame.","python error"));
		return NULL;
	}
	if (it->HasSel)
		Carrier->DeleteSel(it);
	else
	{
		it->Ptext.clear();
		it->CPos = 0;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_settextfill(PyObject */*self*/, PyObject* args)
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
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text fill on a non-text frame.","python error"));
		return NULL;
	}
	else
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
		{
			if (it->HasSel)
			{
				if (it->Ptext.at(b)->cselect)
					it->Ptext.at(b)->ccolor = QString::fromUtf8(Color);
			}
			else
				it->Ptext.at(b)->ccolor = QString::fromUtf8(Color);
		}
		it->TxtFill = QString::fromUtf8(Color);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_settextstroke(PyObject */*self*/, PyObject* args)
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
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text stroke on a non-text frame.","python error"));
		return NULL;
	}
	else
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
		{
			if (it->HasSel)
			{
				if (it->Ptext.at(b)->cselect)
					it->Ptext.at(b)->cstroke = QString::fromUtf8(Color);
			}
			else
				it->Ptext.at(b)->cstroke = QString::fromUtf8(Color);
		}
		it->TxtStroke = QString::fromUtf8(Color);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_settextshade(PyObject */*self*/, PyObject* args)
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
	if ((it->PType != FRAME_TEXT) && (it->PType != FRAME_PATHTEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set text shade on a non-text frame.","python error"));
		return NULL;
	}
	else
	{
		for (uint b = 0; b < it->Ptext.count(); ++b)
		{
			if (it->HasSel)
			{
				if (it->Ptext.at(b)->cselect)
					it->Ptext.at(b)->cshade = w;
			}
			else
				it->Ptext.at(b)->cshade = w;
		}
	it->ShTxtFill = w;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_linktextframes(PyObject */*self*/, PyObject* args)
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
	if ((fromitem->PType != FRAME_TEXT) || (toitem->PType != FRAME_TEXT))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can only link text frames.","python error"));
		return NULL;
	}
	if (toitem->Ptext.count())
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
	fromitem->OwnPage->repaint();
	toitem->OwnPage->repaint();
	// enable 'save icon' stuff
	Carrier->slotDocCh();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_unlinktextframes(PyObject */*self*/, PyObject* args)
{
	char *name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (item->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot unlink a non-text frame.","python error"));
		return NULL;
	}
	// only linked
	if (item->BackBox == 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object is not a linked text frame, cannot unlink.","python error"));
		return NULL;
	}
	if (item->NextBox == 0)
	{
		PyErr_SetString(ScribusException, QObject::tr("Object the last frame in a series, cannot unlink. Unlink the previous frame instead.","python error"));
		return NULL;
	}
	PageItem* nextbox = item->NextBox;
	while (nextbox != 0)
	{
		uint a = nextbox->Ptext.count();
		for (uint s=0; s<a; ++s)
			item->Ptext.append(nextbox->Ptext.take(0));
		nextbox = nextbox->NextBox;
	} // while
	uint a2 = item->Ptext.count();
	for (uint s = 0; s < a2; ++s)
		item->BackBox->Ptext.append(item->Ptext.take(0));
	item->BackBox->NextBox = 0;
	item->BackBox = 0;
	// enable 'save icon' stuff
	Carrier->slotDocCh();
	item->OwnPage->repaint();
	Py_INCREF(Py_None);
	return Py_None;
}

/*
 * Convert the selected text frame to outlines.
 *
 * 2004-09-07 (Craig Ringer)
 * 2004-09-14 pv frame type, optional frame name param
 */
PyObject *scribus_tracetext(PyObject */*self*/, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (item->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot convert a non-text frame to outlines.","python error"));
		return NULL;
	}
	Carrier->doc->ActPage = item->OwnPage;
	Carrier->doc->ActPage->Deselect(true);
	Carrier->doc->ActPage->SelectItemNr(item->ItemNr);
	Carrier->doc->ActPage->TextToPath();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_istextoverflowing(PyObject * /*self*/, PyObject* args, PyObject* kw)
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
	if (item->PType != FRAME_TEXT)
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
		firstFrame = item->Ptext.count();
	uint chars = item->Ptext.count();
	uint maxchars = item->MaxChars;
	while (item->NextBox != 0) {
		item = item->NextBox;
		chars += item->Ptext.count();
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

PyObject *scribus_setpdfbookmark(PyObject */*self*/, PyObject* args)
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
	if (i->PType != FRAME_TEXT)
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
		i->isAnnotation = false;
		Carrier->AddBookMark(i);
	}
	else
		Carrier->DelBookMark(i);
	i->isBookmark = toggle;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_ispdfbookmark(PyObject */*self*/, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == NULL)
		return NULL;
	if (i->PType != FRAME_TEXT)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't get info from a non-text frame", "python error"));
		return NULL;
	}
	if (i->isBookmark)
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}
