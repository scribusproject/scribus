#include "config.h"
#include <qapplication.h>
#include <qfile.h>
#include <qtextstream.h>

#include <iostream>

#include "scfonts_encoding.h"

#ifndef PREL
#define PREL "/usr/local"
#endif


bool SCFonts_Encoding::read(QString name)
{
	QString ISO;
	QString pfad = PREL;
	pfad += "/lib/scribus/" + name;
	QFile tf(pfad);
	if (tf.open(IO_ReadOnly))
	{
		QTextStream t(&tf);
		ISO=t.read();
		tf.close();
	}
	else
		return(false);
	QTextStream isoTs(&ISO, IO_ReadOnly);
	isoTs << oct;
	while (!isoTs.atEnd())
	{
		QString iNam;
		int iNr;
		isoTs >> iNam >> iNr;
		Codes[iNam] = iNr;
		Names[iNr]=iNam;
	}
}


SCFonts_Encoding::SCFonts_Encoding(QString name)
{
	read(name);
}
