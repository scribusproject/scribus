/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolormngtstructs.h"

bool operator==(const ScColorTransformInfo& v1, const ScColorTransformInfo& v2)
{
	return ((v1.inputProfile  == v2.inputProfile) && 
	        (v1.outputProfile == v2.outputProfile) &&
	        (v1.proofingProfile == v2.proofingProfile) &&
	        (v1.inputFormat   == v2.inputFormat) &&
	        (v1.outputFormat  == v2.outputFormat) &&
	        (v1.renderIntent  == v2.renderIntent) &&
            (v1.proofingIntent  == v2.proofingIntent) &&
			(v1.flags  == v2.flags));
}
