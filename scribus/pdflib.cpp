/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pdflib.cpp  -  description
                             -------------------
    begin                : Sat Jan 19 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pdflib.h"
#include "pdflib.moc"

#include "scconfig.h"

#include <string>
#include <qstring.h>
#include <qrect.h>
#include <qimage.h>
#include <qregexp.h>
#include <qdatetime.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qdir.h>
#include <cstdlib>
#include <cmath>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "rc4.h"

#include "commonstrings.h"
#include "page.h"
#include "pageitem.h"
#include "bookmwin.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "multiprogressdialog.h"
#include "bookpalette.h"
#include "scfontmetrics.h"
#include "util.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "pdfoptions.h"
#include "sccolor.h"

using namespace std;

#ifdef HAVE_CMS
extern bool CMSuse;
#endif
#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif

PDFlib::PDFlib(ScribusDoc & docu)
	: QObject(&docu),
	doc(docu),
	ActPageP(0),
	Options(doc.PDF_Options),
	Bvie(0),
	ObjCounter(7),
	ResNam("RE"),
	ResCount(0),
	NDnam("LI"),
	NDnum(0),
	KeyGen(32),
	OwnerKey(32),
	UserKey(32),
	FileID(16),
	EncryKey(5),
	Encrypt(0),
	KeyLen(5),
	colorsToUse(),
	spotNam("Spot"),
	spotCount(0),
	progressDialog(0),
	abortExport(false),
	usingGUI(ScQApp->usingGUI())
{
	Catalog.Outlines = 2;
	Catalog.PageTree = 3;
	Catalog.Dest = 4;
	PageTree.Count = 0;
	Outlines.First = 0;
	Outlines.Last = 0;
	Outlines.Count = 0;
	Seite.ObjNum = 0;
	Seite.Thumb = 0;
#ifdef HAVE_LIBZ
	CompAvail = true;
#else
	CompAvail = false;
#endif
	int kg_array[] = {0x28, 0xbf, 0x4e, 0x5e, 0x4e, 0x75, 0x8a, 0x41, 0x64, 0x00, 0x4e, 0x56, 0xff, 0xfa,
			  0x01, 0x08, 0x2e, 0x2e, 0x00, 0xb6, 0xd0, 0x68, 0x3e, 0x80, 0x2f, 0x0c, 0xa9, 0xfe,
			  0x64, 0x53, 0x69, 0x7a};
	for (int a = 0; a < 32; ++a)
		KeyGen[a] = kg_array[a];
	if (usingGUI)
	{
		progressDialog = new MultiProgressDialog(tr("Saving PDF"), CommonStrings::tr_Cancel, ScMW, "pdfexportprogress");
		Q_CHECK_PTR(progressDialog);
		QStringList barNames, barTexts;
		barNames << "EMP" << "EP" << "ECPI";
		barTexts << tr("Exporting Master Pages:") << tr("Exporting Pages:") << tr("Exporting Items on Current Page:");
		progressDialog->addExtraProgressBars(barNames, barTexts);
		connect(progressDialog->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelRequested()));
	}
}

PDFlib::~PDFlib()
{
	delete progressDialog;
}

static inline QString FToStr(double c)
{
	return QString::number(c, 'f', 5);
};

bool PDFlib::doExport(const QString& fn, const QString& nam, int Components,
					  const std::vector<int> & pageNs, const QMap<int,QPixmap> & thumbs)
{
	QPixmap pm;
	bool ret = false;
	int pc_exportpages=0;
	int pc_exportmasterpages=0;
	if (usingGUI)
		progressDialog->show();
	if (PDF_Begin_Doc(fn, PrefsManager::instance()->appPrefs.AvailFonts, doc.UsedFonts, ScMW->bookmarkPalette->BView))
	{
		QMap<int, int> pageNsMpa;
		for (uint a = 0; a < pageNs.size(); ++a)
		{
			pageNsMpa.insert(doc.MasterNames[doc.Pages->at(pageNs[a]-1)->MPageNam], 0);
		}
		if (usingGUI)
		{
			progressDialog->setOverallTotalSteps(pageNsMpa.count()+pageNs.size());
			progressDialog->setTotalSteps("EMP", pageNsMpa.count());
			progressDialog->setTotalSteps("EP", pageNs.size());
			progressDialog->setOverallProgress(0);
			progressDialog->setProgress("EMP", 0);
			progressDialog->setProgress("EP", 0);
		}
		for (uint ap = 0; ap < doc.MasterPages.count() && !abortExport; ++ap)
		{
			if (doc.MasterItems.count() != 0)
			{
				if (pageNsMpa.contains(ap))
				{
					ScQApp->processEvents();
					PDF_TemplatePage(doc.MasterPages.at(ap));
					++pc_exportmasterpages;
				}
			}

			if (usingGUI)
			{
				progressDialog->setProgress("EMP", pc_exportmasterpages);
				progressDialog->setOverallProgress(pc_exportmasterpages+pc_exportpages);
			}
		}
		for (uint a = 0; a < pageNs.size() && !abortExport; ++a)
		{
			if (doc.PDF_Options.Thumbnails)
				pm = thumbs[pageNs[a]];
			ScQApp->processEvents();
			if (abortExport) break;
			PDF_Begin_Page(doc.Pages->at(pageNs[a]-1), pm);
			ScQApp->processEvents();
			if (abortExport) break;
			PDF_ProcessPage(doc.Pages->at(pageNs[a]-1), pageNs[a]-1, doc.PDF_Options.doClip);
			ScQApp->processEvents();
			if (abortExport) break;
			PDF_End_Page();
			pc_exportpages++;
			if (usingGUI)
			{
				progressDialog->setProgress("EP", pc_exportpages);
				progressDialog->setOverallProgress(pc_exportmasterpages+pc_exportpages);
			}
		}
		ret = true;//Even when aborting we return true. Dont want that "couldnt write msg"
		if (!abortExport)
		{
			if (doc.PDF_Options.Version == PDFOptions::PDFVersion_X3)
				PDF_End_Doc(ScMW->PrinterProfiles[doc.PDF_Options.PrintProf], nam, Components);
			else
				PDF_End_Doc();
		}
		else
			closeAndCleanup();
	}
	if (usingGUI)
		progressDialog->close();
	return ret;
}

void PDFlib::StartObj(int nr)
{
	XRef.append(bytesWritten());
	PutDoc(QString::number(nr)+ " 0 obj\n");
}

// Encode a string for inclusion in a
// PDF (literal) .
QString PDFlib::PDFEncode(const QString & in)
{
	QString tmp("");
	for (uint d = 0; d < in.length(); ++d)
	{
		QChar cc(in.at(d));
		if ((cc == '(') || (cc == ')') || (cc == '\\'))
			tmp += '\\';
		tmp += cc;
	}
	return tmp;
}

QString PDFlib::EncStream(const QString & in, int ObjNum)
{
	if (in.length() < 1)
		return QString("");
	else if (!Options.Encrypt)
		return in;
	rc4_context_t rc4;
	int dlen = 0;
	QString tmp(in);
	QByteArray us(tmp.length());
	QByteArray ou(tmp.length());
	for (uint a = 0; a < tmp.length(); ++a)
		us[a] = uchar(QChar(tmp.at(a)));
	QByteArray data(10);
	if (KeyLen > 5)
		data.resize(21);
	for (int cd = 0; cd < KeyLen; ++cd)
	{
		data[cd] = EncryKey[cd];
		dlen++;
	}
	data[dlen++] = ObjNum;
	data[dlen++] = ObjNum >> 8;
	data[dlen++] = ObjNum >> 16;
	data[dlen++] = 0;
	data[dlen++] = 0;
	QByteArray step1(16);
	step1 = ComputeMD5Sum(&data);
	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), QMIN(KeyLen+5, 16));
	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), tmp.length());
	QString uk = "";
	for (uint cl = 0; cl < tmp.length(); ++cl)
		uk += QChar(ou[cl]);
	return uk;
}

QString PDFlib::EncString(const QString & in, int ObjNum)
{
	if (!Options.Encrypt)
		return in;
	rc4_context_t rc4;
	QString tmp;
	int dlen = 0;
	if (in.length() < 3)
		return "<>";
	tmp = in.mid(1, in.length()-2);
	QByteArray us(tmp.length());
	QByteArray ou(tmp.length());
	for (uint a = 0; a < tmp.length(); ++a)
		us[a] = static_cast<uchar>(QChar(tmp.at(a)));
	QByteArray data(10);
	if (KeyLen > 5)
		data.resize(21);
	for (int cd = 0; cd < KeyLen; ++cd)
	{
		data[cd] = EncryKey[cd];
		dlen++;
	}
	data[dlen++] = ObjNum;
	data[dlen++] = ObjNum >> 8;
	data[dlen++] = ObjNum >> 16;
	data[dlen++] = 0;
	data[dlen++] = 0;
	QByteArray step1(16);
	step1 = ComputeMD5Sum(&data);
	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), QMIN(KeyLen+5, 16));
	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), tmp.length());
	QString uk = "";
	for (uint cl = 0; cl < tmp.length(); ++cl)
		uk += QChar(ou[cl]);
	tmp = "<"+String2Hex(&uk, false)+">";
	return tmp;
}

QString PDFlib::FitKey(const QString & pass)
{
	QString pw(pass);
	if (pw.length() < 32)
	{
		uint l = pw.length();
		for (uint a = 0; a < 32 - l; ++a)
			pw += QChar(KeyGen[a]);
	}
	else
		pw = pw.left(32);
	return pw;
}

void PDFlib::CalcOwnerKey(const QString & Owner, const QString & User)
{
	rc4_context_t rc4;
	QString pw(FitKey(User));
	QString pw2(FitKey(Owner.isEmpty() ? User : Owner));
	QByteArray step1(16);
	step1 = ComputeMD5(pw2);
	if (KeyLen > 5)
	{
		for (int kl = 0; kl < 50; ++kl)
			step1 = ComputeMD5Sum(&step1);
	}
	QByteArray us(32);
	QByteArray enk(16);
	if (KeyLen > 5)
	{
		for (uint a2 = 0; a2 < 32; ++a2)
			OwnerKey[a2] = static_cast<uchar>(QChar(pw.at(a2)));
		for (int rl = 0; rl < 20; rl++)
		{
			for (int j = 0; j < 16; j ++)
				enk[j] = step1[j] ^ rl;
			rc4_init(&rc4, reinterpret_cast<uchar*>(enk.data()), 16);
			rc4_encrypt(&rc4, reinterpret_cast<uchar*>(OwnerKey.data()),
					 reinterpret_cast<uchar*>(OwnerKey.data()), 32);
		}
	}
	else
	{
		for (uint a = 0; a < 32; ++a)
			us[a] = static_cast<uchar>(QChar(pw.at(a)));
		rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), 5);
		rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()),
					reinterpret_cast<uchar*>(OwnerKey.data()), 32);
	}
}

void PDFlib::CalcUserKey(const QString & User, int Permission)
{
	rc4_context_t	rc4;
	QString pw(FitKey(User));
	QByteArray step1(16);
	QByteArray perm(4);
	uint perm_value = static_cast<uint>(Permission);
	perm[0] = perm_value;
	perm[1] = perm_value >> 8;
	perm[2] = perm_value >> 16;
	perm[3] = perm_value >> 24;
	for (uint a = 0; a < 32; ++a)
		pw += QChar(OwnerKey[a]);
	for (uint a1 = 0; a1 < 4; ++a1)
		pw += QChar(perm[a1]);
	for (uint a3 = 0; a3 < 16; ++a3)
		pw += QChar(FileID[a3]);
	step1 = ComputeMD5(pw);
	if (KeyLen > 5)
	{
		for (int kl = 0; kl < 50; ++kl)
			step1 = ComputeMD5Sum(&step1);
		EncryKey.resize(16);
	}
	for (int a2 = 0; a2 < KeyLen; ++a2)
		EncryKey[a2] = step1[a2];
	if (KeyLen > 5)
	{
		QString pr2("");
		for (int kl3 = 0; kl3 < 32; ++kl3)
			pr2 += QChar(KeyGen[kl3]);
		for (uint a4 = 0; a4 < 16; ++a4)
			pr2 += QChar(FileID[a4]);
		step1 = ComputeMD5(pr2);
		QByteArray enk(16);
		for (uint a3 = 0; a3 < 16; ++a3)
			UserKey[a3] = step1[a3];
		for (int rl = 0; rl < 20; rl++)
		{
			for (int j = 0; j < 16; j ++)
				enk[j] = EncryKey[j] ^ rl;
			rc4_init(&rc4, reinterpret_cast<uchar*>(enk.data()), 16);
			rc4_encrypt(&rc4, reinterpret_cast<uchar*>(UserKey.data()), reinterpret_cast<uchar*>(UserKey.data()), 16);
		}
	}
	else
	{
		rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), 5);
		rc4_encrypt(&rc4, reinterpret_cast<uchar*>(KeyGen.data()), reinterpret_cast<uchar*>(UserKey.data()), 32);
	}
}

QByteArray PDFlib::ComputeMD5(const QString& in)
{
	uint inlen=in.length();
	QByteArray TBytes(inlen);
	for (uint a = 0; a < inlen; ++a)
		TBytes[a] = static_cast<uchar>(QChar(in.at(a)));
	return ComputeMD5Sum(&TBytes);
}

bool PDFlib::PDF_Begin_Doc(const QString& fn, SCFonts &AllFonts, QMap<QString,int> DocFonts, BookMView* vi)
{
	Spool.setName(fn);
	if (!Spool.open(IO_WriteOnly))
		return false;
	outStream.setDevice(&Spool);
	QString tmp;
	QString ok = "";
	QString uk = "";
	QFileInfo fd;
	QString fext;
	int a;
	Bvie = vi;
	BookMinUse = false;
	UsedFontsP.clear();
	if ((Options.Version == 15) && (Options.useLayers))
		ObjCounter = 10;
	else
		ObjCounter = 9;
	switch (Options.Version)
	{
		case 12:
		case 13:
			PutDoc("%PDF-1.3\n");
			break;
		case 14:
			PutDoc("%PDF-1.4\n");
			break;
		case 15:
			PutDoc("%PDF-1.5\n");
			break;
	}
	if (Options.Version == 12)
		ObjCounter++;
	PutDoc("%\xc7\xec\x8f\xa2\n");
	StartObj(1);
	PutDoc("<<\n/Type /Catalog\n/Outlines 3 0 R\n/Pages 4 0 R\n/Dests 5 0 R\n/AcroForm 6 0 R\n/Names 7 0 R\n/Threads 8 0 R\n");
	if ((Options.Version == 15) && (Options.useLayers))
		PutDoc("/OCProperties 9 0 R\n");
	if (Options.Version == 12)
		PutDoc("/OutputIntents [ "+QString::number(ObjCounter-1)+" 0 R ]\n");
	PutDoc("/PageLayout ");
	switch (Options.PageLayout)
	{
		case PDFOptions::SinglePage:
			PutDoc("/SinglePage\n");
			break;
		case PDFOptions::OneColumn:
			PutDoc("/OneColumn\n");
			break;
		case PDFOptions::TwoColumnLeft:
			PutDoc("/TwoColumnLeft\n");
			break;
		case PDFOptions::TwoColumnRight:
			PutDoc("/TwoColumnRight\n");
			break;
	}
	if (Options.displayBookmarks)
		PutDoc("/PageMode /UseOutlines\n");
	else if (Options.displayFullscreen)
		PutDoc("/PageMode /FullScreen\n");
	else if (Options.displayThumbs)
		PutDoc("/PageMode /UseThumbs\n");
	else if ((Options.Version == 15) && (Options.displayLayers))
			PutDoc("/PageMode /UseOC\n");
	if (!Options.openAction.isEmpty())
	{
		PutDoc("/OpenAction << /S /JavaScript /JS (this."+Options.openAction+"\\(\\)) >>\n");
	}
	PutDoc("/ViewerPreferences\n<<\n/PageDirection ");
	PutDoc( Options.Binding == 0 ? "/L2R\n" : "/R2L\n");
	if (Options.hideToolBar)
		PutDoc("/HideToolbar true\n");
	if (Options.hideMenuBar)
		PutDoc("/HideMenubar true\n");
	if (Options.fitWindow)
		PutDoc("/FitWindow true\n");
	PutDoc(" >>\n>>\nendobj\n");
	QString IDg(Datum);
	IDg += Options.Datei;
	IDg += "Scribus "+QString(VERSION);
	IDg += "Libpdf for Scribus "+QString(VERSION);
	IDg += doc.documentInfo.getTitle();
	IDg += doc.documentInfo.getAuthor();
	IDg += "/False";
	FileID = ComputeMD5(IDg);
	if (Options.Encrypt)
	{
		KeyLen = Options.Version == 14 ? 16 : 5;
		CalcOwnerKey(Options.PassOwner, Options.PassUser);
		CalcUserKey(Options.PassUser, Options.Permissions);
		for (uint cl2 = 0; cl2 < 32; ++cl2)
			ok += QChar(OwnerKey[cl2]);
		if (KeyLen > 5)
		{
			for (uint cl3 = 0; cl3 < 16; ++cl3)
				uk += QChar(UserKey[cl3]);
			for (uint cl3r = 0; cl3r < 16; ++cl3r)
				uk += QChar(KeyGen[cl3r]);
		}
		else
		{
			for (uint cl = 0; cl < 32; ++cl)
				uk += QChar(UserKey[cl]);
		}
	}
	QDate d = QDate::currentDate();
	Datum = "D:";
	tmp.sprintf("%4d", d.year());
	tmp.replace(QRegExp(" "), "0");
	Datum += tmp;
	tmp.sprintf("%2d", d.month());
	tmp.replace(QRegExp(" "), "0");
	Datum += tmp;
	tmp.sprintf("%2d", d.day());
	tmp.replace(QRegExp(" "), "0");
	Datum += tmp;
	tmp = QTime::currentTime().toString();
	tmp.replace(QRegExp(":"), "");
	Datum += tmp;
	StartObj(2);
	PutDoc("<<\n/Creator "+EncString("(Scribus "+QString(VERSION)+")",2)+"\n");
	PutDoc("/Producer "+EncString("(Libpdf for Scribus "+QString(VERSION)+")",2)+"\n");
	QString docTitle = doc.documentInfo.getTitle();
	if ((Options.Version == 12) && (docTitle.isEmpty()))
		PutDoc("/Title "+EncString("("+doc.DocName+")",2)+"\n");
	else
		PutDoc("/Title "+EncString("("+doc.documentInfo.getTitle()+")",2)+"\n");
	PutDoc("/Author "+EncString("("+doc.documentInfo.getAuthor()+")",2)+"\n");
	PutDoc("/Keywords "+EncString("("+doc.documentInfo.getKeywords()+")",2)+"\n");
	PutDoc("/CreationDate "+EncString("("+Datum+")",2)+"\n");
	PutDoc("/ModDate "+EncString("("+Datum+")",2)+"\n");
	if (Options.Version == 12)
		PutDoc("/GTS_PDFXVersion (PDF/X-3:2002)\n");
	PutDoc("/Trapped /False\n>>\nendobj\n");
	for (int t = 0; t < 6; ++t)
		XRef.append(bytesWritten());
	if ((Options.Version == 15) && (Options.useLayers))
		XRef.append(bytesWritten());
	if (Options.Version == 12)
		XRef.append(bytesWritten());
	if (Options.Encrypt)
	{
		StartObj(ObjCounter);
		Encrypt = ObjCounter;
		ObjCounter++;
		PutDoc("<<\n/Filter /Standard\n");
		PutDoc( KeyLen > 5 ? "/R 3\n/V 2\n/Length 128\n" : "/R 2\n/V 1\n");
		PutDoc("/O <"+String2Hex(&ok)+">\n");
		PutDoc("/U <"+String2Hex(&uk)+">\n");
		PutDoc("/P "+QString::number(Options.Permissions)+"\n>>\nendobj\n");
	}
	RealFonts = DocFonts;
	QMap<QString,int> ReallyUsed;
	ReallyUsed.clear();
	PageItem* pgit;
	QMap<int, QString> ind2PDFabr;
	const QString tmpf[] = {"/Courier", "/Courier-Bold", "/Courier-Oblique", "/Courier-BoldOblique",
												"/Helvetica", "/Helvetica-Bold", "/Helvetica-Oblique", "/Helvetica-BoldOblique",
												"/Times-Roman", "/Times-Bold", "/Times-Italic", "/Times-BoldItalic",
												"/ZapfDingbats", "/Symbol"};
	size_t ar = sizeof(tmpf) / sizeof(*tmpf);
	for (uint ax = 0; ax < ar; ++ax)
		ind2PDFabr[ax] = tmpf[ax];
	for (uint c = 0; c < doc.FrameItems.count(); ++c)
	{
		pgit = doc.FrameItems.at(c);
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
				StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
			for (uint e = 0; e < pgit->itemText.count(); ++e)
			{
				ReallyUsed.insert(pgit->itemText.at(e)->cfont->scName(), DocFonts[pgit->itemText.at(e)->cfont->scName()]);
			}
		}
	}
	for (uint c = 0; c < doc.MasterItems.count(); ++c)
	{
		pgit = doc.MasterItems.at(c);
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
				StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
			for (uint e = 0; e < pgit->itemText.count(); ++e)
			{
				ReallyUsed.insert(pgit->itemText.at(e)->cfont->scName(), DocFonts[pgit->itemText.at(e)->cfont->scName()]);
			}
		}
	}
	for (uint d = 0; d < doc.Items->count(); ++d)
	{
		pgit = doc.Items->at(d);
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
				StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
			for (uint e = 0; e < pgit->itemText.count(); ++e)
			{
				ReallyUsed.insert(pgit->itemText.at(e)->cfont->scName(), DocFonts[pgit->itemText.at(e)->cfont->scName()]);
			}
		}
	}
	a = 0;
	QMap<QString, QString>::Iterator itStd;
	for (itStd = StdFonts.begin(); itStd != StdFonts.end(); ++itStd)
	{
		StartObj(ObjCounter);
		PutDoc("<<\n/Type /Font\n/Subtype /Type1\n");
		PutDoc("/BaseFont "+itStd.key()+"\n");
		PutDoc(">>\nendobj\n");
		Seite.FObjects["FoStd"+QString::number(a)] = ObjCounter;
		itStd.data() = "FoStd"+QString::number(a);
		ObjCounter++;
		a++;
	}
	QMap<QString,int>::Iterator it;
	a = 0;
	for (it = ReallyUsed.begin(); it != ReallyUsed.end(); ++it)
	{
		Foi::FontFormat fformat = AllFonts[it.key()]->formatCode;
		if ((AllFonts[it.key()]->isOTF) || (!AllFonts[it.key()]->HasNames) || (AllFonts[it.key()]->Subset) || (Options.SubsetList.contains(it.key())))
		{
			QString fon("");
			QMap<uint,FPointArray>::Iterator ig;
			for (ig = AllFonts[it.key()]->RealGlyphs.begin(); ig != AllFonts[it.key()]->RealGlyphs.end(); ++ig)
			{
				FPoint np, np1, np2;
				bool nPath = true;
				if (ig.data().size() > 3)
				{
					FPointArray gly = ig.data().copy();
					QWMatrix mat;
					mat.scale(0.1, 0.1);
					gly.map(mat);
					for (uint poi = 0; poi < gly.size()-3; poi += 4)
					{
						if (gly.point(poi).x() > 900000)
						{
							fon += "h\n";
							nPath = true;
							continue;
						}
						if (nPath)
						{
							np = gly.point(poi);
							fon += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
							nPath = false;
						}
						np = gly.point(poi+1);
						np1 = gly.point(poi+3);
						np2 = gly.point(poi+2);
						fon += FToStr(np.x()) + " " + FToStr(-np.y()) + " " +
							 FToStr(np1.x()) + " " + FToStr(-np1.y()) + " " +
							 FToStr(np2.x()) + " " + FToStr(-np2.y()) + " c\n";
					}
					fon += "h f*\n";
					StartObj(ObjCounter);
					ObjCounter++;
					np = getMinClipF(&gly);
					np1 = getMaxClipF(&gly);
					PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
					PutDoc("/BBox [ "+FToStr(np.x())+" "+FToStr(-np.y())+" "+FToStr(np1.x())+ " "+FToStr(-np1.y())+" ]\n");
					PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
					PutDoc(">>\n");
					if ((Options.Compress) && (CompAvail))
						fon = CompressStr(&fon);
					PutDoc("/Length "+QString::number(fon.length()+1));
					if ((Options.Compress) && (CompAvail))
						PutDoc("\n/Filter /FlateDecode");
					PutDoc(" >>\nstream\n"+EncStream(fon, ObjCounter-1)+"\nendstream\nendobj\n");
					Seite.XObjects[AllFonts[it.key()]->RealName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+QString::number(ig.key())] = ObjCounter-1;
					fon = "";
				}
			}
			AllFonts[it.key()]->RealGlyphs.clear();
		}
		else
		{
			UsedFontsP.insert(it.key(), "/Fo"+QString::number(a));
			if ((fformat == Foi::PFB) && (Options.EmbedList.contains(it.key())))
			{
				QString fon("");
				StartObj(ObjCounter);
				QByteArray bb;
				AllFonts[it.key()]->RawData(bb);
				uint posi;
				for (posi = 6; posi < bb.size(); ++posi)
				{
					if ((bb[posi] == static_cast<char>(0x80)) && (static_cast<int>(bb[posi+1]) == 2))
						break;
					fon += QChar(bb[posi]);
				}
				int len1 = fon.length();
				uint ulen;
				ulen = bb[posi+2] & 0xff;
				ulen |= (bb[posi+3] << 8) & 0xff00;
				ulen |= (bb[posi+4] << 16) & 0xff0000;
				ulen |= (bb[posi+5] << 24) & 0xff000000;
				if (ulen > bb.size())
					ulen = bb.size()-7;
				posi += 6;
				for (uint j = 0; j < ulen; ++j)
					fon += QChar(bb[posi++]);
				posi += 6;
				int len2 = fon.length()-len1;
				for (uint j = posi; j < bb.size(); ++j)
				{
					if ((bb[j] == static_cast<char>(0x80)) && (static_cast<int>(bb[j+1]) == 3))
						break;
					if (bb[j] == '\r')
						fon += "\n";
					else
						fon += QChar(bb[j]);
				}
				int len3 = fon.length()-len2-len1;
				if ((Options.Compress) && (CompAvail))
					fon = CompressStr(&fon);
				PutDoc("<<\n/Length "+QString::number(fon.length()+1)+"\n");
				PutDoc("/Length1 "+QString::number(len1)+"\n");
				PutDoc("/Length2 "+QString::number(len2)+"\n");
				PutDoc("/Length3 "+QString::number(len3)+"\n");
				if ((Options.Compress) && (CompAvail))
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n"+EncStream(fon,ObjCounter)+"\nendstream\nendobj\n");
				ObjCounter++;
			}
			if ((fformat == Foi::PFA) && (Options.EmbedList.contains(it.key())))
			{
				QString fon("");
				QString fon2("");
				QString tm("");
				uint value;
				bool ok = true;
				StartObj(ObjCounter);
				AllFonts[it.key()]->EmbedFont(fon);
				int len1 = fon.find("eexec")+5;
				fon2 = fon.left(len1)+"\n";
				int len2 = fon.find("0000000000000000000000000");
				if (len2 == -1)
					len2 = fon.length()+1;
				int count = 0;
				for (int xx = len1; xx < len2-1; ++xx)
				{
					tm = fon.at(xx);
					if ((tm == QChar(13)) || (tm == QChar(10)))
						continue;
					xx++;
					count++;
					tm += fon.at(xx);
					value = tm.toUInt(&ok, 16);
					fon2 += QChar(value);
				}
				fon2 += fon.mid(len2);
				if ((Options.Compress) && (CompAvail))
					fon2 = CompressStr(&fon2);
				PutDoc("<<\n/Length "+QString::number(fon2.length()+1)+"\n");
				PutDoc("/Length1 "+QString::number(len1+1)+"\n");
				PutDoc("/Length2 "+QString::number(count)+"\n");
				PutDoc(static_cast<int>(fon.length()-len2) == -1 ? QString("/Length3 0\n") : "/Length3 "+QString::number(fon.length()-len2)+"\n");
				if ((Options.Compress) && (CompAvail))
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n"+EncStream(fon2, ObjCounter)+"\nendstream\nendobj\n");
				ObjCounter++;
			}
			if ((fformat == Foi::SFNT || fformat == Foi::TTCF) && (Options.EmbedList.contains(it.key())))
			{
				QString fon("");
				StartObj(ObjCounter);
				QByteArray bb;
				AllFonts[it.key()]->RawData(bb);
				//AV: += and append() dont't work because they stop at '\0' :-(
				for (unsigned int i=0; i < bb.size(); i++)
					fon += QChar(bb[i]);
				int len = fon.length();
				if ((Options.Compress) && (CompAvail))
					fon = CompressStr(&fon);
				//qDebug(QString("sfnt data: size=%1 before=%2 compressed=%3").arg(bb.size()).arg(len).arg(fon.length()));
				PutDoc("<<\n/Length "+QString::number(fon.length()+1)+"\n");
				PutDoc("/Length1 "+QString::number(len)+"\n");
				if ((Options.Compress) && (CompAvail))
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n"+EncStream(fon, ObjCounter)+"\nendstream\nendobj\n");
				ObjCounter++;
			}
			StartObj(ObjCounter);
			PutDoc("<<\n/Type /FontDescriptor\n");
			PutDoc("/FontName /"+AllFonts[it.key()]->RealName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+"\n");
			PutDoc("/FontBBox [ "+AllFonts[it.key()]->FontBBox+" ]\n");
			PutDoc("/Flags ");
			//FIXME: isItalic() should be queried from Foi, not from Qt -- AV
			//QFontInfo fo = QFontInfo(it.data());
			int pfl = 0;
			if (AllFonts[it.key()]->IsFixedPitch)
				pfl = pfl ^ 1;
			//if (fo.italic())
			if (AllFonts[it.key()]->ItalicAngle != "0")
				pfl = pfl ^ 64;
//			pfl = pfl ^ 4;
			pfl = pfl ^ 32;
			PutDoc(QString::number(pfl)+"\n");
			PutDoc("/Ascent "+AllFonts[it.key()]->Ascent+"\n");
			PutDoc("/Descent "+AllFonts[it.key()]->Descender+"\n");
			PutDoc("/CapHeight "+AllFonts[it.key()]->CapHeight+"\n");
			PutDoc("/ItalicAngle "+AllFonts[it.key()]->ItalicAngle+"\n");
			PutDoc("/StemV "+AllFonts[it.key()]->StdVW+"\n");
			if ((fformat == Foi::SFNT || fformat == Foi::TTCF) && (Options.EmbedList.contains(it.key())))
				PutDoc("/FontFile2 "+QString::number(ObjCounter-1)+" 0 R\n");
			if ((fformat == Foi::PFB) && (Options.EmbedList.contains(it.key())))
				PutDoc("/FontFile "+QString::number(ObjCounter-1)+" 0 R\n");
			if ((fformat == Foi::PFA) && (Options.EmbedList.contains(it.key())))
				PutDoc("/FontFile "+QString::number(ObjCounter-1)+" 0 R\n");
			PutDoc(">>\nendobj\n");
			ObjCounter++;
/*			if (!FT_Has_PS_Glyph_Names(AllFonts[it.key()])
			{
				StartObj(ObjCounter);
				int chCount = 31;
				PutDoc("[ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
				for (int ww = 31; ww < 256; ++ww)
				{
					PutDoc(QString::number(static_cast<int>(AllFonts[it.key()]->CharWidth[itg.key()]*
							1000))+" ");
					if (itg == gl.end())
						break;
					++itg;
					chCount++;
				}
				PutDoc("]\nendobj\n");
				ObjCounter++;
				// put widths object
				// encoding dictionary w/ base encoding w/o differences
				StartObj(ObjCounter);
				PutDoc("<<\n/Type /Font\n/Subtype ");
				PutDoc((fformat == Foi::SFNT || fformat == Foi::TTCF) ? "/TrueType\n" : "/Type1\n");
				PutDoc("/Name /Fo"+QString::number(a)+"\n");
				PutDoc("/BaseFont /"+AllFonts[it.key()]->RealName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "" )+"\n");
				//cf. widths:
				PutDoc("/FirstChar 0\n");
				PutDoc("/LastChar "+QString::number(chCount-1)+"\n");
				PutDoc("/Widths "+QString::number(ObjCounter-1)+" 0 R\n");
				PutDoc("/FontDescriptor "+QString::number(ObjCounter-2)+" 0 R\n");
				PutDoc(">>\nendobj\n");
				Seite.FObjects["Fo"+QString::number(a)] = ObjCounter;
				ObjCounter++;
			}
			else */
//			{
				GListeInd gl;
				GlyIndex(AllFonts[it.key()], &gl);
				GlyphsIdxOfFont.insert(it.key(), gl);
				uint FontDes = ObjCounter - 1;
				GListeInd::Iterator itg;
				itg = gl.begin();
				GListeInd::Iterator itg2;
				itg2 = gl.begin();
				uint Fcc = gl.count() / 224;
				if ((gl.count() % 224) != 0)
					Fcc += 1;
				for (uint Fc = 0; Fc < Fcc; ++Fc)
				{
					StartObj(ObjCounter);
					int chCount = 31;
					PutDoc("[ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
					for (int ww = 31; ww < 256; ++ww)
					{
						PutDoc(QString::number(static_cast<int>(AllFonts[it.key()]->CharWidth[itg.key()]* 1000))+" ");
						if (itg == gl.end())
							break;
						++itg;
						chCount++;
					}
					PutDoc("]\nendobj\n");
					ObjCounter++;
					StartObj(ObjCounter);
					ObjCounter++;
					PutDoc("<< /Type /Encoding\n");
//					PutDoc("/BaseEncoding /" + AllFonts[it.key()]->FontEnc + "\n");
					PutDoc("/Differences [ 32\n");
					int crc = 0;
					for (int ww2 = 32; ww2 < 256; ++ww2)
					{
						PutDoc(itg2.data().Name+" ");
						if (itg2 == gl.end())
							break;
						++itg2;
						crc++;
						if (crc > 8)
						{
							PutDoc("\n");
							crc = 0;
						}
					}
					PutDoc("]\n");

					PutDoc(">>\nendobj\n");
					StartObj(ObjCounter);
					PutDoc("<<\n/Type /Font\n/Subtype ");
					PutDoc((fformat == Foi::SFNT || fformat == Foi::TTCF) ? "/TrueType\n" : "/Type1\n");
					PutDoc("/Name /Fo"+QString::number(a)+"S"+QString::number(Fc)+"\n");
					PutDoc("/BaseFont /"+AllFonts[it.key()]->RealName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+"\n");
					PutDoc("/FirstChar 0\n");
					PutDoc("/LastChar "+QString::number(chCount-1)+"\n");
					PutDoc("/Widths "+QString::number(ObjCounter-2)+" 0 R\n");
					PutDoc("/Encoding "+QString::number(ObjCounter-1)+" 0 R\n");
					PutDoc("/FontDescriptor "+QString::number(FontDes)+" 0 R\n");
					PutDoc(">>\nendobj\n");
					Seite.FObjects["Fo"+QString::number(a)+"S"+QString::number(Fc)] = ObjCounter;
					ObjCounter++;
				} // for(Fc)
//			} // FT_Has_PS_Glyph_Names
		}
		a++;
	}
	if (Options.UseLPI)
	{
		StartObj(ObjCounter);
		PutDoc("<<\n/Type /Halftone\n/HalftoneType 5\n");
		QMap<QString,LPIData>::const_iterator itlp;
		for (itlp = Options.LPISettings.constBegin(); itlp != Options.LPISettings.constEnd(); ++itlp)
		{
			PutDoc("/"+itlp.key()+"\n<<\n/Type /Halftone\n/HalftoneType 1\n/Frequency ");
			PutDoc(QString::number(itlp.data().Frequency)+"\n/Angle "+QString::number(itlp.data().Angle)+"\n/SpotFunction ");
			QString func ("");
			switch (itlp.data().SpotFunc)
			{
				case 0:
					func = "/SimpleDot";
					break;
				case 1:
					func = "/Line";
					break;
				case 2:
					func = "/Round";
					break;
				case 3:
					func = "/Ellipse";
					break;
				default:
					func = "/SimpleDot";
					break;
			}
			PutDoc(func+"\n>>\n");
		}
		PutDoc("/Default\n<<\n/Type /Halftone\n/HalftoneType 1\n/Frequency 50\n/Angle 45\n/SpotFunction /Round\n>>\n");
		PutDoc(">>\nendobj\n");
		ObjCounter++;
		StartObj(ObjCounter);
		HTName = ResNam+QString::number(ResCount);
		Transpar[HTName] = ObjCounter;
		PutDoc("<< /Type /ExtGState\n/HT "+QString::number(ObjCounter-1)+" 0 R\n>>\nendobj\n");
		ResCount++;
		ObjCounter++;
	}
#ifdef HAVE_CMS
	if ((CMSuse) && (Options.UseProfiles))
	{
		StartObj(ObjCounter);
		ObjCounter++;
		QString dataP;
		struct ICCD dataD;
		loadText(ScMW->InputProfiles[Options.SolidProf], &dataP);
		PutDoc("<<\n");
		if ((Options.Compress) && (CompAvail))
		{
			PutDoc("/Filter /FlateDecode\n");
			dataP = CompressStr(&dataP);
		}
		PutDoc("/Length "+QString::number(dataP.length()+1)+"\n");
		PutDoc("/N "+QString::number(Options.SComp)+"\n");
		PutDoc(">>\nstream\n"+EncStream(dataP, ObjCounter-1)+"\nendstream\nendobj\n");
		StartObj(ObjCounter);
		dataD.ResName = ResNam+QString::number(ResCount);
		dataD.ICCArray = "[ /ICCBased "+QString::number(ObjCounter-1)+" 0 R ]";
		dataD.ResNum = ObjCounter;
		ICCProfiles[Options.SolidProf] = dataD;
		PutDoc("[ /ICCBased "+QString::number(ObjCounter-1)+" 0 R ]\n");
		PutDoc("endobj\n");
		ResCount++;
		ObjCounter++;
	}
#endif
	if (((Options.isGrayscale == false) && (Options.UseRGB == false)) && (Options.UseSpotColors))
	{
		doc.getUsedColors(colorsToUse);
		ColorList::Iterator itf;
		for (itf = colorsToUse.begin(); itf != colorsToUse.end(); ++itf)
		{
			if ((colorsToUse[itf.key()].isSpotColor()) || (colorsToUse[itf.key()].isRegistrationColor()))
			{
				int cc, cm, cy, ck;
				struct SpotC spotD;
				colorsToUse[itf.key()].getCMYK(&cc, &cm, &cy, &ck);
				QString colorDesc = "{\ndup "+FToStr(static_cast<double>(cc) / 255)+"\nmul exch dup ";
				colorDesc += FToStr(static_cast<double>(cm) / 255)+"\nmul exch dup ";
				colorDesc += FToStr(static_cast<double>(cy) / 255)+"\nmul exch ";
				colorDesc += FToStr(static_cast<double>(ck) / 255)+" mul }";
				StartObj(ObjCounter);
				ObjCounter++;
				PutDoc("<<\n/FunctionType 4\n");
				PutDoc("/Domain [0.0 1.0]\n");
				PutDoc("/Range [0.0 1.0 0.0 1.0 0.0 1.0 0.0 1.0]\n");
				PutDoc("/Length "+QString::number(colorDesc.length()+1)+"\n");
				PutDoc(">>\nstream\n"+EncStream(colorDesc, ObjCounter-1)+"\nendstream\nendobj\n");
				StartObj(ObjCounter);
				PutDoc("[ /Separation /");
				if (colorsToUse[itf.key()].isRegistrationColor())
					PutDoc("All");
				else
					PutDoc(itf.key().simplifyWhiteSpace().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ).replace("#", "_"));
				PutDoc(" /DeviceCMYK "+QString::number(ObjCounter-1)+" 0 R ]\nendobj\n");
				spotD.ResName = spotNam+QString::number(spotCount);
				spotD.ResNum = ObjCounter;
				spotMap.insert(itf.key(), spotD);
				spotCount++;
				ObjCounter++;
			}
		}
	}
	if ((Options.Version == 15) && (Options.useLayers))
	{
		struct Layer ll;
		struct OCGInfo ocg;
		ll.isPrintable = false;
		ll.LNr = 0;
		int Lnr = 0;
		QString ocgNam("oc");
		uint docLayersCount=doc.Layers.count();
		for (uint la = 0; la < docLayersCount; ++la)
		{
			QString tmp("");
			Level2Layer(&doc, &ll, Lnr);
			ocg.Name = ocgNam+tmp.setNum(ll.LNr);
			ocg.ObjNum = ObjCounter;
			ocg.visible = ll.isViewable;
			OCGEntries.insert(ll.Name, ocg);
			StartObj(ObjCounter);
			ObjCounter++;
			PutDoc("<<\n");
			PutDoc("/Type /OCG\n");
			PutDoc("/Name ("+ll.Name+")\n");
			PutDoc(">>\nendobj\n");
			Lnr++;
		}
	}
	return true;
}

void PDFlib::PDF_TemplatePage(const Page* pag, bool )
{
	QString tmp;
	ActPageP = pag;
	PageItem* ite;
	QPtrList<PageItem> PItems;
	int Lnr = 0;
	struct Layer ll;
	ll.isPrintable = false;
	ll.LNr = 0;
	Inhalt = "";
	Seite.AObjects.clear();
	for (uint la = 0; la < doc.Layers.count(); ++la)
	{
		Level2Layer(&doc, &ll, Lnr);
		PItems = doc.MasterItems;
		if (ll.isPrintable)
		{
			if ((Options.Version == 15) && (Options.useLayers))
				PutPage("/OC /"+OCGEntries[ll.Name].Name+" BDC\n");
			for (uint a = 0; a < PItems.count(); ++a)
			{
				Inhalt = "";
				ite =PItems.at(a);
				if (ite->LayerNr != ll.LNr)
					continue;
				double x = pag->xOffset();
				double y = pag->yOffset();
				double w = pag->width();
				double h1 = pag->height();
				double ilw=ite->lineWidth();
				double x2 = ite->BoundingX - ilw / 2.0;
				double y2 = ite->BoundingY - ilw / 2.0;
				double w2 = ite->BoundingW + ilw;
				double h2 = ite->BoundingH + ilw;
				if (!( QMAX( x, x2 ) <= QMIN( x+w, x2+w2 ) && QMAX( y, y2 ) <= QMIN( y+h1, y2+h2 )))
					continue;
				if (ite->ChangedMasterItem)
					continue;
				if ((!pag->PageNam.isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
					continue;
				PutPage("q\n");
				if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= 14))
					PutPage(PDF_Transparenz(ite));
/* Bookmarks on Master Pages do not make any sense */
//				if ((ite->isBookmark) && (Options.Bookmarks))
//					PDF_Bookmark(ite, pag->height() - (ite->yPos() - pag->yOffset()));
				if (!ite->printEnabled() || ((ite->itemType() == PageItem::TextFrame) && (!pag->PageNam.isEmpty())))
				{
					PutPage("Q\n");
					continue;
				}
				if (ite->fillColor() != CommonStrings::None)
					PutPage(putColor(ite->fillColor(), ite->fillShade(), true));
				if (ite->lineColor() != CommonStrings::None)
					PutPage(putColor(ite->lineColor(), ite->lineShade(), false));
				Inhalt += FToStr(fabs(ite->lineWidth()))+" w\n";
				if (ite->DashValues.count() != 0)
				{
					PutPage("[ ");
					QValueList<double>::iterator it;
					for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
					{
						int da = static_cast<int>(*it);
						if (da != 0)
							PutPage(QString::number(da)+" ");
					}
					PutPage("] "+QString::number(static_cast<int>(ite->DashOffset))+" d\n");
				}
				else
				{
					QString Dt = FToStr(QMAX(2*fabs(ite->lineWidth()), 1));
					QString Da = FToStr(QMAX(6*fabs(ite->lineWidth()), 1));
					switch (ite->PLineArt)
					{
						case Qt::SolidLine:
							PutPage("[] 0 d\n");
							break;
						case Qt::DashLine:
							PutPage("["+Da+" "+Dt+"] 0 d\n");
							break;
						case Qt::DotLine:
							PutPage("["+Dt+"] 0 d\n");
							break;
						case Qt::DashDotLine:
							PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
							break;
						case Qt::DashDotDotLine:
							PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
							break;
						default:
							PutPage("[] 0 d\n");
							break;
					}
				}
				switch (ite->PLineEnd)
				{
					case Qt::FlatCap:
						PutPage("0 J\n");
						break;
					case Qt::SquareCap:
						PutPage("2 J\n");
						break;
					case Qt::RoundCap:
						PutPage("1 J\n");
						break;
					default:
						PutPage("0 J\n");
						break;
				}
				switch (ite->PLineJoin)
				{
					case Qt::MiterJoin:
						PutPage("0 j\n");
						break;
					case Qt::BevelJoin:
						PutPage("2 j\n");
						break;
					case Qt::RoundJoin:
						PutPage("1 j\n");
						break;
					default:
						PutPage("0 j\n");
						break;
				}
				PutPage("1 0 0 1 "+FToStr(ite->xPos() - pag->xOffset())+" "+FToStr(pag->height() - (ite->yPos()  - pag->yOffset()))+" cm\n");
				if (ite->rotation() != 0)
				{
					double sr = sin(-ite->rotation()* M_PI / 180.0);
					double cr = cos(-ite->rotation()* M_PI / 180.0);
					if ((cr * cr) < 0.000001)
						cr = 0;
					if ((sr * sr) < 0.000001)
						sr = 0;
					PutPage(FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+" 0 0 cm\n");
				}
				switch (ite->itemType())
				{
					case PageItem::ImageFrame:
						if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
						{
							if (ite->GrType != 0)
								PutPage(PDF_Gradient(ite));
							else
							{
								PutPage(SetClipPath(ite));
								PutPage("h\nf*\n");
							}
						}
						PutPage("q\n");
						if (ite->imageClip.size() != 0)
						{
							PutPage(SetClipPathImage(ite));
							PutPage("h\nW*\nn\n");
						}
						PutPage(SetClipPath(ite));
						PutPage("h\nW*\nn\n");
						if (ite->imageFlippedH())
							PutPage("-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n");
						if (ite->imageFlippedV())
							PutPage("1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n");
						if ((ite->PicAvail) && (!ite->Pfile.isEmpty()))
							PutPage(PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), false, ite->IProfile, ite->UseEmbedded, ite->IRender));
						PutPage("Q\n");
						if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty())) && (!ite->isTableItem))
						{
							if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
							{
								PutPage(SetClipPath(ite));
								PutPage("h\nS\n");
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									PutPage(setStrokeMulti(&ml[it]));
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
								}
							}
						}
						break;
					case PageItem::TextFrame:
						break;
					case PageItem::Line:
						if (ite->NamedLStyle.isEmpty())
						{
							PutPage("0 0 m\n");
							PutPage(FToStr(ite->width())+" 0 l\n");
							PutPage("S\n");
						}
						else
						{
							multiLine ml = doc.MLineStyles[ite->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								PutPage(setStrokeMulti(&ml[it]));
								PutPage("0 0 m\n");
								PutPage(FToStr(ite->width())+" 0 l\n");
								PutPage("S\n");
							}
						}
						if (ite->startArrowIndex() != 0)
						{
							QWMatrix arrowTrans;
							FPointArray arrow = (*doc.arrowStyles.at(ite->startArrowIndex()-1)).points.copy();
							arrowTrans.translate(0, 0);
							arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
							arrowTrans.scale(-1,1);
							arrow.map(arrowTrans);
							if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
							{
								StartObj(ObjCounter);
								QString ShName = ResNam+QString::number(ResCount);
								Transpar[ShName] = ObjCounter;
								ResCount++;
								ObjCounter++;
								PutDoc("<< /Type /ExtGState\n");
								PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
								PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
								PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
								PutDoc("/BM /Normal\n>>\nendobj\n");
								PutPage("/"+ShName+" gs\n");
							}
							PutPage(putColor(ite->lineColor(), ite->lineShade(), true));
							PutPage(SetClipPathArray(&arrow));
							PutPage("h\nf*\n");
						}
						if (ite->endArrowIndex() != 0)
						{
							QWMatrix arrowTrans;
							FPointArray arrow = (*doc.arrowStyles.at(ite->endArrowIndex()-1)).points.copy();
							arrowTrans.translate(ite->width(), 0);
							arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
							arrow.map(arrowTrans);
							if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
							{
								StartObj(ObjCounter);
								QString ShName = ResNam+QString::number(ResCount);
								Transpar[ShName] = ObjCounter;
								ResCount++;
								ObjCounter++;
								PutDoc("<< /Type /ExtGState\n");
								PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
								PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
								PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
								PutDoc("/BM /Normal\n>>\nendobj\n");
								PutPage("/"+ShName+" gs\n");
							}
							PutPage(putColor(ite->lineColor(), ite->lineShade(), true));
							PutPage(SetClipPathArray(&arrow));
							PutPage("h\nf*\n");
						}
						break;
					case PageItem::ItemType1:
					case PageItem::ItemType3:
					case PageItem::Polygon:
						if (ite->GrType != 0)
							PutPage(PDF_Gradient(ite));
						else
						{
							if (ite->fillColor() != CommonStrings::None)
							{
								PutPage(SetClipPath(ite));
								PutPage("h\nf*\n");
							}
						}
						if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()))
						{
							if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
							{
								PutPage(SetClipPath(ite));
								PutPage("h\nS\n");
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									PutPage(setStrokeMulti(&ml[it]));
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
								}
							}
						}
						break;
					case PageItem::PolyLine:
						if (ite->PoLine.size() > 4) // && ((ite->PoLine.point(0) != ite->PoLine.point(1)) || (ite->PoLine.point(2) != ite->PoLine.point(3))))
						{
							if (ite->GrType != 0)
								PutPage(PDF_Gradient(ite));
							else
							{
								if (ite->fillColor() != CommonStrings::None)
								{
									PutPage(SetClipPath(ite));
									PutPage("h\nf*\n");
								}
							}
						}
						if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()))
						{
							if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
							{
								PutPage(SetClipPath(ite, false));
								PutPage("S\n");
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									PutPage(setStrokeMulti(&ml[it]));
									PutPage(SetClipPath(ite, false));
									PutPage("S\n");
								}
							}
						}
						if (ite->startArrowIndex() != 0)
						{
							FPoint Start = ite->PoLine.point(0);
							for (uint xx = 1; xx < ite->PoLine.size(); xx += 2)
							{
								FPoint Vector = ite->PoLine.point(xx);
								if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
								{
									double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
									QWMatrix arrowTrans;
									FPointArray arrow = (*doc.arrowStyles.at(ite->startArrowIndex()-1)).points.copy();
									arrowTrans.translate(Start.x(), Start.y());
									arrowTrans.rotate(r);
									arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
									arrow.map(arrowTrans);
									if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
									{
										StartObj(ObjCounter);
										QString ShName = ResNam+QString::number(ResCount);
										Transpar[ShName] = ObjCounter;
										ResCount++;
										ObjCounter++;
										PutDoc("<< /Type /ExtGState\n");
										PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
										PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
										PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
										PutDoc("/BM /Normal\n>>\nendobj\n");
										PutPage("/"+ShName+" gs\n");
									}
									PutPage(putColor(ite->lineColor(), ite->lineShade(), true));
									PutPage(SetClipPathArray(&arrow));
									PutPage("h\nf*\n");
									break;
								}
							}
						}
						if (ite->endArrowIndex() != 0)
						{
							FPoint End = ite->PoLine.point(ite->PoLine.size()-2);
							for (uint xx = ite->PoLine.size()-1; xx > 0; xx -= 2)
							{
								FPoint Vector = ite->PoLine.point(xx);
								if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
								{
									double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
									QWMatrix arrowTrans;
									FPointArray arrow = (*doc.arrowStyles.at(ite->endArrowIndex()-1)).points.copy();
									arrowTrans.translate(End.x(), End.y());
									arrowTrans.rotate(r);
									arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
									arrow.map(arrowTrans);
									if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
									{
										StartObj(ObjCounter);
										QString ShName = ResNam+QString::number(ResCount);
										Transpar[ShName] = ObjCounter;
										ResCount++;
										ObjCounter++;
										PutDoc("<< /Type /ExtGState\n");
										PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
										PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
										PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
										PutDoc("/BM /Normal\n>>\nendobj\n");
										PutPage("/"+ShName+" gs\n");
									}
									PutPage(putColor(ite->lineColor(), ite->lineShade(), true));
									PutPage(SetClipPathArray(&arrow));
									PutPage("h\nf*\n");
									break;
								}
							}
						}
						break;
					case PageItem::PathText:
						if (ite->PoShow)
						{
							if (ite->PoLine.size() > 3)
							{
								PutPage("q\n");
								if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()))
								{
									if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
									{
										PutPage(SetClipPath(ite, false));
										PutPage("S\n");
									}
									else
									{
										multiLine ml = doc.MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1;
											it > -1; it--)
											{
											PutPage(setStrokeMulti(&ml[it]));
											PutPage(SetClipPath(ite, false));
											PutPage("S\n");
											}
									}
								}
								PutPage("Q\n");
							}
						}
						PutPage(setTextSt(ite, pag->pageNr(), pag));
						break;
					}
				PutPage("Q\n");
				StartObj(ObjCounter);
				ObjCounter++;
				PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
				PutDoc("/BBox [ 0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" ]\n");
				PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
				if (Seite.ImgObjects.count() != 0)
				{
					PutDoc("/XObject <<\n");
					QMap<QString,int>::Iterator it;
					for (it = Seite.ImgObjects.begin(); it != Seite.ImgObjects.end(); ++it)
						PutDoc("/"+it.key()+" "+QString::number(it.data())+" 0 R\n");
					PutDoc(">>\n");
				}
				if (Seite.FObjects.count() != 0)
				{
					PutDoc("/Font << \n");
					QMap<QString,int>::Iterator it2;
					for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
						PutDoc("/"+it2.key()+" "+QString::number(it2.data())+" 0 R\n");
					PutDoc(">>\n");
				}
				if (Shadings.count() != 0)
				{
					PutDoc("/Shading << \n");
					QMap<QString,int>::Iterator it3;
					for (it3 = Shadings.begin(); it3 != Shadings.end(); ++it3)
						PutDoc("/"+it3.key()+" "+QString::number(it3.data())+" 0 R\n");
					PutDoc(">>\n");
				}
				if (Transpar.count() != 0)
				{
					PutDoc("/ExtGState << \n");
					QMap<QString,int>::Iterator it3t;
					for (it3t = Transpar.begin(); it3t != Transpar.end(); ++it3t)
						PutDoc("/"+it3t.key()+" "+QString::number(it3t.data())+" 0 R\n");
					PutDoc(">>\n");
				}
				if ((ICCProfiles.count() != 0) || (spotMap.count() != 0))
				{
					PutDoc("/ColorSpace << \n");
					QMap<QString,ICCD>::Iterator it3c;
					if (ICCProfiles.count() != 0)
					{
						for (it3c = ICCProfiles.begin(); it3c != ICCProfiles.end(); ++it3c)
							PutDoc("/"+it3c.data().ResName+" "+QString::number(it3c.data().ResNum)+" 0 R\n");
					}
					QMap<QString,SpotC>::Iterator it3sc;
					if (spotMap.count() != 0)
					{
					for (it3sc = spotMap.begin(); it3sc != spotMap.end(); ++it3sc)
						PutDoc("/"+it3sc.data().ResName+" "+QString::number(it3sc.data().ResNum)+" 0 R\n");
					}
					PutDoc(">>\n");
				}
				PutDoc(">>\n");
				if ((Options.Compress) && (CompAvail))
					Inhalt = CompressStr(&Inhalt);
				PutDoc("/Length "+QString::number(Inhalt.length()+1));
				if ((Options.Compress) && (CompAvail))
					PutDoc("\n/Filter /FlateDecode");
				PutDoc(" >>\nstream\n"+EncStream(Inhalt, ObjCounter-1)+"\nendstream\nendobj\n");
				QString name = pag->PageNam.simplifyWhiteSpace().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ) + QString::number(ite->ItemNr);
				Seite.XObjects[name] = ObjCounter-1;
				}
				if ((Options.Version == 15) && (Options.useLayers))
					PutPage("EMC\n");
			}
		Lnr++;
	}
}

void PDFlib::PDF_Begin_Page(const Page* pag, QPixmap pm)
{
	QString tmp;
	ActPageP = pag;
	Inhalt = "";
	Seite.AObjects.clear();
	if (Options.Thumbnails)
	{
		ScImage img = pm.convertToImage();
		QString im = img.ImageToTxt();
		if ((Options.Compress) && (CompAvail))
			im = CompressStr(&im);
		StartObj(ObjCounter);
		PutDoc("<<\n/Width "+QString::number(img.width())+"\n");
		PutDoc("/Height "+QString::number(img.height())+"\n");
		PutDoc("/ColorSpace /DeviceRGB\n/BitsPerComponent 8\n");
		PutDoc("/Length "+QString::number(im.length()+1)+"\n");
		if ((Options.Compress) && (CompAvail))
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(im, ObjCounter)+"\nendstream\nendobj\n");
		Seite.Thumb = ObjCounter;
		ObjCounter++;
	}
}

void PDFlib::PDF_End_Page()
{
	uint PgNr =  ActPageP->pageNr();
	Seite.ObjNum = ObjCounter;
	WritePDFStream(Inhalt);
	StartObj(ObjCounter);
	PutDoc("<<\n/Type /Page\n/Parent 4 0 R\n");
	PutDoc("/MediaBox [0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+"]\n");
	PutDoc("/TrimBox ["+FToStr(Options.BleedLeft)+" "+FToStr(Options.BleedBottom)+
		" "+FToStr(ActPageP->width()-Options.BleedRight)+" "+FToStr(ActPageP->height()-Options.BleedTop)+"]\n");
	PutDoc("/Rotate "+QString::number(Options.RotateDeg)+"\n");
	PutDoc("/Contents "+QString::number(Seite.ObjNum)+" 0 R\n");
	if (Options.Thumbnails)
		PutDoc("/Thumb "+QString::number(Seite.Thumb)+" 0 R\n");
	if (Seite.AObjects.count() != 0)
	{
		PutDoc("/Annots [ ");
		for (uint b = 0; b < Seite.AObjects.count(); ++b)
			PutDoc(QString::number(Seite.AObjects[b])+" 0 R ");
		PutDoc("]\n");
	}
	if (Options.PresentMode)
	{
		if (Options.PresentVals[PgNr].pageViewDuration > 0)
			PutDoc("/Dur "+QString::number(Options.PresentVals[PgNr].pageViewDuration)+"\n");
		if (Options.PresentVals[PgNr].effectType != 0)
		{
			PutDoc("/Trans << /Type /Trans\n");
			PutDoc("/D "+QString::number(Options.PresentVals[PgNr].pageEffectDuration)+"\n");
			switch (Options.PresentVals[PgNr].effectType)
			{
				case 1:
					PutDoc("/S /Blinds\n");
					PutDoc(Options.PresentVals[PgNr].Dm == 0 ? "/Dm /H\n" : "/Dm /V\n");
					break;
				case 2:
					PutDoc("/S /Box\n");
					PutDoc(Options.PresentVals[PgNr].M == 0 ? "/M /I\n" : "/M /O\n");
					break;
				case 3:
					PutDoc("/S /Dissolve\n");
					break;
				case 4:
					PutDoc("/S /Glitter\n");
					PutDoc("/Di ");
					switch (Options.PresentVals[PgNr].Di)
					{
						case 0:
							PutDoc("0");
							break;
						case 1:
							PutDoc("270");
							break;
						case 4:
							PutDoc("315");
							break;
						default:
							PutDoc("0");
							break;
					}
					PutDoc("\n");
					break;
				case 5:
					PutDoc("/S /Split\n");
					PutDoc(Options.PresentVals[PgNr].Dm == 0 ? "/Dm /H\n" : "/Dm /V\n");
					PutDoc(Options.PresentVals[PgNr].M == 0 ? "/M /I\n" : "/M /O\n");
					break;
				case 6:
					PutDoc("/S /Wipe\n");
					PutDoc("/Di ");
					switch (Options.PresentVals[PgNr].Di)
					{
						case 0:
							PutDoc("0");
							break;
						case 1:
							PutDoc("270");
							break;
						case 2:
							PutDoc("90");
							break;
						case 3:
							PutDoc("180");
							break;
						default:
							PutDoc("0");
							break;
					}
					PutDoc("\n");
					break;
			}
			PutDoc(">>\n");
		}
	}
	PutDoc(">>\nendobj\n");
	PageTree.Count++;
	PageTree.Kids.append(ObjCounter);
	ObjCounter++;
}

void PDFlib::PDF_ProcessPage(const Page* pag, uint PNr, bool clip)
{
	QString tmp;
	ActPageP = pag;
	PageItem* ite;
	QPtrList<PageItem> PItems;
	int Lnr = 0;
	struct Layer ll;
	ll.isPrintable = false;
	ll.LNr = 0;
	if (Options.UseLPI)
		PutPage("/"+HTName+" gs\n");
	if ( (Options.MirrorH) && (!pag->MPageNam.isEmpty()) )
		PutPage("-1 0 0 1 "+FToStr(ActPageP->width())+" 0 cm\n");
	if ( (Options.MirrorV) && (!pag->MPageNam.isEmpty()) )
		PutPage("1 0 0 -1 0 "+FToStr(ActPageP->height())+" cm\n");
	if (clip)
	{
		PutPage(FToStr(pag->Margins.Left) + " " + FToStr(pag->Margins.Bottom) + " m\n");
		PutPage(FToStr(ActPageP->width() - pag->Margins.Right) + " " + FToStr(pag->Margins.Bottom) + " l\n");
		PutPage(FToStr(ActPageP->width() - pag->Margins.Right) + " " + FToStr(ActPageP->height() - pag->Margins.Top) + " l\n");
		PutPage(FToStr(pag->Margins.Left) + " " + FToStr(ActPageP->height() - pag->Margins.Top) + " l h W n\n");
	}
	else
		PutPage("0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" re W n\n");

	if (!pag->MPageNam.isEmpty())
	{
		Page* mPage = doc.MasterPages.at(doc.MasterNames[doc.Pages->at(PNr)->MPageNam]);
		if (doc.MasterItems.count() != 0)
		{
			if (!Options.MirrorH)
				PutPage("1 0 0 1 0 0 cm\n");
			for (uint lam = 0; lam < doc.Layers.count(); ++lam)
			{
				Level2Layer(&doc, &ll, Lnr);
				Lnr++;
				if (ll.isPrintable)
				{
					if ((Options.Version == 15) && (Options.useLayers))
						PutPage("/OC /"+OCGEntries[ll.Name].Name+" BDC\n");
					for (uint am = 0; am < pag->FromMaster.count(); ++am)
					{
						ite = pag->FromMaster.at(am);
						if ((ite->LayerNr != ll.LNr) || (!ite->printEnabled()))
							continue;
						if ((!pag->PageNam.isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
							continue;
						QString name = "/"+pag->MPageNam.simplifyWhiteSpace().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ) + QString::number(ite->ItemNr);
						if (! ite->asTextFrame())
							PutPage(name+" Do\n");
						else
						{
							PutPage("q\n");
							if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= 14))
								PutPage(PDF_Transparenz(ite));
							if (ite->fillColor() != CommonStrings::None)
								PutPage(putColor(ite->fillColor(), ite->fillShade(), true));
							if (ite->lineColor() != CommonStrings::None)
								PutPage(putColor(ite->lineColor(), ite->lineShade(), false));
							Inhalt += FToStr(fabs(ite->lineWidth()))+" w\n";
							if (ite->DashValues.count() != 0)
							{
								PutPage("[ ");
								QValueList<double>::iterator it;
								for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
								{
									int da = static_cast<int>(*it);
									if (da != 0)
										PutPage(QString::number(da)+" ");
									PutPage(QString::number(static_cast<int>(*it))+" ");
								}
							}
							else
							{
								QString Dt = FToStr(QMAX(2*fabs(ite->lineWidth()), 1));
								QString Da = FToStr(QMAX(6*fabs(ite->lineWidth()), 1));
								switch (ite->PLineArt)
								{
									case Qt::SolidLine:
										PutPage("[] 0 d\n");
										break;
									case Qt::DashLine:
										PutPage("["+Da+" "+Dt+"] 0 d\n");
										break;
									case Qt::DotLine:
										PutPage("["+Dt+"] 0 d\n");
										break;
									case Qt::DashDotLine:
										PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
										break;
									case Qt::DashDotDotLine:
										PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
										break;
									default:
										PutPage("[] 0 d\n");
										break;
								}
							}
							PutPage("2 J\n");
							switch (ite->PLineJoin)
							{
								case Qt::MiterJoin:
									PutPage("0 j\n");
									break;
								case Qt::BevelJoin:
									PutPage("2 j\n");
									break;
								case Qt::RoundJoin:
									PutPage("1 j\n");
									break;
								default:
									PutPage("0 j\n");
									break;
							}
							PutPage("1 0 0 1 "+FToStr(ite->xPos() - mPage->xOffset())+" "+FToStr(mPage->height() - (ite->yPos()  - mPage->yOffset()))+" cm\n");
							if (ite->rotation() != 0)
							{
								double sr = sin(-ite->rotation()* M_PI / 180.0);
								double cr = cos(-ite->rotation()* M_PI / 180.0);
								if ((cr * cr) < 0.000001)
									cr = 0;
								if ((sr * sr) < 0.000001)
									sr = 0;
								PutPage(FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+" 0 0 cm\n");
							}
							if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
							{
								if (ite->GrType != 0)
									PutPage(PDF_Gradient(ite));
								else
								{
									PutPage(SetClipPath(ite));
									PutPage("h\nf*\n");
								}
							}
							PutPage("q\n");
							if (ite->imageFlippedH())
								PutPage("-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n");
							if (ite->imageFlippedV())
								PutPage("1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n");
							PutPage(setTextSt(ite, PNr, pag));
							PutPage("Q\n");
							if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty())) && (!ite->isTableItem))
							{
								if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
								{
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
								}
								else
								{
									multiLine ml = doc.MLineStyles[ite->NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage(SetClipPath(ite));
										PutPage("h\nS\n");
									}
								}
							}
							PutPage("Q\n");
						}
					}
					for (uint am = 0; am < pag->FromMaster.count(); ++am)
					{
						ite = pag->FromMaster.at(am);
						if ((ite->LayerNr != ll.LNr) || (!ite->printEnabled()))
							continue;
						if (ite->ChangedMasterItem)
							continue;
						if ((!pag->PageNam.isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
							continue;
						if (!ite->isTableItem)
							continue;
						PutPage("q\n");
						if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= 14))
							PutPage(PDF_Transparenz(ite));
						if (ite->fillColor() != CommonStrings::None)
							PutPage(putColor(ite->fillColor(), ite->fillShade(), true));
						if (ite->lineColor() != CommonStrings::None)
							PutPage(putColor(ite->lineColor(), ite->lineShade(), false));
						Inhalt += FToStr(fabs(ite->lineWidth()))+" w\n";
						if (ite->DashValues.count() != 0)
						{
							PutPage("[ ");
							QValueList<double>::iterator it;
							for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
							{
								int da = static_cast<int>(*it);
								if (da != 0)
									PutPage(QString::number(da)+" ");
								PutPage(QString::number(static_cast<int>(*it))+" ");
							}
						}
						else
						{
							QString Dt = FToStr(QMAX(2*fabs(ite->lineWidth()), 1));
							QString Da = FToStr(QMAX(6*fabs(ite->lineWidth()), 1));
							switch (ite->PLineArt)
							{
								case Qt::SolidLine:
									PutPage("[] 0 d\n");
									break;
								case Qt::DashLine:
									PutPage("["+Da+" "+Dt+"] 0 d\n");
									break;
								case Qt::DotLine:
									PutPage("["+Dt+"] 0 d\n");
									break;
								case Qt::DashDotLine:
									PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
									break;
								case Qt::DashDotDotLine:
									PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
									break;
								default:
									PutPage("[] 0 d\n");
									break;
							}
						}
						PutPage("2 J\n");
						switch (ite->PLineJoin)
						{
							case Qt::MiterJoin:
								PutPage("0 j\n");
								break;
							case Qt::BevelJoin:
								PutPage("2 j\n");
								break;
							case Qt::RoundJoin:
								PutPage("1 j\n");
								break;
							default:
								PutPage("0 j\n");
								break;
						}
						PutPage("1 0 0 1 "+FToStr(ite->xPos() - mPage->xOffset())+" "+FToStr(mPage->height() - (ite->yPos()  - mPage->yOffset()))+" cm\n");
						if (ite->rotation() != 0)
						{
							double sr = sin(-ite->rotation()* M_PI / 180.0);
							double cr = cos(-ite->rotation()* M_PI / 180.0);
							if ((cr * cr) < 0.000001)
								cr = 0;
							if ((sr * sr) < 0.000001)
								sr = 0;
							PutPage(FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+ " 0 0 cm\n");
						}
						if ((ite->TopLine) || (ite->RightLine) || (ite->BottomLine) || (ite->LeftLine))
						{
							if (ite->TopLine)
							{
								PutPage("0 0 m\n");
								PutPage(FToStr(ite->width())+" 0 l\n");
							}
							if (ite->RightLine)
							{
								PutPage(FToStr(ite->width())+" 0 m\n");
								PutPage(FToStr(ite->width())+" "+FToStr(-ite->height())+" l\n");
							}
							if (ite->BottomLine)
							{
								PutPage("0 "+FToStr(-ite->height())+" m\n");
								PutPage(FToStr(ite->width())+" "+FToStr(-ite->height())+" l\n");
							}
							if (ite->LeftLine)
							{
								PutPage("0 0 m\n");
								PutPage("0 "+FToStr(-ite->height())+" l\n");
							}
							PutPage("S\n");
						}
						PutPage("Q\n");
					}
					if ((Options.Version == 15) && (Options.useLayers))
						PutPage("EMC\n");
				}
			}
		}
	}
	ll.isPrintable = false;
	ll.LNr = 0;
	Lnr = 0;
	//CB *2 because the Pitems count loop runs twice.. y.. dunno.
	if (usingGUI && pag->PageNam.isEmpty())
		progressDialog->setProgress("ECPI", 0, doc.DocItems.count()*2);
	int pc_exportpagesitems=0;
	for (uint la = 0; la < doc.Layers.count() && !abortExport; ++la)
	{
		Level2Layer(&doc, &ll, Lnr);
		if (!pag->PageNam.isEmpty())
			PItems = doc.MasterItems;
		else
			PItems = doc.DocItems;
		if (ll.isPrintable)
		{
			if ((Options.Version == 15) && (Options.useLayers))
				PutPage("/OC /"+OCGEntries[ll.Name].Name+" BDC\n");
			for (uint a = 0; a < PItems.count() && !abortExport; ++a)
			{
				if (usingGUI)
				{
					progressDialog->setProgress("ECPI", ++pc_exportpagesitems);
					ScQApp->processEvents();
				}
				ite = PItems.at(a);
				if (ite->LayerNr != ll.LNr)
					continue;
				PutPage(PDF_ProcessItem(ite, pag, PNr));
			}
				for (uint a = 0; a < PItems.count() && !abortExport; ++a)
				{
					if (usingGUI)
					{
						progressDialog->setProgress("ECPI", ++pc_exportpagesitems);
						ScQApp->processEvents();
					}
					ite = PItems.at(a);
					if (ite->LayerNr != ll.LNr)
						continue;
					if (!ite->isTableItem)
						continue;
					double x = pag->xOffset();
					double y = pag->yOffset();
					double w = pag->width();
					double h1 = pag->height();
					double ilw=ite->lineWidth();
					double x2 = ite->BoundingX - ilw / 2.0;
					double y2 = ite->BoundingY - ilw / 2.0;
					double w2 = ite->BoundingW + ilw;
					double h2 = ite->BoundingH + ilw;
					if (!( QMAX( x, x2 ) <= QMIN( x+w, x2+w2 ) && QMAX( y, y2 ) <= QMIN( y+h1, y2+h2 )))
						continue;
					if (ite->ChangedMasterItem)
						continue;
					if ((!pag->PageNam.isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
						continue;
					PutPage("q\n");
					if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= 14))
						PutPage(PDF_Transparenz(ite));
					if (!ite->printEnabled())
					{
						PutPage("Q\n");
						continue;
					}
					if (ite->fillColor() != CommonStrings::None)
						PutPage(putColor(ite->fillColor(), ite->fillShade(), true));
					if (ite->lineColor() != CommonStrings::None)
						PutPage(putColor(ite->lineColor(), ite->lineShade(), false));
					Inhalt += FToStr(fabs(ite->lineWidth()))+" w\n";
					if (ite->DashValues.count() != 0)
					{
						PutPage("[ ");
						QValueList<double>::iterator it;
						for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
						{
							int da = static_cast<int>(*it);
							if (da != 0)
								PutPage(QString::number(da)+" ");
						}
						PutPage("] "+QString::number(static_cast<int>(ite->DashOffset))+" d\n");
					}
					else
					{
						QString Dt = FToStr(QMAX(2*fabs(ite->lineWidth()), 1));
						QString Da = FToStr(QMAX(6*fabs(ite->lineWidth()), 1));
						switch (ite->PLineArt)
						{
							case Qt::SolidLine:
								PutPage("[] 0 d\n");
								break;
							case Qt::DashLine:
								PutPage("["+Da+" "+Dt+"] 0 d\n");
								break;
							case Qt::DotLine:
								PutPage("["+Dt+"] 0 d\n");
								break;
							case Qt::DashDotLine:
								PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
								break;
							case Qt::DashDotDotLine:
								PutPage("["+Da+" "+Dt+" "+Dt+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n");
								break;
							default:
								PutPage("[] 0 d\n");
								break;
						}
					}
					PutPage("2 J\n");
					switch (ite->PLineJoin)
					{
						case Qt::MiterJoin:
							PutPage("0 j\n");
							break;
						case Qt::BevelJoin:
							PutPage("2 j\n");
							break;
						case Qt::RoundJoin:
							PutPage("1 j\n");
							break;
						default:
							PutPage("0 j\n");
							break;
					}
					PutPage("1 0 0 1 "+FToStr(ite->xPos() - pag->xOffset())+" "+FToStr(pag->height() - (ite->yPos()  - pag->yOffset()))+" cm\n");
					if (ite->rotation() != 0)
					{
						double sr = sin(-ite->rotation()* M_PI / 180.0);
						double cr = cos(-ite->rotation()* M_PI / 180.0);
						if ((cr * cr) < 0.000001)
							cr = 0;
						if ((sr * sr) < 0.000001)
							sr = 0;
						PutPage(FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+ " 0 0 cm\n");
					}
					if (ite->isTableItem)
					{
						if ((ite->TopLine) || (ite->RightLine) || (ite->BottomLine) || (ite->LeftLine))
						{
							if (ite->TopLine)
							{
								PutPage("0 0 m\n");
								PutPage(FToStr(ite->width())+" 0 l\n");
							}
							if (ite->RightLine)
							{
								PutPage(FToStr(ite->width())+" 0 m\n");
								PutPage(FToStr(ite->width())+" "+FToStr(-ite->height())+" l\n");
							}
							if (ite->BottomLine)
							{
								PutPage("0 "+FToStr(-ite->height())+" m\n");
								PutPage(FToStr(ite->width())+" "+FToStr(-ite->height())+" l\n");
							}
							if (ite->LeftLine)
							{
								PutPage("0 0 m\n");
								PutPage("0 "+FToStr(-ite->height())+" l\n");
							}
							PutPage("S\n");
						}
					}
					PutPage("Q\n");
				}
				if ((Options.Version == 15) && (Options.useLayers))
					PutPage("EMC\n");
			}
		Lnr++;
	}
}

QString PDFlib::PDF_ProcessItem(PageItem* ite, const Page* pag, uint PNr, bool embedded)
{
	QString tmp("");
	double x = pag->xOffset();
	double y = pag->yOffset();
	double w = pag->width();
	double h1 = pag->height();
	double ilw=ite->lineWidth();
	double x2 = ite->BoundingX - ilw / 2.0;
	double y2 = ite->BoundingY - ilw / 2.0;
	double w2 = ite->BoundingW + ilw;
	double h2 = ite->BoundingH + ilw;
	if (!( QMAX( x, x2 ) <= QMIN( x+w, x2+w2 ) && QMAX( y, y2 ) <= QMIN( y+h1, y2+h2 )))
		return tmp;
	if (ite->ChangedMasterItem)
		return tmp;
	if ((!pag->PageNam.isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
		return tmp;
	tmp += "q\n";
	if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= 14))
		tmp += PDF_Transparenz(ite);
	if ((ite->isBookmark) && (Options.Bookmarks))
		PDF_Bookmark(ite, pag->height() - (ite->yPos() - pag->yOffset()));
	if (!ite->printEnabled() || ((ite->itemType() == PageItem::TextFrame) && (!pag->PageNam.isEmpty())))
	{
		tmp += "Q\n";
		return tmp;
	}
	if (ite->fillColor() != CommonStrings::None)
		tmp += putColor(ite->fillColor(), ite->fillShade(), true);
	if (ite->lineColor() != CommonStrings::None)
		tmp += putColor(ite->lineColor(), ite->lineShade(), false);
	tmp += FToStr(fabs(ite->lineWidth()))+" w\n";
	if (ite->DashValues.count() != 0)
	{
		tmp += "[ ";
		QValueList<double>::iterator it;
		for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
		{
			int da = static_cast<int>(*it);
			if (da != 0)
				tmp += QString::number(da)+" ";
		}
		tmp += "] "+QString::number(static_cast<int>(ite->DashOffset))+" d\n";
	}
	else
	{
		QString Dt = FToStr(QMAX(2*fabs(ite->lineWidth()), 1));
		QString Da = FToStr(QMAX(6*fabs(ite->lineWidth()), 1));
		switch (ite->PLineArt)
		{
			case Qt::SolidLine:
				tmp += "[] 0 d\n";
				break;
			case Qt::DashLine:
				tmp += "["+Da+" "+Dt+"] 0 d\n";
				break;
			case Qt::DotLine:
				tmp += "["+Dt+"] 0 d\n";
				break;
			case Qt::DashDotLine:
				tmp += "["+Da+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n";
				break;
			case Qt::DashDotDotLine:
				tmp += "["+Da+" "+Dt+" "+Dt+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n";
				break;
			default:
				tmp += "[] 0 d\n";
				break;
		}
	}
	switch (ite->PLineEnd)
	{
		case Qt::FlatCap:
			tmp += "0 J\n";
			break;
		case Qt::SquareCap:
			tmp += "2 J\n";
			break;
		case Qt::RoundCap:
			tmp += "1 J\n";
			break;
		default:
			tmp += "0 J\n";
			break;
	}
	switch (ite->PLineJoin)
	{
		case Qt::MiterJoin:
			tmp += "0 j\n";
			break;
		case Qt::BevelJoin:
			tmp += "2 j\n";
			break;
		case Qt::RoundJoin:
			tmp += "1 j\n";
			break;
		default:
			tmp += "0 j\n";
			break;
	}
	if (!embedded)
	{
		tmp += "1 0 0 1 "+FToStr(ite->xPos() - pag->xOffset())+" "+FToStr(pag->height() - (ite->yPos()  - pag->yOffset()))+" cm\n";
	}
	if (ite->rotation() != 0)
	{
		double sr = sin(-ite->rotation()* M_PI / 180.0);
		double cr = cos(-ite->rotation()* M_PI / 180.0);
		if ((cr * cr) < 0.000001)
			cr = 0;
		if ((sr * sr) < 0.000001)
			sr = 0;
		tmp += FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+" 0 0 cm\n";
	}
	switch (ite->itemType())
	{
		case PageItem::ImageFrame:
			if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
			{
				if (ite->GrType != 0)
					tmp += PDF_Gradient(ite);
				else
				{
					tmp += SetClipPath(ite);
					tmp += "h\nf*\n";
				}
			}
			tmp += "q\n";
			if (ite->imageClip.size() != 0)
			{
				tmp += SetClipPathImage(ite);
				tmp += "h\nW*\nn\n";
			}
			tmp += SetClipPath(ite);
			tmp += "h\nW*\nn\n";
			if (ite->imageFlippedH())
				tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
			if (ite->imageFlippedV())
				tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
			if ((ite->PicAvail) && (!ite->Pfile.isEmpty()))
				tmp += PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), false, ite->IProfile, ite->UseEmbedded, ite->IRender);
			tmp += "Q\n";
			if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty())) && (!ite->isTableItem))
			{
				if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
				{
					tmp += SetClipPath(ite);
					tmp += "h\nS\n";
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						tmp += setStrokeMulti(&ml[it]);
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
			}
			break;
		case PageItem::TextFrame:
			if ((ite->isAnnotation()) && (Options.Version != 12))
			{
				PDF_Annotation(ite, PNr);
				break;
				}
				if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
			{
				if (ite->GrType != 0)
					tmp += PDF_Gradient(ite);
				else
				{
					tmp += SetClipPath(ite);
					tmp += "h\nf*\n";
				}
			}
			tmp += "q\n";
			if (ite->imageFlippedH())
				tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
			if (ite->imageFlippedV())
				tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
			tmp += setTextSt(ite, PNr, pag);
			tmp += "Q\n";
			if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty())) && (!ite->isTableItem))
			{
				if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
				{
					tmp += SetClipPath(ite);
					tmp += "h\nS\n";
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						tmp += setStrokeMulti(&ml[it]);
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
			}
			break;
		case PageItem::Line:
			if (ite->NamedLStyle.isEmpty())
			{
				tmp += "0 0 m\n";
				tmp += FToStr(ite->width())+" 0 l\n";
				tmp += "S\n";
			}
			else
			{
				multiLine ml = doc.MLineStyles[ite->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					tmp += setStrokeMulti(&ml[it]);
					tmp += "0 0 m\n";
					tmp += FToStr(ite->width())+" 0 l\n";
					tmp += "S\n";
				}
			}
			if (ite->startArrowIndex() != 0)
			{
				QWMatrix arrowTrans;
				FPointArray arrow = (*doc.arrowStyles.at(ite->startArrowIndex()-1)).points.copy();
				arrowTrans.translate(0, 0);
				arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
				arrowTrans.scale(-1,1);
				arrow.map(arrowTrans);
				if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
				{
					StartObj(ObjCounter);
					QString ShName = ResNam+QString::number(ResCount);
					Transpar[ShName] = ObjCounter;
					ResCount++;
					ObjCounter++;
					PutDoc("<< /Type /ExtGState\n");
					PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
					PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
					PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
					PutDoc("/BM /Normal\n>>\nendobj\n");
					tmp += "/"+ShName+" gs\n";
				}
				tmp += putColor(ite->lineColor(), ite->lineShade(), true);
				tmp += SetClipPathArray(&arrow);
				tmp += "h\nf*\n";
			}
			if (ite->endArrowIndex() != 0)
			{
				QWMatrix arrowTrans;
				FPointArray arrow = (*doc.arrowStyles.at(ite->endArrowIndex()-1)).points.copy();
				arrowTrans.translate(ite->width(), 0);
				arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
				arrow.map(arrowTrans);
				if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
				{
					StartObj(ObjCounter);
					QString ShName = ResNam+QString::number(ResCount);
					Transpar[ShName] = ObjCounter;
					ResCount++;
					ObjCounter++;
					PutDoc("<< /Type /ExtGState\n");
					PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
					PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
					PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
					PutDoc("/BM /Normal\n>>\nendobj\n");
					tmp += "/"+ShName+" gs\n";
				}
				tmp += putColor(ite->lineColor(), ite->lineShade(), true);
				tmp += SetClipPathArray(&arrow);
				tmp += "h\nf*\n";
			}
			break;
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
			if (ite->GrType != 0)
				tmp += PDF_Gradient(ite);
			else
			{
				if (ite->fillColor() != CommonStrings::None)
				{
					tmp += SetClipPath(ite);
					if (ite->fillRule)
						tmp += "h\nf*\n";
					else
						tmp += "h\nf\n";
				}
			}
			if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()))
			{
				if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
				{
					tmp += SetClipPath(ite);
					tmp += "h\nS\n";
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						tmp += setStrokeMulti(&ml[it]);
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
			}
			break;
		case PageItem::PolyLine:
			if (ite->PoLine.size() > 4)  // && ((ite->PoLine.point(0) != ite->PoLine.point(1)) || (ite->PoLine.point(2) != ite->PoLine.point(3))))
			{
				if (ite->GrType != 0)
					tmp += PDF_Gradient(ite);
				else
				{
					if (ite->fillColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += "h\nf*\n";
					}
				}
			}
			if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()))
			{
				if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
				{
					tmp += SetClipPath(ite, false);
					tmp += "S\n";
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						tmp += setStrokeMulti(&ml[it]);
						tmp += SetClipPath(ite, false);
						tmp += "S\n";
					}
				}
			}
			if (ite->startArrowIndex() != 0)
			{
				FPoint Start = ite->PoLine.point(0);
				for (uint xx = 1; xx < ite->PoLine.size(); xx += 2)
				{
					FPoint Vector = ite->PoLine.point(xx);
					if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
					{
						double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
						QWMatrix arrowTrans;
						FPointArray arrow = (*doc.arrowStyles.at(ite->startArrowIndex()-1)).points.copy();
						arrowTrans.translate(Start.x(), Start.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
						arrow.map(arrowTrans);
						if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
						{
							StartObj(ObjCounter);
							QString ShName = ResNam+QString::number(ResCount);
							Transpar[ShName] = ObjCounter;
							ResCount++;
							ObjCounter++;
							PutDoc("<< /Type /ExtGState\n");
							PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
							PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
							PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
							PutDoc("/BM /Normal\n>>\nendobj\n");
							tmp += "/"+ShName+" gs\n";
						}
						tmp += putColor(ite->lineColor(), ite->lineShade(), true);
						tmp += SetClipPathArray(&arrow);
						tmp += "h\nf*\n";
						break;
					}
				}
			}
			if (ite->endArrowIndex() != 0)
			{
				FPoint End = ite->PoLine.point(ite->PoLine.size()-2);
				for (uint xx = ite->PoLine.size()-1; xx > 0; xx -= 2)
				{
					FPoint Vector = ite->PoLine.point(xx);
					if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
					{
						double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
						QWMatrix arrowTrans;
						FPointArray arrow = (*doc.arrowStyles.at(ite->endArrowIndex()-1)).points.copy();
						arrowTrans.translate(End.x(), End.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
						arrow.map(arrowTrans);
						if ((ite->lineTransparency() != 0) && (Options.Version >= 14))
						{
							StartObj(ObjCounter);
							QString ShName = ResNam+QString::number(ResCount);
							Transpar[ShName] = ObjCounter;
							ResCount++;
							ObjCounter++;
							PutDoc("<< /Type /ExtGState\n");
							PutDoc("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n");
							PutDoc("/ca "+FToStr(1.0 - ite->lineTransparency())+"\n");
							PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
							PutDoc("/BM /Normal\n>>\nendobj\n");
							tmp += "/"+ShName+" gs\n";
						}
						tmp += putColor(ite->lineColor(), ite->lineShade(), true);
						tmp += SetClipPathArray(&arrow);
						tmp += "h\nf*\n";
						break;
					}
				}
			}
			break;
		case PageItem::PathText:
			if (ite->PoShow)
			{
				if (ite->PoLine.size() > 3)
				{
					tmp += "q\n";
					if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()))
					{
						if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
						{
							tmp += SetClipPath(ite, false);
							tmp += "S\n";
						}
						else
						{
							multiLine ml = doc.MLineStyles[ite->NamedLStyle];
							for (int it = ml.size()-1;
								it > -1; it--)
								{
								tmp += setStrokeMulti(&ml[it]);
								tmp += SetClipPath(ite, false);
								tmp += "S\n";
								}
						}
					}
					tmp += "Q\n";
				}
			}
			tmp += setTextSt(ite, PNr, pag);
			break;
	}
	tmp += "Q\n";
	return tmp;
}

QString PDFlib::putColor(const QString& color, int shade, bool fill)
{
	QString tmp = "";
	QString colString = SetFarbe(color, shade);
	ScColor tmpC;
	tmpC = doc.PageColors[color];
	if (((tmpC.isSpotColor()) || (tmpC.isRegistrationColor())) && ((Options.isGrayscale == false) && (Options.UseRGB == false))  && (Options.UseSpotColors))
	{
		if (color != CommonStrings::None)
		{
			if (fill)
			{
				tmp += "/"+spotMap[color].ResName+" cs\n";
				tmp += FToStr(shade / 100.0)+" scn\n";
			}
			else
			{
				tmp += "/"+spotMap[color].ResName+" CS\n";
				tmp += FToStr(shade / 100.0)+" SCN\n";
			}
		}
		return tmp;
	}
	if (Options.isGrayscale)
	{
		if (color != CommonStrings::None)
		{
			if (fill)
				tmp += colString+" g\n";
			else
				tmp += colString+" G\n";
		}
		return tmp;
	}
	if (Options.UseRGB)
	{
		if (color != CommonStrings::None)
		{
			if (fill)
				tmp += colString+" rg\n";
			else
				tmp += colString+" RG\n";
		}
	}
	else
	{
#ifdef HAVE_CMS
		if ((CMSuse) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				if (color != CommonStrings::None)
				{
					if (fill)
						tmp += colString+" k\n";
					else
						tmp += colString+" K\n";
				}
			}
			else
			{
				QString tmp2[] = {"/Perceptual", "/RelativeColorimetric", "/Saturation", "/AbsoluteColorimetric"};
				tmp += tmp2[Options.Intent]+ " ri\n";
				if (color != CommonStrings::None)
				{
					if (fill)
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" cs\n";
						tmp += colString+" scn\n";
					}
					else
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" CS\n";
						tmp += colString+" SCN\n";
					}
				}
			}
		}
		else
		{
#endif
			if (color != CommonStrings::None)
			{
				if (fill)
					tmp += colString+" k\n";
				else
					tmp += colString+" K\n";
			}
#ifdef HAVE_CMS
		}
#endif
	}
	return tmp;
}

/*CB 2982: cache code is borked somehow, original function is above
QString PDFlib::putColor(const QString & colorName, int shade, bool fill)
{
	// Cache of last foreground and background colours We cache fg and bg
	// separately because they're alternated so much.  The primary purpose of
	// this cache is to avoid re-caculating the fg and bg colors on each char
	// of text when the color doens't change.
	static QString lastFGColorName, lastFGOutput, lastBGColorName, lastBGOutput;
	static int lastFGShade = -1, lastBGShade = -1;
	if (fill && colorName == lastBGColorName && shade == lastBGShade)
		return lastBGOutput;
	else if (colorName == lastFGColorName && shade == lastFGShade)
		return lastFGOutput;
	// Cache miss, generate the color
	else if (fill)
	{
		lastBGColorName = colorName;
		lastBGShade = shade;
		lastBGOutput = putColorUncached(colorName, shade, fill);
		return lastBGOutput;
	}
	else
	{
		lastFGColorName = colorName;
		lastFGShade = shade;
		lastFGOutput = putColorUncached(colorName, shade, fill);
		return lastFGOutput;
	}
}
*/

QString PDFlib::putColorUncached(const QString& color, int shade, bool fill)
{
	ScColor tmpC(doc.PageColors[color]);
	if (((tmpC.isSpotColor()) || (tmpC.isRegistrationColor())) && ((Options.isGrayscale == false) && (Options.UseRGB == false))  && (Options.UseSpotColors))
	{
		QString tmpSpot("");
		if (color != CommonStrings::None)
		{
			if (fill)
			{
				tmpSpot += "/"+spotMap[color].ResName+" cs\n";
				tmpSpot += FToStr(shade / 100.0)+" scn\n";
			}
			else
			{
				tmpSpot += "/"+spotMap[color].ResName+" CS\n";
				tmpSpot += FToStr(shade / 100.0)+" SCN\n";
			}
		}
		return tmpSpot;
	}
	QString colString(SetFarbe(color, shade));
	if (Options.isGrayscale)
	{
		QString tmpGray("");
		if (color != CommonStrings::None)
		{
			if (fill)
				tmpGray += colString+" g\n";
			else
				tmpGray += colString+" G\n";
		}
		return tmpGray;
	}
	QString tmp("");
	if (Options.UseRGB)
	{
		if (color != CommonStrings::None)
		{
			if (fill)
				tmp += colString+" rg\n";
			else
				tmp += colString+" RG\n";
		}
	}
	else
	{
#ifdef HAVE_CMS
		if ((CMSuse) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				if (color != CommonStrings::None)
				{
					if (fill)
						tmp += colString+" k\n";
					else
						tmp += colString+" K\n";
				}
			}
			else
			{
				QString tmp2[] = {"/Perceptual", "/RelativeColorimetric", "/Saturation", "/AbsoluteColorimetric"};
				tmp += tmp2[Options.Intent]+ " ri\n";
				if (color != CommonStrings::None)
				{
					if (fill)
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" cs\n";
						tmp += colString+" scn\n";
					}
					else
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" CS\n";
						tmp += colString+" SCN\n";
					}
				}
			}
		}
		else
		{
#endif
			if (color != CommonStrings::None)
			{
				if (fill)
					tmp += colString+" k\n";
				else
					tmp += colString+" K\n";
			}
#ifdef HAVE_CMS
		}
#endif
	}
	return tmp;
}

QString PDFlib::setStrokeMulti(struct SingleLine *sl)
{
	QString tmp(
			putColor(sl->Color, sl->Shade, false) +
			FToStr(sl->Width)+" w\n"
			);
	QString Dt = FToStr(QMAX(2*sl->Width, 1));
	QString Da = FToStr(QMAX(6*sl->Width, 1));
	switch (static_cast<PenStyle>(sl->Dash))
	{
		case Qt::SolidLine:
			tmp += "[] 0 d\n";
			break;
		case Qt::DashLine:
			tmp += "["+Da+" "+Dt+"] 0 d\n";
			break;
		case Qt::DotLine:
			tmp += "["+Dt+"] 0 d\n";
			break;
		case Qt::DashDotLine:
			tmp += "["+Da+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n";
			break;
		case Qt::DashDotDotLine:
			tmp += "["+Da+" "+Dt+" "+Dt+" "+Dt+" "+Dt+" "+Dt+"] 0 d\n";
			break;
		default:
			tmp += "[] 0 d\n";
			break;
		}
	switch (static_cast<PenCapStyle>(sl->LineEnd))
	{
		case Qt::FlatCap:
			tmp += "0 J\n";
			break;
		case Qt::SquareCap:
			tmp += "2 J\n";
			break;
		case Qt::RoundCap:
			tmp += "1 J\n";
			break;
		default:
			tmp += "0 J\n";
			break;
	}
	switch (static_cast<PenJoinStyle>(sl->LineJoin))
	{
		case Qt::MiterJoin:
			tmp += "0 j\n";
			break;
		case Qt::BevelJoin:
			tmp += "2 j\n";
			break;
		case Qt::RoundJoin:
			tmp += "1 j\n";
			break;
		default:
			tmp += "0 j\n";
			break;
	}
	return tmp;
}

// Return a PDF substring representing a PageItem's text
QString PDFlib::setTextSt(PageItem *ite, uint PNr, const Page* pag)
{
	QString tmp("");
	QString tmp2("");
	uint tabCc = 0;
	QValueList<PageItem::TabRecord> tTabValues;
	double tabDist=ite->textToFrameDistLeft();
	if (ite->lineColor() != CommonStrings::None)
		tabDist += ite->lineWidth() / 2.0;
	if (ite->itemType() == PageItem::TextFrame)
		tmp += "BT\n";
	// Loop over each character (!) in the pageItem...
	for (uint d = 0; d < ite->MaxChars; ++d)
	{
		const ScText * const hl = ite->itemText.at(d);
		if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || (hl->ch == QChar(26)))
			continue;
		if (hl->cstyle & 4096)
			continue;
		if ((hl->yp == 0) && (ite->asTextFrame()))
			continue;
		if (hl->cab < 5)
			tTabValues = ite->TabValues;
		else
			tTabValues = doc.docParagraphStyles[hl->cab].TabValues;
		if (hl->cstyle & 16384)
			tabCc = 0;
		if ((hl->ch == QChar(9)) && (tTabValues.count() != 0))
		{
			if ((!tTabValues[tabCc].tabFillChar.isNull()) && (tabCc < tTabValues.count()))
			{
				ScText hl2;
				double wt = Cwidth(&doc, hl->cfont, QString(tTabValues[tabCc].tabFillChar), hl->csize);
				int coun = static_cast<int>((hl->xp - tabDist) / wt);
				double sPos = hl->xp - (hl->xp - tabDist) + 1;
				hl2.ch = QString(tTabValues[tabCc].tabFillChar);
				hl2.ccolor = hl->ccolor;
				hl2.cstroke = hl->cstroke;
				hl2.cshade = hl->cshade;
				hl2.cshade2 = hl->cshade2;
				hl2.yp = hl->yp;
				hl2.cselect = hl->cselect;
				hl2.csize = hl->csize;
				hl2.cstyle = hl->cstyle;
				hl2.cfont = hl->cfont;
				hl2.cextra = 0;
				hl2.cscale = 1000;
				hl2.cscalev = 1000;
				hl2.cbase = hl->cbase;
				hl2.cshadowx = hl->cshadowx;
				hl2.cshadowy = hl->cshadowy;
				hl2.coutline = hl->coutline;
				hl2.cunderpos = hl->cunderpos;
				hl2.cunderwidth = hl->cunderwidth;
				hl2.cstrikepos = hl->cstrikepos;
				hl2.cstrikewidth = hl->cstrikewidth;
				for (int cx = 0; cx < coun; ++cx)
				{
					hl2.xp =  sPos + wt * cx;
					if ((hl2.cstyle & 256) && (hl2.cstroke != CommonStrings::None))
					{
						ScText hl3;
						hl3.ch = hl2.ch;
						hl3.ccolor = hl2.cstroke;
						hl3.cstroke = hl2.cstroke;
						hl3.cshade = hl2.cshade2;
						hl3.cshade2 = hl2.cshade2;
						hl3.yp = hl2.yp - (hl2.csize * hl2.cshadowy / 10000.0);
						hl3.xp = hl2.xp + (hl2.csize * hl2.cshadowx / 10000.0);
						hl3.csize = hl2.csize;
						hl3.cstyle = hl2.cstyle;
						hl3.cfont = hl2.cfont;
						hl3.cscale = hl2.cscale;
						hl3.cscalev = hl2.cscalev;
						hl3.cextra = hl2.cextra;
						hl3.cbase = hl2.cbase;
						hl3.coutline = hl2.coutline;
						hl3.cunderpos = hl2.cunderpos;
						hl3.cunderwidth = hl2.cunderwidth;
						hl3.cstrikepos = hl2.cstrikepos;
						hl3.cstrikewidth = hl2.cstrikewidth;
						setTextCh(ite, PNr, d, tmp, tmp2, &hl3, pag);
					}
					setTextCh(ite, PNr, d, tmp, tmp2, &hl2, pag);
				}
				tabCc++;
				continue;
			}
			else
			{
				tabCc++;
				continue;
			}
		}
		if (hl->ch == QChar(9))
			continue;
		if ((hl->cstyle & 256) && (hl->cstroke != CommonStrings::None))
		{
			ScText hl2;
			hl2.ch = hl->ch;
			hl2.ccolor = hl->cstroke;
			hl2.cstroke = hl->cstroke;
			hl2.cshade = hl->cshade2;
			hl2.cshade2 = hl->cshade2;
			hl2.yp = hl->yp - (hl->csize * hl->cshadowy / 10000.0);
			hl2.xp = hl->xp + (hl->csize * hl->cshadowx / 10000.0);
			hl2.csize = hl->csize;
			hl2.cstyle = hl->cstyle;
			hl2.cfont = hl->cfont;
			hl2.cscale = hl->cscale;
			hl2.cscalev = hl->cscalev;
			hl2.cextra = hl->cextra;
			hl2.cbase = hl->cbase;
			hl2.cshadowx = hl->cshadowx;
			hl2.cshadowy = hl->cshadowy;
			hl2.coutline = hl->coutline;
			hl2.cunderpos = hl->cunderpos;
			hl2.cunderwidth = hl->cunderwidth;
			hl2.cstrikepos = hl->cstrikepos;
			hl2.cstrikewidth = hl->cstrikewidth;
			setTextCh(ite, PNr, d, tmp, tmp2, &hl2, pag);
		}
		setTextCh(ite, PNr, d, tmp, tmp2, hl, pag);
		tabDist = hl->xp + Cwidth(&doc, hl->cfont, hl->ch, hl->csize) * (hl->cscale / 1000.0);
	}
	if (ite->itemType() == PageItem::TextFrame)
		tmp += "ET\n"+tmp2;
	return tmp;
}

void PDFlib::setTextCh(PageItem *ite, uint PNr, uint d, QString &tmp, QString &tmp2, const ScText *hl, const Page* pag)
{
	QString FillColor = "";
	QString StrokeColor = "";
	if (ite->asPathText())
	{
		tmp += "q\n";
		QWMatrix trafo = QWMatrix( 1, 0, 0, -1, -hl->PRot, 0 );
		trafo *= QWMatrix( hl->PtransX, -hl->PtransY, -hl->PtransY, -hl->PtransX, hl->xp, -hl->yp );
		tmp += FToStr(trafo.m11())+" "+FToStr(trafo.m12())+" "+FToStr(trafo.m21())+" "+FToStr(trafo.m22())+" "+FToStr(trafo.dx())+" "+FToStr(trafo.dy())+" cm\n";
		if (ite->BaseOffs != 0)
			tmp += "1 0 0 1 0 "+FToStr( -ite->BaseOffs)+" cm\n";
		tmp += "BT\n";
	}
	int tsz = hl->csize;
	QString chx = hl->ch;
	if (hl->cstyle & 2048)
	{
		if (doc.docParagraphStyles[hl->cab].BaseAdj)
			tsz = qRound(10 * ((doc.typographicSettings.valueBaseGrid * (doc.docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (doc.docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(&doc, hl->cfont, chx, 10))));
		else
		{
			if (doc.docParagraphStyles[hl->cab].LineSpaMode == 0)
				tsz = qRound(10 * ((doc.docParagraphStyles[hl->cab].LineSpa *  (doc.docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (doc.docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(&doc, hl->cfont, chx, 10))));
			else
			{
				double currasce = RealFHeight(&doc, hl->cfont, doc.docParagraphStyles[hl->cab].FontSize);
				tsz = qRound(10 * ((currasce * (doc.docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (doc.docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCHeight(&doc, hl->cfont, chx, 10)));
			}
		}
	}
	if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
	{
		QPtrList<PageItem> emG;
		emG.clear();
		emG.append(hl->cembedded);
		if (hl->cembedded->Groups.count() != 0)
		{
			for (uint ga=0; ga<doc.FrameItems.count(); ++ga)
			{
				if (doc.FrameItems.at(ga)->Groups.count() != 0)
				{
					if (doc.FrameItems.at(ga)->Groups.top() == hl->cembedded->Groups.top())
					{
						if (doc.FrameItems.at(ga)->ItemNr != hl->cembedded->ItemNr)
						{
							if (emG.find(doc.FrameItems.at(ga)) == -1)
								emG.append(doc.FrameItems.at(ga));
						}
					}
				}
			}
		}
		for (uint em = 0; em < emG.count(); ++em)
		{
			PageItem* embedded = emG.at(em);
			tmp2 += "q\n";
			tmp2 +=  FToStr(hl->cscale / 1000.0)+" 0 0 "+FToStr(hl->cscalev / 1000.0)+" "+FToStr(hl->xp + embedded->gXpos * (hl->cscale / 1000.0))+" "+FToStr(-hl->yp + (embedded->gHeight * (hl->cscalev / 1000.0)) - embedded->gYpos * (hl->cscalev / 1000.0)+embedded->gHeight * (hl->cbase / 1000.0))+" cm\n";
			tmp2 += PDF_ProcessItem(embedded, pag, PNr, true);
			tmp2 += "Q\n";
		}
		return;
	}
	if (hl->ch == QChar(29))
		chx = " ";
	if (hl->ch == QChar(24))
		chx = "-";
	if (hl->ch == QChar(0xA0))
		chx = " ";
	if (hl->ch == QChar(30))
	{
		//FIXME Stop duplicating PageItem::ExpandToken code!!!
		uint zae = 0;
		uint za2 = d;
		do
		{
			if (za2 == 0)
				break;
			za2--;
		}
		while (ite->itemText.at(za2)->ch == QChar(30));
		if (ite->itemText.at(za2)->ch != QChar(30))
			za2++;
		while (ite->itemText.at(za2+zae)->ch == QChar(30))
		{
			zae++;
			if (za2+zae == ite->MaxChars)
				break;
		}
		QString out="%1";
		QString out2;
		//CB Section numbering
		//out2 = out.arg(PNr+doc.FirstPnum, -zae);
		out2=out.arg(doc.getSectionPageNumberForPageIndex(PNr), -zae);
		chx = out2.mid(d-za2, 1);
	}
	uint cc = chx[0].unicode();
	uint idx = 0;
	if (GlyphsIdxOfFont[hl->cfont->scName()].contains(cc))
		idx = GlyphsIdxOfFont[hl->cfont->scName()][cc].Code;
	uint idx1 = (idx >> 8) & 0xFF;
	if (hl->cstyle & 32)
	{
		if (chx.upper() != chx)
			chx = chx.upper();
	}
	if (hl->cstyle & 64)
	{
		if (chx.upper() != chx)
		{
			tsz = hl->csize * doc.typographicSettings.valueSmallCaps / 100;
			chx = chx.upper();
		}
	}
	if (hl->cstyle & 1)
		tsz = hl->csize * doc.typographicSettings.scalingSuperScript / 100;
	if (hl->cstyle & 2)
		tsz = hl->csize * doc.typographicSettings.scalingSubScript / 100;
	if (hl->cstroke != CommonStrings::None)
	{
		StrokeColor = "";
		StrokeColor += putColor(hl->cstroke, hl->cshade2, false);
	}
	if (hl->ccolor != CommonStrings::None)
	{
		FillColor = "";
		FillColor += putColor(hl->ccolor, hl->cshade, true);
	}
	if ((hl->cfont->isOTF) || (!hl->cfont->HasNames) || (hl->cfont->Subset) || (Options.SubsetList.contains(hl->cfont->scName())))
	{
		uint chr = chx[0].unicode();
		if ((hl->cfont->CharWidth.contains(chr)) && (chr != 32))
		{
			if ((hl->cstroke != CommonStrings::None) && (hl->cstyle & 4))
			{
				tmp2 += FToStr((tsz * hl->coutline / 1000.0) / tsz)+" w\n[] 0 d\n0 J\n0 j\n";
				tmp2 += StrokeColor;
			}
			if (hl->ccolor != CommonStrings::None)
				tmp2 += FillColor;
			tmp2 += "q\n";
			if (ite->itemType() == PageItem::PathText)
			{
				QWMatrix trafo = QWMatrix( 1, 0, 0, -1, -hl->PRot, 0 );
				trafo *= QWMatrix( hl->PtransX, -hl->PtransY, -hl->PtransY, -hl->PtransX, hl->xp, -hl->yp );
				tmp2 += FToStr(trafo.m11())+" "+FToStr(trafo.m12())+" "+FToStr(trafo.m21())+" "+FToStr(trafo.m22())+" "+FToStr(trafo.dx())+" "+FToStr(trafo.dy())+" cm\n";
			}
			if (!ite->asPathText())
			{
				if (ite->reversed())
				{
					double wid = Cwidth(&doc, hl->cfont, chx, hl->csize) * (hl->cscale / 1000.0);
					tmp2 += "1 0 0 1 "+FToStr(hl->xp)+" "+FToStr((hl->yp - (tsz / 10.0)) * -1 + ((tsz / 10.0) * (hl->cbase / 1000.0)))+" cm\n";
					tmp2 += "-1 0 0 1 0 0 cm\n";
					tmp2 += "1 0 0 1 "+FToStr(-wid)+" 0 cm\n";
					tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" 0 0 cm\n";
				}
				else
				{
					tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" "+FToStr(hl->xp)+" "+FToStr((hl->yp - (tsz / 10.0)) * -1 + ((tsz / 10.0) * (hl->cbase / 1000.0)))+" cm\n";
				}
			}
			else
			{
				if (ite->BaseOffs != 0)
					tmp2 += "1 0 0 1 0 "+FToStr( -ite->BaseOffs)+" cm\n";
				tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" 0 "+FToStr(tsz / 10.0)+" cm\n";
			}
			if (hl->cscalev != 1000)
				tmp2 += "1 0 0 1 0 "+FToStr( (((tsz / 10.0) - (tsz / 10.0) * (hl->cscalev / 1000.0)) / (tsz / 10.0)) * -1)+" cm\n";
			tmp2 += FToStr(QMIN(QMAX(hl->cscale, 100), 4000) / 1000.0)+" 0 0 "+FToStr(QMIN(QMAX(hl->cscalev, 100), 4000) / 1000.0)+" 0 0 cm\n";
			if (hl->ccolor != CommonStrings::None)
				tmp2 += "/"+hl->cfont->RealName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+QString::number(chr)+" Do\n";
			if (hl->cstyle & 4)
			{
				FPointArray gly = hl->cfont->GlyphArray[chr].Outlines.copy();
				QWMatrix mat;
				mat.scale(0.1, 0.1);
				gly.map(mat);
				bool nPath = true;
				FPoint np;
				if (gly.size() > 3)
				{
					for (uint poi=0; poi<gly.size()-3; poi += 4)
					{
						if (gly.point(poi).x() > 900000)
						{
							tmp2 += "h\n";
							nPath = true;
							continue;
						}
						if (nPath)
						{
							np = gly.point(poi);
							tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
							nPath = false;
						}
						np = gly.point(poi+1);
						tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" ";
						np = gly.point(poi+3);
						tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" ";
						np = gly.point(poi+2);
						tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" c\n";
					}
				}
				tmp2 += "s\n";
			}
			if (hl->cstyle & 8192)
			{
				int chs = hl->csize;
				double wtr = Cwidth(&doc, hl->cfont, chx, chs) * (hl->cscale / 1000.0);
				tmp2 += "1 0 0 1 "+FToStr(wtr / (tsz / 10.0))+" 0 cm\n";
				chx = "-";
				chr = chx[0].unicode();
				FPointArray gly = hl->cfont->GlyphArray[chr].Outlines.copy();
				QWMatrix mat;
				mat.scale(0.1, 0.1);
				gly.map(mat);
				bool nPath = true;
				FPoint np;
				if (gly.size() > 3)
				{
					for (uint poi=0; poi<gly.size()-3; poi += 4)
					{
						if (gly.point(poi).x() > 900000)
						{
							tmp2 += "h\n";
							nPath = true;
							continue;
						}
						if (nPath)
						{
							np = gly.point(poi);
							tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
							nPath = false;
						}
						np = gly.point(poi+1);
						tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" ";
						np = gly.point(poi+3);
						tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" ";
						np = gly.point(poi+2);
						tmp2 += FToStr(np.x())+" "+FToStr(-np.y())+" c\n";
					}
				}
				tmp2 += "f*\n";
			}
			tmp2 += "Q\n";
		}
	}
	else
	{
		cc = chx[0].unicode();
		idx = 0;
		if (GlyphsIdxOfFont[hl->cfont->scName()].contains(cc))
			idx = GlyphsIdxOfFont[hl->cfont->scName()][cc].Code;
		idx1 = (idx >> 8) & 0xFF;
		tmp += UsedFontsP[hl->cfont->scName()]+"S"+QString::number(idx1)+" "+FToStr(tsz / 10.0)+" Tf\n";
		if (hl->cstroke != CommonStrings::None)
		{
			tmp += StrokeColor;
			if ((hl->cstyle & 8) || (hl->cstyle & 16))
				tmp2 += StrokeColor;
		}
		if (hl->ccolor != CommonStrings::None)
		{
			tmp += FillColor;
			if ((hl->cstyle & 8) || (hl->cstyle & 16))
				tmp2 += FillColor;
		}
		if (hl->cstyle & 4)
			tmp += FToStr(tsz * hl->coutline / 10000.0) + (hl->ccolor != CommonStrings::None ? " w 2 Tr\n" : " w 1 Tr\n");
		else
			tmp += "0 Tr\n";
		if (!ite->asPathText())
		{
			if (ite->reversed())
			{
				int chs = hl->csize;
				double wtr;
				if (d < ite->MaxChars-1)
				{
					QString ctx = ite->itemText.at(d+1)->ch;
					if (ctx == QChar(29))
						ctx = " ";
					if (ctx == QChar(0xA0))
						ctx = " ";
					wtr = Cwidth(&doc, hl->cfont, chx, chs, ctx) * (hl->cscale / 1000.0);
				}
				else
					wtr = Cwidth(&doc, hl->cfont, chx, chs) * (hl->cscale / 1000.0);
				tmp +=  FToStr(-QMIN(QMAX(hl->cscale, 100), 4000) / 1000.0)+" 0 0 "+FToStr(QMIN(QMAX(hl->cscalev, 100), 4000) / 1000.0)+" "+FToStr(hl->xp+wtr)+" "+FToStr(-hl->yp+(hl->csize / 10.0) * (hl->cbase / 1000.0))+" Tm\n";
//				tmp += "-1 0 0 1 "+FToStr(wtr)+" "+FToStr(0)+" Tm\n";
			}
			else
				tmp +=  FToStr(QMIN(QMAX(hl->cscale, 100), 4000) / 1000.0)+" 0 0 "+FToStr(QMIN(QMAX(hl->cscalev, 100), 4000) / 1000.0)+" "+FToStr(hl->xp)+" "+FToStr(-hl->yp+(hl->csize / 10.0) * (hl->cbase / 1000.0))+" Tm\n";
		}
		else
			tmp += FToStr(QMIN(QMAX(hl->cscale, 100), 4000) / 1000.0)+" 0 0 "+FToStr(QMIN(QMAX(hl->cscalev, 100), 4000) / 1000.0)+" 0 0 Tm\n";
		uchar idx2 = idx & 0xFF;
		tmp += "<"+QString(toHex(idx2))+"> Tj\n";
		if (! ite->asPathText())
		{
			if (hl->cstyle & 8192)
			{
				int chs = hl->csize;
				double wtr = Cwidth(&doc, hl->cfont, chx, chs) * (hl->cscale / 1000.0);
				tmp += "1 0 0 1 "+FToStr(hl->xp+wtr)+" "+FToStr(-hl->yp)+" Tm\n";
				chx = "-";
				cc = chx[0].unicode();
				idx = 0;
				if (GlyphsIdxOfFont[hl->cfont->scName()].contains(cc))
					idx = GlyphsIdxOfFont[hl->cfont->scName()][cc].Code;
				idx1 = (idx >> 8) & 0xFF;
				tmp += UsedFontsP[hl->cfont->scName()]+"S"+QString::number(idx1)+" "+FToStr(tsz / 10.0)+" Tf\n";
				idx2 = idx & 0xFF;
				tmp += "<"+QString(toHex(idx2))+"> Tj\n";
			}
		}
	}
	if (((hl->cstyle & 8) && (chx != QChar(13)))  || ((hl->cstyle & 512) && (!chx[0].isSpace())))
	{
		double Ulen = Cwidth(&doc, hl->cfont, chx, hl->csize) * (hl->cscale / 1000.0);
		double Upos, Uwid, kern;
		if (hl->cstyle & 16384)
			kern = 0;
		else
			kern = hl->csize * hl->cextra / 10000.0;
		if ((hl->cunderpos != -1) || (hl->cunderwidth != -1))
		{
			if (hl->cunderpos != -1)
				Upos = (hl->cunderpos / 1000.0) * (hl->cfont->numDescender * (hl->csize / 10.0));
			else
				Upos = hl->cfont->underline_pos * (hl->csize / 10.0);
			if (hl->cunderwidth != -1)
				Uwid = (hl->cunderwidth / 1000.0) * (hl->csize / 10.0);
			else
				Uwid = QMAX(hl->cfont->strokeWidth * (hl->csize / 10.0), 1);
		}
		else
		{
			Upos = hl->cfont->underline_pos * (hl->csize / 10.0);
			Uwid = QMAX(hl->cfont->strokeWidth * (hl->csize / 10.0), 1);
		}
		if (hl->cbase != 0)
			Upos += (hl->csize / 10.0) * (hl->cbase / 1000.0);
		if (hl->ccolor != CommonStrings::None)
			tmp2 += putColor(hl->ccolor, hl->cshade, false);
		tmp2 += FToStr(Uwid)+" w\n";
		tmp2 += FToStr(hl->xp-kern)+" "+FToStr(-hl->yp+Upos)+" m\n";
		tmp2 += FToStr(hl->xp+Ulen)+" "+FToStr(-hl->yp+Upos)+" l\n";
		tmp2 += "S\n";
	}
	if ((hl->cstyle & 16) && (chx != QChar(13)))
	{
		double Ulen = Cwidth(&doc, hl->cfont, chx, hl->csize) * (hl->cscale / 1000.0);
		double Upos, Uwid, kern;
		if (hl->cstyle & 16384)
			kern = 0;
		else
			kern = hl->csize * hl->cextra / 10000.0;
		if ((hl->cstrikepos != -1) || (hl->cstrikewidth != -1))
		{
			if (hl->cstrikepos != -1)
				Upos = (hl->cstrikepos / 1000.0) * (hl->cfont->numAscent * (hl->csize / 10.0));
			else
				Upos = hl->cfont->strikeout_pos * (hl->csize / 10.0);
			if (hl->cstrikewidth != -1)
				Uwid = (hl->cstrikewidth / 1000.0) * (hl->csize / 10.0);
			else
				Uwid = QMAX(hl->cfont->strokeWidth * (hl->csize / 10.0), 1);
		}
		else
		{
			Upos = hl->cfont->strikeout_pos * (hl->csize / 10.0);
			Uwid = QMAX(hl->cfont->strokeWidth * (hl->csize / 10.0), 1);
		}
		if (hl->cbase != 0)
			Upos += (hl->csize / 10.0) * (hl->cbase / 1000.0);
		if (hl->ccolor != CommonStrings::None)
			tmp2 += putColor(hl->ccolor, hl->cshade, false);
		tmp2 += FToStr(Uwid)+" w\n";
		tmp2 += FToStr(hl->xp-kern)+" "+FToStr(-hl->yp+Upos)+" m\n";
		tmp2 += FToStr(hl->xp+Ulen)+" "+FToStr(-hl->yp+Upos)+" l\n";
		tmp2 += "S\n";
	}
	if (ite->asPathText())
	{
		tmp += "ET\nQ\n"+tmp2;
		tmp2 = "";
	}
}

QString PDFlib::SetFarbe(const QString& farbe, int Shade)
{
	QString tmp;
	ScColor tmpC;
	int h, s, v, k;
	tmpC = doc.PageColors[farbe];
	QColor tmpR;
	if (Options.isGrayscale)
	{
		tmpR = tmpC.getShadeColorProof(Shade);
		tmpR.rgb(&h, &s, &v);
		tmp = FToStr((0.3 * h + 0.59 * s + 0.11 * v) / 255.0);
		return tmp;
	}
	if (Options.UseRGB)
	{
		tmpR = tmpC.getShadeColorProof(Shade);
		tmpR.rgb(&h, &s, &v);
		tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
	}
	else
	{
#ifdef HAVE_CMS
		if ((CMSuse) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
			}
			else
			{
				if (Options.SComp == 3)
				{
					tmpC.getShadeColorRGB(&h, &s, &v, Shade);
					tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
				}
				else
				{
					tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
					tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
				}
			}
		}
		else
		{
#endif
			tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
			tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
		}
#ifdef HAVE_CMS
	}
#endif
	return tmp;
}

QString PDFlib::SetFarbeGrad(const QString& farbe, int Shade)
{
	QString tmp;
	ScColor tmpC;
	int h, s, v, k;
	tmpC = doc.PageColors[farbe];
	QColor tmpR;
	if (Options.isGrayscale)
	{
		tmpR = tmpC.getShadeColorProof(Shade);
		tmpR.rgb(&h, &s, &v);
		tmp = FToStr((0.3 * h + 0.59 * s + 0.11 * v) / 255.0);
		return tmp;
	}
	if (Options.UseRGB)
	{
		tmpR = tmpC.getShadeColorProof(Shade);
		tmpR.rgb(&h, &s, &v);
		tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
	}
	else
	{
#ifdef HAVE_CMS
		if ((CMSuse) && (Options.UseProfiles))
		{
			if (Options.SComp == 3)
			{
				tmpC.getShadeColorRGB(&h, &s, &v, Shade);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
			}
			else
			{
				tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
			}
		}
		else
		{
#endif
			tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
			tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
		}
#ifdef HAVE_CMS
	}
#endif
	return tmp;
}

/*CB 2982: cache code is borked somehow, original function is above
QString PDFlib::SetFarbe(const QString& farbe, int Shade)
{
	// Cache last color
	static QString lastColorName;
	static QString lastColorData;
	if (farbe == lastColorName)
		return lastColorData;
	// Not cached, build the color and cache it
	lastColorName = farbe;
	QString tmp;
	ScColor tmpC(doc.PageColors[farbe]);
	int h, s, v, k;
	QColor tmpR;
	if (Options.isGrayscale)
	{
		tmpR = tmpC.getShadeColorProof(Shade);
		tmpR.rgb(&h, &s, &v);
		tmp = lastColorData = FToStr((0.3 * h + 0.59 * s + 0.11 * v) / 255.0);
		return tmp;
	}
	if (Options.UseRGB)
	{
		tmpR = tmpC.getShadeColorProof(Shade);
		tmpR.rgb(&h, &s, &v);
		tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
	}
	else
	{
#ifdef HAVE_CMS
		if ((CMSuse) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
			}
			else
			{
				if (Options.SComp == 3)
				{
					tmpC.getShadeColorRGB(&h, &s, &v, Shade);
					tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
				}
				else
				{
					tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
					tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
				}
			}
		}
		else
		{
#endif
			tmpC.getShadeColorCMYK(&h, &s, &v, &k, Shade);
			tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
		}
#ifdef HAVE_CMS
	}
#endif
	lastColorData = tmp;
	return tmp;
}
*/

QString PDFlib::SetClipPathImage(PageItem *ite)
{
	QString tmp("");
	if (ite->imageClip.size() > 3)
	{
		bool nPath = true;
		for (uint poi=0; poi<ite->imageClip.size()-3; poi += 4)
		{
			if (ite->imageClip.point(poi).x() > 900000)
			{
				tmp += "h\n";
				nPath = true;
				continue;
			}
			FPoint np;
			if (nPath)
			{
				np = ite->imageClip.point(poi);
				tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
				nPath = false;
			}
			np = ite->imageClip.point(poi+1);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" ";
			np = ite->imageClip.point(poi+3);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" ";
			np = ite->imageClip.point(poi+2);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" c\n";
		}
	}
	return tmp;
}

QString PDFlib::SetClipPath(PageItem *ite, bool poly)
{
	QString tmp("");
	if (ite->PoLine.size() > 3)
	{
		bool nPath = true;
		for (uint poi=0; poi<ite->PoLine.size()-3; poi += 4)
		{
			if (ite->PoLine.point(poi).x() > 900000)
			{
				if (poly)
					tmp += "h\n";
				nPath = true;
				continue;
			}
			FPoint np;
			if (nPath)
			{
				np = ite->PoLine.point(poi);
				tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
				nPath = false;
			}
			np = ite->PoLine.point(poi+1);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" ";
			np = ite->PoLine.point(poi+3);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" ";
			np = ite->PoLine.point(poi+2);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" c\n";
		}
	}
	return tmp;
}

QString PDFlib::SetClipPathArray(FPointArray *ite, bool poly)
{
	QString tmp("");
	if (ite->size() > 3)
	{
		bool nPath = true;
		for (uint poi=0; poi<ite->size()-3; poi += 4)
		{
			if (ite->point(poi).x() > 900000)
			{
				if (poly)
					tmp += "h\n";
				nPath = true;
				continue;
			}
			FPoint np;
			if (nPath)
			{
				np = ite->point(poi);
				tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
				nPath = false;
			}
			np = ite->point(poi+1);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" ";
			np = ite->point(poi+3);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" ";
			np = ite->point(poi+2);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" c\n";
		}
	}
	return tmp;
}

QString PDFlib::PDF_Transparenz(PageItem *currItem)
{
	StartObj(ObjCounter);
	QString ShName = ResNam+QString::number(ResCount);
	Transpar[ShName] = ObjCounter;
	ResCount++;
	ObjCounter++;
	PutDoc("<< /Type /ExtGState\n");
	PutDoc("/CA "+FToStr(1.0 - currItem->lineTransparency())+"\n");
	PutDoc("/ca "+FToStr(1.0 - currItem->fillTransparency())+"\n");
	PutDoc("/SMask /None\n/AIS false\n/OPM 1\n");
	PutDoc("/BM /Normal\n>>\nendobj\n");
	QString tmp("/"+ShName+" gs\n");
	return tmp;
}

QString PDFlib::PDF_Gradient(PageItem *currItem)
{
	double w = currItem->width();
	double h = -currItem->height();
	double w2 = w / 2.0;
	double h2 = h / 2.0;
	double StartX = 0;
	double StartY = 0;
	double EndX = 0;
	double EndY =0;
	QValueList<double> StopVec;
	QValueList<double> TransVec;
	QStringList Gcolors;
	QPtrVector<VColorStop> cstops = currItem->fill_gradient.colorStops();
	switch (currItem->GrType)
	{
		case 1:
			StartX = 0;
			StartY = h2;
			EndX = w;
			EndY = h2;
			break;
		case 2:
			StartX = w2;
			StartY = 0;
			EndX = w2;
			EndY = h;
			break;
		case 3:
			StartX = 0;
			StartY = 0;
			EndX = w;
			EndY = h;
			break;
		case 4:
			StartX = 0;
			StartY = h;
			EndX = w;
			EndY = 0;
			break;
		case 5:
			StartX = w2;
			StartY = h2;
			if (w >= h)
			{
				EndX = w;
				EndY = h2;
			}
			else
			{
				EndX = w2;
				EndY = h;
			}
			break;
		case 6:
		case 7:
			StartX = QMIN(QMAX(currItem->GrStartX, 0), currItem->width());
			StartY = QMIN(QMAX(currItem->GrStartY, 0), currItem->height());
			EndX = QMIN(QMAX(currItem->GrEndX, 0), currItem->width());
			EndY = QMIN(QMAX(currItem->GrEndY, 0), currItem->height());
			break;
	}
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	if ((currItem->GrType == 5) || (currItem->GrType == 7))
	{
		for (uint cst = 0; cst < currItem->fill_gradient.Stops(); ++cst)
		{
			TransVec.prepend(cstops.at(cst)->opacity);
			StopVec.prepend(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))*cstops.at(cst)->rampPoint);
			Gcolors.prepend(SetFarbeGrad(cstops.at(cst)->name, cstops.at(cst)->shade));
		}
	}
	else
	{
		for (uint cst = 0; cst < currItem->fill_gradient.Stops(); ++cst)
		{
			QWMatrix ma;
			ma.translate(StartX, StartY);
			ma.rotate(atan2(EndY - StartY, EndX - StartX)*(180.0/M_PI));
			double w2 = sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))*cstops.at(cst)->rampPoint;
			double x = fabs(ma.m11() * w2 + ma.dx());
			double y = fabs(ma.m12() * w2 + ma.dy());
			TransVec.append(cstops.at(cst)->opacity);
			StopVec.append(x);
			StopVec.append(-y);
			Gcolors.append(SetFarbeGrad(cstops.at(cst)->name, cstops.at(cst)->shade));
		}
	}
	QString tmp(PDF_DoLinGradient(currItem, StopVec, TransVec, Gcolors));
	return tmp;
}

QString PDFlib::PDF_DoLinGradient(PageItem *currItem, QValueList<double> Stops, QValueList<double> Trans, const QStringList& Colors)
{
	QString tmp("");
	bool first = true;
	double w = currItem->width();
	double h = -currItem->height();
	double w2 = QMIN(QMAX(currItem->GrStartX, 0), currItem->width());
	double h2 = -1.0 * QMIN(QMAX(currItem->GrStartY, 0), currItem->height());
	uint colorsCountm1=Colors.count()-1;
	for (uint c = 0; c < colorsCountm1; ++c)
	{
		QString TRes("");
		if ((Options.Version >= 14) && (((*Trans.at(c+1)) != 1) || ((*Trans.at(c)) != 1)))
		{
			StartObj(ObjCounter);
			QString ShName = ResNam+QString::number(ResCount);
			Shadings[ShName] = ObjCounter;
			ResCount++;
			ObjCounter++;
			PutDoc("<<\n");
			if ((currItem->GrType == 5) || (currItem->GrType == 7))
				PutDoc("/ShadingType 3\n");
			else
				PutDoc("/ShadingType 2\n");
			PutDoc("/ColorSpace /DeviceGray\n");
			PutDoc("/BBox [0 "+FToStr(h)+" "+FToStr(w)+" 0]\n");
			if ((currItem->GrType == 5) || (currItem->GrType == 7))
			{
				PutDoc("/Coords ["+FToStr(w2)+" "+FToStr(h2)+" "+FToStr((*Stops.at(c+1)))+" "+FToStr(w2)+" "+FToStr(h2)+" "+FToStr((*Stops.at(c)))+"]\n");
				PutDoc("/Extend [true true]\n");
				PutDoc("/Function\n<<\n/FunctionType 2\n/Domain [0 1]\n");
				PutDoc("/C0 ["+FToStr((*Trans.at(c+1)))+"]\n");
				PutDoc("/C1 ["+FToStr((*Trans.at(c)))+"]\n");
			}
			else
			{
				PutDoc("/Coords ["+FToStr((*Stops.at(c*2)))+"  "+FToStr((*Stops.at(c*2+1)))+" "+FToStr((*Stops.at(c*2+2)))+" "+FToStr((*Stops.at(c*2+3)))+"]\n");
				PutDoc("/Extend [true true]\n");
				PutDoc("/Function\n<<\n/FunctionType 2\n/Domain [0 1]\n");
				PutDoc("/C0 ["+FToStr((*Trans.at(c)))+"]\n");
				PutDoc("/C1 ["+FToStr((*Trans.at(c+1)))+"]\n");
			}
			PutDoc("/N 1\n>>\n>>\nendobj\n");
			StartObj(ObjCounter);
			ObjCounter++;
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
			PutDoc("/FormType 1\n");
			PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
			PutDoc("/BBox [ 0 0 "+FToStr(currItem->width())+" "+FToStr(-currItem->height())+" ]\n");
			PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
			if (Shadings.count() != 0)
			{
				PutDoc("/Shading << \n");
				QMap<QString,int>::Iterator it3;
				for (it3 = Shadings.begin(); it3 != Shadings.end(); ++it3)
					PutDoc("/"+it3.key()+" "+QString::number(it3.data())+" 0 R\n");
				PutDoc(">>\n");
			}
			PutDoc(">>\n");
			QString stre = "";
			stre += "q\n"+SetClipPath(currItem)+"h\nW* n\n"+"/"+ShName+" sh\nQ\n";
			if ((Options.Compress) && (CompAvail))
				stre = CompressStr(&stre);
			PutDoc("/Length "+QString::number(stre.length())+"\n");
			if ((Options.Compress) && (CompAvail))
				PutDoc("/Filter /FlateDecode\n");
			PutDoc(">>\nstream\n"+EncStream(stre, ObjCounter-1)+"\nendstream\nendobj\n");
			Seite.XObjects[ResNam+QString::number(ResCount)] = ObjCounter-1;
			ResCount++;
			StartObj(ObjCounter);
			QString GXName = ResNam+QString::number(ResCount);
			Transpar[GXName] = ObjCounter;
			ResCount++;
			ObjCounter++;
			PutDoc("<< /Type /ExtGState\n");
			PutDoc("/SMask << /S /Luminosity /G "+QString::number(ObjCounter-2)+" 0 R >>\n");
			PutDoc("/BM /Normal\n>>\nendobj\n");
			TRes = GXName;
		}
		StartObj(ObjCounter);
		QString ShName = ResNam+QString::number(ResCount);
		Shadings[ShName] = ObjCounter;
		ResCount++;
		ObjCounter++;
		PutDoc("<<\n");
		if ((currItem->GrType == 5) || (currItem->GrType == 7))
			PutDoc("/ShadingType 3\n");
		else
			PutDoc("/ShadingType 2\n");
		if (Options.UseRGB)
			PutDoc("/ColorSpace /DeviceRGB\n");
		else
		{
			if (Options.isGrayscale)
				PutDoc("/ColorSpace /DeviceGray\n");
			else
#ifdef HAVE_CMS
			{
				if ((CMSuse) && (Options.UseProfiles))
					PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
				else
#endif
				PutDoc("/ColorSpace /DeviceCMYK\n");
#ifdef HAVE_CMS
			}
#endif
		}
		PutDoc("/BBox [0 "+FToStr(h)+" "+FToStr(w)+" 0]\n");
		if ((currItem->GrType == 5) || (currItem->GrType == 7))
		{
			PutDoc("/Coords ["+FToStr(w2)+" "+FToStr(h2)+" "+FToStr((*Stops.at(c+1)))+" "+FToStr(w2)+" "+FToStr(h2)+" "+FToStr((*Stops.at(c)))+"]\n");
			if (Colors.count() == 2)
				PutDoc("/Extend [true true]\n");
			else
			{
				if (first)
					PutDoc("/Extend [false true]\n");
				else
				{
					if (c == Colors.count()-2)
						PutDoc("/Extend [true false]\n");
					else
						PutDoc("/Extend [false false]\n");
				}
			}
			first = false;
			PutDoc("/Function\n<<\n/FunctionType 2\n/Domain [0 1]\n");
			PutDoc("/C0 ["+Colors[c+1]+"]\n");
			PutDoc("/C1 ["+Colors[c]+"]\n");
		}
		else
		{
			PutDoc("/Coords ["+FToStr((*Stops.at(c*2)))+"  "+FToStr((*Stops.at(c*2+1)))+" "+FToStr((*Stops.at(c*2+2)))+" "+FToStr((*Stops.at(c*2+3)))+"]\n");
			if (Colors.count() == 2)
				PutDoc("/Extend [true true]\n");
			else
			{
				if (first)
					PutDoc("/Extend [true false]\n");
				else
				{
					if (c == Colors.count()-2)
						PutDoc("/Extend [false true]\n");
					else
						PutDoc("/Extend [false false]\n");
				}
			}
			first = false;
			PutDoc("/Function\n<<\n/FunctionType 2\n/Domain [0 1]\n");
			PutDoc("/C0 ["+Colors[c]+"]\n");
			PutDoc("/C1 ["+Colors[c+1]+"]\n");
		}
		PutDoc("/N 1\n>>\n>>\nendobj\n");
		tmp += "q\n";
		if ((Options.Version == 14) && (((*Trans.at(c+1)) != 1) || ((*Trans.at(c)) != 1)))
			tmp += "/"+TRes+" gs\n";
		tmp += SetClipPath(currItem);
		tmp += "h\nW* n\n";
		tmp += "/"+ShName+" sh\nQ\n";
	}
	return tmp;
}

void PDFlib::PDF_Annotation(PageItem *ite, uint)
{
	ScImage img;
	ScImage img2;
	ScImage img3;
	QMap<int, QString> ind2PDFabr;
	static const QString bifonts[] = {"/Cour", "/CoBo", "/CoOb", "/CoBO", "/Helv", "/HeBo", "/HeOb", "/HeBO",
			"/TiRo", "/TiBo", "/TiIt", "/TiBI", "/ZaDb", "/Symb"};
	static const size_t ar = sizeof(bifonts) / sizeof(*bifonts);
	for (uint a = 0; a < ar; ++a)
		ind2PDFabr[a] = bifonts[a];
	double x = ite->xPos() - ActPageP->xOffset();
	double y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
	double x2 = x+ite->width();
	double y2 = y-ite->height();
	QString bm("");
	QString cc;
	for (uint d = 0; d < ite->itemText.count(); ++d)
	{
		cc = ite->itemText.at(d)->ch;
		if ((cc == "(") || (cc == ")") || (cc == "\\"))
			bm += "\\";
		if (cc == QChar(13))
			cc = "\\r";
		bm += cc;
	}
	QStringList bmst = QStringList::split("\\r", bm);
	const QString m[] = {"4", "5", "F", "l", "H", "n"};
	QString ct(m[ite->annotation().ChkStil()]);
	StartObj(ObjCounter);
	Seite.AObjects.append(ObjCounter);
	ObjCounter++;
	PutDoc("<<\n/Type /Annot\n");
	switch (ite->annotation().Type())
	{
		case 0:
		case 10:
			PutDoc("/Subtype /Text\n");
			PutDoc("/Contents "+EncString("("+bm+")",ObjCounter-1)+"\n");
			break;
		case 1:
		case 11:
			PutDoc("/Subtype /Link\n");
			if (ite->annotation().ActionType() == 2)
			{
				PutDoc("/Dest /"+NDnam+QString::number(NDnum)+"\n");
				Dest de;
				de.Name = NDnam+QString::number(NDnum);
				de.Seite = ite->annotation().Ziel();
				de.Act = ite->annotation().Action();
				NamedDest.append(de);
				NDnum++;
			}
			if (ite->annotation().ActionType() == 7)
			{
				PutDoc("/A << /Type /Action /S /GoToR\n/F "+ EncString("("+Path2Relative(ite->annotation().Extern())+")",ObjCounter-1)+"\n");
				PutDoc("/D ["+QString::number(ite->annotation().Ziel())+" /XYZ "+ite->annotation().Action()+"]\n>>\n");
			}
			if (ite->annotation().ActionType() == 8)
				PutDoc("/A << /Type /Action /S /URI\n/URI "+ EncString("("+ite->annotation().Extern()+")",ObjCounter-1)+"\n>>\n");
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			Seite.FormObjects.append(ObjCounter-1);
			PutDoc("/Subtype /Widget\n");
			PutDoc("/T "+EncString("("+ite->itemName()+")",ObjCounter-1)+"\n");
			if (!ite->annotation().ToolTip().isEmpty())
				PutDoc("/TU "+EncString("("+PDFEncode(ite->annotation().ToolTip())+")",ObjCounter-1)+"\n");
			PutDoc("/F ");
			QString mm[] = {"4", "2", "0", "32"};
			PutDoc(mm[ite->annotation().Vis()]);
			PutDoc("\n");
			PutDoc("/BS << /Type /Border /W ");
			PutDoc(ite->annotation().borderColor() != CommonStrings::None ? QString::number(ite->annotation().Bwid()) : QString("0"));
			PutDoc(" /S /");
			const QString x[] = {"S", "D", "U", "B", "I"};
			PutDoc(x[ite->annotation().Bsty()]);
			PutDoc(" >>\n");
			QString cnx = "("+ind2PDFabr[ite->annotation().Font()]+" "+FToStr(ite->fontSize() / 10.0)+" Tf";
			if (ite->TxtFill != CommonStrings::None)
				cnx += " "+ putColor(ite->TxtFill, ite->ShTxtFill, true);
			if (ite->fillColor() != CommonStrings::None)
				cnx += " "+ putColor(ite->fillColor(), ite->fillShade(), false);
			cnx += ")";
			PutDoc("/DA "+EncString(cnx,ObjCounter-1)+"\n");
			int flg = ite->annotation().Flag();
			if (Options.Version == 13)
				flg = flg & 522247;
			PutDoc("/Ff "+QString::number(flg)+"\n");
			QString xs[] = {"N", "I", "O", "P"};
			switch (ite->annotation().Type())
			{
				case 2:
					PutDoc("/FT /Btn\n");
					PutDoc("/H /");
					PutDoc(xs[ite->annotation().Feed()]);
					PutDoc("\n");
					PutDoc("/Q 0\n");
					break;
				case 3:
					PutDoc("/FT /Tx\n");
					PutDoc("/V "+EncString("("+bm+")",ObjCounter-1)+"\n");
					PutDoc("/DV "+EncString("("+bm+")",ObjCounter-1)+"\n");
					PutDoc("/Q "+QString::number(QMIN(ite->textAlignment,2))+"\n");
					PutDoc("/AP << /N "+QString::number(ObjCounter)+" 0 R >>\n");
					if (ite->annotation().MaxChar() != -1)
						PutDoc("/MaxLen "+QString::number(ite->annotation().MaxChar())+"\n");
					break;
				case 4:
					PutDoc("/FT /Btn\n");
					PutDoc(ite->annotation().IsChk() ? "/V /Yes\n/DV /Yes\n/AS /Yes\n" :
								"/V /Off\n/DV /Off\n/AS /Off\n");
					PutDoc("/AP << /N << /Yes "+QString::number(ObjCounter)+" 0 R >> >>\n");
					break;
				case 5:
				case 6:
					PutDoc("/FT /Ch\n/V (");
					if (bmst.count() > 0)
						PutDoc(bmst[0]);
					PutDoc(")\n/DV ");
					cnx = "(";
					if (bmst.count() > 0)
						cnx += bmst[0];
					cnx += ")";
					PutDoc(EncString(cnx,ObjCounter-1)+"\n");
					PutDoc("/Opt [ ");
					for (uint bmc = 0; bmc < bmst.count(); ++bmc)
						PutDoc(EncString("("+bmst[bmc]+")",ObjCounter-1)+"\n");
					PutDoc("]\n");
					PutDoc("/AP << /N "+QString::number(ObjCounter)+" 0 R >>\n");
					break;
			}
			PutDoc("/MK << ");
			if ((ite->annotation().Type() == 5) || (ite->annotation().Type() == 6))
			{
				PutDoc("/BG [ 1 1 1 ] ");
				if (ite->annotation().borderColor() != CommonStrings::None)
					PutDoc("/BC [ "+SetFarbe(ite->annotation().borderColor(), 100)+" ] ");
			}
      			else
			{
				if (ite->fillColor() != CommonStrings::None)
					PutDoc("/BG [ "+SetFarbe(ite->fillColor(), ite->fillShade())+" ] ");
				if (ite->annotation().borderColor() != CommonStrings::None)
					PutDoc("/BC [ "+SetFarbe(ite->annotation().borderColor(), 100)+" ] ");
			}
			int IconOb = 0;
			switch (ite->annotation().Type())
			{
				case 2:
					PutDoc("/CA "+EncString("("+bm+")",ObjCounter-1)+" ");
					if (!ite->annotation().RollOver().isEmpty())
						PutDoc("/RC "+ EncString("("+PDFEncode(ite->annotation().RollOver())+")",ObjCounter-1)+" ");
					if (!ite->annotation().Down().isEmpty())
						PutDoc("/AC "+ EncString("("+PDFEncode(ite->annotation().Down())+")",ObjCounter-1)+" ");
					if (ite->annotation().UseIcons())
					{
						if (!ite->Pfile.isEmpty())
						{
							IconOb += ite->pixm.hasAlphaBuffer() ? 3 : 2;
							PutDoc("/I "+QString::number(ObjCounter+IconOb-1)+" 0 R ");
						}
						if (!ite->Pfile2.isEmpty())
						{
							img.LoadPicture(ite->Pfile2, "", 0, false, false, 1, 72);
							QString im = "";
							im = img3.getAlpha(ite->Pfile2, true, false);
							IconOb += !im.isEmpty() ? 3 : 2;
							im = "";
							PutDoc("/IX "+QString::number(ObjCounter+IconOb-1)+" 0 R ");
						}
						if (!ite->Pfile3.isEmpty())
						{
							img2.LoadPicture(ite->Pfile3, "", 0, false, false, 1, 72);
							QString im = "";
							im = img3.getAlpha(ite->Pfile3, true, false);
							IconOb += !im.isEmpty() ? 3 : 2;
							im = "";
							PutDoc("/RI "+QString::number(ObjCounter+IconOb-1)+" 0 R ");
						}
						PutDoc("/TP "+QString::number(ite->annotation().IPlace())+" ");
						PutDoc("/IF << /SW /");
						QString x[] = {"A", "S", "B", "N"};
						PutDoc(x[ite->annotation().ScaleW()]);
						PutDoc(" /S /");
						PutDoc(ite->imageXScale() != ite->imageYScale() ? "A" : "P");
						PutDoc(" /A [ ");
						if ((ite->width()/ite->imageXScale() - ite->pixm.width()) != 0)
						{
							if (ite->annotation().ScaleW() == 3)
								PutDoc(FToStr(QMAX(ite->imageXOffset() / (ite->width()/ite->imageXScale() - ite->pixm.width()), 0.01)));
							else
								PutDoc("0.5 ");
						}
						else
							PutDoc("0 ");
						if ((ite->height()/ite->imageYScale() - ite->pixm.height()) != 0)
						{
							if (ite->annotation().ScaleW() == 3)
								PutDoc(FToStr(QMAX(ite->imageYOffset() / (ite->height()/ite->imageYScale() - ite->pixm.height()), 0.01)));
							else
								PutDoc("0.5");
						}
						else
							PutDoc("0");
						PutDoc(" ] >> ");
					}
					break;
				case 6:
				case 5:
				case 3:
					break;
				case 4:
					PutDoc("/CA "+EncString("("+ct+")",ObjCounter-1)+" ");
					break;
			}
			if (ite->rotation() != 0)
				PutDoc("/R "+QString::number((abs(static_cast<int>(ite->rotation())) / 90)*90)+" ");
			PutDoc(">>\n");
			if ((ite->annotation().ActionType() != 0) || (ite->annotation().AAact()))
			{
				if (ite->annotation().ActionType() == 7)
				{
					PutDoc("/A << /Type /Action /S /GoToR\n/F "+ EncString("("+Path2Relative(ite->annotation().Extern())+")",ObjCounter-1)+ "\n");
					PutDoc("/D ["+QString::number(ite->annotation().Ziel())+" /XYZ "+ite->annotation().Action()+"]\n>>\n");
				}
				if (ite->annotation().ActionType() == 5)
					PutDoc("/A << /Type /Action /S /ImportData\n/F "+ EncString("("+ite->annotation().Action()+")",ObjCounter-1)+" >>\n");
				if (ite->annotation().ActionType() == 4)
					PutDoc("/A << /Type /Action /S /ResetForm >>\n");
				if (ite->annotation().ActionType() == 3)
				{
					PutDoc("/A << /Type /Action /S /SubmitForm\n/F << /FS /URL /F "+ EncString("("+ite->annotation().Action()+")",ObjCounter-1)+" >>\n");
					if (ite->annotation().HTML())
						PutDoc("/Flags 4");
					PutDoc(">>\n");
				}
				if (ite->annotation().ActionType() == 1)
				{
					if (!ite->annotation().Action().isEmpty())
					{
						PutDoc("/A << /Type /Action /S /JavaScript /JS ");
						PutDoc(ite->annotation().Type() > 2 ? QString::number(ObjCounter+1+IconOb) :
						 QString::number(ObjCounter+IconOb));
						PutDoc(" 0 R >>\n");
					}
				}
				if (ite->annotation().AAact())
				{
					if (!ite->annotation().Action().isEmpty())
					{
						PutDoc("/A << /Type /Action /S /JavaScript /JS ");
						PutDoc(ite->annotation().Type() > 2 ? QString::number(ObjCounter+1+IconOb) :
						 QString::number(ObjCounter+IconOb));
						PutDoc(" 0 R >>\n");
					}
					PutDoc("/AA ");
					if (ite->annotation().Type() > 2)
						{
						if (!ite->annotation().Action().isEmpty())
							PutDoc(QString::number(ObjCounter+2+IconOb));
						else
							PutDoc(QString::number(ObjCounter+1+IconOb));
						}
					else
						{
						if (!ite->annotation().Action().isEmpty())
							PutDoc(QString::number(ObjCounter+1+IconOb));
						else
							PutDoc(QString::number(ObjCounter));
						}
					PutDoc(" 0 R\n");
					if (!ite->annotation().C_act().isEmpty())
						CalcFields.append(ObjCounter-1+IconOb);
				}
				if (ite->annotation().ActionType() == 2)
				{
					PutDoc("/A << /Type /Action /S /GoTo /D /"+NDnam+QString::number(NDnum)+" >>\n");
					Dest de;
					de.Name = NDnam+QString::number(NDnum);
					de.Seite = ite->annotation().Ziel();
					de.Act = ite->annotation().Action();
					NamedDest.append(de);
					NDnum++;
				}
			}
			break;
		}
	if ((ite->annotation().Type() < 2) || (ite->annotation().Type() > 9))
		PutDoc("/Border [ 0 0 0 ]\n");
	switch (((abs(static_cast<int>(ite->rotation())) / 90)*90))
	{
		case 0:
			break;
		case 90:
			x = ite->xPos() - ActPageP->xOffset();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			x2 = x + ite->height();
			y = y2 + ite->width();
			break;
		case 180:
			x = ite->xPos() - ActPageP->xOffset() - ite->width();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			x2 = ite->xPos() - ActPageP->xOffset();
			y = y2 + ite->height();
			break;
		case 270:
			x = ite->xPos() - ActPageP->xOffset() - ite->height();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset()) - ite->width();
			x2 = ite->xPos() - ActPageP->xOffset();
			y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			break;
	}
	PutDoc("/Rect [ "+FToStr(x)+" "+FToStr(y2)+" "+FToStr(x2)+" "+FToStr(y)+" ]\n");
	PutDoc(">>\nendobj\n");
	QMap<int, QString> ind2PDFabr2;
	const QString tmpf[] = {"/Courier", "/Courier-Bold", "/Courier-Oblique", "/Courier-BoldOblique",
												"/Helvetica", "/Helvetica-Bold", "/Helvetica-Oblique", "/Helvetica-BoldOblique",
												"/Times-Roman", "/Times-Bold", "/Times-Italic", "/Times-BoldItalic",
												"/ZapfDingbats", "/Symbol"};
	size_t ar2 = sizeof(tmpf) / sizeof(*tmpf);
	for (uint ax = 0; ax < ar2; ++ax)
		ind2PDFabr2[ax] = tmpf[ax];
	if ((ite->annotation().Type() == 2) && (ite->annotation().UseIcons()))
	{
		if (!ite->Pfile.isEmpty())
		{
			PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true);
			cc = QString::number(ite->pixm.width())+" 0 0 "+QString::number(ite->pixm.height())+" 0 0 cm\n";
			cc += "/"+ResNam+QString::number(ResCount-1)+" Do";
			PDF_xForm(ite->pixm.width(), ite->pixm.height(), cc);
		}
		if (!ite->Pfile2.isEmpty())
		{
			PDF_Image(ite, ite->Pfile2, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true);
			cc = QString::number(img.width())+" 0 0 "+QString::number(img.height())+" 0 0 cm\n";
			cc += "/"+ResNam+QString::number(ResCount-1)+" Do";
			PDF_xForm(img.width(), img.height(), cc);
		}
		if (!ite->Pfile3.isEmpty())
		{
			PDF_Image(ite, ite->Pfile3, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true);
			cc = QString::number(img2.width())+" 0 0 "+QString::number(img2.height())+" 0 0 cm\n";
			cc += "/"+ResNam+QString::number(ResCount-1)+" Do";
			PDF_xForm(img2.width(), img2.height(), cc);
		}
	}
	if (ite->annotation().Type() == 3)
	{
		cc = "";
		if (ite->fillColor() != CommonStrings::None)
			cc += putColor(ite->fillColor(), ite->fillShade(), false);
		cc += FToStr(x)+" "+FToStr(y2)+" "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += "/Tx BMC\nBT\n";
		if (ite->TxtFill != CommonStrings::None)
			cc += putColor(ite->TxtFill, ite->ShTxtFill, true);
		cc += "/"+StdFonts[ind2PDFabr2[ite->annotation().Font()]];
		cc += " "+FToStr(ite->fontSize() / 10.0)+" Tf\n";
		if (bmst.count() > 1)
		{
			cc += "1 0 0 1 0 0 Tm\n0 0 Td\n";
			for (uint mz = 0; mz < bmst.count(); ++mz)
			{
				cc += EncString("("+bmst[mz]+")",ObjCounter-1);
				cc += " Tj\nT*\n";
			}
			cc += "ET\nEMC";
		}
		else
			cc += "1 0 0 1 0 0 Tm\n0 0 Td\n"+EncString("("+bm+")",ObjCounter-1)+" Tj\nET\nEMC";
//		PDF_Form(cc);
		PDF_xForm(ite->width(), ite->height(), cc);
	}
	if (ite->annotation().Type() == 4)
	{
		cc = "q\nBT\n";
		if (ite->TxtFill != CommonStrings::None)
			cc += putColor(ite->TxtFill, ite->ShTxtFill, true);
		cc += "/ZaDb "+FToStr(ite->fontSize() / 10.0)+" Tf\n";
		cc += "0 0 Td\n("+ct+") Tj\nET\nQ";
//		PDF_Form(cc);
		PDF_xForm(ite->width(), ite->height(), cc);
	}
	if ((ite->annotation().Type() == 5) || (ite->annotation().Type() == 6))
	{
		cc = "";
		cc += "1 g\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += QString::number(ite->annotation().Bwid())+" w\n";
		if (ite->annotation().borderColor() != CommonStrings::None)
			cc += putColor(ite->annotation().borderColor(), 100l, false);
		else
			cc += "0 G\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nS\n";
		cc += "/Tx BMC\nq\nBT\n";
		cc += "0 g\n";
		cc += "/"+StdFonts[ind2PDFabr2[ite->annotation().Font()]];
//		cc += ind2PDFabr[ite->AnFont];
		cc += " "+FToStr(ite->fontSize() / 10.0)+" Tf\n";
		cc += "1 0 0 1 0 0 Tm\n0 0 Td\n";
		if (bmst.count() > 0)
			cc += EncString("("+bmst[0]+")",ObjCounter-1);
		cc += " Tj\nET\nQ\nEMC";
		PDF_xForm(ite->width(), ite->height(), cc);
	}
	if ((ite->annotation().Type() > 1) && ((ite->annotation().ActionType() == 1) || (ite->annotation().AAact())) && (!ite->annotation().Action().isEmpty()))
		WritePDFStream(ite->annotation().Action());
	if ((ite->annotation().Type() > 1) && (ite->annotation().AAact()))
	{
		StartObj(ObjCounter);
		ObjCounter++;
		PutDoc("<<\n");
		int AAcoun = 0;
		if (!ite->annotation().E_act().isEmpty())
		{
			PutDoc("/E << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+" 0 R >>\n");
			AAcoun++;
		}
		if (!ite->annotation().X_act().isEmpty())
		{
			PutDoc("/X << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+" 0 R >>\n");
			AAcoun++;
		}
		if (!ite->annotation().D_act().isEmpty())
		{
			PutDoc("/D << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+" 0 R >>\n");
			AAcoun++;
		}
		if (!ite->annotation().Fo_act().isEmpty())
		{
			PutDoc("/Fo << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+" 0 R >>\n");
			AAcoun++;
		}
		if (!ite->annotation().Bl_act().isEmpty())
		{
			PutDoc("/Bl << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+" 0 R >>\n");
			AAcoun++;
		}
		if ((ite->annotation().Type() == 3) || (ite->annotation().Type() == 5) || (ite->annotation().Type() == 6))
		{
			if (!ite->annotation().K_act().isEmpty())
			{
				PutDoc("/K << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+
					" 0 R >>\n");
				AAcoun++;
			}
			if (!ite->annotation().F_act().isEmpty())
			{
				PutDoc("/F << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+
					" 0 R >>\n");
				AAcoun++;
			}
			if (!ite->annotation().V_act().isEmpty())
			{
				PutDoc("/V << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+
					" 0 R >>\n");
				AAcoun++;
			}
			if (!ite->annotation().C_act().isEmpty())
			{
				PutDoc("/C << /Type /Action /S /JavaScript /JS "+QString::number(ObjCounter+AAcoun)+
					" 0 R >>\n");
				AAcoun++;
			}
		}
		PutDoc(">>\nendobj\n");
		if (!ite->annotation().E_act().isEmpty())
			WritePDFStream(ite->annotation().E_act());
		if (!ite->annotation().X_act().isEmpty())
			WritePDFStream(ite->annotation().X_act());
		if (!ite->annotation().D_act().isEmpty())
			WritePDFStream(ite->annotation().D_act());
		if (!ite->annotation().Fo_act().isEmpty())
			WritePDFStream(ite->annotation().Fo_act());
		if (!ite->annotation().Bl_act().isEmpty())
			WritePDFStream(ite->annotation().Bl_act());
		if ((ite->annotation().Type() == 3) || (ite->annotation().Type() == 5) || (ite->annotation().Type() == 6))
		{
			if (!ite->annotation().K_act().isEmpty())
				WritePDFStream(ite->annotation().K_act());
			if (!ite->annotation().F_act().isEmpty())
				WritePDFStream(ite->annotation().F_act());
			if (!ite->annotation().V_act().isEmpty())
				WritePDFStream(ite->annotation().V_act());
			if (!ite->annotation().C_act().isEmpty())
				WritePDFStream(ite->annotation().C_act());
		}
	}
}

void PDFlib::WritePDFStream(const QString& cc)
{
	QString tmp(cc);
	if ((Options.Compress) && (CompAvail))
		tmp = CompressStr(&tmp);
	StartObj(ObjCounter);
	ObjCounter++;
	PutDoc("<< /Length "+QString::number(tmp.length()));  // moeglicherweise +1
	if ((Options.Compress) && (CompAvail))
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n"+EncStream(tmp, ObjCounter-1)+"\nendstream\nendobj\n");
}

void PDFlib::PDF_xForm(double w, double h, QString im)
{
	StartObj(ObjCounter);
	ObjCounter++;
	PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
	PutDoc("/BBox [ 0 0 "+FToStr(w)+" "+FToStr(h)+" ]\n");
	PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
	if (Seite.ImgObjects.count() != 0)
	{
		PutDoc("/XObject <<\n");
		QMap<QString,int>::Iterator it;
		for (it = Seite.ImgObjects.begin(); it != Seite.ImgObjects.end(); ++it)
			PutDoc("/"+it.key()+" "+QString::number(it.data())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Seite.FObjects.count() != 0)
	{
		PutDoc("/Font << \n");
		QMap<QString,int>::Iterator it2;
		for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
			PutDoc("/"+it2.key()+" "+QString::number(it2.data())+" 0 R\n");
		PutDoc(">>\n");
	}
	PutDoc(">>\n");
	PutDoc("/Length "+QString::number(im.length())+"\n");
	PutDoc(">>\nstream\n"+EncStream(im, ObjCounter-1)+"\nendstream\nendobj\n");
	Seite.XObjects[ResNam+QString::number(ResCount)] = ObjCounter-1;
}

void PDFlib::PDF_Form(const QString& im)
{
	StartObj(ObjCounter);
	ObjCounter++;
	PutDoc("<<\n");
	PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
	if (Seite.FObjects.count() != 0)
	{
		PutDoc("/Font << \n");
		QMap<QString,int>::Iterator it2;
		for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
			PutDoc("/"+it2.key()+" "+QString::number(it2.data())+" 0 R\n");
		PutDoc(">>\n");
	}
	PutDoc(">>\n");
	PutDoc("/Length "+QString::number(im.length())+"\n");
	PutDoc(">>\nstream\n"+EncStream(im, ObjCounter-1)+"\nendstream\nendobj\n");
}

void PDFlib::PDF_Bookmark(PageItem *currItem, double ypos)
{
	Bvie->SetAction(currItem, "/XYZ 0 "+FToStr(ypos)+" 0]");
	BookMinUse = true;
}

QString PDFlib::PDF_Image(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN, const QString& Profil, bool Embedded, int Intent)
{
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	ScImage img;
	QString im, tmp, tmpy, dummy, cmd1, cmd2, BBox;
	QChar tc;
	bool found = false;
	bool alphaM = false;
	bool realCMYK = false;
	int afl;
	double x2, ax, ay, a2, a1;
	double sxn = 0;
	double syn = 0;
	x2 = 0;
	double aufl = Options.Resolution / 72.0;
	int ImRes, ImWid, ImHei, origWidth, origHeight;
	struct ShIm ImInfo;
	if ((!SharedImages.contains(fn)) || (fromAN) || (c->effectsInUse.count() != 0))
	{
/*		if ((ext == "pdf") && (Options.Version  >= 14))
		{
			StartObj(ObjCounter);
			PutDoc("<<\n/Type /EmbeddedFile\n");
			im = "";
			loadText(fn, &im);
			if ((Options.Compress) && (CompAvail))
			{
				PutDoc("/Filter /FlateDecode\n");
				im = CompressStr(&im);
			}
			PutDoc("/Length "+QString::number(im.length())+"\n");
			PutDoc(">>\nstream\n"+EncStream(im, ObjCounter)+"\nendstream\nendobj\n");
			ObjCounter++;
			StartObj(ObjCounter);
			PutDoc("<<\n/Type /Filespec\n/F ("+fn+")\n/EF << /F "+QString::number(ObjCounter-1)+" 0 R >>\n");
			PutDoc(">>\nendobj\n");
			ObjCounter++;
			StartObj(ObjCounter);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
			PutDoc("/BBox [ 0 0 "+FToStr(c->Width)+" "+FToStr(c->Height)+" ]\n");
			PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]>>\n");
			PutDoc("/Ref <<\n/Page 1\n/F "+QString::number(ObjCounter-1)+" 0 R\n>>\n");
			PutDoc("/Length 0\n");
			PutDoc(">>\nstream\nendstream\nendobj\n");
			ObjCounter++;
		}
		else
		{ */
		if ((ext == "eps") || (ext == "pdf"))
		{
			QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.png");
			if (Options.RecalcPic)
			{
				afl = QMIN(Options.PicRes, Options.Resolution);
				aufl = afl / 72.0;
			}
			else
				afl = Options.Resolution;
			if (ext == "pdf")
			{
				if (Options.UseRGB)
					img.LoadPicture(fn, Profil, Embedded, Intent, true, 2, afl);
				else
				{
#ifdef HAVE_CMS
					if ((CMSuse) && (Options.UseProfiles2))
						img.LoadPicture(fn, Profil, Embedded, Intent, true, 1, afl);
					else
					{
#endif
						if (Options.isGrayscale)
							img.LoadPicture(fn, Profil, Embedded, Intent, true, 1, afl);
						else
							img.LoadPicture(fn, Profil, Embedded, Intent, true, 0, afl);
#ifdef HAVE_CMS
					}
#endif
				}
			}
			else
			{
				QFile f(fn);
				if (f.open(IO_ReadOnly))
				{
					QTextStream ts(&f);
					while (!ts.atEnd())
					{
						tc = ' ';
						tmp = "";
						while ((tc != '\n') && (tc != '\r'))
						{
							ts >> tc;
							if ((tc != '\n') && (tc != '\r'))
								tmp += QChar(tc);
						}
						if (tmp.startsWith("%%BoundingBox:"))
						{
							found = true;
							BBox = tmp.remove("%%BoundingBox:");
						}
						if (!found)
						{
							if (tmp.startsWith("%%BoundingBox"))
							{
								found = true;
								BBox = tmp.remove("%%BoundingBox");
							}
						}
						if (tmp.startsWith("%%EndComments"))
							break;
					}
					f.close();
					if (found)
					{
						if (Options.UseRGB)
							img.LoadPicture(fn, Profil, Embedded, Intent, true, 2, afl);
						else
						{
#ifdef HAVE_CMS
							if ((CMSuse) && (Options.UseProfiles2))
								img.LoadPicture(fn, Profil, Embedded, Intent, true, 1, afl);
							else
							{
#endif
								if (Options.isGrayscale)
									img.LoadPicture(fn, Profil, Embedded, Intent, true, 1, afl);
								else
									img.LoadPicture(fn, Profil, Embedded, Intent, true, 0, afl);
#ifdef HAVE_CMS
							}
#endif
						}
					}
				}
			}
			if (Options.RecalcPic)
			{
				sxn = sx * (1.0 / aufl);
				syn = sy * (1.0 / aufl);
			}
		}
		else
		{
			img.imgInfo.valid = false;
			img.imgInfo.clipPath = "";
			img.imgInfo.PDSpathData.clear();
			img.imgInfo.layerInfo.clear();
			img.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
			img.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
			if (Options.UseRGB)
				img.LoadPicture(fn, Profil, Embedded, Intent, true, 2, 72, &realCMYK);
			else
			{
#ifdef HAVE_CMS
				if ((CMSuse) && (Options.UseProfiles2))
					img.LoadPicture(fn, Profil, Embedded, Intent, true, 3, 72, &realCMYK);
				else
				{
#endif
					if (Options.isGrayscale)
						img.LoadPicture(fn, Profil, Embedded, Intent, true, 1, 72, &realCMYK);
					else
						img.LoadPicture(fn, Profil, Embedded, Intent, true, 0, 72, &realCMYK);
#ifdef HAVE_CMS
				}
#endif
			}
			if (Options.RecalcPic)
			{
				double afl = QMIN(Options.PicRes, Options.Resolution);
				a2 = (72.0 / sx) / afl;
				a1 = (72.0 / sy) / afl;
				origWidth = img.width();
				origHeight = img.height();
				ax = img.width() / a2;
				ay = img.height() / a1;
				if ((Options.UseRGB) || (Options.isGrayscale) || ((Options.UseProfiles2) && !(img.imgInfo.colorspace == 1)) )
				{
					int colsp = img.imgInfo.colorspace;
					bool prog = img.imgInfo.progressive;
					img = img.smoothScale(qRound(ax), qRound(ay));
					img.imgInfo.colorspace = colsp;
					img.imgInfo.progressive = prog;
				}
				else
					img.scaleImage(qRound(ax), qRound(ay));
				sxn = sx * a2;
				syn = sy * a1;
			}
			aufl = 1;
		}
#ifdef HAVE_CMS
		if ((CMSuse) && (Options.UseProfiles2))
		{
			if (!ICCProfiles.contains(Profil))
			{
				ScImage img3;
				int components = 0;
				StartObj(ObjCounter);
				ObjCounter++;
				QString dataP = "";
				struct ICCD dataD;
				if ((Embedded) && (!Options.EmbeddedI))
				{
					img3.getEmbeddedProfile(fn, &dataP, &components);
					if (dataP.isEmpty())
					{
						if (img.imgInfo.colorspace == 1)
						{
							loadText((Embedded ? ScMW->InputProfilesCMYK[Options.ImageProf] : ScMW->InputProfilesCMYK[Profil]), &dataP);
							components = 4;
						}
						else
						{
							loadText((Embedded ? ScMW->InputProfiles[Options.ImageProf] : ScMW->InputProfiles[Profil]), &dataP);
							components = 3;
						}
					}
				}
				else
				{
					if (img.imgInfo.colorspace == 1)
					{
						loadText((Embedded ? ScMW->InputProfilesCMYK[Options.ImageProf] : ScMW->InputProfilesCMYK[Profil]), &dataP);
						components = 4;
					}
					else
					{
						loadText((Embedded ? ScMW->InputProfiles[Options.ImageProf] : ScMW->InputProfiles[Profil]), &dataP);
						components = 3;
					}
				}
				PutDoc("<<\n");
				if ((Options.CompressMethod != 3) && (CompAvail))
				{
					PutDoc("/Filter /FlateDecode\n");
					dataP = CompressStr(&dataP);
				}
				PutDoc("/Length "+QString::number(dataP.length()+1)+"\n");
				PutDoc("/N "+QString::number(components)+"\n");
				PutDoc(">>\nstream\n"+EncStream(dataP, ObjCounter-1)+"\nendstream\nendobj\n");
				StartObj(ObjCounter);
				dataD.ResName = ResNam+QString::number(ResCount);
				dataD.ICCArray = "[ /ICCBased "+QString::number(ObjCounter-1)+" 0 R ]";
				dataD.ResNum = ObjCounter;
				ICCProfiles[Profil] = dataD;
				PutDoc("[ /ICCBased "+QString::number(ObjCounter-1)+" 0 R ]\n");
				PutDoc("endobj\n");
				ResCount++;
				ObjCounter++;
			}
		}
#endif
		QString im2 = "";
		ScImage img2;
		if (Options.Version >= 14)
			im2 = img2.getAlpha(fn, true, true, afl);
		else
			im2 = img2.getAlpha(fn, true, false, afl);
		if (!im2.isEmpty())
			alphaM = true;
		bool imgE = false;
		if ((Options.UseRGB) || (Options.isGrayscale))
			imgE = false;
		else
		{
			if ((Options.UseProfiles2) && (img.imgInfo.colorspace != 1))
				imgE = false;
			else
				imgE = true;
		}
		img.applyEffect(c->effectsInUse, c->document()->PageColors, imgE);
		if (!Options.RecalcPic)
		{
			sxn = sx * (1.0 / aufl);
			syn = sy * (1.0 / aufl);
			origWidth = img.width();
			origHeight = img.height();
		}
		if (alphaM)
		{
			StartObj(ObjCounter);
			ObjCounter++;
			PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
			if (Options.Version >= 14)
			{
				if ((Options.CompressMethod != 3) && (CompAvail))
					im2 = CompressStr(&im2);
				PutDoc("/Width "+QString::number(origWidth)+"\n");
				PutDoc("/Height "+QString::number(origHeight)+"\n");
				PutDoc("/ColorSpace /DeviceGray\n");
				PutDoc("/BitsPerComponent 8\n");
				PutDoc("/Length "+QString::number(im2.length())+"\n");
			}
			else
			{
				if ((Options.CompressMethod != 3) && (CompAvail))
					im2 = CompressStr(&im2);
				PutDoc("/Width "+QString::number(origWidth)+"\n");
				PutDoc("/Height "+QString::number(origHeight)+"\n");
				PutDoc("/ImageMask true\n/BitsPerComponent 1\n");
				PutDoc("/Length "+QString::number(im2.length())+"\n");
			}
			if ((Options.CompressMethod != 3) && (CompAvail))
				PutDoc("/Filter /FlateDecode\n");
			PutDoc(">>\nstream\n"+EncStream(im2, ObjCounter-1)+"\nendstream\nendobj\n");
			Seite.ImgObjects[ResNam+QString::number(ResCount)] = ObjCounter-1;
			ResCount++;
		}
		if (Options.UseRGB)
			im = img.ImageToTxt();
		else
		{
			if (Options.isGrayscale)
				im = img.ImageToGray();
			else
			{
#ifdef HAVE_CMS
				if ((CMSuse) && (Options.UseProfiles2) && (!realCMYK))
					im = img.ImageToTxt();
				else
#endif
				im = img.ImageToCMYK_PDF(true);
			}
		}
		StartObj(ObjCounter);
		ObjCounter++;
		if (((Options.CompressMethod == 2) || (Options.CompressMethod == 0)) && (CompAvail))
			im = CompressStr(&im);
		PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
		PutDoc("/Width "+QString::number(img.width())+"\n");
		PutDoc("/Height "+QString::number(img.height())+"\n");
#ifdef HAVE_CMS
		if ((CMSuse) && (Options.UseProfiles2))
		{
			PutDoc("/ColorSpace "+ICCProfiles[Profil].ICCArray+"\n");
			PutDoc("/Intent /");
			int inte2 = Intent;
			if (Options.EmbeddedI)
				inte2 = Options.Intent2;
			static const QString cmsmode[] = {"Perceptual", "RelativeColorimetric", "Saturation", "AbsoluteColorimetric"};
			PutDoc(cmsmode[inte2] + "\n");
		}
		else
		{
#endif
			if (Options.UseRGB)
				PutDoc("/ColorSpace /DeviceRGB\n");
			else
			{
				if (Options.isGrayscale)
					PutDoc("/ColorSpace /DeviceGray\n");
				else
					PutDoc("/ColorSpace /DeviceCMYK\n");
			}
#ifdef HAVE_CMS
		}
#endif
		int cm = Options.CompressMethod;
		bool specialCMYK = false;
		if (((ext == "jpg") || (ext == "jpeg")) && (cm != 3))
		{
			if (((Options.UseRGB || Options.UseProfiles2) && (cm == 0) && (c->effectsInUse.count() == 0) && (img.imgInfo.colorspace == 0)) && (!img.imgInfo.progressive) && (!Options.RecalcPic))
			{
				im = "";
				loadText(fn, &im);
				cm = 1;
			}
			else if (((!Options.UseRGB) && (!Options.isGrayscale) && (!Options.UseProfiles2)) && (cm== 0) && (c->effectsInUse.count() == 0) && (img.imgInfo.colorspace == 1) && (!Options.RecalcPic) && (!img.imgInfo.progressive))
			{
				im = "";
				loadText(fn, &im);
				cm = 1;
				specialCMYK = true;
			}
			else
			{
				if (Options.CompressMethod == 1)
				{
					QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.jpg");
					if ((Options.UseRGB) || (Options.UseProfiles2) && (!realCMYK))
						img.Convert2JPG(tmpFile, Options.Quality, false, false);
					else
					{
						if (Options.isGrayscale)
							img.Convert2JPG(tmpFile, Options.Quality, false, true);
						else
						{
							img.Convert2JPG(tmpFile, Options.Quality, true, false);
							specialCMYK = true;
						}
					}
					im = "";
					loadText(tmpFile, &im);
					cm = 1;
					QFile::remove(tmpFile);
				}
				else
					cm = 2;
			}
		}
		else
		{
			if ((Options.CompressMethod == 1) || (Options.CompressMethod == 0))
			{
				QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.jpg");
				if ((Options.UseRGB) || (Options.UseProfiles2) && (!realCMYK))
					img.Convert2JPG(tmpFile, Options.Quality, false, false);
				else
				{
					if (Options.isGrayscale)
						img.Convert2JPG(tmpFile, Options.Quality, false, true);
					else
					{
						img.Convert2JPG(tmpFile, Options.Quality, true, false);
						specialCMYK = true;
					}
				}
				if (Options.CompressMethod == 0)
				{
					QFileInfo fi(tmpFile);
					if (fi.size() < im.length())
					{
						im = "";
						loadText(tmpFile, &im);
						cm = 1;
					}
					else
						cm = 2;
				}
				else
				{
					im = "";
					loadText(tmpFile, &im);
					cm = 1;
				}
				QFile::remove(tmpFile);
			}
		}
		PutDoc("/BitsPerComponent 8\n");
		PutDoc("/Length "+QString::number(im.length())+"\n");
		if (CompAvail)
		{
			if (cm == 1)
				PutDoc("/Filter /DCTDecode\n");
			else if (cm != 3)
				PutDoc("/Filter /FlateDecode\n");
		}
		if ((specialCMYK) && ((cm == 1) && (CompAvail)))
			PutDoc("/Decode [1 0 1 0 1 0 1 0]\n");
		if (alphaM)
		{
			if (Options.Version >= 14)
				PutDoc("/SMask "+QString::number(ObjCounter-2)+" 0 R\n");
			else
				PutDoc("/Mask "+QString::number(ObjCounter-2)+" 0 R\n");
		}
		PutDoc(">>\nstream\n"+EncStream(im, ObjCounter-1)+"\nendstream\nendobj\n");
//		}
		Seite.ImgObjects[ResNam+QString::number(ResCount)] = ObjCounter-1;
		ImRes = ResCount;
		ImWid = img.width();
		ImHei = img.height();
		ImInfo.ResNum = ImRes;
		ImInfo.Width = ImWid;
		ImInfo.Height = ImHei;
		ImInfo.aufl = aufl;
		ImInfo.sxa = sxn;
		ImInfo.sya = syn;
		ImInfo.xa = sx;
		ImInfo.ya = sy;
		if (c->effectsInUse.count() == 0)
			SharedImages.insert(fn, ImInfo);
		ResCount++;
	}
	else
	{
		ImRes = SharedImages[fn].ResNum;
		ImWid = SharedImages[fn].Width;
		ImHei = SharedImages[fn].Height;
		aufl = SharedImages[fn].aufl;
		if (!Options.RecalcPic)
		{
			sxn = sx * (1.0 / aufl);
			syn = sy * (1.0 / aufl);
		}
		else
		{
			sxn = SharedImages[fn].sxa * sx / SharedImages[fn].xa;
			syn = SharedImages[fn].sya * sy / SharedImages[fn].ya;
		}
	}
	if ((ext == "eps") || (ext == "pdf"))// compensate gsResolution setting
	{
		sxn = PrefsManager::instance()->appPrefs.gs_Resolution / 72.0 * sxn;
		syn = PrefsManager::instance()->appPrefs.gs_Resolution / 72.0 * syn;
	}
	if (!fromAN)
	{
		QString tmp(FToStr(ImWid*sxn)+" 0 0 "+FToStr(ImHei*syn)+" "+FToStr(x*sx)+" "+FToStr((-ImHei*syn+y*sy))+" cm\n/"+ResNam+QString::number(ImRes)+" Do\n");
		return tmp;
	}
	else
		return "";
}

void PDFlib::PDF_End_Doc(const QString& PrintPr, const QString& Name, int Components)
{
	QString tmp;
	uint StX;
	int Basis;
	int ResO;
	BookMItem* ip;
	QListViewItem* pp;
	QString Inhal = "";
	QMap<int,QString> Inha;
	Inha.clear();
	int Bmc = 0;
	if ((Bvie->childCount() != 0) && (Options.Bookmarks) && (BookMinUse))
	{
		Basis = ObjCounter - 1;
		Outlines.Count = Bvie->childCount();
		ip = (BookMItem*)Bvie->firstChild();
		pp = Bvie->firstChild();
		Outlines.First = ip->ItemNr+Basis;
		while (pp)
		{
			if (!pp->nextSibling())
			{
				ip = (BookMItem*)pp;
				Outlines.Last = ip->ItemNr+Basis;
				break;
			}
			pp = pp->nextSibling();
		}
		QListViewItemIterator it(Bvie);
		for ( ; it.current(); ++it)
		{
			ip = (BookMItem*)it.current();
			Inhal = "";
			Bmc++;
			Inhal += QString::number(ip->ItemNr+Basis)+ " 0 obj\n";
			QString encText = QString(QChar(254))+QString(QChar(255));
			for (uint telen = 0; telen < ip->Titel.length(); telen++)
			{
				QChar ch = ip->Titel.at(telen);
				encText += QChar(ch.row());
				encText += QChar(ch.cell());
			}
			Inhal += "<<\n/Title "+EncString("("+encText+")", ip->ItemNr+Basis)+"\n";
			if (ip->Pare == 0)
				Inhal += "/Parent 3 0 R\n";
			else
				Inhal += "/Parent "+QString::number(ip->Pare+Basis)+" 0 R\n";
			if (ip->Prev != 0)
				Inhal += "/Prev "+QString::number(ip->Prev+Basis)+" 0 R\n";
			if (ip->Next != 0)
				Inhal += "/Next "+QString::number(ip->Next+Basis)+" 0 R\n";
			if (ip->First != 0)
				Inhal += "/First "+QString::number(ip->First+Basis)+" 0 R\n";
			if (ip->Last != 0)
				Inhal += "/Last "+QString::number(ip->Last+Basis)+" 0 R\n";
			if (ip->firstChild())
				Inhal += "/Count -"+QString::number(ip->childCount())+"\n";
			if ((ip->PageObject->OwnPage != -1) && (ip->PageObject->OwnPage < static_cast<int>(PageTree.Kids.count())))
				Inhal += "/Dest ["+QString::number(PageTree.Kids[ip->PageObject->OwnPage])+" 0 R "+ip->Action+"\n";
			Inhal += ">>\nendobj\n";
			Inha[ip->ItemNr] = Inhal;
		}
		for (int b = 1; b < Bmc+1; ++b)
		{
			XRef.append(bytesWritten());
			PutDoc(Inha[b]);
			ObjCounter++;
		}
	}
	StartObj(ObjCounter);
	ResO = ObjCounter;
	PutDoc("<< /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
	if ((Seite.ImgObjects.count() != 0) || (Seite.XObjects.count() != 0))
	{
		PutDoc("/XObject <<\n");
		QMap<QString,int>::Iterator it;
		for (it = Seite.ImgObjects.begin(); it != Seite.ImgObjects.end(); ++it)
			PutDoc("/"+it.key()+" "+QString::number(it.data())+" 0 R\n");
		QMap<QString,int>::Iterator iti;
		for (iti = Seite.XObjects.begin(); iti != Seite.XObjects.end(); ++iti)
			PutDoc("/"+iti.key()+" "+QString::number(iti.data())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Seite.FObjects.count() != 0)
	{
		PutDoc("/Font << \n");
		QMap<QString,int>::Iterator it2;
		for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
			PutDoc("/"+it2.key()+" "+QString::number(it2.data())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Shadings.count() != 0)
	{
		PutDoc("/Shading << \n");
		QMap<QString,int>::Iterator it3;
		for (it3 = Shadings.begin(); it3 != Shadings.end(); ++it3)
			PutDoc("/"+it3.key()+" "+QString::number(it3.data())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Transpar.count() != 0)
	{
		PutDoc("/ExtGState << \n");
		QMap<QString,int>::Iterator it3t;
		for (it3t = Transpar.begin(); it3t != Transpar.end(); ++it3t)
			PutDoc("/"+it3t.key()+" "+QString::number(it3t.data())+" 0 R\n");
		PutDoc(">>\n");
	}
	if ((ICCProfiles.count() != 0) || (spotMap.count() != 0))
	{
		PutDoc("/ColorSpace << \n");
		QMap<QString,ICCD>::Iterator it3c;
		if (ICCProfiles.count() != 0)
		{
			for (it3c = ICCProfiles.begin(); it3c != ICCProfiles.end(); ++it3c)
				PutDoc("/"+it3c.data().ResName+" "+QString::number(it3c.data().ResNum)+" 0 R\n");
		}
		QMap<QString,SpotC>::Iterator it3sc;
		if (spotMap.count() != 0)
		{
			for (it3sc = spotMap.begin(); it3sc != spotMap.end(); ++it3sc)
				PutDoc("/"+it3sc.data().ResName+" "+QString::number(it3sc.data().ResNum)+" 0 R\n");
		}
		PutDoc(">>\n");
	}
	if ((Options.Version == 15) && (Options.useLayers))
	{
		PutDoc("/Properties <<\n");
		struct Layer ll;
		ll.isPrintable = false;
		ll.LNr = 0;
		int Lnr = 0;
		for (uint la = 0; la < doc.Layers.count(); ++la)
		{
			Level2Layer(&doc, &ll, la);
			PutDoc("/"+OCGEntries[ll.Name].Name+" "+QString::number(OCGEntries[ll.Name].ObjNum)+" 0 R\n");
			Lnr++;
		}
		PutDoc(">>\n");
	}
	PutDoc(">>\nendobj\n");
	ObjCounter++;
	XRef[2] = bytesWritten();
	PutDoc("3 0 obj\n<<\n/Type /Outlines\n");
	PutDoc("/Count "+QString::number(Outlines.Count)+"\n");
	if ((Bvie->childCount() != 0) && (Options.Bookmarks))
	{
		PutDoc("/First "+QString::number(Outlines.First)+" 0 R\n");
		PutDoc("/Last "+QString::number(Outlines.Last)+" 0 R\n");
	}
	PutDoc(">>\nendobj\n");
	XRef[3] = bytesWritten();
	PutDoc("4 0 obj\n<<\n/Type /Pages\n/Kids [");
	for (uint b = 0; b < PageTree.Kids.count(); ++b)
		PutDoc(QString::number(PageTree.Kids[b])+" 0 R ");
	PutDoc("]\n");
	PutDoc("/Count "+QString::number(PageTree.Count)+"\n");
	PutDoc("/Resources "+QString::number(ObjCounter-1)+" 0 R\n");
	PutDoc(">>\nendobj\n");
	XRef[4] = bytesWritten();
	PutDoc("5 0 obj\n<<\n");
	if (NamedDest.count() != 0)
	{
		QValueList<Dest>::Iterator vt;
		for (vt = NamedDest.begin(); vt != NamedDest.end(); ++vt)
		{
			if ((*vt).Seite < static_cast<int>(PageTree.Kids.count()))
				PutDoc("/"+(*vt).Name+" ["+QString::number(PageTree.Kids[(*vt).Seite])+" 0 R /XYZ "+(*vt).Act+"]\n");
		}
	}
	PutDoc(">>\nendobj\n");
	XRef[5] = bytesWritten();
	PutDoc("6 0 obj\n<<\n");
	if (Seite.FormObjects.count() != 0)
	{
		PutDoc("/Fields [ ");
		for (uint fo = 0; fo < Seite.FormObjects.count(); ++fo)
			PutDoc(QString::number(Seite.FormObjects[fo])+" 0 R ");
		PutDoc(" ]\n");
	}
	if (CalcFields.count() != 0)
	{
		PutDoc("/CO [ ");
		for (uint foc = 0; foc < CalcFields.count(); ++foc)
			PutDoc(QString::number(CalcFields[foc])+" 0 R ");
		PutDoc(" ]\n");
	}
	if ((Seite.FormObjects.count() != 0) || (CalcFields.count() != 0))
		PutDoc("/NeedAppearances true\n/DR "+QString::number(ResO)+" 0 R\n");
	PutDoc(">>\nendobj\n");
	if (doc.JavaScripts.count() != 0)
	{
		int Fjav0 = ObjCounter;
		QMap<QString,QString>::Iterator itja0;
		for (itja0 = doc.JavaScripts.begin(); itja0 != doc.JavaScripts.end(); ++itja0)
			WritePDFStream(itja0.data());
		int Fjav = ObjCounter;
		QMap<QString,QString>::Iterator itja;
		for (itja = doc.JavaScripts.begin(); itja != doc.JavaScripts.end(); ++itja)
		{
			StartObj(ObjCounter);
			ObjCounter++;
			PutDoc("<< /S /JavaScript /JS "+QString::number(Fjav0)+" 0 R >>\n");
			PutDoc("endobj\n");
			Fjav0++;
		}
		StartObj(ObjCounter);
		ObjCounter++;
		PutDoc("<< /Names [ ");
		QMap<QString,QString>::Iterator itja2;
		for (itja2 = doc.JavaScripts.begin(); itja2 != doc.JavaScripts.end(); ++itja2)
		{
			PutDoc(EncString("("+itja2.key()+")", 6)+" "+QString::number(Fjav)+" 0 R ");
			Fjav++;
		}
		PutDoc("] >>\nendobj\n");
	}
	XRef[6] = bytesWritten();
	PutDoc("7 0 obj\n<< ");
	if (doc.JavaScripts.count() != 0)
		PutDoc("/JavaScript "+QString::number(ObjCounter-1)+" 0 R");
	PutDoc(" >>\nendobj\n");
	Threads.clear();
	if (Options.Articles)
	{
		for (uint ele = 0; ele < doc.Items->count(); ++ele)
		{
			PageItem* tel = doc.Items->at(ele);
			if ((tel->asTextFrame()) && (tel->BackBox == 0) && (tel->NextBox != 0) &&
					(!tel->Redrawn))
			{
				StartObj(ObjCounter);
				Threads.append(ObjCounter);
				ObjCounter++;
				PutDoc("<< /Type /Thread\n");
				PutDoc("   /F "+QString::number(ObjCounter)+" 0 R\n");
				PutDoc(">>\nendobj\n");
				Beads.clear();
				struct Bead bd;
				int fir = ObjCounter;
				int ccb = ObjCounter;
				bd.Parent = ObjCounter-1;
				while (tel->NextBox != 0)
				{
					if (tel->OwnPage != -1)
					{
						bd.Next = ccb + 1;
						bd.Prev = ccb - 1;
						ccb++;
						bd.Page = PageTree.Kids[tel->OwnPage];
						bd.Recht = QRect(static_cast<int>(tel->xPos() - doc.Pages->at(tel->OwnPage)->xOffset()),
									static_cast<int>(doc.Pages->at(tel->OwnPage)->height() - (tel->yPos()  - doc.Pages->at(tel->OwnPage)->yOffset())),
									static_cast<int>(tel->width()),
									static_cast<int>(tel->height()));
						Beads.append(bd);
					}
					tel->Redrawn = true;
					tel = tel->NextBox;
				}
				bd.Next = ccb + 1;
				bd.Prev = ccb - 1;
				if (tel->OwnPage != -1)
				{
					bd.Page = PageTree.Kids[tel->OwnPage];
					bd.Recht = QRect(static_cast<int>(tel->xPos() - doc.Pages->at(tel->OwnPage)->xOffset()),
								static_cast<int>(doc.Pages->at(tel->OwnPage)->height() - (tel->yPos()  - doc.Pages->at(tel->OwnPage)->yOffset())),
								static_cast<int>(tel->width()),
								static_cast<int>(tel->height()));
					Beads.append(bd);
				}
				tel->Redrawn = true;
				Beads[0].Prev = fir + Beads.count()-1;
				Beads[Beads.count()-1].Next = fir;
				for (uint beac = 0; beac < Beads.count(); ++beac)
				{
					StartObj(ObjCounter);
					ObjCounter++;
					PutDoc("<< /Type /Bead\n");
					PutDoc("   /T "+QString::number(Beads[beac].Parent)+" 0 R\n");
					PutDoc("   /N "+QString::number(Beads[beac].Next)+" 0 R\n");
					PutDoc("   /V "+QString::number(Beads[beac].Prev)+" 0 R\n");
					PutDoc("   /P "+QString::number(Beads[beac].Page)+" 0 R\n");
					PutDoc("   /R [ "+QString::number(Beads[beac].Recht.x())+" "+
							QString::number(Beads[beac].Recht.y())+" ");
					PutDoc(QString::number(Beads[beac].Recht.bottomRight().x())+" "+QString::number(Beads[beac].Recht.y()-Beads[beac].Recht.height())+" ]\n");
					PutDoc(">>\nendobj\n");
				}
			}
		}
		for (uint ele = 0; ele < doc.Items->count(); ++ele)
			doc.Items->at(ele)->Redrawn = false;
	}
	XRef[7] = bytesWritten();
	PutDoc("8 0 obj\n[");
	for (uint th = 0; th < Threads.count(); ++th)
		PutDoc(QString::number(Threads[th])+" 0 R ");
	PutDoc("]\nendobj\n");
	if ((Options.Version == 15) && (Options.useLayers))
	{
		XRef[8] = bytesWritten();
		QStringList lay;
		PutDoc("9 0 obj\n<<\n");
		PutDoc("/D << /BaseState /ON /Order [ ");
		QMap<QString, OCGInfo>::Iterator itoc;
		for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
		{
			lay.prepend(QString::number(itoc.data().ObjNum)+" 0 R ");
		}
		for (uint layc = 0; layc < lay.count(); ++layc)
		{
			PutDoc(lay[layc]);
		}
		PutDoc("]\n/OFF [ ");
		for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
		{
			if (!itoc.data().visible)
				PutDoc(QString::number(itoc.data().ObjNum)+" 0 R ");
		}
		PutDoc("] >>\n/OCGs [ ");
		for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
		{
			PutDoc(QString::number(itoc.data().ObjNum)+" 0 R ");
		}
		PutDoc("]\n");
		PutDoc(">>\nendobj\n");
	}
	if (Options.Version == 12)
	{
		StartObj(ObjCounter);
		ObjCounter++;
		QString dataP;
		loadText(PrintPr, &dataP);
		PutDoc("<<\n");
		if ((Options.Compress) && (CompAvail))
		{
			PutDoc("/Filter /FlateDecode\n");
			dataP = CompressStr(&dataP);
		}
		PutDoc("/Length "+QString::number(dataP.length()+1)+"\n");
		PutDoc("/N "+QString::number(Components)+"\n");
		PutDoc(">>\nstream\n"+dataP+"\nendstream\nendobj\n");
		XRef[8] = bytesWritten();
		PutDoc("9 0 obj\n");
		PutDoc("<<\n/Type /OutputIntent\n/S /GTS_PDFX\n");
		PutDoc("/DestOutputProfile "+QString::number(ObjCounter-1)+" 0 R\n");
		PutDoc("/OutputConditionIdentifier (Custom)\n");
		PutDoc("/Info ("+PDFEncode(Options.Info)+")\n");
		PutDoc("/OutputCondition ("+PDFEncode(Name)+")\n");
		PutDoc(">>\nendobj\n");
	}
	StX = bytesWritten();
	PutDoc("xref\n");
	PutDoc("0 "+QString::number(ObjCounter)+"\n");
	PutDoc("0000000000 65535 f \n");
	for (uint a = 0; a < XRef.count(); ++a)
	{
		tmp.sprintf("%10d", XRef[a]);
		tmp.replace(QRegExp(" "), "0");
		PutDoc(tmp+" 00000 n \n");
	}
	PutDoc("trailer\n<<\n/Size "+QString::number(XRef.count()+1)+"\n");
	QString IDs ="";
	for (uint cl = 0; cl < 16; ++cl)
		IDs += QChar(FileID[cl]);
	IDs = String2Hex(&IDs);
	PutDoc("/Root 1 0 R\n/Info 2 0 R\n/ID [<"+IDs+"><"+IDs+">]\n");
	if (Options.Encrypt)
		PutDoc("/Encrypt "+QString::number(Encrypt)+" 0 R\n");
	PutDoc(">>\nstartxref\n");
	PutDoc(QString::number(StX)+"\n%%EOF\n");
	closeAndCleanup();
}

void PDFlib::closeAndCleanup()
{
	Spool.close();
	if (abortExport)
	{
		if (Spool.exists())
			Spool.remove();
	}
	Seite.XObjects.clear();
	Seite.ImgObjects.clear();
	Seite.FObjects.clear();
	Seite.AObjects.clear();
	Seite.FormObjects.clear();
	CalcFields.clear();
	Shadings.clear();
	Transpar.clear();
	ICCProfiles.clear();

}

void PDFlib::cancelRequested()
{
	abortExport=true;
}
