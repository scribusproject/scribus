#include <Python.h>
#include "cmdtext.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_getfontsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetFontSize([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyFloat_FromDouble(0.0);
	PageItem *it = GetUniqueItem(QString(Name));
	if (it != NULL)
	{
		if (it->HasSel)
		{
			for (uint b = 0; b < it->Ptext.count(); b++)
				if (it->Ptext.at(b)->cselect)
					return PyFloat_FromDouble(static_cast<double>(it->Ptext.at(b)->csize / 10.0));
		}
		else
			return PyFloat_FromDouble(static_cast<long>(it->ISize / 10.0));
	}
	return PyFloat_FromDouble(0.0);
}

PyObject *scribus_getfont(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetFont([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	PageItem *it = GetUniqueItem(QString(Name));
	if (it != NULL)
	{
		if (it->HasSel)
		{
			for (uint b = 0; b < it->Ptext.count(); b++)
				if (it->Ptext.at(b)->cselect)
					return PyString_FromString(it->Ptext.at(b)->cfont);
		}
		else
			return PyString_FromString(it->IFont);
	}
	return PyString_FromString("");
}

PyObject *scribus_gettextsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetTextLength([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->Ptext.count())) : PyInt_FromLong(0L);
}

PyObject *scribus_getcolumns(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetColumns([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->Cols)) : PyInt_FromLong(0L);
}

PyObject *scribus_getlinespace(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetLineSpacing([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyFloat_FromDouble(0.0);
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->LineSp)) : PyFloat_FromDouble(0.0);
}

PyObject *scribus_getcolumngap(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetColumnGap([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyFloat_FromDouble(0.0);
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->ColGap)) : PyFloat_FromDouble(0.0);
}

PyObject *scribus_getframetext(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetText([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	QString text = "";
	PageItem *it = GetUniqueItem(QString(Name));
	if (it != NULL)
	{
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
		qDebug(text);
		return PyString_FromString(text);
	}
	else
		return PyString_FromString("");
}

PyObject *scribus_gettext(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetAllText([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	QString text = "";
	PageItem *it = GetUniqueItem(QString(Name));
	PageItem *is;
	if (it != NULL)
	{
		while (it->BackBox != 0)
		{
			is = GetUniqueItem(it->BackBox->AnName);
			it = is;
		} // while
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
		while (it->NextBox != 0)
		{
			is = GetUniqueItem(it->NextBox->AnName);
			it = is;
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
	else
		return PyString_FromString("");
}

PyObject *scribus_setboxtext(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Text;
	if (!PyArg_ParseTuple(args, "s|s", &Text, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetTExt(text [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *it = GetUniqueItem(QString(Name));
	QString Daten = QString(Text);
	if (it != NULL)
	{
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
	}
	return Py_None;
}

PyObject *scribus_inserttext(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Text;
	int pos;
	if (!PyArg_ParseTuple(args, "si|s", &Text, &pos, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("InsertText(text, position [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *it = GetUniqueItem(QString(Name));
	QString Daten = QString(Text);
	if (it != NULL)
	{
		if ((pos < 0) && (pos > static_cast<int>(it->Ptext.count())))
			return Py_None;
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
	}
	return Py_None;
}

PyObject *scribus_setalign(PyObject *self, PyObject* args)
{
	char *Name = "";
	int size;
	if (!PyArg_ParseTuple(args, "i|s", &size, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetTextAlignment(type [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if ((size > 3) || (size < 0))
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if ((i != NULL) && (i->PType == 4))
	{
		int Apm = Carrier->doc->AppMode;
		i->OwnPage->SelItem.clear();
		i->OwnPage->SelItem.append(i);
		if (i->HasSel)
			Carrier->doc->AppMode = 7;
		Carrier->setNewAbStyle(size);
		Carrier->doc->AppMode = Apm;
		i->OwnPage->Deselect();
	}
	return Py_None;
}

PyObject *scribus_setfontsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	double size;
	if (!PyArg_ParseTuple(args, "d|s", &size, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetFontSize(size [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if ((size > 512) || (size < 1))
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if ((i != NULL) && (i->PType == 4))
	{
		int Apm = Carrier->doc->AppMode;
		i->OwnPage->SelItem.clear();
		i->OwnPage->SelItem.append(i);
		if (i->HasSel)
			Carrier->doc->AppMode = 7;
		i->OwnPage->chFSize(qRound(size * 10.0));
		Carrier->doc->AppMode = Apm;
		i->OwnPage->Deselect();
	}
	return Py_None;
}

PyObject *scribus_setfont(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Font = "";
	if (!PyArg_ParseTuple(args, "s|s", &Font, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetFont(font [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if ((i != NULL) && (i->PType == 4) && (Carrier->Prefs.AvailFonts.find(QString(Font))))
	{
		int Apm = Carrier->doc->AppMode;
		i->OwnPage->SelItem.clear();
		i->OwnPage->SelItem.append(i);
		if (i->HasSel)
			Carrier->doc->AppMode = 7;
		Carrier->SetNewFont(QString(Font));
		Carrier->doc->AppMode = Apm;
		i->OwnPage->Deselect();
	}
	return Py_None;
}

PyObject *scribus_setlinespace(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetLineSpacing(spacing [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (w < 0.1))
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->LineSp = w;
	return Py_None;
}

PyObject *scribus_setcolumngap(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetColumnGap(gap [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (w < 0.0))
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->ColGap = w;
	return Py_None;
}

PyObject *scribus_setcolumns(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetColumns(columns [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (w < 1))
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->Cols = w;
	return Py_None;
}

PyObject *scribus_selecttext(PyObject *self, PyObject* args)
{
	char *Name = "";
	int start, ende;
	if (!PyArg_ParseTuple(args, "ii|s", &start, &ende, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SelectText(start, end [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it != NULL)
	{
		if ((start < 0) || ((start + ende) > static_cast<int>(it->Ptext.count()-1)))
			return Py_None;
		for (uint a = 0; a < it->Ptext.count(); ++a)
			it->Ptext.at(a)->cselect = false;
		if (ende == 0)
		{
			it->HasSel = false;
			return Py_None;
		}
		for (int aa = start; aa < (start + ende); ++aa)
			it->Ptext.at(aa)->cselect = true;
		it->HasSel = true;
		return Py_None;
	}
	else
		return Py_None;
}

PyObject *scribus_deletetext(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("DeleteText([objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it != NULL)
	{
		if (it->HasSel)
			Carrier->DeleteSel(it);
		else
		{
			it->Ptext.clear();
			it->CPos = 0;
		}
	}
	return Py_None;
}

PyObject *scribus_settextfill(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetTextColor(color [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *it = GetUniqueItem(QString(Name));
	if ((it != NULL) && (it->PType == 4) || (it->PType == 8))
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
	} // if
	return Py_None;
}

PyObject *scribus_settextstroke(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetTextStroke(color [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *it = GetUniqueItem(QString(Name));
	if ((it != NULL) && (it->PType == 4) || (it->PType == 8))
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
	} // if
	return Py_None;
}

PyObject *scribus_settextshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SetTextShade(type [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || ((w < 0) || (w > 100)))
		return Py_None;
	PageItem *it = GetUniqueItem(QString(Name));
	if ((it != NULL) && (it->PType == 4) || (it->PType == 8))
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
	return Py_None;
}

PyObject *scribus_linktextframes(PyObject *self, PyObject* args)
{
	char *name1;
	char *name2;

	if (!PyArg_ParseTuple(args, "ss", &name1, &name2))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("LinkTextFrames(objectname, objectname)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item1 = GetUniqueItem(QString(name1));
	if (!item1)
		return Py_None;
	PageItem *item2 = GetUniqueItem(QString(name2));
	if (!item2)
		return Py_None;
	/* only empty textframe, only not linked and selfhate :) */
	if ((item2->Ptext.count() == 0) && (item2->NextBox == 0)
	        && (item2->BackBox == 0) && (item1 != item2))
	{
		// references to the others boxes
		item1->NextBox = item2;
		item2->BackBox = item1;
		item1->OwnPage->repaint();
		item2->OwnPage->repaint();
		// enable 'save icon' stuff
		Carrier->slotDocCh();
	} // if empty
	return Py_None;
}

PyObject *scribus_unlinktextframes(PyObject * self, PyObject* args)
{
	char *name;
	if (!PyArg_ParseTuple(args, "s", &name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("UnlinkTextFrames(objectname)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(name);
	if (!item)
		return Py_None;
	// only linked
	if (item->BackBox != 0)
	{
		// is not last one.
		if (item->NextBox != 0)
		{
			PageItem* nextbox = item->NextBox;
			while (nextbox != 0)
			{
				uint a = nextbox->Ptext.count();
				for (uint s=0; s<a; ++s)
					item->Ptext.append(nextbox->Ptext.take(0));
				nextbox = nextbox->NextBox;
			} // while
		} // if nextBox
		uint a2 = item->Ptext.count();
		for (uint s = 0; s < a2; ++s)
			item->BackBox->Ptext.append(item->Ptext.take(0));
		item->BackBox->NextBox = 0;
		item->BackBox = 0;
	} // if
	// enable 'save icon' stuff
	Carrier->slotDocCh();
	item->OwnPage->repaint();
	return Py_None;
}

