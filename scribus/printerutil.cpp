/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "printerutil.h"

#if defined( HAVE_CUPS )
 #include <cups/cups.h>
#elif defined(_WIN32)
 #include <windows.h>
 #include <winspool.h>
#endif

#include <qstringlist.h>

#include "util.h"

QStringList PrinterUtil::getPrinterNames()
{
	QString printerName;
	QStringList printerNames;
#if defined (HAVE_CUPS)
	cups_dest_t *dests;
	int num_dests = cupsGetDests(&dests);
	for (int pr = 0; pr < num_dests; ++pr)
	{
		printerName = QString(dests[pr].name);
		printerNames.append(printerName);
	}
	cupsFreeDests(num_dests, dests);
#elif defined(_WIN32)
	DWORD size;
	DWORD numPrinters;
	PRINTER_INFO_2* printerInfos = NULL;
    EnumPrinters ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, 2, NULL, 0, &size, &numPrinters );
	printerInfos = (PRINTER_INFO_2*) malloc(size);
	if ( EnumPrinters ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, (LPBYTE) printerInfos, size, &size, &numPrinters ) )
	{
		for ( uint i = 0; i < numPrinters; i++)
		{
			printerName = printerInfos[i].pPrinterName;
			printerNames.append(printerName);
		}
		printerNames.sort();	
	}
	if ( printerInfos) free(printerInfos);
#else
	QString tmp;
	QString Pcap;
	QStringList wt;
	if (loadText("/etc/printcap", &Pcap))
	{
		QTextStream ts(&Pcap, IO_ReadOnly);
		while(!ts.atEnd())
		{
			tmp = ts.readLine();
			if (tmp.isEmpty())
				continue;
			if ((tmp[0] != '#') && (tmp[0] != ' ') && (tmp[0] != '\n') && (tmp[0] != '\t'))
			{
				tmp = tmp.stripWhiteSpace();
				tmp = tmp.left(tmp.length() - (tmp.right(2) == ":\\" ? 2 : 1));
				wt = QStringList::split("|", tmp);
				printerName = wt[0];
				printerNames.append(printerName);
			}
		}
	}
#endif
	return printerNames;
}
