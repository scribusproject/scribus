/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCOLORSPACE_H
#define SCCOLORSPACE_H

#include <QSharedPointer>
#include <QWeakPointer>
#include "scribusapi.h"
#include "sccolorprofile.h"
#include "sccolorspacedata.h"
#include "sccolortransform.h"

class SCRIBUS_API ScColorSpace
{
public:
	ScColorSpace();
	ScColorSpace(ScColorSpaceData*);
	ScColorSpace(const QSharedPointer<ScColorSpaceData>&);

	ScColorMgmtEngine& engine() { return m_data->engine(); }
	const ScColorMgmtEngine& engine() const { return m_data->engine(); }

	inline bool isNull()    const { return m_data.isNull(); }
	inline operator bool () const { return !isNull(); }

	eColorType   type() const;
	eColorFormat colorFormat() const;
	ScColorProfile profile() const;

	uint  numChannels(void)     const;
	uint  bytesPerChannel(void) const;

	bool  hasAlphaChannel(void) const;
	uint  alphaIndex(void)      const;

	// Restore full opacity of alpha channel
	void flattenAlpha(void* dataIn, uint numElems);

	// Function to create transform to a specific output color space
	ScColorTransform createTransform(const ScColorSpace& outputSpace, eRenderIntent renderIntent, 
	                                 long transformFlags);
	ScColorTransform createTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
	                                 eRenderIntent renderIntent, long transformFlags);

	// Function to create proofing transform to a specific output color space and a specific proofing device
	ScColorTransform createProofingTransform(const ScColorSpace& outputSpace, const ScColorProfile& proofing, 
	                                         eRenderIntent renderIntent,  eRenderIntent proofingIntent, 
											 long transformFlags);
	ScColorTransform createProofingTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofing, eRenderIntent renderIntent, 
	                                         eRenderIntent proofingIntent, long transformFlags);

	// Convert color data to a specific color space
	bool convert(ScColorSpace& data, eRenderIntent renderIntent, long transformFlags, 
	             void* dataIn, void* dataOut, uint numElems, ScColorTransform* lastTrans = 0);
	bool convert(ScColorSpace& data, eRenderIntent renderIntent, long transformFlags, 
	             void* dataIn, QIODevice* device, uint numElems, ScColorTransform* lastTrans = 0);

	const ScColorSpaceData* data() const { return m_data.data(); }

	bool operator==(const ScColorSpace& cspace) const;

protected:
	QSharedPointer<ScColorSpaceData> m_data;

	QWeakPointer<ScColorSpaceData>   weakRef()   const { return m_data.toWeakRef(); }
	QSharedPointer<ScColorSpaceData> strongRef() const { return m_data; }
};

#endif
