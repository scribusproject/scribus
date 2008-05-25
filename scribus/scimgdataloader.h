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

#include "scimagestructs.h"
#include "rawimage.h"

class ScImgDataLoader
{
protected:
	ScImgDataLoader(void);

	QStringList m_supportedFormats;

	QImage m_image;
	ImageInfoRecord m_imageInfoRecord;
	QByteArray m_embeddedProfile;
	int m_profileComponents;

	typedef enum {
		noMsg = 0,
		warningMsg = 1,
		errorMsg = 2
	} MsgType;
	MsgType m_msgType;
	QString m_message;

	void swapRGBA(void);
	void swapRGBA(QImage *img);

	QString getPascalString(QDataStream & s);
	void parseRessourceData( QDataStream & s, const PSDHeader & header, uint size );

public:
	virtual ~ScImgDataLoader(void) {};

	const QStringList& supportedFormats(void) const { return m_supportedFormats; }
	bool  supportFormat(const QString& fmt);

	QImage& image(void) { return m_image; }
	RawImage r_image;
	QByteArray& embeddedProfile(void) { return m_embeddedProfile; }
	ImageInfoRecord& imageInfoRecord(void) { return m_imageInfoRecord; }
	void setRequest(bool valid, QMap<int, ImageLoadRequest> req);

	bool  issuedErrorMsg(void) const { return (m_msgType == errorMsg); }
	bool  issuedWarningMsg(void) const { return (m_msgType == warningMsg); }
	const QString& getMessage(void) const { return m_message; }

	virtual void initialize(void);

//	virtual void preloadAlphaChannel(const QString& fn, int res) = 0;
	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) = 0;
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0) = 0;
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail) = 0;
};

#endif
