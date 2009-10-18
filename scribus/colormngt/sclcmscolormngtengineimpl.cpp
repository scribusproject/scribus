/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <iostream>
#include <QDir>
#include <QFile>

#include "sclcmscolormngtengineimpl.h"
#include "sclcmscolorprofileimpl.h"
#include "sclcmscolortransformimpl.h"

#ifndef cmsFLAGS_PRESERVEBLACK
#define cmsFLAGS_PRESERVEBLACK 0x8000
#endif

jmp_buf ScLcmsColorMngtEngineImpl::cmsJumpBuffer;

QSharedPointer<ScColorProfileCache> ScLcmsColorMngtEngineImpl::m_profileCache;

ScLcmsColorMngtEngineImpl::ScLcmsColorMngtEngineImpl()
                         : ScColorMngtEngineData("Littlecms v1", 0)
{
	if (!m_profileCache)
		m_profileCache = QSharedPointer<ScColorProfileCache>(new ScColorProfileCache());
	cmsSetAlarmCodes(0, 255, 0);
}

void ScLcmsColorMngtEngineImpl::setStrategy(const ScColorMngtStrategy& strategy)
{
	m_strategy = strategy;
	m_transformPool.clear();
}

QList<ScColorProfileInfo> ScLcmsColorMngtEngineImpl::getAvailableProfileInfo(const QString& directory, bool recursive)
{
	QList<ScColorProfileInfo> profileInfos;

	QDir d(directory, "*", QDir::Name, QDir::Files | QDir::Readable | QDir::Dirs | QDir::NoSymLinks);
	if ((!d.exists()) || (d.count() == 0))
		return profileInfos;

	QString nam = "";
	cmsHPROFILE hIn = NULL;

	for (uint dc = 0; dc < d.count(); ++dc)
	{
		QString file = d[dc];
		if (file == "." ||  file == "..")
			continue;
		QFileInfo fi(directory + "/" + file);
		if (fi.isDir() && !recursive)
			continue;
		else if (fi.isDir() && !file.startsWith('.'))
		{
			QList<ScColorProfileInfo> profileInfos2 = getAvailableProfileInfo(fi.filePath()+"/", true);
			profileInfos.append(profileInfos2);
			continue;
		}

		ScColorProfileInfo profileInfo;
		profileInfo.file = fi.filePath();

		QFile f(fi.filePath());
		QByteArray bb(40, ' ');
		if (!f.open(QIODevice::ReadOnly)) {
			profileInfo.debug = QString("couldn't open %1 as color profile").arg(fi.filePath());
			profileInfos.append(profileInfo);
			continue;
		}
		int len = f.read(bb.data(), 40);
		f.close();
		if (len == 40 && bb[36] == 'a' && bb[37] == 'c' && bb[38] == 's' && bb[39] == 'p')
		{
			const QByteArray profilePath( QString(directory + "/" + file).toLocal8Bit() );
			if (setjmp(cmsJumpBuffer))
			{
				// Profile is broken
				profileInfo.debug = QString("Color profile %1 is broken").arg(fi.filePath());
				profileInfos.append(profileInfo);
				// Reset to the default handler otherwise may enter a loop
				// if an error occur in cmsCloseProfile()
				cmsSetErrorHandler(NULL);
				if (hIn)
				{
					cmsCloseProfile(hIn);
					hIn = NULL;
				}
				continue;
			}
			cmsSetErrorHandler(&cmsErrorHandler);
			hIn = cmsOpenProfileFromFile(profilePath.data(), "r");
			if (hIn == NULL)
				continue;
			const char* profileDescriptor = cmsTakeProductDesc(hIn);
			profileInfo.description = QString(profileDescriptor);
			if (profileInfo.description.isEmpty())
			{
				cmsCloseProfile(hIn);
				profileInfo.debug = QString("Color profile %1 is broken : no valid description").arg(fi.filePath());
				profileInfos.append(profileInfo);
				continue;
			}
			profileInfo.colorSpace  = cmsGetColorSpace(hIn);
			profileInfo.deviceClass = cmsGetDeviceClass(hIn);
			profileInfos.append(profileInfo);
			cmsCloseProfile(hIn);
			hIn = NULL;
		}
	}
	cmsSetErrorHandler(NULL);

	return profileInfos;
}

ScColorProfile ScLcmsColorMngtEngineImpl::openProfileFromFile(ScColorMngtEngine& engine, const QString& filePath)
{
	// Search profile in profile cache first
	ScColorProfile profile;
	// We do not use lcms cmsOpenProfileFromFile() to avoid limitations
	// of I/O on 8bit filenames on Windows
	QFile file(filePath);
	if (file.open(QFile::ReadOnly))
	{
		QByteArray data = file.readAll();
		if (!data.isEmpty())
		{
			cmsHPROFILE lcmsProf = cmsOpenProfileFromMem(data.data(), data.size());
			if (lcmsProf)
			{
				ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(engine, lcmsProf);
				profData->m_profileData = data;
				profData->m_profilePath = filePath;
				profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
			}
			if (profile.isNull() && lcmsProf)
				cmsCloseProfile(lcmsProf);
		}
		file.close();
	}
	return profile;
}

ScColorProfile ScLcmsColorMngtEngineImpl::openProfileFromMem(ScColorMngtEngine& engine, const QByteArray& data)
{
	ScColorProfile profile;
	cmsHPROFILE lcmsProf = cmsOpenProfileFromMem((LPVOID) data.data(), data.size());
	if (lcmsProf)
	{
		ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(engine, lcmsProf);
		QString desc = profData->productDescription();
		if (!desc.isEmpty())
			profData->m_profilePath = QString("memprofile://%1").arg(desc);
		profData->m_profileData = data;
		profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
	}
	if (profile.isNull() && lcmsProf)
		cmsCloseProfile(lcmsProf);
	return profile;
}

ScColorProfile ScLcmsColorMngtEngineImpl::createProfile_sRGB(ScColorMngtEngine& engine)
{
	ScColorProfile profile;
	cmsHPROFILE lcmsProf = cmsCreate_sRGBProfile();
	if (lcmsProf)
	{
		ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(engine, lcmsProf);
		profData->m_profilePath = QString("memprofile://Internal sRGB profile");
		profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
	}
	if (profile.isNull() && lcmsProf)
		cmsCloseProfile(lcmsProf);
	return profile;
}

ScColorProfile ScLcmsColorMngtEngineImpl::createProfile_Lab(ScColorMngtEngine& engine)
{
	ScColorProfile profile;
	cmsHPROFILE lcmsProf = cmsCreateLabProfile(NULL);
	if (lcmsProf)
	{
		ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(engine, lcmsProf);
		profData->m_profilePath = QString("memprofile://Internal Lab profile");
		profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
	}
	if (profile.isNull() && lcmsProf)
		cmsCloseProfile(lcmsProf);
	return profile;
}

ScColorTransform ScLcmsColorMngtEngineImpl::createTransform(ScColorMngtEngine& engine,
                                 const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                             const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                 eRenderIntent renderIntent, long transformFlags)
{
	ScColorTransform transform(NULL);
	if (inputProfile.isNull() || outputProfile.isNull())
		return transform;
	if ((engine.engineID() != m_engineID) || (inputProfile.engine() != engine) || (outputProfile.engine() != engine))
		return transform;
	const ScLcmsColorProfileImpl* lcmsInputProf  = dynamic_cast<const ScLcmsColorProfileImpl*>(inputProfile.data());
	const ScLcmsColorProfileImpl* lcmsOutputProf = dynamic_cast<const ScLcmsColorProfileImpl*>(outputProfile.data());
	if (!lcmsInputProf || !lcmsOutputProf)
		return transform;

	transformFlags &= (~Ctf_Softproofing);
	transformFlags &= (~Ctf_GamutCheck);
	long strategyFlags = 0;
	if (m_strategy.useBlackPointCompensation)
		strategyFlags |= Ctf_BlackPointCompensation;
	if (m_strategy.useBlackPreservation)
		strategyFlags |= Ctf_BlackPreservation;

	ScColorTransformInfo transInfo;
	transInfo.inputProfile  = inputProfile.productDescription();
	transInfo.outputProfile = outputProfile.productDescription();
	transInfo.proofingProfile = QString();
	transInfo.inputFormat   = inputFormat;
	transInfo.outputFormat  = outputFormat;
	transInfo.renderIntent  = renderIntent;
	transInfo.proofingIntent = (eRenderIntent) 0;
	transInfo.flags = transformFlags | strategyFlags;

	bool nullTransform = false;
	if (transInfo.inputProfile == transInfo.outputProfile)
	{
		// This is a null transform
		transInfo.inputProfile    = QString();
		transInfo.outputProfile   = QString();
		transInfo.proofingProfile = QString();
		transInfo.renderIntent    = (eRenderIntent) 0;
		transInfo.proofingIntent  = (eRenderIntent) 0;
		transInfo.flags = 0;
		nullTransform = true;
	}

	transform = m_transformPool.findTransform(transInfo);
	if (transform.isNull())
	{
		DWORD lcmsFlags     = translateFlagsToLcmsFlags(transformFlags | strategyFlags);
		DWORD lcmsInputFmt  = translateFormatToLcmsFormat(inputFormat);
		DWORD lcmsOutputFmt = translateFormatToLcmsFormat(outputFormat);
		int   lcmsIntent    = translateIntentToLcmsIntent(renderIntent);
		if (nullTransform)
			lcmsFlags |= cmsFLAGS_NULLTRANSFORM;
		cmsHTRANSFORM hTransform = cmsCreateTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
			                                          lcmsOutputProf->m_profileHandle, lcmsOutputFmt, 
													  lcmsIntent, lcmsFlags | cmsFLAGS_LOWRESPRECALC);
		if (hTransform)
		{
			ScLcmsColorTransformImpl* newTrans = new ScLcmsColorTransformImpl(engine, hTransform);
			newTrans->setTransformInfo(transInfo);
			transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
		}
	}
	return transform;
}

ScColorTransform ScLcmsColorMngtEngineImpl::createProofingTransform(ScColorMngtEngine& engine,
                                             const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                             const ScColorProfile& proofProfile , eRenderIntent renderIntent, 
                                             eRenderIntent proofingIntent, long transformFlags)
{
	ScColorTransform transform(NULL);
	if (inputProfile.isNull() || outputProfile.isNull())
		return transform;
	if ((engine.engineID() != m_engineID) || (inputProfile.engine() != engine) || 
		(outputProfile.engine() != engine)|| (proofProfile.engine() != engine) )
		return transform;
	const ScLcmsColorProfileImpl* lcmsInputProf    = dynamic_cast<const ScLcmsColorProfileImpl*>(inputProfile.data());
	const ScLcmsColorProfileImpl* lcmsOutputProf   = dynamic_cast<const ScLcmsColorProfileImpl*>(outputProfile.data());
	const ScLcmsColorProfileImpl* lcmsProofingProf = dynamic_cast<const ScLcmsColorProfileImpl*>(outputProfile.data());
	if (!lcmsInputProf || !lcmsOutputProf || !lcmsProofingProf)
		return transform;

	long strategyFlags = 0;
	if (m_strategy.useBlackPointCompensation)
		strategyFlags |= Ctf_BlackPointCompensation;
	if (m_strategy.useBlackPreservation)
		strategyFlags |= Ctf_BlackPreservation;

	ScColorTransformInfo transInfo;
	transInfo.inputProfile    = inputProfile.productDescription();
	transInfo.outputProfile   = outputProfile.productDescription();
	transInfo.proofingProfile = proofProfile.productDescription();
	transInfo.inputFormat     = inputFormat;
	transInfo.outputFormat    = outputFormat;
	transInfo.renderIntent    = renderIntent;
	transInfo.proofingIntent  = proofingIntent;
	transInfo.flags = transformFlags | strategyFlags;

	DWORD lcmsFlags     = translateFlagsToLcmsFlags(transformFlags | strategyFlags);
	DWORD lcmsInputFmt  = translateFormatToLcmsFormat(inputFormat);
	DWORD lcmsOutputFmt = translateFormatToLcmsFormat(outputFormat);
	int   lcmsIntent    = translateIntentToLcmsIntent(renderIntent);
	int   lcmsPrfIntent = translateIntentToLcmsIntent(proofingIntent);

	if (transInfo.inputProfile != transInfo.proofingProfile)
	{
		if (transInfo.proofingProfile == transInfo.outputProfile)
		{
			transInfo.proofingIntent = Intent_Relative_Colorimetric;
			lcmsPrfIntent = translateIntentToLcmsIntent(Intent_Relative_Colorimetric);
		}
		transform = m_transformPool.findTransform(transInfo);
		if (transform.isNull())
		{
			cmsHTRANSFORM hTransform = cmsCreateProofingTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
			                                          lcmsOutputProf->m_profileHandle, lcmsOutputFmt,
		                                              lcmsProofingProf->m_profileHandle, lcmsIntent, 
													  lcmsPrfIntent, lcmsFlags | cmsFLAGS_SOFTPROOFING);
			if (hTransform)
			{
				ScLcmsColorTransformImpl* newTrans = new ScLcmsColorTransformImpl(engine, hTransform);
				newTrans->setTransformInfo(transInfo);
				transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
			}
		}
	}
	else
	{
		transformFlags  &= (~Ctf_Softproofing);
		transformFlags  &= (~Ctf_GamutCheck);
		lcmsFlags        = translateFlagsToLcmsFlags(transformFlags | strategyFlags);
		transInfo.flags  = transformFlags | strategyFlags;
		transInfo.renderIntent   = proofingIntent;
		transInfo.proofingIntent = (eRenderIntent) 0;
		if (transInfo.inputProfile == transInfo.outputProfile)
		{
			lcmsFlags |= cmsFLAGS_NULLTRANSFORM;
			transInfo.inputProfile    = QString();
			transInfo.outputProfile   = QString();
			transInfo.proofingProfile = QString();
			transInfo.renderIntent    = (eRenderIntent) 0;
			transInfo.proofingIntent  = (eRenderIntent) 0;
			transInfo.flags = 0;
		}
		transform = m_transformPool.findTransform(transInfo);
		if (transform.isNull())
		{
			cmsHTRANSFORM hTransform = cmsCreateTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
														  lcmsOutputProf->m_profileHandle, lcmsOutputFmt, 
														  lcmsPrfIntent, lcmsFlags | cmsFLAGS_LOWRESPRECALC);
			if (hTransform)
			{
				ScLcmsColorTransformImpl* newTrans = new ScLcmsColorTransformImpl(engine, hTransform);
				newTrans->setTransformInfo(transInfo);
				transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
			}
		}
	}
	return transform;
}

DWORD ScLcmsColorMngtEngineImpl::translateFlagsToLcmsFlags(long flags)
{
	DWORD lFlags = 0;
	if (flags & Ctf_BlackPointCompensation)
		lFlags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
	if (flags & Ctf_BlackPreservation)
		lFlags |= cmsFLAGS_PRESERVEBLACK;
	if (flags & Ctf_Softproofing)
		lFlags |= cmsFLAGS_SOFTPROOFING;
	if (flags & Ctf_GamutCheck)
		lFlags |= cmsFLAGS_GAMUTCHECK;
	return lFlags;
}

DWORD ScLcmsColorMngtEngineImpl::translateFormatToLcmsFormat(eColorFormat format)
{
	DWORD lFormat = 0;
	if (format == Format_RGB_8)
		lFormat = TYPE_RGB_8;
	if (format == Format_RGB_16)
		lFormat = TYPE_RGB_16;
	if (format == Format_RGBA_8)
		lFormat = TYPE_RGBA_8;
	if (format == Format_RGBA_16)
		lFormat = TYPE_RGBA_16;
	if (format == Format_ARGB_8)
		lFormat = TYPE_ARGB_8;
	if (format == Format_ARGB_16)
		lFormat = TYPE_ARGB_16;
	if (format == Format_BGRA_8)
		lFormat = TYPE_BGRA_8;
	if (format == Format_BGRA_16)
		lFormat = TYPE_BGRA_16;
	if (format == Format_CMYK_8)
		lFormat = TYPE_CMYK_8;
	if (format == Format_CMYK_16)
		lFormat = TYPE_CMYK_16;
	if (format == Format_CMYKA_8)
		lFormat = (COLORSPACE_SH(PT_CMYK)|EXTRA_SH(1)|CHANNELS_SH(4)|BYTES_SH(1));
	if (format == Format_CMYKA_16)
		lFormat = (COLORSPACE_SH(PT_CMYK)|EXTRA_SH(1)|CHANNELS_SH(4)|BYTES_SH(2));
	if (format == Format_YMCK_8)
		lFormat = (COLORSPACE_SH(PT_CMYK)|CHANNELS_SH(4)|BYTES_SH(1)|DOSWAP_SH(1)|SWAPFIRST_SH(1));
	if (format == Format_YMCK_16)
		lFormat = (COLORSPACE_SH(PT_CMYK)|CHANNELS_SH(4)|BYTES_SH(2)|DOSWAP_SH(1)|SWAPFIRST_SH(1));
	if (format == Format_GRAY_8)
		lFormat = TYPE_GRAY_8;
	if (format == Format_GRAY_16)
		lFormat = TYPE_GRAY_16;
	if (format == Format_LabA_8)
		lFormat = COLORSPACE_SH(PT_Lab)|CHANNELS_SH(3)|BYTES_SH(1)|EXTRA_SH(1);
	return lFormat;
}

int ScLcmsColorMngtEngineImpl::translateIntentToLcmsIntent(eRenderIntent intent, eRenderIntent defIntent)
{
	int lIntent = defIntent;
	if (intent == Intent_Perceptual)
		lIntent = INTENT_PERCEPTUAL;
	if (intent == Intent_Relative_Colorimetric)
		lIntent = INTENT_RELATIVE_COLORIMETRIC;
	if (intent == Intent_Saturation)
		lIntent = INTENT_SATURATION;
	if (intent == Intent_Absolute_Colorimetric)
		lIntent = INTENT_ABSOLUTE_COLORIMETRIC;
	return lIntent;
}

int ScLcmsColorMngtEngineImpl::cmsErrorHandler(int /*ErrorCode*/, const char *ErrorText)
{
	std::cerr << "Littlecms : " << ErrorText << std::endl;
	longjmp(cmsJumpBuffer, 1);
	return 1;
}
