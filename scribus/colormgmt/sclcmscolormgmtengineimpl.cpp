/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <iostream>
#include <string>
#include <QDir>
#include <QFile>

#include "sclcmscolormgmtengineimpl.h"
#include "sclcmscolorprofileimpl.h"
#include "sclcmscolortransformimpl.h"

#ifndef cmsFLAGS_PRESERVEBLACK
#define cmsFLAGS_PRESERVEBLACK 0x8000
#endif

QSharedPointer<ScColorProfileCache>  ScLcmsColorMgmtEngineImpl::m_profileCache;
QSharedPointer<ScColorTransformPool> ScLcmsColorMgmtEngineImpl::m_transformPool;

ScLcmsColorMgmtEngineImpl::ScLcmsColorMgmtEngineImpl()
                         : ScColorMgmtEngineData("Littlecms v1", 0)
{
	if (!m_profileCache)
		m_profileCache = QSharedPointer<ScColorProfileCache>(new ScColorProfileCache());
	if (!m_transformPool)
		m_transformPool = QSharedPointer<ScColorTransformPool>(new ScColorTransformPool());
	cmsSetAlarmCodes(0, 255, 0);
}

QList<ScColorProfileInfo> ScLcmsColorMgmtEngineImpl::getAvailableProfileInfo(const QString& directory, bool recursive)
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
			cmsSetErrorHandler(&cmsErrorHandler);
			try
			{
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
				profileInfo.colorSpace  = translateLcmsColorSpaceType( cmsGetColorSpace(hIn) );
				profileInfo.deviceClass = translateLcmsProfileClass( cmsGetDeviceClass(hIn) );
				profileInfos.append(profileInfo);
				cmsCloseProfile(hIn);
				hIn = NULL;
			}
			catch (lcmsException&)
			{
				// Profile is broken
				if (hIn)
				{
					cmsCloseProfile(hIn);
					hIn = NULL;
				}
				profileInfo.debug = QString("Color profile %1 is broken").arg(fi.filePath());
				profileInfos.append(profileInfo);
			}
			cmsSetErrorHandler(NULL);
		}
	}
	cmsSetErrorHandler(NULL);

	return profileInfos;
}

ScColorProfile ScLcmsColorMgmtEngineImpl::openProfileFromFile(const QString& filePath)
{
	// Search profile in profile cache first
	ScColorProfile profile = m_profileCache->profile(filePath);
	if (!profile.isNull())
		return profile;
	cmsHPROFILE lcmsProf = NULL;
	cmsSetErrorHandler(&cmsErrorHandler);
	try
	{
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
					ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(lcmsProf);
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
	}
	catch (lcmsException& e)
	{
		std::cerr << e.what() << std::endl;
		if (profile.isNull() && lcmsProf)
			cmsCloseProfile(lcmsProf);
		profile = ScColorProfile();
	}
	cmsSetErrorHandler(NULL);
	return profile;
}

ScColorProfile ScLcmsColorMgmtEngineImpl::openProfileFromMem(const QByteArray& data)
{
	ScColorProfile profile;
	cmsHPROFILE lcmsProf = NULL;
	cmsSetErrorHandler(&cmsErrorHandler);
	try
	{
		lcmsProf = cmsOpenProfileFromMem((LPVOID) data.data(), data.size());
		if (lcmsProf)
		{
			ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(lcmsProf);
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
	}
	catch (lcmsException& e)
	{
		std::cerr << e.what() << std::endl;
		if (profile.isNull() && lcmsProf)
			cmsCloseProfile(lcmsProf);
		profile = ScColorProfile();
	}
	cmsSetErrorHandler(NULL);
	return profile;
}

ScColorProfile ScLcmsColorMgmtEngineImpl::createProfile_sRGB()
{
	QString internalProfilePath("memprofile://Internal sRGB profile");
	ScColorProfile profile = m_profileCache->profile(internalProfilePath);
	if (!profile.isNull())
		return profile;

	cmsHPROFILE lcmsProf = NULL;
	cmsSetErrorHandler(&cmsErrorHandler);
	try
	{
		lcmsProf = cmsCreate_sRGBProfile();
		if (lcmsProf)
		{
			ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(lcmsProf);
			profData->m_profilePath = internalProfilePath;
			profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
			m_profileCache->addProfile(profile);
		}
		if (profile.isNull() && lcmsProf)
		{
			cmsCloseProfile(lcmsProf);
			lcmsProf = NULL;
		}
	}
	catch (lcmsException& e)
	{
		std::cerr << e.what() << std::endl;
		if (profile.isNull() && lcmsProf)
			cmsCloseProfile(lcmsProf);
		profile = ScColorProfile();
	}
	cmsSetErrorHandler(NULL);
	return profile;
}

ScColorProfile ScLcmsColorMgmtEngineImpl::createProfile_Lab()
{
	QString internalProfilePath("memprofile://Internal Lab profile");
	ScColorProfile profile = m_profileCache->profile(internalProfilePath);
	if (!profile.isNull())
		return profile;

	cmsHPROFILE lcmsProf = NULL;
	cmsSetErrorHandler(&cmsErrorHandler);
	try
	{
		lcmsProf = cmsCreateLabProfile(NULL);
		if (lcmsProf)
		{
			ScLcmsColorProfileImpl* profData = new ScLcmsColorProfileImpl(lcmsProf);
			profData->m_profilePath = internalProfilePath;
			profile = ScColorProfile(dynamic_cast<ScColorProfileData*>(profData));
			m_profileCache->addProfile(profile);
		}
		if (profile.isNull() && lcmsProf)
		{
			cmsCloseProfile(lcmsProf);
			lcmsProf = NULL;
		}
	}
	catch (lcmsException& e)
	{
		std::cerr << e.what() << std::endl;
		if (profile.isNull() && lcmsProf)
			cmsCloseProfile(lcmsProf);
		profile = ScColorProfile();
	}
	cmsSetErrorHandler(NULL);
	return profile;
}

ScColorTransform ScLcmsColorMgmtEngineImpl::createTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                             const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                 eRenderIntent renderIntent, long transformFlags)
{
	ScColorTransform transform(NULL);
	if (inputProfile.isNull() || outputProfile.isNull())
		return transform;
	const ScLcmsColorProfileImpl* lcmsInputProf  = dynamic_cast<const ScLcmsColorProfileImpl*>(inputProfile.data());
	const ScLcmsColorProfileImpl* lcmsOutputProf = dynamic_cast<const ScLcmsColorProfileImpl*>(outputProfile.data());
	if (!lcmsInputProf || !lcmsOutputProf)
		return transform;

	transformFlags &= (~Ctf_Softproofing);
	transformFlags &= (~Ctf_GamutCheck);

	ScColorTransformInfo transInfo;
	transInfo.inputProfile  = inputProfile.productDescription();
	transInfo.outputProfile = outputProfile.productDescription();
	transInfo.proofingProfile = QString();
	transInfo.inputFormat   = inputFormat;
	transInfo.outputFormat  = outputFormat;
	transInfo.renderIntent  = renderIntent;
	transInfo.proofingIntent = (eRenderIntent) 0;
	transInfo.flags = transformFlags;

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
		DWORD lcmsFlags     = translateFlagsToLcmsFlags(transformFlags);
		DWORD lcmsInputFmt  = translateFormatToLcmsFormat(inputFormat);
		DWORD lcmsOutputFmt = translateFormatToLcmsFormat(outputFormat);
		int   lcmsIntent    = translateIntentToLcmsIntent(renderIntent);
		if (nullTransform)
			lcmsFlags |= cmsFLAGS_NULLTRANSFORM;
		cmsHTRANSFORM hTransform = NULL;
		cmsSetErrorHandler(&cmsErrorHandler);
		try
		{
			hTransform = cmsCreateTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
											lcmsOutputProf->m_profileHandle, lcmsOutputFmt, 
											lcmsIntent, lcmsFlags | cmsFLAGS_LOWRESPRECALC);
			if (hTransform)
			{
				ScLcmsColorTransformImpl* newTrans = new ScLcmsColorTransformImpl(hTransform);
				newTrans->setTransformInfo(transInfo);
				transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
				m_transformPool->addTransform(transform, true);
			}
		}
		catch (lcmsException& e)
		{
			std::cerr << e.what() << std::endl;
			// #9922 : no idea why that crash in release mode
			/*if (transform.isNull() && hTransform)
				cmsDeleteTransform(hTransform);*/
			transform = ScColorTransform();
		}
		cmsSetErrorHandler(NULL);
	}
	return transform;
}

ScColorTransform ScLcmsColorMgmtEngineImpl::createProofingTransform(
                                             const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                             const ScColorProfile& proofProfile , eRenderIntent renderIntent, 
                                             eRenderIntent proofingIntent, long transformFlags)
{
	ScColorTransform transform(NULL);
	if (inputProfile.isNull() || outputProfile.isNull())
		return transform;
	const ScLcmsColorProfileImpl* lcmsInputProf    = dynamic_cast<const ScLcmsColorProfileImpl*>(inputProfile.data());
	const ScLcmsColorProfileImpl* lcmsOutputProf   = dynamic_cast<const ScLcmsColorProfileImpl*>(outputProfile.data());
	const ScLcmsColorProfileImpl* lcmsProofingProf = dynamic_cast<const ScLcmsColorProfileImpl*>(proofProfile.data());
	if (!lcmsInputProf || !lcmsOutputProf || !lcmsProofingProf)
		return transform;

	ScColorTransformInfo transInfo;
	transInfo.inputProfile    = inputProfile.productDescription();
	transInfo.outputProfile   = outputProfile.productDescription();
	transInfo.proofingProfile = proofProfile.productDescription();
	transInfo.inputFormat     = inputFormat;
	transInfo.outputFormat    = outputFormat;
	transInfo.renderIntent    = renderIntent;
	transInfo.proofingIntent  = proofingIntent;
	transInfo.flags = transformFlags;

	DWORD lcmsFlags     = translateFlagsToLcmsFlags(transformFlags);
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
		transform = m_transformPool->findTransform(transInfo);
		if (transform.isNull())
		{
			cmsSetErrorHandler(&cmsErrorHandler);
			cmsHTRANSFORM hTransform = NULL;
			try
			{
				hTransform = cmsCreateProofingTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
														lcmsOutputProf->m_profileHandle, lcmsOutputFmt,
														lcmsProofingProf->m_profileHandle, lcmsIntent, 
														lcmsPrfIntent, lcmsFlags | cmsFLAGS_SOFTPROOFING);
				if (hTransform)
				{
					ScLcmsColorTransformImpl* newTrans = new ScLcmsColorTransformImpl(hTransform);
					newTrans->setTransformInfo(transInfo);
					transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
					m_transformPool->addTransform(transform, true);
				}
			}
			catch (lcmsException& e)
			{
				std::cerr << e.what() << std::endl;
				// #9922 : no idea why that crash in release mode
				/*if (transform.isNull() && hTransform)
					cmsDeleteTransform(hTransform);*/
				transform = ScColorTransform();
			}
			cmsSetErrorHandler(NULL);
		}
	}
	else
	{
		transformFlags  &= (~Ctf_Softproofing);
		transformFlags  &= (~Ctf_GamutCheck);
		lcmsFlags        = translateFlagsToLcmsFlags(transformFlags);
		transInfo.flags  = transformFlags;
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
			cmsSetErrorHandler(&cmsErrorHandler);
			cmsHTRANSFORM hTransform = NULL;
			try
			{
				hTransform  = cmsCreateTransform(lcmsInputProf->m_profileHandle , lcmsInputFmt, 
											     lcmsOutputProf->m_profileHandle, lcmsOutputFmt, 
												 lcmsPrfIntent, lcmsFlags | cmsFLAGS_LOWRESPRECALC);
				if (hTransform)
				{
					ScLcmsColorTransformImpl* newTrans = new ScLcmsColorTransformImpl(hTransform);
					newTrans->setTransformInfo(transInfo);
					transform = ScColorTransform(dynamic_cast<ScColorTransformData*>(newTrans));
					m_transformPool->addTransform(transform, true);
				}
			}
			catch (lcmsException& e)
			{
				std::cerr << e.what() << std::endl;
				// #9922 : no idea why that crash in release mode
				/*if (transform.isNull() && hTransform)
					cmsDeleteTransform(hTransform);*/
				transform = ScColorTransform();
			}
			cmsSetErrorHandler(NULL);
		}
	}
	return transform;
}

DWORD ScLcmsColorMgmtEngineImpl::translateFlagsToLcmsFlags(long flags)
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
	if (flags & Ctf_LowResPrecalc)
		lFlags |= cmsFLAGS_LOWRESPRECALC;
	if (flags & Ctf_NullTransform)
		lFlags |= cmsFLAGS_NULLTRANSFORM;
	return lFlags;
}

DWORD ScLcmsColorMgmtEngineImpl::translateFormatToLcmsFormat(eColorFormat format)
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

int ScLcmsColorMgmtEngineImpl::translateIntentToLcmsIntent(eRenderIntent intent, eRenderIntent defIntent)
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

eColorSpaceType ScLcmsColorMgmtEngineImpl::translateLcmsColorSpaceType(icColorSpaceSignature signature)
{
	eColorSpaceType colorSpaceType = ColorSpace_Unknown;
    if (signature == icSigXYZData)
		colorSpaceType = ColorSpace_XYZ;
    if (signature == icSigLabData)
		colorSpaceType = ColorSpace_Lab;
    if (signature == icSigLuvData)
		colorSpaceType = ColorSpace_Luv;
    if (signature == icSigYCbCrData)
		colorSpaceType = ColorSpace_YCbCr;
    if (signature == icSigYxyData)
		colorSpaceType = ColorSpace_Yxy;
    if (signature == icSigRgbData)
		colorSpaceType = ColorSpace_Rgb;
    if (signature == icSigGrayData)
		colorSpaceType = ColorSpace_Gray;
    if (signature == icSigHsvData)
		colorSpaceType = ColorSpace_Hsv;
    if (signature == icSigHlsData)
		colorSpaceType = ColorSpace_Hls;
    if (signature == icSigCmykData)
		colorSpaceType = ColorSpace_Cmyk;
    if (signature == icSigCmyData)
		colorSpaceType = ColorSpace_Cmy;
	return colorSpaceType;
}

eProfileClass ScLcmsColorMgmtEngineImpl::translateLcmsProfileClass(icProfileClassSignature signature)
{
	eProfileClass profileClass = Class_Unknown;
	if (signature == icSigInputClass)
		profileClass = Class_Input;
    if (signature == icSigDisplayClass)
		profileClass = Class_Display;
    if (signature == icSigOutputClass)
		profileClass = Class_Output;
    if (signature == icSigLinkClass)
		profileClass = Class_Link;
    if (signature == icSigAbstractClass)
		profileClass = Class_Abstract;
    if (signature == icSigColorSpaceClass)
		profileClass = Class_ColorSpace;
    if (signature == icSigNamedColorClass)
		profileClass = Class_NamedColor;
	return profileClass;
}

int ScLcmsColorMgmtEngineImpl::cmsErrorHandler(int /*ErrorCode*/, const char *ErrorText)
{
	std::string msg = std::string("Littlecms : ") + ErrorText;
	throw lcmsException(msg.c_str());
	return 1;
}
