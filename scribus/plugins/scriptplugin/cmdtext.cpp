#include "cmdvar.h"
#include "cmdutil.h"
#include "cmdtext.h"

PyObject *scribus_getfontsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
		if (it->HasSel)
			{
			for (uint b = 0; b < it->Ptext.count(); b++)
				{
				if (it->Ptext.at(b)->cselect)
					return PyInt_FromLong(static_cast<long>(it->Ptext.at(b)->csize));
				}
			}
		else
			return PyInt_FromLong(static_cast<long>(it->ISize));
		}
	return PyInt_FromLong(0L);
}

PyObject *scribus_getfont(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
		if (it->HasSel)
			{
			for (uint b = 0; b < it->Ptext.count(); b++)
				{
				if (it->Ptext.at(b)->cselect)
					return PyString_FromString(it->Ptext.at(b)->cfont);
				}
			}
		else
			return PyString_FromString(it->IFont);
		}
	return PyString_FromString("");
}

PyObject *scribus_gettextsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	if (i != -1)
		return PyInt_FromLong(static_cast<long>(doc->ActPage->Items.at(i)->Ptext.count()));
	else
		return PyInt_FromLong(0L);
}

PyObject *scribus_getlinespace(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyFloat_FromDouble(0.0);
	i = GetItem(QString(Name));
	if (i != -1)
		return PyFloat_FromDouble(static_cast<double>(doc->ActPage->Items.at(i)->LineSp));
	else
		return PyFloat_FromDouble(0.0);
}

PyObject *scribus_gettext(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	QString text = "";
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
		for (uint a = 0; a < it->Ptext.count(); a++)
			{
			if (it->HasSel)
				{
				if (it->Ptext.at(a)->cselect)
					text += it->Ptext.at(a)->ch;
				}
			else
				text += it->Ptext.at(a)->ch;
			}
		return PyString_FromString(text);
		}
	else
		return PyString_FromString("");
}

PyObject *scribus_setboxtext(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Text;
	int i;
	PageItem *it;
	PageItem *nb;
	struct Pti *hg;
	if (!PyArg_ParseTuple(args, "s|s", &Text, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	QString Daten = QString(Text);
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
		if (it->NextBox != 0)
			{
			nb = it->NextBox;
			while (nb != 0)
				{
				nb->Ptext.clear();
				nb->CPos = 0;
				nb->Dirty = true;
				nb = nb->NextBox;
				}
			}
		it->Ptext.clear();
		for (uint a = 0; a < Daten.length(); a++)
			{
			hg = new Pti;
			hg->ch = Daten.at(a);
			if (hg->ch == QChar(10)) { hg->ch = QChar(13); }
			if (hg->ch == QChar(9)) { hg->ch = " "; }
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
 			hg->cab = doc->CurrentABStil;
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
	int i, pos;
	PageItem *it;
	struct Pti *hg;
	if (!PyArg_ParseTuple(args, "si|s", &Text, &pos, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	QString Daten = QString(Text);
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
		if ((pos < 0) && (pos > static_cast<int>(it->Ptext.count())))
			return Py_None;
		for (uint a = 0; a < Daten.length(); a++)
			{
			hg = new Pti;
			hg->ch = Daten.at(Daten.length()-1-a);
			if (hg->ch == QChar(10)) { hg->ch = QChar(13); }
			if (hg->ch == QChar(9)) { hg->ch = " "; }
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
 			hg->cab = doc->CurrentABStil;
			hg->xp = 0;
			hg->yp = 0;
			hg->PRot = 0;
			hg->PtransX = 0;
			hg->PtransY = 0;
			it->Ptext.insert(pos, hg);
			}
		it->CPos = pos + Daten.length();
 		it->Dirty = true;
 		it->paintObj();
		}
	return Py_None;
}

PyObject *scribus_setalign(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i, size, Apm;
	if (!PyArg_ParseTuple(args, "i|s", &size, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	if ((size > 3) || (size < 0))
		return Py_None;
	if ((i != -1) && (doc->ActPage->Items.at(i)->PType == 4))
		{
		Apm = doc->AppMode;
		doc->ActPage->SelItem.clear();
		doc->ActPage->SelItem.append(doc->ActPage->Items.at(i));
		if (doc->ActPage->Items.at(i)->HasSel)
			doc->AppMode = 7;
		Carrier->setNewAbStyle(size);
		doc->AppMode = Apm;
		doc->ActPage->Deselect();
		}
	return Py_None;
}

PyObject *scribus_setfontsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i, size, Apm;
	if (!PyArg_ParseTuple(args, "i|s", &size, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	if ((size > 512) || (size < 1))
		return Py_None;
	if ((i != -1) && (doc->ActPage->Items.at(i)->PType == 4))
		{
		Apm = doc->AppMode;
		doc->ActPage->SelItem.clear();
		doc->ActPage->SelItem.append(doc->ActPage->Items.at(i));
		if (doc->ActPage->Items.at(i)->HasSel)
			doc->AppMode = 7;
		doc->ActPage->chFSize(size);
		doc->AppMode = Apm;
		doc->ActPage->Deselect();
		}
	return Py_None;
}

PyObject *scribus_setfont(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Font = "";
	int i, Apm;
	if (!PyArg_ParseTuple(args, "s|s", &Font, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	if ((i != -1) && (doc->ActPage->Items.at(i)->PType == 4))
		{
		if (Carrier->Prefs.AvailFonts.find(QString(Font)))
			{
			Apm = doc->AppMode;
			doc->ActPage->SelItem.clear();
			doc->ActPage->SelItem.append(doc->ActPage->Items.at(i));
			if (doc->ActPage->Items.at(i)->HasSel)
				doc->AppMode = 7;
			Carrier->SetNewFont(QString(Font));
			doc->AppMode = Apm;
			doc->ActPage->Deselect();
			}
		}
	return Py_None;
}

PyObject *scribus_setlinespace(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	int i;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (w < 0.1))
		return Py_None;
	i = GetItem(QString(Name));
	if (i != -1)
		doc->ActPage->Items.at(i)->LineSp = w;
	return Py_None;
}

PyObject *scribus_selecttext(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i, start, ende;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "ii|s", &start, &ende, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
		if ((start < 0) || ((start + ende) > static_cast<int>(it->Ptext.count()-1)))
			return Py_None;
		for (uint a = 0; a < it->Ptext.count(); a++)
			{
			it->Ptext.at(a)->cselect = false;
			}
		if (ende == 0)
			{
			it->HasSel = false;
			return Py_None;
			}
		for (int aa = start; aa < (start + ende); aa++)
			{
			it->Ptext.at(aa)->cselect = true;
			}
		it->HasSel = true;
		return Py_None;
		}
	else
		return Py_None;
}

PyObject *scribus_deletetext(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
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
	int i;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
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
		}
	return Py_None;
}

PyObject *scribus_settextstroke(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	int i;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
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
		}
	return Py_None;
}

PyObject *scribus_settextshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i, w;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || ((w < 0) || (w > 100)))
		return Py_None;
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = doc->ActPage->Items.at(i);
		if ((it->PType == 4) || (it->PType == 8))
			{
			for (uint b = 0; b < it->Ptext.count(); b++)
				{
				if (it->HasSel)
					{
					if (it->Ptext.at(b)->cselect)
						it->Ptext.at(b)->cshade = w;
					}
				else
					it->Ptext.at(b)->cshade = w;
				}
			}
		it->ShTxtFill = w;
		}
	return Py_None;
}

