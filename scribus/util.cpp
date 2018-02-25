/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include <algorithm>
#include "util.h"
#include <zlib.h>
#include <qglobal.h>
#include <QApplication>
#include <QCryptographicHash>
#include <QDomElement>
#include <QMessageBox>
#include <QProcess>

#include "pageitem.h"
#include "pageitem_table.h"
#include "scribusview.h"
#include "scribusdoc.h"
#include "scpainter.h"
#include "ui/scmessagebox.h"

#include <signal.h>

#if !defined(_WIN32) && !defined(Q_OS_MAC) 
#include <execinfo.h>
#include <cxxabi.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#endif

using namespace std;

QString cleanupLang(const QString& lang)
{
	int dotIndex = lang.indexOf(QChar('.'));
	if (dotIndex < 0)
		return lang;

	QString cleanLang = lang.left(dotIndex);
	return cleanLang;
}

int System(const QString exename, const QStringList & args, const QString fileStdErr, const QString fileStdOut, bool* cancel)
{
	QProcess proc;
	if (!fileStdOut.isEmpty())
		proc.setStandardOutputFile(fileStdOut);
	if (!fileStdErr.isEmpty())
		proc.setStandardErrorFile(fileStdErr);
	proc.start(exename, args);
	if (proc.waitForStarted(15000))
	{
		while (!proc.waitForFinished(15000))
		{
			qApp->processEvents();
			if (cancel && (*cancel == true))
			{
				proc.kill();
				break;
			}
		}
	}
	if (cancel && (*cancel == true))
		return -1;
	return proc.exitCode();
}

// On Windows, return short path name, else return longPath;
QString getShortPathName(const QString & longPath)
{
	QString shortPath(longPath);
#if defined _WIN32
	QFileInfo fInfo(longPath);
	if (fInfo.exists())
	{
		WCHAR shortName[MAX_PATH + 1];
		// An error should not be blocking as ERROR_INVALID_PARAMETER can simply mean
		// that volume does not support 8.3 filenames, so return longPath in this case
		QString nativePath = QDir::toNativeSeparators(longPath);
		int ret = GetShortPathNameW((LPCWSTR) nativePath.utf16(), shortName, MAX_PATH);
		if (ret != ERROR_INVALID_PARAMETER && ret < MAX_PATH)
			shortPath = QString::fromUtf16((const ushort*) shortName);
	}
#endif
	return shortPath;
}

// On Windows, return short path name, else return longPath;
QString getLongPathName(const QString & shortPath)
{
	QString longPath(shortPath);
#if defined _WIN32
	QFileInfo fInfo(longPath);
	if (fInfo.exists())
	{
		WCHAR longName[MAX_PATH + 1];
		// An error should not be blocking as ERROR_INVALID_PARAMETER can simply mean
		// that volume does not support long filenames, so return shortPath in this case
		QString nativePath = QDir::toNativeSeparators(shortPath);
		int ret = GetLongPathNameW((LPCWSTR) nativePath.utf16(), longName, MAX_PATH);
		if (ret != ERROR_INVALID_PARAMETER && ret < MAX_PATH)
			longPath = QString::fromUtf16((const ushort*) longName);
	}
#endif
	return longPath;
}

// Legacy implementation of LoadText with incorrect
// handling of unicode data. This should be retired.
// Use loadRawText instead.
// FIXME XXX
//
bool loadText(QString filename, QString *Buffer)
{
	QFile f(filename);
	QFileInfo fi(f);
	if (!fi.exists())
		return false;
	bool ret;
	QByteArray bb(f.size(), ' ');
	if (f.open(QIODevice::ReadOnly))
	{
		f.read(bb.data(), f.size());
		f.close();
		for (int posi = 0; posi < bb.size(); ++posi)
			*Buffer += QChar(bb[posi]);
		/*
		int len = bb.size();
		int oldLen = Buffer->length();
		Buffer->setLength( oldLen + len + 1);
		// digged into Qt 3.3 sources to find that. Might break in Qt 4 -- AV
		unsigned short * ucsString = const_cast<unsigned short *>(Buffer->ucs2()) + oldLen;
		char * data = bb.data();
		for (uint posi = 0; posi < len; ++posi)
		*ucsString++ = *data++;
		*ucsString = 0;
		*/
		ret = true;
	}
	else
		ret = false;
	return ret;
}

bool loadRawText(const QString & filename, QByteArray & buf)
{
	bool ret = false;
	QFile f(filename);
	QFileInfo fi(f);
	if (fi.exists())
	{
		// Allocating one more bytes for null terminator is unneeded with Qt4
		// as QByteArray ensure null termination automatically
		// Triggers also QDomDocument parsing errors
		QByteArray tempBuf(f.size() /*+ 1*/, ' ');
		if (f.open(QIODevice::ReadOnly))
		{
			unsigned int bytesRead = f.read(tempBuf.data(), f.size());
			/*tempBuf[bytesRead] = '\0';*/
			ret = bytesRead == f.size();
			if (ret)
				buf = tempBuf; // sharing makes this efficient
		}
	}
	if (f.isOpen())
		f.close();
	return ret;
}

bool loadRawBytes(const QString & filename, QByteArray & buf)
{
	bool ret = false;
	QFile f(filename);
	QFileInfo fi(f);
	if (fi.exists())
	{
		QByteArray tempBuf(f.size(), ' ');
		if (f.open(QIODevice::ReadOnly))
		{
			unsigned int bytesRead = f.read(tempBuf.data(), f.size());
			ret = bytesRead == f.size();
			if (ret)
				buf = tempBuf; // sharing makes this efficient
		}
	}
	if (f.isOpen())
		f.close();
	return ret;
}


QString CompressStr(QString *in)
{
	QString out = "";
	QByteArray bb(in->length(), ' ');
	if (bb.size() == in->length())
	{
		for (int ax = 0; ax < in->length(); ++ax)
		{
			// bb.insert(ax, in->at(ax)); JG monstruously inefficient due to frequent memory reallocation
			bb[ax] = in->at(ax).cell();
			assert(in->at(ax).row() == 0);
		}
		uLong exlen = (uLong)(bb.size() * 0.001 + 16) + bb.size();
		QByteArray bc(exlen, ' ');
		if( bc.size() == static_cast<qint32>(exlen) )
		{
			int errcode = compress2((Byte *)bc.data(), &exlen, (Byte *)bb.data(), uLong(bb.size()), 9);
			if (errcode != Z_OK)
			{
				qDebug("compress2 failed with code %i", errcode);
				out = *in;
			}
			else {
				for (uint cl = 0; cl < exlen; ++cl)
					out += QChar(bc[cl]);
			}
		}
		else
		{
			qDebug("insufficient memory to allocate %i bytes", in->length());
			out = *in;
		}
	}
	else
	{
		qDebug("insufficient memory to allocate %i bytes", in->length());
		out = *in;
	}
	return out;
}

QByteArray CompressArray(const QByteArray& in)
{
	QByteArray out;
	uLong exlen = uint(in.size() * 0.001 + 16) + in.size();
	QByteArray temp(exlen, ' ');
	int errcode = compress2((Byte *)temp.data(), &exlen, (Byte *)in.data(), uLong(in.size()), 9);
	if (errcode == Z_OK)
	{
		temp.resize(exlen);
		out = temp;
	}
	else
		qDebug("compress2 failed with code %i", errcode);
	return out;
}

char *toAscii85( quint32 value, bool& allZero )
{
	int digit;
	static char asciiVal[6];
	allZero = true;
	for (int i = 0; i < 5; ++i)
	{
		digit = value % 85;
		if (digit != 0)
			allZero = false;
		asciiVal[4-i] = digit + 33;
		value = (value - digit) / 85;
	}
	asciiVal[5] = 0;
	return asciiVal;
}

char *toHex( uchar u )
{
	static char hexVal[3];
	int i = 1;
	while ( i >= 0 )
	{
		ushort hex = (u & 0x000f);
		if ( hex < 0x0a )
			hexVal[i] = '0'+hex;
		else
			hexVal[i] = 'A'+(hex-0x0a);
		u = u >> 4;
		i--;
	}
	hexVal[2] = '\0';
	return hexVal;
}

QString String2Hex(QString *in, bool lang)
{
	int i = 0;
	QString out;
	for( int xi = 0; xi < in->length(); ++xi )
	{
		// Qt4 .cell() added ???
		out += toHex(QChar(in->at(xi)).cell());
		++i;
		if ((i>40) && (lang))
		{
			out += '\n';
			i=0;
		}
	}
	return out;
}

QString Path2Relative(QString Path, const QString& baseDir)
{
	QDir d(baseDir);
	return d.relativeFilePath(Path);
}

QString Relative2Path(QString File, const QString& baseDir)
{
	QString   absPath;
	QFileInfo fi(File);
	if (File.isEmpty())
		absPath = File;
	else if (fi.isRelative())
	{
		QDir d(baseDir);
		absPath = d.absoluteFilePath(File);
		absPath = QDir::cleanPath(absPath);
	}
	else
		absPath = File;
	return absPath;
}

/***************************************************************************
    begin                : Wed Oct 29 2003
    copyright            : (C) 2003 The Scribus Team
    email                : paul@all-the-johnsons.co.uk
 ***************************************************************************/
// check if the file exists, if it does, ask if they're sure
// return true if they're sure, else return false;

bool overwrite(QWidget *parent, QString filename)
{
	bool retval = true;
	QFileInfo fi(filename);
	if (fi.exists())
	{
		QString fn = QDir::toNativeSeparators(filename);
		int t = ScMessageBox::warning(parent, QObject::tr("File exists"),
									 "<qt>"+ QObject::tr("A file named '%1' already exists.<br/>Do you want to replace it with the file you are saving?").arg(fn) +"</qt>",
											 QMessageBox::Ok | QMessageBox::Cancel,
											 QMessageBox::Cancel,	// GUI default
											 QMessageBox::Ok);	// batch default
		if (t == QMessageBox::Cancel)
			retval = false;
	}
	return retval;
}

void WordAndPara(PageItem* currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN)
{
	QChar Dat = QChar(32);
	int para = 0;
	int ww = 0;
	int cc = 0;
	int paraN = 0;
	int wwN = 0;
	int ccN = 0;
	bool first = true;
	PageItem *nextItem = currItem;
	PageItem *nbl = currItem;
	while (nextItem != 0)
	{
		if (nextItem->prevInChain() != 0)
			nextItem = nextItem->prevInChain();
		else
			break;
	}
	while (nextItem != 0)
	{
		for (int a = qMax(nextItem->firstInFrame(),0); a <= nextItem->lastInFrame() && a < nextItem->itemText.length(); ++a)
		{
			QChar b = nextItem->itemText.text(a);
			if (b == SpecialChars::PARSEP)
			{
				para++;
			}
			if ((!b.isLetterOrNumber()) && (Dat.isLetterOrNumber()) && (!first))
			{
				ww++;
			}
			if (b.isSurrogate())
				++a;
			cc++;
			Dat = b;
			first = false;
		}
		nbl = nextItem;
		nextItem = nextItem->nextInChain();
	}
	if (nbl->frameOverflows()) {
		paraN++;
		for (int a = nbl->lastInFrame()+1; a < nbl->itemText.length(); ++a)
		{
			QChar b = nbl->itemText.text(a);
			if (b == SpecialChars::PARSEP)
			{
				paraN++;
			}
			if ((!b.isLetterOrNumber()) && (Dat.isLetterOrNumber()) && (!first))
			{
				wwN++;
			}
			if (b.isSurrogate())
				++a;
			ccN++;
			Dat = b;
			first = false;
		}
	}		
	else {
		para++;
	}
	if (Dat.isLetterOrNumber())
	{
		if (nbl->frameOverflows())
			wwN++;
		else
			ww++;
	}
	*w = ww;
	*p = para;
	*c = cc;
	*wN = wwN;
	*pN = paraN;
	*cN = ccN;
}

void ReOrderText(ScribusDoc *currentDoc, ScribusView *view)
{
	double savScale = view->scale();
	view->setScale(1.0);
	currentDoc->RePos = true;
	QImage pgPix(10, 10, QImage::Format_ARGB32_Premultiplied);
	QRect rd; // = QRect(0,0,9,9);
	ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
	for (QList<PageItem*>::iterator it = currentDoc->MasterItems.begin(); it != currentDoc->MasterItems.end(); ++it)
	{
		PageItem* currItem = *it;
		if (currItem->itemType() == PageItem::PathText)
			currItem->DrawObj(painter, rd);
	}
	for (QList<PageItem*>::iterator it = currentDoc->Items->begin(); it != currentDoc->Items->end(); ++it)
	{
		PageItem* currItem = *it;
		currItem->layout();
		if (currItem->itemType() == PageItem::PathText)
			currItem->DrawObj(painter, rd); //FIXME: this should be replaced by code in layout()
	}
	currentDoc->RePos = false;
	view->setScale(savScale);
	delete painter;
}

/*! \brief Helper function for sorting in sortQStringList.
\author 10/06/2004 - pv
\param s1 first string
\param s2 second string
\retval bool t/f related s1>s2
 */
bool compareQStrings(QString s1, QString s2)
{
	if (QString::localeAwareCompare(s1, s2) >= 0)
		return false;
	return true;
}

QStringList sortQStringList(QStringList aList)
{
	std::vector<QString> sortList;
	QStringList retList;
	QStringList::Iterator it;
	for (it = aList.begin(); it != aList.end(); ++it)
		sortList.push_back(*it);
	std::sort(sortList.begin(), sortList.end(), compareQStrings);
	for(uint i = 0; i < sortList.size(); i++)
		retList.append(sortList[i]);
	return retList;
}

bool sortingQPairOfStrings( QPair<QString, QString> aP, QPair<QString, QString> bP)
{
	if (aP.first == bP.first)
		return (aP.second < bP.second);
	return (aP.first < bP.first);
}

QString checkFileExtension(const QString &currName, const QString &extension)
{
	QString newName(currName);
	//If filename ends with a period, just add the extension
	if (newName.right(1)==".")
	{
		newName+=extension.toLower();
		return newName;
	}
	//If filename doesn't end with the period+extension, add it on
	QString dotExt("." + extension.toLower());
	if (!newName.endsWith(dotExt, Qt::CaseInsensitive))
		newName+=dotExt;
	return newName;
}

QString getFileNameByPage(ScribusDoc* currDoc, uint pageNo, QString extension, QString prefix)
{
	uint number = pageNo + currDoc->FirstPnum;
	QString defaultName;
	if (prefix!=QString::null)
		defaultName=prefix;
	else
		defaultName=currDoc->DocName;
	if (defaultName.isNull())
		defaultName = "export";
	else
	{
		QFileInfo fi(defaultName);
		defaultName = fi.completeBaseName();
	}
	return QString("%1-%2%3.%4").arg(defaultName).arg(QObject::tr("page", "page export")).arg(number, 3, 10, QChar('0')).arg(extension);
}

const QString getStringFromSequence(NumFormat type, uint position, QString asterix)
{
	QString retVal("");

	const QString english("abcdefghijklmnopqrstuvwxyz");
	const QString arabic("أبتثجحخدذرزسشصضطظعغفقكلمنهوي");
	const QString abjad("أبجدهوزحطيكلمنسعفصقرشتثخذضظغ");

	switch( type )
	{
		case Type_1_2_3:
			retVal=QString::number(position);
			break;
		case Type_1_2_3_ar:
			retVal=QLocale("ar").toString(position);
			break;
		case Type_A_B_C:
			retVal = numberToLetterSequence(english, position).toUpper();
			break;
		case Type_a_b_c:
			retVal = numberToLetterSequence(english, position);
			break;
		case Type_Alphabet_ar:
			retVal = numberToLetterSequence(arabic, position);
			break;
		case Type_Abjad_ar:
			retVal = numberToLetterSequence(abjad, position);
			break;
		case Type_I_II_III:
			retVal = numberToRoman(position);
			break;
		case Type_i_ii_iii:
			//well, for lower case people will want that, even if its "wrong"
			//ie, X=10, x=10000
			retVal = numberToRoman(position).toLower();
			break;
		case Type_asterix:
			for (uint a=1; a <= position; ++a)
				retVal.append(asterix);
			break;
		case Type_CJK:
			retVal = numberToCJK(position);
		case Type_None:
			break;
		default:
			break;
	}
	return retVal;
}

const QString numberToLetterSequence(const QString& letters, uint num)
{
	QString retVal("");
	unsigned digits = 1;
	unsigned offset = 0;
	uint column = num - 1;

	// FIXME: what is the heck is this?
	if (column > 4058115285U)
		return  QString("@");

	for (unsigned limit = 28; column >= limit+offset; limit *= letters.length(), digits++)
		offset += limit;

	for (unsigned c = column - offset; digits; --digits, c /= letters.length())
	{
		uint i = c % letters.length();
		if (i < static_cast<uint>(letters.length()))
			retVal.prepend(letters.at(i));
		else
			retVal.prepend(QChar::Null);
	}
	return retVal;
}

const QString numberToRoman(uint i)
{
	QString roman("");
	int arabic = i;
	while (arabic - 1000000 >= 0){
		roman += "m";
		arabic -= 1000000;
	}
	while (arabic - 900000 >= 0){
		roman += "cm";
		arabic -= 900000;
	}
	while (arabic - 500000 >= 0){
		roman += "d";
		arabic -= 500000;
	}
	while (arabic - 400000 >= 0){
		roman += "cd";
		arabic -= 400000;
	}
	while (arabic - 100000 >= 0){
		roman += "c";
		arabic -= 100000;
	}
	while (arabic - 90000 >= 0){
		roman += "xc";
		arabic -= 90000;
	}
	while (arabic - 50000 >= 0){
		roman += "l";
		arabic -= 50000;
	}
	while (arabic - 40000 >= 0){
		roman += "xl";
		arabic -= 40000;
	}
	while (arabic - 10000 >= 0){
		roman += "x";
		arabic -= 10000;
	}
	while (arabic - 9000 >= 0){
		roman += "Mx";
		arabic -= 9000;
	}
	while (arabic - 5000 >= 0){
		roman += "v";
		arabic -= 5000;
	}
	while (arabic - 4000 >= 0){
		roman += "Mv";
		arabic -= 4000;
	}
	while (arabic - 1000 >= 0){
		roman += "M";
		arabic -= 1000;
	}
	while (arabic - 900 >= 0){
		roman += "CM";
		arabic -= 900;
	}
	while (arabic - 500 >= 0){
		roman += "D";
		arabic -= 500;
	}
	while (arabic - 400 >= 0){
		roman += "CD";
		arabic -= 400;
	}
	while (arabic - 100 >= 0){
		roman += "C";
		arabic -= 100;
	}
	while (arabic - 90 >= 0){
		roman += "XC";
		arabic -= 90;
	}
	while (arabic - 50 >= 0){
		roman += "L";
		arabic -= 50;
	}
	while (arabic - 40 >= 0){
		roman += "XL";
		arabic -= 40;
	}
	while (arabic - 10 >= 0){
		roman += "X";
		arabic -= 10;
	}
	while (arabic - 9 >= 0){
		roman += "IX";
		arabic -= 9;
	}
	while (arabic - 5 >= 0){
		roman += "V";
		arabic -= 5;
	}
	while (arabic - 4 >= 0){
		roman += "IV";
		arabic -= 4;
	}
	while (arabic - 1 >= 0){
		roman += "I";
		arabic -= 1;
	}
	return roman;
}

//CB Moved from scribus.cpp
void parsePagesString(QString pages, std::vector<int>* pageNs, int sourcePageCount)
{
	QString tmp(pages);
	QString token;
	do
	{
		if (tmp.indexOf(",") == -1)
		{
			token = tmp;
			tmp = "";
		}
		else
		{
			token = tmp.left(tmp.indexOf(","));
			tmp = tmp.right(tmp.length() - tmp.indexOf(",") - 1);
		}

		token = token.trimmed();
		if (token == "*") // Import all source doc pages
		{
			for (int i = 1; i <= sourcePageCount; ++i)
				pageNs->push_back(i);
		}
		else if (token.indexOf("-") != -1) // import a range of source doc pages
		{
			int from = QString(token.left(token.indexOf("-"))).toInt();
			int to = QString(token.right(token.length() - token.indexOf("-") - 1)).toInt();
			if ((from != 0) && (to != 0))
			{
				if (from > sourcePageCount)
					from = sourcePageCount;
				if (to > sourcePageCount)
					to = sourcePageCount;
				if (from == to)
					pageNs->push_back(to);
				else if (from < to)
				{
					for (int i = from; i <= to; ++i)
						pageNs->push_back(i);
				}
				else
				{
					for (int i = from; i >= to; --i)
						pageNs->push_back(i);
				}
			}
		}
		else // import single source doc page
		{
			int pageNr = token.toInt();
			if ((pageNr > 0) && (pageNr <= sourcePageCount))
				pageNs->push_back(pageNr);
		}
	} while (!tmp.isEmpty());
}


QString readLineFromDataStream(QDataStream &s)
{
	QString ret;
	uchar charData;
	while (!s.atEnd())
	{
		s >> charData;
		if (charData == '\x0A')
			break;
		if (charData == '\x0D')
		{
			quint64 oldPos = s.device()->pos();
			s >> charData;
			if (charData != '\x0A')
				s.device()->seek(oldPos);
			break;
		}
		ret += QChar(charData);
	}
	return ret.trimmed();
}

void setCurrentComboItem(QComboBox *box, QString text)
{
	bool sigBlocked = box->blockSignals(true);
	int ind = box->findText(text);
	if (ind > -1)
		box->setCurrentIndex(ind);
	box->blockSignals(sigBlocked);
}

void setCurrentComboItemFromData(QComboBox *box, QString data)
{
	bool sigBlocked = box->blockSignals(true);
	int ind = box->findData(data);
	if (ind > -1)
		box->setCurrentIndex(ind);
	box->blockSignals(sigBlocked);
}

void removeComboItem(QComboBox *box, QString text)
{
	bool sigBlocked = box->blockSignals(true);
	int ind = box->findText(text);
	if (ind > -1)
		box->removeItem(ind);
	box->blockSignals(sigBlocked);
}

QString readAdobeUniCodeString(QDataStream &s)
{
	QString ret;
	quint32 len;
	s >> len;
	for (quint32 i = 0; i < len; i++)
	{
		quint16 ch;
		s >> ch;
		if (ch != 0)
			ret.append(QChar(ch));
	}
	return ret;
}

QString readAdobeUniCodeString16(QDataStream &s)
{
	QString ret;
	quint16 len;
	s >> len;
	for (quint16 i = 0; i < len; i++)
	{
		quint16 ch;
		s >> ch;
		if (ch != 0)
			ret.append(QChar(ch));
	}
	return ret;
}

QString getDashString(int dashtype, double linewidth)
{
	QString dashString;
	QVector<double> dashArray;
	getDashArray(dashtype, linewidth, dashArray);
	for (int i = 0; i < dashArray.size(); ++i)
	{
		dashString += QString::number(dashArray.at(i));
		if (i < (dashArray.size() - 1))
			dashString += " ";
	}
	return dashString;
}

void getDashArray(int dashtype, double linewidth, QVector<float> &dashArray)
{
	QVector<double> tmp;
	getDashArray(dashtype, linewidth, tmp);
	dashArray.clear();
	for (int i = 0; i < tmp.count(); ++i)
		dashArray << static_cast<float>(tmp[i]);
}

void getDashArray(int dashtype, double linewidth, QVector<double> &dashArray)
{
	dashArray.clear();
	if ((dashtype == 1) || (dashtype == 0))
		return;
	double Dt = qMax(1.0*linewidth, 0.1);
	double Sp = qMax(2.0*linewidth, 0.1);
	double Da = qMax(4.0*linewidth, 0.1);
	switch (dashtype)
	{
		case 1:
			break;
		case 2:
			dashArray << Da << Sp;
			break;
		case 3:
			dashArray << Dt << Sp;
			break;
		case 4:
			dashArray << Da << Sp << Dt << Sp;
			break;
		case 5:
			dashArray << Da << Sp << Dt << Sp << Dt << Sp;
			break;
// Additional line styles taken from Inkscape
		case 6:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 7:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(3.0 * linewidth, 0.01);
			break;
		case 8:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(4.0 * linewidth, 0.01);
			break;
		case 9:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(6.0 * linewidth, 0.01);
			break;
		case 10:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(8.0 * linewidth, 0.01);
			break;
		case 11:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(12.0 * linewidth, 0.01);
			break;
		case 12:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(24.0 * linewidth, 0.01);
			break;
		case 13:
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(48.0 * linewidth, 0.01);
			break;
		case 14:
			dashArray << qMax(2.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 15:
			dashArray << qMax(3.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 16:
			dashArray << qMax(4.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 17:
			dashArray << qMax(6.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 18:
			dashArray << qMax(8.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 19:
			dashArray << qMax(10.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 20:
			dashArray << qMax(12.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 21:
			dashArray << qMax(2.0 * linewidth, 0.01) << qMax(2.0 * linewidth, 0.01);
			break;
		case 22:
			dashArray << qMax(3.0 * linewidth, 0.01) << qMax(3.0 * linewidth, 0.01);
			break;
		case 23:
			dashArray << qMax(4.0 * linewidth, 0.01) << qMax(4.0 * linewidth, 0.01);
			break;
		case 24:
			dashArray << qMax(6.0 * linewidth, 0.01) << qMax(6.0 * linewidth, 0.01);
			break;
		case 25:
			dashArray << qMax(8.0 * linewidth, 0.01) << qMax(8.0 * linewidth, 0.01);
			break;
		case 26:
			dashArray << qMax(10.0 * linewidth, 0.01) << qMax(10.0 * linewidth, 0.01);
			break;
		case 27:
			dashArray << qMax(12.0 * linewidth, 0.01) << qMax(12.0 * linewidth, 0.01);
			break;
		case 28:
			dashArray << qMax(2.0 * linewidth, 0.01) << qMax(4.0 * linewidth, 0.01);
			break;
		case 29:
			dashArray << qMax(2.0 * linewidth, 0.01) << qMax(6.0 * linewidth, 0.01);
			break;
		case 30:
			dashArray << qMax(6.0 * linewidth, 0.01) << qMax(2.0 * linewidth, 0.01);
			break;
		case 31:
			dashArray << qMax(4.0 * linewidth, 0.01) << qMax(8.0 * linewidth, 0.01);
			break;
		case 32:
			dashArray << qMax(8.0 * linewidth, 0.01) << qMax(4.0 * linewidth, 0.01);
			break;
		case 33:
			dashArray << qMax(2.0 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			dashArray << qMax(0.5 * linewidth, 0.01) << qMax(1.0 * linewidth, 0.01);
			break;
		case 34:
			dashArray << qMax(8.0 * linewidth, 0.01) << qMax(2.0 * linewidth, 0.01);
			dashArray << qMax(1.0 * linewidth, 0.01) << qMax(2.0 * linewidth, 0.01);
			break;
		case 35:
			dashArray << qMax(0.5 * linewidth, 0.01) << qMax(0.5 * linewidth, 0.01);
			break;
		case 36:
			dashArray << qMax(0.25 * linewidth, 0.01) << qMax(0.25 * linewidth, 0.01);
			break;
		case 37:
			dashArray << qMax(0.1 * linewidth, 0.01) << qMax(0.1 * linewidth, 0.01);
			break;
		default:
			break;
	}
}

bool convertOldTable(ScribusDoc *m_Doc, PageItem* gItem, QList<PageItem*> &gpL, QStack<QList<PageItem *> > *groupStackT, QList<PageItem *> *target)
{
	QList<double> colWidths;
	QList<double> rowHeights;

	// 1. Although this was not intended, legacy tables allowed user to link frames together
	// New table do not support that, so if one frame has any link, we stop the conversion
	// here, those frame will be converted to a standard group.
	// 2. Pre-1.4.3 versions had a bug where item TopLink/LeftLink/BottomLink/RightLink were
	// lost when copy/pasting tables. Exit conversion too so these broken tables can be
	// converted to standard groups (at least until we find a good way to process that case)
	bool hasTableLinks = false;
	bool hasTextLinks = false;
	for (int i = 0; i < gpL.count(); i++)
	{
		PageItem* it = gpL[i];
		it->isTableItem = false;
		if (it->nextInChain() || it->prevInChain())
			hasTextLinks = true;
		if (it->LeftLink || it->RightLink || it->BottomLink || it->TopLink)
			hasTableLinks = true;
	}

	if (!hasTableLinks || hasTextLinks)
		return false;

	PageItem *topLeft = NULL;
	for (int i = 0; i < gpL.count(); i++)
	{
		PageItem* it = gpL[i];
		if ((it->TopLink == NULL) && (it->LeftLink == NULL))	// we got the topleft item
		{
			topLeft = it;
			PageItem *tl = it;
			while (tl->RightLink != NULL)
			{
				colWidths.append(tl->width());
				tl = tl->RightLink;
			}
			colWidths.append(tl->width());
			while (tl->BottomLink != NULL)
			{
				rowHeights.append(tl->height());
				tl = tl->BottomLink;
			}
			rowHeights.append(tl->height());
			break;
		}
	}

	// Check we have found enough rows and columns so that no item will disappear
	if ((colWidths.count() * rowHeights.count()) < gpL.count())
		return false;

	m_Doc->dontResize = true;
	int z = m_Doc->itemAdd(PageItem::Table, PageItem::Unspecified, gItem->xPos(), gItem->yPos(), gItem->width(), gItem->height(), 0.0, CommonStrings::None, CommonStrings::None);
	PageItem_Table* currItem = m_Doc->Items->takeAt(z)->asTable();

	currItem->LayerID = gItem->LayerID;
	currItem->OwnPage = gItem->OwnPage;
	currItem->OnMasterPage = gItem->OnMasterPage;

	currItem->insertRows(0, rowHeights.count()-1);
	m_Doc->dontResize = true;
	currItem->insertColumns(0, colWidths.count()-1);
	m_Doc->dontResize = true;
	for (int i = 0; i < rowHeights.count(); i++)
	{
		currItem->resizeRow(i, rowHeights[i]);
	}
	m_Doc->dontResize = true;
	for (int i = 0; i < colWidths.count(); i++)
	{
		currItem->resizeColumn(i, colWidths[i]);
	}
	TableBorder border(0.0, Qt::SolidLine, CommonStrings::None, 100);
	currItem->setLeftBorder(border);
	currItem->setTopBorder(border);
	currItem->setRightBorder(border);
	currItem->setBottomBorder(border);
	m_Doc->dontResize = true;
	PageItem *tr = topLeft;
	int rowCount = 0;
	int colCount = 0;
	while (rowCount < rowHeights.count())
	{
		PageItem *tl = tr;
		while (colCount < colWidths.count())
		{
			currItem->cellAt(rowCount, colCount).textFrame()->itemText = tl->itemText.copy();
			currItem->cellAt(rowCount, colCount).setFillColor(tl->fillColor());
			currItem->cellAt(rowCount, colCount).setFillShade(tl->fillShade());
			currItem->cellAt(rowCount, colCount).setLeftBorder(border);
			currItem->cellAt(rowCount, colCount).setTopBorder(border);
			currItem->cellAt(rowCount, colCount).setRightBorder(border);
			currItem->cellAt(rowCount, colCount).setBottomBorder(border);
			if ((tl->lineColor() != CommonStrings::None) && (tl->lineWidth() != 0.0))
			{
				TableBorder bb(tl->lineWidth(), tl->lineStyle(), tl->lineColor(), tl->lineShade());
				if (tl->LeftLine)
					currItem->cellAt(rowCount, colCount).setLeftBorder(bb);
				if (tl->TopLine)
					currItem->cellAt(rowCount, colCount).setTopBorder(bb);
				if (tl->RightLine)
					currItem->cellAt(rowCount, colCount).setRightBorder(bb);
				if (tl->BottomLine)
					currItem->cellAt(rowCount, colCount).setBottomBorder(bb);
			}
			if (colCount == colWidths.count()-1)
				break;
			colCount++;
			tl = tl->RightLink;
		}
		if (rowCount == rowHeights.count()-1)
			break;
		colCount = 0;
		rowCount++;
		tr = tr->BottomLink;
	}
	m_Doc->dontResize = true;
	currItem->setLayer(gItem->LayerID);
	currItem->setMasterPage(gItem->OwnPage, gItem->OnMasterPage);
	currItem->adjustFrameToTable();
	if (target != NULL)
	{
		int ind = target->indexOf(gItem);
		target->replace(ind, currItem);
	}
	else
	{
		int ind = m_Doc->FrameItems.key(gItem);
		m_Doc->FrameItems.remove(ind);
		m_Doc->FrameItems.insert(ind, currItem);
	}
	if (groupStackT != NULL)
	{
		if (groupStackT->count() > 0)
		{
			int ii = groupStackT->top().indexOf(gItem);
			if (ii >= 0)
				groupStackT->top().replace(ii, currItem);
		}
	}
	while (!gpL.isEmpty())
	{
		PageItem* item = gpL.takeFirst();
		if (item->asTextFrame())
			item->dropLinks();
		delete item;
	}
	delete gItem;

	return true;
}

void setWidgetBoldFont(QWidget* w, bool wantBold)
{
	QFont f(w->font());
	f.setBold(wantBold);
	w->setFont(f);
}

void getUniqueName(QString &name, QStringList list, QString separator, bool prepend)
{
	if (!list.contains(name))
		return;
	int token = -1;
	QString newName;
	do {
		token++;
		if (prepend)
			newName = separator + QString::number(token) + " " + name;
		else
			newName = name + separator + QString::number(token);
	} while (list.contains(newName));
	name = newName;
}


const QString numberToCJK(uint i)
{
	QString result;
	if (i<10)
		result = QString(cjkDigit(i));

	if (i>9 && i<=99)
	{
		int tens=i/10;
		int ones=i%10;
		if (tens!=1)
			result.append(cjkDigit(tens));
		result.append(cjkDigit(10));
		if (ones!=0)
			result.append(cjkDigit(ones));
	}

	if (i>99 && i<=999)
	{
		int hundreds=i/100;
		int tens=(i-hundreds*100)/10;
		int ones=i%10;
		result.append(cjkDigit(hundreds));
		result.append(cjkDigit(100));
		if (tens!=0)
		{
			result.append(cjkDigit(tens));
			result.append(cjkDigit(10));
		}
		else if (ones!=0)
			result.append(cjkDigit(0));
		if (ones!=0)
			result.append(cjkDigit(ones));
	}
	return result;
}


QChar cjkDigit(uint i)
{
	switch (i)
	{
		case 0:
			return QChar(0x96f6);
			break;
		case 1:
			return QChar(0x4e00);
			break;
		case 2:
			return QChar(0x4e8c);
			break;
		case 3:
			return QChar(0x4e09);
			break;
		case 4:
			return QChar(0x56db);
			break;
		case 5:
			return QChar(0x4e94);
			break;
		case 6:
			return QChar(0x516d);
			break;
		case 7:
			return QChar(0x4e03);
			break;
		case 8:
			return QChar(0x516b);
			break;
		case 9:
			return QChar(0x4e5d);
			break;
		case 10:
			return QChar(0x5341);
			break;
		case 100:
			return QChar(0x767e);
			break;
		case 1000:
			return QChar(0x5343);
			break;
		case 10000:
			return QChar(0x842c);
			break;
		case 100000000:
			return QChar(0x5104);
			break;
	}
	return QChar::Null;
}
