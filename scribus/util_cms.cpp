/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "util_cms.h"
#include <QString>

cmsHTRANSFORM scCmsCreateTransform(cmsHPROFILE Input, DWORD InputFormat,
                                cmsHPROFILE Output,DWORD OutputFormat,
                                int Intent, DWORD dwFlags)
{
	cmsHTRANSFORM hTransform = NULL;
	if ( Input == NULL || Output == NULL)
		return NULL;
	dwFlags &= (~cmsFLAGS_GAMUTCHECK);
	dwFlags &= (~cmsFLAGS_SOFTPROOFING);
	QString desc1 = cmsTakeProductDesc(Input);
	QString desc2 = cmsTakeProductDesc(Output);
	if ( desc1 == desc2 )
		dwFlags |= cmsFLAGS_NULLTRANSFORM;
	hTransform = cmsCreateTransform(Input, InputFormat, Output, OutputFormat, Intent, dwFlags);
	return hTransform;
}

cmsHTRANSFORM scCmsCreateProofingTransform(cmsHPROFILE Input, DWORD InputFormat,
								cmsHPROFILE Output,DWORD OutputFormat,
								cmsHPROFILE Proofing, int Intent, 
								int ProofingIntent, DWORD dwFlags)
{
	cmsHTRANSFORM hTransform = NULL;
	if ( Input == NULL || Output == NULL || Proofing == NULL)
		return NULL;
	QString desc1 = cmsTakeProductDesc(Input);
	QString desc2 = cmsTakeProductDesc(Output);
	QString desc3 = cmsTakeProductDesc(Proofing);
	dwFlags &= ~cmsFLAGS_BLACKPOINTCOMPENSATION;
	if ( desc1 != desc3 )
	{
		if( desc2 == desc3 )
			ProofingIntent = INTENT_RELATIVE_COLORIMETRIC;
		hTransform = cmsCreateProofingTransform(Input, InputFormat, Output, OutputFormat, Proofing, Intent, 
												ProofingIntent, dwFlags);
	}
	else
	{
		dwFlags &= (~cmsFLAGS_GAMUTCHECK);
		dwFlags &= (~cmsFLAGS_SOFTPROOFING);
		if ( desc1 == desc2 )
			dwFlags |= cmsFLAGS_NULLTRANSFORM;
		hTransform = cmsCreateTransform(Input, InputFormat, Output, OutputFormat, ProofingIntent, dwFlags);
	}
	return hTransform;
}
