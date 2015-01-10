/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "objpdffile.h"
#include "cmdutil.h"
#include "colormgmt/sccolormgmtengine.h"
#include "bookmarkpalette.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "scribuscore.h"

#include <structmember.h>
#include <QFileInfo>
#include <QList>
#include <QPixmap>
#include <vector>

// these functions are located at utils.cpp
void SCRIBUS_API ReOrderText(ScribusDoc *doc, ScribusView *view);
// end of utils.cpp
//this is in file scribus.cpp


static int minmaxi(int val, int min, int max)
{
	if (val < min)
		return min;
	else if (val > max)
		return max;
	else return val;
}

static double minmaxd(double val, double min, double max)
{
	if (val < min)
		return min;
	else if (val > max)
		return max;
	else return val;
}

typedef struct
{
	PyObject_HEAD
	PyObject *file; // string - file to save into
	PyObject *fonts; // list of string - fonts to  embed
	PyObject *SubsetList; // list of string - fonts to outline
	PyObject *pages; // list of int - pages to print
	int thumbnails; // bool -
	int cropMarks; // bool -
	int bleedMarks; // bool -
	int registrationMarks; // bool -
	int colorMarks; // bool -
	int docInfoMarks; // bool -
	double markOffset; // double -
	int compress; // bool -
	int compressmtd; // int - 0=automatic 1=jpeg 2=zip 3=none
	int quality; // int - 0=Maximum 4=minimum
	PyObject *resolution; // int - 35 - 4000 default=300 dpi
	PyObject *downsample; // int - 35 - 4000 default=0 do not downsample ; other downsample to this resolution
	int bookmarks; // bool -
	int binding; // bool - 0 -left margin 1 -right margin
	int presentation; // bool -
	PyObject *effval; // list of list of 5 int - effect to apply to each page during presentation
	int article; // bool -
	int encrypt; // bool -
	int uselpi; // bool -
	int usespot;
	int domulti;
	PyObject *lpival; // list of elements which has structure [siii]
	PyObject *owner; // string - owner's password
	PyObject *user; // string - user's password
	int aprint; // bool -  allow printing
	int achange; // bool - allow changing
	int acopy; // bool - allow copying
	int aanot; // bool - allow adding annotation and fields
	int version; // int - version of pdf (12=1.2; 13=1.3; 14=1.4)
	int outdst; // int - output destination 0 - screen, 1 - printer

	int profiles; // bool
	int profilei; // bool
	int intents; // int - 0 - 3 QString tmp_ip[] = { tr("Perceptual"), tr("Relative Colorimetric"), tr("Saturation"), tr("Absolute Colorimetric")};
	int intenti; // int - 0 - 3 QString tmp_ip[] = { tr("Perceptual"), tr("Relative Colorimetric"), tr("Saturation"), tr("Absolute Colorimetric")};
	int noembicc; // bool - "Don't use embedded ICC profiles"
	PyObject *solidpr; // string
	PyObject *imagepr; // string
	PyObject *printprofc; // string
	PyObject *info; // string
	double bleedt; // double - 0 to hight of page
	double bleedl; // double - 0 to width of page
	double bleedr; // double - 0 to width of page
	double bleedb; // double - 0 to hight of page
	int usedocbleeds; // bool
	int useLayers;
	int embedPDF;
	int mirrorH;
	int mirrorV;
	int doClip;
	PyObject * RotateDeg; // int
	int isGrayscale;
	int pageLayout;
	int displayBookmarks;
	int displayThumbs;
	int displayLayers;
	int displayFullscreen;
	int hideToolBar;
	int hideMenuBar;
	int fitWindow;
	PyObject *openAction;

} PDFfile;

static void PDFfile_dealloc(PDFfile *self)
{
	Py_XDECREF(self->file);
	Py_XDECREF(self->fonts);
	Py_XDECREF(self->SubsetList);
	Py_XDECREF(self->pages);
	Py_XDECREF(self->resolution);
	Py_XDECREF(self->downsample);
	Py_XDECREF(self->effval);
	Py_XDECREF(self->lpival);
	Py_XDECREF(self->owner);
	Py_XDECREF(self->user);
	Py_XDECREF(self->solidpr);
	Py_XDECREF(self->imagepr);
	Py_XDECREF(self->printprofc);
	Py_XDECREF(self->info);
	Py_XDECREF(self->RotateDeg);
	Py_XDECREF(self->openAction);
	self->ob_type->tp_free((PyObject *)self);
}

static PyObject * PDFfile_new(PyTypeObject *type, PyObject * /*args*/, PyObject * /*kwds*/)
{
// do not create new object if there is no opened document
	if (!ScCore->primaryMainWindow()->HaveDoc) {
		PyErr_SetString(PyExc_SystemError, "Need to open document first");
		return NULL;
	}

	PDFfile *self;

	self = (PDFfile *)type->tp_alloc(type, 0);
	if (self) {
// set file attribute
		self->file = PyString_FromString("");
		if (!self->file){
			Py_DECREF(self);
			return NULL;
		}
// set fonts attribute
		self->fonts = PyList_New(0);
		if (!self->fonts){
			Py_DECREF(self);
			return NULL;
		}
		self->SubsetList = PyList_New(0);
		if (!self->SubsetList){
			Py_DECREF(self);
			return NULL;
		}
// set pages attribute
		self->pages = PyList_New(0);
		if (self->pages == NULL){
			Py_DECREF(self);
			return NULL;
		}
// set thumbnails attribute
		self->thumbnails = 0;
// set cropMarks attribute
		self->cropMarks = 0;
// set bleedMarks attribute
		self->bleedMarks = 0;
// set registrationMarks attribute
		self->registrationMarks = 0;
// set colorMarks attribute
		self->colorMarks = 0;
// set docInfoMarks attribute
		self->docInfoMarks = 0;
// set mark offset attribute
		self->markOffset = 0;
// set compress attribute
		self->compress = 0;
// set compressmtd attribute
		self->compressmtd = 0;
// set quality attribute
		self->quality = 0;
// set resolution attribute
		self->resolution = PyInt_FromLong(300);
		if (!self->resolution){
			Py_DECREF(self);
			return NULL;
		}
// set downsample attribute
		self->downsample = PyInt_FromLong(0);
		if (!self->downsample){
			Py_DECREF(self);
			return NULL;
		}
// set bookmarks attribute
		self->bookmarks = 0;
// set binding attribute
		self->binding = 0;
// set presentation attribute
		self->presentation = 0;
// set effval attribute
		self->effval = PyList_New(0);
		if (!self->effval){
			Py_DECREF(self);
			return NULL;
		}
// set article attribute
		self->article = 0;
// set encrypt attribute
		self->encrypt = 0;
// set uselpi attribute
		self->uselpi = 0;
		self->usespot = 1;
		self->domulti = 0;
// set lpival attribute
		self->lpival = PyList_New(0);
		if (!self->lpival){
			Py_DECREF(self);
			return NULL;
		}
// set owner attribute
		self->owner = PyString_FromString("");
		if (!self->owner){
			Py_DECREF(self);
			return NULL;
		}
// set user attribute
		self->user = PyString_FromString("");
		if (!self->user){
			Py_DECREF(self);
			return NULL;
		}
// set aprint attribute
		self->aprint = 1;
// set achange attribute
		self->achange = 1;
// set acopy attribute
		self->acopy = 1;
// set aanot attribute
		self->aanot = 1;
// set version attribute
		self->version = 14;
// set output attribute
		self->outdst = 0;


		self->profiles = 0; // bool
		self->profilei = 0; // bool
		self->intents = 0; // int - 0 - ?
		self->intenti = 0; // int - 0 - ?
		self->noembicc = 0; // bool
		self->solidpr = PyString_FromString("");
		if (!self->solidpr){
			Py_DECREF(self);
			return NULL;
		}
		self->imagepr = PyString_FromString("");
		if (!self->imagepr){
			Py_DECREF(self);
			return NULL;
		}
		self->printprofc = PyString_FromString("");
		if (!self->printprofc){
			Py_DECREF(self);
			return NULL;
		}
		self->info = PyString_FromString("");
		if (!self->info){
			Py_DECREF(self);
			return NULL;
		}
		self->bleedt = 0; // double -
		self->bleedl = 0; // double -
		self->bleedr = 0; // double -
		self->bleedb = 0; // double -
		self->usedocbleeds = 0; // bool
		self->useLayers = 0;
		self->embedPDF = 0;
		self->mirrorH = 0;
		self->mirrorV = 0;
		self->doClip = 0;
		self->RotateDeg = PyInt_FromLong(0);
		if (!self->RotateDeg){
			Py_DECREF(self);
			return NULL;
		}
		self->isGrayscale = 0;
		self->pageLayout = 0;
		self->displayBookmarks = 0;
		self->displayThumbs = 0;
		self->displayLayers = 0;
		self->displayFullscreen = 0;
		self->hideToolBar = 0;
		self->hideMenuBar = 0;
		self->fitWindow = 0;
		self->openAction = PyString_FromString("");
		if (!self->openAction){
			Py_DECREF(self);
			return NULL;
		}
	}
	return (PyObject *) self;
}

static int PDFfile_init(PDFfile *self, PyObject * /*args*/, PyObject * /*kwds*/)
{
	int i;
	if (!ScCore->primaryMainWindow()->HaveDoc) {
		PyErr_SetString(PyExc_SystemError, "Must open doc first");
		return -1;
	}

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	PDFOptions& pdfOptions = currentDoc->PDF_Options;

// defaut save into file
	QString tf = pdfOptions.fileName;
	if (tf.isEmpty()) {
		QFileInfo fi = QFileInfo(currentDoc->DocName);
		tf = fi.path()+"/"+fi.baseName()+".pdf";
	}
	PyObject *file = NULL;
	file = PyString_FromString(tf.toAscii());
	if (file){
		Py_DECREF(self->file);
		self->file = file;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'file' attribute");
		return -1;
	}
// embed all used fonts
	PyObject *fonts = NULL;
	fonts = PyList_New(0);
	if (fonts){
		Py_DECREF(self->fonts);
		self->fonts = fonts;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'fonts' attribute");
		return -1;
	}
	// get all used fonts
	QMap<QString,int> ReallyUsed = currentDoc->UsedFonts;
	// create list of all used fonts
	QList<QString> tmpEm;
	tmpEm = ReallyUsed.keys();
	QList<QString>::Iterator itef;
	for (itef = tmpEm.begin(); itef != tmpEm.end(); ++itef) {
// AV: dunno what this is for, but it looks as if it's the only place where HasMetrics is used...
//		if (PrefsManager::instance()->appPrefs.AvailFonts[(*itef).toAscii()]->HasMetrics) {
			PyObject *tmp= NULL;
			tmp = PyString_FromString((*itef).toAscii());
			if (tmp) {
				PyList_Append(self->fonts, tmp);
// do i need Py_DECREF(tmp) here?
// Does PyList_Append increase reference or 'steal' one from provided argument
// If it 'steal' reference comment next line
				Py_DECREF(tmp);
			}
			else {
				PyErr_SetString(PyExc_SystemError, "Can not initialize 'fonts' attribute");
				return -1;
			}
//		}
	}
// init SubsetList
	fonts = PyList_New(0);
	if (fonts){
		Py_DECREF(self->SubsetList);
		self->SubsetList = fonts;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'SubsetList' attribute");
		return -1;
	}
// copied from TabPDFOptions::restoreDefaults()
	for (int fe = 0; fe < pdfOptions.SubsetList.count(); ++fe)
	{
		PyObject *tmp= NULL;
		tmp = PyString_FromString(pdfOptions.SubsetList[fe].toLatin1().data());
		if (tmp) {
			PyList_Append(self->SubsetList, tmp);
			Py_DECREF(tmp);
		}
		else {
			PyErr_SetString(PyExc_SystemError, "Can not initialize 'SubsetList' attribute");
			return -1;
		}
	}

// set to print all pages
	PyObject *pages = NULL;
	int num = 0;
	// which one should I use ???
	// new = ScCore->primaryMainWindow()->view->Pages.count()
	num = currentDoc->Pages->count();
	pages = PyList_New(num);
	if (!pages){
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'pages' attribute");
		return -1;
	}
	for (i = 0; i<num; ++i) {
		PyObject *tmp;
		tmp = PyInt_FromLong((long)i+1L);
		if (tmp)
			PyList_SetItem(pages, i, tmp);
		else {
			PyErr_SetString(PyExc_SystemError, "Can not initialize 'pages' attribute");
			return -1;
		}
	}
	Py_DECREF(self->pages);
	self->pages = pages;
// print thumbnails ?
	self->thumbnails = pdfOptions.Thumbnails;
// output crop marks ?
	self->cropMarks = pdfOptions.cropMarks;
// output bleed marks ?
	self->bleedMarks = pdfOptions.bleedMarks;
// output registration marks ?
	self->registrationMarks = pdfOptions.registrationMarks;
// output color bars ?
	self->colorMarks = pdfOptions.colorMarks;
// output doc info marks ?
	self->docInfoMarks = pdfOptions.docInfoMarks;
// cropmarks offset 
	self->markOffset = pdfOptions.markOffset * currentDoc->unitRatio();
// set automatic compression
	self->compress = pdfOptions.Compress;
	self->compressmtd = pdfOptions.CompressMethod;
// use maximum image quality
	self->quality = pdfOptions.Quality;
// default resolution
	PyObject *resolution = NULL;
	resolution = PyInt_FromLong(300);
	if (resolution){
		Py_DECREF(self->resolution);
		self->resolution = resolution;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'resolutin' attribute");
		return -1;
	}
// do not downsample images
	int down = pdfOptions.RecalcPic ? pdfOptions.PicRes : 0;
	PyObject *downsample = NULL;
	downsample = PyInt_FromLong(down);
	if (downsample){
		Py_DECREF(self->downsample);
		self->downsample = downsample;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'downsamle' attribute");
		return -1;
	}
	// no bookmarks
	self->bookmarks = pdfOptions.Bookmarks;
	// left margin binding
	self->binding = pdfOptions.Binding;
	// do not enable presentation effects
	self->presentation = pdfOptions.PresentMode;
	// set effects values for all pages
	PyObject *effval = NULL;
	num = 0;
	// which one should I use ???
	// new = ScCore->primaryMainWindow()->view->Pages.count();
	num = currentDoc->Pages->count();
	effval = PyList_New(num);
	if (!effval){
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'effval' attribute");
		return -1;
	}
	int num2 = pdfOptions.PresentVals.count();
	for (i = 0; i<num2; ++i) {
		PyObject *tmp;
		PDFPresentationData t = pdfOptions.PresentVals[i];
		tmp = Py_BuildValue(const_cast<char*>("[iiiiii]"), t.pageEffectDuration, t.pageViewDuration, t.effectType, t.Dm, t.M, t.Di );
		if (tmp)
			PyList_SetItem(effval, i, tmp);
		else {
			PyErr_SetString(PyExc_SystemError, "Can not initialize 'effval' attribute");
			return -1;
		}
		for (; i<num; ++i) {
			PyObject *tmp;
			tmp = Py_BuildValue(const_cast<char*>("[iiiiii]"), 1, 1, 0, 0, 0, 0);
			if (tmp)
				PyList_SetItem(effval, i, tmp);
			else {
				PyErr_SetString(PyExc_SystemError, "Can not initialize 'effval' attribute");
				return -1;
			}
		}
	}
	Py_DECREF(self->effval);
	self->effval = effval;
// do not save linked text frames as PDF article
	self->article = pdfOptions.Articles;
// do not encrypt file
	self->encrypt = pdfOptions.Encrypt;
// do not Use Custom Rendering Settings
	self->uselpi = pdfOptions.UseLPI;
	self->usespot = pdfOptions.UseSpotColors;
	self->domulti = pdfOptions.doMultiFile;
// get default values for lpival
	int n = pdfOptions.LPISettings.size();
	PyObject *lpival=PyList_New(n);
	if (!lpival){
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'lpival' attribute");
		return -1;
	}
	QMap<QString,LPIData>::Iterator it = pdfOptions.LPISettings.begin();
	while (it != pdfOptions.LPISettings.end()) {
		PyObject *tmp;
		tmp = Py_BuildValue(const_cast<char*>("[siii]"), it.key().toAscii().constData(), it.value().Frequency, it.value().Angle, it.value().SpotFunc);
		if (!tmp) {
			PyErr_SetString(PyExc_SystemError, "Can not initialize 'lpival' attribute");
			return -1;
		}
		PyList_SetItem(lpival, --n, tmp);
		++it;
	}
	PyList_Reverse(lpival);
	Py_DECREF(self->lpival);
	self->lpival = lpival;
// set owner's password
	PyObject *owner = NULL;
	owner = PyString_FromString(pdfOptions.PassOwner.toAscii());
	if (owner){
		Py_DECREF(self->owner);
		self->owner = owner;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'owner' attribute");
		return -1;
	}
// set user'a password
	PyObject *user = NULL;
	user = PyString_FromString(pdfOptions.PassUser.toAscii());
	if (user){
		Py_DECREF(self->user);
		self->user = user;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'user' attribute");
		return -1;
	}
// allow printing document
	self->aprint = pdfOptions.Permissions & 4;
// allow changing document
	self->achange = pdfOptions.Permissions & 8;
// allow copying document
	self->acopy = pdfOptions.Permissions & 16;
// allow adding annotation and fields
	self->aanot = pdfOptions.Permissions & 32;
// use 1.4 pdf version *aka. Acrobat 5)
	self->version = pdfOptions.Version;
// output destination is screen
	self->outdst = pdfOptions.UseRGB ? 0 : 1;

	self->profiles = pdfOptions.UseProfiles; // bool
	self->profilei = pdfOptions.UseProfiles2; // bool
	self->noembicc = pdfOptions.EmbeddedI; // bool
	self->intents = pdfOptions.Intent; // int - 0 - 3
	self->intenti = pdfOptions.Intent2; // int - 0 - 3
	QString tp = pdfOptions.SolidProf;
	if (!ScCore->InputProfiles.contains(tp))
		tp = currentDoc->CMSSettings.DefaultSolidColorRGBProfile;
	PyObject *solidpr = NULL;
	solidpr = PyString_FromString(tp.toAscii());
	if (solidpr){
		Py_DECREF(self->solidpr);
		self->solidpr = solidpr;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'solidpr' attribute");
		return -1;
	}
	QString tp2 = pdfOptions.ImageProf;
	if (!ScCore->InputProfiles.contains(tp2))
		tp2 = currentDoc->CMSSettings.DefaultSolidColorRGBProfile;
	PyObject *imagepr = NULL;
	imagepr = PyString_FromString(tp2.toAscii());
	if (imagepr){
		Py_DECREF(self->imagepr);
		self->imagepr = imagepr;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'imagepr' attribute");
		return -1;
	}
	QString tp3 = pdfOptions.PrintProf;
	if (!ScCore->PDFXProfiles.contains(tp3))
		tp3 = currentDoc->CMSSettings.DefaultPrinterProfile;
	PyObject *printprofc = NULL;
	printprofc = PyString_FromString(tp3.toAscii());
	if (printprofc){
		Py_DECREF(self->printprofc);
		self->printprofc = printprofc;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'printprofc' attribute");
		return -1;
	}
	QString tinfo = pdfOptions.Info;
	PyObject *info = NULL;
	info = PyString_FromString(tinfo.toAscii());
	if (info){
		Py_DECREF(self->info);
		self->info = info;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'info' attribute");
		return -1;
	}
	self->bleedt = pdfOptions.bleeds.Top * currentDoc->unitRatio(); // double -
	self->bleedl = pdfOptions.bleeds.Left * currentDoc->unitRatio(); // double -
	self->bleedr = pdfOptions.bleeds.Right * currentDoc->unitRatio(); // double -
	self->bleedb = pdfOptions.bleeds.Bottom * currentDoc->unitRatio(); // double -
	self->usedocbleeds = 0; // bool
	self->useLayers = pdfOptions.useLayers; // bool
	self->embedPDF = pdfOptions.embedPDF; // bool
	self->mirrorH = pdfOptions.MirrorH; // bool
	self->mirrorV = pdfOptions.MirrorV; // bool
	self->doClip = pdfOptions.doClip; // bool
	PyObject *RotateDeg = NULL;
	RotateDeg = PyInt_FromLong(0);
	if (RotateDeg){
		Py_DECREF(self->RotateDeg);
		self->RotateDeg = RotateDeg;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'RotateDeg' attribute");
		return -1;
	}
	self->isGrayscale = pdfOptions.isGrayscale; // bool
	self->pageLayout = pdfOptions.PageLayout; // int
	self->displayBookmarks = pdfOptions.displayBookmarks; // bool
	self->displayThumbs = pdfOptions.displayThumbs; // bool
	self->displayLayers = pdfOptions.displayLayers; // bool
	self->displayFullscreen = pdfOptions.displayFullscreen; // bool
	self->hideToolBar = pdfOptions.hideToolBar; // bool
	self->hideMenuBar = pdfOptions.hideMenuBar; // bool
	self->fitWindow = pdfOptions.fitWindow; // bool

	PyObject *openAction = NULL;
	openAction = PyString_FromString(pdfOptions.openAction.toLatin1().data());
	if (openAction){
		Py_DECREF(self->openAction);
		self->openAction = openAction;
	} else {
		PyErr_SetString(PyExc_SystemError, "Can not initialize 'openAction' attribute");
		return -1;
	}

	return 0;
}

static PyMemberDef PDFfile_members[] = {
	{const_cast<char*>("thumbnails"), T_INT, offsetof(PDFfile, thumbnails), 0, const_cast<char*>("Generate thumbnails. Bool value.")},
	{const_cast<char*>("cropMarks"), T_INT, offsetof(PDFfile, cropMarks), 0, const_cast<char*>("Create crop marks in the PDF indicating where the paper should be cut or trimmed after printing.")},
	{const_cast<char*>("bleedMarks"), T_INT, offsetof(PDFfile, bleedMarks), 0, const_cast<char*>("Create marks delimiting the bleed area.")},
	{const_cast<char*>("registrationMarks"), T_INT, offsetof(PDFfile, registrationMarks), 0, const_cast<char*>("Add registration marks to each separation.")},
	{const_cast<char*>("colorMarks"), T_INT, offsetof(PDFfile, colorMarks), 0, const_cast<char*>("Add color calibration bars.")},
	{const_cast<char*>("docInfoMarks"), T_INT, offsetof(PDFfile, docInfoMarks), 0, const_cast<char*>("Add document information which includes the document title and page numbers.")},
	{const_cast<char*>("markOffset"), T_DOUBLE, offsetof(PDFfile, markOffset), 0, const_cast<char*>("Indicate the distance offset between mark and page area.")},
	{const_cast<char*>("compress"), T_INT, offsetof(PDFfile, compress), 0, const_cast<char*>("Compression switch. Bool value.")},
	{const_cast<char*>("compressmtd"), T_INT, offsetof(PDFfile, compressmtd), 0, const_cast<char*>("Compression method.\n\t0 - Automatic\n\t1 - JPEG\n\t2 - zip\n\t3 - None.")},
	{const_cast<char*>("quality"), T_INT, offsetof(PDFfile, quality), 0, const_cast<char*>("Image quality\n\t0 - Maximum\n\t1 - High\n\t2 - Medium\n\t3 - Low\n\t4 - Minimum")},
	{const_cast<char*>("bookmarks"), T_INT, offsetof(PDFfile, bookmarks), 0, const_cast<char*>("Embed the bookmarks you created in your document.\nThese are useful for navigating long PDF documents.\nBool value")},
	{const_cast<char*>("binding"), T_INT, offsetof(PDFfile, binding), 0, const_cast<char*>("Choose binding.\n\t0 - Left binding\n\t1 - Right binding")},
	{const_cast<char*>("presentation"), T_INT, offsetof(PDFfile, presentation), 0, const_cast<char*>("Enable Presentation Effects.Bool value")},
	{const_cast<char*>("article"), T_INT, offsetof(PDFfile, article), 0, const_cast<char*>("Save Linked Text Frames as PDF Articles\n\tBool value")},
	{const_cast<char*>("encrypt"), T_INT, offsetof(PDFfile, encrypt), 0, const_cast<char*>("Use Encription. Bool value")},
	{const_cast<char*>("uselpi"), T_INT, offsetof(PDFfile, uselpi), 0, const_cast<char*>("Use Custom Rendering Settings. Bool value")},
	{const_cast<char*>("usespot"), T_INT, offsetof(PDFfile, usespot), 0, const_cast<char*>("Use Spot Colors. Bool value")},
	{const_cast<char*>("domulti"), T_INT, offsetof(PDFfile, domulti), 0, const_cast<char*>("Produce a PDF File for every Page. Bool value")},
	{const_cast<char*>("aprint"), T_INT, offsetof(PDFfile, aprint), 0, const_cast<char*>("Allow Printing the Document. Bool value")},
	{const_cast<char*>("achange"), T_INT, offsetof(PDFfile, achange), 0, const_cast<char*>("Allow Changing the Document. Bool value")},
	{const_cast<char*>("acopy"), T_INT, offsetof(PDFfile, acopy), 0, const_cast<char*>("Allow Copying Text and Graphics. Bool value")},
	{const_cast<char*>("aanot"), T_INT, offsetof(PDFfile, aanot), 0, const_cast<char*>("Allow Adding Annotations and Fields. Bool value")},
	{const_cast<char*>("version"), T_INT, offsetof(PDFfile, version), 0, const_cast<char*>("Choose PDF version to use:\n\t12 = PDF/X-3\n\t13 = PDF 1.3 (Acrobat 4)\n\t14 = PDF 1.4 (Acrobat 5)")},
	{const_cast<char*>("outdst"), T_INT, offsetof(PDFfile, outdst), 0, const_cast<char*>("Output destination.\n\t0 - screen\n\t1 - printer")},
	{const_cast<char*>("profiles"), T_INT, offsetof(PDFfile, profiles), 0, const_cast<char*>("Embed a color profile for solid colors. Bool value.")},
	{const_cast<char*>("profilei"), T_INT, offsetof(PDFfile, profilei), 0, const_cast<char*>("Embed a color profile for images. Bool value.")},
	{const_cast<char*>("intents"), T_INT, offsetof(PDFfile, intents), 0, const_cast<char*>("Rendering intent for solid colors\n\t0 - Perceptual\n\t1 - Relative Colorimetric\n\t2 - Saturation\n\t3 - Absolute Colorimetric")},
	{const_cast<char*>("intenti"), T_INT, offsetof(PDFfile, intenti), 0, const_cast<char*>("Rendering intent for images\n\t0 - Perceptual\n\t1 - Relative Colorimetric\n\t2 - Saturation\n\t3 - Absolute Colorimetric")},
	{const_cast<char*>("noembicc"), T_INT, offsetof(PDFfile, noembicc), 0, const_cast<char*>("Don't use embedded ICC profiles. Bool value")},
	{const_cast<char*>("bleedt"), T_DOUBLE, offsetof(PDFfile, bleedt), 0, const_cast<char*>("Bleed Top\n""Distance for bleed from the top of the physical page")},
	{const_cast<char*>("bleedl"), T_DOUBLE, offsetof(PDFfile, bleedl), 0, const_cast<char*>("Bleed Left\n""Distance for bleed from the left of the physical page")},
	{const_cast<char*>("bleedr"), T_DOUBLE, offsetof(PDFfile, bleedr), 0, const_cast<char*>("Bleed Right\n""Distance for bleed from the right of the physical page")},
	{const_cast<char*>("bleedb"), T_DOUBLE, offsetof(PDFfile, bleedb), 0, const_cast<char*>("Bleed Bottom\n""Distance for bleed from the bottom of the physical page")},
	{const_cast<char*>("usedocbleeds"), T_INT, offsetof(PDFfile, usedocbleeds), 0, const_cast<char*>("Use the existing bleed settings from the document preferences. Bool value")},
	{const_cast<char*>("useLayers"), T_INT, offsetof(PDFfile, useLayers), 0, const_cast<char*>("Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.")},
	{const_cast<char*>("embedPDF"), T_INT, offsetof(PDFfile, embedPDF), 0, const_cast<char*>("Export EPS and PDFs in image frames as embedded PDFs. This does *not* yet take care of colorspaces, so you should know what you are doing before setting this to 'true'.")},
	{const_cast<char*>("mirrorH"), T_INT, offsetof(PDFfile, mirrorH), 0, const_cast<char*>("Mirror Page(s) horizontally")},
	{const_cast<char*>("mirrorV"), T_INT, offsetof(PDFfile, mirrorV), 0, const_cast<char*>("Mirror Page(s) vertically")},
	{const_cast<char*>("doClip"), T_INT, offsetof(PDFfile, doClip), 0, const_cast<char*>("Do not show objects outside the margins in the exported file")},
	{const_cast<char*>("isGrayscale"), T_INT, offsetof(PDFfile, isGrayscale), 0, const_cast<char*>("Export PDF in grayscale")},
	{const_cast<char*>("pageLayout"), T_INT, offsetof(PDFfile, pageLayout), 0, const_cast<char*>("Document layout in PDF viewer:\n"
												     "\t0 - Show the document in single page mode\n"
												     "\t1 - Show the document in single page mode with the pages displayed continuously end to end like a scroll\n"
												     "\t2 - Show the document with facing pages, starting with the first page displayed on the left\n"
												     "\t3 - Show the document with facing pages, starting with the first page displayed on the right"
												     )},
	{const_cast<char*>("displayBookmarks"), T_INT, offsetof(PDFfile, displayBookmarks), 0, const_cast<char*>("Display the bookmarks upon opening")},
	{const_cast<char*>("displayThumbs"), T_INT, offsetof(PDFfile, displayThumbs), 0, const_cast<char*>("Display the page thumbnails upon opening")},
	{const_cast<char*>("displayLayers"), T_INT, offsetof(PDFfile, displayLayers), 0, const_cast<char*>("Display the layer list upon opening. Useful only for PDF 1.5+.")},
	{const_cast<char*>("displayFullscreen"), T_INT, offsetof(PDFfile, displayFullscreen), 0, const_cast<char*>("Display the document in full screen mode upon opening.")},
	{const_cast<char*>("hideToolBar"), T_INT, offsetof(PDFfile, hideToolBar), 0, const_cast<char*>("Hide the viewer toolbar. The toolbar has usually selection and other editing capabilities.")},
	{const_cast<char*>("hideMenuBar"), T_INT, offsetof(PDFfile, hideMenuBar), 0, const_cast<char*>("Hide the viewer menu bar, the PDF will display in a plain window.")},
	{const_cast<char*>("fitWindow"), T_INT, offsetof(PDFfile, fitWindow), 0, const_cast<char*>("Fit the document page or pages to the available space in the viewer window.")},
	{NULL, 0, 0, 0, NULL} // sentinel
};


/* Here begins Getter & Setter functions */

static PyObject *PDFfile_getfile(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->file);
	return self->file;
}

static int PDFfile_setfile(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'file' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The 'file' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->file);
	Py_INCREF(value);
	self->file = value;
	return 0;
}

static PyObject *PDFfile_getfonts(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->fonts);
	return self->fonts;
}

static int PDFfile_setfonts(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'fonts' attribute.");
		return -1;
	}
	if (!PyList_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The 'fonts' attribute value must be list of strings.");
		return -1;
	}
	int n;
	n = PyList_Size(value);
	for (int i=0; i<n; ++i)
		if (!PyString_Check(PyList_GetItem(value, i))) {
			PyErr_SetString(PyExc_TypeError, "The 'fonts' list must contain only strings.");
			return -1;
		}
// Do I need to check if supplied string is really
// name of available font???
// this is not implemented yet
	Py_DECREF(self->fonts);
	Py_INCREF(value);
	self->fonts = value;
	PyList_Sort(self->fonts);
	return 0;
}

static PyObject *PDFfile_getSubsetList(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->SubsetList);
	return self->SubsetList;
}

static int PDFfile_setSubsetList(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'SubsetList' attribute.");
		return -1;
	}
	if (!PyList_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The 'SubsetList' attribute value must be list of strings.");
		return -1;
	}
	int n;
	n = PyList_Size(value);
	for (int i=0; i<n; ++i)
		if (!PyString_Check(PyList_GetItem(value, i))) {
			PyErr_SetString(PyExc_TypeError, "The 'SubsetList' list must contain only strings.");
			return -1;
		}
	Py_DECREF(self->SubsetList);
	Py_INCREF(value);
	self->SubsetList = value;
	PyList_Sort(self->SubsetList);
	return 0;
}

static PyObject *PDFfile_getpages(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->pages);
	return self->pages;
}

static int PDFfile_setpages(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'pages' attribute.");
		return -1;
	}
	if (!PyList_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'pages' attribute value must be list of integers.");
		return -1;
	}
	int len = PyList_Size(value);
	for (int i = 0; i<len; i++){
		PyObject *tmp = PyList_GetItem(value, i);
		// I did not check if tmp is NULL
		// how can PyList_GetItem fail in this case (my guess: short of available memory?)
		// Also do I need Py_INCREF or Py_DECREF here?
		if (!PyInt_Check(tmp)){
			PyErr_SetString(PyExc_TypeError, "'pages' list must contain only integers.");
			return -1;
		}
		if (PyInt_AsLong(tmp) > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count()) || PyInt_AsLong(tmp) < 1) {
			PyErr_SetString(PyExc_ValueError, "'pages' value out of range.");
			return -1;
		}
	}
	Py_DECREF(self->pages);
	Py_INCREF(value);
	self->pages = value;
	PyList_Sort(self->pages);
	return 0;
}


static PyObject *PDFfile_getresolution(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->resolution);
	return self->resolution;
}

static int PDFfile_setresolution(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'resolution' attribute.");
		return -1;
	}
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'resolution' attribute value must be integer.");
		return -1;
	}
	int n = PyInt_AsLong(value);
	if (n<35 || n>4000) {
		PyErr_SetString(PyExc_ValueError, "'resolution' value must be in interval from 35 to 4000");
		return -1;
	}
	Py_DECREF(self->resolution);
	Py_INCREF(value);
	self->resolution = value;
	return 0;
}

static PyObject *PDFfile_getdownsample(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->downsample);
	return self->downsample;
}

static int PDFfile_setdownsample(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'downsample' attribute.");
		return -1;
	}
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'downsample' attribute value must be integer.");
		return -1;
	}
	int n = PyInt_AsLong(value);
	if (n!=0 && (n<35 || n>PyInt_AsLong(self->resolution))) {
		PyErr_SetString(PyExc_TypeError, "'downsample' value must be 0 or in interval from 35 to value of 'resolutin'");
		return -1;
	}
	Py_DECREF(self->downsample);
	Py_INCREF(value);
	self->downsample = value;
	return 0;
}

static PyObject *PDFfile_geteffval(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->effval);
	return self->effval;
}

static int PDFfile_seteffval(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'effval' attribute.");
		return -1;
	}
	if (!PyList_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'effval' must be list.");
		return -1;
	}
	int n = PyList_Size(value);
	for (int i=0; i<n; ++i) {
		PyObject *tmp = PyList_GetItem(value, i);
		if (!PyList_Check(tmp)) {
			PyErr_SetString(PyExc_TypeError, "elemets of 'effval' must be list of five integers.");
			return -1;
		}
		int j = PyList_Size(tmp);
		if (j != 6) {
			PyErr_SetString(PyExc_TypeError, "elemets of 'effval' must have exactly six integers.");
			return -1;
		}
		for ( --j; j > -1; --j) {
			if (!PyInt_Check(PyList_GetItem(tmp, j))) {
				PyErr_SetString(PyExc_TypeError, "innermost element of 'effval' must be integers.");
				return -1;
			}
		}
	}
	Py_DECREF(self->effval);
	Py_INCREF(value);
	self->effval = value;
	return 0;
}

static PyObject *PDFfile_getlpival(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->lpival);
	return self->lpival;
}

static int PDFfile_setlpival(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'lpival' attribute.");
		return -1;
	}
	if (!PyList_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'lpival' must be list.");
		return -1;
	}
	// Do I need Py_INCREF or Py_DECREF here?
	int n = PyList_Size(value);
	for (int i=0; i<n; ++i) {
		PyObject *tmp = PyList_GetItem(value, i);
		if (!PyList_Check(tmp)) {
			PyErr_SetString(PyExc_TypeError, "elemets of 'lpival' must be list of five integers.");
			return -1;
		}
		int j = PyList_Size(tmp);
		if (j != 4) {
			PyErr_SetString(PyExc_TypeError, "elemets of 'lpival' must have exactly four members.");
			return -1;
		}
		for ( --j; j > 0; --j) {
			if (!PyInt_Check(PyList_GetItem(tmp, j))) {
				PyErr_SetString(PyExc_TypeError, "'lpival'elements must have structure [siii]");
				return -1;
			}
		}
		if (!PyString_Check(PyList_GetItem(tmp, 0))) {
			PyErr_SetString(PyExc_TypeError, "'lpival'elements must have structure [siii]");
			return -1;
		}
	}
	Py_DECREF(self->lpival);
	Py_INCREF(value);
	self->lpival = value;
	return 0;
}

static PyObject *PDFfile_getowner(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->owner);
	return self->owner;
}

static int PDFfile_setowner(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'owner' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'owner' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->owner);
	Py_INCREF(value);
	self->owner = value;
	return 0;
}

static PyObject *PDFfile_getuser(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->user);
	return self->user;
}

static int PDFfile_setuser(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'user' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'user' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->user);
	Py_INCREF(value);
	self->user = value;
	return 0;
}

static PyObject *PDFfile_getsolidpr(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->solidpr);
	return self->solidpr;
}

static int PDFfile_setsolidpr(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'solidpr' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The 'solidpr' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->solidpr);
	Py_INCREF(value);
	self->solidpr = value;
	return 0;
}

static PyObject *PDFfile_getimagepr(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->imagepr);
	return self->imagepr;
}

static int PDFfile_setimagepr(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'imagepr' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The 'imagepr' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->imagepr);
	Py_INCREF(value);
	self->imagepr = value;
	return 0;
}

static PyObject *PDFfile_getprintprofc(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->printprofc);
	return self->printprofc;
}

static int PDFfile_setprintprofc(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'printprofc' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The 'printprofc' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->printprofc);
	Py_INCREF(value);
	self->printprofc = value;
	return 0;
}

static PyObject *PDFfile_getinfo(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->info);
	return self->info;
}

static int PDFfile_setinfo(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'info' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The 'info' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->info);
	Py_INCREF(value);
	self->info = value;
	return 0;
}

static PyObject *PDFfile_getRotateDeg(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->RotateDeg);
	return self->RotateDeg;
}

static int PDFfile_setRotateDeg(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'RotateDeg' attribute.");
		return -1;
	}
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'RotateDeg' attribute value must be integer.");
		return -1;
	}
	int n = PyInt_AsLong(value);
	if (n!=0 && n!=90 && n!=180 && n!=270) {
		PyErr_SetString(PyExc_TypeError, "'RotateDeg' value must be 0 or 90 or 180 or 270");
		return -1;
	}
	Py_DECREF(self->RotateDeg);
	Py_INCREF(value);
	self->RotateDeg = value;
	return 0;
}

static PyObject *PDFfile_getopenAction(PDFfile *self, void * /*closure*/)
{
	Py_INCREF(self->openAction);
	return self->openAction;
}

static int PDFfile_setopenAction(PDFfile *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete 'openAction' attribute.");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "'openAction' attribute value must be string.");
		return -1;
	}
	Py_DECREF(self->openAction);
	Py_INCREF(value);
	self->openAction = value;
	return 0;
}

static char *effval_doc = const_cast<char*>(
"List of effection values for each saved page.\n"
"It is list of list of six integers. Those int has followin meaning:\n\t"
"- Length of time the page is shown before the presentation\n\tstarts on the selected page. (1-3600)\n\t"
"- Length of time the effect runs. (1 - 3600)\n\t\tA shorter time will speed up the effect,\n\t\ta longer one will slow it down\n\t"
"- Type of the display effect\n\t\t0 - No Effect\n\t\t1 - Blinds\n\t\t2 - Box\n\t\t3 - Dissolve\n\t\t4 - Glitter\n\t\t5 - Split\n\t\t6 - Wipe\n\t"
"- Direction of the effect of moving lines\n\tfor the split and blind effects.\n\t\t0 - Horizontal\n\t\t1 - Vertical\n\t"
"- Starting position for the box and split effects.\n\t\t0 - Inside\n\t\t1 - Outside\n\t"
"- Direction of the glitter or wipe effects.\n\t\t0 - Left to Right\n\t\t1 - Top to Bottom\n\t\t2 - Bottom to Top\n\t\t3 - Right to Left\n\t\t4 - Top-left to Bottom-Right");

static char *lpival_doc = const_cast<char*>(
"Rendering Settings for individual colors.\n\n"
"This is list of values for each color\n"
"Color values have structure [siii] which stand for:\n\t"
"s - Color name ('Black', 'Cyan', 'Magenta', 'Yellow')\n\t"
"i - Frequency (10 to 1000)\n\t"
"i - Angle (-180 to 180)\n\t"
"i - Spot Function\n\t\t0 - Simple Dot\n\t\t1 - Line\n\t\t2 - Round\n\t\t3 - Ellipse\n"
"Be carefull when supplying these values as they\nare not checked for validity.");

static PyGetSetDef PDFfile_getseters [] = {
	{const_cast<char*>("file"), (getter)PDFfile_getfile, (setter)PDFfile_setfile, const_cast<char*>("Name of file to save into"), NULL},
	{const_cast<char*>("fonts"), (getter)PDFfile_getfonts, (setter)PDFfile_setfonts, const_cast<char*>("List of fonts to embed."), NULL},
	{const_cast<char*>("SubsetList"), (getter)PDFfile_getSubsetList, (setter)PDFfile_setSubsetList, const_cast<char*>("List of fonts to outlined."), NULL},
	{const_cast<char*>("pages"), (getter)PDFfile_getpages, (setter)PDFfile_setpages, const_cast<char*>("List of pages to print"), NULL},
	{const_cast<char*>("resolution"), (getter)PDFfile_getresolution, (setter)PDFfile_setresolution, const_cast<char*>("Resolution of output file. Values from 35 to 4000."), NULL},
	{const_cast<char*>("downsample"), (getter)PDFfile_getdownsample, (setter)PDFfile_setdownsample, const_cast<char*>("Downsample image resolusion to this value. Values from 35 to 4000\nSet 0 for not to downsample"), NULL},
	{const_cast<char*>("effval"), (getter)PDFfile_geteffval, (setter)PDFfile_seteffval, effval_doc, NULL},
	{const_cast<char*>("lpival"), (getter)PDFfile_getlpival, (setter)PDFfile_setlpival, lpival_doc, NULL},
	{const_cast<char*>("owner"), (getter)PDFfile_getowner, (setter)PDFfile_setowner, const_cast<char*>("Owner's password"), NULL},
	{const_cast<char*>("user"), (getter)PDFfile_getuser, (setter)PDFfile_setuser, const_cast<char*>("User's password"), NULL},
	{const_cast<char*>("solidpr"), (getter)PDFfile_getsolidpr, (setter)PDFfile_setsolidpr, const_cast<char*>("Color profile for solid colors"), NULL},
	{const_cast<char*>("imagepr"), (getter)PDFfile_getimagepr, (setter)PDFfile_setimagepr, const_cast<char*>("Color profile for images"), NULL},
	{const_cast<char*>("printprofc"), (getter)PDFfile_getprintprofc, (setter)PDFfile_setprintprofc, const_cast<char*>("Output profile for printing. If possible, get some guidance from your printer on profile selection."), NULL},
	{const_cast<char*>("info"), (getter)PDFfile_getinfo, (setter)PDFfile_setinfo, const_cast<char*>("Mandatory string for PDF/X-3 or the PDF will fail\nPDF/X-3 conformance. We recommend you use the title of the document."), NULL},
	{const_cast<char*>("RotateDeg"), (getter)PDFfile_getRotateDeg, (setter)PDFfile_setRotateDeg, const_cast<char*>("Automatically rotate the exported pages\n\tValue must be one of integers: 0, 90, 180 or 270"), NULL},
	{const_cast<char*>("openAction"), (getter)PDFfile_getopenAction, (setter)PDFfile_setopenAction, const_cast<char*>("Javascript to be executed when PDF document is opened."), NULL},
	{NULL, NULL, NULL, NULL, NULL}  // sentinel
};

static PyObject *PDFfile_save(PDFfile *self)
{
	if (!ScCore->primaryMainWindow()->HaveDoc) {
		PyErr_SetString(PyExc_SystemError, "Need to open document first");
		return NULL;
	};

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	PDFOptions& pdfOptions = currentDoc->PDF_Options;

// copied from file scribus.cpp
//void ScribusMainWindow::SaveAsPDF()
	int Components = 3;
	QString nam = "";
	if (ScCore->primaryMainWindow()->bookmarkPalette->BView->topLevelItemCount() == 0)
		pdfOptions.Bookmarks = false;

// apply fonts attribute
	pdfOptions.EmbedList.clear();
	int n = PyList_Size(self->fonts);
	for ( int i=0; i<n; ++i){
		QString tmpFon;
		tmpFon = QString(PyString_AsString(PyList_GetItem(self->fonts, i)));
		pdfOptions.EmbedList.append(tmpFon);
	}
// apply SubsetList attribute
	pdfOptions.SubsetList.clear();
	n = PyList_Size(self->SubsetList);
	for ( int i=0; i<n; ++i){
		QString tmpFon;
		tmpFon = QString(PyString_AsString(PyList_GetItem(self->SubsetList, i)));
		pdfOptions.SubsetList.append(tmpFon);
	}
// apply file attribute
	QString fn;
	fn = QString(PyString_AsString(self->file));
	pdfOptions.fileName = fn;
// apply pages attribute
	std::vector<int> pageNs;
	int nn=PyList_Size(self->pages);
	for (int i = 0; i < nn; ++i) {
		pageNs.push_back((int)PyInt_AsLong(PyList_GetItem(self->pages, i)));
	}
// apply thumbnails attribute
	pdfOptions.Thumbnails = self->thumbnails;
// apply cropmarks attribute
	pdfOptions.cropMarks = self->cropMarks;
// apply bleedmarks attribute
	pdfOptions.bleedMarks = self->bleedMarks;
// apply registrationmarks attribute
	pdfOptions.registrationMarks = self->registrationMarks;
// apply colormarks attribute
	pdfOptions.colorMarks = self->colorMarks;
// apply docInfoMark attribute
	pdfOptions.docInfoMarks = self->docInfoMarks;
// apply mark offset attribute
	pdfOptions.markOffset = qMax(0.0, self->markOffset / currentDoc->unitRatio());
// apply compress attribute
	self->compressmtd = minmaxi(self->compressmtd, 0, 3);
	pdfOptions.Compress = self->compress;
	pdfOptions.CompressMethod = (PDFOptions::PDFCompression) self->compressmtd;
// apply quality attribute
	self->quality = minmaxi(self->quality, 0, 4);
	pdfOptions.Quality = self->quality;
// apply resolusion attribute
	pdfOptions.Resolution = PyInt_AsLong(self->resolution);
// apply downsample attribute
	pdfOptions.RecalcPic = PyInt_AsLong(self->downsample);
	if (pdfOptions.RecalcPic)
		pdfOptions.PicRes = PyInt_AsLong(self->downsample);
	else
		pdfOptions.PicRes = pdfOptions.Resolution;
// apply bookmarks attribute
	pdfOptions.Bookmarks = self->bookmarks;
// apply binding attribute
	pdfOptions.Binding = self->binding;
// apply presentation attribute
	pdfOptions.PresentMode = self->presentation;

	QList<PDFPresentationData> PresentVals;
	PresentVals.clear();
	int tmpnum;
	tmpnum=PyList_Size(self->effval);
	for (int i=0; i<tmpnum; ++i) {
		PDFPresentationData t;
// How do I make this commented piece of code to work?
// I always get an error here
		PyObject *ti = PyList_GetItem(self->effval, i);
//		 if (!PyArg_ParseTuple(ti , "[iiiiii]",
//				  &t.pageEffectDuration, &t.pageViewDuration, &t.effectType, &t.Dm,
//				  &t.M, &t.Di)) {
//			 PyErr_SetString(PyExc_SystemError, "while parsing 'effval'. WHY THIS HAPPENED????");
//			 return NULL;
//		 }
//		 PresentVals.append(t);
				// pv 10/03/2004 crashed when pt is null
				if (ti)
				{
					// Do I Need to check if every PyInt_AsLong and PyList_GetItem funtion succeed???
					t.pageEffectDuration = PyInt_AsLong(PyList_GetItem(ti, 0));
					t.pageViewDuration = PyInt_AsLong(PyList_GetItem(ti, 1));
					t.effectType = PyInt_AsLong(PyList_GetItem(ti, 2));
					t.Dm = PyInt_AsLong(PyList_GetItem(ti, 3));
					t.M = PyInt_AsLong(PyList_GetItem(ti, 4));
					t.Di = PyInt_AsLong(PyList_GetItem(ti, 5));
					PresentVals.append(t);
				} // if ti=NULL

	}

	pdfOptions.PresentVals = PresentVals;
// apply lpival
	int n2 = PyList_Size(self->lpival);
	for (int i=0; i<n2; ++i){
		LPIData lpi;
		PyObject *t = PyList_GetItem(self->lpival, i);
// This code always raise exception - WHY???
//		char *s;
//		 if (!PyArg_ParseTuple(t, "[siii]", &s, &lpi.Frequency,
//				  &lpi.Angle, &lpi.SpotFunc)) {
//			 PyErr_SetString(PyExc_SystemError, "while parsing 'lpival'. WHY THIS HAPPENED????");
//			 return NULL;
//		 }
//		 pdfOptions.LPISettings[QString(s)]=lpi;
		QString st;
		st = QString(PyString_AsString(PyList_GetItem(t,0)));
		lpi.Frequency = PyInt_AsLong(PyList_GetItem(t, 1));
		lpi.Angle = PyInt_AsLong(PyList_GetItem(t, 2));
		lpi.SpotFunc = PyInt_AsLong(PyList_GetItem(t, 3));
		pdfOptions.LPISettings[st]=lpi;
	}

	pdfOptions.Articles = self->article;
	pdfOptions.Encrypt = self->encrypt;
	pdfOptions.UseLPI = self->uselpi;
	pdfOptions.UseSpotColors = self->usespot;
	pdfOptions.doMultiFile = self->domulti;
	self->version = minmaxi(self->version, 11, 15);
	// FIXME: Sanity check version
	pdfOptions.Version = (PDFOptions::PDFVersion)self->version;
	if (self->encrypt)
	{
		int Perm = -64;
		if (pdfOptions.Version == PDFOptions::PDFVersion_14)
			Perm &= ~0x00240000;
		if (self->aprint)
			Perm += 4;
		if (self->achange)
			Perm += 8;
		if (self->acopy)
			Perm += 16;
		if (self->aanot)
			Perm += 32;
		pdfOptions.Permissions = Perm;
		pdfOptions.PassOwner = QString(PyString_AsString(self->owner));
		pdfOptions.PassUser = QString(PyString_AsString(self->user));
	}
	if (self->outdst == 0)
	{
		pdfOptions.UseRGB = true;
		pdfOptions.UseProfiles = false;
		pdfOptions.UseProfiles2 = false;
	}
	else
	{
		pdfOptions.UseRGB = false;
		if (currentDoc->HasCMS)
		{
			pdfOptions.UseProfiles = self->profiles;
			pdfOptions.UseProfiles2 = self->profilei;
			self->intents = minmaxi(self->intents, 0, 3);
			pdfOptions.Intent = self->intents;
			self->intenti = minmaxi(self->intenti, 0, 3);
			pdfOptions.Intent2 = self->intenti;
			pdfOptions.EmbeddedI = self->noembicc;
			pdfOptions.SolidProf = PyString_AsString(self->solidpr);
			pdfOptions.ImageProf = PyString_AsString(self->imagepr);
			pdfOptions.PrintProf = PyString_AsString(self->printprofc);
			if (pdfOptions.Version == PDFOptions::PDFVersion_X1a ||
				pdfOptions.Version == PDFOptions::PDFVersion_X3)
			{
				// Where does compiler find cms function when I have not included header for it
				ScColorProfile hIn;
				hIn = ScColorMgmtEngine::openProfileFromFile(ScCore->PrinterProfiles[pdfOptions.PrintProf]);
				nam = hIn.productDescription();
				if (hIn.colorSpace() == ColorSpace_Rgb)
					Components = 3;
				if (hIn.colorSpace() == ColorSpace_Cmyk)
					Components = 4;
				if (hIn.colorSpace() == ColorSpace_Gray)
					Components = 3;
				pdfOptions.Info = PyString_AsString(self->info);
				pdfOptions.Encrypt = false;
				pdfOptions.PresentMode = false;
			}
		}
		else
		{
			pdfOptions.UseProfiles = false;
			pdfOptions.UseProfiles2 = false;
		}

	}
	QMap<int,QPixmap> thumbs;
	for (uint ap = 0; ap < pageNs.size(); ++ap)
	{
		QPixmap pm(10,10);
		if (pdfOptions.Thumbnails)
			pm = QPixmap::fromImage(ScCore->primaryMainWindow()->view->PageToPixmap(pageNs[ap]-1, 100));
		thumbs.insert(pageNs[ap], pm);
	}
	ReOrderText(currentDoc, ScCore->primaryMainWindow()->view);

	MarginStruct optBleeds(pdfOptions.bleeds);
	pdfOptions.useDocBleeds = self->usedocbleeds;
	if (self->usedocbleeds)
		pdfOptions.bleeds = currentDoc->bleeds;
	else {
		self->bleedt = minmaxd(self->bleedt, 0, currentDoc->pageHeight * currentDoc->unitRatio());
		pdfOptions.bleeds.Top = self->bleedt / currentDoc->unitRatio();
		self->bleedl = minmaxd(self->bleedl, 0, currentDoc->pageWidth * currentDoc->unitRatio());
		pdfOptions.bleeds.Left = self->bleedl / currentDoc->unitRatio();
		self->bleedr = minmaxd(self->bleedr, 0, currentDoc->pageWidth * currentDoc->unitRatio());
		pdfOptions.bleeds.Right = self->bleedr / currentDoc->unitRatio();
		self->bleedb = minmaxd(self->bleedb, 0, currentDoc->pageHeight * currentDoc->unitRatio());
		pdfOptions.bleeds.Bottom = self->bleedb / currentDoc->unitRatio();
	}
	pdfOptions.useLayers = self->useLayers;
	pdfOptions.embedPDF = self->embedPDF;
	pdfOptions.MirrorH = self->mirrorH;
	pdfOptions.MirrorV = self->mirrorV;
	pdfOptions.doClip = self->doClip;
	pdfOptions.RotateDeg = PyInt_AsLong(self->RotateDeg);
	pdfOptions.isGrayscale = self->isGrayscale;
	pdfOptions.PageLayout = minmaxi(self->pageLayout, 0, 3);
	pdfOptions.displayBookmarks = self->displayBookmarks;
	pdfOptions.displayThumbs = self->displayThumbs;
	pdfOptions.displayLayers = self->displayLayers;
	pdfOptions.displayFullscreen = self->displayFullscreen;
	pdfOptions.hideToolBar = self->hideToolBar;
	pdfOptions.hideMenuBar = self->hideMenuBar;
	pdfOptions.fitWindow = self->fitWindow;
	pdfOptions.openAction = QString(PyString_AsString(self->openAction));
	pdfOptions.firstUse = false;

	QString errorMessage;
	bool success = ScCore->primaryMainWindow()->getPDFDriver(fn, nam, Components, pageNs, thumbs, errorMessage);
	if (!success) {
		fn  = "Cannot write the File: " + fn;
		if (!errorMessage.isEmpty())
			fn += QString("\n%1").arg(errorMessage);
		PyErr_SetString(PyExc_SystemError, fn.toAscii());
	}

	if (self->usedocbleeds || (pdfOptions.Version != PDFOptions::PDFVersion_X1a && 
	                           pdfOptions.Version != PDFOptions::PDFVersion_X3))
		pdfOptions.bleeds = optBleeds;

	if (success)
		Py_RETURN_NONE;
	return NULL;
}

static PyMethodDef PDFfile_methods[] = {
	{const_cast<char*>("save"), (PyCFunction)PDFfile_save, METH_NOARGS, const_cast<char*>("Save selected pages to pdf file")},
	{NULL, (PyCFunction)(0), 0, NULL} // sentinel
};

PyTypeObject PDFfile_Type = {
	PyObject_HEAD_INIT(NULL) // PyObject_VAR_HEAD
	0,		      //
	const_cast<char*>("PDFfile"), // char *tp_name; /* For printing, in format "<module>.<name>" */
	sizeof(PDFfile),     // int tp_basicsize, /* For allocation */
	0,		    // int tp_itemsize; /* For allocation */

	/* Methods to implement standard operations */

	(destructor) PDFfile_dealloc, //     destructor tp_dealloc;
	0, //     printfunc tp_print;
	0, //     getattrfunc tp_getattr;
	0, //     setattrfunc tp_setattr;
	0, //     cmpfunc tp_compare;
	0, //     reprfunc tp_repr;

	/* Method suites for standard classes */

	0, //     PyNumberMethods *tp_as_number;
	0, //     PySequenceMethods *tp_as_sequence;
	0, //     PyMappingMethods *tp_as_mapping;

	/* More standard operations (here for binary compatibility) */

	0, //     hashfunc tp_hash;
	0, //     ternaryfunc tp_call;
	0, //     reprfunc tp_str;
	0, //     getattrofunc tp_getattro;
	0, //     setattrofunc tp_setattro;

	/* Functions to access object as input/output buffer */
	0, //     PyBufferProcs *tp_as_buffer;

	/* Flags to define presence of optional/expanded features */
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,    // long tp_flags;

	pdffile__doc__,      // char *tp_doc; /* Documentation string */

	/* Assigned meaning in release 2.0 */
	/* call function for all accessible objects */
	0, //     traverseproc tp_traverse;

	/* delete references to contained objects */
	0, //     inquiry tp_clear;

	/* Assigned meaning in release 2.1 */
	/* rich comparisons */
	0, //     richcmpfunc tp_richcompare;

	/* weak reference enabler */
	0, //     long tp_weaklistoffset;

	/* Added in release 2.2 */
	/* Iterators */
	0, //     getiterfunc tp_iter;
	0, //     iternextfunc tp_iternext;

	/* Attribute descriptor and subclassing stuff */
	PDFfile_methods, //     struct PyMethodDef *tp_methods;
	PDFfile_members, //     struct PyMemberDef *tp_members;
	PDFfile_getseters, //     struct PyGetSetDef *tp_getset;
	0, //     struct _typeobject *tp_base;
	0, //     PyObject *tp_dict;
	0, //     descrgetfunc tp_descr_get;
	0, //     descrsetfunc tp_descr_set;
	0, //     long tp_dictoffset;
	(initproc)PDFfile_init, //     initproc tp_init;
	0, //     allocfunc tp_alloc;
	PDFfile_new, //     newfunc tp_new;
	0, //     freefunc tp_free; /* Low-level free-memory routine */
	0, //     inquiry tp_is_gc; /* For PyObject_IS_GC */
	0, //     PyObject *tp_bases;
	0, //     PyObject *tp_mro; /* method resolution order */
	0, //     PyObject *tp_cache;
	0, //     PyObject *tp_subclasses;
	0, //     PyObject *tp_weaklist;
	0, //     destructor tp_del;

#ifdef COUNT_ALLOCS
	/* these must be last and never explicitly initialized */
	//    int tp_allocs;
	//    int tp_frees;
	//    int tp_maxalloc;
	//    struct _typeobject *tp_next;
#endif
};
