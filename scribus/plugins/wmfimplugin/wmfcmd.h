/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/* Original code from KOffice libwmf */

#ifndef WMFCMD_H
#define WMFCMD_H

class WmfCmd
{
public:
	WmfCmd()  { params = NULL; }
    ~WmfCmd() { if (params) delete params; }
    unsigned short funcIndex;
    long   numParam;
    short* params;
};

#endif
