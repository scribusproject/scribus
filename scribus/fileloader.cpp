/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtl.h>
#include <qcursor.h>
#include <qregexp.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qprogressbar.h>
#include <cstdlib>
#include <cmath>
#include <qtextstream.h>

#include "commonstrings.h"
#include "fileloader.h"
#include "fileloader.moc"
#include "fontreplacedialog.h"
#include "missing.h"
#include "page.h"
#include "pluginmanager.h"
#include "plugins/formatidlist.h"
#include "pagestructs.h"
#include "scconfig.h"
#include "scribus.h"
#include "scribusXml.h"
#include "units.h"
#include "loadsaveplugin.h"
#include "guidemanager.h"
#include "text/nlsconfig.h"

// We need to include the headers for the plugins we support until we start
// using LoadSavePlugin to pick them for us. We only use these headers to
// get the format IDs, NOTHING ELSE.
// #include "plugins/svgimplugin/svgplugin.h"
// #include "plugins/psimport/importpsplugin.h"
// #include "plugins/fileloader/oodraw/oodrawimp.h"

#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
#include "util.h"
#include "prefsmanager.h"

/*!
 \author Franz Schmid
 \date
 \brief Constructor, sets the variable "FileName" to the input parameter fileName
 \param fileName filename to load
 \retval None
 */
FileLoader::FileLoader(const QString & fileName) :
	QObject(0, "FileLoader"),
	FileName(fileName),
	FileType(-1),
	formatSLA12x(LoadSavePlugin::getFormatById(FORMATID_SLA12XIMPORT)),
	formatSLA13x(LoadSavePlugin::getFormatById(FORMATID_SLA13XIMPORT)),
	formatPS(LoadSavePlugin::getFormatById(FORMATID_PSIMPORT)),
	formatSVG(LoadSavePlugin::getFormatById(FORMATID_SVGIMPORT)),
	formatSXD(LoadSavePlugin::getFormatById(FORMATID_SXDIMPORT)),
	formatODG(LoadSavePlugin::getFormatById(FORMATID_ODGIMPORT)),
	prefsManager(PrefsManager::instance())
{
	dummyFois.setAutoDelete(true);
}

// FIXME: This static method is here as a temporary transitional
// measure during the process of converting to file loader plugins.
const QString FileLoader::getLoadFilterString()
{
	/*
	PluginManager& pluginManager(PluginManager::instance());
	QString formats;
#ifdef HAVE_LIBZ
	formats += tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;");
#else
	formats += tr("Documents (*.sla *.scd);;");
#endif
	if (pluginManager.DLLexists("importps"))
		formats += tr("PostScript Files (*.eps *.EPS *.ps *.PS);;");
	if (pluginManager.DLLexists("svgimplugin"))
#ifdef HAVE_LIBZ
		formats += tr("SVG Images (*.svg *.svgz);;");
#else
		formats += tr("SVG Images (*.svg);;");
#endif
	if (pluginManager.DLLexists("oodrawimp"))
		formats += tr("OpenOffice.org Draw (*.sxd);;");
	formats += tr("All Files (*)");
	return formats;
	*/
	return LoadSavePlugin::fileDialogLoadFilter().join(";;");
}

/*!
 \fn int FileLoader::TestFile()
 \author Franz Schmid
 \date
 \brief Tests if the file "FileName" exists and determines the type of the file.
 \retval int -1 if the file doesn't exist or any other error has occured, 0 for the old Format, 1 for the new Format, 2 for EPS and PS files, 3 for SVG files and 4 for PDF files
 */
int FileLoader::TestFile()
{
	QFileInfo fi = QFileInfo(FileName);
	int ret = -1;
	if (!fi.exists())
		ret = -1;
	QString ext = fi.extension(true).lower();

	QValueList<FileFormat> fileFormats(LoadSavePlugin::supportedFormats());
	QValueList<FileFormat>::const_iterator it(fileFormats.constBegin());
	QValueList<FileFormat>::const_iterator itEnd(fileFormats.constEnd());
	for ( ; it != itEnd ; ++it )
	{
		if ((*it).nameMatch.search("."+ext)!=-1)
		{
// 			qDebug(QString("Match :%1: :.%2: on %3").arg((*it).nameMatch.pattern()).arg(ext).arg((*it).trName));
			if ((*it).plug!=0)
			{
				if ((*it).plug->fileSupported(0, FileName))
				{
// 					qDebug(QString("File Supported With: %1").arg((*it).trName));
					ret=(*it).formatId;
					break;
				}
			}
		}
// 		else
// 			qDebug(QString("No Match :%1: :.%2: on %3").arg((*it).nameMatch.pattern()).arg(ext).arg((*it).trName));
	}
	
// 	if (ret==-1)
// 	{
// 		if ((ext.endsWith("sla.gz")) || (ext.endsWith("sla")) || (ext.endsWith("scd.gz")) || (ext.endsWith("scd")))
// 			ret = CheckScribus();
// 		else 
// 		if (((ext.endsWith("ps")) || (ext.endsWith("eps"))) && (formatPS))
// 			ret = FORMATID_PSIMPORT;
// 		else if (((ext.endsWith("svg")) || (ext.endsWith("svgz"))) && (formatSVG))
// 			ret = FORMATID_SVGIMPORT;
// 		else if ((ext.endsWith("sxd")) && (formatSXD))
// 			ret = FORMATID_SXDIMPORT;
// 		else if ((ext.endsWith("odg")) && (formatODG))
// 			ret = FORMATID_ODGIMPORT;
// 	}
/*	if (ext == "pdf")
		ret = 4; */
	FileType = ret;
	return ret;
}

static void replaceFonts(PageItem *it, QMap<QString, int> UsedFonts, QMap<QString, QString> ReplacedFonts)
{	
	if ((it->asTextFrame()) || (it->asPathText()))
	{
		CharStyle newFontStyle;
		for (uint e = 0; e < it->itemText.nrOfRuns(); ++e)
		{
			int start = it->itemText.startOfRun(e);
			Foi* oldFont = it->itemText.charStyle(start).font();
			if (!UsedFonts.contains(oldFont->scName())) {
				newFontStyle.cfont = (*ScMW->doc->AllFonts)[ReplacedFonts[oldFont->scName()]];
				it->itemText.applyStyle(start, it->itemText.endOfRun(e) - start, newFontStyle );
			}
		}
	}
}


bool FileLoader::LoadPage(int PageToLoad, bool Mpage, QString renamedPageName)
{
	bool ret = false;
	newReplacement = false;
	ReplacedFonts.clear();
	dummyFois.clear();
	dummyFois.setAutoDelete(true);
	QValueList<FileFormat>::const_iterator it;
	if (findFormat(FileType, it))
	{
		if (FileType==FORMATID_SLA12XIMPORT)
		{
			(*it).plug->setupTargets(ScMW->doc, ScMW->view, ScMW->mainWindowProgressBar, &(prefsManager->appPrefs.AvailFonts));
			ret=(*it).plug->loadPage(FileName, PageToLoad, Mpage, renamedPageName);
			if (ret)
				(*it).plug->getReplacedFontData(newReplacement, ReplacedFonts, dummyFois);
		}
		if (FileType==FORMATID_SLA13XIMPORT)
		{
			(*it).plug->setupTargets(ScMW->doc, 0, ScMW->mainWindowProgressBar, &(prefsManager->appPrefs.AvailFonts));
			ret=(*it).plug->loadPage(FileName, PageToLoad, Mpage, renamedPageName);
			if (ret)
				(*it).plug->getReplacedFontData(newReplacement, ReplacedFonts, dummyFois);
		}
	}
	if (ReplacedFonts.count() != 0)
	{
		if ((prefsManager->appPrefs.askBeforeSubstituite) || (newReplacement))
		{
			qApp->setOverrideCursor(QCursor(Qt::arrowCursor), true);
			FontReplaceDialog dia(0, &ReplacedFonts);
			if (dia.exec())
			{
				QMap<QString,QString>::Iterator itfsu;
				for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
				{
					if (dia.stickyReplacements->isChecked())
						prefsManager->appPrefs.GFontSub[itfsu.key()] = itfsu.data();
				}
			}
			else
			{
				return false;
			}
		}
		for (uint d = 0; d < ScMW->doc->MasterItems.count(); ++d)
		{
			replaceFonts(ScMW->doc->MasterItems.at(d), ScMW->doc->UsedFonts, ReplacedFonts);
		}
		for (uint d = 0; d < ScMW->doc->DocItems.count(); ++d)
		{
			replaceFonts(ScMW->doc->DocItems.at(d), ScMW->doc->UsedFonts, ReplacedFonts);
		}
		for (uint d = 0; d < ScMW->doc->FrameItems.count(); ++d)
		{
			replaceFonts(ScMW->doc->FrameItems.at(d), ScMW->doc->UsedFonts, ReplacedFonts);
		}
		for (uint a = 0; a < ScMW->doc->docParagraphStyles.count(); ++a)
		{
			if ( ScMW->doc->docParagraphStyles[a].charStyle().font() != &Foi::NONE && !ScMW->doc->UsedFonts.contains(ScMW->doc->docParagraphStyles[a].charStyle().font()->scName()))
				ScMW->doc->docParagraphStyles[a].charStyle().cfont =
					(*ScMW->doc->AllFonts)[ReplacedFonts[ScMW->doc->docParagraphStyles[a].charStyle().font()->scName()]];
		}
		QMap<QString,QString>::Iterator itfsu;
		for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
		{
			if (!ScMW->doc->UsedFonts.contains(itfsu.data()))
			{
//				QFont fo = prefsManager->appPrefs.AvailFonts[itfsu.data()]->Font;
//				fo.setPointSize(qRound(ScMW->doc->toolSettings.defSize / 10.0));
				ScMW->doc->AddFont(itfsu.data(), qRound(ScMW->doc->toolSettings.defSize / 10.0));
			}
		}
		if (prefsManager->appPrefs.askBeforeSubstituite)
			ReplacedFonts.clear();
		dummyFois.clear();
	}
	return ret;
}

/*!
 \fn bool FileLoader::LoadFile()
 \author Franz Schmid
 \date
 \brief Loads the file "FileName" as a Scribus document
 \retval bool true when loading is succsessful, false otherwise
 */
bool FileLoader::LoadFile()
{
	newReplacement = false;
	ScMW->doc->guidesSettings.marginsShown = prefsManager->appPrefs.guidesSettings.marginsShown;
	ScMW->doc->guidesSettings.framesShown = prefsManager->appPrefs.guidesSettings.framesShown;
	ScMW->doc->guidesSettings.layerMarkersShown = prefsManager->appPrefs.guidesSettings.layerMarkersShown;
	ScMW->doc->guidesSettings.gridShown = prefsManager->appPrefs.guidesSettings.gridShown;
	ScMW->doc->guidesSettings.guidesShown = prefsManager->appPrefs.guidesSettings.guidesShown;
	ScMW->doc->guidesSettings.colBordersShown = prefsManager->appPrefs.guidesSettings.colBordersShown;
	ScMW->doc->guidesSettings.baseShown = prefsManager->appPrefs.guidesSettings.baseShown;
	ScMW->doc->guidesSettings.linkShown = prefsManager->appPrefs.guidesSettings.linkShown;
	ScMW->doc->toolSettings.polyC = prefsManager->appPrefs.toolSettings.polyC;
	ScMW->doc->toolSettings.polyF = prefsManager->appPrefs.toolSettings.polyF;
	ScMW->doc->toolSettings.polyR = prefsManager->appPrefs.toolSettings.polyR;
	ScMW->doc->toolSettings.polyFd = prefsManager->appPrefs.toolSettings.polyFd;
	ScMW->doc->toolSettings.polyS = prefsManager->appPrefs.toolSettings.polyS;
	ScMW->doc->AutoSave = prefsManager->appPrefs.AutoSave;
	ScMW->doc->AutoSaveTime = prefsManager->appPrefs.AutoSaveTime;
	ReplacedFonts.clear();
	dummyFois.clear();
	dummyFois.setAutoDelete(true);
	bool ret = false;
	QValueList<FileFormat>::const_iterator it;
	if (findFormat(FileType, it))
		switch (FileType)
		{
			case FORMATID_SLA12XIMPORT:
				{
					(*it).setupTargets(ScMW->doc, ScMW->view, ScMW->mainWindowProgressBar, &(prefsManager->appPrefs.AvailFonts));
					ret=(*it).loadFile(FileName, LoadSavePlugin::lfCreateDoc);
					if (ret)
						(*it).getReplacedFontData(newReplacement, ReplacedFonts, dummyFois);
				}
				break;
			case FORMATID_SLA13XIMPORT:
				{
					(*it).setupTargets(ScMW->doc, 0, ScMW->mainWindowProgressBar, &(prefsManager->appPrefs.AvailFonts));
					ret=(*it).loadFile(FileName, LoadSavePlugin::lfCreateDoc);
					if (ret)
						(*it).getReplacedFontData(newReplacement, ReplacedFonts, dummyFois);
				}
				break;
			default:
				ret = (*it).loadFile(FileName, LoadSavePlugin::lfCreateDoc);
				break;
		}
	return ret;
}

bool FileLoader::ReadStyles(const QString& fileName, ScribusDoc* doc, QValueList<ParagraphStyle> &docParagraphStyles)
{
	QValueList<FileFormat>::const_iterator it;
	if (findFormat(FileType, it))
		switch (FileType)
		{
			case FORMATID_SLA12XIMPORT:
			case FORMATID_SLA13XIMPORT:
				return (*it).readStyles(fileName, doc, docParagraphStyles);
				break;
			default:
				break;
		}
	return false;
}

bool FileLoader::ReadColors(const QString& fileName, ColorList & colors)
{
	QValueList<FileFormat>::const_iterator it;
	if (findFormat(FileType, it))
		switch (FileType)
		{
			case FORMATID_SLA12XIMPORT:
			case FORMATID_SLA13XIMPORT:
				return (*it).readColors(fileName, colors);
				break;
			default:
				break;
		}
	return false;
}


bool FileLoader::ReadPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames)
{
	QValueList<FileFormat>::const_iterator it;
	if (findFormat(FileType, it))
		switch (FileType)
		{
			case FORMATID_SLA12XIMPORT:
			case FORMATID_SLA13XIMPORT:
				return (*it).readPageCount(fileName, num1, num2, masterPageNames);
				break;
			default:
				break;
		}
	return false;
}

bool FileLoader::ReadLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty)
{
	QValueList<FileFormat>::const_iterator it;
	if (findFormat(FileType, it))
		switch (FileType)
		{
			case FORMATID_SLA12XIMPORT:
			case FORMATID_SLA13XIMPORT:
				return (*it).readLineStyles(fileName, Sty);
				break;
			default:
				break;
		}
	return false;
}

void FileLoader::readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, SCFonts &avail, ScribusDoc *doc)
{
	vg.setName(pg.attribute("NAME"));
	vg.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(pg.attribute("LINESPMode", "0").toInt()));
	vg.setLineSpacing(pg.attribute("LINESP").toDouble());
	vg.setLeftMargin(pg.attribute("INDENT", "0").toDouble());
	if (pg.hasAttribute("RMARGIN"))
		vg.setRightMargin(pg.attribute("RMARGIN", "0").toDouble());
	else
		vg.setRightMargin(0);
	vg.setFirstIndent(pg.attribute("FIRST", "0").toDouble());
	vg.setAlignment(pg.attribute("ALIGN").toInt());
	vg.setGapBefore(pg.attribute("VOR", "0").toDouble());
	vg.setGapAfter(pg.attribute("NACH", "0").toDouble());
	PrefsManager * prefsManager = PrefsManager::instance();
	QString tmpf = pg.attribute("FONT", doc->toolSettings.defFont);
	if ((!avail.find(tmpf)) || (!avail[tmpf]->usable()))
	{
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]]->usable()))
		{
			newReplacement = true;
			ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
		}
		else
			ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
		}
		else
		{
			if (!doc->UsedFonts.contains(tmpf))
			{
				//						QFont fo = avail[tmpf]->Font;
				//						fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
				doc->AddFont(tmpf, qRound(doc->toolSettings.defSize / 10.0));
			}
		}
		vg.charStyle().cfont = (*ScMW->doc->AllFonts)[tmpf];
		vg.charStyle().csize = qRound(pg.attribute("FONTSIZE", "12").toDouble() * 10.0);
		vg.setHasDropCap(static_cast<bool>(pg.attribute("DROP", "0").toInt()));
		vg.setDropCapLines(pg.attribute("DROPLIN", "2").toInt());
		vg.setDropCapOffset(pg.attribute("DROPDIST", "0").toDouble());
		vg.charStyle().cstyle = static_cast<StyleFlag>(pg.attribute("EFFECT", "0").toInt());
		vg.charStyle().ccolor = pg.attribute("FCOLOR", doc->toolSettings.dBrush);
		vg.charStyle().cshade = pg.attribute("FSHADE", "100").toInt();
		vg.charStyle().cstroke = pg.attribute("SCOLOR", doc->toolSettings.dPen);
		vg.charStyle().cshade2 = pg.attribute("SSHADE", "100").toInt();
		vg.setUseBaselineGrid(static_cast<bool>(pg.attribute("BASE", "0").toInt()));
		vg.charStyle().cshadowx = qRound(pg.attribute("TXTSHX", "5").toDouble() * 10);
		vg.charStyle().cshadowy = qRound(pg.attribute("TXTSHY", "-5").toDouble() * 10);
		vg.charStyle().coutline = qRound(pg.attribute("TXTOUT", "1").toDouble() * 10);
		vg.charStyle().cunderpos = qRound(pg.attribute("TXTULP", "-0.1").toDouble() * 10);
		vg.charStyle().cunderwidth = qRound(pg.attribute("TXTULW", "-0.1").toDouble() * 10);
		vg.charStyle().cstrikepos = qRound(pg.attribute("TXTSTP", "-0.1").toDouble() * 10);
		vg.charStyle().cstrikewidth = qRound(pg.attribute("TXTSTW", "-0.1").toDouble() * 10);
		vg.charStyle().cscale = qRound(pg.attribute("SCALEH", "100").toDouble() * 10);
		vg.charStyle().cscalev = qRound(pg.attribute("SCALEV", "100").toDouble() * 10);
		vg.charStyle().cbase = qRound(pg.attribute("BASEO", "0").toDouble() * 10);
		vg.charStyle().cextra = qRound(pg.attribute("KERN", "0").toDouble() * 10);
		vg.tabValues().clear();
		if ((pg.hasAttribute("NUMTAB")) && (pg.attribute("NUMTAB", "0").toInt() != 0))
		{
			ParagraphStyle::TabRecord tb;
			QString tmp = pg.attribute("TABS");
			QTextStream tgv(&tmp, IO_ReadOnly);
			double xf, xf2;
			for (int cxv = 0; cxv < pg.attribute("NUMTAB", "0").toInt(); cxv += 2)
			{
				tgv >> xf;
				tgv >> xf2;
				tb.tabPosition = xf2;
				tb.tabType = static_cast<int>(xf);
				tb.tabFillChar =  QChar();
				vg.tabValues().append(tb);
			}
			tmp = "";
		}
		else
		{
			QDomNode IT = pg.firstChild();
			while(!IT.isNull())
			{
				QDomElement it = IT.toElement();
				if (it.tagName()=="Tabs")
				{
					ParagraphStyle::TabRecord tb;
					tb.tabPosition = it.attribute("Pos").toDouble();
					tb.tabType = it.attribute("Type").toInt();
					QString tbCh = "";
					tbCh = it.attribute("Fill","");
					if (tbCh.isEmpty())
						tb.tabFillChar = QChar();
					else
						tb.tabFillChar = tbCh[0];
					vg.tabValues().append(tb);
				}
				IT=IT.nextSibling();
			}
		}
}

bool FileLoader::postLoad()
{
/*	for (uint d = 0; d < ScMW->doc->MasterItems.count(); ++d)
	{
		PageItem *it = ScMW->doc->MasterItems.at(d);
		if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				ScText *hl = it->itemText.at(e);
				if ((hl->ch == QChar(25)) && ((int)hl->cembedded != -1))
					hl->cembedded = ScMW->doc->FrameItems.at((int)hl->cembedded);
				else
					hl->cembedded = 0;
			}
		}
	}
	for (uint d = 0; d < ScMW->doc->DocItems.count(); ++d)
	{
		PageItem *it = ScMW->doc->DocItems.at(d);
		if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				ScText *hl = it->itemText.at(e);
				if ((hl->ch == QChar(25)) && ((int)hl->cembedded != -1))
					hl->cembedded = ScMW->doc->FrameItems.at((int)hl->cembedded);
				else
					hl->cembedded = 0;
			}
		}
	} */
	if (ReplacedFonts.count() != 0)
	{
		if ((prefsManager->appPrefs.askBeforeSubstituite) || (newReplacement))
		{
			qApp->setOverrideCursor(QCursor(Qt::arrowCursor), true);
			FontReplaceDialog dia(0, &ReplacedFonts);
			if (dia.exec())
			{
				QMap<QString,QString>::Iterator itfsu;
				for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
				{
					if (dia.stickyReplacements->isChecked())
						prefsManager->appPrefs.GFontSub[itfsu.key()] = itfsu.data();
				}
			}
			else
			{
				return false;
			}
		}
		for (uint d = 0; d < ScMW->doc->MasterItems.count(); ++d)
		{
			replaceFonts(ScMW->doc->MasterItems.at(d), ScMW->doc->UsedFonts, ReplacedFonts);
		}
		for (uint d = 0; d < ScMW->doc->DocItems.count(); ++d)
		{
			replaceFonts(ScMW->doc->DocItems.at(d), ScMW->doc->UsedFonts, ReplacedFonts);
		}
		for (uint d = 0; d < ScMW->doc->FrameItems.count(); ++d)
		{
			replaceFonts(ScMW->doc->FrameItems.at(d), ScMW->doc->UsedFonts, ReplacedFonts);
		}
		for (uint a = 0; a < ScMW->doc->docParagraphStyles.count(); ++a)
		{
			if ( ScMW->doc->docParagraphStyles[a].charStyle().font() != &Foi::NONE && !ScMW->doc->UsedFonts.contains(ScMW->doc->docParagraphStyles[a].charStyle().font()->scName()))
				ScMW->doc->docParagraphStyles[a].charStyle().cfont =
					(*ScMW->doc->AllFonts)[ReplacedFonts[ScMW->doc->docParagraphStyles[a].charStyle().font()->scName()]];
		}
		QValueList<QString> tmpList;
		tmpList.clear();
		for (uint fe = 0; fe <  ScMW->doc->PDF_Options.EmbedList.count(); ++fe)
		{
			if (ReplacedFonts.contains(ScMW->doc->PDF_Options.EmbedList[fe]))
				tmpList.append(ReplacedFonts[ScMW->doc->PDF_Options.EmbedList[fe]]);
			else
				tmpList.append(ScMW->doc->PDF_Options.EmbedList[fe]);
		}
		ScMW->doc->PDF_Options.EmbedList = tmpList;
		tmpList.clear();
		for (uint fe = 0; fe <  ScMW->doc->PDF_Options.SubsetList.count(); ++fe)
		{
			if (ReplacedFonts.contains(ScMW->doc->PDF_Options.SubsetList[fe]))
				tmpList.append(ReplacedFonts[ScMW->doc->PDF_Options.SubsetList[fe]]);
			else
				tmpList.append(ScMW->doc->PDF_Options.SubsetList[fe]);
		}
		ScMW->doc->PDF_Options.SubsetList = tmpList;
		QMap<QString,QString>::Iterator itfsu;
		for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
		{
			if (!ScMW->doc->UsedFonts.contains(itfsu.data()))
			{
//				QFont fo = prefsManager->appPrefs.AvailFonts[itfsu.data()]->Font;
//				fo.setPointSize(qRound(ScMW->doc->toolSettings.defSize / 10.0));
				ScMW->doc->AddFont(itfsu.data(), qRound(ScMW->doc->toolSettings.defSize / 10.0));
			}
		}
		if (prefsManager->appPrefs.askBeforeSubstituite)
			ReplacedFonts.clear();
		dummyFois.clear();
	}

	return true;
}

void FileLoader::informReplacementFonts()
{
	if (ReplacedFonts.count() != 0)
	{
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		QString mess = tr("Some fonts used by this document have been substituted:")+"\n\n";
		QMap<QString,QString>::Iterator it;
		for (it = ReplacedFonts.begin(); it != ReplacedFonts.end(); ++it)
		{
			mess += it.key() + tr(" was replaced by: ")+ it.data() +"\n";
		}
		QMessageBox::warning(ScMW, CommonStrings::trWarning, mess, 1, 0, 0);
	}
}

bool FileLoader::findFormat(int formatId, QValueList<FileFormat>::const_iterator &it)
{
	QValueList<FileFormat> fileFormats(LoadSavePlugin::supportedFormats());
	it=fileFormats.constBegin();
	QValueList<FileFormat>::const_iterator itEnd(fileFormats.constEnd());
	for ( ; it != itEnd ; ++it )
	{
		if(formatId==(*it).formatId)
			return true;
	}
	return false;
}
