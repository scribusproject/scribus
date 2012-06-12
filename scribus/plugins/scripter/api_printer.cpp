/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_printer.h"
#include "util_printer.h"
#include "util_ghostscript.h"
#include "util_file.h"
#include "pslib.h"

#ifdef HAVE_CUPS
#include <cups/cups.h>
#endif

#if defined(_WIN32)
#include "scprintengine_gdi.h"
#endif


PrinterAPI::PrinterAPI() : QObject(COLLECTOR)
{
	qDebug() << "PrinterAPI loaded";
	setObjectName("printer");
	printer = QString("File");
	cmd = QString("");
	color = true;
	useICC = false;
	mph = false;
	mpv = false;
	ucr = true;
	copies = true;



	QString tf(ScCore->primaryMainWindow()->doc->pdfOptions().fileName);
	if (tf.isEmpty())
	{
		QFileInfo fi = QFileInfo(ScCore->primaryMainWindow()->doc->DocName);
		tf = fi.path()+"/"+fi.baseName()+".pdf";
	}
	file = tf;


	int num = 0;
	if (ScCore->primaryMainWindow()->HaveDoc)
		num = ScCore->primaryMainWindow()->doc->Pages->count();

	for (int i = 0; i<num; i++)
	{
		pages.append(i+1);
	}
	
	separation = "No";
}

QList<QVariant> PrinterAPI::allPrinters()
{
	QStringList list = PrinterUtil::getPrinterNames();
	QList<QVariant> l;
	for (int i=0; i < list.count(); ++i)
	{
		l.append(list[i]);
	}
	l.append(QString("File"));
	return l;
}

void PrinterAPI::doPrint()
{
// copied from void ScribusMainWindow::slotFilePrint() in file scribus.cpp
	QString fna, prn, cmd, scmd, cc, data, SepName;
	QString printcomm;
	bool fil, PSfile;
	PSfile = false;

//    ReOrderText(ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow()->view);
	prn = printer;
	fna = file;
	fil = (printer == QString("File")) ? true : false;
	std::vector<int> pageNs;
	PrintOptions options;
	for (int i = 0; i < pages.size(); ++i) {
		options.pageNumbers.push_back(pages[i]);
	}
	int Nr = (copies < 1) ? 1 : copies;
	SepName = separation;
	options.printer   = prn;
	options.prnEngine = (PrintEngine) pslevel;
	options.toFile    = fil;
	options.separationName = SepName;
	options.outputSeparations = (SepName == QString("No")) ?  false : true;
	options.useColor = color;
	options.mirrorH  = mph;
	options.mirrorV  = mpv;
	options.useICC   = useICC;
	options.doGCR    = ucr;
	options.cropMarks  = false;
	options.bleedMarks = false;
	options.registrationMarks = false;
	options.colorMarks = false;
	options.markOffset = 0.0;
	options.bleeds.Top    = 0.0;
	options.bleeds.Left   = 0.0;
	options.bleeds.Right  = 0.0;
	options.bleeds.Bottom = 0.0;
	if (!PrinterUtil::checkPrintEngineSupport(options.printer, options.prnEngine, options.toFile))
		options.prnEngine = PrinterUtil::getDefaultPrintEngine(options.printer, options.toFile);
	printcomm = cmd;
	QMap<QString, QMap<uint, FPointArray> > ReallyUsed;
	ReallyUsed.clear();
	ScCore->primaryMainWindow()->doc->getUsedFonts(ReallyUsed);
	PrefsManager *prefsManager=PrefsManager::instance();

#if defined(_WIN32)
	if (!options.toFile)
	{
		QByteArray devMode;
		bool printDone = false;
		if ( PrinterUtil::getDefaultSettings(prn, options.devMode) )
		{
			ScPrintEngine_GDI winPrint;
			printDone = winPrint.print( *ScCore->primaryMainWindow()->doc, options );
		}
		if (!printDone)
			RAISE("Printing failed");
	}
#endif

	PSLib *dd = new PSLib(options, true, prefsManager->appPrefs.fontPrefs.AvailFonts, ReallyUsed, ScCore->primaryMainWindow()->doc->PageColors, false, true);
	if (dd != NULL)
	{
		if (!fil)
			fna = QDir::toNativeSeparators(ScPaths::getTempFileDir()+"/tmp.ps");
		PSfile = dd->PS_set_file(fna);
		fna = QDir::toNativeSeparators(fna);
		if (PSfile)
		{
			options.setDevParam = false;
			options.doClip = false;
			dd->CreatePS(ScCore->primaryMainWindow()->doc, options);
			if (options.prnEngine == PostScript1 || options.prnEngine == PostScript2)
			{
				if (ScCore->haveGS())
				{
					QString tmp;
					QStringList opts;
					opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(tmp.setNum(ScCore->primaryMainWindow()->doc->pageWidth())) );
					opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(tmp.setNum(ScCore->primaryMainWindow()->doc->pageHeight())) );
					convertPS2PS(fna, fna+".tmp", opts, options.prnEngine);
					moveFile( fna + ".tmp", fna );
				}
				else
				{
					RAISE("Printing failed : GhostScript is needed to print to PostScript Level 1 or Level 2");
				}
			}

			if (!fil)
			{
				if (!printcomm.isEmpty())
					cmd = printcomm + " "+fna;
				else
				{
					cmd = "lpr -P" + prn;
					if (Nr > 1)
						cmd += " -#" + cc.setNum(Nr);
#ifdef HAVE_CUPS
// This need yet to be implemented by object Printer
//					cmd += printer->PrinterOpts;
#endif
					cmd += " "+fna;
				}
				system(cmd.toLocal8Bit().constData());
				unlink(fna.toLocal8Bit().constData());
			}
		}
		else {
			delete dd;
			RAISE("Printing failed");
		}
		delete dd;
	}
}

QString PrinterAPI::getPrinter()
{
	return this->printer;
}

void PrinterAPI::setPrinter(QString name)
{
	this->printer = name;
}

QString PrinterAPI::getFile()
{
	return this->file;
}

void PrinterAPI::setFile(QString name)
{
	this->file = name;
}

QString PrinterAPI::getCmd()
{
	return this->cmd;
}

void PrinterAPI::setCmd(QString name)
{
	this->cmd = name;
}

QString PrinterAPI::getSeparation()
{
	return this->separation;
}

void PrinterAPI::setSeparation(QString name)
{
	this->separation = name;
}


QList<QVariant> PrinterAPI::getPages()
{
	QList<QVariant> p;
	for(int i=0; i <pages.count(); i++)
	{
		p.append(pages[i]);
	}
	return p;
}

void PrinterAPI::setPages(QList<QVariant> list)
{
	this->pages.clear();
	for(int i=0; i<list.count(); ++i)
	{
		if(list[i].toInt() > ScCore->primaryMainWindow()->doc->Pages->count())
		{
			qDebug()<<"trying to raise an exception";
			RAISE("pages value out of range.");
		}
		else
			this->pages.append(list[i].toInt());
	}
}

bool PrinterAPI::isColor()
{
	return this->color;
}

void PrinterAPI::setColor(bool value)
{
	this->color = value;
}

bool PrinterAPI::getUseICC()
{
	return this->useICC;
}

void PrinterAPI::setUseICC(bool value)
{
	this->useICC = value;
}

int PrinterAPI::getPsLevel()
{
	return this->pslevel;
}

void PrinterAPI::setPsLevel(int value)
{
	this->pslevel = value;
}

bool PrinterAPI::getMph()
{
	return this->mph;
}

void PrinterAPI::setMph(bool value)
{
	this->mph = value;
}

bool PrinterAPI::getMpv()
{
	return this->mpv;
}

void PrinterAPI::setMpv(bool value)
{
	this->mpv = value;
}

bool PrinterAPI::getUcr()
{
	return this->ucr;
}

void PrinterAPI::setUcr(bool value)
{
	this->ucr = value;
}

PrinterAPI::~PrinterAPI()
{
	qDebug() << "PrinterAPI deleted";
}


