/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scdocoutput_ps2.h"
#include "scpageoutput_ps2.h"
#include "scribuscore.h"
#include "cmserrorhandling.h"
#include "commonstrings.h"
#include "scribusdoc.h"
#include "page.h"

using namespace std;

#include CMS_INC

ScDocOutput_Ps2::ScDocOutput_Ps2(ScribusDoc* doc, QString fileName, vector<int>& pageNumbers, QRect& clip, ScPs2OutputParams& options)
{
	m_doc = doc;
	m_file.setName(fileName);
	m_device = &m_file;
	m_pageNumbers = pageNumbers;
	m_clip = clip;
	m_options = options;
	m_author = doc->documentInfo.getAuthor();
	m_title  = doc->documentInfo.getTitle();
	m_creator = QString("Scribus ") + QString(VERSION);
}

ScDocOutput_Ps2::~ScDocOutput_Ps2()
{
	if (m_file.isOpen())
		m_file.close();
	closeTransforms();
}

void ScDocOutput_Ps2::begin(void)
{
	m_file.open(IO_WriteOnly);
	m_stream.setDevice(&m_file);

	m_stream << "%!PS-Adobe-2.0\n";
	m_stream << QString("%%For: %1\n").arg(m_author);
	m_stream << QString("%%Title: %1\n").arg(m_title);
	m_stream << QString("%%Creator: %1\n").arg(m_creator);
	m_stream << QString("%%Pages: %1\n").arg(m_pageNumbers.size());
	m_stream << QString("%%BoundingBox: 0 0 %1 %2\n").arg((int) m_clip.width()).arg((int) m_clip.height());
	m_stream << QString("%%HiResBoundingBox: 0 0 %1 %2\n").arg(m_clip.width()).arg(m_clip.height());
	//TODO Color description
	//m_stream << QString("%%CMYKCustomColor: ");
	//m_stream << QString("%%DocumentCustomColors: ");
	m_stream << "%%BeginSetup\n";
	if (m_options.toGray)
	{
		m_stream << "/setcmykcolor {exch 0.11 mul add exch 0.59 mul add exch 0.3 mul add\n";
		m_stream << "               dup 1 gt {pop 1} if 1 exch sub oldsetgray} bind def\n";
		m_stream << "/setrgbcolor {0.11 mul exch 0.59 mul add exch 0.3 mul add\n";
		m_stream << "              oldsetgray} bind def\n";
	}
	m_stream << QString("<< /PageSize [ %1 %2 ]\n").arg((int) m_clip.width()).arg((int) m_clip.height());
	m_stream << ">> setpagedevice\n";
	m_stream << "%%EndSetup\n";
}

void ScDocOutput_Ps2::end(void)
{
	m_stream << "%%Trailer\n";
	m_stream << "end\n";
	m_stream << "%%EOF\n";
	m_file.close();
}

void ScDocOutput_Ps2::closeTransforms(void)
{
	if (m_options.rgbToOutputColorTransform)
		cmsDeleteTransform(m_options.rgbToOutputColorTransform);
	if (m_options.rgbToOutputImageTransform)
		cmsDeleteTransform(m_options.rgbToOutputImageTransform);
	if (m_options.cmykToOutputColorTransform)
		cmsDeleteTransform(m_options.cmykToOutputColorTransform);
	if (m_options.cmykToOutputImageTransform)
		cmsDeleteTransform(m_options.cmykToOutputImageTransform);
	if (m_options.hProfile)
		cmsCloseProfile(m_options.hProfile);
	m_options.hProfile = NULL;
	m_options.rgbToOutputColorTransform = NULL;
	m_options.rgbToOutputImageTransform = NULL;
	m_options.cmykToOutputColorTransform = NULL;
	m_options.cmykToOutputImageTransform = NULL;
}

bool ScDocOutput_Ps2::initializeCmsTransforms(void)
{
	bool success = false;
	if (!m_options.outputProfile.isEmpty() && QFile::exists(m_options.outputProfile))
	{
		cmsErrorAction(LCMS_ERROR_ABORT);
		if (setjmp(cmsJumpBuffer))
		{
			cmsSetErrorHandler(NULL);
			cmsErrorAction(LCMS_ERROR_IGNORE);
			closeTransforms();
			cmsErrorAction(LCMS_ERROR_ABORT);
			m_lastError = QObject::tr("An error occurred while initializing icc transforms");
			qWarning( "%s", m_lastError.toLocal8Bit().data() );
			return false;
		}
		cmsSetErrorHandler(&cmsErrorHandler);

		int dcmsflags = 0;
		dcmsflags |= cmsFLAGS_LOWRESPRECALC;
		if (m_doc->BlackPoint)
			dcmsflags |= cmsFLAGS_BLACKPOINTCOMPENSATION;

		int outputDataTypeColors = 0;
		int outputDataTypeImages = 0;
		QByteArray outputProfilePath(m_options.outputProfile.toLocal8Bit());
		m_options.hProfile = cmsOpenProfileFromFile(outputProfilePath.data(), "r");
		if (static_cast<int>(cmsGetColorSpace(m_options.hProfile)) == icSigRgbData)
		{
			outputDataTypeColors = TYPE_RGB_16;
			outputDataTypeImages = TYPE_ARGB_8;
		}
		else if (static_cast<int>(cmsGetColorSpace(m_options.hProfile)) == icSigCmykData)
		{
			outputDataTypeColors = TYPE_CMYK_16;
			outputDataTypeImages = TYPE_CMYK_8;
		}
		else
		{
			m_lastError = QObject::tr("Output profile is not supported");
			return false;
		}
		m_options.rgbToOutputColorTransform = cmsCreateTransform(m_doc->DocInputRGBProf, TYPE_RGB_16, m_options.hProfile, 
													outputDataTypeColors, m_doc->IntentColors, dcmsflags); 
		m_options.rgbToOutputImageTransform = cmsCreateTransform(m_doc->DocInputRGBProf, TYPE_ARGB_8, m_options.hProfile, 
													outputDataTypeImages, m_doc->IntentImages, dcmsflags);
		m_options.cmykToOutputColorTransform = cmsCreateTransform(m_doc->DocInputRGBProf, TYPE_CMYK_16, m_options.hProfile, 
													outputDataTypeColors, m_doc->IntentColors, dcmsflags);
		m_options.cmykToOutputImageTransform = cmsCreateTransform(m_doc->DocInputRGBProf, TYPE_CMYK_8 , m_options.hProfile, 
													outputDataTypeImages, m_doc->IntentImages, dcmsflags);
		cmsSetErrorHandler(NULL);
		success = true;
	}
	return success;
}

ScPageOutput* ScDocOutput_Ps2::createPageOutputComponent(int pageIndex)
{
	ScPageOutput* po = new ScPageOutput_Ps2(m_device, m_doc, pageIndex, m_options);
	return po;
}
