/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <QDir>
#include <QFile>

#include "sclcms2colormgmtengineimpl.h"
#include "sclcms2colorprofileimpl.h"
#include "sclcms2colortransformimpl.h"

#ifndef cmsFLAGS_PRESERVEBLACK
#define cmsFLAGS_PRESERVEBLACK 0x8000
#endif

QSharedPointer<ScColorProfileCache>  ScLcms2ColorMgmtEngineImpl::m_profileCache;
QSharedPointer<ScColorTransformPool> ScLcms2ColorMgmtEngineImpl::m_transformPool;

ScLcms2ColorMgmtEngineImpl::ScLcms2ColorMgmtEngineImpl()
                         : ScColorMgmtEngineData("Littlecms v2", 1)
{
	if (!m_profileCache)
		m_profileCache = QSharedPointer<ScColorProfileCache>(new ScColorProfileCache());
	if (!m_transformPool)
		m_transformPool = QSharedPointer<ScColorTransformPool>(new ScColorTransformPool(0));
	cmsUInt16Number alarmCodes[cmsMAXCHANNELS] = { 0 };
	alarmCodes[1] = 0xFFFF;
	cmsSetAlarmCodes(alarmCodes);
	cmsSetLogErrorHandler(&cmsErrorHandler);
}

void ScLcms2ColorMgmtEngineImpl::setStrategy(const ScColorMgmtStrategy& strategy)
{
	m_strategy = strategy;
}

QList<ScColorProfileInfo> ScLcms2ColorMgmtEngineImpl::getAvailableProfileInfo(const QString& directory, bool recursive)
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
			hIn = cmsOpenProfileFromFile(profilePath.data(), "r");
			if (hIn == NULL)
				continue;
#ifdef _WIN32
			cmsUInt32Number descSize = cmsGetProfileInfo(hIn, cmsInfoDescription, "en", "US", NULL, 0);
			if (descSize > 0)
			{
				wchar_t* descData = (wchar_t*) malloc(descSize + sizeof(wchar_t));
				descSize = cmsGetProfileInfo(hIn, cmsInfoDescription, "en", "US", descData, descSize);
				if (descSize > 0)
				{
					uint stringLen = descSize / sizeof(wchar_t);
					descData[stringLen] = 0;
					if (sizeof(wchar_t) == sizeof(QChar)) {
						profileInfo.description = QString::fromUtf16((ushort *) descData);
					} else {
						profileInfo.description = QString::fromUcs4((uint *) descData);
					}
					free(descData);
				}
			}
#else
			cmsUInt32Number descSize = cmsGetProfileInfoASCII(hIn, cmsInfoDescription, "en", "US", NULL, 0);
			if (descSize > 0)
			{
				char* descData = (char*) malloc(descSize + sizeof(char));
				descSize = cmsGetProfileInfoASCII(hIn, cmsInfoDescription, "en", "US", descData, descSize);
				if (descSize > 0)
				{
					profileInfo.description = QString(descData);
					free(descData);
				}
			}
#endif
			if (profileInfo.description.isEmpty())
			{
				cmsCloseProfile(hIn);
				profileInfo.debug = QString("Color profile %1 is broken : no valid description").arg(fi.filePath());
				profileInfos.append(profileInfo);
				continue;
			}
			profileInfo.colorSpace  = translateLcmsColorSpaceType( cmsGetColorSpace(hIn) );
			profileInfo.deviceClass = translateLcmsProfileClass( cmsGetDeviceClass(hIn) );
			profileInfos.append(profileInfo);
			cmsCloseProfile(hIn);
			hIn = NULL;
		}
	}

	return profileInfos;
}

ScColorProfile ScLcms2ColorMgmtEngineImpl::openProfileFromFile(ScColorMgmtEngine& engine, const QString& filePath)
{
	// Search profile in profile cache first
	ScColorProfile profile = m_profileCache->profile(filePath);
	if (!profile.isNull())
		return profile;
	cmsHPROFILE lcmsProf = NULL;
	QFile file(filePath);
	if (file.open(QFile::ReadOnly))
	{
		// We do not use lcms cmsOpenProfileFromFile() to avoid limitations
		// of I/O on 8bit filenames on Windows
		QByteArray data = file.readAll();
		if (!data.isEmpty())
		{
			lcmsProf = cmsOpenProfileFromMem(data.data(), data.size());
			if (lcmsProf)
			{
				ScLcms2ColorProfileImpl* profData = new ScLcms2ColorProfileImpl(engine, lcmsProf);
				profData->m_profileData = data;
				profData->m_profilePath = filePath;
				profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
				m_profileCache->addProfile(profile);
			}
			if (profile.isNull() && lcmsProf)
			{
				cmsCloseProfile(lcmsProf);
				lcmsProf = NULL;
			}
		}
		file.close();
	}
	return profile;
}

ScColorProfile ScLcms2ColorMgmtEngineImpl::openProfileFromMem(ScColorMgmtEngine& engine, const QByteArray& data)
{
	ScColorProfile profile;
	cmsHPROFILE	lcmsProf = cmsOpenProfileFromMem((const void *) data.data(), data.size());
	if (lcmsProf)
	{
		ScLcms2ColorProfileImpl* profData = new ScLcms2ColorProfileImpl(engine, lcmsProf);
		QString desc = profData->productDescription();
		if (!desc.isEmpty())
			profData->m_profilePath = QString("memprofile://%1").arg(desc);
		profData->m_profileData = data;
		profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
	}
	if (profile.isNull() && lcmsProf)
	{
		cmsCloseProfile(lcmsProf);
		lcmsProf = NULL;
	}
	return profile;
}

ScColorProfile ScLcms2ColorMgmtEngineImpl::createProfile_sRGB(ScColorMgmtEngine& engine)
{
	QString internalProfilePath("memprofile://Internal sRGB profile");
	ScColorProfile profile = m_profileCache->profile(internalProfilePath);
	if (!profile.isNull())
		return profile;

	cmsHPROFILE lcmsProf = cmsCreate_sRGBProfile();
	if (lcmsProf)
	{
		ScLcms2ColorProfileImpl* profData = new ScLcms2ColorProfileImpl(engine, lcmsProf);
		profData->m_profilePath = internalProfilePath;
		profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
		m_profileCache->addProfile(profile);
	}
	if (profile.isNull() && lcmsProf)
	{
		cmsCloseProfile(lcmsProf);
		lcmsProf = NULL;
	}
	return profile;
}

ScColorProfile ScLcms2ColorMgmtEngineImpl::createProfile_Lab(ScColorMgmtEngine& engine)
{
	QString internalProfilePath("memprofile://Internal Lab profile");
	ScColorProfile profile = m_profileCache->profile(internalProfilePath);
	if (!profile.isNull())
		return profile;

	cmsHPROFILE lcmsProf = cmsCreateLab2Profile(NULL);
	if (lcmsProf)
	{
		ScLcms2ColorProfileImpl* profData = new ScLcms2ColorProfileImpl(engine, lcmsProf);
		profData->m_profilePath = internalProfilePath;
		profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
		m_profileCache->addProfile(profile);
	}
	if (profile.isNull() && lcmsProf)
	{
		cmsCloseProfile(lcmsProf);
		lcmsProf = NULL;
	}
	return profile;
}

ScColorTransform ScLcms2ColorMgmtEngineImpl::createTransform(ScColorMgmtEngine& engine,
                                 const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                             const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                 eRenderIntent renderIntent, long transformFlags)
{
	ScColorTransform transform(NULL);
	if (inputProfile.isNull() || outputProfile.isNull())
		return transform;
	int inputProfEngineID  = inputProfile.engine().engineID();
	int outputProfEngineID = outputProfile.engine().engineID();
	if ((engine.engineID() != m_engineID) || (inputProfEngineID != m_engineID) || (outputProfEngineID != m_engineID))
		return transform;
	const ScLcms2ColorProfileImpl* lcmsInputProf  = dynamic_cast<const ScLcms2ColorProfileImpl*>(inputProfile.data());
	const ScLcms2ColorProfileImpl* lcmsOutputProf = dynamic_cast<const ScLcms2ColorProfileImpl*>(outputProfile.data());
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

	transform = m_transformPool->findTransform(transInfo);
	if (transform.isNull())
	{
		cmsUInt32Number lcmsFlags     = translateFlagsToLcmsFlags(transformFlags | strategyFlags);
		cmsUInt32Number lcmsInputFmt  = translateFormatToLcmsFormat(inputFormat);
		cmsUInt32Number lcmsOutputFmt = translateFormatToLcmsFormat(outputFormat);
		int   lcmsIntent    = translateIntentToLcmsIntent(renderIntent);
		if (nullTransform)
			lcmsFlags |= cmsFLAGS_NULLTRANSFORM;
		cmsHTRANSFORM hTransform = NULL;
		hTransform = cmsCreateTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
										lcmsOutputProf->m_profileHandle, lcmsOutputFmt, 
										lcmsIntent, lcmsFlags | cmsFLAGS_LOWRESPRECALC);
		if (hTransform)
		{
			ScLcms2ColorTransformImpl* newTrans = new ScLcms2ColorTransformImpl(engine, hTransform);
			newTrans->setTransformInfo(transInfo);
			transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
			m_transformPool->addTransform(transform, true);
		}
	}
	return transform;
}

ScColorTransform ScLcms2ColorMgmtEngineImpl::createProofingTransform(ScColorMgmtEngine& engine,
                                             const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                             const ScColorProfile& proofProfile , eRenderIntent renderIntent, 
                                             eRenderIntent proofingIntent, long transformFlags)
{
	ScColorTransform transform(NULL);
	if (inputProfile.isNull() || outputProfile.isNull())
		return transform;
	int inputProfEngineID  = inputProfile.engine().engineID();
	int outputProfEngineID = outputProfile.engine().engineID();
	int proofProfEngineID  = proofProfile.engine().engineID();
	if ((engine.engineID()  != m_engineID) || (inputProfEngineID != m_engineID) || 
		(outputProfEngineID != m_engineID) || (proofProfEngineID != m_engineID))
		return transform;
	const ScLcms2ColorProfileImpl* lcmsInputProf    = dynamic_cast<const ScLcms2ColorProfileImpl*>(inputProfile.data());
	const ScLcms2ColorProfileImpl* lcmsOutputProf   = dynamic_cast<const ScLcms2ColorProfileImpl*>(outputProfile.data());
	const ScLcms2ColorProfileImpl* lcmsProofingProf = dynamic_cast<const ScLcms2ColorProfileImpl*>(proofProfile.data());
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

	cmsUInt32Number lcmsFlags     = translateFlagsToLcmsFlags(transformFlags | strategyFlags);
	cmsUInt32Number lcmsInputFmt  = translateFormatToLcmsFormat(inputFormat);
	cmsUInt32Number lcmsOutputFmt = translateFormatToLcmsFormat(outputFormat);
	int   lcmsIntent    = translateIntentToLcmsIntent(renderIntent);
	int   lcmsPrfIntent = translateIntentToLcmsIntent(proofingIntent);

	if (transInfo.inputProfile != transInfo.proofingProfile)
	{
		if (transInfo.proofingProfile == transInfo.outputProfile)
		{
			transInfo.proofingIntent = Intent_Relative_Colorimetric;
			lcmsPrfIntent = translateIntentToLcmsIntent(Intent_Relative_Colorimetric);
		}
		transform = m_transformPool->findTransform(transInfo);
		if (transform.isNull())
		{
			cmsHTRANSFORM hTransform = NULL;
			hTransform = cmsCreateProofingTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
													lcmsOutputProf->m_profileHandle, lcmsOutputFmt,
													lcmsProofingProf->m_profileHandle, lcmsIntent, 
													lcmsPrfIntent, lcmsFlags | cmsFLAGS_SOFTPROOFING);
			if (hTransform)
			{
				ScLcms2ColorTransformImpl* newTrans = new ScLcms2ColorTransformImpl(engine, hTransform);
				newTrans->setTransformInfo(transInfo);
				transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
				m_transformPool->addTransform(transform, true);
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
		transform = m_transformPool->findTransform(transInfo);
		if (transform.isNull())
		{
			cmsHTRANSFORM hTransform = NULL;
			hTransform  = cmsCreateTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
										     lcmsOutputProf->m_profileHandle, lcmsOutputFmt, 
											 lcmsPrfIntent, lcmsFlags | cmsFLAGS_LOWRESPRECALC);
			if (hTransform)
			{
				ScLcms2ColorTransformImpl* newTrans = new ScLcms2ColorTransformImpl(engine, hTransform);
				newTrans->setTransformInfo(transInfo);
				transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
				m_transformPool->addTransform(transform, true);
			}
		}
	}
	return transform;
}

cmsUInt32Number ScLcms2ColorMgmtEngineImpl::translateFlagsToLcmsFlags(long flags)
{
	cmsUInt32Number lFlags = 0;
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

cmsUInt32Number ScLcms2ColorMgmtEngineImpl::translateFormatToLcmsFormat(eColorFormat format)
{
	cmsUInt32Number lFormat = 0;
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

int ScLcms2ColorMgmtEngineImpl::translateIntentToLcmsIntent(eRenderIntent intent, eRenderIntent defIntent)
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

eColorSpaceType ScLcms2ColorMgmtEngineImpl::translateLcmsColorSpaceType(cmsColorSpaceSignature signature)
{
	eColorSpaceType colorSpaceType = ColorSpace_Unknown;
    if (signature == cmsSigXYZData)
		colorSpaceType = ColorSpace_XYZ;
    if (signature == cmsSigLabData)
		colorSpaceType = ColorSpace_Lab;
    if (signature == cmsSigLuvData)
		colorSpaceType = ColorSpace_Luv;
    if (signature == cmsSigYCbCrData)
		colorSpaceType = ColorSpace_YCbCr;
    if (signature == cmsSigYxyData)
		colorSpaceType = ColorSpace_Yxy;
    if (signature == cmsSigRgbData)
		colorSpaceType = ColorSpace_Rgb;
    if (signature == cmsSigGrayData)
		colorSpaceType = ColorSpace_Gray;
    if (signature == cmsSigHsvData)
		colorSpaceType = ColorSpace_Hsv;
    if (signature == cmsSigHlsData)
		colorSpaceType = ColorSpace_Hls;
    if (signature == cmsSigCmykData)
		colorSpaceType = ColorSpace_Cmyk;
    if (signature == cmsSigCmyData)
		colorSpaceType = ColorSpace_Cmy;
	return colorSpaceType;
}

eProfileClass ScLcms2ColorMgmtEngineImpl::translateLcmsProfileClass(cmsProfileClassSignature signature)
{
	eProfileClass profileClass = Class_Unknown;
	if (signature == cmsSigInputClass)
		profileClass = Class_Input;
    if (signature == cmsSigDisplayClass)
		profileClass = Class_Display;
    if (signature == cmsSigOutputClass)
		profileClass = Class_Output;
    if (signature == cmsSigLinkClass)
		profileClass = Class_Link;
    if (signature == cmsSigAbstractClass)
		profileClass = Class_Abstract;
    if (signature == cmsSigColorSpaceClass)
		profileClass = Class_ColorSpace;
    if (signature == cmsSigNamedColorClass)
		profileClass = Class_NamedColor;
	return profileClass;
}

void ScLcms2ColorMgmtEngineImpl::cmsErrorHandler(cmsContext contextID, cmsUInt32Number /*ErrorCode*/, 
												 const char *ErrorText)
{
	std::string msg = std::string("Littlecms : ") + ErrorText;
	std::cerr << ErrorText << std::endl;
}
