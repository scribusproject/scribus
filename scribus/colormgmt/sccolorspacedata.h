/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCOLORSPACEDATA_H
#define SCCOLORSPACEDATA_H

#include <QIODevice>
#include "sccolormgmtelem.h"
#include "sccolorprofile.h"
#include "sccolortransform.h"
#include "sccolormgmtstructs.h"

class ScColorSpaceData : public ScColorMgmtElem
{
protected:
	eColorFormat   m_colorFormat;
	ScColorProfile m_profile;
	
public:

	eColorType   type() const             { return colorFormatType(m_colorFormat); }
	eColorFormat colorFormat() const      { return m_colorFormat; }
	const ScColorProfile& profile() const { return m_profile; }

	uint  numChannels(void)     const { return colorFormatNumChannels(m_colorFormat); }
	uint  bytesPerChannel(void) const { return colorFormatBytesPerChannel(m_colorFormat); }
	bool  hasAlphaChannel(void) const { return colorFormatHasAlpha(m_colorFormat); }

	virtual ScColorMgmtEngine& engine() { return m_profile.engine(); }
	virtual const ScColorMgmtEngine& engine() const { return m_profile.engine(); }

	// Index of alpha data
	virtual uint  alphaIndex(void) const = 0;

	// Restore full opacity of alpha channel
	virtual void flattenAlpha(void* dataIn, uint numElems) const = 0;

	// Function to create transform to a specific output color space
	virtual ScColorTransform createTransform(const ScColorSpaceData& outputSpace, eRenderIntent renderIntent, 
	                                         long transformFlags);
	virtual ScColorTransform createTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 eRenderIntent renderIntent, long transformFlags);

	// Function to create proofing transform to a specific output color space and a specific proofing device
	virtual ScColorTransform createProofingTransform(const ScColorSpaceData& outputSpace, const ScColorProfile& proofing, 
	                                         eRenderIntent renderIntent,  eRenderIntent proofingIntent, 
											 long transformFlags);
	virtual ScColorTransform createProofingTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofing, eRenderIntent renderIntent, 
	                                         eRenderIntent proofingIntent, long transformFlags);

	// Convert color data to a specific color space
	virtual bool convert(ScColorSpaceData& data, eRenderIntent renderIntent, long transformFlags, 
	                     void* dataIn, void* dataOut, uint numElems, ScColorTransform* lastTrans = 0);
	virtual bool convert(ScColorSpaceData& data, eRenderIntent renderIntent, long transformFlags, 
	                     void* dataIn, QIODevice* device, uint numElems, ScColorTransform* lastTrans = 0);
};

#endif
