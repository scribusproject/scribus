#include "cmdtext.h"
#include "cmdutil.h"

PyObject *scribus_getfontsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	if (it->HasSel)
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
			if (it->Ptext.at(b)->cselect)
				return PyFloat_FromDouble(static_cast<double>(it->Ptext.at(b)->csize / 10.0));
	}
	else
		return PyFloat_FromDouble(static_cast<long>(it->ISize / 10.0));
	//FIXME: How can we reach this point? Should we raise an exception instead of returning zero?
	return PyFloat_FromDouble(0.0);
}

PyObject *scribus_getfont(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	if (it->HasSel)
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
			if (it->Ptext.at(b)->cselect)
				return PyString_FromString(it->Ptext.at(b)->cfont);
	}
	else
		return PyString_FromString(it->IFont);
	//FIXME: How do we reach this point? Should we be raising an exception instead of returning ""?
	return PyString_FromString("");
}

PyObject *scribus_gettextsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->Ptext.count())) : NULL;
}

PyObject *scribus_getcolumns(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->Cols)) : NULL;
}

PyObject *scribus_getlinespace(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->LineSp)) : NULL;
}

PyObject *scribus_getcolumngap(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->ColGap)) : NULL;
}

PyObject *scribus_getframetext(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	QString text = "";
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
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
	//qDebug(text);		disabled by cr 2004-11-11
	return PyString_FromString(text);
}

PyObject *scribus_gettext(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	QString text = "";
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	PageItem *is = NULL;
	// Scan backwards to find the first frame in a linked series
	while (it->BackBox != 0)
	{
		is = GetUniqueItem(it->BackBox->AnName);
		if (is == NULL)
		{
			// While GetUniqueItem has already set an exception, we'll
			// overwrite that with a more suitable one for this particular case.
			PyErr_SetString(PyExc_RuntimeError, QString("(System Error) Broken linked frame series when scanning back"));
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
	return PyString_FromString(text);
}

PyObject *scribus_setboxtext(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Text;
	if (!PyArg_ParseTuple(args, "es|s", "utf-8", &Text, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
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

PyObject *scribus_inserttext(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Text;
	int pos;
	if (!PyArg_ParseTuple(args, "esi|s", "utf-8", &Text, &pos, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	QString Daten = QString::fromUtf8(Text);
	PyMem_Free(Text);
	if ((pos < 0) && (pos > static_cast<int>(it->Ptext.count())))
	{
		PyErr_SetString(PyExc_IndexError, QString("Insert index out of bounds"));
		return NULL;
	}
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

PyObject *scribus_setalign(PyObject *self, PyObject* args)
{
	char *Name = "";
	int alignment;
	if (!PyArg_ParseTuple(args, "i|s", &alignment, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((alignment > 3) || (alignment < 0))
	{
		PyErr_SetString(PyExc_ValueError, QString("Alignment out of range. Use one of the scribus.ALIGN* constants."));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	if (i->PType == 4)
	{
		PyErr_SetString(ScribusException, QString("Can't set text alignment on a non-text frame"));
		return NULL;
	}
	int Apm = Carrier->doc->AppMode;
	Carrier->view->SelItem.clear();
	Carrier->view->SelItem.append(i);
	if (i->HasSel)
		Carrier->doc->AppMode = 7;
	Carrier->setNewAbStyle(alignment);
	Carrier->doc->AppMode = Apm;
	Carrier->view->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfontsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	double size;
	if (!PyArg_ParseTuple(args, "d|s", &size, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((size > 512) || (size < 1))
	{
		PyErr_SetString(ScribusException, QString("Font size out of bounds - must be 1 <= size <= 512"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	
	if (i->PType == 4)
	{
		PyErr_SetString(ScribusException, QString("Can't set text alignment on a non-text frame"));
		return NULL;
	}
	int Apm = Carrier->doc->AppMode;
	Carrier->view->SelItem.clear();
	Carrier->view->SelItem.append(i);
	if (i->HasSel)
		Carrier->doc->AppMode = 7;
	Carrier->view->chFSize(qRound(size * 10.0));
	Carrier->doc->AppMode = Apm;
	Carrier->view->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfont(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Font = "";
	if (!PyArg_ParseTuple(args, "s|s", &Font, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	if (i->PType != 4)
	{
		PyErr_SetString(ScribusException, QString("Can't set font on a non-text frame"));
		return NULL;
	}
	if (Carrier->Prefs.AvailFonts.find(QString(Font)))
	{
		int Apm = Carrier->doc->AppMode;
		Carrier->view->SelItem.clear();
		Carrier->view->SelItem.append(i);
		if (i->HasSel)
			Carrier->doc->AppMode = 7;
		Carrier->SetNewFont(QString(Font));
		Carrier->doc->AppMode = Apm;
		Carrier->view->Deselect();
	}
	else
	{
		PyErr_SetString(ScribusException, QString("Font not found"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinespace(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 0.1)
	{
		PyErr_SetString(PyExc_ValueError, QString("Line space out of bounds, must be >= 0.1"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->LineSp = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setcolumngap(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QString("Column gap out of bounds, must be positive"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->ColGap = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setcolumns(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 1)
	{
		PyErr_SetString(PyExc_ValueError, QString("Column count out of bounds, must be > 1"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->Cols = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_selecttext(PyObject *self, PyObject* args)
{
	char *Name = "";
	int start, ende;
	if (!PyArg_ParseTuple(args, "ii|s", &start, &ende, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	if ((start < 0) || ((start + ende) > static_cast<int>(it->Ptext.count()-1)))
	{
		PyErr_SetString(PyExc_IndexError, QString("Selection index out of bounds"));
		return NULL;
	}
	/* FIXME: not sure if we should make this check or not
	if (start > ende)
	{
		PyErr_SetString(PyExc_ValueError, QString("Selection start > selection end"));
		return NULL;
	}
	*/
	for (uint a = 0; a < it->Ptext.count(); ++a)
		it->Ptext.at(a)->cselect = false;
	if (ende == 0)
	{
		it->HasSel = false;
		Py_INCREF(Py_None);
		return Py_None;
	}
	for (int aa = start; aa < (start + ende); ++aa)
		it->Ptext.at(aa)->cselect = true;
	it->HasSel = true;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_deletetext(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
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

PyObject *scribus_settextfill(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	if ((it->PType == 4) || (it->PType == 8))
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
		{
			if (it->HasSel)
			{
				if (it->Ptext.at(b)->cselect)
					it->Ptext.at(b)->ccolor = QString(Color);
			}
			else
				it->Ptext.at(b)->ccolor = QString(Color);
		}
		it->TxtFill = QString(Color);
	}
	else
	{
		PyErr_SetString(ScribusException, QString("Wrong frame type, can't set text color"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_settextstroke(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	if ((it->PType == 4) || (it->PType == 8))
	{
		for (uint b = 0; b < it->Ptext.count(); b++)
		{
			if (it->HasSel)
			{
				if (it->Ptext.at(b)->cselect)
					it->Ptext.at(b)->cstroke = QString(Color);
			}
			else
				it->Ptext.at(b)->cstroke = QString(Color);
		}
		it->TxtStroke = QString(Color);
	}
	else
	{
		PyErr_SetString(ScribusException, QString("Wrong frame type, can't set text color"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_settextshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	if ((it->PType == 4) || (it->PType == 8))
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
	else
	{
		PyErr_SetString(ScribusException, QString("Wrong frame type, can't set text shade"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_linktextframes(PyObject *self, PyObject* args)
{
	char *name1;
	char *name2;

	if (!PyArg_ParseTuple(args, "ss", &name1, &name2))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *fromitem = GetUniqueItem(QString(name1));
	if (fromitem == NULL)
		return NULL;
	PageItem *toitem = GetUniqueItem(QString(name2));
	if (toitem == NULL)
		return NULL;
	if (toitem->Ptext.count())
	{
		PyErr_SetString(ScribusException, QString("Target frame must be empty"));
		return NULL;
	}
	if (toitem->NextBox != 0)
	{
		PyErr_SetString(ScribusException, QString("Target frame links to another frame"));
		return NULL;
	}
	if (toitem->BackBox != 0)
	{
		PyErr_SetString(ScribusException, QString("Target frame is linked to by another frame"));
		return NULL;
	}
	if (toitem == fromitem)
	{
		PyErr_SetString(ScribusException, QString("Source and target are the same object"));
		return NULL;
	}
	// references to the others boxes
	fromitem->NextBox = toitem;
	toitem->BackBox = fromitem;
	Carrier->view->DrawNew();
	// enable 'save icon' stuff
	Carrier->slotDocCh();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_unlinktextframes(PyObject * self, PyObject* args)
{
	char *name;
	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(name);
	if (item == NULL)
		return NULL;
	// only linked
	if (item->BackBox == 0)
	{
		PyErr_SetString(ScribusException, QString("Object is not a linked text frame, can't unlink."));
		return NULL;
	}
	if (item->NextBox == 0)
	{
		PyErr_SetString(ScribusException, QString("Object the last frame in a series, can't unlink."));
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
	Carrier->view->DrawNew();
	Py_INCREF(Py_None);
	return Py_None;
}

/*
 * Convert the selected text frame to outlines.
 *
 * 2004-09-07 (Craig Ringer)
 * 2004-09-14 pv frame type, optional frame name param
 */
PyObject *scribus_tracetext(PyObject *self, PyObject* args)
{
	char *name = "";
	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(name));
	if (item == NULL)
		return NULL;
	if (item->PType == 4)
	{
		PyErr_SetString(ScribusException, QString("Cannot convert a non-text frame to outlines"));
		return NULL;
	}
	Carrier->view->Deselect(true);
	Carrier->view->SelectItemNr(item->ItemNr);
	Carrier->view->TextToPath();
	/* FIXME: this won't work. need to know why. maybe later...
	Carrier->view->Deselect(true);
	Carrier->view->SelectItemNr(item->ItemNr);
	Carrier->view->TextToPath(); */
	Py_INCREF(Py_None);
	return Py_None;
}
