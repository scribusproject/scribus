/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Okt 03 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef RTFIM_H
#define RTFIM_H

#include "pluginapi.h"
#include "pageitem.h"

#include <QString>

extern "C" PLUGIN_API void GetText2(QString filename, QString encoding, bool textOnly, PageItem *textItem);
extern "C" PLUGIN_API QString FileFormatName();
extern "C" PLUGIN_API QStringList FileExtensions();

#endif			/* XTGIM_H */
