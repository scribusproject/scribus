/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/* Code inspired by KOffice libwmf and adapted for Scribus by Jean Ghali */

#include "scconfig.h"
#include "wmfimport.h"

#include <QCursor>
#include <QDrag>
#include <QBuffer>
#include <QDataStream>
#include <QFile>
#include <QMimeData>
#include <QPainterPath>
#include <QRegExp>

#include "customfdialog.h"
#include "scribus.h"
#include "scribusXml.h"
#include "propertiespalette.h"
#include "prefsfile.h"

#include <cmath>
#include "commonstrings.h"
#include "fpointarray.h"
#include "menumanager.h"
#include "prefsmanager.h"
#include "pageitem.h"
#include "scmimedata.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"
#include "loadsaveplugin.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"
#include "fonts/scfontmetrics.h"

using namespace std;

#include "wmfstruct.h"
#include "wmfcmd.h"
#include "wmfcontext.h"
#include "wmfhandle.h"
#include "metafuncs.h"

#define WMFIMPORT_DEBUG 0
#define MAX_OBJHANDLE   128

#define ANSI_CHARSET			0		// ANSI charset (Windows-1252)
#define DEFAULT_CHARSET			1
#define SYMBOL_CHARSET			2
#define MAC_CHARSET				77
#define SHIFTJIS_CHARSET		128		// Shift JIS charset    (Windows-932)
#define HANGEUL_CHARSET			129		// Hangeul charset      (Windows-949)
#define JOHAB_CHARSET			130		// Johab charset        (Windows-1361)
#define GB2312_CHARSET			134		// GB2312 charset       (Windows-936)
#define CHINESEBIG5_CHARSET		136		// Chinese Big5 charset (Windows-950)
#define GREEK_CHARSET			161		// Greek charset        (Windows-1253)
#define TURKISH_CHARSET			162		// Turkish charset      (Windows-1254)
#define VIETNAMESE_CHARSET		163		// Vietnamese charset   (Windows-1258)
#define HEBREW_CHARSET			177		// Hebrew charset       (Windows-1255)
#define ARABIC_CHARSET			178		// Arabic charset       (Windows-1256)
#define BALTIC_CHARSET			186		// Baltic charset       (Windows-1257)
#define RUSSIAN_CHARSET			204		// Cyrillic charset     (Windows-1251)
#define THAI_CHARSET			222		// Thai charset         (Windows-874)
#define EASTEUROPE_CHARSET		238		// Eastern european charset (Windows-1250)
#define OEM_CHARSET				255

/* Symbol to unicode conversion table copied from http://root.cern.ch/viewvc/trunk/qt/src/TQtSymbolCodec.cxx
This code is placed under GPL licence (http://root.cern.ch/viewvc/trunk/LICENSE) */
static const ushort greek_symbol_to_unicode[64] = {
//
//  upper case letters:
//  ----------------
//   Alpha   Beta     Chi   Delta   Epsilon   Phi    Gamma    Eta
	0x0391, 0x0392, 0x03A7, 0x0394, 0x0395, 0x03A6, 0x0393, 0x0397,
//   Iota   Theta(S) Kappa  Lambda    Mu      Nu    Omicron   Pi
	0x0399, 0x03D1, 0x039A, 0x039B, 0x039C, 0x039D, 0x039F, 0x03A0,
//  Theta    Rho    Sigma    Tau    Upsilon Stigma   Omega     Xi
	0x0398, 0x03A1, 0x03A3, 0x03A4, 0x03A5, 0x03DB, 0x03A9, 0x039E,
//   Psi     Zeta   Sigma    Tau    Upsilon  ????   Omega     Xi
	0x03A8, 0x0396, 0x03EA, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD,
//
//  lower case letters:
//  ----------------
//   Alpha   Beta    Chi    Delta   Epsilon   Phi    Gamma    Eta
	0x03B1, 0x03B2, 0x03C7, 0x03B4, 0x03B5, 0x03D5, 0x03B3, 0x03B7,
//   Iota   Phi (2) Kappa   Lambda    Mu      Nu    Omicron   Pi
	0x03B9, 0x03C6, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BF, 0x03C0,
//  Theta    Rho    Sigma    Tau    Upsilon OmegaPi Omega     Xi
	0x03B8, 0x03C1, 0x03C3, 0x03C4, 0x03C5, 0x03D6, 0x03B9, 0x03BE,
//   Psi     Zeta   Sigma    Tau    Upsilon  ????   Omega     Xi
	0x03C8, 0x03B6, 0x03EA, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD
};

/* Symbol to unicode conversion table copied from http://root.cern.ch/viewvc/trunk/qt/src/TQtSymbolCodec.cxx
This code is placed under GPL licence (http://root.cern.ch/viewvc/trunk/LICENSE) */
static const ushort symbol_to_unicode[96] = {
//
//  upper case letters:
//  ----------------
// UPSILON  NUMBER  LessEq   Div   INFINITY ???????  CLUB   DIAMOND     
	0x03D2, 0x0374, 0x2264, 0x2215, 0x221E, 0x0395, 0x2663, 0x2666,
//   HEART  SPADE   <--->   <---             -->             RING
	0x2665, 0x2660, 0x2194, 0x2190, 0x2191, 0x2192, 0x2193, 0x2218,
//  Theta    Rho    Great    ????   Propor  diff    bullet   ????
	0x0398, 0x03A1, 0x2265, 0x03A4, 0x221D, 0x2202, 0x2219, 0x03A8,
//   NonEq  Ident    ????   ellips   div     minus  ?????   aleph
	0x2260, 0x2261, 0xFFFD, 0x22EF, 0x2223, 0x2212, 0xFFFD, 0xFFFD,
//
//  lower case letters:
//  ----------------
//   Alpha   Beta    Chi    Delta   Epsilon   Phi    Gamma    Eta
	0x03B1, 0x03B2, 0x03C7, 0x03B4, 0x03B5, 0x03D5, 0x03B3, 0x03B7,
//   Iota   Phi (2) Kappa   Lambda    Mu      Nu    Omicron   Pi
	0x03B9, 0x03C6, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BF, 0x03C0,
//  Theta    Rho    Sigma    Tau    Upsilon OmegaPi Omega     Xi
	0x03B8, 0x03C1, 0x03C3, 0x03C4, 0x03C5, 0x03D6, 0x03B9, 0x03BE,
//   Psi     Zeta   arrow    Tau    Upsilon  ????   Omega     Xi
	0x03C8, 0x03B6, 0x21D4, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD,
//  lower case letters:
//  ----------------
//   Alpha   Beta    Chi    Delta   Sigma     Phi    Gamma    Eta
	0x03B1, 0x03B2, 0x03C7, 0x03B4, 0x2211, 0x03D5, 0x03B3, 0x03B7,
//   Iota   Phi (2) Kappa   Lambda    Mu      Nu    Omicron   Pi
	0x03B9, 0x03C6, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BF, 0x03C0,
//  Theta   Integr  Sigma    Tau    Upsilon OmegaPi Omega     Xi
	0x03B8, 0x222b, 0x03C3, 0x03C4, 0x03C5, 0x03D6, 0x03B9, 0x03BE,
//   Psi     Zeta   Sigma    Tau    Upsilon  ????   Omega     Xi
	0x03C8, 0x03B6, 0x03EA, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD
};

WMFImport::WMFImport( ScribusMainWindow* mw, int flags ) :
	QObject(mw)
{	
	m_tmpSel = new Selection(this, false);
	m_Doc    = mw->doc;
	unsupported = false;
	importFailed = false;
	importCanceled = true;
	importedColors.clear();
	m_docDesc = "";
	m_docTitle = "";
	interactive = (flags & LoadSavePlugin::lfInteractive);

	m_Valid        = false;
	m_ObjHandleTab = NULL;
	m_Dpi          = 1440;
}

WMFImport::~WMFImport()
{
	qDeleteAll(m_commands);
	m_commands.clear();
	if ( m_tmpSel) delete m_tmpSel;
	if ( m_ObjHandleTab ) 
	{
		for (int i = 0; i < MAX_OBJHANDLE; ++i)
		{
			if (m_ObjHandleTab[i])
				delete m_ObjHandleTab[i];
		}
		delete[] m_ObjHandleTab;
	}
}

QString WMFImport::importColor(const QColor& color)
{
	bool found = false;
	int r, g, b;
	QColor  tmpColor;
	QString retColorName;
	ColorList::Iterator it;
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&r, &g, &b);
			tmpColor.setRgb(r, g, b);
			if (color == tmpColor)
			{
				retColorName = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		ScColor tmp;
		tmp.fromQColor(color);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		m_Doc->PageColors.insert("FromWMF"+color.name(), tmp);
		importedColors.append("FromWMF"+color.name());
		retColorName = "FromWMF"+color.name();
	}
	return retColorName;
}

QColor WMFImport::colorFromParam( short* params )
{
	unsigned int colorRef;
	int red, green, blue;

	colorRef = toDWord( params ) & 0xffffff;
	red      = colorRef & 255;
	green    = ( colorRef>>8 ) & 255;
	blue     = ( colorRef>>16 ) & 255;

	return QColor( red, green, blue );
}

QTextCodec* WMFImport::codecFromCharset( int charset )
{
	QTextCodec* codec = NULL;
	if (charset == DEFAULT_CHARSET || charset == OEM_CHARSET)
		codec = QTextCodec::codecForLocale();
	else if (charset == ANSI_CHARSET)
		codec = QTextCodec::codecForName("windows-1252");
	/*else if (charset == SYMBOL_CHARSET)
		codec = QTextCodec::codecForLocale();// Not supported yet*/
	else if (charset == MAC_CHARSET)
		codec = QTextCodec::codecForName("Apple Roman");
	else if (charset == SHIFTJIS_CHARSET)
		codec = QTextCodec::codecForName("Shift-JIS");
	else if (charset == HANGEUL_CHARSET)
		codec = QTextCodec::codecForName("CP949");
	else if (charset == GB2312_CHARSET)
		codec = QTextCodec::codecForName("CP936");
	else if (charset == CHINESEBIG5_CHARSET)
		codec = QTextCodec::codecForName("Big5");
	else if (charset == GREEK_CHARSET)
		codec = QTextCodec::codecForName("windows-1253");
	else if (charset == TURKISH_CHARSET)
		codec = QTextCodec::codecForName("windows-1254");
	else if (charset == VIETNAMESE_CHARSET)
		codec = QTextCodec::codecForName("windows-1258");
	else if (charset == HEBREW_CHARSET)
		codec = QTextCodec::codecForName("windows-1255");
	else if (charset == ARABIC_CHARSET)
		codec = QTextCodec::codecForName("windows-1256");
	else if (charset == BALTIC_CHARSET)
		codec = QTextCodec::codecForName("windows-1257");
	else if (charset == RUSSIAN_CHARSET)
		codec = QTextCodec::codecForName("windows-1251");
	else if (charset == THAI_CHARSET)
		codec = QTextCodec::codecForName("CP874");
	else if (charset == EASTEUROPE_CHARSET)
		codec = QTextCodec::codecForName("windows-1250");
	return codec;
}

QString WMFImport::symbolToUnicode ( const QByteArray& chars )
{
	/*Code under GPL licence (http://root.cern.ch/viewvc/trunk/LICENSE)
	taken from http://root.cern.ch/viewvc/trunk/qt/src/TQtSymbolCodec.cxx*/
	QString r;
	const unsigned char * c = (const unsigned char *) chars.data();
	
	if( chars.size() == 0 ) return r;
	
	for (int i = 0; i < chars.size(); ++i) 
	{
		if ( 64 < c[i] && c[i] <= 64+32 )
			r.append( QChar(greek_symbol_to_unicode[c[i]-65]) );
		else if (64+32+1 <= c[i] && c[i] < 64+32+32+1)
			r.append( QChar(greek_symbol_to_unicode[c[i]-65-32]+32) );
		else if (161 <= c[i] )
			r.append(  QChar(symbol_to_unicode[c[i]-161]) );
		else
			r.append( QChar(c[i]) );
	}
	return r;
}

FPointArray WMFImport::pointsFromParam( short num, short* params )
{
	FPointArray points;
	points.resize( num );

	for ( int i = 0; i < num; i++, params += 2 )
		points.setPoint( i, params[ 0 ], params[ 1 ] );

	return points;
}

FPointArray WMFImport::pointsToPolyline( const FPointArray& points, bool closePath )
{
	bool bFirst = true;
	double x = 0.0, y = 0.0;
	FPointArray polyline;
	polyline.svgInit();
	for( uint i = 0; i < points.size(); ++i )
	{
		FPoint point = points.point(i);
		if( bFirst )
		{
			x = point.x();
			y = point.y();
			polyline.svgMoveTo(x, y);
			bFirst = false;
		}
		else
		{
			x = point.x();
			y = point.y();
			polyline.svgLineTo(x, y);
		}
	}
	if (closePath && (points.size() > 4))
		polyline.svgClosePath();
	return polyline;
}

void WMFImport::pointsToAngle( double xStart, double yStart, double xEnd, double yEnd, double& angleStart, double& angleLength )
{
	double aStart  = atan2( yStart,  xStart );
	double aLength = atan2( yEnd, xEnd ) - aStart;

	angleStart  = (int)(aStart * 180.0 / 3.14166);
	angleLength = (int)(aLength * 180.0 / 3.14166);
	if ( angleLength < 0 ) angleLength = 360.0 + angleLength;
}

bool WMFImport::import(QString fname, const TransactionSettings& trSettings, int flags)
{
	if (!loadWMF(fname))
	{
		importFailed = true;
		return false;
	}
	QString CurDirP = QDir::currentPath();
	QFileInfo efp(fname);
	QDir::setCurrent(efp.path());
	bool success = importWMF(trSettings, flags);
	QDir::setCurrent(CurDirP);
	return success;
}

bool WMFImport::loadWMF( const QString &fileName )
{
	QFile file( fileName );

	if ( !file.exists() )
	{
		cerr << "File " << QFile::encodeName(fileName).data() << " does not exist" << endl;
		return false;
	}

	if ( !file.open( QIODevice::ReadOnly ) )
	{
		cerr << "Cannot open file " << QFile::encodeName(fileName).data() << endl;
		return false;
	}

	QByteArray ba = file.readAll();
	file.close();

	QBuffer buffer( &ba );
	buffer.open( QIODevice::ReadOnly );
	return loadWMF( buffer );
}

bool WMFImport::loadWMF( QBuffer &buffer )
{
	QDataStream st;
	WmfEnhMetaHeader eheader;
	WmfMetaHeader header;
	WmfPlaceableHeader pheader;
	WORD16 checksum;
	int filePos, idx, i;
	WmfCmd *cmd;
	WORD32 rdSize;
	WORD16 rdFunc;

	header.mtSize = 0;
	header.mtHeaderSize = 0;
	header.mtNoParameters = 0;

	qDeleteAll(m_commands);
	m_commands.clear();

	st.setDevice( &buffer );
	st.setByteOrder( QDataStream::LittleEndian ); // Great, I love Qt !

	//----- Read placeable metafile header
	st >> pheader.key;
	m_IsPlaceable = (pheader.key == (WORD32) APMHEADER_KEY);
	if ( m_IsPlaceable )
	{
		st >> pheader.hmf;
		st >> pheader.bbox.left;
		st >> pheader.bbox.top;
		st >> pheader.bbox.right;
		st >> pheader.bbox.bottom;
		st >> pheader.inch;
		st >> pheader.reserved;
		st >> pheader.checksum;
		checksum = calcCheckSum( &pheader );
		if ( pheader.checksum!=checksum ) m_IsPlaceable = false;

		m_Dpi = pheader.inch;
		m_BBox.setLeft( pheader.bbox.left );
		m_BBox.setTop( pheader.bbox.top );
		m_BBox.setRight( pheader.bbox.right );
		m_BBox.setBottom( pheader.bbox.bottom );
		m_HeaderBoundingBox = m_BBox;
		if ( WMFIMPORT_DEBUG )
		{
			cerr << endl << "-------------------------------------------------" << endl;
			cerr << "WMF Placeable Header ( " << static_cast<int>(sizeof( pheader ) ) << "):" << endl;
			cerr << "  bbox=( " << m_BBox.left() << "; " << m_BBox.top() << "; " << m_BBox.width() << "; " << m_BBox.height() << ")" << endl;
			cerr << "  inch=" << pheader.inch << endl;
			cerr << "  checksum=" << pheader.checksum << "( " << (pheader.checksum==checksum?"ok":"wrong") << " )" << endl;
		}
	}
	else buffer.seek( 0 );

	//----- Read as enhanced metafile header
	filePos = buffer.pos();
	st >> eheader.iType;
	st >> eheader.nSize;
	st >> eheader.rclBounds.left;
	st >> eheader.rclBounds.top;
	st >> eheader.rclBounds.right;
	st >> eheader.rclBounds.bottom;
	st >> eheader.rclFrame.left;
	st >> eheader.rclFrame.top;
	st >> eheader.rclFrame.right;
	st >> eheader.rclFrame.bottom;
	st >> eheader.dSignature;
	m_IsEnhanced = ( eheader.dSignature==ENHMETA_SIGNATURE );
	if ( m_IsEnhanced ) // is it really enhanced ?
	{
		st >> eheader.nVersion;
		st >> eheader.nBytes;
		st >> eheader.nRecords;
		st >> eheader.nHandles;
		st >> eheader.sReserved;
		st >> eheader.nDescription;
		st >> eheader.offDescription;
		st >> eheader.nPalEntries;
		st >> eheader.szlDevice.width;
		st >> eheader.szlDevice.height;
		st >> eheader.szlMillimeters.width;
		st >> eheader.szlMillimeters.height;

		if ( WMFIMPORT_DEBUG )
		{
			cerr << endl << "-------------------------------------------------" << endl;
			cerr << "WMF Extended Header:" << endl;
			cerr << "  iType=" << eheader.iType << endl;
			cerr << "  nSize=" << eheader.nSize << endl;
			cerr << "  rclBounds=( " << eheader.rclBounds.left << "; " << eheader.rclBounds.top << "; "
					  << eheader.rclBounds.right << "; " << eheader.rclBounds.bottom << ")" << endl;
			cerr << "  rclFrame=( " << eheader.rclFrame.left << "; " << eheader.rclFrame.top << "; "
					  << eheader.rclFrame.right << "; " << eheader.rclFrame.bottom << ")" << endl;
			cerr << "  nBytes=" << eheader.nBytes << endl;
			cerr << "\nNOT YET IMPLEMENTED, SORRY." << endl;
		}
	}
	else // no, not enhanced
	{
		//----- Read as standard metafile header
		buffer.seek( filePos );
		st >> header.mtType;
		st >> header.mtHeaderSize;
		st >> header.mtVersion;
		st >> header.mtSize;
		st >> header.mtNoObjects;
		st >> header.mtMaxRecord;
		st >> header.mtNoParameters;
		if ( WMFIMPORT_DEBUG ) {
			cerr << "WMF Header: " <<  "mtSize=" << header.mtSize << endl;
		}
	}

	//----- Test header validity
	m_Valid = ((header.mtHeaderSize == 9) && (header.mtNoParameters == 0)) || m_IsEnhanced || m_IsPlaceable;
	if ( m_Valid && !m_IsEnhanced )
	{
		//----- Read Metafile Records
		rdFunc = -1;
		while ( !st.atEnd() && (rdFunc != 0) )
		{
			st >> rdSize;
			st >> rdFunc;
			idx = findFunc( rdFunc );
			rdSize -= 3;

			cmd = new WmfCmd;
			m_commands.append(cmd);

			cmd->funcIndex = idx;
			cmd->numParam = rdSize;
			cmd->params = new WORD16[ rdSize ];

			for ( i=0; i<rdSize && !st.atEnd(); i++ )
				st >> cmd->params[ i ];


			if ( rdFunc == 0x020B && !m_IsPlaceable) {         // SETWINDOWORG: dimensions
				m_BBox.setLeft( qMin((int) cmd->params[ 1 ], m_BBox.left()) );
				m_BBox.setTop ( qMin((int) cmd->params[ 0 ], m_BBox.top()) );
			}
			if ( rdFunc == 0x020C && !m_IsPlaceable ) {         // SETWINDOWEXT: dimensions
				m_BBox.setWidth ( qMax((int) cmd->params[ 1 ], m_BBox.width()) );
				m_BBox.setHeight( qMax((int) cmd->params[ 0 ], m_BBox.height()));
			}

			if ( i<rdSize )
			{
				cerr << "WMF : file truncated !" << endl;
				return false;
			}
		}
		//----- Test records validities
		m_Valid = (rdFunc == 0) && (m_BBox.width() != 0) && (m_BBox.height() != 0);
		if ( !m_Valid ) {
			cerr << "WMF : incorrect file format !" << endl;
		}
	}
	else if ( m_IsEnhanced ) {
		cerr << "WMF : unsupported Enhanced Metafile !" << endl;
	}
	else {
		cerr << "WMF Header : incorrect header !" << endl;
	}

	buffer.close();
	return (m_Valid && !m_IsEnhanced);
}

bool WMFImport::importWMF(const TransactionSettings& trSettings, int flags)
{
	bool ret = false;
	//double scale = (m_Dpi > 288) ? 288.0 / m_Dpi : 1.0;
	double scale  = (m_Dpi > 0) ? 72.0 / m_Dpi : 0.05;
	double width  = m_BBox.width() * scale;
	double height = m_BBox.height() * scale;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(fabs(width), fabs(height), 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0);
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(fabs(width), fabs(height), 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
		}
	}
	if ((ret) || (!interactive))
	{
		if (width > height)
			m_Doc->PageOri = 1;
		else
			m_Doc->PageOri = 0;
		m_Doc->m_pageSize = "Custom";
	}
	m_Doc->view()->Deselect();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	//gc->Family = m_Doc->toolSettings.defFont;
	m_Doc->PageColors.ensureDefaultColors();
	//m_gc.push( gc );
	QList<PageItem*> Elements = parseWmfCommands();
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->documentInfo.setTitle(m_docTitle);
		m_Doc->documentInfo.setComments(m_docDesc);
	}
	m_tmpSel->clear();
	if (Elements.count() == 0)
	{
		importFailed = true;
		if (importedColors.count() != 0)
		{
			for (int cd = 0; cd < importedColors.count(); cd++)
			{
				m_Doc->PageColors.remove(importedColors[cd]);
			}
		}
	}
	if (Elements.count() > 1)
	{
		bool isGroup = true;
		int firstElem = -1;
		if (Elements.at(0)->Groups.count() != 0)
			firstElem = Elements.at(0)->Groups.top();
		for (int bx = 0; bx < Elements.count(); ++bx)
		{
			PageItem* bxi = Elements.at(bx);
			if (bxi->Groups.count() != 0)
			{
				if (bxi->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		if (!isGroup)
		{
			double minx = 99999.9;
			double miny = 99999.9;
			double maxx = -99999.9;
			double maxy = -99999.9;
			uint lowestItem = 999999;
			uint highestItem = 0;
			for (int a = 0; a < Elements.count(); ++a)
			{
				Elements.at(a)->Groups.push(m_Doc->GroupCounter);
				PageItem* currItem = Elements.at(a);
				lowestItem = qMin(lowestItem, currItem->ItemNr);
				highestItem = qMax(highestItem, currItem->ItemNr);
				double x1, x2, y1, y2;
				currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
				minx = qMin(minx, x1);
				miny = qMin(miny, y1);
				maxx = qMax(maxx, x2);
				maxy = qMax(maxy, y2);
			}
			double gx = minx;
			double gy = miny;
			double gw = maxx - minx;
			double gh = maxy - miny;
			PageItem *high = m_Doc->Items->at(highestItem);
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
			PageItem *neu = m_Doc->Items->takeAt(z);
			m_Doc->Items->insert(lowestItem, neu);
			neu->Groups.push(m_Doc->GroupCounter);
			neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
			neu->isGroupControl = true;
			neu->groupsLastItem = high;
			for (int a = 0; a < m_Doc->Items->count(); ++a)
			{
				m_Doc->Items->at(a)->ItemNr = a;
			}
			neu->setRedrawBounding();
			neu->setTextFlowMode(PageItem::TextFlowDisabled);
			Elements.prepend(neu);
			m_Doc->GroupCounter++;
		}
	}
	m_Doc->DoDrawing = true;
	m_Doc->scMW()->setScriptRunning(false);
	if (interactive)
		m_Doc->setLoading(false);
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));
	if ((Elements.count() > 0) && (!ret) && (interactive))
	{
		if (flags & LoadSavePlugin::lfScripted)
		{
			bool loadF = m_Doc->isLoading();
			m_Doc->setLoading(false);
			m_Doc->changed();
			m_Doc->setLoading(loadF);
			m_Doc->m_Selection->delaySignalsOn();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				m_Doc->m_Selection->addItem(Elements.at(dre), true);
			}
			m_Doc->m_Selection->delaySignalsOff();
			m_Doc->m_Selection->setGroupRect();
			m_Doc->view()->updatesOn(true);
			importCanceled = false;
		}
		else
		{
			m_Doc->DragP = true;
			m_Doc->DraggedElem = 0;
			m_Doc->DragElements.clear();
			m_Doc->m_Selection->delaySignalsOn();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				m_Doc->DragElements.append(Elements.at(dre)->ItemNr);
				m_tmpSel->addItem(Elements.at(dre), true);
			}
			ScriXmlDoc *ss = new ScriXmlDoc();
			m_tmpSel->setGroupRect();
			ScElemMimeData* md = new ScElemMimeData();
			md->setScribusElem(ss->WriteElem(m_Doc, m_Doc->view(), m_tmpSel));
			delete ss;
/*#ifndef QT_MAC*/
// see #2526
			m_Doc->itemSelection_DeleteItem(m_tmpSel);
/*#endif*/
			m_Doc->view()->updatesOn(true);
			m_Doc->m_Selection->delaySignalsOff();
			// We must copy the TransationSettings object as it is owned
			// by handleObjectImport method afterwards
			TransactionSettings* transacSettings = new TransactionSettings(trSettings);
			m_Doc->view()->handleObjectImport(md, transacSettings);
			m_Doc->DragP = false;
			m_Doc->DraggedElem = 0;
			m_Doc->DragElements.clear();
		}
	}
	else
	{
		bool loadF = m_Doc->isLoading();
		m_Doc->setLoading(false);
		m_Doc->changed();
		m_Doc->reformPages();
		m_Doc->view()->updatesOn(true);
		m_Doc->setLoading(loadF);
	}
	qApp->restoreOverrideCursor();
	return true;
}

QList<PageItem*> WMFImport::parseWmfCommands(void)
{
	int idx, i;
	const WmfCmd* cmd;
	QList<PageItem*> elements;

	m_context.reset();

	if ( !m_Valid )  return elements;

	if ( m_ObjHandleTab ) delete[] m_ObjHandleTab;
	m_ObjHandleTab = new WmfObjHandle* [ MAX_OBJHANDLE ];
	for ( i = MAX_OBJHANDLE-1; i >= 0; i-- )
		m_ObjHandleTab[ i ] = NULL;

	if ( WMFIMPORT_DEBUG )  {
		cerr << "Bounding box : " << m_BBox.left() << " " << m_BBox.top() << " " << m_BBox.right() << " " << m_BBox.bottom() << endl;
	}

	double scale  = (m_Dpi > 0) ? 72.0 / m_Dpi : 0.05;
	m_context.setViewportOrg( 0, 0 );
	m_context.setViewportExt( m_BBox.width() * scale, m_BBox.height() * scale );
	m_context.setWindowOrg( m_BBox.left(), m_BBox.bottom() );
	m_context.setWindowExt( m_BBox.width(), m_BBox.height() );

	for (int index = 0; index < m_commands.count(); ++index)
	{
		cmd = m_commands.at(index);
		idx = cmd->funcIndex;
		( this->*metaFuncTab[ idx ].method )( elements, cmd->numParam, cmd->params );

		if ( WMFIMPORT_DEBUG )  {
			QString str = "", param;
			if ( metaFuncTab[ idx ].name == NULL ) {
				str += "UNKNOWN ";
			}
			if ( metaFuncTab[ idx ].method == &WMFImport::noop ) {
				str += "UNIMPLEMENTED ";
			}
			str += metaFuncTab[ idx ].name;
			str += " : ";

			for ( i=0 ; i < cmd->numParam ; i++ ) {
				param.setNum( cmd->params[ i ] );
				str += param;
				str += " ";
			}
			cerr << str.toAscii().data() << endl;
		}
	}
	return elements;
}

void WMFImport::finishCmdParsing( PageItem* item )
{
	QMatrix gcm  = m_context.worldMatrix();
	double coeff1 = sqrt(gcm.m11() * gcm.m11() + gcm.m12() * gcm.m12());
	double coeff2 = sqrt(gcm.m21() * gcm.m21() + gcm.m22() * gcm.m22());
	if( item->asImageFrame() )
	{
		item->moveBy(gcm.dx(), gcm.dy());
		item->setWidthHeight(item->width() * gcm.m11(), item->height() * gcm.m22());
		item->setLineWidth(qMax(item->lineWidth() * (coeff1 + coeff2) / 2.0, 0.25));
		if (item->PictureIsAvailable)
			item->setImageXYScale(item->width() / item->pixm.width(), item->height() / item->pixm.height());
	}
	else if( item->asTextFrame() )
	{
		item->setLineWidth(qMax(item->lineWidth() * (coeff1 + coeff2) / 2.0, 0.25));
	}
	else
	{
		item->ClipEdited = true;
		item->FrameType = 3;
		item->PoLine.map(gcm);
		item->setLineWidth(qMax(item->lineWidth() * (coeff1 + coeff2) / 2.0, 0.25));
		FPoint wh = getMaxClipF(&item->PoLine);
		item->setWidthHeight(wh.x(), wh.y());
		m_Doc->AdjustItemSize(item);
	}
	item->setRedrawBounding();
	item->OwnPage   = m_Doc->OnPage(item);
	item->PLineEnd  = m_context.pen().capStyle();
	item->PLineJoin = m_context.pen().joinStyle();
	item->setTextFlowMode(PageItem::TextFlowDisabled);
	//item->DashOffset = gc->dashOffset;
	//item->DashValues = gc->dashArray;
}

void WMFImport::setWindowOrg( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setWindowOrg( params[ 1 ], params[ 0 ] );
}

void WMFImport::setWindowExt( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setWindowExt( params[ 1 ], params[ 0 ] );
}

void WMFImport::lineTo( QList<PageItem*>& items, long, short* params )
{
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	QPoint currentPos = m_context.position();
	bool   doStroke = (m_context.pen().style() != Qt::NoPen);
	if (doStroke)
	{
		double x1 = currentPos.x();
		double y1 = currentPos.y();
		double x2 = params[1];
		double y2 = params[0];
		double  lineWidth = m_context.pen().width();
		QString lineColor = importColor(m_context.pen().color());
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, CommonStrings::None, lineColor, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine.resize(4);
		ite->PoLine.setPoint(0, FPoint(x1, y1));
		ite->PoLine.setPoint(1, FPoint(x1, y1));
		ite->PoLine.setPoint(2, FPoint(x2, y2));
		ite->PoLine.setPoint(3, FPoint(x2, y2));
		finishCmdParsing(ite);
		items.append(ite);
	}
	m_context.setPosition( QPoint(params[1], params[0]) );
}

void WMFImport::moveTo( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setPosition( QPoint(params[1], params[0]) );
}

void WMFImport::ellipse( QList<PageItem*>& items, long, short* params )
{
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double rx = (params[ 1 ] - params[ 3 ]) / 2.0;
	double ry = (params[ 0 ] - params[ 2 ]) / 2.0;
	double px = (params[ 1 ] + params[ 3 ]) / 2.0 - rx;
	double py = (params[ 0 ] + params[ 2 ]) / 2.0 - ry;
	bool   doFill = m_context.brush().style() != Qt::NoBrush;
	bool   doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = doFill ? importColor( m_context.brush().color() ) : CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, rx * 2.0, ry * 2.0, lineWidth, fillColor, strokeColor, true);
	PageItem* ite = m_Doc->Items->at(z);
	QMatrix mm(1.0, 0.0, 0.0, 1.0, px, py);
	ite->PoLine.map(mm);
	finishCmdParsing(ite);
	items.append(ite);
}

void WMFImport::polygon( QList<PageItem*>& items, long, short* params )
{
	//cerr << "WMFImport::polygon unimplemented" << endl;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();
	bool    doFill = m_context.brush().style() != Qt::NoBrush;
	bool    doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = doFill ? importColor( m_context.brush().color() ) : CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	if (doStroke && lineWidth <= 0.0 )
		lineWidth = 1.0;
	FPointArray paramPoints = pointsFromParam( params[0], &params[1] );
	FPointArray points      = pointsToPolyline( paramPoints, true );
	if( paramPoints.size() > 0 )
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, fillColor, strokeColor, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = points;
		ite->fillRule = !m_context.windingFill();
		finishCmdParsing(ite);
		items.append(ite);
	}
}

void WMFImport::polyPolygon( QList<PageItem*>& items, long num, short* params )
{
	int numPolys   = params[0];
	int pointIndex = params[0] + 1;
	for (int i = 0; i < numPolys; ++i)
	{
		short  numPoints  = params[i + 1];
		short* paramArray = new short[1 + 2 * numPoints];
		paramArray[0] = numPoints;
		memcpy(&paramArray[1], &params[pointIndex], 2 * numPoints * sizeof(short));
		polygon(items, num, paramArray);
		delete[] paramArray;
		pointIndex += (2 * numPoints);
	}
}

void WMFImport::polyline( QList<PageItem*>& items, long, short* params )
{
	//cerr << "WMFImport::polyline unimplemented" << endl;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();
	bool    doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	if (doStroke && lineWidth <= 0.0 )
		lineWidth = 1.0;
	FPointArray paramPoints = pointsFromParam( params[0], &params[1] );
	FPointArray points      = pointsToPolyline( paramPoints, false );
	if( paramPoints.size() > 0 )
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, fillColor, strokeColor, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = points;
		finishCmdParsing(ite);
		items.append(ite);
	}
}

void WMFImport::rectangle( QList<PageItem*>& items, long, short* params )
{
	//cerr << "WMFImport::rectangle unimplemented" << endl;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();
	bool    doFill = m_context.brush().style() != Qt::NoBrush;
	bool    doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = doFill ? importColor( m_context.brush().color() ) : CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	if (doStroke && lineWidth <= 0.0 )
		lineWidth = 1.0;
	double x = ((params[3] - params[1]) > 0) ? params[1] : params[3];
	double y = ((params[2] - params[0]) > 0) ? params[0] : params[2];
	double width  = fabs((double) params[3] - params[1]);
	double height = fabs((double) params[2] - params[0]);
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, width, height, lineWidth, fillColor, strokeColor, true);
	PageItem* ite = m_Doc->Items->at(z);
	QMatrix mm(1.0, 0.0, 0.0, 1.0, x, y);
	ite->PoLine.map(mm);
	finishCmdParsing(ite);
	items.append(ite);
}

void WMFImport::roundRect( QList<PageItem*>& items, long, short* params )
{
	//cerr << "WMFImport::roundRect unimplemented" << endl;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();
	bool    doFill = m_context.brush().style() != Qt::NoBrush;
	bool    doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = doFill ? importColor( m_context.brush().color() ) : CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	if (doStroke && lineWidth <= 0.0 )
		lineWidth = 1.0;
	double x = ((params[5] - params[3]) > 0) ? params[3] : params[5];
	double y = ((params[4] - params[2]) > 0) ? params[2] : params[4];
	double width  = fabs((double) params[5] - params[3]);
	double height = fabs((double) params[4] - params[2]);
	double rx     = params[1] / 2.0;
	double ry     = params[0] / 2.0;
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, width, height, lineWidth, fillColor, strokeColor, true);
	PageItem* ite = m_Doc->Items->at(z);
	if ((rx != 0.0) || (ry != 0.0))
	{
		ite->setCornerRadius(qMax(rx, ry));
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
	QMatrix mm(1.0, 0.0, 0.0, 1.0, x, y);
	ite->PoLine.map(mm);
	finishCmdParsing(ite);
	items.append(ite);
}

void WMFImport::arc( QList<PageItem*>& items, long, short* params )
{
	FPointArray  pointArray;
	QPainterPath painterPath;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();
	bool    doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	if (doStroke && lineWidth <= 0.0 )
		lineWidth = 1.0;
	double  angleStart, angleLength;
	double  x = ((params[7] - params[5]) > 0) ? params[5] : params[7];
	double  y = ((params[6] - params[4]) > 0) ? params[4] : params[6];
	double  xCenter = (params[5] + params[7]) / 2.0;
	double  yCenter = (params[4] + params[6]) / 2.0;
	double  xWidth  =  fabs((double) params[5] - params[7]);
	double  yHeight =  fabs((double) params[4] - params[6]);
	pointsToAngle(params[3] - xCenter, yCenter - params[2], params[1] - xCenter, yCenter - params[0], angleStart, angleLength);
	painterPath.arcMoveTo(x, y, xWidth, yHeight, angleStart);
	painterPath.arcTo(x, y, xWidth, yHeight, angleStart, angleLength);
	pointArray.fromQPainterPath(painterPath);
	if( pointArray.size() > 0 )
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, fillColor, strokeColor, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointArray;
		finishCmdParsing(ite);
		items.append(ite);
	}
}

void WMFImport::chord( QList<PageItem*>& items, long, short* params )
{
	QPointF      firstPoint;
	FPointArray  pointArray;
	QPainterPath painterPath;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();
	bool    doFill   = m_context.brush().style() != Qt::NoBrush;
	bool    doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = doFill ? importColor( m_context.brush().color() ) : CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	if (doStroke && lineWidth <= 0.0 )
		lineWidth = 1.0;
	double  angleStart, angleLength;
	double  x = ((params[7] - params[5]) > 0) ? params[5] : params[7];
	double  y = ((params[6] - params[4]) > 0) ? params[4] : params[6];
	double  xCenter = (params[5] + params[7]) / 2.0;
	double  yCenter = (params[4] + params[6]) / 2.0;
	double  xWidth  =  fabs((double) params[5] - params[7]);
	double  yHeight =  fabs((double) params[4] - params[6]);
	pointsToAngle(params[3] - xCenter, yCenter - params[2], params[1] - xCenter, yCenter - params[0], angleStart, angleLength);
	painterPath.arcMoveTo(x, y, xWidth, yHeight, angleStart);
	firstPoint = painterPath.currentPosition();
	painterPath.arcTo (x, y, xWidth, yHeight, angleStart, angleLength);
	painterPath.lineTo(firstPoint);
	pointArray.fromQPainterPath(painterPath);
	if( pointArray.size() > 0 )
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, fillColor, strokeColor, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointArray;
		finishCmdParsing(ite);
		items.append(ite);
	}
}

void WMFImport::pie( QList<PageItem*>& items, long, short* params )
{
	QPointF      firstPoint;
	FPointArray  pointArray;
	QPainterPath painterPath;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();
	bool    doFill   = m_context.brush().style() != Qt::NoBrush;
	bool    doStroke = m_context.pen().style() != Qt::NoPen;
	QString fillColor   = doFill ? importColor( m_context.brush().color() ) : CommonStrings::None;
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : CommonStrings::None;
	double  lineWidth   = m_context.pen().width();
	if (doStroke && lineWidth <= 0.0 )
		lineWidth = 1.0;
	double  angleStart, angleLength;
	double  x = ((params[7] - params[5]) > 0) ? params[5] : params[7];
	double  y = ((params[6] - params[4]) > 0) ? params[4] : params[6];
	double  xCenter = (params[5] + params[7]) / 2.0;
	double  yCenter = (params[4] + params[6]) / 2.0;
	double  xWidth  =  fabs((double) params[5] - params[7]);
	double  yHeight =  fabs((double) params[4] - params[6]);
	pointsToAngle(params[3] - xCenter, yCenter - params[2], params[1] - xCenter, yCenter - params[0], angleStart, angleLength);
	painterPath.arcMoveTo(x,y, xWidth, yHeight, angleStart);
	firstPoint = painterPath.currentPosition();
	painterPath.arcTo (x, y, xWidth, yHeight, angleStart, angleLength);
	painterPath.lineTo(xCenter, yCenter);
	painterPath.lineTo(firstPoint);
	pointArray.fromQPainterPath(painterPath);
	if( pointArray.size() > 0 )
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, fillColor, strokeColor, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointArray;
		finishCmdParsing(ite);
		items.append(ite);
	}
}

void WMFImport::setPolyFillMode( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setWindingFill( params[0] );
}

void WMFImport::setBkColor( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setBackgroundColor( colorFromParam( params ) );
}

void WMFImport::setBkMode( QList<PageItem*>& /*items*/, long, short* params )
{
	if ( params[ 0 ]==1 ) 
		m_context.setBackgroundMode( Qt::TransparentMode );
	else 
		m_context.setBackgroundMode( Qt::OpaqueMode );
}

void WMFImport::saveDC( QList<PageItem*>& /*items*/, long, short* )
{
	m_context.save();
}

void WMFImport::restoreDC( QList<PageItem*>& /*items*/, long, short *params )
{
	for ( int i=0; i > params[ 0 ] ; i-- )
		m_context.restore();
}

void WMFImport::intersectClipRect( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	cerr << "WMFImport::intersectClipRect unimplemented" << endl;
}

void WMFImport::excludeClipRect( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	cerr << "WMFImport::excludeClipRect unimplemented" << endl;
}

void WMFImport::setTextColor( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setTextColor( colorFromParam(params) );
}

void WMFImport::setTextAlign( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setTextAlign(params[ 0 ]);
}

void WMFImport::textOut( QList<PageItem*>& items, long num, short* params )
{
	short *copyParm = new short[num + 1];

	// re-order parameters
	int idxOffset = (params[0] / 2) + 1 + (params[0] & 1);
	copyParm[0] = params[idxOffset];
	copyParm[1] = params[idxOffset + 1];
	copyParm[2] = params[0];
	copyParm[3] = 0;
	memcpy( &copyParm[4], &params[1], params[0] );

	extTextOut(items, num + 1, copyParm );
	delete [] copyParm;
}

void WMFImport::extTextOut( QList<PageItem*>& items, long num, short* params )
{
	QString    textString;
	double  BaseX = m_Doc->currentPage()->xOffset();
	double  BaseY = m_Doc->currentPage()->yOffset();

	if (params[3] & 0x0010) // ETO_GLYPH_INDEX
	{
		cerr << "WMFImport::extTextOut: ETO_GLYPH_INDEX option not supported " << endl;
		unsupported = true;
		return;
	}

	// ETO_CLIPPED flag add 4 parameters
	char* ptStr = (params[3] & 0x0004) ? ((char*)&params[8]) : ((char*)&params[4]);
	QByteArray textArray( ptStr, params[2] );
	
	QTextCodec* codec = codecFromCharset( m_context.textCharSet() );
	if (codec)
		textString = codec->toUnicode(textArray);
	else if ( m_context.textCharSet() == SYMBOL_CHARSET )
		textString = symbolToUnicode(textArray);
	else
		textString = QString::fromLocal8Bit(textArray.data());

	QFontMetrics fm( m_context.font() );
	int width  = fm.width(textString) + fm.descent();  // because fm.width(text) isn't rigth with Italic text
	/*int height = fm.height();
	int ascent = fm.ascent();*/

	double startX = params[1], startY = params[0];
	int    textAlign    = m_context.textAlign();
	double textRotation = m_context.textRotation();
	if ( textAlign & 0x01 ) {       // (left, top) position = current logical position
		QPoint pos = m_context.position();
		startX = pos.x();
		startY = pos.y();
	}

	m_context.save();
	// A bit hacky but should be sufficient to have text not flipped
	// top down in some wmfs (see cell.wmf sample from libwmf)
	if ( m_context.worldMatrix().m22() < 0.0)
	{
		m_context.translate(startX, startY);
		m_context.scale (1.0, -1.0);
		m_context.translate( -startX, -startY);
	}
	if ( textRotation != 0.0) {
		m_context.translate(startX, startY);
		m_context.rotate ( textRotation );
		m_context.translate( -startX, -startY);
	}

	if ( (textAlign & 0x04) && (textAlign & 0x02) ) // TA_CENTER 0x06
		startX -= (width / 2);
	else if ( textAlign & 0x02 ) // TA_RIGHT 0x02
		startX -= width;
	if ( textAlign == 0 ) // TA_TOP                       
		startY += fm.ascent();

	bool eto_empty = (params[3] == 0);
	bool eto_clipped_set = (params[3] & 0x0004);
	bool eto_pdy_set     = (params[3] & 0x2000);

	int  idxOffset = (params[ 2 ] / 2) + 4 + (params[ 2 ] & 1) + (eto_clipped_set ? 4 : 0);
	int  minParams = eto_pdy_set ? (idxOffset + 2 * (params[2] - 1)) : (idxOffset + params[2]);
	bool useCharInterdistances = (num >= minParams) && (eto_empty || eto_pdy_set);
	if ((params[2] > 1) && useCharInterdistances) 
	{
		double xpos  = startX;
		double ypos  = startY;
		double lineWidth = 0.0;
		FPointArray textPath;
		QString textColor = importColor( m_context.textColor() );
		for (int index = 0; (index < params[2] && index < textString.length()); ++index) 
		{
			QPainterPath painterPath;
			if (index > 0 && eto_pdy_set)
			{
				xpos += params[idxOffset + index * 2 - 1];
				ypos += params[idxOffset + index * 2];
			}
			else if (index > 0)
				xpos += params[idxOffset + index - 1];
			painterPath.addText(xpos, ypos, m_context.font(), textString.at(index));
			textPath.fromQPainterPath(painterPath);
			if (textPath.size() > 0)
			{
				int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, textColor, CommonStrings::None, true);
				PageItem* ite = m_Doc->Items->at(z);
				ite->PoLine = textPath;
				finishCmdParsing(ite);
				items.append(ite);
			}
		}
	}
	else 
	{
		FPointArray textPath;
		QString textColor = importColor( m_context.textColor() );
		QPainterPath painterPath;
		painterPath.addText( startX, startY, m_context.font(), textString );
		textPath.fromQPainterPath(painterPath);
		if (textPath.size() > 0)
		{
			double  lineWidth = 0.0;
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, textColor, CommonStrings::None, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = textPath;
			finishCmdParsing(ite);
			items.append(ite);
		}
	}
	m_context.restore();
}

void WMFImport::selectObject( QList<PageItem*>& /*items*/, long, short* params )
{
	int idx = params[ 0 ];
	if ( idx >= 0 && idx < MAX_OBJHANDLE && m_ObjHandleTab[ idx ] )
		m_ObjHandleTab[ idx ]->apply( m_context );
}

void WMFImport::deleteObject( QList<PageItem*>& /*items*/, long, short* params )
{
	deleteHandle( params[ 0 ] );
}

void WMFImport::createEmptyObject( QList<PageItem*>& /*items*/, long, short* )
{
	// allocation of an empty object (to keep object counting in sync)
	WmfObjPenHandle* handle = new WmfObjPenHandle();
	addHandle( handle );
	cerr << "WMFImport: unimplemented createObject " << endl;
}

void WMFImport::createBrushIndirect( QList<PageItem*>& /*items*/, long, short* params )
{
	static Qt::BrushStyle hatchedStyleTab[] =
	{
		Qt::HorPattern,
		Qt::FDiagPattern,
		Qt::BDiagPattern,
		Qt::CrossPattern,
		Qt::DiagCrossPattern
	};
	static Qt::BrushStyle styleTab[] =
	{ Qt::SolidPattern,
	  Qt::NoBrush,
	  Qt::FDiagPattern,   /* hatched */
	  Qt::Dense4Pattern,  /* should be custom bitmap pattern */
	  Qt::HorPattern,     /* should be BS_INDEXED (?) */
	  Qt::VerPattern,     /* should be device-independent bitmap */
	  Qt::Dense6Pattern,  /* should be device-independent packed-bitmap */
	  Qt::Dense2Pattern,  /* should be BS_PATTERN8x8 */
	  Qt::Dense3Pattern   /* should be device-independent BS_DIBPATTERN8x8 */
	};
	Qt::BrushStyle style;
	short arg;
	WmfObjBrushHandle* handle = new WmfObjBrushHandle();
	addHandle( handle );

	arg = params[0];
	if ( arg == 2 )
	{
		arg = params[3];
		if ( arg >= 0 && arg < 5 ) 
			style = hatchedStyleTab[ arg ];
		else
		{
			cerr << "WMFImport::createBrushIndirect: invalid hatched brush " << arg << endl;
			style = Qt::SolidPattern;
		}
	}
	else if ( arg >= 0 && arg < 9 )
		style = styleTab[arg];
	else
	{
		cerr << "WMFImport::createBrushIndirect: invalid brush " << arg << endl;
		style = Qt::SolidPattern;
	}
	handle->brush.setStyle( style );
	handle->brush.setColor( colorFromParam( params+1 ) );
}

void WMFImport::createPenIndirect( QList<PageItem*>& /*items*/, long, short* params )
{
	static Qt::PenStyle styleTab[] = { Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine,
	  Qt::NoPen, Qt::SolidLine };
	Qt::PenStyle style;
	WmfObjPenHandle* handle = new WmfObjPenHandle();
	addHandle( handle );

	if ( params[0] >= 0 && params[0] < 6 ) 
		style = styleTab[ params[0] ];
	else
	{
		cerr << "WMFImport::createPenIndirect: invalid pen " << params[ 0 ] << endl;
		style = Qt::SolidLine;
	}

	handle->pen.setStyle( style );
	handle->pen.setWidth( params[1] );
	handle->pen.setColor( colorFromParam( params+3 ) );
	handle->pen.setCapStyle( Qt::RoundCap );
}

void WMFImport::createFontIndirect( QList<PageItem*>& /*items*/, long , short* params)
{
	WmfObjFontHandle* handle = new WmfObjFontHandle();
	addHandle( handle );

	QString family( (const char*)&params[ 9 ] );

	handle->rotation = -params[ 2 ]  / 10; // text rotation (in 1/10 degree)
	handle->font.setFamily( family );
	handle->font.setStyleStrategy( QFont::PreferOutline );
	handle->font.setFixedPitch( ((params[ 8 ] & 0x01) == 0) );
	// TODO: investigation why some test case need -2. (size of font in logical point)
	int fontSize = (params[0] != 0) ? (qAbs(params[0]) - 2) : 12;
	handle->font.setPixelSize( fontSize );
	handle->font.setWeight( (params[ 4 ] >> 3) );
	handle->font.setItalic( (params[ 5 ] & 0x01) );
	handle->font.setUnderline( (params[ 5 ] & 0x100) );
	handle->font.setStrikeOut( (params[ 6 ] & 0x01) );
	handle->charset = (params[ 6 ] & 0xFF00) >> 8;
}

void WMFImport::noop( QList<PageItem*>& /*items*/, long, short* )
{
}

void WMFImport::end( QList<PageItem*>& /*items*/, long, short* )
{
	//cerr << "WMFImport::end unimplemented" << endl;
}

void WMFImport::addHandle( WmfObjHandle* handle )
{
	int idx;

	for ( idx =0; idx < MAX_OBJHANDLE ; idx++ )
		if ( m_ObjHandleTab[ idx ] == NULL )  break;

	if ( idx < MAX_OBJHANDLE )
		m_ObjHandleTab[ idx ] = handle;
	else
		cerr << "WMFImport error: handle table full !" << endl;
}

void WMFImport::deleteHandle( int idx )
{
	if ( idx >= 0 && idx < MAX_OBJHANDLE && m_ObjHandleTab[ idx ] )
	{
		delete m_ObjHandleTab[ idx ];
		m_ObjHandleTab[ idx ] = NULL;
	}
}

unsigned short WMFImport::calcCheckSum( WmfPlaceableHeader* apmfh )
{
	WORD16*  lpWord;
	WORD16   wResult, i;

	// Start with the first word
	wResult = *( lpWord = ( WORD16* )( apmfh ) );
	// XOR in each of the other 9 words
	for( i=1; i<=9; i++ )
	{
		wResult ^= lpWord[ i ];
	}
	return wResult;
}

int WMFImport::findFunc( unsigned short aFunc ) const
{
	int i = 0;

	for ( i=0; metaFuncTab[ i ].name; i++ )
		if ( metaFuncTab[ i ].func == aFunc ) return i;

	// here : unknown function
	return i;
}

unsigned int WMFImport::toDWord( short* params )
{
	unsigned int l;
#if !defined( WORDS_BIGENDIAN )
	l = *( unsigned int* )( params );
#else
	char *bytes;
	char swap[ 4 ];
	bytes = ( char* )params;
	swap[ 0 ] = bytes[ 2 ];
	swap[ 1 ] = bytes[ 3 ];
	swap[ 2 ] = bytes[ 0 ];
	swap[ 3 ] = bytes[ 1 ];
	l = *( unsigned int* )( swap );
#endif
	return l;
}
