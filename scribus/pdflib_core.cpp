/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  pdflib_core.cpp  -  description
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

#include "pdflib_core.h"

#include "scconfig.h"

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <cstdlib>
#include <string>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "rc4.h"

#include <QByteArray>
#include <QDateTime>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QList>
#include <QPainterPath>
#include <QPixmap>
#include <QRect>
#include <QRegExp>
#include <QStack>
#include <QString>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QtXml>
#include <QUuid>


#include "ui/bookmwin.h"
#include "ui/bookmarkpalette.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "ui/multiprogressdialog.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "pageitem_group.h"
#include "pdfoptions.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scfonts.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scstreamfilter_flate.h"
#include "scstreamfilter_rc4.h"
#include "text/nlsconfig.h"
#include "util.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_math.h"
#include "util_ghostscript.h"
#ifdef HAVE_OSG
	#include "third_party/prc/exportPRC.h"
#endif

using namespace std;

#if defined(_WIN32)
#undef GetObject
#endif


PDFLibCore::PDFLibCore(ScribusDoc & docu)
	: QObject(&docu),
	doc(docu),
	ActPageP(0),
	Options(doc.pdfOptions()),
	Bvie(0),
	ucs2Codec(0),
	ObjCounter(7),
	ResNam("RE"),
	ResCount(0),
	NDnam("LI"),
	NDnum(0),
	KeyGen(""),
	OwnerKey(""),
	UserKey(""),
	FileID(""),
	EncryKey(""),
	Encrypt(0),
	KeyLen(5),
	colorsToUse(),
	spotNam("Spot"),
	spotCount(0),
	progressDialog(0),
	abortExport(false),
	usingGUI(ScCore->usingGUI())
{
	KeyGen.resize(32);
	OwnerKey.resize(32);
	UserKey.resize(32);
	FileID.resize(16);
	EncryKey.resize(5);
	Catalog.Outlines = 2;
	Catalog.PageTree = 3;
	Catalog.Dest = 4;
	PageTree.Count = 0;
	Outlines.First = 0;
	Outlines.Last = 0;
	Outlines.Count = 0;
	Seite.ObjNum = 0;
	Seite.Thumb = 0;
	int kg_array[] = {0x28, 0xbf, 0x4e, 0x5e, 0x4e, 0x75, 0x8a, 0x41, 0x64, 0x00, 0x4e, 0x56, 0xff, 0xfa,
			  0x01, 0x08, 0x2e, 0x2e, 0x00, 0xb6, 0xd0, 0x68, 0x3e, 0x80, 0x2f, 0x0c, 0xa9, 0xfe,
			  0x64, 0x53, 0x69, 0x7a};
	for (int a = 0; a < 32; ++a)
		KeyGen[a] = kg_array[a];
	if (usingGUI)
	{
		progressDialog = new MultiProgressDialog( tr("Saving PDF"), CommonStrings::tr_Cancel, doc.scMW());
		Q_CHECK_PTR(progressDialog);
		QStringList barNames, barTexts;
		barNames << "EMP" << "EP" << "ECPI";
		barTexts << tr("Exporting Master Page:") << tr("Exporting Page:") << tr("Exporting Items on Current Page:");
		QList<bool> barsNumeric;
		barsNumeric << true << true << false;
		progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
	}
}

PDFLibCore::~PDFLibCore()
{
	delete progressDialog;
}

static inline QString FToStr(double c)
{
	double v = c;
	if (fabs(c) < 0.0000001)
		v = 0.0;
	return QString::number(v, 'f', 5);
};

bool PDFLibCore::doExport(const QString& fn, const QString& nam, int Components,
					  const std::vector<int> & pageNs, const QMap<int,QPixmap> & thumbs)
{
	QPixmap pm;
	bool ret = false, error = false;
	int  pc_exportpages=0;
	int  pc_exportmasterpages=0;
	if (usingGUI)
		progressDialog->show();
	QMap<QString, QMap<uint, FPointArray> > usedFonts;
	usedFonts.clear();
	doc.getUsedFonts(usedFonts);
	ucs2Codec = QTextCodec::codecForName("ISO-10646-UCS-2");
	if(!ucs2Codec)
		ucs2Codec = QTextCodec::codecForName("UTF-16");
	if (!ucs2Codec)
	{
		PDF_Error( tr("Qt build miss both \"UTF-16\" and \"ISO-10646-UCS-2\" text codecs, pdf export is not possible") );
		return false;
	}
	if (PDF_Begin_Doc(fn, PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts, usedFonts, doc.scMW()->bookmarkPalette->BView))
	{
		QMap<int, int> pageNsMpa;
		for (uint a = 0; a < pageNs.size(); ++a)
		{
			pageNsMpa.insert(doc.MasterNames[doc.DocPages.at(pageNs[a]-1)->MPageNam], 0);
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
		for (int ap = 0; ap < doc.MasterPages.count() && !abortExport; ++ap)
		{
			if (doc.MasterItems.count() != 0)
			{
				if (pageNsMpa.contains(ap))
				{
					qApp->processEvents();
					if (!PDF_TemplatePage(doc.MasterPages.at(ap)))
						error = abortExport = true;
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
			if (doc.pdfOptions().Thumbnails)
				pm = thumbs[pageNs[a]];
			qApp->processEvents();
			if (abortExport) break;

			PDF_Begin_Page(doc.DocPages.at(pageNs[a]-1), pm);
			qApp->processEvents();
			if (abortExport) break;

			if (!PDF_ProcessPage(doc.DocPages.at(pageNs[a]-1), pageNs[a]-1, doc.pdfOptions().doClip))
				error = abortExport = true;
			qApp->processEvents();
			if (abortExport) break;

			PDF_End_Page(a);
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
			if ((doc.pdfOptions().Version == PDFOptions::PDFVersion_X3) || (doc.pdfOptions().Version == PDFOptions::PDFVersion_X1a) || (doc.pdfOptions().Version == PDFOptions::PDFVersion_X4))
				ret = PDF_End_Doc(ScCore->PrinterProfiles[doc.pdfOptions().PrintProf], nam, Components);
			else
				ret = PDF_End_Doc();
		}
		else
			closeAndCleanup();
	}
	if (usingGUI)
		progressDialog->close();
	return (ret && !error);
}

const QString& PDFLibCore::errorMessage(void) const
{
	return ErrorMessage;
}

bool PDFLibCore::exportAborted(void) const
{
	return abortExport;
}

void PDFLibCore::StartObj(int nr)
{
	for (int i=XRef.size(); i < nr; ++i)
		XRef.append(0);
	XRef[nr-1] = bytesWritten();
	PutDoc(QString::number(nr)+ " 0 obj\n");
}

// Encode a string for inclusion in a
// PDF (literal) .
static QString PDFEncode(const QString & in)
{
	QString tmp("");
	for (int d = 0; d < in.length(); ++d)
	{
		QChar cc(in.at(d));
		if ((cc == '(') || (cc == ')') || (cc == '\\'))
			tmp += '\\';
		tmp += cc;
	}
	return tmp;
}

static QString blendMode(int code)
{
	switch (code)
	{
		case 0:
			return("Normal");
			break;
		case 1:
			return("Darken");
			break;
		case 2:
			return("Lighten");
			break;
		case 3:
			return("Multiply");
			break;
		case 4:
			return("Screen");
			break;
		case 5:
			return("Overlay");
			break;
		case 6:
			return("HardLight");
			break;
		case 7:
			return("SoftLight");
			break;
		case 8:
			return("Difference");
			break;
		case 9:
			return("Exclusion");
			break;
		case 10:
			return("ColorDodge");
			break;
		case 11:
			return("ColorBurn");
			break;
		case 12:
			return("Hue");
			break;
		case 13:
			return("Saturation");
			break;
		case 14:
			return("Color");
			break;
		case 15:
			return("Luminosity");
			break;
		default:
			return "";
	}
}

QByteArray PDFLibCore::EncodeUTF16(const QString &in)
{
//	QString tmp("");
//	for (int d = 0; d < in.length(); ++d)
//	{
//		QChar cc(in.at(d));
//		if ((cc == '(') || (cc == ')') || (cc == '\\'))
//			tmp += '\\';
//		tmp += cc;
//	}
	QString tmp = in;
	QByteArray cres = ucs2Codec->fromUnicode( tmp );
#ifndef WORDS_BIGENDIAN
	// on little endian systems we ned to swap bytes:
	uchar sw;
	for(int d = 0; d < cres.size()-1; d += 2)
	{
		sw = cres[d];
		cres[d] = cres[d+1];
		cres[d+1] = sw;
	}
#endif
	return cres;
}

QString PDFLibCore::EncStream(const QString & in, int ObjNum)
{
	if (in.length() < 1)
		return QString("");
	else if (!Options.Encrypt)
		return in;
	rc4_context_t rc4;
	QString tmp(in);
	QByteArray us(tmp.length(), ' ');
	QByteArray ou(tmp.length(), ' ');
	for (int a = 0; a < tmp.length(); ++a)
		us[a] = QChar(tmp.at(a)).cell();
	QByteArray step1 = ComputeRC4Key(ObjNum);
	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), qMin(KeyLen+5, 16));
	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), tmp.length());
	QString uk = "";
	for (int cl = 0; cl < tmp.length(); ++cl)
		uk += QChar(ou[cl]);
	return uk;
}

QString PDFLibCore::EncString(const QString & in, int ObjNum)
{
	if (!Options.Encrypt)
		return in;
	rc4_context_t rc4;
	QString tmp;
	if (in.length() < 3)
		return "<>";
	tmp = in.mid(1, in.length()-2);
	QByteArray us(tmp.length(), ' ');
	QByteArray ou(tmp.length(), ' ');
	for (int a = 0; a < tmp.length(); ++a)
		us[a] = static_cast<uchar>(QChar(tmp.at(a)).cell());
	QByteArray step1 = ComputeRC4Key(ObjNum);
	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), qMin(KeyLen+5, 16));
	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), tmp.length());
	QString uk = "";
	for (int cl = 0; cl < tmp.length(); ++cl)
		uk += QChar(ou[cl]);
	tmp = "<"+String2Hex(&uk, false)+">";
	return tmp;
}

QString PDFLibCore::EncStringUTF16(const QString & in, int ObjNum)
{
	if (in.length() < 3)
		return "<>";
	if (!Options.Encrypt)
	{
		QString tmp = in.mid(1, in.length()-2);
		QByteArray us = EncodeUTF16(tmp);
		QString uk = "";
		for (int cl = 0; cl < us.size(); ++cl)
			uk += QChar(us[cl]);
		return "<"+String2Hex(&uk, false)+">";
	}
	rc4_context_t rc4;
	QString tmp = in.mid(1, in.length()-2);
	QByteArray us = EncodeUTF16(tmp);
	QByteArray ou(us.size(), ' ');
	QByteArray step1 = ComputeRC4Key(ObjNum);
	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), qMin(KeyLen+5, 16));
	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), ou.size());
	QString uk = "";
	for (int cl = 0; cl < ou.size(); ++cl)
		uk += QChar(ou[cl]);
	tmp = "<"+String2Hex(&uk, false)+">";
	return tmp;
}

bool PDFLibCore::EncodeArrayToStream(const QByteArray& in, int ObjNum)
{
	if (in.size() < 1)
		return true;
	bool succeed = false;
	if (Options.Encrypt)
	{
		QByteArray step1 = ComputeRC4Key(ObjNum);
		ScRC4EncodeFilter rc4Encode(&outStream, step1.data(), qMin(KeyLen+5, 16));
		if (rc4Encode.openFilter())
		{
			succeed  = rc4Encode.writeData(in.data(), in.size());
			succeed &= rc4Encode.closeFilter();
		}
	}
	else
		outStream.writeRawData(in, in.size());
	return (outStream.status() == QDataStream::Ok);
}

int PDFLibCore::WriteImageToStream(ScImage& image, int ObjNum, bool cmyk, bool gray, bool precal)
{
	bool succeed = false;
	int  bytesWritten = 0;
	if (Options.Encrypt)
	{
		QByteArray step1 = ComputeRC4Key(ObjNum);
		ScRC4EncodeFilter rc4Encode(&outStream, step1.data(), qMin(KeyLen+5, 16));
		if (rc4Encode.openFilter())
		{
			if (gray)
				succeed = image.writeGrayDataToFilter(&rc4Encode, precal);
			else if (cmyk)
				succeed = image.writeCMYKDataToFilter(&rc4Encode);
			else
				succeed = image.writeRGBDataToFilter(&rc4Encode);
			succeed &= rc4Encode.closeFilter();
			bytesWritten = rc4Encode.writtenToStream();
		}
	}
	else
	{
		ScNullEncodeFilter nullEncode(&outStream);
		if (nullEncode.openFilter())
		{
			if (gray)
				succeed = image.writeGrayDataToFilter(&nullEncode, precal);
			else if (cmyk)
				succeed = image.writeCMYKDataToFilter(&nullEncode);
			else
				succeed = image.writeRGBDataToFilter(&nullEncode);
			succeed &= nullEncode.closeFilter();
			bytesWritten = nullEncode.writtenToStream();
		}
	}
	return (succeed ? bytesWritten : 0);
}

int PDFLibCore::WriteJPEGImageToStream(ScImage& image, const QString& fn, int ObjNum, int quality, bool cmyk,
										bool gray, bool sameFile, bool precal)
{
	bool succeed = true;
	int  bytesWritten = 0;
	QFileInfo fInfo(fn);
	QString   ext = fInfo.suffix().toLower();
	QString   jpgFileName, tmpFile;
	if (extensionIndicatesJPEG(ext) && sameFile)
		jpgFileName = fn;
	else
	{
		tmpFile  = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.jpg");
		if ((gray) && (!precal))
			image.convertToGray();
		if (image.convert2JPG(tmpFile, quality, cmyk, gray))
			jpgFileName = tmpFile;
	}
	if (jpgFileName.isEmpty())
		return 0;
	if (Options.Encrypt)
	{
		succeed = false;
		QByteArray step1 = ComputeRC4Key(ObjNum);
		ScRC4EncodeFilter rc4Encode(&outStream, step1.data(), qMin(KeyLen+5, 16));
		if (rc4Encode.openFilter())
		{
			succeed  = copyFileToFilter(jpgFileName, rc4Encode);
			succeed &= rc4Encode.closeFilter();
			bytesWritten = rc4Encode.writtenToStream();
		}
	}
	else
	{
		succeed &= copyFileToStream(jpgFileName, outStream);
		QFileInfo jpgInfo(jpgFileName);
		bytesWritten = jpgInfo.size();
	}
	if (!tmpFile.isEmpty() && QFile::exists(tmpFile))
		QFile::remove(tmpFile);
	return (succeed ? bytesWritten : 0);
}

int PDFLibCore::WriteFlateImageToStream(ScImage& image, int ObjNum, bool cmyk, bool gray, bool precal)
{
	bool succeed = false;
	int  bytesWritten = 0;
	if (Options.Encrypt)
	{
		QByteArray step1 = ComputeRC4Key(ObjNum);
		ScRC4EncodeFilter   rc4Encode(&outStream, step1.data(), qMin(KeyLen+5, 16));
		ScFlateEncodeFilter flateEncode(&rc4Encode);
		if (flateEncode.openFilter())
		{
			if (gray)
				succeed = image.writeGrayDataToFilter(&flateEncode, precal);
			else if (cmyk)
				succeed = image.writeCMYKDataToFilter(&flateEncode);
			else
				succeed = image.writeRGBDataToFilter(&flateEncode);
			succeed &= flateEncode.closeFilter();
			bytesWritten = flateEncode.writtenToStream();
		}
	}
	else
	{
		ScFlateEncodeFilter flateEncode(&outStream);
		if (flateEncode.openFilter())
		{
			if (gray)
				succeed = image.writeGrayDataToFilter(&flateEncode, precal);
			else if (cmyk)
				succeed = image.writeCMYKDataToFilter(&flateEncode);
			else
				succeed = image.writeRGBDataToFilter(&flateEncode);
			succeed &= flateEncode.closeFilter();
			bytesWritten = flateEncode.writtenToStream();
		}
	}
	return (succeed ? bytesWritten : 0);
}

QString PDFLibCore::FitKey(const QString & pass)
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

void PDFLibCore::CalcOwnerKey(const QString & Owner, const QString & User)
{
	rc4_context_t rc4;
	QString pw(FitKey(User));
	QString pw2(FitKey(Owner.isEmpty() ? User : Owner));
	QByteArray step1(16, ' ');
	step1 = ComputeMD5(pw2);
	if (KeyLen > 5)
	{
		for (int kl = 0; kl < 50; ++kl)
			step1 = ComputeMD5Sum(&step1);
	}
	QByteArray us(32, ' ');
	QByteArray enk(16, ' ');
	if (KeyLen > 5)
	{
		for (uint a2 = 0; a2 < 32; ++a2)
			OwnerKey[a2] = QChar(pw.at(a2)).cell();
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
			us[a] = static_cast<uchar>(QChar(pw.at(a)).cell());
		rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), 5);
		rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()),
					reinterpret_cast<uchar*>(OwnerKey.data()), 32);
	}
}

void PDFLibCore::CalcUserKey(const QString & User, int Permission)
{
	rc4_context_t	rc4;
	QString pw(FitKey(User));
	QByteArray step1(16, ' ');
	QByteArray perm(4, ' ');
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
		QByteArray enk(16, ' ');
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

QByteArray PDFLibCore::ComputeMD5(const QString& in)
{
	uint inlen=in.length();
	QByteArray TBytes(inlen, ' ');
	for (uint a = 0; a < inlen; ++a)
		TBytes[a] = static_cast<uchar>(QChar(in.at(a)).cell());
	return ComputeMD5Sum(&TBytes);
}

QByteArray PDFLibCore::ComputeRC4Key(int ObjNum)
{
	int dlen = 0;
	QByteArray data(10, ' ');
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
	QByteArray rc4Key(16, ' ');
	rc4Key = ComputeMD5Sum(&data);
	rc4Key.resize(qMin(KeyLen+5, 16));
	return rc4Key;
}

bool PDFLibCore::PDF_Begin_Doc(const QString& fn, SCFonts &AllFonts, QMap<QString, QMap<uint, FPointArray> > DocFonts, BookMView* vi)
{
	Spool.setFileName(fn);
	if (!Spool.open(QIODevice::WriteOnly))
		return false;
	outStream.setDevice(&Spool);
	QString tmp;
	QString ok = "";
	QString uk = "";
	QFileInfo fd;
	QString fext;
	int a;
	inPattern = 0;
	Bvie = vi;
	BookMinUse = false;
	UsedFontsP.clear();
	UsedFontsF.clear();
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
		ObjCounter = 10;
	else
		ObjCounter = 9;
	switch (Options.Version)
	{
		case PDFOptions::PDFVersion_X1a:
		case PDFOptions::PDFVersion_X3:
		case PDFOptions::PDFVersion_13:
			PutDoc("%PDF-1.3\n");
			break;
		case PDFOptions::PDFVersion_14:
			PutDoc("%PDF-1.4\n");
			break;
		case PDFOptions::PDFVersion_X4:
		case PDFOptions::PDFVersion_15:
			PutDoc("%PDF-1.5\n");
			break;
	}
	if ((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || (Options.Version == PDFOptions::PDFVersion_X4))
		ObjCounter++;
	PutDoc("%\xc7\xec\x8f\xa2\n");
	StartObj(1);
	PutDoc("<<\n/Type /Catalog\n/Outlines 3 0 R\n/Pages 4 0 R\n/Dests 5 0 R\n/AcroForm 6 0 R\n/Names 7 0 R\n/Threads 8 0 R\n");
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
		PutDoc("/OCProperties 9 0 R\n");
	if ((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || (Options.Version == PDFOptions::PDFVersion_X4))
		PutDoc("/OutputIntents [ "+QString::number(ObjCounter-1)+" 0 R ]\n");
	if ((Options.Version == PDFOptions::PDFVersion_X4))
	{
		ObjCounter++;
		PutDoc("/Metadata "+QString::number(ObjCounter-1)+" 0 R\n");
	}
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
	else if ((Options.Version == PDFOptions::PDFVersion_15) && (Options.displayLayers))
			PutDoc("/PageMode /UseOC\n");
	if (!Options.openAction.isEmpty())
	{
		PutDoc("/OpenAction << /S /JavaScript /JS (this."+Options.openAction+"\\(\\)) >>\n");
	}
	QDateTime dt = QDateTime::currentDateTime().toUTC();
	QDate d = dt.date();
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
	tmp = dt.time().toString();
	tmp.replace(QRegExp(":"), "");
	Datum += tmp;
	Datum += "Z";

	// only include XMP to PDF/X-4 at the moment, could easily be extended to include it to any PDF
	if (Options.Version == PDFOptions::PDFVersion_X4)
		generateXMP(dt.toString("yyyy-MM-ddThh:mm:ssZ"));

/* The following code makes the resulting PDF "Reader enabled" in Acrobat Reader 8
   but sadly it doesn't work with newer version, because its based on a bug in AR 8
	PutDoc("/Perms\n");
	PutDoc("<<\n");
	PutDoc("/UR3\n");
	PutDoc("<<\n");
	PutDoc("/M ("+Datum+")\n");
	PutDoc("/Name (Scribus "+QString(VERSION)+")\n");
	PutDoc("/Reference [\n");
	PutDoc("<<\n");
	PutDoc("/TransformParams\n");
	PutDoc("<<\n");
	PutDoc("/Type /TransformParams\n");
	PutDoc("/V /2.2\n");
	PutDoc("/Document [/FullSave]\n");
	PutDoc("/Annots [/Create/Delete/Modify/Copy/Import/Export]\n");
	PutDoc("/Form [/Add/Delete/FillIn/Import/Export/SubmitStandalone/SpawnTemplate]\n");
	PutDoc("/Signature [/Modify]\n");
	PutDoc(">>\n");
	PutDoc("/TransformMethod /UR3\n");
	PutDoc("/Type /SigRef\n");
	PutDoc(">>\n");
	PutDoc("]\n");
	PutDoc("/Type /Sig\n");
	PutDoc(">>\n");
	PutDoc(">>\n");
*/

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
	IDg += Options.fileName;
	IDg += "Scribus "+QString(VERSION);
	IDg += "Scribus PDF Library "+QString(VERSION);
	IDg += doc.documentInfo().title();
	IDg += doc.documentInfo().author();
	IDg += "/False";
	FileID = ComputeMD5(IDg);
	if (Options.Encrypt)
	{
		if ((Options.Version == PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_15))
			KeyLen = 16;
		else
			KeyLen = 5;
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
	StartObj(2);
	PutDoc("<<\n/Creator "+EncString("(Scribus "+QString(VERSION)+")",2)+"\n");
	PutDoc("/Producer "+EncString("(Scribus PDF Library "+QString(VERSION)+")",2)+"\n");
	QString docTitle = doc.documentInfo().title();
	if (((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || (Options.Version == PDFOptions::PDFVersion_X4)) && (docTitle.isEmpty()))
		PutDoc("/Title "+EncStringUTF16("("+doc.DocName+")",2)+"\n");
	else
		PutDoc("/Title "+EncStringUTF16("("+doc.documentInfo().title()+")",2)+"\n");
	PutDoc("/Author "+EncStringUTF16("("+doc.documentInfo().author()+")",2)+"\n");
	PutDoc("/Subject "+EncStringUTF16("("+doc.documentInfo().subject()+")",2)+"\n");
	PutDoc("/Keywords "+EncStringUTF16("("+doc.documentInfo().keywords()+")",2)+"\n");
	PutDoc("/CreationDate "+EncString("("+Datum+")",2)+"\n");
	PutDoc("/ModDate "+EncString("("+Datum+")",2)+"\n");
	if (Options.Version == PDFOptions::PDFVersion_X3)
		PutDoc("/GTS_PDFXVersion (PDF/X-3:2002)\n");
	if (Options.Version == PDFOptions::PDFVersion_X1a)
	{
		PutDoc("/GTS_PDFXVersion (PDF/X-1:2001)\n");
		PutDoc("/GTS_PDFXConformance (PDF/X-1a:2001)\n");
	}
	if (Options.Version == PDFOptions::PDFVersion_X4)
		PutDoc("/GTS_PDFXVersion (PDF/X-4)\n");
	PutDoc("/Trapped /False\n>>\nendobj\n");
	for (int t = 0; t < 6; ++t)
		XRef.append(bytesWritten());
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
		XRef.append(bytesWritten());
	if ((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || (Options.Version == PDFOptions::PDFVersion_X4))
		XRef.append(bytesWritten());
	if (Options.Version == PDFOptions::PDFVersion_X4)
		XRef.append(bytesWritten());
	if (Options.Encrypt)
	{
		Encrypt = newObject();
		StartObj(Encrypt);
		PutDoc("<<\n/Filter /Standard\n");
		PutDoc( KeyLen > 5 ? "/R 3\n/V 2\n/Length 128\n" : "/R 2\n/V 1\n");
		PutDoc("/O <"+String2Hex(&ok)+">\n");
		PutDoc("/U <"+String2Hex(&uk)+">\n");
		PutDoc("/P "+QString::number(Options.Permissions)+"\n>>\nendobj\n");
	}
	QMap<QString, QMap<uint, FPointArray> > ReallyUsed;
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
	for (int c = 0; c < doc.FrameItems.count(); ++c)
	{
		pgit = doc.FrameItems.at(c);
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= 2) && (annotType <= 6) && (annotType != 4));
				if (pgit->annotation().Type() == 4)
					StdFonts.insert("/ZapfDingbats", "");
				if (pgit->itemText.length() > 0 || mustEmbed)
				{
					if (Options.Version < PDFOptions::PDFVersion_14)
						StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
					ReallyUsed.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), DocFonts[pgit->itemText.defaultStyle().charStyle().font().replacementName()]);
				}
			}
			for (uint e = 0; e < static_cast<uint>(pgit->itemText.length()); ++e)
			{
				ReallyUsed.insert(pgit->itemText.charStyle(e).font().replacementName(), DocFonts[pgit->itemText.charStyle(e).font().replacementName()]);
			}
		}
	}
	for (int c = 0; c < doc.MasterItems.count(); ++c)
	{
		pgit = doc.MasterItems.at(c);
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= 2) && (annotType <= 6) && (annotType != 4));
				if (pgit->annotation().Type() == 4)
					StdFonts.insert("/ZapfDingbats", "");
				if (pgit->itemText.length() > 0 || mustEmbed)
				{
					if (Options.Version < PDFOptions::PDFVersion_14)
						StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
					ReallyUsed.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), DocFonts[pgit->itemText.defaultStyle().charStyle().font().replacementName()]);
				}
			}
			for (uint e = 0; e < static_cast<uint>(pgit->itemText.length()); ++e)
			{
				ReallyUsed.insert(pgit->itemText.charStyle(e).font().replacementName(), DocFonts[pgit->itemText.charStyle(e).font().replacementName()]);
			}
		}
	}
	for (int d = 0; d < doc.Items->count(); ++d)
	{
		pgit = doc.Items->at(d);
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= 2) && (annotType <= 6) && (annotType != 4));
				if (pgit->annotation().Type() == 4)
					StdFonts.insert("/ZapfDingbats", "");
				if (pgit->itemText.length() > 0 || mustEmbed)
				{
					if (Options.Version < PDFOptions::PDFVersion_14)
						StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
					ReallyUsed.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), DocFonts[pgit->itemText.defaultStyle().charStyle().font().replacementName()]);
				}
			}
			for (uint e = 0; e < static_cast<uint>(pgit->itemText.length()); ++e)
			{
				ReallyUsed.insert(pgit->itemText.charStyle(e).font().replacementName(), DocFonts[pgit->itemText.charStyle(e).font().replacementName()]);
			}
		}
	}
/*	if (Options.docInfoMarks)
	{
		StdFonts.insert("/Helvetica", "");
	} */
	QStringList patterns = doc.getPatternDependencyList(doc.getUsedPatterns());
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = doc.docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			pgit = pa.items.at(o);
			if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
			{
				if (pgit->isAnnotation())
				{
					if (pgit->annotation().Type() == 4)
						StdFonts.insert("/ZapfDingbats", "");
					if (pgit->itemText.length() > 0)
					{
						if (Options.Version < PDFOptions::PDFVersion_14)
							StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
						ReallyUsed.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), DocFonts[pgit->itemText.defaultStyle().charStyle().font().replacementName()]);
					}
				}
				for (uint e = 0; e < static_cast<uint>(pgit->itemText.length()); ++e)
				{
					ReallyUsed.insert(pgit->itemText.charStyle(e).font().replacementName(), DocFonts[pgit->itemText.charStyle(e).font().replacementName()]);
				}
			}
		}
	}
	a = 0;
	QMap<QString, QString>::Iterator itStd;
	for (itStd = StdFonts.begin(); itStd != StdFonts.end(); ++itStd)
	{
		uint fontObject = newObject();
		StartObj(fontObject);
		PutDoc("<<\n/Type /Font\n/Subtype /Type1\n");
		PutDoc("/Name /FoStd"+QString::number(a)+"\n");
		PutDoc("/BaseFont "+itStd.key()+"\n");
		if (itStd.key() != "/ZapfDingbats")
		{
			PutDoc("/Encoding << \n");
			PutDoc("/Differences [ \n");
			PutDoc("24 /breve /caron /circumflex /dotaccent /hungarumlaut /ogonek /ring /tilde\n");
			PutDoc("39 /quotesingle 96 /grave 128 /bullet /dagger /daggerdbl /ellipsis /emdash /endash /florin /fraction /guilsinglleft /guilsinglright\n");
			PutDoc("/minus /perthousand /quotedblbase /quotedblleft /quotedblright /quoteleft /quoteright /quotesinglbase /trademark /fi /fl /Lslash /OE /Scaron\n");
			PutDoc("/Ydieresis /Zcaron /dotlessi /lslash /oe /scaron /zcaron 164 /currency 166 /brokenbar 168 /dieresis /copyright /ordfeminine 172 /logicalnot\n");
			PutDoc("/.notdef /registered /macron /degree /plusminus /twosuperior /threesuperior /acute /mu 183 /periodcentered /cedilla /onesuperior /ordmasculine\n");
			PutDoc("188 /onequarter /onehalf /threequarters 192 /Agrave /Aacute /Acircumflex /Atilde /Adieresis /Aring /AE /Ccedilla /Egrave /Eacute /Ecircumflex\n");
			PutDoc("/Edieresis /Igrave /Iacute /Icircumflex /Idieresis /Eth /Ntilde /Ograve /Oacute /Ocircumflex /Otilde /Odieresis /multiply /Oslash\n");
			PutDoc("/Ugrave /Uacute /Ucircumflex /Udieresis /Yacute /Thorn /germandbls /agrave /aacute /acircumflex /atilde /adieresis /aring /ae /ccedilla\n");
			PutDoc("/egrave /eacute /ecircumflex /edieresis /igrave /iacute /icircumflex /idieresis /eth /ntilde /ograve /oacute /ocircumflex /otilde /odieresis\n");
			PutDoc("/divide /oslash /ugrave /uacute /ucircumflex /udieresis /yacute /thorn /ydieresis\n");
			PutDoc("] >>\n");
		}
		PutDoc(">>\nendobj\n");
		Seite.FObjects["FoStd"+QString::number(a)] = fontObject;
		itStd.value() = "FoStd"+QString::number(a);
		a++;
	}
	QMap<QString,QMap<uint, FPointArray> >::Iterator it;
	a = 0;
	for (it = ReallyUsed.begin(); it != ReallyUsed.end(); ++it)
	{
		ScFace& face(AllFonts[it.key()]);
		ScFace::FontFormat fformat = face.format();
		if ((!face.hasNames()) || (Options.SubsetList.contains(it.key())))
		{
			if (face.hasNames())
			{
				UsedFontsP.insert(it.key(), "/Fo"+QString::number(a));
				uint SubFonts = 0;
				int glyphCount = 0;
				double minx = 999999.9;
				double miny = 999999.9;
				double maxx = -999999.9;
				double maxy = -999999.9;
				QList<uint> glyphWidths;
				QStringList charProcs;
				QString encoding = "<< /Type /Encoding\n/Differences [ 0\n";
				QString fon("");
				QMap<uint, uint> glyphMapping;
				QMap<uint,std::pair<QChar,QString> > gl;
				face.glyphNames(gl);
				QMap<uint,FPointArray>& RealGlyphs(it.value());
				QMap<uint,FPointArray>::Iterator ig;
				for (ig = RealGlyphs.begin(); ig != RealGlyphs.end(); ++ig)
				{
					FPoint np, np1, np2;
					bool nPath = true;
					fon = "";
					if (ig.value().size() > 3)
					{
						FPointArray gly = ig.value();
						QTransform mat;
						mat.scale(100.0, -100.0);
						gly.map(mat);
						gly.translate(0, 1000);
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
								fon += FToStr(np.x())+" "+FToStr(np.y())+" m\n";
								nPath = false;
							}
							np = gly.point(poi+1);
							np1 = gly.point(poi+3);
							np2 = gly.point(poi+2);
							fon += FToStr(np.x()) + " " + FToStr(np.y()) + " " + FToStr(np1.x()) + " " + FToStr(np1.y()) + " " + FToStr(np2.x()) + " " + FToStr(np2.y()) + " c\n";
						}
						fon += "h f*\n";
						np = getMinClipF(&gly);
						np1 = getMaxClipF(&gly);
					}
					else
					{
						fon = "h";
						np = FPoint(0, 0);
						np1 = FPoint(0, 0);
					}
					fon.prepend(QString::number(qRound(np1.x())) + " 0 "+QString::number(qRound(np.x()))+" "+QString::number(qRound(np.y()))+" "+QString::number(qRound(np1.x()))+ " "+QString::number(qRound(np1.y()))+" d1\n");
					minx = qMin(minx, np.x());
					miny = qMin(miny, np.y());
					maxx = qMax(maxx, np1.x());
					maxy = qMax(maxy, np1.y());
					glyphWidths.append(qRound(np1.x()));
					uint charProcObject = newObject();
					charProcs.append("/"+gl[ig.key()].second+" "+QString::number(charProcObject)+" 0 R\n");
					encoding += "/"+gl[ig.key()].second+" ";
					glyphMapping.insert(ig.key(), glyphCount + SubFonts * 256);
					StartObj(charProcObject);
					if (Options.Compress)
						fon = CompressStr(&fon);
					PutDoc("<< /Length "+QString::number(fon.length()+1));
					if (Options.Compress)
						PutDoc("\n/Filter /FlateDecode");
					PutDoc("\n>>\nstream\n"+EncStream(fon, charProcObject)+"\nendstream\nendobj\n");
					glyphCount++;
					int glyphsLeft = RealGlyphs.count() - SubFonts * 256;
					if ((glyphCount > 255) || (glyphCount == glyphsLeft))
					{
						uint fontWidths = newObject();
						StartObj(fontWidths);
						PutDoc("[ ");
						for (int ww = 0; ww < glyphWidths.count(); ++ww)
						{
							PutDoc(QString::number(qRound(glyphWidths[ww]))+" ");
						}
						PutDoc("]\nendobj\n");
						uint fontCharProcs = newObject();
						StartObj(fontCharProcs);
						PutDoc("<<\n");
						for (int ww = 0; ww < charProcs.count(); ++ww)
						{
							PutDoc(charProcs[ww]);
						}
						PutDoc(">>\nendobj\n");
						uint fontEncoding = newObject();
						StartObj(fontEncoding);
						PutDoc(encoding);
						PutDoc("]\n");
						PutDoc(">>\nendobj\n");
						uint font3Object = newObject();
						StartObj(font3Object);
						PutDoc("<<\n/Type /Font\n/Subtype /Type3\n");
						PutDoc("/Name /Fo"+QString::number(a)+"S"+QString::number(SubFonts)+"\n");
						PutDoc("/FirstChar 0\n");
						PutDoc("/LastChar "+QString::number(glyphCount-1)+"\n");
						PutDoc("/Widths "+QString::number(fontWidths)+" 0 R\n");
						PutDoc("/CharProcs "+QString::number(fontCharProcs)+" 0 R\n");
						PutDoc("/FontBBox ["+QString::number(qRound(minx))+" "+QString::number(qRound(miny))+" "+QString::number(qRound(maxx))+ " "+QString::number(qRound(maxy))+"]\n");
						PutDoc("/FontMatrix [0.001 0 0 0.001 0 0]\n");
						PutDoc("/Encoding "+QString::number(fontEncoding)+" 0 R\n");
						PutDoc(">>\nendobj\n");
						Seite.FObjects["Fo"+QString::number(a)+"S"+QString::number(SubFonts)] = font3Object;
						charProcs.clear();
						glyphWidths.clear();
//						glyphMapping.clear();
						glyphCount = 0;
						++SubFonts;
						minx = 999999.9;
						miny = 999999.9;
						maxx = -999999.9;
						maxy = -999999.9;
						encoding = "<< /Type /Encoding\n/Differences [ 0\n";
					}
				}
				Type3Fonts.insert("/Fo"+QString::number(a), glyphMapping);
			}
			else
			{
				QString fon("");
				QMap<uint,FPointArray>& RealGlyphs(it.value());
				QMap<uint,FPointArray>::Iterator ig;
				for (ig = RealGlyphs.begin(); ig != RealGlyphs.end(); ++ig)
				{
					FPoint np, np1, np2;
					bool nPath = true;
					fon = "";
					if (ig.value().size() > 3)
					{
						FPointArray gly = ig.value();
						QTransform mat;
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
						np = getMinClipF(&gly);
						np1 = getMaxClipF(&gly);
					}
					else
					{
						fon = "h";
						np = FPoint(0, 0);
						np1 = FPoint(0, 0);
					}
					uint fontGlyphXForm = newObject();
					StartObj(fontGlyphXForm);
					PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
					PutDoc("/BBox [ "+FToStr(np.x())+" "+FToStr(-np.y())+" "+FToStr(np1.x())+ " "+FToStr(-np1.y())+" ]\n");
					PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
					PutDoc(">>\n");
					if (Options.Compress)
						fon = CompressStr(&fon);
					PutDoc("/Length "+QString::number(fon.length()+1));
					if (Options.Compress)
						PutDoc("\n/Filter /FlateDecode");
					PutDoc(" >>\nstream\n"+EncStream(fon, fontGlyphXForm)+"\nendstream\nendobj\n");
					Seite.XObjects[AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+QString::number(ig.key())] = fontGlyphXForm;
				}
			}
		}
		else
		{
			UsedFontsP.insert(it.key(), "/Fo"+QString::number(a));
			uint embeddedFontObject = 0;
			if ((fformat == ScFace::PFB) && (Options.EmbedList.contains(it.key())))
			{
				QString fon("");
				embeddedFontObject = newObject();
				StartObj(embeddedFontObject);
				QByteArray bb;
				AllFonts[it.key()].RawData(bb);
				int posi;
				for (posi = 6; posi < bb.size(); ++posi)
				{
					if ((bb[posi] == static_cast<char>(0x80)) && (static_cast<int>(bb[posi+1]) == 2))
						break;
					fon += QChar(bb[posi]);
				}
				int len1 = fon.length();
				int ulen;
				ulen = bb[posi+2] & 0xff;
				ulen |= (bb[posi+3] << 8) & 0xff00;
				ulen |= (bb[posi+4] << 16) & 0xff0000;
				ulen |= (bb[posi+5] << 24) & 0xff000000;
				if (ulen > bb.size())
					ulen = bb.size()-7;
				posi += 6;
				for (int j = 0; j < ulen; ++j)
					fon += QChar(bb[posi++]);
				posi += 6;
				int len2 = fon.length()-len1;
				for (int j = posi; j < bb.size(); ++j)
				{
					if ((bb[j] == static_cast<char>(0x80)) && (static_cast<int>(bb[j+1]) == 3))
						break;
					if (bb[j] == '\r')
						fon += "\n";
					else
						fon += QChar(bb[j]);
				}
				int len3 = fon.length()-len2-len1;
				if (Options.Compress)
					fon = CompressStr(&fon);
				PutDoc("<<\n/Length "+QString::number(fon.length()+1)+"\n");
				PutDoc("/Length1 "+QString::number(len1)+"\n");
				PutDoc("/Length2 "+QString::number(len2)+"\n");
				PutDoc("/Length3 "+QString::number(len3)+"\n");
				if (Options.Compress)
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n"+EncStream(fon,embeddedFontObject)+"\nendstream\nendobj\n");
			}
			if ((fformat == ScFace::PFA) && (Options.EmbedList.contains(it.key())))
			{
				QString fon("");
				QString fon2("");
				QString tm("");
				uint value;
				bool ok = true;
				embeddedFontObject = newObject();
				StartObj(embeddedFontObject);
				AllFonts[it.key()].EmbedFont(fon);
				int len1 = fon.indexOf("eexec")+5;
				fon2 = fon.left(len1)+"\n";
				int len2 = fon.indexOf("0000000000000000000000000");
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
				if (Options.Compress)
					fon2 = CompressStr(&fon2);
				PutDoc("<<\n/Length "+QString::number(fon2.length()+1)+"\n");
				PutDoc("/Length1 "+QString::number(len1+1)+"\n");
				PutDoc("/Length2 "+QString::number(count)+"\n");
				PutDoc(static_cast<int>(fon.length()-len2) == -1 ? QString("/Length3 0\n") : "/Length3 "+QString::number(fon.length()-len2)+"\n");
				if (Options.Compress)
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n"+EncStream(fon2, embeddedFontObject)+"\nendstream\nendobj\n");
			}
			if ((fformat == ScFace::SFNT || fformat == ScFace::TTCF) && (Options.EmbedList.contains(it.key())))
			{
				QString fon("");
				embeddedFontObject = newObject();
				StartObj(embeddedFontObject);
				QByteArray bb;
				AllFonts[it.key()].RawData(bb);
				//AV: += and append() dont't work because they stop at '\0' :-(
				for (int i=0; i < bb.size(); i++)
					fon += QChar(bb[i]);
				int len = fon.length();
				if (Options.Compress)
					fon = CompressStr(&fon);
				//qDebug() << QString("sfnt data: size=%1 before=%2 compressed=%3").arg(bb.size()).arg(len).arg(fon.length());
				PutDoc("<<\n/Length "+QString::number(fon.length()+1)+"\n");
				PutDoc("/Length1 "+QString::number(len)+"\n");
				if (Options.Compress)
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n"+EncStream(fon, embeddedFontObject)+"\nendstream\nendobj\n");
			}
			uint fontDescriptor = newObject();
			StartObj(fontDescriptor);
			// TODO: think about QByteArray ScFace::getFontDescriptor() -- AV
			PutDoc("<<\n/Type /FontDescriptor\n");
			PutDoc("/FontName /"+AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+"\n");
			PutDoc("/FontBBox [ "+AllFonts[it.key()].fontBBoxAsString()+" ]\n");
			PutDoc("/Flags ");
			//FIXME: isItalic() should be queried from ScFace, not from Qt -- AV
			//QFontInfo fo = QFontInfo(it.data());
			int pfl = 0;
			if (AllFonts[it.key()].isFixedPitch())
				pfl = pfl ^ 1;
			//if (fo.italic())
			if (AllFonts[it.key()].italicAngleAsString() != "0")
				pfl = pfl ^ 64;
//			pfl = pfl ^ 4;
			pfl = pfl ^ 32;
			PutDoc(QString::number(pfl)+"\n");
			PutDoc("/Ascent "+AllFonts[it.key()].ascentAsString()+"\n");
			PutDoc("/Descent "+AllFonts[it.key()].descentAsString()+"\n");
			PutDoc("/CapHeight "+AllFonts[it.key()].capHeightAsString()+"\n");
			PutDoc("/ItalicAngle "+AllFonts[it.key()].italicAngleAsString()+"\n");
//			PutDoc("/Ascent "+QString::number(static_cast<int>(AllFonts[it.key()].ascent()))+"\n");
//			PutDoc("/Descent "+QString::number(static_cast<int>(AllFonts[it.key()].descent()))+"\n");
//			PutDoc("/CapHeight "+QString::number(static_cast<int>(AllFonts[it.key()].capHeight()))+"\n");
//			PutDoc("/ItalicAngle "+AllFonts[it.key()].italicAngle()+"\n");
//			PutDoc("/StemV "+ AllFonts[it.key()].stemV() + "\n");
			PutDoc("/StemV 1\n");
			if ((fformat == ScFace::SFNT || fformat == ScFace::TTCF) && (Options.EmbedList.contains(it.key())))
				PutDoc("/FontFile2 "+QString::number(embeddedFontObject)+" 0 R\n");
			if ((fformat == ScFace::PFB) && (Options.EmbedList.contains(it.key())))
				PutDoc("/FontFile "+QString::number(embeddedFontObject)+" 0 R\n");
			if ((fformat == ScFace::PFA) && (Options.EmbedList.contains(it.key())))
				PutDoc("/FontFile "+QString::number(embeddedFontObject)+" 0 R\n");
			PutDoc(">>\nendobj\n");
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
				PutDoc((fformat == ScFace::SFNT || fformat == ScFace::TTCF) ? "/TrueType\n" : "/Type1\n");
				PutDoc("/Name /Fo"+QString::number(a)+"\n");
				PutDoc("/BaseFont /"+AllFonts[it.key()]->psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "" )+"\n");
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
				QMap<uint,std::pair<QChar,QString> > gl;
				AllFonts[it.key()].glyphNames(gl);
				int nglyphs = 0;
				QMap<uint,std::pair<QChar,QString> >::Iterator gli;
				for (gli = gl.begin(); gli != gl.end(); ++gli)
				{
					if (gli.key() > static_cast<uint>(nglyphs))
						nglyphs = gli.key();
				}
				++nglyphs;
//				qDebug() << QString("pdflib: nglyphs %1 max %2").arg(nglyphs).arg(AllFonts[it.key()].maxGlyph());
				uint FontDes = fontDescriptor;
				if (Options.Version == PDFOptions::PDFVersion_X4 && (fformat == ScFace::SFNT || fformat == ScFace::TTCF))
				{
					uint fontWidths2 = newObject();
					StartObj(fontWidths2);
					QStringList toUnicodeMaps;
					QList<int> toUnicodeMapsCount;
					QString toUnicodeMap = "";
					int toUnicodeMapCounter = 0;

					PutDoc("[ ");
					QList<uint> keys = gl.uniqueKeys();
					QList<uint>::iterator git;
					for (git = keys.begin(); git != keys.end(); ++git)
					{
						PutDoc(QString::number(*git)+" ["+QString::number(static_cast<int>(AllFonts[it.key()].glyphWidth(*git)* 1000))+"] " );
						QString tmp, tmp2;
						tmp.sprintf("%02X", *git);
						tmp2.sprintf("%04X", gl.value(*git).first.unicode());
						toUnicodeMap += QString("<%1> <%2>\n").arg(tmp).arg((tmp2));
						toUnicodeMapCounter++;
						if (toUnicodeMapCounter == 100)
						{
							toUnicodeMaps.append(toUnicodeMap);
							toUnicodeMapsCount.append(toUnicodeMapCounter);
							toUnicodeMap = "";
							toUnicodeMapCounter = 0;
						}
					}
					PutDoc("]\nendobj\n");
					if (toUnicodeMapCounter != 0)
					{
						toUnicodeMaps.append(toUnicodeMap);
						toUnicodeMapsCount.append(toUnicodeMapCounter);
					}
					QString toUnicodeMapStream = "";
					toUnicodeMapStream += "/CIDInit /ProcSet findresource begin\n";
					toUnicodeMapStream += "12 dict begin\n";
					toUnicodeMapStream += "begincmap\n";
					toUnicodeMapStream += "/CIDSystemInfo <<\n";
					toUnicodeMapStream += "/Registry (Adobe)\n";
					toUnicodeMapStream += "/Ordering (UCS)\n";
					toUnicodeMapStream += "/Supplement 0\n";
					toUnicodeMapStream += ">> def\n";
					toUnicodeMapStream += "/CMapName /Adobe-Identity-UCS def\n";
					toUnicodeMapStream += "/CMapType 2 def\n";
					toUnicodeMapStream += "1 begincodespacerange\n";
					toUnicodeMapStream += "<0000> <FFFF>\n";
					toUnicodeMapStream += "endcodespacerange\n";
					for (int uniC = 0; uniC < toUnicodeMaps.count(); uniC++)
					{
						toUnicodeMapStream += QString("%1 beginbfchar\n").arg(toUnicodeMapsCount[uniC]);
						toUnicodeMapStream += toUnicodeMaps[uniC];
						toUnicodeMapStream += "endbfchar\n";
					}
					toUnicodeMapStream += "endcmap\n";
					toUnicodeMapStream += "CMapName currentdict /CMap defineresource pop\n";
					toUnicodeMapStream += "end\n";
					toUnicodeMapStream += "end\n";
					uint fontToUnicode2 = WritePDFStream(toUnicodeMapStream);
					uint fontObject2 = newObject();
					StartObj(fontObject2);
					PutDoc("<<\n/Type /Font\n/Subtype /Type0\n");
					PutDoc("/Name /Fo"+QString::number(a)+"\n");
					PutDoc("/BaseFont /"+AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+"\n");
					PutDoc("/Encoding /Identity-H\n");
					PutDoc("/ToUnicode "+QString::number(fontToUnicode2)+" 0 R\n");
					PutDoc("/DescendantFonts [");
					PutDoc("<</Type /Font");
					PutDoc("/Subtype /CIDFontType2");
					PutDoc("/BaseFont /"+AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ));
					PutDoc("/FontDescriptor "+QString::number(FontDes)+" 0 R");
					PutDoc("/CIDSystemInfo <</Ordering(Identity)/Registry(Adobe)/Supplement 0>>");
					PutDoc("/DW 1000");
					PutDoc("/W "+QString::number(fontWidths2)+" 0 R");
					PutDoc("/CIDToGIDMap /Identity");
					PutDoc(">>"); // close CIDFont dictionary
					PutDoc("]\n"); // close DescendantFonts array
					PutDoc(">>\nendobj\n");
					Seite.FObjects["Fo"+QString::number(a)] = fontObject2;
				}
				else
				{
					uint Fcc = nglyphs / 224;
					if ((nglyphs % 224) != 0)
						Fcc += 1;
					for (uint Fc = 0; Fc < Fcc; ++Fc)
					{
						uint fontWidths2 = newObject();
						StartObj(fontWidths2);
						int chCount = 32;
						PutDoc("[ 0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 ");
						for (int ww = 32; ww < 256; ++ww)
						{
							uint glyph = 224 * Fc + ww - 32;
							if (gl.contains(glyph))
								PutDoc(QString::number(static_cast<int>(AllFonts[it.key()].glyphWidth(glyph)* 1000))+" ");
							else
								PutDoc("0 ");
							chCount++;
							if (signed(glyph) == nglyphs-1)
								break;
						}
						PutDoc("]\nendobj\n");
						uint fontEncoding2 = newObject();
						StartObj(fontEncoding2);
						QStringList toUnicodeMaps;
						QList<int> toUnicodeMapsCount;
						QString toUnicodeMap = "";
						int toUnicodeMapCounter = 0;
						PutDoc("<< /Type /Encoding\n");
						PutDoc("/Differences [ \n");
						int crc = 0;
						bool startOfSeq = true;
						for (int ww2 = 32; ww2 < 256; ++ww2)
						{
							uint glyph = 224 * Fc + ww2 - 32;
							QMap<uint,std::pair<QChar,QString> >::Iterator glIt = gl.find(glyph);
							if (glIt != gl.end() && !glIt.value().second.isEmpty())
							{
								if (startOfSeq)
								{
									PutDoc(QString::number(ww2)+" ");
									startOfSeq = false;
								}
								PutDoc("/"+glIt.value().second+" ");
								QString tmp, tmp2;
								tmp.sprintf("%02X", ww2);
								tmp2.sprintf("%04X", glIt.value().first.unicode());
								toUnicodeMap += QString("<%1> <%2>\n").arg(tmp).arg((tmp2));
								toUnicodeMapCounter++;
								if (toUnicodeMapCounter == 100)
								{
									toUnicodeMaps.append(toUnicodeMap);
									toUnicodeMapsCount.append(toUnicodeMapCounter);
									toUnicodeMap = "";
									toUnicodeMapCounter = 0;
								}
								crc++;
							}
							else
							{
								startOfSeq = true;
							}
							if (signed(glyph) == nglyphs-1)
								break;
							if (crc > 8)
							{
								PutDoc("\n");
								crc = 0;
							}
						}
						if (toUnicodeMapCounter != 0)
						{
							toUnicodeMaps.append(toUnicodeMap);
							toUnicodeMapsCount.append(toUnicodeMapCounter);
						}
						PutDoc("]\n");
						PutDoc(">>\nendobj\n");
						QString toUnicodeMapStream = "";
						toUnicodeMapStream += "/CIDInit /ProcSet findresource begin\n";
						toUnicodeMapStream += "12 dict begin\n";
						toUnicodeMapStream += "begincmap\n";
						toUnicodeMapStream += "/CIDSystemInfo <<\n";
						toUnicodeMapStream += "/Registry (Adobe)\n";
						toUnicodeMapStream += "/Ordering (UCS)\n";
						toUnicodeMapStream += "/Supplement 0\n";
						toUnicodeMapStream += ">> def\n";
						toUnicodeMapStream += "/CMapName /Adobe-Identity-UCS def\n";
						toUnicodeMapStream += "/CMapType 2 def\n";
						toUnicodeMapStream += "1 begincodespacerange\n";
						toUnicodeMapStream += "<0000> <FFFF>\n";
						toUnicodeMapStream += "endcodespacerange\n";
						for (int uniC = 0; uniC < toUnicodeMaps.count(); uniC++)
						{
							toUnicodeMapStream += QString("%1 beginbfchar\n").arg(toUnicodeMapsCount[uniC]);
							toUnicodeMapStream += toUnicodeMaps[uniC];
							toUnicodeMapStream += "endbfchar\n";
						}
						toUnicodeMapStream += "endcmap\n";
						toUnicodeMapStream += "CMapName currentdict /CMap defineresource pop\n";
						toUnicodeMapStream += "end\n";
						toUnicodeMapStream += "end\n";
						uint fontToUnicode2 = WritePDFStream(toUnicodeMapStream);
						uint fontObject2 = newObject();
						StartObj(fontObject2);
						PutDoc("<<\n/Type /Font\n/Subtype ");
						PutDoc((fformat == ScFace::SFNT || fformat == ScFace::TTCF) ? "/TrueType\n" : "/Type1\n");
						PutDoc("/Name /Fo"+QString::number(a)+"S"+QString::number(Fc)+"\n");
						PutDoc("/BaseFont /"+AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+"\n");
						PutDoc("/FirstChar 0\n");
						PutDoc("/LastChar "+QString::number(chCount-1)+"\n");
						PutDoc("/Widths "+QString::number(fontWidths2)+" 0 R\n");
						PutDoc("/Encoding "+QString::number(fontEncoding2)+" 0 R\n");
						PutDoc("/ToUnicode "+QString::number(fontToUnicode2)+" 0 R\n");
						PutDoc("/FontDescriptor "+QString::number(FontDes)+" 0 R\n");
						PutDoc(">>\nendobj\n");
						Seite.FObjects["Fo"+QString::number(a)+"S"+QString::number(Fc)] = fontObject2;
					} // for(Fc)
					uint fontWidthsForm = newObject();
					StartObj(fontWidthsForm);
					PutDoc("[ 0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 ");
					for (int ww = 32; ww < 256; ++ww)
					{
						uint glyph = AllFonts[it.key()].char2CMap(QChar(ww));
						if (gl.contains(glyph))
							PutDoc(QString::number(static_cast<int>(AllFonts[it.key()].glyphWidth(glyph)* 1000))+" ");
						else
							PutDoc("0 ");
					}
					PutDoc("]\nendobj\n");
					uint fontObjectForm = newObject();
					StartObj(fontObjectForm);
					PutDoc("<<\n/Type /Font\n/Subtype ");
					PutDoc((fformat == ScFace::SFNT || fformat == ScFace::TTCF) ? "/TrueType\n" : "/Type1\n");
	//				if (fformat == ScFace::SFNT || fformat == ScFace::TTCF)
	//				{
	//					PutDoc("/TrueType\n");
						PutDoc("/Name /Fo"+QString::number(a)+"Form"+"\n");
						Seite.FObjects["Fo"+QString::number(a)+"Form"] = fontObjectForm;
						UsedFontsF.insert(it.key(), "/Fo"+QString::number(a)+"Form");
	/*				}
					else
					{
						PutDoc("/Type1\n");
						PutDoc("/Name /"+AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+"\n");
						Seite.FObjects[AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )] = ObjCounter;
						UsedFontsF.insert(it.key(), "/"+AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ));
					} */
					PutDoc("/BaseFont /"+AllFonts[it.key()].psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+"\n");
					PutDoc("/Encoding << \n");
					PutDoc("/Differences [ \n");
					PutDoc("24 /breve /caron /circumflex /dotaccent /hungarumlaut /ogonek /ring /tilde\n");
					PutDoc("39 /quotesingle 96 /grave 128 /bullet /dagger /daggerdbl /ellipsis /emdash /endash /florin /fraction /guilsinglleft /guilsinglright\n");
					PutDoc("/minus /perthousand /quotedblbase /quotedblleft /quotedblright /quoteleft /quoteright /quotesinglbase /trademark /fi /fl /Lslash /OE /Scaron\n");
					PutDoc("/Ydieresis /Zcaron /dotlessi /lslash /oe /scaron /zcaron 164 /currency 166 /brokenbar 168 /dieresis /copyright /ordfeminine 172 /logicalnot\n");
					PutDoc("/.notdef /registered /macron /degree /plusminus /twosuperior /threesuperior /acute /mu 183 /periodcentered /cedilla /onesuperior /ordmasculine\n");
					PutDoc("188 /onequarter /onehalf /threequarters 192 /Agrave /Aacute /Acircumflex /Atilde /Adieresis /Aring /AE /Ccedilla /Egrave /Eacute /Ecircumflex\n");
					PutDoc("/Edieresis /Igrave /Iacute /Icircumflex /Idieresis /Eth /Ntilde /Ograve /Oacute /Ocircumflex /Otilde /Odieresis /multiply /Oslash\n");
					PutDoc("/Ugrave /Uacute /Ucircumflex /Udieresis /Yacute /Thorn /germandbls /agrave /aacute /acircumflex /atilde /adieresis /aring /ae /ccedilla\n");
					PutDoc("/egrave /eacute /ecircumflex /edieresis /igrave /iacute /icircumflex /idieresis /eth /ntilde /ograve /oacute /ocircumflex /otilde /odieresis\n");
					PutDoc("/divide /oslash /ugrave /uacute /ucircumflex /udieresis /yacute /thorn /ydieresis\n");
					PutDoc("] >>\n");
					PutDoc("/FirstChar 0\n");
					PutDoc("/LastChar 255\n");
					PutDoc("/Widths "+QString::number(fontWidthsForm)+" 0 R\n");
					PutDoc("/FontDescriptor "+QString::number(FontDes)+" 0 R\n");
					PutDoc(">>\nendobj\n");
	//			} // FT_Has_PS_Glyph_Names
				}

		}
		a++;
	}
	if (Options.UseLPI)
	{
		uint halftones = newObject();
		StartObj(halftones);
		PutDoc("<<\n/Type /Halftone\n/HalftoneType 5\n");
		QMap<QString,LPIData>::const_iterator itlp;
		for (itlp = Options.LPISettings.constBegin(); itlp != Options.LPISettings.constEnd(); ++itlp)
		{
			PutDoc("/"+itlp.key()+"\n<<\n/Type /Halftone\n/HalftoneType 1\n/Frequency ");
			PutDoc(QString::number(itlp.value().Frequency)+"\n/Angle "+QString::number(itlp.value().Angle)+"\n/SpotFunction ");
			QString func ("");
			switch (itlp.value().SpotFunc)
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
		HTName = ResNam+QString::number(ResCount);
		Transpar[HTName] = writeGState("/HT "+QString::number(halftones)+" 0 R\n");
		ResCount++;
	}
	if ((doc.HasCMS) && (Options.UseProfiles) && (Options.Version != PDFOptions::PDFVersion_X1a ))
	{
		uint iccProfileObject = newObject();
		StartObj(iccProfileObject);
		QByteArray dataP;
		struct ICCD dataD;
		loadRawBytes(ScCore->InputProfiles[Options.SolidProf], dataP);
		PutDoc("<<\n");
		if (Options.Compress)
		{
			QByteArray compData = CompressArray(dataP);
			if (compData.size() > 0)
			{
				PutDoc("/Filter /FlateDecode\n");
				dataP = compData;
			}
		}
		PutDoc("/Length "+QString::number(dataP.size()+1)+"\n");
		PutDoc("/N "+QString::number(Options.SComp)+"\n");
		PutDoc(">>\nstream\n");
		EncodeArrayToStream(dataP, iccProfileObject);
		PutDoc("\nendstream\nendobj\n");
		uint iccColorspace = newObject();
		StartObj(iccColorspace);
		dataD.ResName = ResNam+QString::number(ResCount);
		dataD.ICCArray = "[ /ICCBased "+QString::number(iccProfileObject)+" 0 R ]";
		dataD.ResNum = iccColorspace;
		dataD.components = Options.SComp;
		ICCProfiles[Options.SolidProf] = dataD;
		PutDoc("[ /ICCBased "+QString::number(iccProfileObject)+" 0 R ]\n");
		PutDoc("endobj\n");
		ResCount++;
	}
	if (((Options.isGrayscale == false) && (Options.UseRGB == false)) && (Options.UseSpotColors))
	{
		doc.getUsedColors(colorsToUse);
		ColorList::Iterator itf;
		for (itf = colorsToUse.begin(); itf != colorsToUse.end(); ++itf)
		{
			if ((colorsToUse[itf.key()].isSpotColor()) || (colorsToUse[itf.key()].isRegistrationColor()))
			{
				CMYKColor cmykValues;
				int cc, cm, cy, ck;
				struct SpotC spotD;
				ScColorEngine::getCMYKValues(colorsToUse[itf.key()], &doc, cmykValues);
				cmykValues.getValues(cc, cm, cy, ck);
				QString colorDesc = "{\ndup "+FToStr(static_cast<double>(cc) / 255)+"\nmul exch dup ";
				colorDesc += FToStr(static_cast<double>(cm) / 255)+"\nmul exch dup ";
				colorDesc += FToStr(static_cast<double>(cy) / 255)+"\nmul exch ";
				colorDesc += FToStr(static_cast<double>(ck) / 255)+" mul }";
				uint separationFunction = newObject();
				StartObj(separationFunction);
				PutDoc("<<\n/FunctionType 4\n");
				PutDoc("/Domain [0.0 1.0]\n");
				PutDoc("/Range [0.0 1.0 0.0 1.0 0.0 1.0 0.0 1.0]\n");
				PutDoc("/Length "+QString::number(colorDesc.length()+1)+"\n");
				PutDoc(">>\nstream\n"+EncStream(colorDesc, separationFunction)+"\nendstream\nendobj\n");
				uint separationColorspace= newObject();
				StartObj(separationColorspace);
				PutDoc("[ /Separation /");
				if (colorsToUse[itf.key()].isRegistrationColor())
					PutDoc("All");
				else
					PutDoc(itf.key().simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				PutDoc(" /DeviceCMYK "+QString::number(separationFunction)+" 0 R ]\nendobj\n");
				spotD.ResName = spotNam+QString::number(spotCount);
				spotD.ResNum = separationColorspace;
				spotMap.insert(itf.key(), spotD);
				spotCount++;
			}
		}
	}
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
	{
		struct SpotC spotD;
		uint registrationColorspace = newObject();
		StartObj(registrationColorspace);
		PutDoc("[ /Separation /All /DeviceCMYK\n");
		PutDoc("<<\n/FunctionType 2\n");
		PutDoc("/Domain [0.0 1.0]\n");
		PutDoc("/Range [0.0 1.0 0.0 1.0 0.0 1.0 0.0 1.0]\n");
		PutDoc("/C0 [0 0 0 0] \n");
		PutDoc("/C1 [1 1 1 1] \n");
		PutDoc("/N 1\n");
		PutDoc(">>\n]\nendobj\n");
		spotD.ResName = spotNam+QString::number(spotCount);
		spotD.ResNum = registrationColorspace;
		spotMapReg.insert("Register", spotD);
		spotCount++;
	}


	if ( ((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
	{
		ScLayer ll;
		struct OCGInfo ocg;
		ll.isPrintable = false;
		ll.ID = 0;
		int Lnr = 0;
		QString ocgNam("oc");
		uint docLayersCount=doc.Layers.count();
		for (uint la = 0; la < docLayersCount; ++la)
		{
			uint optionalContent = newObject();
			QString tmp("");
			doc.Layers.levelToLayer(ll, Lnr);
			ocg.Name = ocgNam+tmp.setNum(ll.ID);
			ocg.ObjNum = optionalContent;
			ocg.visible = ll.isViewable;
			OCGEntries.insert(ll.Name, ocg);
			StartObj(optionalContent);
			PutDoc("<<\n");
			PutDoc("/Type /OCG\n");
			PutDoc("/Name ");
			PutDoc(EncStringUTF16("("+ll.Name+")", optionalContent));
			PutDoc("\n");
			PutDoc("/Usage <</Print <</PrintState ");
			if (ll.isPrintable)
				PutDoc("/ON");
			else
				PutDoc("/OFF");
			PutDoc(">> /View <</ViewState ");
			if (ll.isViewable)
				PutDoc("/ON");
			else
				PutDoc("/OFF");
			PutDoc(">>>>>>");
			PutDoc("\n");
			PutDoc(">>\nendobj\n");
			Lnr++;
		}
	}
	return true;
}

bool PDFLibCore::PDF_TemplatePage(const Page* pag, bool )
{
	QString tmp, tmpOut;
	ActPageP = pag;
	PageItem* ite;
	QList<PageItem*> PItems;
	int Lnr = 0;
	ScLayer ll;
	ll.isPrintable = false;
	ll.ID = 0;
	Content = "";
	Seite.AObjects.clear();
	for (int la = 0; la < doc.Layers.count(); ++la)
	{
		doc.Layers.levelToLayer(ll, Lnr);
		PItems = doc.MasterItems;
		if ((ll.isPrintable) || (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers)))
		{
			if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
				PutPage("/OC /"+OCGEntries[ll.Name].Name+" BDC\n");
			for (int a = 0; a < PItems.count(); ++a)
			{
				Content = "";
				ite =PItems.at(a);
				if (ite->LayerID != ll.ID)
					continue;
				double bLeft, bRight, bBottom, bTop;
				getBleeds(pag, bLeft, bRight, bBottom, bTop);
				double x  = pag->xOffset() - bLeft;
				double y  = pag->yOffset() - bTop;
				double w  = pag->width() + bLeft + bRight;
				double h1 = pag->height() + bBottom + bTop;
				double ilw= ite->lineWidth();
				double x2 = ite->BoundingX - ilw / 2.0;
				double y2 = ite->BoundingY - ilw / 2.0;
				double w2 = ite->BoundingW + ilw;
				double h2 = ite->BoundingH + ilw;
				if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )))
					continue;
				if (ite->ChangedMasterItem)
					continue;
				if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
					continue;
				PutPage("q\n");
				if ((ite->doOverprint) && (!Options.UseRGB))
				{
					QString ShName = ResNam+QString::number(ResCount);
					ResCount++;
					Transpar[ShName] = writeGState("/OP true\n"
												   "/op true\n"
												   "/OPM 1\n");
					PutPage("/"+ShName+" gs\n");
				}
/* Bookmarks on Master Pages do not make any sense */
//				if ((ite->isBookmark) && (Options.Bookmarks))
//					PDF_Bookmark(ite, pag->height() - (ite->yPos() - pag->yOffset()));
				if (!ite->printEnabled() || ((ite->itemType() == PageItem::TextFrame) && (!pag->pageName().isEmpty())))
				{
					PutPage("Q\n");
					continue;
				}
				if (ite->fillColor() != CommonStrings::None)
					PutPage(putColor(ite->fillColor(), ite->fillShade(), true));
				if (ite->lineColor() != CommonStrings::None)
					PutPage(putColor(ite->lineColor(), ite->lineShade(), false));
				Content += FToStr(fabs(ite->lineWidth()))+" w\n";
				if (ite->DashValues.count() != 0)
				{
					PutPage("[ ");
					QVector<double>::iterator it;
					for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
					{
						int da = static_cast<int>(*it);
						// #8758: Custom dotted lines don't export properly to pdf
						// Null values have to be exported if line end != flat
						if ((da != 0) || (ite->lineEnd() != Qt::FlatCap))
							PutPage(QString::number(da)+" ");
					}
					PutPage("] "+QString::number(static_cast<int>(ite->DashOffset))+" d\n");
				}
				else
					PutPage("["+getDashString(ite->PLineArt, ite->lineWidth())+"] 0 d\n");
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
					case PageItem::LatexFrame:
						PutPage("q\n");
						// Same functions as for ImageFrames work for LatexFrames too
						if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
						{
							PutPage(PDF_TransparenzFill(ite));
						}
						if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
						{
							if (ite->GrType != 0)
							{
								if (ite->GrType == 8)
								{
									if (!PDF_PatternFillStroke(tmpOut, ite))
										return false;
								}
								else
								{
									if (!PDF_GradientFillStroke(tmpOut, ite))
										return false;
								}
								PutPage(tmpOut);
							}
							PutPage(SetClipPath(ite));
							if (ite->fillRule)
								PutPage("h\nf*\n");
							else
								PutPage("h\nf\n");
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
						if ((ite->PictureIsAvailable) && (!ite->Pfile.isEmpty()))
						{
							if (!PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), false, ite->IProfile, ite->UseEmbedded, ite->IRender, &tmpOut))
								return false;
							PutPage(tmpOut);
						}
						PutPage("Q\n");
						PutPage("Q\n");
						if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0)) && (!ite->isTableItem))
						{
							if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
								PutPage(PDF_TransparenzStroke(ite));
							if (ite->NamedLStyle.isEmpty())
							{
								if (!ite->strokePattern().isEmpty())
								{
									if (ite->patternStrokePath)
									{
										QPainterPath path = ite->PoLine.toQPainterPath(false);
										PutPage(HandleBrushPattern(ite, path, pag, pag->pageNr()));
									}
									else
									{
										if (!PDF_PatternFillStroke(tmpOut, ite, 1))
											return false;
										PutPage(SetClipPath(ite));
										PutPage(tmpOut);
										PutPage("h\nS\n");
									}
								}
								else if (ite->GrTypeStroke > 0)
								{
									if (!PDF_GradientFillStroke(tmpOut, ite, true))
										return false;
									PutPage("q\n");
									PutPage(tmpOut);
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
									PutPage("Q\n");
								}
								else
								{
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
								}
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage(SetClipPath(ite));
										PutPage("h\nS\n");
									}
								}
							}
						}
						break;
					case PageItem::TextFrame:
						break;
					case PageItem::Line:
						if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
							PutPage(PDF_TransparenzStroke(ite));
						if (ite->NamedLStyle.isEmpty())
						{
							if (!ite->strokePattern().isEmpty())
							{
								if (ite->patternStrokePath)
								{
									QPainterPath path;
									path.moveTo(0, 0);
									path.lineTo(ite->width(), 0);
									PutPage(HandleBrushPattern(ite, path, pag, pag->pageNr()));
								}
								else
								{
									if (!PDF_PatternFillStroke(tmpOut, ite, 1))
										return false;
									PutPage(tmpOut);
									PutPage("0 0 m\n");
									PutPage(FToStr(ite->width())+" 0 l\n");
									PutPage("S\n");
								}
							}
							else if (ite->GrTypeStroke > 0)
							{
								if (!PDF_GradientFillStroke(tmpOut, ite, true))
									return false;
								PutPage("q\n");
								PutPage(tmpOut);
								PutPage("0 0 m\n");
								PutPage(FToStr(ite->width())+" 0 l\n");
								PutPage("S\nQ\n");
							}
							else
							{
								PutPage("0 0 m\n");
								PutPage(FToStr(ite->width())+" 0 l\n");
								PutPage("S\n");
							}
						}
						else
						{
							multiLine ml = doc.MLineStyles[ite->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
									if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage("0 0 m\n");
										PutPage(FToStr(ite->width())+" 0 l\n");
										PutPage("S\n");
									}
							}
						}
						if (ite->startArrowIndex() != 0)
						{
							QTransform arrowTrans;
							arrowTrans.scale(-1,1);
							arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
							PutPage(drawArrow(ite, arrowTrans, ite->startArrowIndex()));
						}
						if (ite->endArrowIndex() != 0)
						{
							QTransform arrowTrans;
							arrowTrans.translate(ite->width(), 0);
							arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
							PutPage(drawArrow(ite, arrowTrans, ite->endArrowIndex()));
						}
						break;
					case PageItem::ItemType1:
					case PageItem::ItemType3:
					case PageItem::Polygon:
					case PageItem::RegularPolygon:
					case PageItem::Arc:
						PutPage("q\n");
						if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
						{
							PutPage(PDF_TransparenzFill(ite));
						}
						if (ite->GrType != 0)
						{
							if (ite->GrType == 8)
							{
								if (!PDF_PatternFillStroke(tmpOut, ite))
									return false;
							}
							else
							{
								if (!PDF_GradientFillStroke(tmpOut, ite))
									return false;
							}
							PutPage(tmpOut);
							PutPage(SetClipPath(ite));
							if (ite->fillRule)
								PutPage("h\nf*\n");
							else
								PutPage("h\nf\n");
						}
						else
						{
							if (ite->fillColor() != CommonStrings::None)
							{
								PutPage(SetClipPath(ite));
								if (ite->fillRule)
									PutPage("h\nf*\n");
								else
									PutPage("h\nf\n");
							}
						}
						PutPage("Q\n");
						if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
						{
							if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
								PutPage(PDF_TransparenzStroke(ite));
							if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
							{
								if (!ite->strokePattern().isEmpty())
								{
									if (ite->patternStrokePath)
									{
										QPainterPath path = ite->PoLine.toQPainterPath(false);
										PutPage(HandleBrushPattern(ite, path, pag, pag->pageNr()));
									}
									else
									{
										if (!PDF_PatternFillStroke(tmpOut, ite, 1))
											return false;
										PutPage(tmpOut);
										PutPage("h\nS\n");
									}
								}
								else if (ite->GrTypeStroke > 0)
								{
									if (!PDF_GradientFillStroke(tmpOut, ite, true))
										return false;
									PutPage("q\n");
									PutPage(tmpOut);
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
									PutPage("Q\n");
								}
								else
								{
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
								}
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage(SetClipPath(ite));
										PutPage("h\nS\n");
									}
								}
							}
						}
						break;
					case PageItem::PolyLine:
					case PageItem::Spiral:
						if (ite->PoLine.size() > 4) // && ((ite->PoLine.point(0) != ite->PoLine.point(1)) || (ite->PoLine.point(2) != ite->PoLine.point(3))))
						{
							PutPage("q\n");
							if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
							{
								PutPage(PDF_TransparenzFill(ite));
							}
							if (ite->GrType != 0)
							{
								if (ite->GrType == 8)
								{
									if (!PDF_PatternFillStroke(tmpOut, ite))
										return false;
								}
								else
								{
									if (!PDF_GradientFillStroke(tmpOut, ite))
										return false;
								}
								PutPage(tmpOut);
								PutPage(SetClipPath(ite));
								if (ite->fillRule)
									PutPage("h\nf*\n");
								else
									PutPage("h\nf\n");
							}
							else
							{
								if (ite->fillColor() != CommonStrings::None)
								{
									PutPage(SetClipPath(ite));
									if (ite->fillRule)
										PutPage("h\nf*\n");
									else
										PutPage("h\nf\n");
								}
							}
							PutPage("Q\n");
						}
						if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
						{
							if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
								PutPage(PDF_TransparenzStroke(ite));
							if (ite->NamedLStyle.isEmpty())
							{
								if (!ite->strokePattern().isEmpty())
								{
									if (ite->patternStrokePath)
									{
										QPainterPath path = ite->PoLine.toQPainterPath(false);
										PutPage(HandleBrushPattern(ite, path, pag, pag->pageNr()));
									}
									else
									{
										if (!PDF_PatternFillStroke(tmpOut, ite, 1))
											return false;
										PutPage(tmpOut);
										PutPage(SetClipPath(ite, false));
										PutPage("S\n");
									}
								}
								else if (ite->GrTypeStroke > 0)
								{
									if (!PDF_GradientFillStroke(tmpOut, ite, true))
										return false;
									PutPage("q\n");
									PutPage(tmpOut);
									PutPage(SetClipPath(ite, false));
									PutPage("S\n");
									PutPage("Q\n");
								}
								else
								{
									PutPage(SetClipPath(ite, false));
									PutPage("S\n");
								}
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage(SetClipPath(ite, false));
										PutPage("S\n");
									}
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
									QTransform arrowTrans;
									arrowTrans.translate(Start.x(), Start.y());
									arrowTrans.rotate(r);
									arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
									PutPage(drawArrow(ite, arrowTrans, ite->startArrowIndex()));
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
									QTransform arrowTrans;
									arrowTrans.translate(End.x(), End.y());
									arrowTrans.rotate(r);
									arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
									PutPage(drawArrow(ite, arrowTrans, ite->endArrowIndex()));
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
								if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
								{
									if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
										PutPage(PDF_TransparenzStroke(ite));
									if (ite->NamedLStyle.isEmpty())
									{
										if (!ite->strokePattern().isEmpty())
										{
											if (ite->patternStrokePath)
											{
												QPainterPath path = ite->PoLine.toQPainterPath(false);
												PutPage(HandleBrushPattern(ite, path, pag, pag->pageNr()));
											}
											else
											{
												if (!PDF_PatternFillStroke(tmpOut, ite, 1))
													return false;
												PutPage(tmpOut);
												PutPage(SetClipPath(ite, false));
												PutPage("S\n");
											}
										}
										else if (ite->GrTypeStroke > 0)
										{
											if (!PDF_GradientFillStroke(tmpOut, ite, true))
												return false;
											PutPage("q\n");
											PutPage(tmpOut);
											PutPage(SetClipPath(ite, false));
											PutPage("S\n");
											PutPage("Q\n");
										}
										else
										{
											PutPage(SetClipPath(ite, false));
											PutPage("S\n");
										}
									}
									else
									{
										multiLine ml = doc.MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
										{
											if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
											{
												PutPage(setStrokeMulti(&ml[it]));
												PutPage(SetClipPath(ite, false));
												PutPage("S\n");
											}
										}
									}
								}
								PutPage("Q\n");
							}
						}
						if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
						{
							if (ite->GrMask > 0)
								PutPage("q\n");
							PutPage(PDF_TransparenzFill(ite));
						}
						PutPage(setTextSt(ite, pag->pageNr(), pag));
						if (ite->GrMask > 0)
							PutPage("Q\n");
						break;
					case PageItem::OSGFrame:
						break;
					case PageItem::Symbol:
						if (doc.docPatterns.contains(ite->pattern()))
						{
							QString tmpD = "";
							ScPattern pat = doc.docPatterns[ite->pattern()];
							PutPage("q\n");
							PutPage(SetClipPath(ite));
							PutPage("h W* n\n");
							if (ite->imageFlippedH())
								PutPage("-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n");
							if (ite->imageFlippedV())
								PutPage("1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n");
							QTransform trans;
							trans.scale(ite->width() / pat.width, ite->height() / pat.height);
							trans.translate(0.0, -ite->height());
							trans.translate(pat.items.at(0)->gXpos, -pat.items.at(0)->gYpos);
							PutPage(FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n");
							for (int em = 0; em < pat.items.count(); ++em)
							{
								PageItem* embedded = pat.items.at(em);
								tmpD += "q\n";
								tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
								QString output;
								if (!PDF_ProcessItem(output, embedded, pag, pag->pageNr(), true))
									return "";
								tmpD += output;
								tmpD += "Q\n";
							}
							for (int em = 0; em < pat.items.count(); ++em)
							{
								PageItem* embedded = pat.items.at(em);
								if (!embedded->isTableItem)
									continue;
								if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
									continue;
								tmpD += "q\n";
								tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
								tmpD += PDF_ProcessTableItem(embedded, pag);
								tmpD += "Q\n";
							}
							if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
								PutPage(Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite));
							else
								PutPage(tmpD);
							PutPage("Q\n");
						}
						break;
					case PageItem::Group:
						if (ite->groupItemList.count() > 0)
						{
							QString tmpD = "";
							PutPage("q\n");
							PutPage(SetClipPath(ite));
							PutPage("h W* n\n");
							if (ite->imageFlippedH())
								PutPage("-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n");
							if (ite->imageFlippedV())
								PutPage("1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n");
							QTransform trans;
							trans.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
							trans.translate(0.0, -ite->height());
							trans.translate(ite->groupItemList.at(0)->gXpos, -ite->groupItemList.at(0)->gYpos);
							PutPage(FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n");
							for (int em = 0; em < ite->groupItemList.count(); ++em)
							{
								PageItem* embedded = ite->groupItemList.at(em);
								tmpD += "q\n";
								tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
								QString output;
								if (!PDF_ProcessItem(output, embedded, pag, pag->pageNr(), true))
									return "";
								tmpD += output;
								tmpD += "Q\n";
							}
							for (int em = 0; em < ite->groupItemList.count(); ++em)
							{
								PageItem* embedded = ite->groupItemList.at(em);
								if (!embedded->isTableItem)
									continue;
								if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
									continue;
								tmpD += "q\n";
								tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
								tmpD += PDF_ProcessTableItem(embedded, pag);
								tmpD += "Q\n";
							}
							if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
								PutPage(Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite));
							else
								PutPage(tmpD);
							PutPage("Q\n");
						}
						break;
					case PageItem::Multiple:
						Q_ASSERT(false);
						break;
				}
				PutPage("Q\n");
				uint templateObject = newObject();
				StartObj(templateObject);
				PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
				double bleedRight = 0.0;
				double bleedLeft  = 0.0;
				getBleeds(ActPageP, bleedLeft, bleedRight);
				double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
				double maxBoxY = ActPageP->height()+Options.bleeds.Top+Options.bleeds.Bottom;
				PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.Bottom)+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
//				PutDoc("/BBox [ 0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" ]\n");
				PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
				if (Seite.ImgObjects.count() != 0)
				{
					PutDoc("/XObject <<\n");
					QMap<QString,int>::Iterator it;
					for (it = Seite.ImgObjects.begin(); it != Seite.ImgObjects.end(); ++it)
						PutDoc("/"+it.key()+" "+QString::number(it.value())+" 0 R\n");
					PutDoc(">>\n");
				}
				if (Seite.FObjects.count() != 0)
				{
					PutDoc("/Font << \n");
					QMap<QString,int>::Iterator it2;
					for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
						PutDoc("/"+it2.key()+" "+QString::number(it2.value())+" 0 R\n");
					PutDoc(">>\n");
				}
				if (Shadings.count() != 0)
				{
					PutDoc("/Shading << \n");
					QMap<QString,int>::Iterator it3;
					for (it3 = Shadings.begin(); it3 != Shadings.end(); ++it3)
						PutDoc("/"+it3.key()+" "+QString::number(it3.value())+" 0 R\n");
					PutDoc(">>\n");
				}
				if (Patterns.count() != 0)
				{
					PutDoc("/Pattern << \n");
					QMap<QString,int>::Iterator it3p;
					for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
						PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
					PutDoc(">>\n");
				}
				if (Transpar.count() != 0)
				{
					PutDoc("/ExtGState << \n");
					QMap<QString,int>::Iterator it3t;
					for (it3t = Transpar.begin(); it3t != Transpar.end(); ++it3t)
						PutDoc("/"+it3t.key()+" "+QString::number(it3t.value())+" 0 R\n");
					PutDoc(">>\n");
				}
				if ((ICCProfiles.count() != 0) || (spotMap.count() != 0))
				{
					PutDoc("/ColorSpace << \n");
					QMap<QString,ICCD>::Iterator it3c;
					if (ICCProfiles.count() != 0)
					{
						for (it3c = ICCProfiles.begin(); it3c != ICCProfiles.end(); ++it3c)
							PutDoc("/"+it3c.value().ResName+" "+QString::number(it3c.value().ResNum)+" 0 R\n");
					}
					QMap<QString,SpotC>::Iterator it3sc;
					if (spotMap.count() != 0)
					{
						for (it3sc = spotMap.begin(); it3sc != spotMap.end(); ++it3sc)
							PutDoc("/"+it3sc.value().ResName+" "+QString::number(it3sc.value().ResNum)+" 0 R\n");
					}
					PutDoc(">>\n");
				}
				PutDoc(">>\n");
				if (Options.Compress)
					Content = CompressStr(&Content);
				PutDoc("/Length "+QString::number(Content.length()+1));
				if (Options.Compress)
					PutDoc("\n/Filter /FlateDecode");
				PutDoc(" >>\nstream\n"+EncStream(Content, templateObject)+"\nendstream\nendobj\n");
				int pIndex   = doc.MasterPages.indexOf((Page* const) pag) + 1;
				QString name = QString("master_page_obj_%1_%2").arg(pIndex).arg(ite->ItemNr);
				Seite.XObjects[name] = templateObject;
			}
			if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
				PutPage("EMC\n");
		}
		Lnr++;
	}
	return true;
}

void PDFLibCore::PDF_Begin_Page(const Page* pag, QPixmap pm)
{
	QString tmp;
	ActPageP = pag;
	Content = "";
	Seite.AObjects.clear();
	if (Options.Thumbnails)
	{
		ScImage img(pm.toImage());
		bool compDataAvail = false;
		QByteArray array = img.ImageToArray();
		if (Options.Compress)
		{
			QByteArray compArray = CompressArray(array);
			if (compArray.size() > 0)
			{
				array = compArray;
				compDataAvail = true;
			}
		}
		uint thumbnail = newObject();
		StartObj(thumbnail);
		PutDoc("<<\n/Width "+QString::number(img.width())+"\n");
		PutDoc("/Height "+QString::number(img.height())+"\n");
		PutDoc("/ColorSpace /DeviceRGB\n/BitsPerComponent 8\n");

		PutDoc("/Length "+QString::number(array.size()+1)+"\n");
		if (Options.Compress && compDataAvail)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n");
		EncodeArrayToStream(array, thumbnail);
		PutDoc("\nendstream\nendobj\n");
		Seite.Thumb = thumbnail;
	}
}

void PDFLibCore::PDF_End_Page(int physPage)
{
	uint PgNr =  ActPageP->pageNr();
	double markOffs = 0.0;
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
		markOffs = 20.0 + Options.markOffset;
	double bleedRight, bleedLeft;
	getBleeds(ActPageP, bleedLeft, bleedRight);
	double maxBoxX = ActPageP->width()+bleedLeft+bleedRight+markOffs*2.0;
	double maxBoxY = ActPageP->height()+Options.bleeds.Bottom+Options.bleeds.Top+markOffs*2.0;
	// (JG) Fix #5977 and #6075 (invalid restore)
	//PutPage("Q\n");
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
	{
		PutPage("0.5 w 0 j 0 J [] 0 d\n");
		PutPage("/"+spotMapReg["Register"].ResName+" CS 1 SCN\n");
		if (Options.cropMarks)
		{
		// Bottom Left
			PutPage("0 "+FToStr(markOffs+Options.bleeds.Bottom)+" m\n");
			PutPage(FToStr(20.0)+" "+FToStr(markOffs+Options.bleeds.Bottom)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs+bleedLeft)+" 0 m\n");
			PutPage(FToStr(markOffs+bleedLeft)+" 20 l\n");
			PutPage("S\n");
		// Top Left
			PutPage("0 "+FToStr(maxBoxY-Options.bleeds.Top-markOffs)+" m\n");
			PutPage(FToStr(20.0)+" "+FToStr(maxBoxY-Options.bleeds.Top-markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs+bleedLeft)+" "+FToStr(maxBoxY)+" m\n");
			PutPage(FToStr(markOffs+bleedLeft)+" "+FToStr(maxBoxY-20.0)+" l\n");
			PutPage("S\n");
		// Bottom Right
			PutPage(FToStr(maxBoxX)+" "+FToStr(markOffs+Options.bleeds.Bottom)+" m\n");
			PutPage(FToStr(maxBoxX-20.0)+" "+FToStr(markOffs+Options.bleeds.Bottom)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(0.0)+" m\n");
			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(20.0)+" l\n");
			PutPage("S\n");
		// Top Right
			PutPage(FToStr(maxBoxX)+" "+FToStr(maxBoxY-Options.bleeds.Top-markOffs)+" m\n");
			PutPage(FToStr(maxBoxX-20.0)+" "+FToStr(maxBoxY-Options.bleeds.Top-markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(maxBoxY)+" m\n");
			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(maxBoxY-20.0)+" l\n");
			PutPage("S\n");
		}
		if (Options.bleedMarks)
		{
			PutPage("q\n");
			PutPage("[3 1 1 1] 0 d\n");
		// Bottom Left
			PutPage("0 "+FToStr(markOffs)+" m\n");
			PutPage(FToStr(20.0)+" "+FToStr(markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs)+" 0 m\n");
			PutPage(FToStr(markOffs)+" 20 l\n");
			PutPage("S\n");
		// Top Left
			PutPage("0 "+FToStr(maxBoxY-markOffs)+" m\n");
			PutPage(FToStr(20.0)+" "+FToStr(maxBoxY-markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs)+" "+FToStr(maxBoxY)+" m\n");
			PutPage(FToStr(markOffs)+" "+FToStr(maxBoxY-20.0)+" l\n");
			PutPage("S\n");
		// Bottom Right
			PutPage(FToStr(maxBoxX)+" "+FToStr(markOffs)+" m\n");
			PutPage(FToStr(maxBoxX-20.0)+" "+FToStr(markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(0.0)+" m\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(20.0)+" l\n");
			PutPage("S\n");
		// Top Right
			PutPage(FToStr(maxBoxX)+" "+FToStr(maxBoxY-markOffs)+" m\n");
			PutPage(FToStr(maxBoxX-20.0)+" "+FToStr(maxBoxY-markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(maxBoxY)+" m\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(maxBoxY-20.0)+" l\n");
			PutPage("S\n");
			PutPage("Q\n");
		}
		if (Options.registrationMarks)
		{
			QString regCross = "0 7 m\n14 7 l\nh\n7 0 m\n7 14 l\nh\n13 7 m\n13 10.31383 10.31383 13 7 13 c\n3.68629 13 1 10.31383 1 7 c\n1 3.68629 3.68629 1 7 1 c\n";
			regCross += "10.31383 1 13 3.68629 13 7 c\nh\n10.5 7 m\n10.5 8.93307 8.93307 10.5 7 10.5 c\n5.067 10.5 3.5 8.93307 3.5 7 c\n";
			regCross += "3.5 5.067 5.067 3.5 7 3.5 c\n8.93307 3.5 10.5 5.067 10.5 7 c\nh\nS\n";
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX / 2.0 - 7.0)+" 3 cm\n");
			PutPage(regCross);
			PutPage("Q\n");
			PutPage("q\n");
			PutPage("1 0 0 1 3 "+FToStr(maxBoxY / 2.0 - 7.0)+" cm\n");
			PutPage(regCross);
			PutPage("Q\n");
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX / 2.0 - 7.0)+" "+FToStr(maxBoxY - 17.0)+" cm\n");
			PutPage(regCross);
			PutPage("Q\n");
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX - 17.0)+" "+FToStr(maxBoxY / 2.0 - 7.0)+" cm\n");
			PutPage(regCross);
			PutPage("Q\n");
		}
		if (Options.colorMarks)
		{
			double startX = markOffs+bleedLeft+6.0;
			double startY = maxBoxY - 18.0;
			PutPage("0 0 0 1 K\n");
			double col = 1.0;
			for (int bl = 0; bl < 11; bl++)
			{
				PutPage("0 0 0 "+FToStr(col)+" k\n");
				PutPage(FToStr(startX+bl*14.0)+" "+FToStr(startY)+" 14 14 re B\n");
				col -= 0.1;
			}
			if (!Options.isGrayscale)
			{
				startX = maxBoxX-bleedRight-markOffs-20.0;
				PutPage("0 0 0 0.5 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0 0.5 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0.5 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0.5 0 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("1 1 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("1 0 1 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 1 1 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0 0 1 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0 1 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 1 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("1 0 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
			}
		}
		if (Options.docInfoMarks)
		{
//			QString tmp = "";
//			double startX = markOffs+bleedLeft+10.0;
			FPointArray  textPath;
			QPainterPath painter1, painter2;
			QFont   infoFont("Helvetica", 7);
			double  startX = markOffs+bleedLeft+10.0;
			QString docTitle = doc.documentInfo().title();
			if (docTitle.isEmpty())
			{
				QFileInfo fi(doc.DocName);
				docTitle = fi.fileName();
			}
//			docTitle += "  "+ tr("Page:")+" "+tmp.setNum(PgNr+1);
			docTitle += "  "+ tr("Page:")+" "+ QString::number(PgNr+1);
			PutPage("/"+spotMapReg["Register"].ResName+" cs 1 scn\n");
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(startX)+" 6 cm\n");
//			PutPage("BT\n");
//			PutPage("/"+StdFonts["/Helvetica"]+" 7 Tf\n");
//			PutPage(EncString("("+docTitle+")",ObjCounter)+" Tj\nET\n");
			painter1.addText( QPointF(0.0,0.0), infoFont, docTitle );
			textPath.fromQPainterPath(painter1);
			PutPage(SetClipPathArray(&textPath, true));
			PutPage("h\nf*\n");
			PutPage("Q\n");
			QDate d = QDate::currentDate();
			QString docDate = tr("Date:")+" "+d.toString(Qt::TextDate);
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX / 2.0 + 20.0)+" 6 cm\n");
//			PutPage("BT\n");
//			PutPage("/"+StdFonts["/Helvetica"]+" 7 Tf\n");
//			QDate d = QDate::currentDate();
//			PutPage(EncString("("+ tr("Date:")+" "+d.toString(Qt::TextDate)+")",ObjCounter)+" Tj\nET\n");
			painter2.addText( QPointF(0.0,0.0), infoFont, docDate );
			textPath.fromQPainterPath(painter2);
			PutPage(SetClipPathArray(&textPath, true));
			PutPage("h\nf*\n");
			PutPage("Q\n");
		}
	}
	Seite.ObjNum = WritePDFStream(Content);
	int Gobj = 0;
	if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4))
	{
		Gobj = newObject();
		StartObj(Gobj);
		PutDoc("<< /S /Transparency\n");
		if (Options.UseRGB)
			PutDoc("/CS /DeviceRGB\n");
		else
		{
			if (Options.isGrayscale)
				PutDoc("/CS /DeviceGray\n");
			else
			{
				if ((doc.HasCMS) && (Options.UseProfiles))
					PutDoc("/CS "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
				else
					PutDoc("/CS /DeviceCMYK\n");
			}
		}
		PutDoc(">>\nendobj\n");
	}
	uint pageObject = newObject();
	StartObj(pageObject);
	PutDoc("<<\n/Type /Page\n/Parent 4 0 R\n");
	PutDoc("/MediaBox [0 0 "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+"]\n");
	PutDoc("/BleedBox ["+FToStr(markOffs)+" "+FToStr(markOffs)+" "+FToStr(maxBoxX-markOffs)+" "+FToStr(maxBoxY-markOffs)+"]\n");
	PutDoc("/CropBox [0 0 "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+"]\n");
	PutDoc("/TrimBox ["+FToStr(bleedLeft+markOffs)+" "+FToStr(Options.bleeds.Bottom+markOffs)+" "+FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(maxBoxY-Options.bleeds.Top-markOffs)+"]\n");
	if (Options.Version >= PDFOptions::PDFVersion_13) // PDF/X forbids having both art and trim box!
		PutDoc("/ArtBox ["+FToStr(bleedLeft+markOffs)+" "+FToStr(Options.bleeds.Bottom+markOffs)+" "+FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(maxBoxY-Options.bleeds.Top-markOffs)+"]\n");
	PutDoc("/Rotate "+QString::number(Options.RotateDeg)+"\n");
	PutDoc("/Contents "+QString::number(Seite.ObjNum)+" 0 R\n");
	if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) // && (Transpar.count() != 0))
		PutDoc("/Group "+QString::number(Gobj)+" 0 R\n");
	if (Options.Thumbnails)
		PutDoc("/Thumb "+QString::number(Seite.Thumb)+" 0 R\n");
	if (Seite.AObjects.count() != 0)
	{
		PutDoc("/Annots [ ");
		for (int b = 0; b < Seite.AObjects.count(); ++b)
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
				case 7:
					PutDoc("/S /Push\n");
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
				case 8:
					PutDoc("/S /Cover\n");
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
				case 9:
					PutDoc("/S /Uncover\n");
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
				case 10:
					PutDoc("/S /Fade\n");
					break;
			}
			PutDoc(">>\n");
		}
	}
	PutDoc(">>\nendobj\n");
	PageTree.Count++;
	PageTree.Kids[physPage] = pageObject;
}


void PDFLibCore::writeXObject(uint objNr, QString dictionary, QByteArray stream)
{
	StartObj(objNr);
	PutDoc("<<");
	PutDoc(dictionary);
	PutDoc(">>\nstream\n");
	EncodeArrayToStream(stream, objNr);
	PutDoc("\nendstream\nendobj\n");
}


uint PDFLibCore::writeObject(QString type, QString dictionary)
{
	uint result = newObject();
	StartObj(result);
	PutDoc("<<");
	if (!type.isEmpty())
		PutDoc("/Type " + type + "\n");
	PutDoc(dictionary);
	PutDoc(">>\nendobj\n");
	return result;
}


bool PDFLibCore::PDF_ProcessPage(const Page* pag, uint PNr, bool clip)
{
	ActPageP = pag;
	ScLayer ll;
	ll.isPrintable = false;
	if (Options.UseLPI)
		PutPage("/"+HTName+" gs\n");
	double bleedRight = 0.0;
	double bleedLeft = 0.0;
	double markOffs = 0.0;
	bleedDisplacementX = 0.0;
	bleedDisplacementY = 0.0;
	PutPage("q\n"); // Save
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
		markOffs = 20.0 + Options.markOffset;
	// #8773 - incorrect page position if MPageNam.isEmpty()
	/*if (!pag->MPageNam.isEmpty())
	{*/
		getBleeds(ActPageP, bleedLeft, bleedRight);
		PutPage("1 0 0 1 "+FToStr(bleedLeft+markOffs)+" "+FToStr(Options.bleeds.Bottom+markOffs)+" cm\n");
		bleedDisplacementX = bleedLeft+markOffs;
		bleedDisplacementY = Options.bleeds.Bottom+markOffs;
	/*}*/
	if ( (Options.MirrorH) && (!pag->MPageNam.isEmpty()) )
		PutPage("-1 0 0 1 "+FToStr(ActPageP->width())+" 0 cm\n");
	if ( (Options.MirrorV) && (!pag->MPageNam.isEmpty()) )
		PutPage("1 0 0 -1 0 "+FToStr(ActPageP->height())+" cm\n");
	if (clip)
	{
		double maxBoxX = ActPageP->width() - ActPageP->Margins.Right - ActPageP->Margins.Left;
		double maxBoxY = ActPageP->height() - ActPageP->Margins.Top - ActPageP->Margins.Bottom;
		PutPage(FToStr(ActPageP->Margins.Left)+" "+FToStr(ActPageP->Margins.Bottom)+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" re W n\n");
	//	PutPage("0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" re W n\n");
	}
	//CB *2 because the Pitems count loop runs twice.. y.. dunno.
	if (usingGUI && pag->pageName().isEmpty())
		progressDialog->setProgress("ECPI", 0, doc.DocItems.count()*2);
	for (int lam = 0; lam < doc.Layers.count() && !abortExport; ++lam)
	{
		ll.isPrintable = false;
		doc.Layers.levelToLayer(ll, lam);
		if (!PDF_ProcessMasterElements(ll, pag, PNr))
			return false;
		if (!PDF_ProcessPageElements(ll, pag, PNr))
			return false;
	}
	PutPage("Q\n"); // Restore
	return true;
}

bool PDFLibCore::PDF_ProcessMasterElements(const ScLayer& layer, const Page* pag, uint PNr)
{
	PageItem* ite;
	QString output;
	QList<PageItem*> PItems;

	if (pag->MPageNam.isEmpty())
		return true;
	if (doc.MasterItems.count() <= 0)
		return true;
	const Page* mPage = doc.MasterPages.at(doc.MasterNames[doc.DocPages.at(PNr)->MPageNam]);
	int   mPageIndex  = doc.MasterPages.indexOf((Page* const) mPage) + 1;

	if (!Options.MirrorH)
		PutPage("1 0 0 1 0 0 cm\n");
	if ((layer.isPrintable) || (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers)))
	{
		if ((((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4))) && (Options.useLayers))
			PutPage("/OC /"+OCGEntries[layer.Name].Name+" BDC\n");
		for (int am = 0; am < pag->FromMaster.count() && !abortExport; ++am)
		{
			ite = pag->FromMaster.at(am);
			if (usingGUI)
				qApp->processEvents();
			if ((ite->LayerID != layer.ID) || (!ite->printEnabled()))
				continue;
			if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
				continue;
			QString name = QString("/master_page_obj_%1_%2").arg(mPageIndex).arg(ite->ItemNr);
			if (! ite->asTextFrame())
				PutPage(name+" Do\n");
			else
			{
				double oldX = ite->xPos();
				double oldY = ite->yPos();
				double OldBX = ite->BoundingX;
				double OldBY = ite->BoundingY;
				ite->setXPos(ite->xPos() - mPage->xOffset() + pag->xOffset(), true);
				ite->setYPos(ite->yPos() - mPage->yOffset() + pag->yOffset(), true);
				if (!PDF_ProcessItem(output, ite, pag, pag->pageNr()))
					return false;
				PutPage(output);
				ite->setXYPos(oldX, oldY, true);
				ite->BoundingX = OldBX;
				ite->BoundingY = OldBY;
			}
		}
		for (int am = 0; am < pag->FromMaster.count(); ++am)
		{
			ite = pag->FromMaster.at(am);
			if ((ite->LayerID != layer.ID) || (!ite->printEnabled()))
				continue;
			if (ite->ChangedMasterItem)
				continue;
			if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
				continue;
			if (!ite->isTableItem)
				continue;
			double oldX = ite->xPos();
			double oldY = ite->yPos();
			double OldBX = ite->BoundingX;
			double OldBY = ite->BoundingY;
			ite->setXPos(ite->xPos() - mPage->xOffset() + pag->xOffset(), true);
			ite->setYPos(ite->yPos() - mPage->yOffset() + pag->yOffset(), true);
			PutPage(PDF_ProcessTableItem(ite, pag));
			ite->setXYPos(oldX, oldY, true);
			ite->BoundingX = OldBX;
			ite->BoundingY = OldBY;
		}
		if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
			PutPage("EMC\n");
	}
	return true;
}

bool PDFLibCore::PDF_ProcessPageElements(const ScLayer& layer, const Page* pag, uint PNr)
{
	PageItem* ite;
	QString output;
	QList<PageItem*> PItems;

	int pc_exportpagesitems = usingGUI ? progressDialog->progress("ECPI") : 0;
	PItems = (pag->pageName().isEmpty()) ? doc.DocItems : doc.MasterItems;
	if ((layer.isPrintable) || (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers)))
	{
		QString inh = "";
		if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
			PutPage("/OC /"+OCGEntries[layer.Name].Name+" BDC\n");
		for (int a = 0; a < PItems.count() && !abortExport; ++a)
		{
			ite = PItems.at(a);
			if (ite->LayerID != layer.ID)
				continue;
			if (usingGUI)
			{
				progressDialog->setProgress("ECPI", ++pc_exportpagesitems);
				qApp->processEvents();
			}
			if (!PDF_ProcessItem(output, ite, pag, PNr))
				return false;
			if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				inh += output;
			else
				PutPage(output);
		}
		for (int a = 0; a < PItems.count() && !abortExport; ++a)
		{
			ite = PItems.at(a);
			if (ite->LayerID != layer.ID)
				continue;
			if (usingGUI)
			{
				progressDialog->setProgress("ECPI", ++pc_exportpagesitems);
				qApp->processEvents();
			}
			if (!ite->isTableItem)
				continue;
			double bLeft, bRight, bBottom, bTop;
			getBleeds(ActPageP, bLeft, bRight, bBottom, bTop);
			double x  = pag->xOffset() - bLeft;
			double y  = pag->yOffset() - bTop;
			double w  = pag->width()  + bLeft + bRight;
			double h1 = pag->height() + bBottom + bTop;
			double ilw= ite->lineWidth();
			double x2 = ite->BoundingX - ilw / 2.0;
			double y2 = ite->BoundingY - ilw / 2.0;
			double w2 = qMax(ite->BoundingW + ilw, 1.0);
			double h2 = qMax(ite->BoundingH + ilw, 1.0);
			if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )))
				continue;
			if (ite->ChangedMasterItem)
				continue;
			if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
				continue;
			if (!ite->printEnabled())
				continue;
			if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				inh += PDF_ProcessTableItem(ite, pag);
			else
				PutPage(PDF_ProcessTableItem(ite, pag));
		}
		if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) ||(Options.Version == PDFOptions::PDFVersion_X4)))
		{
			int Gobj = newObject();
			StartObj(Gobj);
			PutDoc("<< /Type /Group\n");
			PutDoc("/S /Transparency\n");
			PutDoc("/I false\n");
			PutDoc("/K false\n");
			PutDoc(">>\nendobj\n");
			QString ShName = ResNam+QString::number(ResCount);
			ResCount++;
			Transpar[ShName] = writeGState("/CA "+FToStr(layer.transparency)+"\n"
										   + "/ca "+FToStr(layer.transparency)+"\n"
										   + "/SMask /None\n/AIS false\n/OPM 1\n"
										   + "/BM /" + blendMode(layer.blendMode) + "\n");
			uint formObject = newObject();
			StartObj(formObject);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
			double bleedRight = 0.0;
			double bleedLeft  = 0.0;
			getBleeds(ActPageP, bleedLeft, bleedRight);
			double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
			double maxBoxY = ActPageP->height()+Options.bleeds.Top+Options.bleeds.Bottom;
			PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.Bottom)+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
			PutDoc("/Group "+QString::number(Gobj)+" 0 R\n");
			if (Options.Compress)
				inh = CompressStr(&inh);
			PutDoc("/Length "+QString::number(inh.length()+1));
			if (Options.Compress)
				PutDoc("\n/Filter /FlateDecode");
			PutDoc(" >>\nstream\n"+EncStream(inh, formObject)+"\nendstream\nendobj\n");
			QString name = layer.Name.simplified().replace(QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_") + QString::number(layer.ID) + QString::number(PNr);
			Seite.XObjects[name] = formObject;
			PutPage("q\n");
			PutPage("/"+ShName+" gs\n");
			PutPage("/"+name+" Do\n");
			PutPage("Q\n");
		}
		if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
			PutPage("EMC\n");
	}
	return true;
}

QString PDFLibCore::Write_TransparencyGroup(double trans, int blend, QString &data, PageItem *controlItem)
{
	QString ShName = "";
	QString retString = "";
	int Gobj = newObject();
	StartObj(Gobj);
	PutDoc("<< /Type /Group\n");
	PutDoc("/S /Transparency\n");
	PutDoc("/I false\n");
	PutDoc("/K false\n");
	PutDoc(">>\nendobj\n");
	if (controlItem != NULL)
	{
		retString += "q\n";
		retString += PDF_TransparenzFill(controlItem);
	}
	else
	{
		ShName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/CA "+FToStr(1.0 - trans)+"\n"
									+ "/ca "+FToStr(1.0 - trans)+"\n"
									+ "/SMask /None\n/AIS false\n/OPM 1\n"
									+ "/BM /" + blendMode(blend) + "\n");
		retString += "q\n";
		retString += "/"+ShName+" gs\n";
	}
	uint formObject = newObject();
	StartObj(formObject);
	PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
	double bleedRight = 0.0;
	double bleedLeft  = 0.0;
	getBleeds(ActPageP, bleedLeft, bleedRight);
	double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
	double maxBoxY = ActPageP->height()+Options.bleeds.Top+Options.bleeds.Bottom;
	if (controlItem != NULL)
	{
		double scaleW, scaleH;
		if (controlItem->isGroup())
		{
			if (controlItem->groupWidth > controlItem->width())
				scaleW = controlItem->groupWidth / controlItem->width();
			else
				scaleW = 1.0 / (controlItem->groupWidth / controlItem->width());
			if (controlItem->groupHeight > controlItem->height())
				scaleH = controlItem->groupHeight / controlItem->height();
			else
				scaleH = 1.0 / (controlItem->groupHeight / controlItem->height());
			PutDoc("/BBox [ "+FToStr(0)+" "+FToStr(-controlItem->height() * scaleH)+" "+FToStr(controlItem->groupWidth * scaleW)+" "+FToStr(controlItem->groupHeight * scaleH)+" ]\n");
		}
		if (controlItem->isSymbol())
		{
			ScPattern pat = doc.docPatterns[controlItem->pattern()];
			if (pat.width > controlItem->width())
				scaleW = pat.width / controlItem->width();
			else
				scaleW = 1.0 / (pat.width / controlItem->width());
			if (pat.height > controlItem->height())
				scaleH = pat.height / controlItem->height();
			else
				scaleH = 1.0 / (pat.height / controlItem->height());
			PutDoc("/BBox [ "+FToStr(0)+" "+FToStr(-controlItem->height() * scaleH)+" "+FToStr(pat.width * scaleW)+" "+FToStr(pat.height * scaleH)+" ]\n");
		}
	}
	else
		PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.Bottom)+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
	PutDoc("/Group "+QString::number(Gobj)+" 0 R\n");
	if (Options.Compress)
		data = CompressStr(&data);
	PutDoc("/Length "+QString::number(data.length()+1));
	if (Options.Compress)
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n"+EncStream(data, formObject)+"\nendstream\nendobj\n");
	QString name = ResNam+QString::number(ResCount);
	ResCount++;
	Seite.XObjects[name] = formObject;
	retString += "/"+name+" Do\n";
	retString += "Q\n";
	return retString;
}

QString PDFLibCore::PDF_ProcessTableItem(PageItem* ite, const Page* pag)
{
	if ((ite->lineColor() == CommonStrings::None) || (ite->lineWidth() == 0.0))
		return "";
	QString tmp("");
	tmp += "q\n";
	if ((ite->doOverprint) && (!Options.UseRGB))
	{
		QString ShName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/OP true\n"
									   "/op true\n"
									   "/OPM 1\n");
		tmp += "/"+ShName+" gs\n";
	}
//	if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= PDFOptions::PDFVersion_14))
//		tmp += PDF_Transparenz(ite);
//	if (ite->fillColor() != CommonStrings::None)
//		tmp += putColor(ite->fillColor(), ite->fillShade(), true);
	if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
		tmp += PDF_TransparenzStroke(ite);
	if (ite->lineColor() != CommonStrings::None)
		tmp += putColor(ite->lineColor(), ite->lineShade(), false);
	tmp += FToStr(fabs(ite->lineWidth()))+" w\n";
	if (ite->DashValues.count() != 0)
	{
		tmp += "[ ";
		QVector<double>::iterator it;
		for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
		{
			int da = static_cast<int>(*it);
			// #8758: Custom dotted lines don't export properly to pdf
			// Null values have to be exported if line end != flat
			if ((da != 0) || (ite->lineEnd() != Qt::FlatCap))
				tmp += QString::number(da)+" ";
		}
		tmp += "] "+QString::number(static_cast<int>(ite->DashOffset))+" d\n";
	}
	else
		tmp += "["+getDashString(ite->PLineArt, ite->lineWidth())+"] 0 d\n";
	tmp += "2 J\n";
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
	tmp += "1 0 0 1 "+FToStr(ite->xPos() - pag->xOffset())+" "+FToStr(pag->height() - (ite->yPos()  - pag->yOffset()))+" cm\n";
	if (ite->rotation() != 0)
	{
		double sr = sin(-ite->rotation()* M_PI / 180.0);
		double cr = cos(-ite->rotation()* M_PI / 180.0);
		if ((cr * cr) < 0.000001)
			cr = 0;
		if ((sr * sr) < 0.000001)
			sr = 0;
		tmp += FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+ " 0 0 cm\n";
	}
	if ((ite->TopLine) || (ite->RightLine) || (ite->BottomLine) || (ite->LeftLine))
	{
		if (ite->TopLine)
		{
			tmp += "0 0 m\n";
			tmp += FToStr(ite->width())+" 0 l\n";
		}
		if (ite->RightLine)
		{
			tmp += FToStr(ite->width())+" 0 m\n";
			tmp += FToStr(ite->width())+" "+FToStr(-ite->height())+" l\n";
		}
		if (ite->BottomLine)
		{
			tmp += "0 "+FToStr(-ite->height())+" m\n";
			tmp += FToStr(ite->width())+" "+FToStr(-ite->height())+" l\n";
		}
		if (ite->LeftLine)
		{
			tmp += "0 0 m\n";
			tmp += "0 "+FToStr(-ite->height())+" l\n";
		}
		tmp += "S\n";
	}
	tmp += "Q\n";
	return tmp;
}

bool PDFLibCore::PDF_ProcessItem(QString& output, PageItem* ite, const Page* pag, uint PNr, bool embedded, bool pattern)
{
	QString tmp(""), tmpOut;
	if (ite->isGroup())
		ite->asGroupFrame()->adjustXYPosition();
	ite->setRedrawBounding();
	double bLeft, bRight, bBottom, bTop;
	getBleeds(pag, bLeft, bRight, bBottom, bTop);
	double x  = pag->xOffset() - bLeft;
	double y  = pag->yOffset() - bTop;
	double w  = pag->width() + bLeft + bRight;
	double h1 = pag->height()+ bBottom + bTop;
	double ilw=ite->lineWidth();
	double x2 = ite->BoundingX - ilw / 2.0;
	double y2 = ite->BoundingY - ilw / 2.0;
	double w2 = qMax(ite->BoundingW + ilw, 1.0);
	double h2 = qMax(ite->BoundingH + ilw, 1.0);
	output.resize(0);
	if (!pattern)
	{
//		qDebug() << QString("pdflib process item: pagename=%1 ownpage=%2 pagenr=%3 changedMP=%4").arg(pag->pageName()).arg(ite->OwnPage).arg(pag->pageNr()).arg(ite->ChangedMasterItem);
//		qDebug() << QString("pdflib process item: x=%1 x2=%2 y=%3 y2=%4 w=%5 w2=%6 h1=%7 h2=%8 ilw=%9").arg(x).arg(x2).arg(y).arg(y2).arg(w).arg(w2).arg(h1).arg(h2).arg(ilw);
		if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )) && !embedded)
		{
			output = tmp;
			return true;
		}
//		qDebug() << "bb test done";
		if (ite->ChangedMasterItem)
		{
			output = tmp;
			return true;
		}
		if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
		{
			output = tmp;
			return true;
		}
	}

	tmp += "q\n";
	if ((ite->doOverprint) && (!Options.UseRGB))
	{
		QString ShName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/OP true\n"
									   "/op true\n"
									   "/OPM 1\n");
		tmp += "/"+ShName+" gs\n";
	}
//	if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= PDFOptions::PDFVersion_14))
//		tmp += PDF_Transparenz(ite);
	if ((ite->isBookmark) && (Options.Bookmarks))
		PDF_Bookmark(ite, pag->height() - (ite->yPos() - pag->yOffset()));
	if (!pattern)
	{
		if (!ite->printEnabled() || ((ite->itemType() == PageItem::TextFrame) && (!pag->pageName().isEmpty())))
		{
//			qDebug() << "Q exit";
			tmp += "Q\n";
			output = tmp;
			return true;
		}
	}
	if (ite->fillColor() != CommonStrings::None)
		tmp += putColor(ite->fillColor(), ite->fillShade(), true);
	if (ite->lineColor() != CommonStrings::None)
		tmp += putColor(ite->lineColor(), ite->lineShade(), false);
	tmp += FToStr(fabs(ite->lineWidth()))+" w\n";
	if (ite->DashValues.count() != 0)
	{
		tmp += "[ ";
		QVector<double>::iterator it;
		for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
		{
			int da = static_cast<int>(*it);
			// #8758: Custom dotted lines don't export properly to pdf
			// Null values have to be exported if line end != flat
			if ((da != 0) || (ite->lineEnd() != Qt::FlatCap))
				tmp += QString::number(da)+" ";
		}
		tmp += "] "+QString::number(static_cast<int>(ite->DashOffset))+" d\n";
	}
	else
		tmp += "["+getDashString(ite->PLineArt, ite->lineWidth())+"] 0 d\n";
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
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
#ifdef HAVE_OSG
			if (ite->asOSGFrame())
			{
				if (Options.Version != PDFOptions::PDFVersion_X3)
				{
					if (!PDF_3DAnnotation(ite, PNr))
						return false;
				}
				break;
			}
#endif
			tmp += "q\n";
			// Same functions as for ImageFrames work for LatexFrames too
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
			{
				tmp += PDF_TransparenzFill(ite);
			}
			if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
			{
				if (ite->GrType != 0)
				{
					if (ite->GrType == 8)
					{
						if (!PDF_PatternFillStroke(tmpOut, ite))
							return false;
					}
					else
					{
						if (!PDF_GradientFillStroke(tmpOut, ite))
							return false;
					}
					tmp += tmpOut;
					tmp += SetClipPath(ite);
					if (ite->fillRule)
						tmp += "h\nf*\n";
					else
						tmp += "h\nf\n";
				}
				else
				{
					tmp += SetClipPath(ite);
					if (ite->fillRule)
						tmp += "h\nf*\n";
					else
						tmp += "h\nf\n";
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
			if ((ite->PictureIsAvailable) && (!ite->Pfile.isEmpty()))
			{
				if (!PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), false, ite->IProfile, ite->UseEmbedded, ite->IRender, &tmpOut))
					return false;
				tmp += tmpOut;
			}
			tmp += "Q\n";
			tmp += "Q\n";
			if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0)) && (!ite->isTableItem))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					if (!ite->strokePattern().isEmpty())
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += HandleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "h\nS\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite);
							tmp += "h\nS\n";
						}
					}
				}
			}
			break;
		case PageItem::TextFrame:
			if ((ite->isAnnotation()) && (Options.Version != PDFOptions::PDFVersion_X3) && (Options.Version != PDFOptions::PDFVersion_X1a) && (Options.Version != PDFOptions::PDFVersion_X4))
			{
				if (!PDF_Annotation(ite, PNr))
					return false;
				break;
			}
			tmp += "q\n";
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
			{
				tmp += PDF_TransparenzFill(ite);
			}
			if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
			{
				if (ite->GrType != 0)
				{
					if (ite->GrType == 8)
					{
						if (!PDF_PatternFillStroke(tmpOut, ite))
							return false;
					}
					else
					{
						if (!PDF_GradientFillStroke(tmpOut, ite))
							return false;
					}
					tmp += tmpOut;
					tmp += SetClipPath(ite);
					if (ite->fillRule)
						tmp += "h\nf*\n";
					else
						tmp += "h\nf\n";
				}
				else
				{
					tmp += SetClipPath(ite);
					if (ite->fillRule)
						tmp += "h\nf*\n";
					else
						tmp += "h\nf\n";
				}
			}
			tmp += "q\n";
			if (ite->imageFlippedH())
				tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
			if (ite->imageFlippedV())
				tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
			tmp += setTextSt(ite, PNr, pag);
			tmp += "Q\n";
			tmp += "Q\n";
			if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0)) && (!ite->isTableItem))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					if (!ite->strokePattern().isEmpty())
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += HandleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "h\nS\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite);
							tmp += "h\nS\n";
						}
					}
				}
			}
			break;
		case PageItem::Line:
			if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				tmp += PDF_TransparenzStroke(ite);
			if (ite->NamedLStyle.isEmpty())
			{
				if (!ite->strokePattern().isEmpty())
				{
					if (ite->patternStrokePath)
					{
						QPainterPath path;
						path.moveTo(0, 0);
						path.lineTo(ite->width(), 0);
						tmp += HandleBrushPattern(ite, path, pag, PNr);
					}
					else
					{
						if (!PDF_PatternFillStroke(tmpOut, ite, 1))
							return false;
						tmp += tmpOut;
						tmp += "0 0 m\n";
						tmp += FToStr(ite->width())+" 0 l\n";
						tmp += "S\n";
					}
				}
				else if (ite->GrTypeStroke > 0)
				{
					if (!PDF_GradientFillStroke(tmpOut, ite, true))
						return false;
					tmp += "q\n";
					tmp += tmpOut;
					tmp += "0 0 m\n";
					tmp += FToStr(ite->width())+" 0 l\n";
					tmp += "S\n";
					tmp += "Q\n";
				}
				else if (ite->lineColor() != CommonStrings::None)
				{
					tmp += "0 0 m\n";
					tmp += FToStr(ite->width())+" 0 l\n";
					tmp += "S\n";
				}
			}
			else
			{
				multiLine ml = doc.MLineStyles[ite->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
					{
						tmp += setStrokeMulti(&ml[it]);
						tmp += "0 0 m\n";
						tmp += FToStr(ite->width())+" 0 l\n";
						tmp += "S\n";
					}
				}
			}
			if (ite->startArrowIndex() != 0)
			{
				QTransform arrowTrans;
				arrowTrans.scale(-1,1);
				arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
				tmp += drawArrow(ite, arrowTrans, ite->startArrowIndex());
			}
			if (ite->endArrowIndex() != 0)
			{
				QTransform arrowTrans;
				arrowTrans.translate(ite->width(), 0);
				arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
				tmp += drawArrow(ite, arrowTrans, ite->endArrowIndex());
			}
			break;
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
			tmp += "q\n";
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
			{
				tmp += PDF_TransparenzFill(ite);
			}
			if (ite->GrType != 0)
			{
				if (ite->GrType == 8)
				{
					if (!PDF_PatternFillStroke(tmpOut, ite))
						return false;
				}
				else
				{
					if (!PDF_GradientFillStroke(tmpOut, ite))
						return false;
				}
				tmp += tmpOut;
				tmp += SetClipPath(ite);
				if (ite->fillRule)
					tmp += "h\nf*\n";
				else
					tmp += "h\nf\n";
			}
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
			tmp += "Q\n";
			if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					if (!ite->strokePattern().isEmpty())
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += HandleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "h\nS\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite);
							tmp += "h\nS\n";
						}
					}
				}
			}
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			if (ite->PoLine.size() > 4)  // && ((ite->PoLine.point(0) != ite->PoLine.point(1)) || (ite->PoLine.point(2) != ite->PoLine.point(3))))
			{
				tmp += "q\n";
				if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				{
					tmp += PDF_TransparenzFill(ite);
				}
				if (ite->GrType != 0)
				{
					if (ite->GrType == 8)
					{
						if (!PDF_PatternFillStroke(tmpOut, ite))
							return false;
					}
					else
					{
						if (!PDF_GradientFillStroke(tmpOut, ite))
							return false;
					}
					tmp += tmpOut;
					tmp += SetClipPath(ite);
					if (ite->fillRule)
						tmp += "h\nf*\n";
					else
						tmp += "h\nf\n";
				}
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
				tmp += "Q\n";
			}
			if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					if (!ite->strokePattern().isEmpty())
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += HandleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite, false);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "S\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite, false);
						tmp += "S\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite, false);
							tmp += "S\n";
						}
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
						QTransform arrowTrans;
						arrowTrans.translate(Start.x(), Start.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
						tmp += drawArrow(ite, arrowTrans, ite->startArrowIndex());
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
						QTransform arrowTrans;
						arrowTrans.translate(End.x(), End.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
						tmp += drawArrow(ite, arrowTrans, ite->endArrowIndex());
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
					if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
					{
						if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
							tmp += PDF_TransparenzStroke(ite);
						if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
						{
							if (!ite->strokePattern().isEmpty())
							{
								if (ite->patternStrokePath)
								{
									QPainterPath path = ite->PoLine.toQPainterPath(false);
									tmp += HandleBrushPattern(ite, path, pag, PNr);
								}
								else
								{
									tmp += SetClipPath(ite, false);
									if (!PDF_PatternFillStroke(tmpOut, ite, 1))
										return false;
									tmp += tmpOut;
									tmp += "S\n";
								}
							}
							else if (ite->GrTypeStroke > 0)
							{
								if (!PDF_GradientFillStroke(tmpOut, ite, true))
									return false;
								tmp += "q\n";
								tmp += tmpOut;
								tmp += SetClipPath(ite, false);
								tmp += "S\n";
								tmp += "Q\n";
							}
							else if (ite->lineColor() != CommonStrings::None)
							{
								tmp += SetClipPath(ite, false);
								tmp += "S\n";
							}
						}
						else
						{
							multiLine ml = doc.MLineStyles[ite->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
								{
									tmp += setStrokeMulti(&ml[it]);
									tmp += SetClipPath(ite, false);
									tmp += "S\n";
								}
							}
						}
					}
					tmp += "Q\n";
				}
			}
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
			{
				if (ite->GrMask > 0)
					tmp += "q\n";
				tmp += PDF_TransparenzFill(ite);
			}
			tmp += setTextSt(ite, PNr, pag);
			if (ite->GrMask > 0)
				tmp += "Q\n";
			break;
		case PageItem::Symbol:
			if (doc.docPatterns.contains(ite->pattern()))
			{
				QString tmpD = "";
				ScPattern pat = doc.docPatterns[ite->pattern()];
				tmp += "q\n";
				tmp += SetClipPath(ite);
				tmp += "h W* n\n";
				if (ite->imageFlippedH())
					tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
				if (ite->imageFlippedV())
					tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
				QTransform trans;
				trans.scale(ite->width() / pat.width, ite->height() / pat.height);
				trans.translate(0.0, -ite->height());
			//	trans.translate(pat.items.at(0)->gXpos, -pat.items.at(0)->gYpos);
				tmp += FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n";
				for (int em = 0; em < pat.items.count(); ++em)
				{
					PageItem* embedded = pat.items.at(em);
					tmpD += "q\n";
					tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
					QString output;
					if (!PDF_ProcessItem(output, embedded, pag, PNr, true))
						return "";
					tmpD += output;
					tmpD += "Q\n";
				}
				for (int em = 0; em < pat.items.count(); ++em)
				{
					PageItem* embedded = pat.items.at(em);
					if (!embedded->isTableItem)
						continue;
					if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
						continue;
					tmpD += "q\n";
					tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
					tmpD += PDF_ProcessTableItem(embedded, pag);
					tmpD += "Q\n";
				}
				if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
					tmp += Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite);
				else
					tmp += tmpD;
				tmp += "Q\n";
			}
			break;
		case PageItem::Group:
			if (ite->groupItemList.count() > 0)
			{
				QString tmpD = "";
				tmp += "q\n";
				tmp += SetClipPath(ite);
				tmp += "h W* n\n";
				if (ite->imageFlippedH())
					tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
				if (ite->imageFlippedV())
					tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
				QTransform trans;
				trans.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
				trans.translate(0.0, -ite->height());
				tmp += FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n";
				groupStackPos.push(QPointF(ite->xPos(), ite->height()));
				for (int em = 0; em < ite->groupItemList.count(); ++em)
				{
					PageItem* embedded = ite->groupItemList.at(em);
					tmpD += "q\n";
					tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
					QString output;
					if (!PDF_ProcessItem(output, embedded, pag, PNr, true))
						return "";
					tmpD += output;
					tmpD += "Q\n";
				}
				for (int em = 0; em < ite->groupItemList.count(); ++em)
				{
					PageItem* embedded = ite->groupItemList.at(em);
					if (!embedded->isTableItem)
						continue;
					if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
						continue;
					tmpD += "q\n";
					tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
					tmpD += PDF_ProcessTableItem(embedded, pag);
					tmpD += "Q\n";
				}
				groupStackPos.pop();
				if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
					tmp += Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite);
				else
					tmp += tmpD;
				tmp += "Q\n";
			}
			break;
		case PageItem::Multiple:
			Q_ASSERT(false);
			break;
	}
	tmp += "Q\n";
	output = tmp;
	return true;
}

QString PDFLibCore::HandleBrushPattern(PageItem* ite, QPainterPath &path, const Page* pag, uint PNr)
{
	QString tmp;
	tmp = "";
	ScPattern pat = doc.docPatterns[ite->strokePattern()];
	double pLen = path.length() - ((pat.width / 2.0) * (ite->patternStrokeScaleX / 100.0));
	double adv = pat.width * ite->patternStrokeScaleX / 100.0 * ite->patternStrokeSpace;
	double xpos = ite->patternStrokeOffsetX * ite->patternStrokeScaleX / 100.0;
	while (xpos < pLen)
	{
		double currPerc = path.percentAtLength(xpos);
		double currAngle = path.angleAtPercent(currPerc);
#if QT_VERSION  >= 0x040400
		if (currAngle <= 180.0)
			currAngle *= -1.0;
		else
			currAngle = 360.0 - currAngle;
#endif
		QPointF currPoint = path.pointAtPercent(currPerc);
		tmp += "q\n";
		QTransform base;
		base.translate(currPoint.x(), -currPoint.y());
		base.rotate(-currAngle);
		tmp += FToStr(base.m11())+" "+FToStr(base.m12())+" "+FToStr(base.m21())+" "+FToStr(base.m22())+" "+FToStr(base.dx())+" "+FToStr(base.dy())+" cm\n";
		QTransform trans;
		trans.translate(0.0, -ite->patternStrokeOffsetY);
		trans.rotate(-ite->patternStrokeRotation);
		trans.shear(ite->patternStrokeSkewX, -ite->patternStrokeSkewY);
		trans.scale(ite->patternStrokeScaleX / 100.0, ite->patternStrokeScaleY / 100.0);
		trans.translate(-pat.width / 2.0, -pat.height / 2.0);
		if (ite->patternStrokeMirrorX)
		{
			trans.translate(pat.width, 0);
			trans.scale(-1, 1);
		}
		if (ite->patternStrokeMirrorY)
		{
			trans.translate(0, pat.height);
			trans.scale(1, -1);
		}
		tmp += FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n";
		for (int em = 0; em < pat.items.count(); ++em)
		{
			PageItem* embedded = pat.items.at(em);
			tmp += "q\n";
			tmp +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(embedded->gHeight - embedded->gYpos)+" cm\n";
			QString output;
			if (!PDF_ProcessItem(output, embedded, pag, PNr, true))
				return "";
			tmp += output;
			tmp += "Q\n";
		}
		for (int em = 0; em < pat.items.count(); ++em)
		{
			PageItem* embedded = pat.items.at(em);
			if (!embedded->isTableItem)
				continue;
			if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
				continue;
			tmp += "q\n";
			tmp +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(embedded->gHeight - embedded->gYpos)+" cm\n";
			tmp += PDF_ProcessTableItem(embedded, pag);
			tmp += "Q\n";
		}
		tmp += "Q\n";
		xpos += adv;
	}
	return tmp;
}

QString PDFLibCore::drawArrow(PageItem *ite, QTransform &arrowTrans, int arrowIndex)
{
	QString tmp = "";
	FPointArray arrow = doc.arrowStyles().at(arrowIndex-1).points.copy();
	if (ite->NamedLStyle.isEmpty())
	{
		if (ite->lineWidth() != 0.0)
			arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
	}
	else
	{
		multiLine ml = doc.MLineStyles[ite->NamedLStyle];
		if (ml[ml.size()-1].Width != 0.0)
			arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
	}
	arrow.map(arrowTrans);
	if ((ite->lineTransparency() != 0) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
	{
		QString ShName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n"
									   + "/ca "+FToStr(1.0 - ite->lineTransparency())+"\n"
									   + "/SMask /None\n/AIS false\n/OPM 1\n"
									   + "/BM /Normal\n");
		tmp += "/"+ShName+" gs\n";
	}
	if (ite->NamedLStyle.isEmpty())
	{
		if (!ite->strokePattern().isEmpty())
		{
			tmp += SetClipPathArray(&arrow);
			QString tmpOut;
			PDF_PatternFillStroke(tmpOut, ite, 1, true);
			tmp += tmpOut;
			tmp += "h\nf*\n";
		}
		else if (ite->GrTypeStroke > 0)
		{
			tmp += SetClipPathArray(&arrow);
			QString tmpOut;
			PDF_GradientFillStroke(tmpOut, ite, true, true);
			tmp += "q\n";
			tmp += tmpOut;
			tmp += "h\nf*\nQ\n";
		}
		else if (ite->lineColor() != CommonStrings::None)
		{
			tmp += putColor(ite->lineColor(), ite->lineShade(), true);
			tmp += SetClipPathArray(&arrow);
			tmp += "h\nf*\n";
		}
	}
	else
	{
		multiLine ml = doc.MLineStyles[ite->NamedLStyle];
		if (ml[0].Color != CommonStrings::None)
		{
			tmp += putColor(ml[0].Color, ml[0].Shade, true);
			tmp += SetClipPathArray(&arrow);
			tmp += "h\nf*\n";
		}
		for (int it = ml.size()-1; it > 0; it--)
		{
			if (ml[it].Color != CommonStrings::None)
			{
				tmp += setStrokeMulti(&ml[it]);
				tmp += SetClipPathArray(&arrow);
				tmp += "h\nS\n";
			}
		}
	}
	return tmp;
}

QString PDFLibCore::putColor(const QString& color, double shade, bool fill)
{
	QString tmp = "";
	QString colString = SetColor(color, shade);
	ScColor tmpC;
	tmpC = doc.PageColors[color];
	if (((tmpC.isSpotColor()) || (tmpC.isRegistrationColor())) && ((Options.isGrayscale == false) && (Options.UseRGB == false))  && (Options.UseSpotColors))
	{
		if ((color != CommonStrings::None) && (spotMap.contains(color)))
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
		if ((doc.HasCMS) && (Options.UseProfiles))
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
			if (color != CommonStrings::None)
			{
				if (fill)
					tmp += colString+" k\n";
				else
					tmp += colString+" K\n";
			}
		}
	}
	return tmp;
}

/*CB 2982: cache code is borked somehow, original function is above
QString PDFLibCore::putColor(const QString & colorName, int shade, bool fill)
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

QString PDFLibCore::putColorUncached(const QString& color, int shade, bool fill)
{
	ScColor tmpC(doc.PageColors[color]);
	if (((tmpC.isSpotColor()) || (tmpC.isRegistrationColor())) && ((Options.isGrayscale == false) && (Options.UseRGB == false))  && (Options.UseSpotColors))
	{
		QString tmpSpot("");
		if ((color != CommonStrings::None) && (spotMap.contains(color)))
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
	QString colString(SetColor(color, shade));
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
		if ((doc.HasCMS) && (Options.UseProfiles))
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
			if (color != CommonStrings::None)
			{
				if (fill)
					tmp += colString+" k\n";
				else
					tmp += colString+" K\n";
			}
		}
	}
	return tmp;
}

QString PDFLibCore::setStrokeMulti(struct SingleLine *sl)
{
	QString tmp(
			putColor(sl->Color, sl->Shade, false) +
			FToStr(sl->Width)+" w\n"
			);
	QString Dt = FToStr(qMax(1*sl->Width, 1.0));
	QString Sp = FToStr(qMax(2*sl->Width, 1.0));
	QString Da = FToStr(qMax(4*sl->Width, 1.0));
	switch (static_cast<Qt::PenStyle>(sl->Dash))
	{
		case Qt::SolidLine:
			tmp += "[] 0 d\n";
			break;
		case Qt::DashLine:
			tmp += "["+Da+" "+Sp+"] 0 d\n";
			break;
		case Qt::DotLine:
			tmp += "["+Dt+" "+Sp+"] 0 d\n";
			break;
		case Qt::DashDotLine:
			tmp += "["+Da+" "+Sp+" "+Dt+" "+Sp+"] 0 d\n";
			break;
		case Qt::DashDotDotLine:
			tmp += "["+Da+" "+Sp+" "+Dt+" "+Sp+" "+Dt+" "+Sp+"] 0 d\n";
			break;
		default:
			tmp += "[] 0 d\n";
			break;
		}
	switch (static_cast<Qt::PenCapStyle>(sl->LineEnd))
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
	switch (static_cast<Qt::PenJoinStyle>(sl->LineJoin))
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
QString PDFLibCore::setTextSt(PageItem *ite, uint PNr, const Page* pag)
{
	int tabCc = 0;
	int savedOwnPage = ite->OwnPage;
	double tabDist = ite->textToFrameDistLeft();
	double colLeft = 0.0;
	QString tmp(""), tmp2("");
	QList<ParagraphStyle::TabRecord> tTabValues;
	ite->OwnPage = PNr;
	ite->layout();
	ite->OwnPage = savedOwnPage;
	if (ite->lineColor() != CommonStrings::None)
		tabDist += ite->lineWidth() / 2.0;
#ifndef NLS_PROTO
	// Loop over each character (!) in the pageItem...
	if (ite->itemType() == PageItem::TextFrame)
	{
		tmp += "BT\n";
		for (uint ll=0; ll < ite->itemText.lines(); ++ll)
		{
			LineSpec ls = ite->itemText.line(ll);
			colLeft = ls.colLeft;
			tabDist = ls.x;
			double CurX = ls.x;
			for (int d = ls.firstItem; d <= ls.lastItem; ++d)
			{
				const ScText * const hl = ite->itemText.item(d);
				const QChar ch = hl->ch;
				const CharStyle& chstyle(ite->itemText.charStyle(d));
				const ParagraphStyle& pstyle(ite->itemText.paragraphStyle(d));
				if ((ch == SpecialChars::PARSEP) || (ch == QChar(10)) || (ch == SpecialChars::LINEBREAK) || (ch == SpecialChars::FRAMEBREAK) || (ch == SpecialChars::COLBREAK))
					continue;
				if (chstyle.effects() & ScStyle_SuppressSpace)
					continue;
				tTabValues = pstyle.tabValues();
				if (chstyle.effects() & ScStyle_StartOfLine)
					tabCc = 0;
				if ((ch == SpecialChars::TAB) && (tTabValues.count() != 0))
				{
					QChar tabFillChar;
					const TabLayout* tabLayout = dynamic_cast<const TabLayout*>(hl->glyph.more);
					if (tabLayout)
						tabFillChar = tabLayout->fillChar;
					if (!tabFillChar.isNull())
					{
						ScText hl2;
						static_cast<CharStyle&>(hl2) = static_cast<const CharStyle&>(*hl);
						const GlyphLayout * const gl = hl->glyph.more;
						double scale = gl ? gl->scaleV : 1.0;
						double wt    = chstyle.font().charWidth(tabFillChar, chstyle.fontSize() * scale / 10.0);
						double len   = hl->glyph.xadvance;
						int coun     = static_cast<int>(len / wt);
						// #6728 : update code according to fillInTabLeaders() and PageItem::layout() - JG
						double sPos  = 0.0 /*CurX - len + chstyle.fontSize() / 10.0 * 0.7 + 1*/;
						hl2.ch = tTabValues[tabCc].tabFillChar;
						hl2.setTracking(0);
						hl2.setScaleH(1000);
						hl2.setScaleV(1000);
						hl2.glyph.glyph   = chstyle.font().char2CMap(tabFillChar);
						hl2.glyph.yoffset = hl->glyph.yoffset;
						for (int cx = 0; cx < coun; ++cx)
						{
							hl2.glyph.xoffset =  sPos + wt * cx;
							if ((chstyle.effects() & ScStyle_Shadowed) && (chstyle.strokeColor() != CommonStrings::None))
							{
								ScText hl3;
								static_cast<CharStyle&>(hl3) = static_cast<const CharStyle&>(hl2);
								//<< HACK to fix Bug #8446
								hl3.setEffects(hl3.effects() & (~(ScStyle_Outline)));
								//>>
								hl3.ch = hl2.ch;
								hl3.glyph.glyph = hl2.glyph.glyph;
								hl3.setFillColor(hl2.strokeColor());
								hl3.setFillShade(hl2.strokeShade());
								hl3.glyph.yoffset = hl2.glyph.yoffset - (chstyle.fontSize() * chstyle.shadowYOffset() / 10000.0);
								hl3.glyph.xoffset = hl2.glyph.xoffset + (chstyle.fontSize() * chstyle.shadowXOffset() / 10000.0);
								setTextCh(ite, PNr, CurX, ls.y, d, tmp, tmp2, &hl3, pstyle, pag);
							}
							setTextCh(ite, PNr, CurX, ls.y, d, tmp, tmp2, &hl2, pstyle, pag);
						}
					}
					tabCc++;
				}
				if (ch == SpecialChars::TAB)
				{
					CurX += hl->glyph.wide();
					continue;
				}
				if ((chstyle.effects() & ScStyle_Shadowed) && (chstyle.strokeColor() != CommonStrings::None))
				{
					ScText hl2;
					hl2.ch = ch;
					hl2.glyph.glyph = hl->glyph.glyph;
					const GlyphLayout *gl1 = &hl->glyph;
					GlyphLayout	*gl2 = &hl2.glyph;
					while (gl1->more)
					{
						gl2->more = new GlyphLayout(*gl1->more);
						gl2->more->yoffset -= (chstyle.fontSize() * chstyle.shadowYOffset() / 10000.0);
						gl2->more->xoffset += (chstyle.fontSize() * chstyle.shadowXOffset() / 10000.0);
						gl2->more->more = NULL;
						gl1 = gl1->more;
						gl2 = gl2->more;
					}
					static_cast<CharStyle&>(hl2) = static_cast<const CharStyle&>(*hl);
					//<< HACK to fix Bug #8446
					hl2.setEffects(hl2.effects() & (~(ScStyle_Outline)));
					//>>
					hl2.setFillColor(hl->strokeColor());
					hl2.setFillShade(hl->strokeShade());
					hl2.glyph.xadvance = hl->glyph.xadvance;
					hl2.glyph.yadvance = hl->glyph.yadvance;
					hl2.glyph.yoffset = hl->glyph.yoffset - (chstyle.fontSize() * chstyle.shadowYOffset() / 10000.0);
					hl2.glyph.xoffset = hl->glyph.xoffset + (chstyle.fontSize() * chstyle.shadowXOffset() / 10000.0);
					hl2.glyph.scaleH = hl->glyph.scaleH;
					hl2.glyph.scaleV = hl->glyph.scaleV;
					setTextCh(ite, PNr, CurX, ls.y, d, tmp, tmp2, &hl2, pstyle, pag);
				}
				setTextCh(ite, PNr, CurX, ls.y, d, tmp, tmp2, hl, pstyle, pag);
				// Unneeded now that glyph xadvance is set appropriately for inline objects by PageItem_TextFrame::layout() - JG
				/*if (hl->ch == SpecialChars::OBJECT)
				{
					InlineFrame& embedded(const_cast<InlineFrame&>(hl->embedded));
					CurX += (embedded.getItem()->gWidth + embedded.getItem()->lineWidth()) * hl->glyph.scaleH;
				}
				else*/
				CurX += hl->glyph.wide();
				tabDist = CurX;
			}
		}
	}
	else
	{
		double CurX = 0;
		for (int d = ite->firstInFrame(); d <= ite->lastInFrame(); ++d)
		{
			const ScText * const hl = ite->itemText.item(d);
			const QChar ch = hl->ch;
			const CharStyle& chstyle(ite->itemText.charStyle(d));
			const ParagraphStyle& pstyle(ite->itemText.paragraphStyle(d));
			if ((ch == SpecialChars::PARSEP) || (ch == QChar(10)) || (ch == SpecialChars::LINEBREAK) || (ch == SpecialChars::FRAMEBREAK) || (ch == SpecialChars::COLBREAK))
				continue;
			if (chstyle.effects() & ScStyle_SuppressSpace)
				continue;
			tTabValues = pstyle.tabValues();
			if (chstyle.effects() & ScStyle_StartOfLine)
				tabCc = 0;
			if ((ch == SpecialChars::TAB) && (tTabValues.count() != 0))
			{
				if ((tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
				{
					ScText hl2;
					static_cast<CharStyle&>(hl2) = static_cast<const CharStyle&>(*hl);
					double wt = chstyle.font().charWidth(tTabValues[tabCc].tabFillChar, chstyle.fontSize());
					int coun = static_cast<int>((CurX+hl->glyph.xoffset - tabDist) / wt);
					// #6728 : update code according to fillInTabLeaders() and PageItem::layout() - JG
					double sPos = 0.0 /* CurX+hl->glyph.xoffset - (CurX+hl->glyph.xoffset - tabDist) + 1 */;
					hl2.ch = tTabValues[tabCc].tabFillChar;
					hl2.setTracking(0);
					hl2.setScaleH(1000);
					hl2.setScaleV(1000);
					hl2.glyph.yoffset = hl->glyph.yoffset;
					for (int cx = 0; cx < coun; ++cx)
					{
						hl2.glyph.xoffset =  sPos + wt * cx;
						if ((chstyle.effects() & ScStyle_Shadowed) && (chstyle.strokeColor() != CommonStrings::None))
						{
							ScText hl3;
							static_cast<CharStyle&>(hl3) = static_cast<const CharStyle&>(hl2);
							//<< HACK to fix Bug #8446
							hl3.setEffects(hl3.effects() & (~(ScStyle_Outline)));
							//>>
							hl3.ch = hl2.ch;
							hl3.glyph.glyph = hl2.glyph.glyph;
							hl3.setFillColor(hl2.strokeColor());
							hl3.setFillShade(hl2.strokeShade());
							hl3.glyph.yoffset = hl2.glyph.yoffset - (chstyle.fontSize() * chstyle.shadowYOffset() / 10000.0);
							hl3.glyph.xoffset = hl2.glyph.xoffset + (chstyle.fontSize() * chstyle.shadowXOffset() / 10000.0);
							setTextCh(ite, PNr, 0, 0, d, tmp, tmp2, &hl3, pstyle, pag);
						}
						setTextCh(ite, PNr, 0, 0, d, tmp, tmp2, &hl2, pstyle, pag);
					}
					tabCc++;
				}
				else
				{
					tabCc++;
				}
			}
			if (ch == SpecialChars::TAB)
			{
				CurX += hl->glyph.wide();
				continue;
			}
			if ((chstyle.effects() & ScStyle_Shadowed) && (chstyle.strokeColor() != CommonStrings::None))
			{
				ScText hl2;
				hl2.ch = ch;
				hl2.glyph.glyph = hl->glyph.glyph;
				static_cast<CharStyle&>(hl2) = static_cast<const CharStyle&>(*hl);
				//<< HACK to fix Bug #8446
				hl2.setEffects(hl2.effects() & (~(ScStyle_Outline)));
				//>>
				hl2.setFillColor(hl->strokeColor());
				hl2.setFillShade(hl->strokeShade());
				hl2.glyph.yoffset = hl->glyph.yoffset - (chstyle.fontSize() * chstyle.shadowYOffset() / 10000.0);
				hl2.glyph.xoffset = hl->glyph.xoffset + (chstyle.fontSize() * chstyle.shadowXOffset() / 10000.0);
				hl2.glyph.scaleH = hl->glyph.scaleH;
				hl2.glyph.scaleV = hl->glyph.scaleV;
				hl2.PtransX = hl->PtransX;
				hl2.PtransY = hl->PtransY;
				hl2.PRot = hl->PRot;
				hl2.PDx = hl->PDx;
				setTextCh(ite, PNr, 0, 0, d, tmp, tmp2, &hl2, pstyle, pag);
			}
			setTextCh(ite, PNr, 0, 0, d, tmp, tmp2, hl, pstyle, pag);
			CurX += hl->glyph.wide();
			tabDist = CurX;
		}
	}
#endif
	if (ite->itemType() == PageItem::TextFrame)
		tmp += "ET\n"+tmp2;
	return tmp;
}

bool PDFLibCore::setTextCh(PageItem *ite, uint PNr, double x,  double y, uint d, QString &tmp, QString &tmp2, const ScText *hl, const ParagraphStyle& pstyle, const Page* pag)
{
#ifndef NLS_PROTO
	QString output;
	QString FillColor = "";
	QString StrokeColor = "";
	if (ite->asPathText())
	{
		tmp += "q\n";
		QPointF tangt = QPointF( cos(hl->PRot), sin(hl->PRot) );
		QTransform trafo = QTransform( 1, 0, 0, -1, -hl->PDx, 0 );
		if (ite->textPathFlipped)
			trafo *= QTransform(1, 0, 0, -1, 0, 0);
		if (ite->textPathType == 0)
			trafo *= QTransform( tangt.x(), -tangt.y(), -tangt.y(), -tangt.x(), hl->PtransX, -hl->PtransY );
		else if (ite->textPathType == 1)
			trafo *= QTransform(1, 0, 0, -1, hl->PtransX, -hl->PtransY );
		else if (ite->textPathType == 2)
		{
			double a = 1;
			double b = -1;
			if (tangt.x() < 0)
			{
				a = -1;
				b = 1;
			}
			if (fabs(tangt.x()) > 0.1)
				trafo *= QTransform( a, (tangt.y() / tangt.x()) * b, 0, -1, hl->PtransX, -hl->PtransY ); // ID's Skew mode
			else
				trafo *= QTransform( a, 6 * b, 0, -1, hl->PtransX, -hl->PtransY );
		}
		tmp += FToStr(trafo.m11())+" "+FToStr(trafo.m12())+" "+FToStr(trafo.m21())+" "+FToStr(trafo.m22())+" "+FToStr(trafo.dx())+" "+FToStr(trafo.dy())+" cm\n";
		if (ite->BaseOffs != 0)
			tmp += "1 0 0 1 0 "+ FToStr( -ite->BaseOffs)+" cm\n";
		if (hl->glyph.xoffset != 0.0 || hl->glyph.yoffset != 0.0)
			tmp += "1 0 0 1 " + FToStr( hl->glyph.xoffset)+ " " + FToStr( -hl->glyph.yoffset)+" cm\n";
		if (hl->ch != SpecialChars::OBJECT)
			tmp += "BT\n";
	}
	double tsz = hl->fontSize();
	QChar chstr = hl->ch;
	const CharStyle& style(*hl);

/*	if (hl->effects() & ScStyle_DropCap)
	{
		if (pstyle.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
			tsz = qRound(10 * ((doc.typographicSettings.valueBaseGrid * (pstyle.dropCapLines()-1)+(hl->font().ascent(pstyle.charStyle().fontSize() / 10.0))) / (hl->font().realCharHeight(chstr, 10))));
		else
		{
			if (pstyle.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
				tsz = qRound(10 * ((pstyle.lineSpacing() *  (pstyle.dropCapLines()-1)+(hl->font().ascent(pstyle.charStyle().fontSize() / 10.0))) / (hl->font().realCharHeight(chstr, 10))));
			else
			{
				double currasce = hl->font().height(pstyle.charStyle().fontSize());
				tsz = qRound(10 * ((currasce * (pstyle.dropCapLines()-1)+(hl->font().ascent(pstyle.charStyle().fontSize() / 10.0))) / hl->font().realCharHeight(chstr, 10)));
			}
		}
	}
	*/
	InlineFrame& embedded(const_cast<InlineFrame&>(hl->embedded));
	if ((hl->ch == SpecialChars::OBJECT) && (embedded.hasItem()))
	{
		if (!ite->asPathText())
		{
			tmp += "ET\n"+tmp2;
			tmp2 = "";
		}
		QList<PageItem*> emG = embedded.getGroupedItems();
		for (int em = 0; em < emG.count(); ++em)
		{
			PageItem* embedded = emG.at(em);
			tmp2 += "q\n";
			if (ite->asPathText())
				tmp2 +=  FToStr(style.scaleH() / 1000.0)+" 0 0 "+FToStr(style.scaleV() / 1000.0)+" "+FToStr(embedded->gXpos * (style.scaleH() / 1000.0))+" "+FToStr((embedded->gHeight * (style.scaleV() / 1000.0)) - embedded->gYpos * (style.scaleV() / 1000.0)+embedded->gHeight * (style.baselineOffset() / 1000.0))+" cm\n";
			else
				tmp2 +=  FToStr(style.scaleH() / 1000.0)+" 0 0 "+FToStr(style.scaleV() / 1000.0)+" "+FToStr(x+hl->glyph.xoffset + embedded->gXpos * (style.scaleH() / 1000.0))+" "+FToStr(-y-hl->glyph.yoffset + (embedded->gHeight * (style.scaleV() / 1000.0)) - embedded->gYpos * (style.scaleV() / 1000.0)+embedded->gHeight * (style.baselineOffset() / 1000.0))+" cm\n";
			if (!PDF_ProcessItem(output, embedded, pag, PNr, true))
				return false;
			tmp2 +=output;
			tmp2 += "Q\n";
		}
		for (int em = 0; em < emG.count(); ++em)
		{
			PageItem* embedded = emG.at(em);
			if (!embedded->isTableItem)
				continue;
			if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
				continue;
			tmp2 += "q\n";
			if (ite->asPathText())
				tmp2 +=  FToStr(style.scaleH() / 1000.0)+" 0 0 "+FToStr(style.scaleV() / 1000.0)+" "+FToStr(embedded->gXpos * (style.scaleH() / 1000.0))+" "+FToStr((embedded->gHeight * (style.scaleV() / 1000.0)) - embedded->gYpos * (style.scaleV() / 1000.0)+embedded->gHeight * (style.baselineOffset() / 1000.0))+" cm\n";
			else
				tmp2 +=  FToStr(style.scaleH() / 1000.0)+" 0 0 "+FToStr(style.scaleV() / 1000.0)+" "+FToStr(x+hl->glyph.xoffset + embedded->gXpos * (style.scaleH() / 1000.0))+" "+FToStr(-y-hl->glyph.yoffset + (embedded->gHeight * (style.scaleV() / 1000.0)) - embedded->gYpos * (style.scaleV() / 1000.0)+embedded->gHeight * (style.baselineOffset() / 1000.0))+" cm\n";

			if ((embedded->doOverprint) && (!Options.UseRGB))
			{
				QString ShName = ResNam+QString::number(ResCount);
				ResCount++;
				Transpar[ShName] = writeGState("/OP true\n"
											"/op true\n"
											"/OPM 1\n");
				tmp2 += "/"+ShName+" gs\n";
			}
			if (((embedded->lineTransparency() != 0) || (embedded->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				tmp2 += PDF_TransparenzStroke(embedded);
			if (embedded->lineColor() != CommonStrings::None)
				tmp2 += putColor(embedded->lineColor(), embedded->lineShade(), false);
			tmp2 += FToStr(fabs(embedded->lineWidth()))+" w\n";
			if (embedded->DashValues.count() != 0)
			{
				tmp2 += "[ ";
				QVector<double>::iterator it;
				for ( it = embedded->DashValues.begin(); it != embedded->DashValues.end(); ++it )
				{
					int da = static_cast<int>(*it);
					// #8758: Custom dotted lines don't export properly to pdf
					// Null values have to be exported if line end != flat
					if ((da != 0) || (embedded->lineEnd() != Qt::FlatCap))
						tmp2 += QString::number(da)+" ";
				}
				tmp2 += "] "+QString::number(static_cast<int>(embedded->DashOffset))+" d\n";
			}
			else
				tmp2 += "["+getDashString(embedded->PLineArt, embedded->lineWidth())+"] 0 d\n";
			tmp2 += "2 J\n";
			switch (embedded->PLineJoin)
			{
				case Qt::MiterJoin:
					tmp2 += "0 j\n";
					break;
				case Qt::BevelJoin:
					tmp2 += "2 j\n";
					break;
				case Qt::RoundJoin:
					tmp2 += "1 j\n";
					break;
				default:
					tmp2 += "0 j\n";
					break;
			}
			if ((embedded->TopLine) || (embedded->RightLine) || (embedded->BottomLine) || (embedded->LeftLine))
			{
				if (embedded->TopLine)
				{
					tmp2 += "0 0 m\n";
					tmp2 += FToStr(embedded->width())+" 0 l\n";
				}
				if (embedded->RightLine)
				{
					tmp2 += FToStr(embedded->width())+" 0 m\n";
					tmp2 += FToStr(embedded->width())+" "+FToStr(-embedded->height())+" l\n";
				}
				if (embedded->BottomLine)
				{
					tmp2 += "0 "+FToStr(-embedded->height())+" m\n";
					tmp2 += FToStr(embedded->width())+" "+FToStr(-embedded->height())+" l\n";
				}
				if (embedded->LeftLine)
				{
					tmp2 += "0 0 m\n";
					tmp2 += "0 "+FToStr(-embedded->height())+" l\n";
				}
				tmp2 += "S\n";
			}
			tmp2 += "Q\n";
		}
		tmp += tmp2+"\n";
		tmp2 = "";
		if (ite->asPathText())
			tmp += "Q\n";
		else
			tmp += "BT\n";
		return true;
	}

	uint glyph = hl->glyph.glyph;

	if (glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBSPACE.unicode()) ||
		glyph == (ScFace::CONTROL_GLYPHS + 32))
	{
		glyph = style.font().char2CMap(QChar(' '));
		chstr = ' ';
	}
	else if (glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBHYPHEN.unicode()))
	{
		glyph = style.font().char2CMap(QChar('-'));
		chstr = '-';
	}

	if (glyph < ScFace::CONTROL_GLYPHS)
	{
		if (style.strokeColor() != CommonStrings::None)
		{
			StrokeColor = "";
			StrokeColor += putColor(style.strokeColor(), style.strokeShade(), false);
		}
		if (style.fillColor() != CommonStrings::None)
		{
			FillColor = "";
			FillColor += putColor(style.fillColor(), style.fillShade(), true);
		}
		if (((style.effects() & ScStyle_Underline) && (chstr != SpecialChars::PARSEP))  || ((style.effects() & ScStyle_UnderlineWords) && (!chstr.isSpace())))
		{
			//		double Ulen = style.font().charWidth(chstr, style.fontSize()) * (hl->glyph.scaleH);
			double Ulen = hl->glyph.xadvance;
			double Upos, Uwid, kern;
			if (style.effects() & ScStyle_StartOfLine)
				kern = 0;
			else
				kern = style.fontSize() * style.tracking() / 10000.0;
			if ((style.underlineOffset() != -1) || (style.underlineWidth() != -1))
			{
				if (style.underlineOffset() != -1)
					Upos = (style.underlineOffset() / 1000.0) * (style.font().descent(style.fontSize() / 10.0));
				else
					Upos = style.font().underlinePos(style.fontSize() / 10.0);
				if (style.underlineWidth() != -1)
					Uwid = (style.underlineWidth() / 1000.0) * (style.fontSize() / 10.0);
				else
					Uwid = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			else
			{
				Upos = style.font().underlinePos(style.fontSize() / 10.0);
				Uwid = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			if (style.baselineOffset() != 0)
				Upos += (style.fontSize() / 10.0) * hl->glyph.scaleV * (style.baselineOffset() / 1000.0);
			if (style.fillColor() != CommonStrings::None)
				tmp2 += putColor(style.fillColor(), style.fillShade(), false);
			tmp2 += FToStr(Uwid)+" w\n";
			if (ite->itemType() == PageItem::PathText)
			{
				if (style.effects() & ScStyle_Subscript)
				{
					tmp2 += FToStr(x+hl->glyph.xoffset)     +" "+FToStr(-y+Upos)+" m\n";
					tmp2 += FToStr(x+hl->glyph.xoffset+Ulen)+" "+FToStr(-y+Upos)+" l\n";
				}
				else
				{
					tmp2 += FToStr(x+hl->glyph.xoffset)     +" "+FToStr(-y+hl->glyph.yoffset+Upos)+" m\n";
					tmp2 += FToStr(x+hl->glyph.xoffset+Ulen)+" "+FToStr(-y+hl->glyph.yoffset+Upos)+" l\n";
				}
			}
			else
			{
				if (style.effects() & ScStyle_Subscript)
				{
					tmp2 += FToStr(x+hl->glyph.xoffset)     +" "+FToStr(-y-hl->glyph.yoffset+Upos)+" m\n";
					tmp2 += FToStr(x+hl->glyph.xoffset+Ulen)+" "+FToStr(-y-hl->glyph.yoffset+Upos)+" l\n";
				}
				else
				{
					tmp2 += FToStr(x+hl->glyph.xoffset)     +" "+FToStr(-y+Upos)+" m\n";
					tmp2 += FToStr(x+hl->glyph.xoffset+Ulen)+" "+FToStr(-y+Upos)+" l\n";
				}
			}
			tmp2 += "S\n";
		}
		if (!style.font().hasNames())
		{
			if (glyph != style.font().char2CMap(QChar(' ')))
			{
				if ((style.strokeColor() != CommonStrings::None) && (style.effects() & ScStyle_Outline))
				{
					tmp2 += FToStr((tsz * style.outlineWidth() / 1000.0) / tsz)+" w\n[] 0 d\n0 J\n0 j\n";
					tmp2 += StrokeColor;
				}
				if (style.fillColor() != CommonStrings::None)
					tmp2 += FillColor;
				tmp2 += "q\n";
				// #see 8257 : transform is already computed at beginning of the function
				/*if (ite->itemType() == PageItem::PathText)
				{
					QPointF tangt = QPointF( cos(hl->PRot), sin(hl->PRot) );
					QTransform trafo = QTransform( 1, 0, 0, -1, -hl->PDx, 0 );
					if (ite->textPathFlipped)
						trafo *= QTransform(1, 0, 0, -1, 0, 0);
					if (ite->textPathType == 0)
						trafo *= QTransform(tangt.x(), -tangt.y(), -tangt.y(), -tangt.x(), hl->PtransX, -hl->PtransY);
					else if (ite->textPathType == 1)
						trafo *= QTransform(1, 0, 0, -1, hl->PtransX, -hl->PtransY );
					tmp2 += FToStr(trafo.m11())+" "+FToStr(trafo.m12())+" "+FToStr(trafo.m21())+" "+FToStr(trafo.m22())+" "+FToStr(trafo.dx())+" "+FToStr(trafo.dy())+" cm\n";
				}*/
				if (!ite->asPathText())
				{
					if (ite->reversed())
					{
						double wid = style.font().charWidth(chstr, style.fontSize()) * (hl->glyph.scaleH);
						tmp2 += "1 0 0 1 "+FToStr(x+hl->glyph.xoffset)+" "+FToStr((y+hl->glyph.yoffset - (tsz / 10.0)) * -1 + ((tsz / 10.0) * (style.baselineOffset() / 1000.0)))+" cm\n";
						tmp2 += "-1 0 0 1 0 0 cm\n";
						tmp2 += "1 0 0 1 "+FToStr(-wid)+" 0 cm\n";
						tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" 0 0 cm\n";
					}
					else
					{
						tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" "+FToStr(x+hl->glyph.xoffset)+" "+FToStr((y+hl->glyph.yoffset - (tsz / 10.0)) * -1 + ((tsz / 10.0) * (style.baselineOffset() / 1000.0)))+" cm\n";
					}
				}
				else
				{
					if (ite->BaseOffs != 0)
						tmp2 += "1 0 0 1 0 "+FToStr( -ite->BaseOffs)+" cm\n";
					tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" 0 "+FToStr(tsz / 10.0)+" cm\n";
				}
				if (hl->glyph.scaleV != 1.0)
					tmp2 += "1 0 0 1 0 "+FToStr( (((tsz / 10.0) - (tsz / 10.0) * (hl->glyph.scaleV)) / (tsz / 10.0)) * -1)+" cm\n";
				tmp2 += FToStr(qMax(hl->glyph.scaleH, 0.1))+" 0 0 "+FToStr(qMax(hl->glyph.scaleV, 0.1))+" 0 0 cm\n";
				if (style.fillColor() != CommonStrings::None)
					tmp2 += "/"+style.font().psName().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+QString::number(glyph)+" Do\n";
				if (style.effects() & ScStyle_Outline)
				{
					FPointArray gly = style.font().glyphOutline(glyph);
					QTransform mat;
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
					tmp2 += "h s\n";
				}
				tmp2 += "Q\n";
			}
		}
		else
		{
			if (style.strokeColor() != CommonStrings::None)
			{
				if ((style.effects() & ScStyle_Underline) || (style.effects() & ScStyle_Strikethrough) || (style.effects() & ScStyle_Outline))
					tmp2 += StrokeColor;
			}
			if (style.fillColor() != CommonStrings::None)
			{
				if ((style.effects() & ScStyle_Underline) || (style.effects() & ScStyle_Strikethrough))
					tmp2 += FillColor;
			}
			if (glyph != style.font().char2CMap(QChar(' ')))
			{
				uint idx = hl->glyph.glyph;
				uint idx1;
				if (Options.SubsetList.contains(style.font().replacementName()))
					idx1 = Type3Fonts[UsedFontsP[style.font().replacementName()]][idx] / 256;
				else
					idx1 = idx / 224;
				ScFace currentFace = style.font();
				if (Options.Version == PDFOptions::PDFVersion_X4
					&& (currentFace.format() == ScFace::SFNT || currentFace.format() == ScFace::TTCF)
					&& ( !Options.SubsetList.contains(style.font().replacementName()) ) )
					tmp+= UsedFontsP[currentFace.replacementName()]+" "+FToStr(tsz / 10.0)+" Tf\n";
				else
					tmp += UsedFontsP[style.font().replacementName()]+"S"+QString::number(idx1)+" "+FToStr(tsz / 10.0)+" Tf\n";
				if (style.strokeColor() != CommonStrings::None)
					tmp += StrokeColor;
				if (style.fillColor() != CommonStrings::None)
					tmp += FillColor;
				if ((Options.SubsetList.contains(style.font().replacementName())) && (style.effects() & ScStyle_Outline) && (style.strokeColor() != CommonStrings::None))
				{
					tmp2 += "q\n";
					tmp2 += FToStr((tsz * style.outlineWidth() / 1000.0) / tsz)+" w\n[] 0 d\n0 J\n0 j\n";
					// #see 8257 : transform is already computed at beginning of the function
					/*if (ite->itemType() == PageItem::PathText)
					{
						QPointF tangt = QPointF( cos(hl->PRot), sin(hl->PRot) );
						QTransform trafo = QTransform( 1, 0, 0, -1, -hl->PDx, 0 );
						if (ite->textPathFlipped)
							trafo *= QTransform(1, 0, 0, -1, 0, 0);
						if (ite->textPathType == 0)
							trafo *= QTransform(tangt.x(), -tangt.y(), -tangt.y(), -tangt.x(), hl->PtransX, -hl->PtransY);
						else if (ite->textPathType == 1)
							trafo *= QTransform(1, 0, 0, -1, hl->PtransX, -hl->PtransY );
						tmp2 += FToStr(trafo.m11())+" "+FToStr(trafo.m12())+" "+FToStr(trafo.m21())+" "+FToStr(trafo.m22())+" "+FToStr(trafo.dx())+" "+FToStr(trafo.dy())+" cm\n";
					}*/
					if (!ite->asPathText())
					{
						if (ite->reversed())
						{
							double wid = style.font().charWidth(chstr, style.fontSize()) * (hl->glyph.scaleH);
							tmp2 += "1 0 0 1 "+FToStr(x+hl->glyph.xoffset)+" "+FToStr((y+hl->glyph.yoffset - (tsz / 10.0)) * -1 + ((tsz / 10.0) * (style.baselineOffset() / 1000.0)))+" cm\n";
							tmp2 += "-1 0 0 1 0 0 cm\n";
							tmp2 += "1 0 0 1 "+FToStr(-wid)+" 0 cm\n";
							tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" 0 0 cm\n";
						}
						else
						{
							tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" "+FToStr(x+hl->glyph.xoffset)+" "+FToStr((y+hl->glyph.yoffset - (tsz / 10.0)) * -1 + ((tsz / 10.0) * (style.baselineOffset() / 1000.0)))+" cm\n";
						}
					}
					else
					{
						if (ite->BaseOffs != 0)
							tmp2 += "1 0 0 1 0 "+FToStr( -ite->BaseOffs)+" cm\n";
						tmp2 += FToStr(tsz / 10.0)+" 0 0 "+FToStr(tsz / 10.0)+" 0 "+FToStr(tsz / 10.0)+" cm\n";
					}
					if (hl->glyph.scaleV != 1.0)
						tmp2 += "1 0 0 1 0 "+FToStr( (((tsz / 10.0) - (tsz / 10.0) * (hl->glyph.scaleV)) / (tsz / 10.0)) * -1)+" cm\n";
					tmp2 += FToStr(qMax(hl->glyph.scaleH, 0.1))+" 0 0 "+FToStr(qMax(hl->glyph.scaleV, 0.1))+" 0 0 cm\n";
					FPointArray gly = style.font().glyphOutline(glyph);
					QTransform mat;
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
					tmp2 += "h s\n";
					tmp2 += "Q\n";
				}
				else
				{
					if (style.effects() & ScStyle_Outline)
						tmp += FToStr(tsz * style.outlineWidth() / 10000.0) + (style.fillColor() != CommonStrings::None ? " w 2 Tr\n" : " w 1 Tr\n");
					else
						tmp += "0 Tr\n";
				}
				if (!ite->asPathText())
				{
					if (ite->reversed())
					{
						double wtr = hl->glyph.xadvance;
						tmp +=  FToStr(-qMax(hl->glyph.scaleH, 0.1))+" 0 0 "+FToStr(qMax(hl->glyph.scaleV, 0.1)) +" "+FToStr(x+hl->glyph.xoffset+wtr)+" "+FToStr(-y-hl->glyph.yoffset+(style.fontSize() / 10.0) * (style.baselineOffset() / 1000.0))+" Tm\n";
					}
					else
						tmp +=  FToStr(qMax(hl->glyph.scaleH, 0.1))+" 0 0 "+FToStr(qMax(hl->glyph.scaleV, 0.1))+" "+FToStr(x+hl->glyph.xoffset)+" "+FToStr(-y-hl->glyph.yoffset+(style.fontSize() / 10.0) * (style.baselineOffset() / 1000.0))+" Tm\n";
				}
				else
					tmp += FToStr(qMax(hl->glyph.scaleH, 0.1))+" 0 0 "+FToStr(qMax(hl->glyph.scaleV, 0.1))+" 0 0 Tm\n";
				uchar idx2;
				if (Options.SubsetList.contains(style.font().replacementName()))
				{
					if (style.fillColor() != CommonStrings::None)
					{
						idx2 = Type3Fonts[UsedFontsP[style.font().replacementName()]][idx] % 256;
						tmp += "<"+QString(toHex(idx2))+"> Tj\n";
					}
				}
				else if (Options.Version == PDFOptions::PDFVersion_X4 && (currentFace.format() == ScFace::SFNT || currentFace.format() == ScFace::TTCF))
				{
					QString val;
					val.setNum(idx,16);
					int numberOfZero = 4-val.size();
					for (int i=0; i<numberOfZero; ++i)
						val.prepend("0");
					tmp += "<"+val+"> Tj\n";
				}
				else
				{
					idx2 = idx % 224 + 32;
					tmp += "<"+QString(toHex(idx2))+"> Tj\n";
				}
			}
		}
		if ((style.effects() & ScStyle_Strikethrough) && (chstr != SpecialChars::PARSEP))
		{
			//		double Ulen = style.font().charWidth(chstr, style.fontSize()) * (hl->glyph.scaleH);
			double Ulen = hl->glyph.xadvance;
			double Upos, Uwid, kern;
			if (hl->effects() & ScStyle_StartOfLine)
				kern = 0;
			else
				kern = style.fontSize() * style.tracking() / 10000.0;
			if ((style.strikethruOffset() != -1) || (style.strikethruWidth() != -1))
			{
				if (style.strikethruOffset() != -1)
					Upos = (style.strikethruOffset() / 1000.0) * (style.font().ascent(style.fontSize() / 10.0));
				else
					Upos = style.font().strikeoutPos(style.fontSize() / 10.0);
				if (style.strikethruWidth() != -1)
					Uwid = (style.strikethruWidth() / 1000.0) * (style.fontSize() / 10.0);
				else
					Uwid = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			else
			{
				Upos = style.font().strikeoutPos(style.fontSize() / 10.0);
				Uwid = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			if (style.baselineOffset() != 0)
				Upos += (style.fontSize() / 10.0) * hl->glyph.scaleV * (style.baselineOffset() / 1000.0);
			if (style.fillColor() != CommonStrings::None)
				tmp2 += putColor(style.fillColor(), style.fillShade(), false);
			tmp2 += FToStr(Uwid)+" w\n";
			if (ite->itemType() == PageItem::PathText)
			{
				tmp2 += FToStr(x+hl->glyph.xoffset)     +" "+FToStr(-y+Upos)+" m\n";
				tmp2 += FToStr(x+hl->glyph.xoffset+Ulen)+" "+FToStr(-y+Upos)+" l\n";
			}
			else
			{
				tmp2 += FToStr(x+hl->glyph.xoffset)     +" "+FToStr(-y-hl->glyph.yoffset+Upos)+" m\n";
				tmp2 += FToStr(x+hl->glyph.xoffset+Ulen)+" "+FToStr(-y-hl->glyph.yoffset+Upos)+" l\n";
			}
			tmp2 += "S\n";
		}
		if (ite->asPathText())
		{
			tmp += "ET\n"+tmp2+"Q\n";
			tmp2 = "";
		}
	}
	if (hl->glyph.more) {
		// ugly hack until setTextCh interface is changed
		ScText hl2(*hl);
		hl2.glyph = *(hl->glyph.more);
		if (!setTextCh(ite, PNr, x + hl->glyph.xadvance, y, d, tmp, tmp2, &hl2, pstyle, pag))
			return false;
		// don't let hl2's destructor delete these!
		hl2.glyph.more = 0;
	}
	return true;
#endif
}

QString PDFLibCore::SetColor(const QString& farbe, double Shade)
{
	const ScColor& col = doc.PageColors[farbe];
	return SetColor(col, Shade);
}

QString PDFLibCore::SetColor(const ScColor& farbe, double Shade)
{
	QString tmp;
	RGBColor rgb;
	CMYKColor cmyk;
	int h, s, v, k;
	ScColor tmpC(farbe);
	QColor tmpR;
	if (Options.isGrayscale)
	{
		bool kToGray = false;
		if (tmpC.getColorModel() == colorModelCMYK)
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			kToGray = (h == 0 && s == 0 && v == 0);
		}
		if (kToGray)
			tmp = FToStr(1.0 - k / 255.0);
		else
		{
			tmpR = ScColorEngine::getShadeColor(tmpC, &doc, Shade);
			tmpR.getRgb(&h, &s, &v);
			tmp = FToStr((0.3 * h + 0.59 * s + 0.11 * v) / 255.0);
		}
		return tmp;
	}
	if (Options.UseRGB)
	{
		tmpR = ScColorEngine::getShadeColor(tmpC, &doc, Shade);
		tmpR.getRgb(&h, &s, &v);
		tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
	}
	else
	{
		if ((doc.HasCMS) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
				cmyk.getValues(h, s, v, k);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
			}
			else
			{
				if (Options.SComp == 3)
				{
					ScColorEngine::getShadeColorRGB(tmpC, &doc, rgb, Shade);
					rgb.getValues(h, s, v);
					tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
				}
				else
				{
					ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
					cmyk.getValues(h, s, v, k);
					tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
				}
			}
		}
		else
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
		}
	}
	return tmp;
}

QString PDFLibCore::SetGradientColor(const QString& farbe, double Shade)
{
	QString tmp;
	RGBColor rgb;
	CMYKColor cmyk;
	int h, s, v, k;
	if (farbe == CommonStrings::None)
	{
		if (Options.isGrayscale)
			tmp = "0.0";
		if (Options.UseRGB)
			tmp = "1.0 1.0 1.0";
		else
		{
			if ((doc.HasCMS) && (Options.UseProfiles))
			{
				if (Options.SComp == 3)
					tmp = "1.0 1.0 1.0";
				else
					tmp = "0.0 0.0 0.0 0.0";
			}
			else
				tmp = "0.0 0.0 0.0 0.0";
		}
		return tmp;
	}
	ScColor tmpC(doc.PageColors[farbe]);
	QColor tmpR;
	if (Options.isGrayscale)
	{
		bool kToGray = false;
		if (tmpC.getColorModel() == colorModelCMYK)
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			kToGray = (h == 0 && s == 0 && v == 0);
		}
		if (kToGray)
			tmp = FToStr(1.0 - k / 255.0);
		else
		{
			tmpR = ScColorEngine::getShadeColor(tmpC, &doc, Shade);
			tmpR.getRgb(&h, &s, &v);
			tmp = FToStr((0.3 * h + 0.59 * s + 0.11 * v) / 255.0);
		}
		return tmp;
	}
	if (Options.UseRGB)
	{
		tmpR = ScColorEngine::getShadeColor(tmpC, &doc, Shade);
		tmpR.getRgb(&h, &s, &v);
		tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
	}
	else
	{
		if ((doc.HasCMS) && (Options.UseProfiles))
		{
			if (Options.SComp == 3)
			{
				ScColorEngine::getShadeColorRGB(tmpC, &doc, rgb, Shade);
				rgb.getValues(h, s, v);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
			}
			else
			{
				ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
				cmyk.getValues(h, s, v, k);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
			}
		}
		else
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
		}
	}
	return tmp;
}

QString PDFLibCore::SetClipPathImage(PageItem *ite)
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
			FPoint np, np1, np2, np3;
			if (nPath)
			{
				np = ite->imageClip.point(poi);
				tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
				nPath = false;
			}
			np = ite->imageClip.point(poi);
			np1 = ite->imageClip.point(poi+1);
			np2 = ite->imageClip.point(poi+3);
			np3 = ite->imageClip.point(poi+2);
			if ((np == np1) && (np2 == np3))
				tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" l\n";
			else
			{
				tmp += FToStr(np1.x())+" "+FToStr(-np1.y())+" ";
				tmp += FToStr(np2.x())+" "+FToStr(-np2.y())+" ";
				tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" c\n";
			}
		}
	}
	return tmp;
}

QString PDFLibCore::SetClipPath(PageItem *ite, bool poly)
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
			FPoint np, np1, np2, np3;
			if (nPath)
			{
				np = ite->PoLine.point(poi);
				tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
				nPath = false;
			}
			np = ite->PoLine.point(poi);
			np1 = ite->PoLine.point(poi+1);
			np2 = ite->PoLine.point(poi+3);
			np3 = ite->PoLine.point(poi+2);
			if ((np == np1) && (np2 == np3))
				tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" l\n";
			else
			{
				tmp += FToStr(np1.x())+" "+FToStr(-np1.y())+" ";
				tmp += FToStr(np2.x())+" "+FToStr(-np2.y())+" ";
				tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" c\n";
			}
		}
	}
	return tmp;
}

QString PDFLibCore::SetClipPathArray(FPointArray *ite, bool poly)
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
			FPoint np, np1, np2, np3;
			if (nPath)
			{
				np = ite->point(poi);
				tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
				nPath = false;
			}
			np = ite->point(poi);
			np1 = ite->point(poi+1);
			np2 = ite->point(poi+3);
			np3 = ite->point(poi+2);
			if ((np == np1) && (np2 == np3))
				tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" l\n";
			else
			{
				tmp += FToStr(np1.x())+" "+FToStr(-np1.y())+" ";
				tmp += FToStr(np2.x())+" "+FToStr(-np2.y())+" ";
				tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" c\n";
			}
		}
	}
	return tmp;
}

void PDFLibCore::getBleeds(const Page* page, double &left, double &right)
{
	MarginStruct values;
	doc.getBleeds(page, Options.bleeds, values);
	left   = values.Left;
	right  = values.Right;
}

void PDFLibCore::getBleeds(const Page* page, double &left, double &right, double &bottom, double& top)
{
	MarginStruct values;
	doc.getBleeds(page, Options.bleeds, values);
	left   = values.Left;
	right  = values.Right;
	bottom = values.Bottom;
	top    = values.Top;
}

QString PDFLibCore::PDF_TransparenzFill(PageItem *currItem)
{
	QString ShName = ResNam+QString::number(ResCount);
	ResCount++;
	QString tmp;
	QString GXName;
	double scaleX = 1.0;
	double scaleY = 1.0;
	if (currItem->itemType() == PageItem::Symbol)
	{
		ScPattern pat = doc.docPatterns[currItem->pattern()];
		scaleX = 1.0 / (currItem->width() / pat.width);
		scaleY = 1.0 / (currItem->height() / pat.height);
	}
	if ((currItem->GrMask == 1) || (currItem->GrMask == 2) || (currItem->GrMask == 4) || (currItem->GrMask == 5))
	{
		QList<double> StopVec;
		QList<double> TransVec;
		VGradient gradient;
		double StartX, StartY, EndX, EndY, FocalX, FocalY, Gscale, Gskew;
		int GType;
		GType = currItem->GrMask;
		StartX = currItem->GrMaskStartX;
		StartY = currItem->GrMaskStartY;
		EndX = currItem->GrMaskEndX;
		EndY = currItem->GrMaskEndY;
		FocalX = currItem->GrMaskFocalX;
		FocalY = currItem->GrMaskFocalY;
		Gscale = currItem->GrMaskScale;
		Gskew = currItem->GrMaskSkew;
		gradient = currItem->mask_gradient;
		QList<VColorStop*> cstops = gradient.colorStops();
		StopVec.clear();
		TransVec.clear();
		QTransform mpa;
		if (currItem->isGroup())
		{
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
		}
		else if (currItem->itemType() == PageItem::Symbol)
		{
			mpa.translate(0, currItem->height() * scaleY);
			mpa.scale(scaleX, scaleY);
		}
		else
			mpa.rotate(-currItem->rotation());
		if (Gskew == 90)
			Gskew = 1;
		else if (Gskew == 180)
			Gskew = 0;
		else if (Gskew == 270)
			Gskew = -1;
		else if (Gskew == 390)
			Gskew = 0;
		else
			Gskew = tan(M_PI / 180.0 * Gskew);
		if ((GType == 1) || (GType == 4))
		{
			mpa.translate(StartX, -StartY);
			mpa.shear(Gskew, 0);
			mpa.translate(-StartX, StartY);
		}
		else
		{
			double rotEnd = xy2Deg(EndX - StartX, EndY - StartY);
			mpa.translate(StartX, -StartY);
			mpa.rotate(-rotEnd);
			mpa.shear(-Gskew, 0);
			mpa.translate(0, -StartY * (1.0 - Gscale));
			mpa.translate(-StartX, StartY);
			mpa.scale(1, Gscale);
		}
		QColor qStopColor;
		for (uint cst = 0; cst < gradient.Stops(); ++cst)
		{
			double actualStop = cstops.at(cst)->rampPoint;
			qStopColor = cstops.at(cst)->color;
			int h, s, v, sneu, vneu;
			int shad = cstops.at(cst)->shade;
			qStopColor.getHsv(&h, &s, &v);
			sneu = s * shad / 100;
			vneu = 255 - ((255 - v) * shad / 100);
			qStopColor.setHsv(h, sneu, vneu);
			double a = cstops.at(cst)->opacity;
			double r, g, b;
			qStopColor.getRgbF(&r, &g, &b);
			if ((GType == 4) || (GType == 5))
				a = /* 1.0 - */ (0.3 * r + 0.59 * g + 0.11 * b);
			if ((cst == 0) && (actualStop != 0.0))
			{
				StopVec.append(0.0);
				TransVec.append(a);
			}
			StopVec.append(actualStop);
			TransVec.append(a);
			if ((cst == gradient.Stops()-1) && (actualStop < 1.0))
			{
				StopVec.append(1.0);
				TransVec.append(a);
			}
		}
		QString TRes("");
		uint patObject = newObject();
		StartObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
		PutDoc("/Shading\n");
		PutDoc("<<\n");
		if ((GType == 1) || (GType == 4))
			PutDoc("/ShadingType 2\n");
		else
			PutDoc("/ShadingType 3\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/Extend [true true]\n");
		if ((GType == 1) || (GType == 4))
			PutDoc("/Coords ["+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(EndX)+" "+FToStr(-EndY)+"]\n");
		else
			PutDoc("/Coords ["+FToStr(FocalX)+" "+FToStr(-FocalY)+" 0.0 "+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2)))+"]\n");
		PutDoc("/Function\n");
		PutDoc("<<\n");
		PutDoc("/FunctionType 3\n");
		PutDoc("/Domain [0 1]\n");
		if (StopVec.count() > 2)
		{
			PutDoc("/Bounds [");
			QString bctx = "";
			for (int bc = 1; bc < StopVec.count() - 1; bc++)
			{
				bctx += FToStr(StopVec.at(bc))+" ";
			}
			PutDoc(bctx.trimmed()+"]\n");
		}
		else
			PutDoc("/Bounds []\n");
		QString entx = "";
		PutDoc("/Functions\n");
		PutDoc("[\n");
		for (int cc = 0; cc < TransVec.count() - 1; cc++)
		{
			entx += "0 1 ";
			PutDoc("<<\n");
			PutDoc("/FunctionType 2\n");
			PutDoc("/Domain [0 1]\n");
			PutDoc("/C0 ["+FToStr(TransVec.at(cc))+"]\n");
			PutDoc("/C1 ["+FToStr(TransVec.at(cc+1))+"]\n");
			PutDoc("/N 1\n");
			PutDoc(">>\n");
		}
		PutDoc("]\n");
		PutDoc("/Encode ["+entx.trimmed()+"]\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		Patterns.insert("Pattern"+QString::number(patObject), patObject);
		uint formObject = newObject();
		StartObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = currItem->lineWidth();
		if ((currItem->isGroup()) || (currItem->itemType() == PageItem::Symbol))
			PutDoc("/BBox [0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" ]\n");
		else
			PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(currItem->width()+lw)+" "+FToStr(-(currItem->height()+lw))+" ]\n");
		PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		if (Patterns.count() != 0)
		{
			PutDoc("/Pattern << \n");
			QMap<QString,int>::Iterator it3p;
			for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
				PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
			PutDoc(">>\n");
		}
		PutDoc(">>\n");
		QString stre = "q\n";
		if (currItem->isGroup())
		{
			QTransform mpa;
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
			stre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
		}
		else if (currItem->itemType() == PageItem::Symbol)
		{
			QTransform mpa;
			mpa.translate(0, currItem->height() * scaleY);
			mpa.scale(scaleX, scaleY);
			stre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
		}
		stre += SetClipPath(currItem)+"h\n";
		stre += FToStr(fabs(currItem->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+QString::number(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressStr(&stre);
		PutDoc("/Length "+QString::number(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream\nendobj\n");
		Seite.XObjects[ResNam+QString::number(ResCount)] = formObject;
		ResCount++;
		GXName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+QString::number(formObject)+" 0 R >>\n/AIS false\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		tmp = "/"+GXName+" gs\n";
	}
	else if ((currItem->GrMask == 3) || (currItem->GrMask == 6))
	{
		QString tmpOut = "";
		PDF_PatternFillStroke(tmpOut, currItem, 2);
		uint formObject = newObject();
		StartObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency ");
		if (Options.UseRGB)
			PutDoc("/CS /DeviceRGB");
		else
		{
			if (Options.isGrayscale)
				PutDoc("/CS /DeviceGray");
			else
			{
				if ((doc.HasCMS) && (Options.UseProfiles))
					PutDoc("/CS "+ICCProfiles[Options.SolidProf].ICCArray);
				else
					PutDoc("/CS /DeviceCMYK");
			}
		}
		PutDoc(" >>\n");
		if ((currItem->isGroup()) || (currItem->itemType() == PageItem::Symbol))
			PutDoc("/BBox [0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" ]\n");
		else
			PutDoc("/BBox [0 0 "+FToStr(currItem->width())+" "+FToStr(-(currItem->height()))+" ]\n");
		PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		if (Patterns.count() != 0)
		{
			PutDoc("/Pattern << \n");
			QMap<QString,int>::Iterator it3p;
			for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
				PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
			PutDoc(">>\n");
		}
		PutDoc(">>\n");
		QString stre = "q\n";
		if (currItem->isGroup())
		{
			QTransform mpa;
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
			stre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
		}
		else if (currItem->itemType() == PageItem::Symbol)
		{
			QTransform mpa;
			mpa.translate(0, currItem->height() * scaleY);
			mpa.scale(scaleX, scaleY);
			stre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
		}
		stre += SetClipPath(currItem)+"h\n";
		stre += FToStr(fabs(currItem->lineWidth()))+" w\n";
		stre += tmpOut+" f*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressStr(&stre);
		PutDoc("/Length "+QString::number(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream\nendobj\n");
		Seite.XObjects[ResNam+QString::number(ResCount)] = formObject;
		ResCount++;
		GXName = ResNam+QString::number(ResCount);
		ResCount++;
		if (currItem->GrMask == 6)
			Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+QString::number(formObject)+" 0 R >>\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		else
			Transpar[GXName] = writeGState("/SMask << /S /Alpha /G "+QString::number(formObject)+" 0 R >>\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		tmp = "/"+GXName+" gs\n";
	}
	else
	{
		Transpar[ShName] = writeGState("/ca "+FToStr(1.0 - currItem->fillTransparency())+"\n/SMask /None\n/AIS false\n/OPM 1\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		tmp = "/"+ShName+" gs\n";
	}
	return tmp;
}

QString PDFLibCore::PDF_TransparenzStroke(PageItem *currItem)
{
	QString ShName = ResNam+QString::number(ResCount);
	ResCount++;
	Transpar[ShName] = writeGState("/CA "+FToStr(1.0 - currItem->lineTransparency())+"\n"
								   + "/SMask /None\n/AIS false\n/OPM 1\n"
								   + "/BM /" + blendMode(currItem->lineBlendmode()) + "\n");
	QString tmp("/"+ShName+" gs\n");
	return tmp;
}

bool PDFLibCore::PDF_PatternFillStroke(QString& output, PageItem *currItem, int kind, bool forArrow)
{
	QString tmp2 = "", tmpOut;
	ScPattern *pat = NULL;
	if (kind == 0)
		pat = &doc.docPatterns[currItem->pattern()];
	else if (kind == 1)
		pat = &doc.docPatterns[currItem->strokePattern()];
	else if (kind == 2)
		pat = &doc.docPatterns[currItem->patternMask()];
	else
		return false;
	for (int em = 0; em < pat->items.count(); ++em)
	{
		PageItem* item = pat->items.at(em);
		tmp2 += "q\n";
		tmp2 +=  "1 0 0 1 "+FToStr(item->gXpos)+" "+FToStr(-(item->gYpos - pat->height))+" cm\n";
		item->setXYPos(item->xPos() + ActPageP->xOffset(), item->yPos() + ActPageP->yOffset(), true);
		inPattern++;
		if (!PDF_ProcessItem(tmpOut, item, doc.DocPages.at(0), 0, true, true))
			return false;
		tmp2 += tmpOut;
		item->setXYPos(item->xPos() - ActPageP->xOffset(), item->yPos() - ActPageP->yOffset(), true);
		inPattern--;
		tmp2 += "Q\n";
	}
	for (int em = 0; em < pat->items.count(); ++em)
	{
		PageItem* item = pat->items.at(em);
		if (!item->isTableItem)
			continue;
		if ((item->lineColor() == CommonStrings::None) || (item->lineWidth() == 0.0))
			continue;
		tmp2 += "q\n";
		if ((item->doOverprint) && (!Options.UseRGB))
		{
			QString ShName = ResNam+QString::number(ResCount);
			ResCount++;
			Transpar[ShName] = writeGState("/OP true\n"
										"/op true\n"
										"/OPM 1\n");
			tmp2 += "/"+ShName+" gs\n";
		}
		if (((item->lineTransparency() != 0) || (item->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
			tmp2 += PDF_TransparenzStroke(item);
		if (item->lineColor() != CommonStrings::None)
			tmp2 += putColor(item->lineColor(), item->lineShade(), false);
		tmp2 += FToStr(fabs(item->lineWidth()))+" w\n";
		if (item->DashValues.count() != 0)
		{
			tmp2 += "[ ";
			QVector<double>::iterator it;
			for ( it = item->DashValues.begin(); it != item->DashValues.end(); ++it )
			{
				int da = static_cast<int>(*it);
				// #8758: Custom dotted lines don't export properly to pdf
				// Null values have to be exported if line end != flat
				if ((da != 0) || (item->lineEnd() != Qt::FlatCap))
					tmp2 += QString::number(da)+" ";
			}
			tmp2 += "] "+QString::number(static_cast<int>(item->DashOffset))+" d\n";
		}
		else
			tmp2 += "["+getDashString(item->PLineArt, item->lineWidth())+"] 0 d\n";
		tmp2 += "2 J\n";
		switch (item->PLineJoin)
		{
			case Qt::MiterJoin:
				tmp2 += "0 j\n";
				break;
			case Qt::BevelJoin:
				tmp2 += "2 j\n";
				break;
			case Qt::RoundJoin:
				tmp2 += "1 j\n";
				break;
			default:
				tmp2 += "0 j\n";
				break;
		}
		tmp2 +=  "1 0 0 1 "+FToStr(item->gXpos)+" "+FToStr(-(item->gYpos - pat->height))+" cm\n";
		if (item->rotation() != 0)
		{
			double sr = sin(-item->rotation()* M_PI / 180.0);
			double cr = cos(-item->rotation()* M_PI / 180.0);
			if ((cr * cr) < 0.000001)
				cr = 0;
			if ((sr * sr) < 0.000001)
				sr = 0;
			tmp2 += FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+ " 0 0 cm\n";
		}
		if ((item->TopLine) || (item->RightLine) || (item->BottomLine) || (item->LeftLine))
		{
			if (item->TopLine)
			{
				tmp2 += "0 0 m\n";
				tmp2 += FToStr(item->width())+" 0 l\n";
			}
			if (item->RightLine)
			{
				tmp2 += FToStr(item->width())+" 0 m\n";
				tmp2 += FToStr(item->width())+" "+FToStr(-item->height())+" l\n";
			}
			if (item->BottomLine)
			{
				tmp2 += "0 "+FToStr(-item->height())+" m\n";
				tmp2 += FToStr(item->width())+" "+FToStr(-item->height())+" l\n";
			}
			if (item->LeftLine)
			{
				tmp2 += "0 0 m\n";
				tmp2 += "0 "+FToStr(-item->height())+" l\n";
			}
			tmp2 += "S\n";
		}
		tmp2 += "Q\n";
	}
	if (Options.Compress)
		tmp2 = CompressStr(&tmp2);
	uint patObject = newObject();
	StartObj(patObject);
	PutDoc("<< /Type /Pattern\n");
	PutDoc("/PatternType 1\n");
	PutDoc("/PaintType 1\n");
	PutDoc("/TilingType 1\n");
	PutDoc("/BBox [ 0 0 "+FToStr(pat->width)+" "+FToStr(pat->height)+" ]\n");
	double scaleX = 1.0;
	double scaleY = 1.0;
	if (currItem->itemType() == PageItem::Symbol)
	{
		ScPattern pat = doc.docPatterns[currItem->pattern()];
		scaleX = 1.0 / (currItem->width() / pat.width);
		scaleY = 1.0 / (currItem->height() / pat.height);
	}
	QTransform mpa;
	if ((inPattern == 0) && (kind != 2))
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(currItem->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - currItem->gYpos);
			mpa.rotate(-currItem->rotation());
		}
		else
		{
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
		}
	}
	double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace;
	bool mirrorX, mirrorY;
	if (kind == 0)
	{
		currItem->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		currItem->patternFlip(mirrorX, mirrorY);
	}
	else if (kind == 1)
	{
		currItem->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
		currItem->strokePatternFlip(mirrorX, mirrorY);
	}
	else if (kind == 2)
	{
		if (currItem->isGroup())
		{
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
		}
		else if (currItem->itemType() == PageItem::Symbol)
		{
			mpa.translate(0, currItem->height() * scaleY);
			mpa.scale(scaleX, scaleY);
		}
		currItem->maskTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		currItem->maskFlip(mirrorX, mirrorY);
	}
	if (kind == 1)
		mpa.translate(-currItem->lineWidth() / 2.0, currItem->lineWidth() / 2.0);
	mpa.translate(patternOffsetX, -patternOffsetY);
	if (currItem->itemType() == PageItem::Symbol)
		mpa.rotate(patternRotation);
	else
		mpa.rotate(-patternRotation);
	mpa.shear(patternSkewX, -patternSkewY);
	mpa.scale(pat->scaleX, pat->scaleY);
	mpa.scale(patternScaleX / 100.0 , patternScaleY / 100.0);
	if (mirrorX)
		mpa.scale(-1, 1);
	if (mirrorY)
		mpa.scale(1, -1);
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/XStep "+FToStr(pat->width)+"\n");
	PutDoc("/YStep "+FToStr(pat->height)+"\n");
	PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
	if (Seite.ImgObjects.count() != 0)
	{
		PutDoc("/XObject <<\n");
		QMap<QString,int>::Iterator it;
		for (it = Seite.ImgObjects.begin(); it != Seite.ImgObjects.end(); ++it)
			PutDoc("/"+it.key()+" "+QString::number(it.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Seite.FObjects.count() != 0)
	{
		PutDoc("/Font << \n");
		QMap<QString,int>::Iterator it2;
		for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
			PutDoc("/"+it2.key()+" "+QString::number(it2.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Shadings.count() != 0)
	{
		PutDoc("/Shading << \n");
		QMap<QString,int>::Iterator it3;
		for (it3 = Shadings.begin(); it3 != Shadings.end(); ++it3)
			PutDoc("/"+it3.key()+" "+QString::number(it3.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Patterns.count() != 0)
	{
		PutDoc("/Pattern << \n");
		QMap<QString,int>::Iterator it3p;
		for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
			PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Transpar.count() != 0)
	{
		PutDoc("/ExtGState << \n");
		QMap<QString,int>::Iterator it3t;
		for (it3t = Transpar.begin(); it3t != Transpar.end(); ++it3t)
			PutDoc("/"+it3t.key()+" "+QString::number(it3t.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if ((ICCProfiles.count() != 0) || (spotMap.count() != 0))
	{
		PutDoc("/ColorSpace << \n");
		QMap<QString,ICCD>::Iterator it3c;
		if (ICCProfiles.count() != 0)
		{
			for (it3c = ICCProfiles.begin(); it3c != ICCProfiles.end(); ++it3c)
				PutDoc("/"+it3c.value().ResName+" "+QString::number(it3c.value().ResNum)+" 0 R\n");
		}
		QMap<QString,SpotC>::Iterator it3sc;
		if (spotMap.count() != 0)
		{
			for (it3sc = spotMap.begin(); it3sc != spotMap.end(); ++it3sc)
				PutDoc("/"+it3sc.value().ResName+" "+QString::number(it3sc.value().ResNum)+" 0 R\n");
		}
		PutDoc(">>\n");
	}
	PutDoc(">>\n");
	PutDoc("/Length "+QString::number(tmp2.length()));
	if (Options.Compress)
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n"+EncStream(tmp2, patObject)+"\nendstream\nendobj\n");
	Patterns.insert("Pattern"+QString::number(patObject), patObject);
	QString tmp;
	if ((forArrow) || (kind != 1))
	{
		tmp = "/Pattern cs\n";
		tmp += "/Pattern"+QString::number(patObject)+" scn\n";
	}
	else
	{
		tmp = "/Pattern CS\n";
		tmp += "/Pattern"+QString::number(patObject)+" SCN\n";
	}
	ResCount++;
	output = tmp;
	return true;
}

quint32 PDFLibCore::encode32dVal(double val)
{
	quint32 res = static_cast<quint32>(((val - (-40000.0)) / 80000.0) * 0xFFFFFFFF);
	return res;
}

quint16 PDFLibCore::encode16dVal(double val)
{
	quint16 m = val * 0xFFFF;
	return m;
}

void PDFLibCore::encodeColor(QDataStream &vs, QString colName, int colShade, QStringList &spotColorSet, bool spotMode)
{
	if (spotMode)
	{
		if (spotColorSet.contains(colName))
		{
			vs << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0);
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				if (spotColorSet.at(sc) == colName)
					vs << encode16dVal(colShade / 100.0);
				else
					vs << encode16dVal(0.0);
			}
		}
		else
		{
			QStringList gcol = SetGradientColor(colName, colShade).split(" ");
			for (int gcs = 0; gcs < gcol.count(); gcs++)
			{
				vs << encode16dVal(gcol[gcs].toDouble());
			}
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				vs << encode16dVal(0.0);
			}
		}
	}
	else
	{
		QStringList gcol = SetGradientColor(colName, colShade).split(" ");
		for (int gcs = 0; gcs < gcol.count(); gcs++)
		{
			vs << encode16dVal(gcol[gcs].toDouble());
		}
	}
}

bool PDFLibCore::PDF_MeshGradientFill(QString& output, PageItem *c)
{
	QList<double> StopVec;
	QList<double> TransVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	bool spotMode = false;
	bool transparencyFound = false;
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	colorNames.clear();
	colorShades.clear();
	for (int grow = 0; grow < c->meshGradientArray.count(); grow++)
	{
		for (int gcol = 0; gcol < c->meshGradientArray[grow].count(); gcol++)
		{
			meshPoint mp1 = c->meshGradientArray[grow][gcol];
			colorNames.append(mp1.colorName);
			colorShades.append(mp1.shade);
			TransVec.append(mp1.transparency);
			if (mp1.transparency != 1.0)
				transparencyFound = true;
			if (spotMap.contains(mp1.colorName))
			{
				if (!spotColorSet.contains(mp1.colorName))
					spotColorSet.append(mp1.colorName);
			}
			Gcolors.append(SetGradientColor(mp1.colorName, mp1.shade));
		}
	}
	QString TRes("");
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		uint shadeObjectT = newObject();
		StartObj(shadeObjectT);
		PutDoc("<<\n");
		PutDoc("/ShadingType 6\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/BitsPerCoordinate 32\n");
		PutDoc("/BitsPerComponent 16\n");
		PutDoc("/BitsPerFlag 8\n");
		QByteArray vertStreamT;
		QDataStream vst(&vertStreamT, QIODevice::WriteOnly);
		vst.setByteOrder(QDataStream::BigEndian);
		quint8 flg = 0;
		for (int grow = 0; grow < c->meshGradientArray.count()-1; grow++)
		{
			for (int gcol = 0; gcol < c->meshGradientArray[grow].count()-1; gcol++)
			{
				meshPoint mp1 = c->meshGradientArray[grow][gcol];
				meshPoint mp2 = c->meshGradientArray[grow][gcol+1];
				meshPoint mp3 = c->meshGradientArray[grow+1][gcol+1];
				meshPoint mp4 = c->meshGradientArray[grow+1][gcol];
				int colInd1 = grow * c->meshGradientArray[grow].count() + gcol;
				int colInd2 = grow * c->meshGradientArray[grow].count() + gcol + 1;
				int colInd3 = (grow + 1) * c->meshGradientArray[grow].count() + gcol + 1;
				int colInd4 = (grow + 1) * c->meshGradientArray[grow].count() + gcol;
				vst << flg;
				vst << encode32dVal(mp4.gridPoint.x()) << encode32dVal(-mp4.gridPoint.y()) << encode32dVal(mp4.controlTop.x()) << encode32dVal(-mp4.controlTop.y()) << encode32dVal(mp1.controlBottom.x()) << encode32dVal(-mp1.controlBottom.y());
				vst << encode32dVal(mp1.gridPoint.x()) << encode32dVal(-mp1.gridPoint.y()) << encode32dVal(mp1.controlRight.x()) << encode32dVal(-mp1.controlRight.y()) << encode32dVal(mp2.controlLeft.x()) << encode32dVal(-mp2.controlLeft.y());
				vst << encode32dVal(mp2.gridPoint.x()) << encode32dVal(-mp2.gridPoint.y()) << encode32dVal(mp2.controlBottom.x()) << encode32dVal(-mp2.controlBottom.y()) << encode32dVal(mp3.controlTop.x()) << encode32dVal(-mp3.controlTop.y());
				vst << encode32dVal(mp3.gridPoint.x()) << encode32dVal(-mp3.gridPoint.y()) << encode32dVal(mp3.controlLeft.x()) << encode32dVal(-mp3.controlLeft.y()) << encode32dVal(mp4.controlRight.x()) << encode32dVal(-mp4.controlRight.y());
				encodeColor(vst, colorNames[colInd4], colorShades[colInd4], spotColorSet, spotMode);
				encodeColor(vst, colorNames[colInd1], colorShades[colInd1], spotColorSet, spotMode);
				encodeColor(vst, colorNames[colInd2], colorShades[colInd2], spotColorSet, spotMode);
				encodeColor(vst, colorNames[colInd3], colorShades[colInd3], spotColorSet, spotMode);
			}
		}
		PutDoc("/Decode [-40000 40000 -40000 40000 0 1]\n");
		QString dat = "";
		for (int vd = 0; vd < vertStreamT.count(); vd++)
		{
			dat += vertStreamT[vd];
		}
		if (Options.Compress)
			dat = CompressStr(&dat);
		PutDoc("/Length "+QString::number(dat.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(dat, shadeObjectT)+"\nendstream\nendobj\n");
		uint patObject = newObject();
		StartObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Shading "+QString::number(shadeObjectT)+" 0 R\n");
		PutDoc(">>\nendobj\n");
		Patterns.insert("Pattern"+QString::number(patObject), patObject);
		uint formObject = newObject();
		StartObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = c->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(c->width()+lw)+" "+FToStr(-(c->height()+lw))+" ]\n");
		PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		if (Patterns.count() != 0)
		{
			PutDoc("/Pattern << \n");
			QMap<QString,int>::Iterator it3p;
			for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
				PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
			PutDoc(">>\n");
		}
		PutDoc(">>\n");
		QString stre = "q\n"+SetClipPath(c)+"h\n";
		stre += FToStr(fabs(c->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+QString::number(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressStr(&stre);
		PutDoc("/Length "+QString::number(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream\nendobj\n");
		Seite.XObjects[ResNam+QString::number(ResCount)] = formObject;
		ResCount++;
		QString GXName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+QString::number(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	QString entx = "";
	uint spotObject = 0;
	uint shadeObject = newObject();
	StartObj(shadeObject);
	PutDoc("<<\n");
	PutDoc("/ShadingType 6\n");
	if (Options.UseRGB)
	{
		PutDoc("/ColorSpace /DeviceRGB\n");
		entx = "0 1 0 1 0 1";
	}
	else if (Options.isGrayscale)
	{
		PutDoc("/ColorSpace /DeviceGray\n");
		entx = "0 1";
	}
	else if ((doc.HasCMS) && (Options.UseProfiles))
	{
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
		entx = "0 1 0 1 0 1";
	}
	else
	{
		entx = "0 1 0 1 0 1 0 1";
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				entx += " 0 1";
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(QString::number(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/BitsPerCoordinate 32\n");
	PutDoc("/BitsPerComponent 16\n");
	PutDoc("/BitsPerFlag 8\n");
	QByteArray vertStream;
	QDataStream vs(&vertStream, QIODevice::WriteOnly);
	vs.setByteOrder(QDataStream::BigEndian);
	quint8 flg = 0;
	for (int grow = 0; grow < c->meshGradientArray.count()-1; grow++)
	{
		for (int gcol = 0; gcol < c->meshGradientArray[grow].count()-1; gcol++)
		{
			meshPoint mp1 = c->meshGradientArray[grow][gcol];
			meshPoint mp2 = c->meshGradientArray[grow][gcol+1];
			meshPoint mp3 = c->meshGradientArray[grow+1][gcol+1];
			meshPoint mp4 = c->meshGradientArray[grow+1][gcol];
			int colInd1 = grow * c->meshGradientArray[grow].count() + gcol;
			int colInd2 = grow * c->meshGradientArray[grow].count() + gcol + 1;
			int colInd3 = (grow + 1) * c->meshGradientArray[grow].count() + gcol + 1;
			int colInd4 = (grow + 1) * c->meshGradientArray[grow].count() + gcol;
			vs << flg;
			vs << encode32dVal(mp4.gridPoint.x()) << encode32dVal(-mp4.gridPoint.y()) << encode32dVal(mp4.controlTop.x()) << encode32dVal(-mp4.controlTop.y()) << encode32dVal(mp1.controlBottom.x()) << encode32dVal(-mp1.controlBottom.y());
			vs << encode32dVal(mp1.gridPoint.x()) << encode32dVal(-mp1.gridPoint.y()) << encode32dVal(mp1.controlRight.x()) << encode32dVal(-mp1.controlRight.y()) << encode32dVal(mp2.controlLeft.x()) << encode32dVal(-mp2.controlLeft.y());
			vs << encode32dVal(mp2.gridPoint.x()) << encode32dVal(-mp2.gridPoint.y()) << encode32dVal(mp2.controlBottom.x()) << encode32dVal(-mp2.controlBottom.y()) << encode32dVal(mp3.controlTop.x()) << encode32dVal(-mp3.controlTop.y());
			vs << encode32dVal(mp3.gridPoint.x()) << encode32dVal(-mp3.gridPoint.y()) << encode32dVal(mp3.controlLeft.x()) << encode32dVal(-mp3.controlLeft.y()) << encode32dVal(mp4.controlRight.x()) << encode32dVal(-mp4.controlRight.y());
			encodeColor(vs, colorNames[colInd4], colorShades[colInd4], spotColorSet, spotMode);
			encodeColor(vs, colorNames[colInd1], colorShades[colInd1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[colInd2], colorShades[colInd2], spotColorSet, spotMode);
			encodeColor(vs, colorNames[colInd3], colorShades[colInd3], spotColorSet, spotMode);
		}
	}
	PutDoc("/Decode [-40000 40000 -40000 40000 "+entx+"]\n");
	QString dat = "";
	for (int vd = 0; vd < vertStream.count(); vd++)
	{
		dat += vertStream[vd];
	}
	if (Options.Compress)
		dat = CompressStr(&dat);
	PutDoc("/Length "+QString::number(dat.length())+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(dat, shadeObject)+"\nendstream\nendobj\n");
	uint patObject = newObject();
	StartObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	QTransform mpa;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(c->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - c->gYpos);
			mpa.rotate(-c->rotation());
		}
		else
		{
			mpa.translate(c->xPos() - ActPageP->xOffset(), ActPageP->height() - (c->yPos() - ActPageP->yOffset()));
			mpa.rotate(-c->rotation());
		}
	}
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading "+QString::number(shadeObject)+" 0 R\n");
	PutDoc(">>\nendobj\n");
	Patterns.insert("Pattern"+QString::number(patObject), patObject);
	if (spotMode)
	{
		QString colorDesc;
		StartObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			int cc = 0;
			int mc = 0;
			int yc = 0;
			int kc = 0;
			CMYKColor cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			else
				colorDesc += QString::number(sc*4 + 1)+" -1 roll dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(mc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(yc) / 255.0)+" mul ";
			colorDesc += "exch "+FToStr(static_cast<double>(kc) / 255.0)+" mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+QString::number(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream\nendobj\n");
	}
	QString tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += "/"+TRes+" gs\n";
	tmp += "/Pattern cs\n";
	tmp += "/Pattern"+QString::number(patObject)+" scn\n";
	output = tmp;
	return true;
}

bool PDFLibCore::PDF_DiamondGradientFill(QString& output, PageItem *c)
{
	QList<double> StopVec;
	QList<double> TransVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	VGradient gradient;
	bool spotMode = false;
	bool transparencyFound = false;
	gradient = c->fill_gradient;
	QList<VColorStop*> cstops = gradient.colorStops();
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	colorNames.clear();
	colorShades.clear();
	for (uint cst = 0; cst < gradient.Stops(); ++cst)
	{
		double actualStop = cstops.at(cst)->rampPoint;
		StopVec.append(actualStop);
		colorNames.append(cstops.at(cst)->name);
		colorShades.append(cstops.at(cst)->shade);
		TransVec.append(cstops.at(cst)->opacity);
		if (cstops.at(cst)->opacity != 1.0)
			transparencyFound = true;
		if (spotMap.contains(cstops.at(cst)->name))
		{
			if (!spotColorSet.contains(cstops.at(cst)->name))
				spotColorSet.append(cstops.at(cst)->name);
		}
		Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
	}
	QPointF cP = QPointF(c->GrControl5.x(), -c->GrControl5.y());
	QLineF edge1 = QLineF(cP, QPointF(c->GrControl1.x(), -c->GrControl1.y()));
	QLineF edge2 = QLineF(cP, QPointF(c->GrControl2.x(), -c->GrControl2.y()));
	QLineF edge3 = QLineF(cP, QPointF(c->GrControl3.x(), -c->GrControl3.y()));
	QLineF edge4 = QLineF(cP, QPointF(c->GrControl4.x(), -c->GrControl4.y()));
	QString TRes("");
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		uint shadeObjectT = newObject();
		StartObj(shadeObjectT);
		PutDoc("<<\n");
		PutDoc("/ShadingType 6\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/BitsPerCoordinate 32\n");
		PutDoc("/BitsPerComponent 16\n");
		PutDoc("/BitsPerFlag 8\n");
		QByteArray vertStreamT;
		QDataStream vst(&vertStreamT, QIODevice::WriteOnly);
		vst.setByteOrder(QDataStream::BigEndian);
		quint8 flg = 0;
		for (uint offset = 1; offset < c->fill_gradient.Stops(); ++offset)
		{
			QLineF e1 = edge1;
			QLineF e1s = edge1;
			QLineF e2 = edge2;
			QLineF e2s = edge2;
			QLineF e3 = edge3;
			QLineF e3s = edge3;
			QLineF e4 = edge4;
			QLineF e4s = edge4;
			e1.setLength(edge1.length() * StopVec[ offset ]);
			e2.setLength(edge2.length() * StopVec[ offset ]);
			e3.setLength(edge3.length() * StopVec[ offset ]);
			e4.setLength(edge4.length() * StopVec[ offset ]);
			e1s.setLength(edge1.length() * StopVec[ offset - 1 ]);
			e2s.setLength(edge2.length() * StopVec[ offset - 1 ]);
			e3s.setLength(edge3.length() * StopVec[ offset - 1 ]);
			e4s.setLength(edge4.length() * StopVec[ offset - 1 ]);
			if (offset == 1)
			{
				vst << flg;
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode16dVal(TransVec[0]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[0]);
				vst << flg;
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode16dVal(TransVec[0]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[1]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[1]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[1]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[1]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[0]);
			}
			else
			{
				vst << flg;
				vst << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
				vst << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
				vst << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset-1]);
				vst << flg;
				vst << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
				vst << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
				vst << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
				vst << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
				vst << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
				vst << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
				vst << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset-1]);
			}
		}
		PutDoc("/Decode [-40000 40000 -40000 40000 0 1]\n");
		QString dat = "";
		for (int vd = 0; vd < vertStreamT.count(); vd++)
		{
			dat += vertStreamT[vd];
		}
		if (Options.Compress)
			dat = CompressStr(&dat);
		PutDoc("/Length "+QString::number(dat.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(dat, shadeObjectT)+"\nendstream\nendobj\n");
		uint patObject = newObject();
		StartObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Shading "+QString::number(shadeObjectT)+" 0 R\n");
		PutDoc(">>\nendobj\n");
		Patterns.insert("Pattern"+QString::number(patObject), patObject);
		uint formObject = newObject();
		StartObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = c->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(c->width()+lw)+" "+FToStr(-(c->height()+lw))+" ]\n");
		PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		if (Patterns.count() != 0)
		{
			PutDoc("/Pattern << \n");
			QMap<QString,int>::Iterator it3p;
			for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
				PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
			PutDoc(">>\n");
		}
		PutDoc(">>\n");
		QString stre = "q\n"+SetClipPath(c)+"h\n";
		stre += FToStr(fabs(c->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+QString::number(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressStr(&stre);
		PutDoc("/Length "+QString::number(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream\nendobj\n");
		Seite.XObjects[ResNam+QString::number(ResCount)] = formObject;
		ResCount++;
		QString GXName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+QString::number(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	QString entx = "";
	uint spotObject = 0;
	uint shadeObject = newObject();
	StartObj(shadeObject);
	PutDoc("<<\n");
	PutDoc("/ShadingType 6\n");
	if (Options.UseRGB)
	{
		PutDoc("/ColorSpace /DeviceRGB\n");
		entx = "0 1 0 1 0 1";
	}
	else if (Options.isGrayscale)
	{
		PutDoc("/ColorSpace /DeviceGray\n");
		entx = "0 1";
	}
	else if ((doc.HasCMS) && (Options.UseProfiles))
	{
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
		entx = "0 1 0 1 0 1";
	}
	else
	{
		entx = "0 1 0 1 0 1 0 1";
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				entx += " 0 1";
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(QString::number(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/BitsPerCoordinate 32\n");
	PutDoc("/BitsPerComponent 16\n");
	PutDoc("/BitsPerFlag 8\n");
	QByteArray vertStream;
	QDataStream vs(&vertStream, QIODevice::WriteOnly);
	vs.setByteOrder(QDataStream::BigEndian);
	quint8 flg = 0;
	for (uint offset = 1; offset < c->fill_gradient.Stops(); ++offset)
	{
		QLineF e1 = edge1;
		QLineF e1s = edge1;
		QLineF e2 = edge2;
		QLineF e2s = edge2;
		QLineF e3 = edge3;
		QLineF e3s = edge3;
		QLineF e4 = edge4;
		QLineF e4s = edge4;
		e1.setLength(edge1.length() * StopVec[ offset ]);
		e2.setLength(edge2.length() * StopVec[ offset ]);
		e3.setLength(edge3.length() * StopVec[ offset ]);
		e4.setLength(edge4.length() * StopVec[ offset ]);
		e1s.setLength(edge1.length() * StopVec[ offset - 1 ]);
		e2s.setLength(edge2.length() * StopVec[ offset - 1 ]);
		e3s.setLength(edge3.length() * StopVec[ offset - 1 ]);
		e4s.setLength(edge4.length() * StopVec[ offset - 1 ]);
		if (offset == 1)
		{
			vs << flg;
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
		}
		else
		{
			vs << flg;
			vs << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
			vs << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
			vs << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
			vs << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
			vs << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
			vs << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
			vs << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
		}
	}
	PutDoc("/Decode [-40000 40000 -40000 40000 "+entx+"]\n");
	QString dat = "";
	for (int vd = 0; vd < vertStream.count(); vd++)
	{
		dat += vertStream[vd];
	}
	if (Options.Compress)
		dat = CompressStr(&dat);
	PutDoc("/Length "+QString::number(dat.length())+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(dat, shadeObject)+"\nendstream\nendobj\n");
	uint patObject = newObject();
	StartObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	QTransform mpa;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(c->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - c->gYpos);
			mpa.rotate(-c->rotation());
		}
		else
		{
			mpa.translate(c->xPos() - ActPageP->xOffset(), ActPageP->height() - (c->yPos() - ActPageP->yOffset()));
			mpa.rotate(-c->rotation());
		}
	}
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading "+QString::number(shadeObject)+" 0 R\n");
	PutDoc(">>\nendobj\n");
	Patterns.insert("Pattern"+QString::number(patObject), patObject);
	if (spotMode)
	{
		QString colorDesc;
		StartObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			int cc = 0;
			int mc = 0;
			int yc = 0;
			int kc = 0;
			CMYKColor cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			else
				colorDesc += QString::number(sc*4 + 1)+" -1 roll dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(mc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(yc) / 255.0)+" mul ";
			colorDesc += "exch "+FToStr(static_cast<double>(kc) / 255.0)+" mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+QString::number(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream\nendobj\n");
	}
	QString tmp;
	tmp += "q\n";
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		QString ShName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/ca "+FToStr(TransVec[TransVec.count()-1])+"\n/SMask /None\n/AIS false\n/OPM 1\n");
		tmp += "/"+ShName+" gs\n";
	}
	tmp += putColor(colorNames[colorNames.count()-1], colorShades[colorShades.count()-1], true);
	tmp += SetClipPath(c);
	tmp += "h\n";
	tmp += FToStr(c->GrControl1.x())+" "+FToStr(-c->GrControl1.y())+" m\n";
	tmp += FToStr(c->GrControl2.x())+" "+FToStr(-c->GrControl2.y())+" l\n";
	tmp += FToStr(c->GrControl3.x())+" "+FToStr(-c->GrControl3.y())+" l\n";
	tmp += FToStr(c->GrControl4.x())+" "+FToStr(-c->GrControl4.y())+" l\n";
	tmp += "h\nf*\n";
	tmp += "Q\n";
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += "/"+TRes+" gs\n";
	tmp += "/Pattern cs\n";
	tmp += "/Pattern"+QString::number(patObject)+" scn\n";
	output = tmp;
	return true;

}

bool PDFLibCore::PDF_TensorGradientFill(QString& output, PageItem *c)
{
	QList<int> colorShades;
	QStringList spotColorSet;
	QStringList colorNames;
	QStringList Gcolors;
	bool spotMode = false;
	bool transparencyFound = false;
	colorNames << c->GrColorP4 << c->GrColorP1 << c->GrColorP2 << c->GrColorP3;
	colorShades << c->GrCol4Shade << c->GrCol1Shade << c->GrCol2Shade << c->GrCol3Shade;
	for (int cst = 0; cst < colorNames.count(); ++cst)
	{
		if (cst == 0)
		{
			if (c->GrCol4transp != 1.0)
				transparencyFound = true;
		}
		else if (cst == 1)
		{
			if (c->GrCol1transp != 1.0)
				transparencyFound = true;
		}
		else if (cst == 2)
		{
			if (c->GrCol2transp != 1.0)
				transparencyFound = true;
		}
		else if (cst == 3)
		{
			if (c->GrCol3transp != 1.0)
				transparencyFound = true;
		}
		if (spotMap.contains(colorNames.at(cst)))
		{
			if (!spotColorSet.contains(colorNames.at(cst)))
				spotColorSet.append(colorNames.at(cst));
		}
		Gcolors.append(SetGradientColor(colorNames.at(cst), colorShades[cst]));
	}
	QString TRes("");
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		uint shadeObjectT = newObject();
		StartObj(shadeObjectT);
		PutDoc("<<\n");
		PutDoc("/ShadingType 7\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/BitsPerCoordinate 32\n");
		PutDoc("/BitsPerComponent 16\n");
		PutDoc("/BitsPerFlag 8\n");
		QByteArray vertStreamT;
		QDataStream vst(&vertStreamT, QIODevice::WriteOnly);
		vst.setByteOrder(QDataStream::BigEndian);
		quint8 flg = 0;
		vst << flg;
		quint32 val = 0;
		vst << encode32dVal(val) << encode32dVal(-c->height());
		vst << encode32dVal(val) << encode32dVal(-c->height());
		vst << encode32dVal(val) << encode32dVal(val);
		vst << encode32dVal(val) << encode32dVal(val);
		vst << encode32dVal(val) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(-c->height());
		vst << encode32dVal(c->width()) << encode32dVal(-c->height());
		vst << encode32dVal(c->width()) << encode32dVal(-c->height());
		vst << encode32dVal(val) << encode32dVal(-c->height());
		vst << encode32dVal(c->GrControl1.x()) << encode32dVal(-c->GrControl1.y());
		vst << encode32dVal(c->GrControl4.x()) << encode32dVal(-c->GrControl4.y());
		vst << encode32dVal(c->GrControl3.x()) << encode32dVal(-c->GrControl3.y());
		vst << encode32dVal(c->GrControl2.x()) << encode32dVal(-c->GrControl2.y());
		vst << encode16dVal(c->GrCol4transp) << encode16dVal(c->GrCol1transp) << encode16dVal(c->GrCol2transp) << encode16dVal(c->GrCol3transp);
		PutDoc("/Decode [-40000 40000 -40000 40000 0 1]\n");
		QString dat = "";
		for (int vd = 0; vd < vertStreamT.count(); vd++)
		{
			dat += vertStreamT[vd];
		}
		if (Options.Compress)
			dat = CompressStr(&dat);
		PutDoc("/Length "+QString::number(dat.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(dat, shadeObjectT)+"\nendstream\nendobj\n");
		uint patObject = newObject();
		StartObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Shading "+QString::number(shadeObjectT)+" 0 R\n");
		PutDoc(">>\nendobj\n");
		Patterns.insert("Pattern"+QString::number(patObject), patObject);
		uint formObject = newObject();
		StartObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = c->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(c->width()+lw)+" "+FToStr(-(c->height()+lw))+" ]\n");
		PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		if (Patterns.count() != 0)
		{
			PutDoc("/Pattern << \n");
			QMap<QString,int>::Iterator it3p;
			for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
				PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
			PutDoc(">>\n");
		}
		PutDoc(">>\n");
		QString stre = "q\n"+SetClipPath(c)+"h\n";
		stre += FToStr(fabs(c->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+QString::number(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressStr(&stre);
		PutDoc("/Length "+QString::number(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream\nendobj\n");
		Seite.XObjects[ResNam+QString::number(ResCount)] = formObject;
		ResCount++;
		QString GXName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+QString::number(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	QString entx = "";
	uint spotObject = 0;
	uint shadeObject = newObject();
	StartObj(shadeObject);
	PutDoc("<<\n");
	PutDoc("/ShadingType 7\n");
	if (Options.UseRGB)
	{
		PutDoc("/ColorSpace /DeviceRGB\n");
		entx = "0 1 0 1 0 1";
	}
	else if (Options.isGrayscale)
	{
		PutDoc("/ColorSpace /DeviceGray\n");
		entx = "0 1";
	}
	else if ((doc.HasCMS) && (Options.UseProfiles))
	{
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
		entx = "0 1 0 1 0 1";
	}
	else
	{
		entx = "0 1 0 1 0 1 0 1";
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				entx += " 0 1";
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(QString::number(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/BitsPerCoordinate 32\n");
	PutDoc("/BitsPerComponent 16\n");
	PutDoc("/BitsPerFlag 8\n");
	QByteArray vertStream;
	QDataStream vs(&vertStream, QIODevice::WriteOnly);
	vs.setByteOrder(QDataStream::BigEndian);
	quint8 flg = 0;
	vs << flg;
	quint32 val = 0;
	vs << encode32dVal(val) << encode32dVal(-c->height());
	vs << encode32dVal(val) << encode32dVal(-c->height());
	vs << encode32dVal(val) << encode32dVal(val);
	vs << encode32dVal(val) << encode32dVal(val);
	vs << encode32dVal(val) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(-c->height());
	vs << encode32dVal(c->width()) << encode32dVal(-c->height());
	vs << encode32dVal(c->width()) << encode32dVal(-c->height());
	vs << encode32dVal(val) << encode32dVal(-c->height());
	vs << encode32dVal(c->GrControl1.x()) << encode32dVal(-c->GrControl1.y());
	vs << encode32dVal(c->GrControl4.x()) << encode32dVal(-c->GrControl4.y());
	vs << encode32dVal(c->GrControl3.x()) << encode32dVal(-c->GrControl3.y());
	vs << encode32dVal(c->GrControl2.x()) << encode32dVal(-c->GrControl2.y());
	for (int cc = 0; cc < colorNames.count(); cc++)
	{
		if (spotMode)
		{
			if (spotColorSet.contains(colorNames.at(cc)))
			{
				vs << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc))
						vs << encode16dVal(colorShades[cc] / 100.0);
					else
						vs << encode16dVal(0.0);
				}
			}
			else
			{
				QStringList gcol = Gcolors[cc].split(" ");
				for (int gcs = 0; gcs < gcol.count(); gcs++)
				{
					vs << encode16dVal(gcol[gcs].toDouble());
				}
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					vs << encode16dVal(0.0);
				}
			}
		}
		else
		{
			QStringList gcol = Gcolors[cc].split(" ");
			for (int gcs = 0; gcs < gcol.count(); gcs++)
			{
				vs << encode16dVal(gcol[gcs].toDouble());
			}
		}
	}
	PutDoc("/Decode [-40000 40000 -40000 40000 "+entx+"]\n");
	QString dat = "";
	for (int vd = 0; vd < vertStream.count(); vd++)
	{
		dat += vertStream[vd];
	}
	if (Options.Compress)
		dat = CompressStr(&dat);
	PutDoc("/Length "+QString::number(dat.length())+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(dat, shadeObject)+"\nendstream\nendobj\n");
	uint patObject = newObject();
	StartObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	QTransform mpa;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(c->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - c->gYpos);
			mpa.rotate(-c->rotation());
		}
		else
		{
			mpa.translate(c->xPos() - ActPageP->xOffset(), ActPageP->height() - (c->yPos() - ActPageP->yOffset()));
			mpa.rotate(-c->rotation());
		}
	}
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading "+QString::number(shadeObject)+" 0 R\n");
	PutDoc(">>\nendobj\n");
	Patterns.insert("Pattern"+QString::number(patObject), patObject);
	if (spotMode)
	{
		QString colorDesc;
		StartObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			int cc = 0;
			int mc = 0;
			int yc = 0;
			int kc = 0;
			CMYKColor cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			else
				colorDesc += QString::number(sc*4 + 1)+" -1 roll dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(mc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(yc) / 255.0)+" mul ";
			colorDesc += "exch "+FToStr(static_cast<double>(kc) / 255.0)+" mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+QString::number(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream\nendobj\n");
	}
	QString tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += "/"+TRes+" gs\n";
	tmp += "/Pattern cs\n";
	tmp += "/Pattern"+QString::number(patObject)+" scn\n";
	output = tmp;
	return true;
}

bool PDFLibCore::PDF_GradientFillStroke(QString& output, PageItem *currItem, bool stroke, bool forArrow)
{
	QList<double> StopVec;
	QList<double> TransVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	VGradient gradient;
	double StartX, StartY, EndX, EndY, FocalX, FocalY, Gscale, Gskew;
	int GType;
	if (stroke)
	{
		GType = currItem->GrTypeStroke;
		StartX = currItem->GrStrokeStartX;
		StartY = currItem->GrStrokeStartY;
		EndX = currItem->GrStrokeEndX;
		EndY = currItem->GrStrokeEndY;
		FocalX = currItem->GrStrokeFocalX;
		FocalY = currItem->GrStrokeFocalY;
		Gscale = currItem->GrStrokeScale;
		Gskew = currItem->GrStrokeSkew;
		gradient = currItem->stroke_gradient;
	}
	else
	{
		GType = currItem->GrType;
		if (GType == 9)
			return PDF_TensorGradientFill(output, currItem);
		else if (GType == 10)
			return PDF_DiamondGradientFill(output, currItem);
		else if (GType == 11)
			return PDF_MeshGradientFill(output, currItem);
		StartX = currItem->GrStartX;
		StartY = currItem->GrStartY;
		EndX = currItem->GrEndX;
		EndY = currItem->GrEndY;
		FocalX = currItem->GrFocalX;
		FocalY = currItem->GrFocalY;
		Gscale = currItem->GrScale;
		Gskew = currItem->GrSkew;
		gradient = currItem->fill_gradient;
	}
	QList<VColorStop*> cstops = gradient.colorStops();
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	colorNames.clear();
	colorShades.clear();
	QTransform mpa;
	bool spotMode = false;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(currItem->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - currItem->gYpos);
			mpa.rotate(-currItem->rotation());
		}
		else
		{
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
		}
	}
	if (Gskew == 90)
		Gskew = 1;
	else if (Gskew == 180)
		Gskew = 0;
	else if (Gskew == 270)
		Gskew = -1;
	else if (Gskew == 390)
		Gskew = 0;
	else
		Gskew = tan(M_PI / 180.0 * Gskew);
	if (GType == 6)
	{
		mpa.translate(StartX, -StartY);
		mpa.shear(Gskew, 0);
		mpa.translate(-StartX, StartY);
	}
	else
	{
		double rotEnd = xy2Deg(EndX - StartX, EndY - StartY);
		mpa.translate(StartX, -StartY);
		mpa.rotate(-rotEnd);
		mpa.shear(-Gskew, 0);
		mpa.translate(0, -StartY * (1.0 - Gscale));
		mpa.translate(-StartX, StartY);
		mpa.scale(1, Gscale);
	}
	double lastStop = -1.0;
	double actualStop = 0.0;
	bool   isFirst = true, transparencyFound = false;
	for (uint cst = 0; cst < gradient.Stops(); ++cst)
	{
		actualStop = cstops.at(cst)->rampPoint;
		if ((actualStop == lastStop) && (!isFirst))
			continue;
		if ((isFirst) && (actualStop != 0.0))
		{
			StopVec.append(0.0);
			colorNames.append(cstops.at(cst)->name);
			colorShades.append(cstops.at(cst)->shade);
			TransVec.append(cstops.at(cst)->opacity);
			Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
		}
		isFirst = false;
		StopVec.append(actualStop);
		colorNames.append(cstops.at(cst)->name);
		colorShades.append(cstops.at(cst)->shade);
		TransVec.append(cstops.at(cst)->opacity);
		if (cstops.at(cst)->opacity != 1.0)
			transparencyFound = true;
		if (spotMap.contains(cstops.at(cst)->name))
		{
			if (!spotColorSet.contains(cstops.at(cst)->name))
				spotColorSet.append(cstops.at(cst)->name);
		}
		Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
		if ((cst == gradient.Stops()-1) && (actualStop < 1.0))
		{
			StopVec.append(1.0);
			colorNames.append(cstops.at(cst)->name);
			colorShades.append(cstops.at(cst)->shade);
			TransVec.append(cstops.at(cst)->opacity);
			Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
		}
		lastStop = actualStop;
	}
	QString TRes("");
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		QTransform mpM;
		mpM.rotate(-currItem->rotation());
		if (GType == 6)
		{
			mpM.translate(StartX, -StartY);
			mpM.shear(Gskew, 0);
			mpM.translate(-StartX, StartY);
		}
		else
		{
			double rotEnd = xy2Deg(EndX - StartX, EndY - StartY);
			mpM.translate(StartX, -StartY);
			mpM.rotate(-rotEnd);
			mpM.shear(-Gskew, 0);
			mpM.translate(0, -StartY * (1.0 - Gscale));
			mpM.translate(-StartX, StartY);
			mpM.scale(1, Gscale);
		}
		uint patObject = newObject();
		StartObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Matrix ["+FToStr(mpM.m11())+" "+FToStr(mpM.m12())+" "+FToStr(mpM.m21())+" "+FToStr(mpM.m22())+" "+FToStr(mpM.dx())+" "+FToStr(mpM.dy())+"]\n");
		PutDoc("/Shading\n");
		PutDoc("<<\n");
		if (GType == 6)
			PutDoc("/ShadingType 2\n");
		else
			PutDoc("/ShadingType 3\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/Extend [true true]\n");
		if (GType == 6)
			PutDoc("/Coords ["+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(EndX)+" "+FToStr(-EndY)+"]\n");
		else
			PutDoc("/Coords ["+FToStr(FocalX)+" "+FToStr(-FocalY)+" 0.0 "+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2)))+"]\n");
		PutDoc("/Function\n");
		PutDoc("<<\n");
		PutDoc("/FunctionType 3\n");
		PutDoc("/Domain [0 1]\n");
		if (StopVec.count() > 2)
		{
			PutDoc("/Bounds [");
			QString bctx = "";
			for (int bc = 1; bc < StopVec.count() - 1; bc++)
			{
				bctx += FToStr(StopVec.at(bc))+" ";
			}
			PutDoc(bctx.trimmed()+"]\n");
		}
		else
			PutDoc("/Bounds []\n");
		QString entx = "";
		PutDoc("/Functions\n");
		PutDoc("[\n");
		for (int cc = 0; cc < TransVec.count() - 1; cc++)
		{
			entx += "0 1 ";
			PutDoc("<<\n");
			PutDoc("/FunctionType 2\n");
			PutDoc("/Domain [0 1]\n");
			PutDoc("/C0 ["+FToStr(TransVec.at(cc))+"]\n");
			PutDoc("/C1 ["+FToStr(TransVec.at(cc+1))+"]\n");
			PutDoc("/N 1\n");
			PutDoc(">>\n");
		}
		PutDoc("]\n");
		PutDoc("/Encode ["+entx.trimmed()+"]\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		Patterns.insert("Pattern"+QString::number(patObject), patObject);
		uint formObject = newObject();
		StartObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = currItem->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(currItem->width()+lw)+" "+FToStr(-(currItem->height()+lw))+" ]\n");
		PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		if (Patterns.count() != 0)
		{
			PutDoc("/Pattern << \n");
			QMap<QString,int>::Iterator it3p;
			for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
				PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
			PutDoc(">>\n");
		}
		PutDoc(">>\n");
		QString stre = "q\n"+SetClipPath(currItem)+"h\n";
		stre += FToStr(fabs(currItem->lineWidth()))+" w\n";
		if ((forArrow) || (!stroke))
		{
			stre += "/Pattern cs\n";
			stre += "/Pattern"+QString::number(patObject)+" scn\nf*\n";
		}
		else
		{
			stre += "/Pattern CS\n";
			stre += "/Pattern"+QString::number(patObject)+" SCN\nS\n";
		}
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressStr(&stre);
		PutDoc("/Length "+QString::number(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream\nendobj\n");
		Seite.XObjects[ResNam+QString::number(ResCount)] = formObject;
		ResCount++;
		QString GXName = ResNam+QString::number(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+QString::number(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	uint patObject = newObject();
	uint spotObject = 0;
	StartObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading\n");
	PutDoc("<<\n");
	if (GType == 6)
		PutDoc("/ShadingType 2\n");
	else
		PutDoc("/ShadingType 3\n");
	if (Options.UseRGB)
		PutDoc("/ColorSpace /DeviceRGB\n");
	else if (Options.isGrayscale)
		PutDoc("/ColorSpace /DeviceGray\n");
	else if ((doc.HasCMS) && (Options.UseProfiles))
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
	else
	{
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(QString::number(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/Extend [true true]\n");
	if (GType == 6)
		PutDoc("/Coords ["+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(EndX)+" "+FToStr(-EndY)+"]\n");
	else
		PutDoc("/Coords ["+FToStr(FocalX)+" "+FToStr(-FocalY)+" 0.0 "+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2)))+"]\n");
	PutDoc("/Function\n");
	PutDoc("<<\n");
	PutDoc("/FunctionType 3\n");
	PutDoc("/Domain [0 1]\n");
	if (StopVec.count() > 2)
	{
		PutDoc("/Bounds [");
		QString bctx = "";
		for (int bc = 1; bc < StopVec.count() - 1; bc++)
		{
			bctx += FToStr(StopVec.at(bc))+" ";
		}
		PutDoc(bctx.trimmed()+"]\n");
	}
	else
		PutDoc("/Bounds []\n");
	QString entx = "";
	PutDoc("/Functions\n");
	PutDoc("[\n");
	for (int cc = 0; cc < colorNames.count() - 1; cc++)
	{
		entx += "0 1 ";
		PutDoc("<<\n");
		PutDoc("/FunctionType 2\n");
		PutDoc("/Domain [0 1]\n");
		if (spotMode)
		{
			if (spotColorSet.contains(colorNames.at(cc)))
			{
				PutDoc("/C0 [0 0 0 0");
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc))
						PutDoc(" "+FToStr(colorShades[cc] / 100.0));
					else
						PutDoc(" 0");
				}
			}
			else
			{
				PutDoc("/C0 ["+Gcolors[cc]);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					PutDoc(" 0");
				}
			}
			PutDoc("]\n");
			if (spotColorSet.contains(colorNames.at(cc+1)))
			{
				PutDoc("/C1 [0 0 0 0");
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc+1))
						PutDoc(" "+FToStr(colorShades[cc+1] / 100.0));
					else
						PutDoc(" 0");
				}
			}
			else
			{
				PutDoc("/C1 ["+Gcolors[cc+1]);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					PutDoc(" 0");
				}
			}
			PutDoc("]\n");
		}
		else
		{
			PutDoc("/C0 ["+Gcolors[cc]+"]\n");
			PutDoc("/C1 ["+Gcolors[cc+1]+"]\n");
		}
		PutDoc("/N 1\n");
		PutDoc(">>\n");
	}
	PutDoc("]\n");
	PutDoc("/Encode ["+entx.trimmed()+"]\n");
	PutDoc(">>\n");
	PutDoc(">>\n");
	PutDoc(">>\n");
	PutDoc("endobj\n");
	Patterns.insert("Pattern"+QString::number(patObject), patObject);
	if (spotMode)
	{
		QString colorDesc;
		StartObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			int cc = 0;
			int mc = 0;
			int yc = 0;
			int kc = 0;
			CMYKColor cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			else
				colorDesc += QString::number(sc*4 + 1)+" -1 roll dup "+FToStr(static_cast<double>(cc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(mc) / 255.0)+" mul ";
			colorDesc += "exch dup "+FToStr(static_cast<double>(yc) / 255.0)+" mul ";
			colorDesc += "exch "+FToStr(static_cast<double>(kc) / 255.0)+" mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+QString::number(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream\nendobj\n");
	}
	QString tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += "/"+TRes+" gs\n";
	if ((forArrow) || (!stroke))
	{
		tmp += "/Pattern cs\n";
		tmp += "/Pattern"+QString::number(patObject)+" scn\n";
	}
	else
	{
		tmp += "/Pattern CS\n";
		tmp += "/Pattern"+QString::number(patObject)+" SCN\n";
	}
	output = tmp;
	return true;
}

#ifdef HAVE_OSG
bool PDFLibCore::PDF_3DAnnotation(PageItem *ite, uint)
{
	QStringList lightModes;
	lightModes << "None" << "Headlamp" << "White" << "Day" << "Night" << "Hard" << "Primary";
	lightModes << "Blue" << "Red" << "Cube" << "CAD" << "Artwork";
	QStringList renderModes;
	renderModes << "Solid" << "SolidWireframe" << "Transparent" << "TransparentWireframe";
	renderModes << "BoundingBox" << "TransparentBoundingBox" << "TransparentBoundingBoxOutline";
	renderModes << "Wireframe" << "ShadedWireframe" << "HiddenWireframe" << "Vertices";
	renderModes << "ShadedVertices" << "Illustration" << "SolidOutline" << "ShadedIllustration";
	QTemporaryFile *tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_osg_XXXXXX.prc");
	tempImageFile->open();
	QString imgName = getLongPathName(tempImageFile->fileName());
	tempImageFile->close();
	PRCExporter *exprc = new PRCExporter();
	exprc->convertFile(imgName, ite->asOSGFrame());
	delete exprc;
	double x = ite->xPos() - ActPageP->xOffset();
	double y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
	double x2 = x+ite->width();
	double y2 = y-ite->height();
	PageItem_OSGFrame *osgframe = ite->asOSGFrame();
	QList<uint> viewList;
	uint viewObj = 0;
	QHash<QString, PageItem_OSGFrame::viewDefinition>::iterator itv;
	for (itv = osgframe->viewMap.begin(); itv != osgframe->viewMap.end(); ++itv)
	{
		uint viewObjL = newObject();
		viewList.append(viewObjL);
		if (osgframe->currentView == itv.key())
			viewObj = viewObjL;
		StartObj(viewObjL);
		PutDoc("<<\n/Type /3DView\n");
		PutDoc("/MS /M\n");
		PutDoc("/C2W ["+osgframe->getPDFMatrix(itv.key())+" ]\n");
		PutDoc("/LS << /Subtype /" + lightModes[itv.value().illumination] + " >>\n");
		PutDoc("/RM\n<<\n");
		PutDoc("/Subtype /" + renderModes[itv.value().rendermode] + "\n");
		PutDoc("/AC [/DeviceRGB " + FToStr(itv.value().colorAC.redF()) + " " + FToStr(itv.value().colorAC.greenF()) + " " + FToStr(itv.value().colorAC.blueF()) + "]\n");
		PutDoc("/FC [/DeviceRGB " + FToStr(itv.value().colorFC.redF()) + " " + FToStr(itv.value().colorFC.greenF()) + " " + FToStr(itv.value().colorFC.blueF()) + "]\n");
		PutDoc("/O " + FToStr(1.0 - itv.value().addedTransparency) + "\n");
		PutDoc(">>\n");
		PutDoc("/P << /FOV "+FToStr(itv.value().angleFOV)+" /PS /Min /Subtype /P >>\n");
		PutDoc("/CO "+FToStr(osgframe->distanceToObj)+"\n");
		if (ite->fillColor() != CommonStrings::None)
		{
			PutDoc("/BG << /Type /3DBG\n");
			PutDoc("/C [ "+SetColor(ite->fillColor(), ite->fillShade())+" ]\n>>\n");
		}
		PutDoc("/XN ("+PDFEncode(itv.key())+")\n");
		PutDoc("/IN ("+PDFEncode(itv.key())+")\n");
		PutDoc(">>\nendobj\n");
	}
	uint appearanceObj = newObject();
	StartObj(appearanceObj);
	PutDoc("<<\n/Type /3D\n");
	PutDoc("/Subtype /PRC\n");
	PutDoc("/VA [");
	for (int vl = 0; vl < viewList.count(); vl++)
	{
		PutDoc(QString::number(viewList.at(vl))+" 0 R ");
	}
	PutDoc("]\n");
	QByteArray dataP;
	loadRawBytes(imgName, dataP);
	if ((Options.CompressMethod != PDFOptions::Compression_None) && Options.Compress)
	{
		QByteArray compData = CompressArray(dataP);
		if (compData.size() > 0)
		{
			PutDoc("/Filter /FlateDecode\n");
			dataP = compData;
		}
	}
	PutDoc("/Length "+QString::number(dataP.size()+1)+"\n");
	PutDoc(">>\nstream\n");
	EncodeArrayToStream(dataP, appearanceObj);
	PutDoc("\nendstream\nendobj\n");
	uint annotationObj = newObject();
	StartObj(annotationObj);
	Seite.AObjects.append(annotationObj);
	PutDoc("<<\n/Type /Annot\n");
	PutDoc("/Subtype /3D\n");
	PutDoc("/F 4\n");
	PutDoc("/3DD "+QString::number(appearanceObj)+" 0 R\n");
	PutDoc("/3DV "+QString::number(viewObj)+" 0 R\n");
	PutDoc("/3DA <<\n/A /PV\n/TB true\n/NP true\n>>\n");
	PutDoc("/Rect [ "+FToStr(x+bleedDisplacementX)+" "+FToStr(y2+bleedDisplacementY)+" "+FToStr(x2+bleedDisplacementX)+" "+FToStr(y+bleedDisplacementY)+" ]\n");
	PutDoc(">>\nendobj\n");
	delete tempImageFile;
	return true;
}
#endif

bool PDFLibCore::PDF_Annotation(PageItem *ite, uint)
{
	ScImage img;
	ScImage img2;
	ScImage img3;
	QMap<int, QString> ind2PDFabr;
	static const QString bifonts[] = {"/Courier", "/Courier-Bold", "/Courier-Oblique", "/Courier-BoldOblique",
												"/Helvetica", "/Helvetica-Bold", "/Helvetica-Oblique", "/Helvetica-BoldOblique",
												"/Times-Roman", "/Times-Bold", "/Times-Italic", "/Times-BoldItalic",
												"/ZapfDingbats", "/Symbol"};
	static const size_t ar = sizeof(bifonts) / sizeof(*bifonts);
	for (uint a = 0; a < ar; ++a)
		ind2PDFabr[a] = bifonts[a];
	double x = ite->xPos() - ActPageP->xOffset();
	double y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
	double x2 = x+ite->width();
	double y2 = y-ite->height();
	QString bm(""), bmUtf16("");
	QString cc;
	QFileInfo fiBase(Spool.fileName());
	QString baseDir = fiBase.absolutePath();
	if (!((ite->itemText.length() == 1) && (ite->itemText.text(0, 1) == QChar(13))))
	{
		// #6823 EncStringUTF16() perform the string encoding by its own
		// via EncodeUTF16() so bmUtf16 must not encoded before
		for (uint d = 0; d < static_cast<uint>(ite->itemText.length()); ++d)
		{
			cc = ite->itemText.text(d, 1);
			bmUtf16 += cc;
			if ((cc == "(") || (cc == ")") || (cc == "\\"))
				bm += "\\";
			if (cc == QChar(13))
				cc = "\\r";
			bm += cc;
		}
	}
	QString anTitle = ite->itemName().replace(".", "_" );
	QStringList bmst = bm.split("\\r", QString::SkipEmptyParts);
	QStringList bmstUtf16 = bmUtf16.split(QChar(13), QString::SkipEmptyParts);
	const QString m[] = {"4", "5", "F", "l", "H", "n"};
	QString ct(m[ite->annotation().ChkStil()]);
	uint annotationObj = newObject();
	uint appearanceObj = 0;
	uint icon1Obj = 0;
	uint icon2Obj = 0;
	uint icon3Obj = 0;
	uint actionObj = 0;
	if ((ite->annotation().Type() > 1) && ((ite->annotation().ActionType() == 1) || (ite->annotation().AAact())) && (!ite->annotation().Action().isEmpty()))
		actionObj = WritePDFString(ite->annotation().Action());
	uint AActionObj = writeActions(ite->annotation(), annotationObj);
	StartObj(annotationObj);
	Seite.AObjects.append(annotationObj);
	PutDoc("<<\n/Type /Annot\n");
	switch (ite->annotation().Type())
	{
		case 0:
		case 10:
			PutDoc("/Subtype /Text\n");
			PutDoc("/Contents "+EncStringUTF16("("+bmUtf16+")",annotationObj)+"\n");
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
				PutDoc("/A << /Type /Action /S /GoToR\n/F "+ EncString("("+Path2Relative(ite->annotation().Extern(), baseDir)+")",annotationObj)+"\n");
				PutDoc("/D ["+QString::number(ite->annotation().Ziel())+" /XYZ "+ite->annotation().Action()+"]\n>>\n");
			}
			if (ite->annotation().ActionType() == 8)
				PutDoc("/A << /Type /Action /S /URI\n/URI "+ EncString("("+ite->annotation().Extern()+")",annotationObj)+"\n>>\n");
			if (ite->annotation().ActionType() == 9)
			{
				PutDoc("/A << /Type /Action /S /GoToR\n/F "+ EncString("("+ite->annotation().Extern()+")",ObjCounter-1)+"\n");
				PutDoc("/D ["+QString::number(ite->annotation().Ziel())+" /XYZ "+ite->annotation().Action()+"]\n>>\n");
			}
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			Seite.FormObjects.append(annotationObj);
			PutDoc("/Subtype /Widget\n");
			PutDoc("/T "+EncString("("+anTitle+")",annotationObj)+"\n");
			if (!ite->annotation().ToolTip().isEmpty())
				PutDoc("/TU "+EncStringUTF16("("+PDFEncode(ite->annotation().ToolTip())+")",annotationObj)+"\n");
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
			QString cnx = "(";
			if (ite->annotation().Type() == 4)
				cnx += "/"+StdFonts["/ZapfDingbats"];
			else
			{
				if (Options.Version < PDFOptions::PDFVersion_14)
					cnx += "/"+StdFonts[ind2PDFabr[ite->annotation().Font()]];
				else
					cnx += UsedFontsF[ite->itemText.defaultStyle().charStyle().font().replacementName()];
//					cnx += UsedFontsP[ite->itemText.defaultStyle().charStyle().font().replacementName()]+"Form";
			}
			cnx += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf";
			if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
				cnx += " "+ putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
			if (ite->fillColor() != CommonStrings::None)
				cnx += " "+ putColor(ite->fillColor(), ite->fillShade(), false);
			cnx += ")";
			PutDoc("/DA "+EncString(cnx,annotationObj)+"\n");
			int flg = ite->annotation().Flag();
			if (Options.Version == PDFOptions::PDFVersion_13)
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
					PutDoc("/V " + EncStringUTF16("("+bmUtf16+")",annotationObj)+"\n");
					PutDoc("/DV "+ EncStringUTF16("("+bmUtf16+")",annotationObj)+"\n");
					PutDoc("/Q "+QString::number(qMin(ite->itemText.defaultStyle().alignment(), ParagraphStyle::Rightaligned))+"\n");
					appearanceObj = newObject();
					PutDoc("/AP << /N "+QString::number(appearanceObj)+" 0 R >>\n");
					if (ite->annotation().MaxChar() != -1)
						PutDoc("/MaxLen "+QString::number(ite->annotation().MaxChar())+"\n");
					break;
				case 4:
					PutDoc("/FT /Btn\n");
					PutDoc(ite->annotation().IsChk() ? "/V /Yes\n/DV /Yes\n/AS /Yes\n" :
								"/V /Off\n/DV /Off\n/AS /Off\n");
					appearanceObj = newObject();
					PutDoc("/AP << /N << /Yes "+QString::number(appearanceObj)+" 0 R >> >>\n");
					break;
				case 5:
				case 6:
					PutDoc("/FT /Ch\n/V (");
					if (bmst.count() > 0)
						PutDoc(bmst[0]);
					PutDoc(")\n/DV ");
					cnx = "(";
					if (bmst.count() > 0)
						cnx += bmstUtf16[0];
					cnx += ")";
					PutDoc(EncStringUTF16(cnx,annotationObj)+"\n");
					PutDoc("/Opt [ ");
					for (int bmc = 0; bmc < bmstUtf16.count(); ++bmc)
						PutDoc(EncStringUTF16("("+bmstUtf16[bmc]+")",annotationObj)+"\n");
					PutDoc("]\n");
					appearanceObj = newObject();
					PutDoc("/AP << /N "+QString::number(appearanceObj)+" 0 R >>\n");
					break;
			}
			PutDoc("/MK << ");
			if ((ite->annotation().Type() == 5) || (ite->annotation().Type() == 6))
			{
				PutDoc("/BG [ 1 1 1 ] ");
				if (ite->annotation().borderColor() != CommonStrings::None)
					PutDoc("/BC [ "+SetColor(ite->annotation().borderColor(), 100)+" ] ");
			}
				else
			{
				if (ite->fillColor() != CommonStrings::None)
					PutDoc("/BG [ "+SetColor(ite->fillColor(), ite->fillShade())+" ] ");
				if (ite->annotation().borderColor() != CommonStrings::None)
					PutDoc("/BC [ "+SetColor(ite->annotation().borderColor(), 100)+" ] ");
			}
			int IconOb = 0;
			switch (ite->annotation().Type())
			{
				case 2:
					PutDoc("/CA "+EncString("("+bm+")",annotationObj)+" ");
					if (!ite->annotation().RollOver().isEmpty())
						PutDoc("/RC "+ EncString("("+PDFEncode(ite->annotation().RollOver())+")",annotationObj)+" ");
					if (!ite->annotation().Down().isEmpty())
						PutDoc("/AC "+ EncString("("+PDFEncode(ite->annotation().Down())+")",annotationObj)+" ");
					if (ite->annotation().UseIcons())
					{
						if (!ite->Pfile.isEmpty())
						{
							IconOb += ite->pixm.hasAlpha() ? 3 : 2;
							icon1Obj = newObject();
							PutDoc("/I "+QString::number(icon1Obj)+" 0 R ");
						}
						if (!ite->Pfile2.isEmpty())
						{
							CMSettings cms(ite->doc(), "", Intent_Perceptual);
							cms.allowColorManagement(false);
							img.loadPicture(ite->Pfile2, 1, cms, ScImage::RGBData, 72);
							QByteArray im;
							img3.getAlpha(ite->Pfile2, 1, im, true, false);
							IconOb += !im.isEmpty() ? 3 : 2;
							im.resize(0);
							icon2Obj = newObject();
							PutDoc("/IX "+QString::number(icon2Obj)+" 0 R ");
						}
						if (!ite->Pfile3.isEmpty())
						{
							CMSettings cms(ite->doc(), "", Intent_Perceptual);
							cms.allowColorManagement(false);
							img2.loadPicture(ite->Pfile3, 1, cms, ScImage::RGBData, 72);
							QByteArray im;
							img3.getAlpha(ite->Pfile3, 1, im, true, false);
							IconOb += !im.isEmpty() ? 3 : 2;
							im.resize(0);
							icon3Obj = newObject();
							PutDoc("/RI "+QString::number(icon3Obj)+" 0 R ");
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
								PutDoc(FToStr(qMax(ite->imageXOffset() / (ite->width()/ite->imageXScale() - ite->pixm.width()), 0.01))+" ");
							else
								PutDoc("0.5 ");
						}
						else
							PutDoc("0 ");
						if ((ite->height()/ite->imageYScale() - ite->pixm.height()) != 0)
						{
							if (ite->annotation().ScaleW() == 3)
								PutDoc(FToStr(1.0 - qMax(ite->imageYOffset() / (ite->height()/ite->imageYScale() - ite->pixm.height()), 0.01)));
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
					PutDoc("/CA "+EncString("("+ct+")",annotationObj)+" ");
					break;
			}
			if (ite->rotation() != 0)
				PutDoc("/R "+QString::number((abs(static_cast<int>(ite->rotation())) / 90)*90)+" ");
			PutDoc(">>\n");
			if ((ite->annotation().ActionType() != 0) || (ite->annotation().AAact()))
			{
				if (ite->annotation().ActionType() == 7)
				{
					PutDoc("/A << /Type /Action /S /GoToR\n/F "+ EncString("("+Path2Relative(ite->annotation().Extern(), baseDir)+")",annotationObj)+ "\n");
					PutDoc("/D ["+QString::number(ite->annotation().Ziel())+" /XYZ "+ite->annotation().Action()+"]\n>>\n");
				}
				if (ite->annotation().ActionType() == 9)
				{
					PutDoc("/A << /Type /Action /S /GoToR\n/F "+ EncString("("+ite->annotation().Extern()+")",ObjCounter-1)+"\n");
					PutDoc("/D ["+QString::number(ite->annotation().Ziel())+" /XYZ "+ite->annotation().Action()+"]\n>>\n");
				}
				if (ite->annotation().ActionType() == 5)
					PutDoc("/A << /Type /Action /S /ImportData\n/F "+ EncString("("+ite->annotation().Action()+")",annotationObj)+" >>\n");
				if (ite->annotation().ActionType() == 4)
					PutDoc("/A << /Type /Action /S /ResetForm >>\n");
				if (ite->annotation().ActionType() == 3)
				{
					PutDoc("/A << /Type /Action /S /SubmitForm\n/F << /FS /URL /F "+ EncString("("+ite->annotation().Action()+")",annotationObj)+" >>\n");
//					if (ite->annotation().HTML())
//						PutDoc("/Flags 4");
					switch (ite->annotation().HTML())
					{
					case 1:
					  // HTML
					  PutDoc("/Flags 4"); // bit 3 on
					  break;
					case 2:
					  // XFDF
					  PutDoc("/Flags 64"); // bit 6 on
					  break;
					case 3:
					  // PDF
					  PutDoc("/Flags 512"); // bit 9 on
					  break;
					case 0:
					default:
					  // FDF
					  // bit 3 off
					  break;
					}
					PutDoc(">>\n");
				}
				if (ite->annotation().ActionType() == 1)
				{
					if (!ite->annotation().Action().isEmpty())
					{
						PutDoc("/A << /Type /Action /S /JavaScript /JS " + QString::number(actionObj) + " 0 R >>\n");
					}
				}
				if (ite->annotation().AAact())
				{
					if (!ite->annotation().Action().isEmpty())
					{
						PutDoc("/A << /Type /Action /S /JavaScript /JS " + QString::number(actionObj) + " 0 R >>\n");
					}
					PutDoc("/AA " + QString::number(AActionObj) + " 0 R\n");
				}
				if (ite->annotation().ActionType() == 2)
				{
					PutDoc("/A << /Type /Action /S /GoTo /D /" + NDnam + QString::number(NDnum) + " >>\n");
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
	PutDoc("/Rect [ "+FToStr(x+bleedDisplacementX)+" "+FToStr(y2+bleedDisplacementY)+" "+FToStr(x2+bleedDisplacementX)+" "+FToStr(y+bleedDisplacementY)+" ]\n");
	PutDoc(">>\nendobj\n");
	// write icons
	if ((ite->annotation().Type() == 2) && (ite->annotation().UseIcons()))
	{
		if (!ite->Pfile.isEmpty())
		{
			if (!PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true))
				return false;
			cc = QString::number(ite->pixm.width())+" 0 0 "+QString::number(ite->pixm.height())+" 0 0 cm\n";
			cc += "/"+ResNam+"I"+QString::number(ResCount-1)+" Do";
			PDF_xForm(icon1Obj, ite->pixm.width(), ite->pixm.height(), cc);
		}
		if (!ite->Pfile2.isEmpty())
		{
			if (!PDF_Image(ite, ite->Pfile2, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true))
				return false;
			cc = QString::number(img.width())+" 0 0 "+QString::number(img.height())+" 0 0 cm\n";
			cc += "/"+ResNam+"I"+QString::number(ResCount-1)+" Do";
			PDF_xForm(icon2Obj, img.width(), img.height(), cc);
		}
		if (!ite->Pfile3.isEmpty())
		{
			if (!PDF_Image(ite, ite->Pfile3, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true))
				return false;
			cc = QString::number(img2.width())+" 0 0 "+QString::number(img2.height())+" 0 0 cm\n";
			cc += "/"+ResNam+"I"+QString::number(ResCount-1)+" Do";
			PDF_xForm(icon3Obj, img2.width(), img2.height(), cc);
		}
	}
	// write Appearance?
	if (ite->annotation().Type() == 3)
	{
		cc = "";
		if (ite->fillColor() != CommonStrings::None)
			cc += putColor(ite->fillColor(), ite->fillShade(), false);
		cc += FToStr(x)+" "+FToStr(y2)+" "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += "/Tx BMC\nBT\n";
		if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
			cc += putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
		if (Options.Version < PDFOptions::PDFVersion_14)
			cc += "/"+StdFonts[ind2PDFabr[ite->annotation().Font()]];
		else
			cc += UsedFontsF[ite->itemText.defaultStyle().charStyle().font().replacementName()];
//			cc += UsedFontsP[ite->itemText.defaultStyle().charStyle().font().replacementName()]+"Form";
		cc += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf\n";
		if (bmst.count() > 1)
		{
			cc += "1 0 0 1 0 0 Tm\n0 0 Td\n";
			for (int mz = 0; mz < bmst.count(); ++mz)
			{
				cc += EncStringUTF16("("+bmst[mz]+")",annotationObj);
				cc += " Tj\nT*\n";
			}
			cc += "ET\nEMC";
		}
		else
			cc += "1 0 0 1 0 0 Tm\n0 0 Td\n"+EncStringUTF16("("+bmUtf16+")",annotationObj)+" Tj\nET\nEMC";
		PDF_xForm(appearanceObj, ite->width(), ite->height(), cc);
	}
	if (ite->annotation().Type() == 4)
	{
		cc = "q\nBT\n";
		if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
			cc += putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
		cc += "/"+StdFonts["/ZapfDingbats"]+" "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf\n";
		cc += "0 0 Td\n("+ct+") Tj\nET\nQ";
		PDF_xForm(appearanceObj, ite->width(), ite->height(), cc);
	}
	if ((ite->annotation().Type() == 5) || (ite->annotation().Type() == 6))
	{
		cc = "";
		cc += "1 g\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += QString::number(ite->annotation().Bwid())+" w\n";
		if (ite->annotation().borderColor() != CommonStrings::None)
			cc += putColor(ite->annotation().borderColor(), 100.0, false);
		else
			cc += "0 G\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nS\n";
		cc += "/Tx BMC\nq\nBT\n";
		cc += "0 g\n";
		if (Options.Version < PDFOptions::PDFVersion_14)
			cc += "/"+StdFonts[ind2PDFabr[ite->annotation().Font()]];
		else
			cc += UsedFontsF[ite->itemText.defaultStyle().charStyle().font().replacementName()];
//			cc += UsedFontsP[ite->itemText.defaultStyle().charStyle().font().replacementName()]+"Form";
		cc += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf\n";
		cc += "1 0 0 1 0 0 Tm\n0 0 Td\n";
		if (bmst.count() > 0)
			cc += EncStringUTF16("("+bmst[0]+")",annotationObj);
		cc += " Tj\nET\nQ\nEMC";
		PDF_xForm(appearanceObj, ite->width(), ite->height(), cc);
	}
	return true;
}

uint PDFLibCore::writeActions(const Annotation&	annot, uint annotationObj)
{
	// write actions
	if ((annot.Type() > 1) && (annot.AAact()))
	{
		uint E = 0;
		if (!annot.E_act().isEmpty())
			E = WritePDFString(annot.E_act());
		uint X = 0;
		if (!annot.X_act().isEmpty())
			X = WritePDFString(annot.X_act());
		uint D = 0;
		if (!annot.D_act().isEmpty())
			D = WritePDFString(annot.D_act());
		uint Fo = 0;
		if (!annot.Fo_act().isEmpty())
			Fo = WritePDFString(annot.Fo_act());
		uint Bl = 0;
		if (!annot.Bl_act().isEmpty())
			Bl = WritePDFString(annot.Bl_act());
		uint K = 0;
		uint F = 0;
		uint V = 0;
		uint C = 0;
		if ((annot.Type() == 3) || (annot.Type() == 5) || (annot.Type() == 6))
		{
			if (!annot.K_act().isEmpty())
				K = WritePDFString(annot.K_act());
			if (!annot.F_act().isEmpty())
				F = WritePDFString(annot.F_act());
			if (!annot.V_act().isEmpty())
				V = WritePDFString(annot.V_act());
			if (!annot.C_act().isEmpty())
			{
				C = WritePDFString(annot.C_act());
				CalcFields.append(annotationObj);
			}
		}

		uint result = newObject();
		StartObj(result);
		PutDoc("<<\n");
		if (E)
		{
			PutDoc("/E << /Type /Action /S /JavaScript /JS "+QString::number(E)+" 0 R >>\n");
		}
		if (X)
		{
			PutDoc("/X << /Type /Action /S /JavaScript /JS "+QString::number(X)+" 0 R >>\n");
		}
		if (D)
		{
			PutDoc("/D << /Type /Action /S /JavaScript /JS "+QString::number(D)+" 0 R >>\n");
		}
		if (Fo)
		{
			PutDoc("/Fo << /Type /Action /S /JavaScript /JS "+QString::number(Fo)+" 0 R >>\n");
		}
		if (Bl)
		{
			PutDoc("/Bl << /Type /Action /S /JavaScript /JS "+QString::number(Bl)+" 0 R >>\n");
		}
		if ((annot.Type() == 3) || (annot.Type() == 5) || (annot.Type() == 6))
		{
			if (K)
			{
				PutDoc("/K << /Type /Action /S /JavaScript /JS "+QString::number(K) + " 0 R >>\n");
			}
			if (F)
			{
				PutDoc("/F << /Type /Action /S /JavaScript /JS "+QString::number(F) + " 0 R >>\n");
			}
			if (V)
			{
				PutDoc("/V << /Type /Action /S /JavaScript /JS "+QString::number(V)+" 0 R >>\n");
			}
			if (C)
			{
				PutDoc("/C << /Type /Action /S /JavaScript /JS "+QString::number(C)+" 0 R >>\n");
			}
		}
		PutDoc(">>\nendobj\n");
		return result;
	}
	return 0;
}

uint PDFLibCore::WritePDFStream(const QString& cc)
{
	uint result = newObject();
	QString tmp(cc);
	if (Options.Compress)
		tmp = CompressStr(&tmp);
	StartObj(result);
	PutDoc("<< /Length "+QString::number(tmp.length()));  // moeglicherweise +1
	if (Options.Compress)
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n"+EncStream(tmp, result)+"\nendstream\nendobj\n");
	return result;
}

uint PDFLibCore::WritePDFString(const QString& cc)
{
	QString tmp;
	for (int i = 0; i < cc.length(); ++i)
	{
		if (cc[i].unicode() > 255)
		{
			tmp += "\\u";
			tmp += toHex(cc[i].row());
			tmp += toHex(cc[i].cell());
		}
		else
			tmp += cc[i];
	}
	return WritePDFStream(tmp);
}

void PDFLibCore::PDF_xForm(uint objNr, double w, double h, QString im)
{
	StartObj(objNr);
	PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
	PutDoc("/BBox [ 0 0 "+FToStr(w)+" "+FToStr(h)+" ]\n");
	PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
	if (Seite.ImgObjects.count() != 0)
	{
		PutDoc("/XObject <<\n");
		QMap<QString,int>::Iterator it;
		for (it = Seite.ImgObjects.begin(); it != Seite.ImgObjects.end(); ++it)
			PutDoc("/"+it.key()+" "+QString::number(it.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Seite.FObjects.count() != 0)
	{
		PutDoc("/Font << \n");
		QMap<QString,int>::Iterator it2;
		for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
			PutDoc("/"+it2.key()+" "+QString::number(it2.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	PutDoc(">>\n");
	PutDoc("/Length "+QString::number(im.length())+"\n");
	PutDoc(">>\nstream\n"+EncStream(im, objNr)+"\nendstream\nendobj\n");
	Seite.XObjects[ResNam+QString::number(ResCount)] = objNr;
	ResCount++;
}

void PDFLibCore::PDF_Form(const QString& im) // unused? - av
{
	uint form = newObject();
	StartObj(form);
	PutDoc("<<\n");
	PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
	if (Seite.FObjects.count() != 0)
	{
		PutDoc("/Font << \n");
		QMap<QString,int>::Iterator it2;
		for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
			PutDoc("/"+it2.key()+" "+QString::number(it2.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	PutDoc(">>\n");
	PutDoc("/Length "+QString::number(im.length())+"\n");
	PutDoc(">>\nstream\n"+EncStream(im, form)+"\nendstream\nendobj\n");
}

void PDFLibCore::PDF_Bookmark(PageItem *currItem, double ypos)
{
	Bvie->SetAction(currItem, "/XYZ 0 "+FToStr(ypos)+" 0]");
	BookMinUse = true;
}


bool PDFLibCore::PDF_EmbeddedPDF(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN, const QString& Profil, bool Embedded, int Intent, ShIm& imgInfo, QString* output)
{
	if (!Options.embedPDF)
		return false;

#ifdef HAVE_PODOFO
	try
	{
		PoDoFo::PdfError::EnableDebug( false );
		PoDoFo::PdfError::EnableLogging( false );
		PoDoFo::PdfMemDocument doc( fn.toLocal8Bit().data() );
		PoDoFo::PdfPage*         page      = doc.GetPage(qMin(qMax(1, c->pixm.imgInfo.actualPageNumber), c->pixm.imgInfo.numberOfPages) - 1);
		PoDoFo::PdfObject* contents  = page? page->GetContents() : NULL;
		PoDoFo::PdfObject* resources = page? page->GetResources() : NULL;
		for (PoDoFo::PdfObject* par = page->GetObject(); par && !resources; par = par->GetIndirectKey("Parent"))
		{
			resources = par->GetIndirectKey("Resources");
		}
		if (contents && contents->GetDataType() ==  PoDoFo::ePdfDataType_Dictionary)
		{
			PoDoFo::PdfStream* stream = contents->GetStream();
			QMap<PoDoFo::PdfReference, uint> importedObjects;
			QList<PoDoFo::PdfReference> referencedObjects;
			PoDoFo::PdfObject* nextObj;
			uint xObj = newObject();
			uint xResources = newObject();
			uint xParents = 0;
			importedObjects[page->GetObject()->Reference()] = xObj;
			StartObj(xObj);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1");
			PoDoFo::PdfRect pagesize = page->GetPageSize();
			int rotation = page->GetRotation();
			QTransform pageM;
			pageM.scale(1.0/pagesize.GetWidth(), 1.0/pagesize.GetHeight());
			pageM.rotate(-rotation);
			PutDoc("\n/BBox [" + QString::number(pagesize.GetLeft(), 'f'));
			PutDoc(" " + QString::number(pagesize.GetBottom(), 'f'));
			PutDoc(" " + QString::number(pagesize.GetLeft() + pagesize.GetWidth(), 'f'));
			PutDoc(" " + QString::number(pagesize.GetBottom() + pagesize.GetHeight(), 'f'));
			PutDoc("]");
			PutDoc("\n/Matrix [" + QString::number(pageM.m11(), 'f') + " "
								 + QString::number(pageM.m12(), 'f') + " "
								 + QString::number(pageM.m21(), 'f') + " "
								 + QString::number(pageM.m22(), 'f') + " ");
			if (rotation == 0)
				PutDoc("0 0");
			else if (rotation == 90)
				PutDoc("0 1");
			else if (rotation == 180)
				PutDoc("1 1");
			else if (rotation == 270)
				PutDoc(QString::number(pagesize.GetHeight() / pagesize.GetWidth(), 'f') + " " + QString::number(1.0 - 1.0 / (pagesize.GetHeight() / pagesize.GetWidth()), 'f'));
			else
				PutDoc("0 0");
			PutDoc("]");
//			PutDoc("\n/Matrix [" + QString::number(1.0/pagesize.GetWidth()) + " 0 0 " + QString::number(1.0/pagesize.GetHeight()) + " 0 0]");
			PutDoc("\n/Resources " + QString::number(xResources) + " 0 R");
			nextObj = page->GetObject()->GetIndirectKey("Group");
			if (nextObj)
			{
				PutDoc("\n/Group "); // PDF 1.4
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			/*
			PoDoFo::PdfObject parents = page->GetObject()->GetIndirectKey("StructParents");
			if (parents)
			{
				xParents = newObject();
				PutDoc("\n/StructParents " + QString::number(xParents)); // required if page uses structured content
			}
			*/
			char * mbuffer = NULL;
			long mlen = 0;

#if ( (PODOFO_VERSION_MAJOR == 0) && (PODOFO_VERSION_MINOR == 7) && (PODOFO_VERSION_PATCH == 99) ) || ( (PODOFO_VERSION_MAJOR == 0) && (PODOFO_VERSION_MINOR > 7) )
			// seems more complicated at first, but in fact it makes the code more stable wrt podofo changes
			PoDoFo::PdfMemoryOutputStream oStream(1);
			stream->GetCopy(&oStream);
			oStream.Close();
			mlen = oStream.GetLength();
			mbuffer = oStream.TakeBuffer();
#else
			stream->GetCopy(&mbuffer, &mlen);
#endif
			if (mbuffer[mlen-1] == '\n')
				--mlen;
			PutDoc("\n/Length " + QString::number(mlen));
			nextObj = contents->GetIndirectKey("Filter");
			if (nextObj)
			{
				PutDoc("\n/Filter ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			nextObj = contents->GetIndirectKey("DecodeParms");
			if (nextObj)
			{
				PutDoc("\n/DecodeParms ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			PutDoc("\n>>\nstream\n");
			{
				QByteArray buffer = QByteArray::fromRawData(mbuffer, mlen);
				EncodeArrayToStream(buffer, xObj);
			}  // disconnect QByteArray from raw data
			free (mbuffer);
			PutDoc("\nendstream\nendobj\n");
			// write resources
			if (resources)
			{
				copyPoDoFoObject(resources, xResources, importedObjects);
			}
			else
			{
				StartObj(xResources);
				PutDoc("<< >>\nendobj\n");
			}
			if (xParents)
			{
				// create structured parents
			}
			// write referenced objects
			PoDoFo::PdfVecObjects* allObjects = contents->GetOwner();
			for (int i=0; i < referencedObjects.size();  ++i)
			{
				nextObj = allObjects->GetObject(referencedObjects[i]);
				copyPoDoFoObject(nextObj, importedObjects[nextObj->Reference()], importedObjects);
			}

			Seite.ImgObjects[ResNam+"I"+QString::number(ResCount)] = xObj;
			imgInfo.ResNum = ResCount;
			ResCount++;
			// Avoid a divide-by-zero if width/height are less than 1 point:
			imgInfo.Width  = qMax(1, (int) pagesize.GetWidth());
			imgInfo.Height = qMax(1, (int) pagesize.GetHeight());
			imgInfo.xa  = sx * pagesize.GetWidth()/imgInfo.Width;
			imgInfo.ya  = sy * pagesize.GetHeight()/imgInfo.Height;
			// Width/Height are integers and may not exactly equal pagesize.GetWidth()/
			// pagesize.GetHeight(). Adjust scale factor to compensate for the difference.
			imgInfo.sxa = sx * pagesize.GetWidth()/imgInfo.Width;
			imgInfo.sya = sy * pagesize.GetHeight()/imgInfo.Height;

			return true;
		}
		else if (contents && contents->GetDataType() ==  PoDoFo::ePdfDataType_Array)//Page contents might be an array
		{
			QMap<PoDoFo::PdfReference, uint> importedObjects;
			QList<PoDoFo::PdfReference> referencedObjects;
			PoDoFo::PdfObject* nextObj;
			uint xObj = newObject();
			uint xResources = newObject();
			uint xParents = 0;
			importedObjects[page->GetObject()->Reference()] = xObj;
			StartObj(xObj);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1");
			PoDoFo::PdfRect pagesize = page->GetPageSize();
			int rotation = page->GetRotation();
			QMatrix pageM;
			pageM.scale(1.0/pagesize.GetWidth(), 1.0/pagesize.GetHeight());
			pageM.rotate(-rotation);
			PutDoc("\n/BBox [" + QString::number(pagesize.GetLeft(), 'f'));
			PutDoc(" " + QString::number(pagesize.GetBottom(), 'f'));
			PutDoc(" " + QString::number(pagesize.GetLeft() + pagesize.GetWidth(), 'f'));
			PutDoc(" " + QString::number(pagesize.GetBottom() + pagesize.GetHeight(), 'f'));
			PutDoc("]");
			PutDoc("\n/Matrix [" + QString::number(pageM.m11(), 'f') + " "
								 + QString::number(pageM.m12(), 'f') + " "
								 + QString::number(pageM.m21(), 'f') + " "
								 + QString::number(pageM.m22(), 'f') + " ");
			if (rotation == 0)
				PutDoc("0 0");
			else if (rotation == 90)
				PutDoc("0 1");
			else if (rotation == 180)
				PutDoc("1 1");
			else if (rotation == 270)
				PutDoc(QString::number(pagesize.GetHeight() / pagesize.GetWidth(), 'f') + " " + QString::number(1.0 - 1.0 / (pagesize.GetHeight() / pagesize.GetWidth()), 'f'));
			else
				PutDoc("0 0");
			PutDoc("]");
			PutDoc("\n/Resources " + QString::number(xResources) + " 0 R");
			nextObj = page->GetObject()->GetIndirectKey("Group");
			if (nextObj)
			{
				PutDoc("\n/Group "); // PDF 1.4
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}

			char * mbuffer = NULL;
			long mlen = 0;
			// copied from podofoimpose
			PoDoFo::PdfMemoryOutputStream outMemStream ( 1 );
//			PoDoFo::PdfFilteredEncodeStream outMemStream (outMemStreamRaw, ePdfFilter_FlateDecode, false);
			PoDoFo::PdfArray carray(page->GetContents()->GetArray());
			for(unsigned int ci = 0; ci < carray.size(); ++ci)
			{
				if(carray[ci].HasStream())
				{
					carray[ci].GetStream()->GetFilteredCopy ( &outMemStream );
				}
				else if(carray[ci].IsReference())
				{

					nextObj = doc.GetObjects().GetObject(carray[ci].GetReference());

					while(nextObj != NULL)
					{

						if(nextObj->IsReference())
						{
							nextObj = doc.GetObjects().GetObject(nextObj->GetReference());
						}
						else if(nextObj->HasStream())
						{
							nextObj->GetStream()->GetFilteredCopy ( &outMemStream );
							break;
						}
					}

				}

			}
			// end of copy
			mlen = outMemStream.GetLength();
			mbuffer = outMemStream.TakeBuffer();
//			if (mbuffer[mlen-1] == '\n')
//				--mlen;
			PutDoc("\n/Length " + QString::number(mlen));
/*
 nextObj = contents->GetIndirectKey("Filter");
			if (nextObj)
			{
				PutDoc("\n/Filter ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			nextObj = contents->GetIndirectKey("DecodeParms");
			if (nextObj)
			{
				PutDoc("\n/DecodeParms ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
*/
			PutDoc("\n>>\nstream\n");
			{
				QByteArray buffer = QByteArray::fromRawData(mbuffer, mlen);
				EncodeArrayToStream(buffer, xObj);
			}  // disconnect QByteArray from raw data
			free (mbuffer);
			PutDoc("\nendstream\nendobj\n");
			// write resources
			if (resources)
			{
				copyPoDoFoObject(resources, xResources, importedObjects);
			}
			else
			{
				StartObj(xResources);
				PutDoc("<< >>\nendobj\n");
			}
			if (xParents)
			{
				// create structured parents
			}
			// write referenced objects
			PoDoFo::PdfVecObjects* allObjects = contents->GetOwner();
			for (int i=0; i < referencedObjects.size();  ++i)
			{
				nextObj = allObjects->GetObject(referencedObjects[i]);
				copyPoDoFoObject(nextObj, importedObjects[nextObj->Reference()], importedObjects);
			}

			Seite.ImgObjects[ResNam+"I"+QString::number(ResCount)] = xObj;
			imgInfo.ResNum = ResCount;
			ResCount++;
			// Avoid a divide-by-zero if width/height are less than 1 point:
			imgInfo.Width  = qMax(1, (int) pagesize.GetWidth());
			imgInfo.Height = qMax(1, (int) pagesize.GetHeight());
			imgInfo.xa  = sx * pagesize.GetWidth()/imgInfo.Width;
			imgInfo.ya  = sy * pagesize.GetHeight()/imgInfo.Height;
			// Width/Height are integers and may not exactly equal pagesize.GetWidth()/
			// pagesize.GetHeight(). Adjust scale factor to compensate for the difference.
			imgInfo.sxa = sx * pagesize.GetWidth()/imgInfo.Width;
			imgInfo.sya = sy * pagesize.GetHeight()/imgInfo.Height;

			return true;
		}

	}
	catch(PoDoFo::PdfError& e)
	{
		qDebug() << "PoDoFo error!";
		e.PrintErrorMsg();
		assert (false);
		return false;
	}
#endif
	return false;
}


#if HAVE_PODOFO

void PDFLibCore::copyPoDoFoDirect(const PoDoFo::PdfVariant* obj, QList<PoDoFo::PdfReference>& referencedObjects, QMap<PoDoFo::PdfReference, uint>& importedObjects)
{
	switch (obj->GetDataType())
	{
	case PoDoFo::ePdfDataType_Reference:
		{
		const PoDoFo::PdfReference reference(obj->GetReference());
		uint objNr;
		if (!importedObjects.contains(reference))
		{
			objNr = newObject();
			importedObjects[reference] = objNr;
			referencedObjects.append(reference);
		}
		else
		{
			objNr = importedObjects[reference];
		}
		PutDoc(" " + QString::number(objNr) + " 0 R");
		}
		break;
	case PoDoFo::ePdfDataType_Array:
		{
		const PoDoFo::PdfArray& array(obj->GetArray());
		PutDoc("[");
		for (uint i=0; i < array.size(); ++i)
			copyPoDoFoDirect( &(array[i]), referencedObjects, importedObjects);
			PutDoc("]");
		}
		break;
	case PoDoFo::ePdfDataType_Dictionary:
		{
		const PoDoFo::PdfDictionary& dict(obj->GetDictionary());
		const PoDoFo::TKeyMap keys = dict.GetKeys();
		PutDoc("<<");
		for (PoDoFo::TCIKeyMap k=keys.begin(); k != keys.end(); ++k)
		{
			PutDoc("\n/" + k->first.GetEscapedName());
			copyPoDoFoDirect(k->second, referencedObjects, importedObjects);
		}
		PutDoc(" >>");
		}
		break;
	default:
		{
		std::string str;
		obj->ToString(str);
		PutDoc(" " + str);
		}
	}

}

void PDFLibCore::copyPoDoFoObject(const PoDoFo::PdfObject* obj, uint scObjID, QMap<PoDoFo::PdfReference, uint>& importedObjects)
{
	PoDoFo::PdfVecObjects* allObjects = obj->GetOwner();
	QList<PoDoFo::PdfReference> referencedObjects;
	StartObj(scObjID);
	copyPoDoFoDirect(obj, referencedObjects, importedObjects);
	if (obj->HasStream())
	{
		const PoDoFo::PdfStream* stream = obj->GetStream();
		char * mbuffer = NULL;
		long mlen = 0;

#if ( (PODOFO_VERSION_MAJOR == 0) && (PODOFO_VERSION_MINOR == 7) && (PODOFO_VERSION_PATCH == 99) ) || ( (PODOFO_VERSION_MAJOR == 0) && (PODOFO_VERSION_MINOR > 7) )
		// seems more complicated at first, but in fact it makes the code more stable wrt podofo changes
		PoDoFo::PdfMemoryOutputStream oStream(1);
		stream->GetCopy(&oStream);
		oStream.Close();
		mlen = oStream.GetLength();
		mbuffer = oStream.TakeBuffer();
#else
		stream->GetCopy(&mbuffer, &mlen);
#endif
		if (mbuffer[mlen-1] == '\n')
			--mlen;
		PutDoc("\nstream\n");
		{
			QByteArray buffer = QByteArray::fromRawData(mbuffer, mlen);
			EncodeArrayToStream(buffer, scObjID);
		}  // disconnect QByteArray from raw data
		free (mbuffer);
		PutDoc("\nendstream");
	}
	PutDoc("\nendobj\n");
	// recurse:
	for (int i=0; i < referencedObjects.size();  ++i)
	{
		PoDoFo::PdfObject* nextObj = allObjects->GetObject(referencedObjects[i]);
		copyPoDoFoObject(nextObj, importedObjects[nextObj->Reference()], importedObjects);
	}
}
#endif


bool PDFLibCore::PDF_Image(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN, const QString& Profil, bool Embedded, eRenderIntent Intent, QString* output)
{
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	ScImage img;
	QString tmp, tmpy, dummy, cmd1, cmd2, BBox;
	QChar  tc;
	bool   found = false;
	bool   alphaM = false;
	bool   realCMYK = false;
	bool   bitmapFromGS = false;
	bool   isEmbeddedPDF = false;
	bool   hasGrayProfile = false;
	QString profInUse = Profil;
	int    afl = Options.Resolution;
	double ax, ay, a2, a1;
	int    origWidth = 1;
	int    origHeight = 1;
	ShIm   ImInfo;
	ImInfo.ResNum = 0;
	ImInfo.sxa = 0;
	ImInfo.sya = 0;
	if (Options.RecalcPic)
		ImInfo.reso = Options.PicRes / 72.0;
	else
		ImInfo.reso = Options.Resolution / 72.0;
	ImInfo.Width = 0;
	ImInfo.Height = 0;
	ImInfo.Page = 0;
	ImInfo.xa = x;
	ImInfo.ya = y;
	ImInfo.origXsc = c->imageXScale();
	ImInfo.origYsc = c->imageYScale();
	ShIm   ImInfo2;
	ImInfo2.origXsc = c->imageXScale();
	ImInfo2.origYsc = c->imageYScale();
	if (SharedImages.contains(fn))
		ImInfo2 = SharedImages[fn];
	if ((!SharedImages.contains(fn))
		 || (fromAN)
		 || (c->asLatexFrame())
		 || (c->effectsInUse.count() != 0)
		 || ((ImInfo2.origXsc != ImInfo.origXsc) || (ImInfo2.origYsc != ImInfo.origYsc))
		 || (ImInfo2.RequestProps != c->pixm.imgInfo.RequestProps)
		 || (ImInfo2.Page != c->pixm.imgInfo.actualPageNumber))
	{
		bool imageLoaded = false;
		if ((extensionIndicatesPDF(ext) || ((extensionIndicatesEPSorPS(ext)) && (c->pixm.imgInfo.type != ImageType7))) && c->effectsInUse.count() == 0)
		{
			if (extensionIndicatesEPSorPS(ext))
			{
				QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.pdf");
				QStringList opts;
				opts.append("-dEPSCrop");
				if (Options.Version >= PDFOptions::PDFVersion_14)
					opts.append( "-dCompatibilityLevel=1.4" );
				else
					opts.append( "-dCompatibilityLevel=1.3" );
/*				These options don't seem to work
				if ((Options.UseRGB) || ((doc.HasCMS) && (Options.UseProfiles2)) || (Options.isGrayscale))
					opts.append( "-sProcessColorModel=/DeviceRGB" );
				else
					opts.append( "-sProcessColorModel=/DeviceCMYK" ); */
				if (convertPS2PDF(fn, tmpFile, opts) == 0)
				{
					imageLoaded = PDF_EmbeddedPDF(c, tmpFile, sx, sy, x, y, fromAN, Profil, Embedded, Intent, ImInfo, output);
					QFile::remove(tmpFile);
				}
			}
			else
				imageLoaded = PDF_EmbeddedPDF(c, fn, sx, sy, x, y, fromAN, Profil, Embedded, Intent, ImInfo, output);
			isEmbeddedPDF = true;
			ImInfo.Page = c->pixm.imgInfo.actualPageNumber;
		}
		if(!imageLoaded && extensionIndicatesPDF(ext) && c->effectsInUse.count() == 0 && Options.embedPDF)
			qDebug() << "Failed to embed the PDF file";
		// no embedded PDF:
		if (!imageLoaded)
		{
			if ((extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)) && (c->pixm.imgInfo.type != ImageType7))
			{
				bitmapFromGS = true;
				if (Options.RecalcPic)
				{
					afl = qMin(Options.PicRes, Options.Resolution);
					ImInfo.reso = afl / 72.0;
				}
				else
					afl = Options.Resolution;
				if (ext == "pdf")
				{
					CMSettings cms(c->doc(), Profil, Intent);
					cms.setUseEmbeddedProfile(Embedded);
					if (Options.UseRGB)
						imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
					else
					{
						if ((doc.HasCMS) && (Options.UseProfiles2))
							imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
						else
						{
							if (Options.isGrayscale)
								imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
							else
								imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, afl);
						}
					}
				}
				else
				{
					QFile f(fn);
					if (f.open(QIODevice::ReadOnly))
					{
						QDataStream ts(&f);
						while (!ts.atEnd())
						{
							tmp = readLinefromDataStream(ts);
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
							CMSettings cms(c->doc(), Profil, Intent);
							cms.setUseEmbeddedProfile(Embedded);
							if (Options.UseRGB)
								imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
							else
							{
								if ((doc.HasCMS) && (Options.UseProfiles2))
									imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
								else
								{
									if (Options.isGrayscale)
										imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
									else
										imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, afl);
								}
							}
						}
					}
				}
				if (!imageLoaded)
				{
					PDF_Error_ImageLoadFailure(fn);
					return false;
				}
				if (Options.RecalcPic)
				{
					ImInfo.sxa = sx * (1.0 / ImInfo.reso);
					ImInfo.sya = sy * (1.0 / ImInfo.reso);
				}
			}
			// not PS/PDF
			else
			{
				img.imgInfo.valid = false;
				img.imgInfo.clipPath = "";
				img.imgInfo.PDSpathData.clear();
				img.imgInfo.layerInfo.clear();
				img.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
				img.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
				CMSettings cms(c->doc(), Profil, Intent);
				cms.setUseEmbeddedProfile(Embedded);
				if (Options.UseRGB)
					imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 72, &realCMYK);
				else
				{
					if ((doc.HasCMS) && (Options.UseProfiles2))
						imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RawData, 72, &realCMYK);
					else
					{
						if (Options.isGrayscale)
							imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 72, &realCMYK);
						else
							imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, 72, &realCMYK);
					}
				}
				if (!imageLoaded)
				{
					PDF_Error_ImageLoadFailure(fn);
					return false;
				}
				if ((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale()))))
				{
					double afl = Options.PicRes;
					a2 = (72.0 / sx) / afl;
					a1 = (72.0 / sy) / afl;
					origWidth = img.width();
					origHeight = img.height();
					ax = img.width() / a2;
					ay = img.height() / a1;
					if ((Options.UseRGB) || (Options.isGrayscale) || ((Options.UseProfiles2) && !(img.imgInfo.colorspace == ColorSpaceCMYK)) )
					{
						ColorSpaceEnum colsp = img.imgInfo.colorspace;
						bool prog = img.imgInfo.progressive;
						img = img.scaled(qRound(ax), qRound(ay), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
						img.imgInfo.colorspace = colsp;
						img.imgInfo.progressive = prog;
					}
					else
						img.scaleImage(qRound(ax), qRound(ay));
					ImInfo.sxa = sx * a2;
					ImInfo.sya = sy * a1;
				}
				ImInfo.reso = 1;
			}
			bool hasColorEffect = false;
			if (c->effectsInUse.count() != 0)
			{
				for (int a = 0; a < c->effectsInUse.count(); ++a)
				{
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_COLORIZE)
						hasColorEffect = true;
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_DUOTONE)
						hasColorEffect = true;
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_TRITONE)
						hasColorEffect = true;
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_QUADTONE)
						hasColorEffect = true;
				}
			}
			if ((doc.HasCMS) && (Options.UseProfiles2))
			{
				if (!ICCProfiles.contains(Profil))
				{
					ScImage img3;
					int components = 0;
					uint embeddedProfile = newObject();
					StartObj(embeddedProfile);
					QByteArray dataP;
					struct ICCD dataD;
					if ((Embedded) && (!Options.EmbeddedI))
						img3.getEmbeddedProfile(fn, &dataP, &components);
					if ((dataP.isEmpty()) || ((img.imgInfo.colorspace == ColorSpaceGray) && (hasColorEffect) && (components == 1)))
					{
						if (img.imgInfo.colorspace == ColorSpaceCMYK)
						{
							QString profilePath;
							if (Embedded && ScCore->InputProfilesCMYK.contains(Options.ImageProf))
							{
								profilePath = ScCore->InputProfilesCMYK[Options.ImageProf];
								profInUse = Options.ImageProf;
							}
							else if (ScCore->InputProfilesCMYK.contains(Profil))
								profilePath = ScCore->InputProfilesCMYK[Profil];
							else
							{
								profilePath = ScCore->InputProfilesCMYK[c->doc()->cmsSettings().DefaultImageCMYKProfile];
								profInUse = c->doc()->cmsSettings().DefaultImageCMYKProfile;
							}
							loadRawBytes(profilePath, dataP);
							components = 4;
						}
						else
						{
							QString profilePath;
							if (Embedded && ScCore->InputProfiles.contains(Options.ImageProf))
							{
								profilePath = ScCore->InputProfiles[Options.ImageProf];
								profInUse = Options.ImageProf;
							}
							else if (ScCore->InputProfiles.contains(Profil))
								profilePath = ScCore->InputProfiles[Profil];
							else
							{
								profilePath = ScCore->InputProfiles[c->doc()->cmsSettings().DefaultImageRGBProfile];
								profInUse = c->doc()->cmsSettings().DefaultImageRGBProfile;
							}
							loadRawBytes(profilePath, dataP);
							components = 3;
						}
					}
					if (!ICCProfiles.contains(profInUse))
					{
						PutDoc("<<\n");
						if ((Options.CompressMethod != PDFOptions::Compression_None) && Options.Compress)
						{
							QByteArray compData = CompressArray(dataP);
							if (compData.size() > 0)
							{
								PutDoc("/Filter /FlateDecode\n");
								dataP = compData;
							}
						}
						PutDoc("/Length "+QString::number(dataP.size()+1)+"\n");
						PutDoc("/N "+QString::number(components)+"\n");
						PutDoc(">>\nstream\n");
						EncodeArrayToStream(dataP, embeddedProfile);
						PutDoc("\nendstream\nendobj\n");
						uint profileResource = newObject();
						StartObj(profileResource);
						dataD.ResName = ResNam+QString::number(ResCount);
						dataD.ICCArray = "[ /ICCBased "+QString::number(embeddedProfile)+" 0 R ]";
						dataD.ResNum = profileResource;
						dataD.components = components;
						ICCProfiles[profInUse] = dataD;
						PutDoc("[ /ICCBased "+QString::number(embeddedProfile)+" 0 R ]\n");
						PutDoc("endobj\n");
						ResCount++;
					}
					if (components == 1)
						hasGrayProfile = true;
				}
				else
				{
					if (ICCProfiles[Profil].components == 1)
					{
						if ((img.imgInfo.colorspace == ColorSpaceGray) && (hasColorEffect))
						{
							profInUse = c->doc()->cmsSettings().DefaultImageRGBProfile;
							if (!ICCProfiles.contains(profInUse))
							{
								int components = 3;
								uint embeddedProfile = newObject();
								StartObj(embeddedProfile);
								QByteArray dataP;
								struct ICCD dataD;
								loadRawBytes(ScCore->InputProfiles[c->doc()->cmsSettings().DefaultImageRGBProfile], dataP);
								components = 3;
								PutDoc("<<\n");
								if ((Options.CompressMethod != PDFOptions::Compression_None) && Options.Compress)
								{
									QByteArray compData = CompressArray(dataP);
									if (compData.size() > 0)
									{
										PutDoc("/Filter /FlateDecode\n");
										dataP = compData;
									}
								}
								PutDoc("/Length "+QString::number(dataP.size()+1)+"\n");
								PutDoc("/N "+QString::number(components)+"\n");
								PutDoc(">>\nstream\n");
								EncodeArrayToStream(dataP, embeddedProfile);
								PutDoc("\nendstream\nendobj\n");
								uint profileResource = newObject();
								StartObj(profileResource);
								dataD.ResName = ResNam+QString::number(ResCount);
								dataD.ICCArray = "[ /ICCBased "+QString::number(embeddedProfile)+" 0 R ]";
								dataD.ResNum = profileResource;
								dataD.components = components;
								ICCProfiles[profInUse] = dataD;
								PutDoc("[ /ICCBased "+QString::number(embeddedProfile)+" 0 R ]\n");
								PutDoc("endobj\n");
								ResCount++;
							}
						}
						else
							hasGrayProfile = true;
					}
				}
			}
			QByteArray im2;
			ScImage img2;
			img2.imgInfo.clipPath = "";
			img2.imgInfo.PDSpathData.clear();
			img2.imgInfo.layerInfo.clear();
			img2.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
			img2.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
			if (c->pixm.imgInfo.type == ImageType7)
				alphaM = false;
			else
			{
				bool gotAlpha = false;
				bool pdfVer14 = (Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4);
				gotAlpha = img2.getAlpha(fn, c->pixm.imgInfo.actualPageNumber, im2, true, pdfVer14, afl, img.width(), img.height());
				if (!gotAlpha)
				{
					PDF_Error_MaskLoadFailure(fn);
					return false;
				}
				alphaM = !im2.isEmpty();
			}
			bool imgE = false;
			if ((Options.UseRGB) || (Options.isGrayscale))
				imgE = false;
			else
			{
				if ((Options.UseProfiles2) && (img.imgInfo.colorspace != ColorSpaceCMYK))
					imgE = false;
				else
					imgE = true;
			}
			origWidth = img.width();
			origHeight = img.height();
			img.applyEffect(c->effectsInUse, c->doc()->PageColors, imgE);
			if (!((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale())))))
			{
				ImInfo.sxa = sx * (1.0 / ImInfo.reso);
				ImInfo.sya = sy * (1.0 / ImInfo.reso);
			}
			uint maskObj = 0;
			if (alphaM)
			{
				bool compAlphaAvail = false;
				maskObj = newObject();
				StartObj(maskObj);
				PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
				if (Options.CompressMethod != PDFOptions::Compression_None)
				{
					QByteArray compAlpha = CompressArray(im2);
					if (compAlpha.size() > 0)
					{
						im2 = compAlpha;
						compAlphaAvail = true;
					}
				}
				if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4))
				{
					PutDoc("/Width "+QString::number(origWidth)+"\n");
					PutDoc("/Height "+QString::number(origHeight)+"\n");
					PutDoc("/ColorSpace /DeviceGray\n");
					PutDoc("/BitsPerComponent 8\n");
					PutDoc("/Length "+QString::number(im2.size())+"\n");
				}
				else
				{
					PutDoc("/Width "+QString::number(origWidth)+"\n");
					PutDoc("/Height "+QString::number(origHeight)+"\n");
					PutDoc("/ImageMask true\n/BitsPerComponent 1\n");
					PutDoc("/Length "+QString::number(im2.size())+"\n");
				}
				if ((Options.CompressMethod != PDFOptions::Compression_None) && compAlphaAvail)
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n");
				EncodeArrayToStream(im2, maskObj);
				PutDoc("\nendstream\nendobj\n");
				Seite.ImgObjects[ResNam+"I"+QString::number(ResCount)] = maskObj;
				ResCount++;
			}
			uint imageObj = newObject();
			StartObj(imageObj);
			PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
			PutDoc("/Width "+QString::number(img.width())+"\n");
			PutDoc("/Height "+QString::number(img.height())+"\n");
			if ((doc.HasCMS) && (Options.UseProfiles2))
			{
				PutDoc("/ColorSpace "+ICCProfiles[profInUse].ICCArray+"\n");
				PutDoc("/Intent /");
				int inte2 = Intent;
				if (Options.EmbeddedI)
					inte2 = Options.Intent2;
				static const QString cmsmode[] = {"Perceptual", "RelativeColorimetric", "Saturation", "AbsoluteColorimetric"};
				PutDoc(cmsmode[inte2] + "\n");
			}
			else
			{
				if (Options.UseRGB)
					PutDoc("/ColorSpace /DeviceRGB\n");
				else
				{
					if (Options.isGrayscale)
						PutDoc("/ColorSpace /DeviceGray\n");
					else
						PutDoc("/ColorSpace /DeviceCMYK\n");
				}
			}
			enum PDFOptions::PDFCompression compress_method = Options.CompressMethod;
 			enum PDFOptions::PDFCompression cm = Options.CompressMethod;
			bool exportToCMYK = false, exportToGrayscale = false, jpegUseOriginal = false;
			if (!Options.UseRGB && !(doc.HasCMS && Options.UseProfiles2 && !realCMYK))
			{
				exportToGrayscale = Options.isGrayscale;
				if (exportToGrayscale)
					exportToCMYK      = !Options.isGrayscale;
				else
					exportToCMYK      = !Options.UseRGB;
			}
			if (c->OverrideCompressionMethod)
				compress_method = cm = (enum PDFOptions::PDFCompression) c->CompressionMethodIndex;
			if (extensionIndicatesJPEG(ext) && (cm != PDFOptions::Compression_None))
			{
				if (((Options.UseRGB || Options.UseProfiles2) && (cm == PDFOptions::Compression_Auto) && (c->effectsInUse.count() == 0) && (img.imgInfo.colorspace == ColorSpaceRGB)) && (!img.imgInfo.progressive) && (!((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale()))))))
				{
					jpegUseOriginal = true;
					cm = PDFOptions::Compression_JPEG;
				}
				// We can't unfortunately use directly cmyk jpeg files. Otherwise we have to use the /Decode argument in image
				// dictionnary, which we do not quite want as this argument is simply ignored by some rips and software
				// amongst which photoshop and illustrator
				/*else if (((!Options.UseRGB) && (!Options.isGrayscale) && (!Options.UseProfiles2)) && (cm== 0) && (c->effectsInUse.count() == 0) && (img.imgInfo.colorspace == ColorSpaceCMYK) && (!((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale()))))) && (!img.imgInfo.progressive))
				{
					jpegUseOriginal = false;
					exportToCMYK = true;
					cm = PDFOptions::Compression_JPEG;
				}*/
				else
				{
					if (compress_method == PDFOptions::Compression_JPEG)
					{
						if (realCMYK || !((Options.UseRGB) || (Options.UseProfiles2)))
						{
							exportToGrayscale = Options.isGrayscale;
							if (exportToGrayscale)
								exportToCMYK      = !Options.isGrayscale;
							else
								exportToCMYK      = !Options.UseRGB;
						}
						cm = PDFOptions::Compression_JPEG;
					}
					else
						cm = PDFOptions::Compression_ZIP;
				}
			}
			else
			{
				if ((compress_method == PDFOptions::Compression_JPEG) || (compress_method == PDFOptions::Compression_Auto))
				{
					if (realCMYK || !((Options.UseRGB) || (Options.UseProfiles2)))
					{
						exportToGrayscale = Options.isGrayscale;
						if (exportToGrayscale)
							exportToCMYK      = !Options.isGrayscale;
						else
							exportToCMYK      = !Options.UseRGB;
					}
					cm = PDFOptions::Compression_JPEG;
					/*if (compress_method == PDFOptions::Compression_Auto)
					{
						QFileInfo fi(tmpFile);
						if (fi.size() < im.size())
						{
							im.resize(0);
							if (!loadRawBytes(tmpFile, im))
								return false;
							cm = PDFOptions::Compression_JPEG;
						}
						else
							cm = PDFOptions::Compression_ZIP;
					}*/
				}
			}
			int bytesWritten = 0;
			PutDoc("/BitsPerComponent 8\n");
			uint lengthObj = newObject();
			PutDoc("/Length "+QString::number(lengthObj)+" 0 R\n");
			if (cm == PDFOptions::Compression_JPEG)
				PutDoc("/Filter /DCTDecode\n");
			else if (cm != PDFOptions::Compression_None)
				PutDoc("/Filter /FlateDecode\n");
//			if (exportToCMYK && (cm == PDFOptions::Compression_JPEG))
//				PutDoc("/Decode [1 0 1 0 1 0 1 0]\n");
			if (alphaM)
			{
				if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4))
					PutDoc("/SMask "+QString::number(maskObj)+" 0 R\n");
				else
					PutDoc("/Mask "+QString::number(maskObj)+" 0 R\n");
			}
			PutDoc(">>\nstream\n");
			if ((hasGrayProfile) && (doc.HasCMS) && (Options.UseProfiles2) && (!hasColorEffect))
				exportToGrayscale = true;
			if (cm == PDFOptions::Compression_JPEG)
			{
				int quality = c->OverrideCompressionQuality ? c->CompressionQualityIndex : Options.Quality;
				if (c->OverrideCompressionQuality)
					jpegUseOriginal = false;
				bytesWritten = WriteJPEGImageToStream(img, fn, imageObj, quality, exportToCMYK, exportToGrayscale, jpegUseOriginal, (!hasColorEffect && hasGrayProfile));
			}
			else if (cm == PDFOptions::Compression_ZIP)
				bytesWritten = WriteFlateImageToStream(img, imageObj, exportToCMYK, exportToGrayscale, (!hasColorEffect && hasGrayProfile));
			else
				bytesWritten = WriteImageToStream(img, imageObj, exportToCMYK, exportToGrayscale, (!hasColorEffect && hasGrayProfile));
			PutDoc("\nendstream\nendobj\n");
			if (bytesWritten <= 0)
			{
				PDF_Error_ImageWriteFailure(fn);
				return false;
			}
			StartObj(lengthObj);
			PutDoc(QString("    %1\n").arg(bytesWritten));
			PutDoc("endobj\n");
			Seite.ImgObjects[ResNam+"I"+QString::number(ResCount)] = imageObj;
			ImInfo.ResNum = ResCount;
			ImInfo.Width = img.width();
			ImInfo.Height = img.height();
			ImInfo.xa = sx;
			ImInfo.ya = sy;
			ImInfo.RequestProps = c->pixm.imgInfo.RequestProps;
		} // not embedded PDF
		if ((c->effectsInUse.count() == 0) && (!SharedImages.contains(fn)))
			SharedImages.insert(fn, ImInfo);
		ResCount++;
	}
	else
	{
		ImInfo = SharedImages[fn];
		ImInfo.sxa *= sx / ImInfo.xa;
		ImInfo.sya *= sy / ImInfo.ya;
		/*
		ImRes = SharedImages[fn].ResNum;
		ImWid = SharedImages[fn].Width;
		ImHei = SharedImages[fn].Height;
		aufl = SharedImages[fn].reso;
		sxn = SharedImages[fn].sxa * sx / SharedImages[fn].xa;
		syn = SharedImages[fn].sya * sy / SharedImages[fn].ya;
		*/
	}
	QString embedPre = "";
	if ((bitmapFromGS) || (isEmbeddedPDF)) // compensate gsResolution setting
	{
		if (isEmbeddedPDF)
		{
			// #9268 : per specs default color space is grayscale
			/*if (Options.isGrayscale)
				embedPre = "0 g 0 G";
			else if (Options.UseRGB)
				embedPre = "0 0 0 rg 0 0 0 RG";
			else
				embedPre = "0 0 0 0 k 0 0 0 0 K";*/
			embedPre  = "0 g 0 G";
			embedPre += " 1 w 0 J 0 j [] 0 d\n"; // add default graphics stack parameters pdftex relies on them
		}
		if (c->asLatexFrame())
		{
			ImInfo.sxa *= 1.0 / c->imageXScale();
			ImInfo.sya *= 1.0 / c->imageYScale();
		}
		else
		{
			ImInfo.sxa *= PrefsManager::instance()->appPrefs.extToolPrefs.gs_Resolution / 72.0;
			ImInfo.sya *= PrefsManager::instance()->appPrefs.extToolPrefs.gs_Resolution / 72.0;
		}
	}
	if (!fromAN && output)
	{
		if (c->imageRotation())
		{
			embedPre += "1 0 0 1 "+FToStr(x*sx)+" "+FToStr((-ImInfo.Height*ImInfo.sya+y*sy))+" cm\n";
			QTransform mpa;
			mpa.rotate(-c->imageRotation());
			embedPre += "1 0 0 1 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" cm\n";
			embedPre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" 0 0 cm\n";
			embedPre += "1 0 0 1 0 "+FToStr(-ImInfo.Height*ImInfo.sya)+" cm\n";
			embedPre += FToStr(ImInfo.Width*ImInfo.sxa)+" 0 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" 0 0 cm\n";
		}
		else
			embedPre += FToStr(ImInfo.Width*ImInfo.sxa)+" 0 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" "+FToStr(x*sx)+" "+FToStr((-ImInfo.Height*ImInfo.sya+y*sy))+" cm\n";
		*output = QString(embedPre + "/"+ResNam+"I"+QString::number(ImInfo.ResNum)+" Do\n");
//		*output = QString(embedPre + FToStr(ImInfo.Width*ImInfo.sxa)+" 0 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" "+FToStr(x*sx)+" "+FToStr((-ImInfo.Height*ImInfo.sya+y*sy))+" cm\n/"+ResNam+"I"+QString::number(ImInfo.ResNum)+" Do\n");
	}
	else if (output)
		*output = QString("");
	return true;
}

bool PDFLibCore::PDF_End_Doc(const QString& PrintPr, const QString& Name, int Components)
{
	QString tmp;
	int ResO;
	BookMItem* ip;
	QTreeWidgetItem* pp;
	QString Inhal = "";
	QMap<int,QString> Inha;
	if ((Bvie->topLevelItemCount() != 0) && (Options.Bookmarks) && (BookMinUse))
	{
		int Basis = ObjCounter - 1;
		Outlines.Count = Bvie->topLevelItemCount();
		ip = (BookMItem*)Bvie->topLevelItem(0);
		pp = Bvie->topLevelItem(0);
		Outlines.First = ip->ItemNr+Basis;
		Outlines.Last  = ((BookMItem*) Bvie->topLevelItem(Outlines.Count - 1))->ItemNr+Basis;
		QTreeWidgetItemIterator it(Bvie);
		while (*it)
		{
			ip = (BookMItem*)(*it);
			QString encText = ip->text(0);
			Inhal  = QString::number(ip->ItemNr+Basis)+ " 0 obj\n";
			Inhal += "<<\n/Title "+EncStringUTF16("("+encText+")", ip->ItemNr+Basis)+"\n";
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
			if (ip->childCount())
				Inhal += "/Count -"+QString::number(ip->childCount())+"\n";
			if ((ip->PageObject->OwnPage != -1) && PageTree.Kids.contains(ip->PageObject->OwnPage))
			{
				QString action = ip->Action;
				if (action.isEmpty())
				{
					const Page* page = doc.DocPages.at(ip->PageObject->OwnPage);
					double actionPos = page->height() - (ip->PageObject->yPos() - page->yOffset());
					action = QString("/XYZ 0 %1 0").arg(actionPos);
				}
				Inhal += "/Dest ["+QString::number(PageTree.Kids[ip->PageObject->OwnPage])+" 0 R "+action+"\n";
			}
			Inhal += ">>\nendobj\n";
			Inha[ip->ItemNr] = Inhal;
			++it;
		}
		QMap<int,QString> ::ConstIterator contentIt;
		for (contentIt = Inha.begin(); contentIt != Inha.end(); ++contentIt)
		{
			XRef.append(bytesWritten());
			PutDoc(contentIt.value());
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
			PutDoc("/"+it.key()+" "+QString::number(it.value())+" 0 R\n");
		QMap<QString,int>::Iterator iti;
		for (iti = Seite.XObjects.begin(); iti != Seite.XObjects.end(); ++iti)
			PutDoc("/"+iti.key()+" "+QString::number(iti.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Seite.FObjects.count() != 0)
	{
		PutDoc("/Font << \n");
		QMap<QString,int>::Iterator it2;
		for (it2 = Seite.FObjects.begin(); it2 != Seite.FObjects.end(); ++it2)
			PutDoc("/"+it2.key()+" "+QString::number(it2.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Shadings.count() != 0)
	{
		PutDoc("/Shading << \n");
		QMap<QString,int>::Iterator it3;
		for (it3 = Shadings.begin(); it3 != Shadings.end(); ++it3)
			PutDoc("/"+it3.key()+" "+QString::number(it3.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Patterns.count() != 0)
	{
		PutDoc("/Pattern << \n");
		QMap<QString,int>::Iterator it3p;
		for (it3p = Patterns.begin(); it3p != Patterns.end(); ++it3p)
			PutDoc("/"+it3p.key()+" "+QString::number(it3p.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if (Transpar.count() != 0)
	{
		PutDoc("/ExtGState << \n");
		QMap<QString,int>::Iterator it3t;
		for (it3t = Transpar.begin(); it3t != Transpar.end(); ++it3t)
			PutDoc("/"+it3t.key()+" "+QString::number(it3t.value())+" 0 R\n");
		PutDoc(">>\n");
	}
	if ((ICCProfiles.count() != 0) || (spotMap.count() != 0) || (spotMapReg.count() != 0))
	{
		PutDoc("/ColorSpace << \n");
		QMap<QString,ICCD>::Iterator it3c;
		if (ICCProfiles.count() != 0)
		{
			for (it3c = ICCProfiles.begin(); it3c != ICCProfiles.end(); ++it3c)
				PutDoc("/"+it3c.value().ResName+" "+QString::number(it3c.value().ResNum)+" 0 R\n");
		}
		QMap<QString,SpotC>::Iterator it3sc;
		if (spotMap.count() != 0)
		{
			for (it3sc = spotMap.begin(); it3sc != spotMap.end(); ++it3sc)
				PutDoc("/"+it3sc.value().ResName+" "+QString::number(it3sc.value().ResNum)+" 0 R\n");
		}
		QMap<QString,SpotC>::Iterator it3scr;
		if (spotMapReg.count() != 0)
		{
			for (it3scr = spotMapReg.begin(); it3scr != spotMapReg.end(); ++it3scr)
				PutDoc("/"+it3scr.value().ResName+" "+QString::number(it3scr.value().ResNum)+" 0 R\n");
		}
		PutDoc(">>\n");
	}
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4))&& (Options.useLayers))
	{
		PutDoc("/Properties <<\n");
		ScLayer ll;
		ll.isPrintable = false;
		ll.ID = 0;
		int Lnr = 0;
		for (int la = 0; la < doc.Layers.count(); ++la)
		{
			doc.Layers.levelToLayer(ll, la);
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
// 	if ((Bvie->childCount() != 0) && (Options.Bookmarks))
	if ((Bvie->topLevelItemCount() != 0) && (Options.Bookmarks))
	{
		PutDoc("/First "+QString::number(Outlines.First)+" 0 R\n");
		PutDoc("/Last "+QString::number(Outlines.Last)+" 0 R\n");
	}
	PutDoc(">>\nendobj\n");
	XRef[3] = bytesWritten();
	PutDoc("4 0 obj\n<<\n/Type /Pages\n/Kids [");
	QMap<int, int>::Iterator kidsIt;
	for (kidsIt = PageTree.Kids.begin(); kidsIt != PageTree.Kids.end(); ++kidsIt)
		PutDoc(QString::number(kidsIt.value())+" 0 R ");
	PutDoc("]\n");
	PutDoc("/Count "+QString::number(PageTree.Count)+"\n");
	PutDoc("/Resources "+QString::number(ObjCounter-1)+" 0 R\n");
	PutDoc(">>\nendobj\n");
	XRef[4] = bytesWritten();
	PutDoc("5 0 obj\n<<\n");
	if (NamedDest.count() != 0)
	{
		QList<Dest>::Iterator vt;
		for (vt = NamedDest.begin(); vt != NamedDest.end(); ++vt)
		{
			if (PageTree.Kids.contains((*vt).Seite))
				PutDoc("/"+(*vt).Name+" ["+QString::number(PageTree.Kids[(*vt).Seite])+" 0 R /XYZ "+(*vt).Act+"]\n");
		}
	}
	PutDoc(">>\nendobj\n");
	XRef[5] = bytesWritten();
	PutDoc("6 0 obj\n<<\n");
	PutDoc("/Fields [ ");
	if (Seite.FormObjects.count() != 0)
	{
		for (int fo = 0; fo < Seite.FormObjects.count(); ++fo)
			PutDoc(QString::number(Seite.FormObjects[fo])+" 0 R ");
	}
	PutDoc(" ]\n");
	if (CalcFields.count() != 0)
	{
		PutDoc("/CO [ ");
		for (int foc = 0; foc < CalcFields.count(); ++foc)
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
			WritePDFString(itja0.value());
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
		for (int ele = 0; ele < doc.Items->count(); ++ele)
		{
			PageItem* tel = doc.Items->at(ele);
			if ((tel->asTextFrame()) && (tel->prevInChain() == 0) && (tel->nextInChain() != 0) &&
					(!tel->inPdfArticle))
			{
				Beads.clear();
				struct Bead bd;
				int fir = ObjCounter + 1;
				int ccb = ObjCounter + 1;
				bd.Parent = ObjCounter;
				while (tel->nextInChain() != 0)
				{
					if ((tel->OwnPage != -1) && PageTree.Kids.contains(tel->OwnPage))
					{
						bd.Next = ccb + 1;
						bd.Prev = ccb - 1;
						ccb++;
						bd.Page = PageTree.Kids[tel->OwnPage];
						bd.Recht = QRect(static_cast<int>(tel->xPos() - doc.DocPages.at(tel->OwnPage)->xOffset()),
									static_cast<int>(doc.DocPages.at(tel->OwnPage)->height() - (tel->yPos()  - doc.DocPages.at(tel->OwnPage)->yOffset())),
									static_cast<int>(tel->width()),
									static_cast<int>(tel->height()));
						Beads.append(bd);
					}
					tel->inPdfArticle = true;
					tel = tel->nextInChain();
				}
				bd.Next = ccb + 1;
				bd.Prev = ccb - 1;
				if ((tel->OwnPage != -1) && PageTree.Kids.contains(tel->OwnPage))
				{
					bd.Page = PageTree.Kids[tel->OwnPage];
					bd.Recht = QRect(static_cast<int>(tel->xPos() - doc.DocPages.at(tel->OwnPage)->xOffset()),
								static_cast<int>(doc.DocPages.at(tel->OwnPage)->height() - (tel->yPos()  - doc.DocPages.at(tel->OwnPage)->yOffset())),
								static_cast<int>(tel->width()),
								static_cast<int>(tel->height()));
					Beads.append(bd);
				}
				tel->inPdfArticle = true;
				if (Beads.count() > 0)
				{
					int threadObj = newObject();
					StartObj(threadObj);
					Threads.append(threadObj);
					PutDoc("<< /Type /Thread\n");
					PutDoc("   /F "+QString::number(threadObj + 1)+" 0 R\n");
					PutDoc(">>\nendobj\n");
					Beads[0].Prev = fir + Beads.count()-1;
					Beads[Beads.count()-1].Next = fir;
				}
				for (int beac = 0; beac < Beads.count(); ++beac)
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
		for (int ele = 0; ele < doc.Items->count(); ++ele)
			doc.Items->at(ele)->inPdfArticle = false;
	}
	XRef[7] = bytesWritten();
	PutDoc("8 0 obj\n[");
	for (int th = 0; th < Threads.count(); ++th)
		PutDoc(QString::number(Threads[th])+" 0 R ");
	PutDoc("]\nendobj\n");
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
	{
		XRef[8] = bytesWritten();
		QStringList lay;
		PutDoc("9 0 obj\n<<\n");
		PutDoc("/D << /Order [ ");
		ScLayer ll;
		ll.isPrintable = false;
		ll.ID = 0;
		int Lnr = 0;
		for (int la = 0; la < doc.Layers.count(); ++la)
		{
			doc.Layers.levelToLayer(ll, la);
			if (ll.isEditable)
				lay.prepend(QString::number(OCGEntries[ll.Name].ObjNum)+" 0 R ");
			Lnr++;
		}
		for (int layc = 0; layc < lay.count(); ++layc)
		{
			if (Options.Version != PDFOptions::PDFVersion_X4)
				PutDoc(lay[layc]);
		}
		PutDoc("]\n");
		if (Options.Version == PDFOptions::PDFVersion_X4)
		{
			PutDoc("/BaseState /ON\n");
			QString occdName = "Default";
			PutDoc("/Name ("+PDFEncode(occdName)+")\n");
		}
		PutDoc("/OFF [ ");
		QHash<QString, OCGInfo>::Iterator itoc;
		for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
		{
			if (!itoc.value().visible)
				PutDoc(QString::number(itoc.value().ObjNum)+" 0 R ");
		}
		PutDoc("]\n");
		if (Options.Version != PDFOptions::PDFVersion_X4)
		{
			PutDoc("/AS [<</Event /Print /OCGs [ ");
			for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
			{
				PutDoc(QString::number(itoc.value().ObjNum)+" 0 R ");
			}
			PutDoc("] /Category [/Print]>> <</Event /View /OCGs [");
			for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
			{
				PutDoc(QString::number(itoc.value().ObjNum)+" 0 R ");
			}
			PutDoc("] /Category [/View]>>]\n");
		}
		PutDoc(">>\n");
		PutDoc("/OCGs [ ");
		for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
		{
			PutDoc(QString::number(itoc.value().ObjNum)+" 0 R ");
		}
		PutDoc("]\n");
		PutDoc(">>\nendobj\n");
	}
	if ((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || (Options.Version == PDFOptions::PDFVersion_X4))
	{
		StartObj(ObjCounter);
		ObjCounter++;
		QByteArray dataP;
		loadRawBytes(PrintPr, dataP);
		PutDoc("<<\n");
		if (Options.Compress)
		{
			QByteArray compData = CompressArray(dataP);
			if (compData.size() > 0)
			{
				PutDoc("/Filter /FlateDecode\n");
				dataP = compData;
			}
		}
		PutDoc("/Length "+QString::number(dataP.size()+1)+"\n");
		PutDoc("/N "+QString::number(Components)+"\n");
		PutDoc(">>\nstream\n");
		PutDoc(dataP);
		PutDoc("\nendstream\nendobj\n");

		if ((Options.Version == PDFOptions::PDFVersion_X4) && (Options.useLayers))
		{
			XRef[9] = bytesWritten();
			PutDoc("10 0 obj\n");
		}
		if ((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || ((Options.Version == PDFOptions::PDFVersion_X4) && !(Options.useLayers)))
		{
			XRef[8] = bytesWritten();
			PutDoc("9 0 obj\n");
		}

		PutDoc("<<\n/Type /OutputIntent\n/S /GTS_PDFX\n");
		PutDoc("/DestOutputProfile "+QString::number(ObjCounter-1)+" 0 R\n");
		PutDoc("/OutputConditionIdentifier (Custom)\n");
		PutDoc("/Info ("+PDFEncode(Options.Info)+")\n");
		PutDoc("/OutputCondition ("+PDFEncode(Name)+")\n");
		PutDoc(">>\nendobj\n");
	}
	if (Options.Version == PDFOptions::PDFVersion_X4)
	{
		if (Options.useLayers) // OCProperties dictionary was included as '9 0 obj', OutputIntents was included as '10 0 obj'
		{
			XRef[10] = bytesWritten();
			PutDoc("11 0 obj\n");
		}
		else // There was no OCProperties dictionary
		{
			XRef[9] = bytesWritten();
			PutDoc("10 0 obj\n");
		}
		PutDoc("<<\n");
		PutDoc("/Length "+QString::number(xmpPacket.size()+1)+"\n");
		PutDoc("/Type /Metadata\n");
		PutDoc("/Subtype /XML\n");
		PutDoc(">>\nstream\n");
		PutDoc(xmpPacket);
		PutDoc("\nendstream\nendobj\n");
	}
	uint StX = bytesWritten();
	PutDoc("xref\n");
	PutDoc("0 "+QString::number(ObjCounter)+"\n");
	PutDoc("0000000000 65535 f \n");
	for (int a = 0; a < XRef.count(); ++a)
	{
		if (XRef[a] > 0)
		{
			tmp.sprintf("%10d", XRef[a]);
			tmp.replace(QRegExp(" "), "0");
			PutDoc(tmp+" 00000 n \n");
		}
		else
		{
			// unused object, mark as free-never-to-be-used-again
			PutDoc("0000000000 65535 f \n");
		}
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
	return closeAndCleanup();
}

void PDFLibCore::generateXMP(const QString& timeStamp)
{
	/*
		This is a rather 'manual' way to generate XMP. Since only a few basic properties are included here,
		this method tries to build XMP from the generic XML's API of Qt and follow the XMP's spec very closely.
		A better (and less typing) way to support XMP is to use an XMP's API such as Exiv2 or Exempi
	*/
	QDomDocument xmpDoc;
	QDomProcessingInstruction xpacket = xmpDoc.createProcessingInstruction("xpacket", "begin=\"\" id=\"W5M0MpCehiHzreSzNTczkc9d\"");
	xmpDoc.appendChild(xpacket);
	QString xNS = "adobe:ns:meta/";
	QDomElement xmpmeta = xmpDoc.createElementNS(xNS, "x:xmpmeta");
	xmpmeta.setAttributeNS(xNS, "x:xmptk", "Scribus PDF Library "+QString(VERSION));
	xmpDoc.appendChild(xmpmeta);
	QString rdfNS = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
	QDomElement rdf = xmpDoc.createElementNS(rdfNS, "rdf:RDF");
	xmpmeta.appendChild(rdf);
	QDomElement desc = xmpDoc.createElement("rdf:Description");
	desc.setAttribute("rdf:about", "");

	QDomElement descXMP = desc.cloneNode().toElement();
	rdf.appendChild(descXMP);
	QString xmpNS = "http://ns.adobe.com/xap/1.0/";
	descXMP.setAttributeNS(xmpNS, "xmp:CreateDate", timeStamp);
	descXMP.setAttribute("xmp:ModifyDate", timeStamp);
	descXMP.setAttribute("xmp:MetadataDate", timeStamp);
	descXMP.setAttribute("xmp:CreatorTool", "Scribus "+QString(VERSION));

	QDomElement descPDF = desc.cloneNode().toElement();
	rdf.appendChild(descPDF);
	QString pdfNS = "http://ns.adobe.com/pdf/1.3/";
	descPDF.setAttributeNS(pdfNS, "pdf:Producer", "Scribus PDF Library "+QString(VERSION));
	descPDF.setAttribute("pdf:Trapped", "False");
	descPDF.setAttribute("pdf:Keywords", doc.documentInfo().keywords());

	QDomElement descDC = desc.cloneNode().toElement();
	rdf.appendChild(descDC);
	QString dcNS = "http://purl.org/dc/elements/1.1/";
	descDC.setAttributeNS(dcNS, "dc:format", "application/pdf");
	QDomElement title = xmpDoc.createElement("dc:title");
	descDC.appendChild(title);
	QDomElement alt1 = xmpDoc.createElement("rdf:Alt");
	title.appendChild(alt1);
	QDomElement li1 = xmpDoc.createElement("rdf:li");
	li1.setAttribute("xml:lang", "x-default");
	alt1.appendChild(li1);
	QString docTitle = doc.documentInfo().title();
	if (((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || (Options.Version == PDFOptions::PDFVersion_X4)) && (docTitle.isEmpty()))
		docTitle = doc.DocName;
	li1.appendChild(xmpDoc.createTextNode(docTitle));
	QDomElement creator = xmpDoc.createElement("dc:creator");
	descDC.appendChild(creator);
	QDomElement seq = xmpDoc.createElement("rdf:Seq");
	creator.appendChild(seq);
	QDomElement li2 = xmpDoc.createElement("rdf:li");
	seq.appendChild(li2);
	li2.appendChild(xmpDoc.createTextNode(doc.documentInfo().author()));
	// Subject's entry in Document Info dictionary is actually dc:description in XMP, not dc:subject.
	QDomElement description = xmpDoc.createElement("dc:description");
	descDC.appendChild(description);
	QDomElement alt2 = xmpDoc.createElement("rdf:Alt");
	description.appendChild(alt2);
	QDomElement li3 = xmpDoc.createElement("rdf:li");
	li3.setAttribute("xml:lang", "x-default");
	alt2.appendChild(li3);
	li3.appendChild(xmpDoc.createTextNode(doc.documentInfo().subject()));

	if ((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || (Options.Version == PDFOptions::PDFVersion_X4))
	{
		QDomElement descPDFXID = desc.cloneNode().toElement();
		rdf.appendChild(descPDFXID);
		QString pdfxidNS = "http://www.npes.org/pdfx/ns/id/";
		if (Options.Version == PDFOptions::PDFVersion_X1a)
		{
			descPDFXID.setAttributeNS(pdfxidNS, "pdfx:GTS_PDFXConformance", "PDF/X-1a:2001");
			descPDFXID.setAttribute("pdfx:GTS_PDFXVersion", "PDF/X-1:2001");
		}
		else if (Options.Version == PDFOptions::PDFVersion_X3)
			descPDFXID.setAttributeNS(pdfxidNS, "pdfxid:GTS_PDFXVersion", "PDF/X-3");
		else if (Options.Version == PDFOptions::PDFVersion_X4)
			descPDFXID.setAttributeNS(pdfxidNS, "pdfxid:GTS_PDFXVersion", "PDF/X-4");
	}


	QDomElement descXMPMM = desc.cloneNode().toElement();
	rdf.appendChild(descXMPMM);
	QString xmpmmNS = "http://ns.adobe.com/xap/1.0/mm/";
	QString uuid = QUuid::createUuid().toString();
	// remove the enclosing braces
	uuid.remove(0, 1);
	uuid.chop(1);
	descXMPMM.setAttributeNS(xmpmmNS, "xmpMM:DocumentID", "uuid:"+uuid);
	descXMPMM.setAttribute("xmpMM:RenditionClass", "default");
	descXMPMM.setAttribute("xmpMM:VersionID", 1);

	xmpPacket.append(xmpDoc.toString(4));
	QString tenSpaces = "          ";
	for (int i=0; i<25; ++i)
	{
		for (int j=0; j<10; ++j)
			xmpPacket.append(tenSpaces);
		xmpPacket.append("\n");
	}
	xmpPacket.append("<?xpacket end='w'?>");
}

void PDFLibCore::PDF_Error(const QString& errorMsg)
{
	ErrorMessage = errorMsg;
	if (!ScCore->usingGUI())
		qDebug("%s", errorMsg.toLocal8Bit().data());
}

void PDFLibCore::PDF_Error_WriteFailure(void)
{
	PDF_Error( tr("A write error occurred, please check available disk space") );
}

void PDFLibCore::PDF_Error_ImageLoadFailure(const QString& fileName)
{
	PDF_Error( tr("Failed to load an image : %1").arg(fileName) );
}

void PDFLibCore::PDF_Error_ImageWriteFailure(const QString& fileName)
{
	PDF_Error( tr("Failed to write an image : %1").arg(fileName) );
}

void PDFLibCore::PDF_Error_MaskLoadFailure(const QString& fileName)
{
	PDF_Error( tr("Failed to load an image mask : %1").arg(fileName) );
}

void PDFLibCore::PDF_Error_InsufficientMemory(void)
{
	PDF_Error( tr("Insufficient memory for processing an image"));
}

bool PDFLibCore::closeAndCleanup()
{
	bool writeSucceed = (Spool.error() == QFile::NoError);
	if (!writeSucceed)
		PDF_Error_WriteFailure();
	Spool.close();
	if (abortExport || !writeSucceed)
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
	return writeSucceed;
}

void PDFLibCore::cancelRequested()
{
	abortExport=true;
}
