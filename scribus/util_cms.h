/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMSUTIL_H
#define CMSUTIL_H

#include "scconfig.h"
#include "scribusapi.h"
#include CMS_INC

cmsHTRANSFORM scCmsCreateTransform(cmsHPROFILE Input, DWORD InputFormat,
                                cmsHPROFILE Output,DWORD OutputFormat,
                                int Intent, DWORD dwFlags);

cmsHTRANSFORM scCmsCreateProofingTransform(cmsHPROFILE Input, DWORD InputFormat,
								cmsHPROFILE Output,DWORD OutputFormat,
								cmsHPROFILE Proofing,int Intent, 
								int ProofingIntent, DWORD dwFlags);

#endif
