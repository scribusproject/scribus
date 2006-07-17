#ifndef SCIMGDATALOADER_H
#define SCIMGDATALOADER_H

#include "scribusapi.h"
#include <qcstring.h>
#include <qimage.h>
#include <qstringlist.h>

#include "scimagestructs.h"

class ScImgDataLoader
{
protected:
	ScImgDataLoader(void);

	QStringList m_supportedFormats;

	QImage m_image;
	ImageInfoRecord m_imageInfoRecord;
	QByteArray m_embeddedProfile;
	int m_profileComponents;

	void swapRGBA(void);

	QString getPascalString(QDataStream & s);
	void parseRessourceData( QDataStream & s, const PSDHeader & header, uint size );

public:
	virtual ~ScImgDataLoader(void) {};

	const QStringList& supportedFormats(void) const { return m_supportedFormats; }
	bool  supportFormat(const QString& fmt);

	QImage& image(void) { return m_image; }
	QByteArray& embeddedProfile(void) { return m_embeddedProfile; }
	ImageInfoRecord& imageInfoRecord(void) { return m_imageInfoRecord; }

	virtual void initialize(void);

	virtual void preloadAlphaChannel(const QString& fn, int res) = 0;
	virtual void loadEmbeddedProfile(const QString& fn) = 0;
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail) = 0;
};

#endif
