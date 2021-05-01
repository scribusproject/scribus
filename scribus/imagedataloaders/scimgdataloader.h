/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_H
#define SCIMGDATALOADER_H

#include "scribusapi.h"
#include <QByteArray>
#include <QImage>
#include <QStringList>

#include "colormgmt/sccolormgmtstructs.h"
#include "scimagestructs.h"
#include "rawimage.h"

class ScImgDataLoader
{
public:
	virtual ~ScImgDataLoader() {};

	const QStringList& supportedFormats() const { return m_supportedFormats; }
	bool  supportFormat(const QString& fmt);

	RawImage    r_image;

	QImage&          image() { return m_image; }
	QByteArray&      embeddedProfile() { return m_embeddedProfile; }
	ImageInfoRecord& imageInfoRecord() { return m_imageInfoRecord; }
	eColorFormat     pixelFormat() { return m_pixelFormat; }
	void             setRequest(bool valid, const QMap<int, ImageLoadRequest>& req);

	bool  issuedErrorMsg(void)      const { return (m_msgType == errorMsg); }
	bool  issuedWarningMsg(void)    const { return (m_msgType == warningMsg); }
	const QString& getMessage(void) const { return m_message; }

	virtual void initialize(void);

	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) = 0;
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0) = 0;
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail) = 0;
	virtual bool useRawImage() const { return false; }

protected:
	ScImgDataLoader();

	QStringList m_supportedFormats;

	QImage          m_image;
	ImageInfoRecord m_imageInfoRecord;
	QByteArray      m_embeddedProfile;
	int             m_profileComponents;
	eColorFormat    m_pixelFormat;

	typedef enum
	{
		noMsg = 0,
		warningMsg = 1,
		errorMsg = 2
	} MsgType;
	MsgType m_msgType;
	QString m_message;

	void swapRGBA();
	void swapRGBA(QImage *img);

	QString getPascalString(QDataStream & s);
	double decodePSDfloat(uint data);
	void parseResourceData( QDataStream & s, const PSDHeader & header, uint size );
};

#endif
