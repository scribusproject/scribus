/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Jan 11 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#include <QByteArray>
#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QRawFont>
#include <QRegExp>
#include <QTextCodec>
#include <QUuid>
#include <QDebug>

#include <cstdlib>

#include "ui/customfdialog.h"
#include "importemf.h"
#include "loadsaveplugin.h"
#include "fileloader.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "pageitem_imageframe.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "ui/propertiespalette.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"

#define U_EMR_HEADER                   1
#define U_EMR_POLYBEZIER               2
#define U_EMR_POLYGON                  3
#define U_EMR_POLYLINE                 4
#define U_EMR_POLYBEZIERTO             5
#define U_EMR_POLYLINETO               6
#define U_EMR_POLYPOLYLINE             7
#define U_EMR_POLYPOLYGON              8
#define U_EMR_SETWINDOWEXTEX           9
#define U_EMR_SETWINDOWORGEX          10
#define U_EMR_SETVIEWPORTEXTEX        11
#define U_EMR_SETVIEWPORTORGEX        12
#define U_EMR_SETBRUSHORGEX           13
#define U_EMR_EOF                     14
#define U_EMR_SETPIXELV               15
#define U_EMR_SETMAPPERFLAGS          16
#define U_EMR_SETMAPMODE              17
#define U_EMR_SETBKMODE               18
#define U_EMR_SETPOLYFILLMODE         19
#define U_EMR_SETROP2                 20
#define U_EMR_SETSTRETCHBLTMODE       21
#define U_EMR_SETTEXTALIGN            22
#define U_EMR_SETCOLORADJUSTMENT      23
#define U_EMR_SETTEXTCOLOR            24
#define U_EMR_SETBKCOLOR              25
#define U_EMR_OFFSETCLIPRGN           26
#define U_EMR_MOVETOEX                27
#define U_EMR_SETMETARGN              28
#define U_EMR_EXCLUDECLIPRECT         29
#define U_EMR_INTERSECTCLIPRECT       30
#define U_EMR_SCALEVIEWPORTEXTEX      31
#define U_EMR_SCALEWINDOWEXTEX        32
#define U_EMR_SAVEDC                  33
#define U_EMR_RESTOREDC               34
#define U_EMR_SETWORLDTRANSFORM       35
#define U_EMR_MODIFYWORLDTRANSFORM    36
#define U_EMR_SELECTOBJECT            37
#define U_EMR_CREATEPEN               38
#define U_EMR_CREATEBRUSHINDIRECT     39
#define U_EMR_DELETEOBJECT            40
#define U_EMR_ANGLEARC                41
#define U_EMR_ELLIPSE                 42
#define U_EMR_RECTANGLE               43
#define U_EMR_ROUNDRECT               44
#define U_EMR_ARC                     45
#define U_EMR_CHORD                   46
#define U_EMR_PIE                     47
#define U_EMR_SELECTPALETTE           48
#define U_EMR_CREATEPALETTE           49
#define U_EMR_SETPALETTEENTRIES       50
#define U_EMR_RESIZEPALETTE           51
#define U_EMR_REALIZEPALETTE          52
#define U_EMR_EXTFLOODFILL            53
#define U_EMR_LINETO                  54
#define U_EMR_ARCTO                   55
#define U_EMR_POLYDRAW                56
#define U_EMR_SETARCDIRECTION         57
#define U_EMR_SETMITERLIMIT           58
#define U_EMR_BEGINPATH               59
#define U_EMR_ENDPATH                 60
#define U_EMR_CLOSEFIGURE             61
#define U_EMR_FILLPATH                62
#define U_EMR_STROKEANDFILLPATH       63
#define U_EMR_STROKEPATH              64
#define U_EMR_FLATTENPATH             65
#define U_EMR_WIDENPATH               66
#define U_EMR_SELECTCLIPPATH          67
#define U_EMR_ABORTPATH               68
#define U_EMR_UNDEF69                 69
#define U_EMR_COMMENT                 70
#define U_EMR_FILLRGN                 71
#define U_EMR_FRAMERGN                72
#define U_EMR_INVERTRGN               73
#define U_EMR_PAINTRGN                74
#define U_EMR_EXTSELECTCLIPRGN        75
#define U_EMR_BITBLT                  76
#define U_EMR_STRETCHBLT              77
#define U_EMR_MASKBLT                 78
#define U_EMR_PLGBLT                  79
#define U_EMR_SETDIBITSTODEVICE       80
#define U_EMR_STRETCHDIBITS           81
#define U_EMR_EXTCREATEFONTINDIRECTW  82
#define U_EMR_EXTTEXTOUTA             83
#define U_EMR_EXTTEXTOUTW             84
#define U_EMR_POLYBEZIER16            85
#define U_EMR_POLYGON16               86
#define U_EMR_POLYLINE16              87
#define U_EMR_POLYBEZIERTO16          88
#define U_EMR_POLYLINETO16            89
#define U_EMR_POLYPOLYLINE16          90
#define U_EMR_POLYPOLYGON16           91
#define U_EMR_POLYDRAW16              92
#define U_EMR_CREATEMONOBRUSH         93
#define U_EMR_CREATEDIBPATTERNBRUSHPT 94
#define U_EMR_EXTCREATEPEN            95
#define U_EMR_POLYTEXTOUTA            96
#define U_EMR_POLYTEXTOUTW            97
#define U_EMR_SETICMMODE              98
#define U_EMR_CREATECOLORSPACE        99
#define U_EMR_SETCOLORSPACE          100
#define U_EMR_DELETECOLORSPACE       101
#define U_EMR_GLSRECORD              102
#define U_EMR_GLSBOUNDEDRECORD       103
#define U_EMR_PIXELFORMAT            104
#define U_EMR_DRAWESCAPE             105
#define U_EMR_EXTESCAPE              106
#define U_EMR_UNDEF107               107
#define U_EMR_SMALLTEXTOUT           108
#define U_EMR_FORCEUFIMAPPING        109
#define U_EMR_NAMEDESCAPE            110
#define U_EMR_COLORCORRECTPALETTE    111
#define U_EMR_SETICMPROFILEA         112
#define U_EMR_SETICMPROFILEW         113
#define U_EMR_ALPHABLEND             114
#define U_EMR_SETLAYOUT              115
#define U_EMR_TRANSPARENTBLT         116
#define U_EMR_UNDEF117               117
#define U_EMR_GRADIENTFILL           118
#define U_EMR_SETLINKEDUFIS          119
#define U_EMR_SETTEXTJUSTIFICATION   120
#define U_EMR_COLORMATCHTOTARGETW    121
#define U_EMR_CREATECOLORSPACEW      122

#define U_STOCK_OBJECT          0x80000000
#define U_WHITE_BRUSH           0x80000000
#define U_LTGRAY_BRUSH          0x80000001
#define U_GRAY_BRUSH            0x80000002
#define U_DKGRAY_BRUSH          0x80000003
#define U_BLACK_BRUSH           0x80000004
#define U_NULL_BRUSH            0x80000005
#define U_WHITE_PEN             0x80000006
#define U_BLACK_PEN             0x80000007
#define U_NULL_PEN              0x80000008
#define U_OEM_FIXED_FONT        0x8000000A
#define U_ANSI_FIXED_FONT       0x8000000B
#define U_ANSI_VAR_FONT         0x8000000C
#define U_SYSTEM_FONT           0x8000000D
#define U_DEVICE_DEFAULT_FONT   0x8000000E
#define U_DEFAULT_PALETTE       0x8000000F
#define U_SYSTEM_FIXED_FONT     0x80000010
#define U_DEFAULT_GUI_FONT      0x80000011
#define U_DC_BRUSH              0x80000012
#define U_DC_PEN                0x80000013

#define U_PMR_HEADER                  0x4001
#define U_PMR_ENDOFFILE               0x4002
#define U_PMR_COMMENT                 0x4003
#define U_PMR_GETDC                   0x4004
#define U_PMR_MULTIFORMATSTART        0x4005
#define U_PMR_MULTIFORMATSECTION      0x4006
#define U_PMR_MULTIFORMATEND          0x4007
#define U_PMR_OBJECT                  0x4008
#define U_PMR_CLEAR                   0x4009
#define U_PMR_FILLRECTS               0x400A
#define U_PMR_DRAWRECTS               0x400B
#define U_PMR_FILLPOLYGON             0x400C
#define U_PMR_DRAWLINES               0x400D
#define U_PMR_FILLELLIPSE             0x400E
#define U_PMR_DRAWELLIPSE             0x400F
#define U_PMR_FILLPIE                 0x4010
#define U_PMR_DRAWPIE                 0x4011
#define U_PMR_DRAWARC                 0x4012
#define U_PMR_FILLREGION              0x4013
#define U_PMR_FILLPATH                0x4014
#define U_PMR_DRAWPATH                0x4015
#define U_PMR_FILLCLOSEDCURVE         0x4016
#define U_PMR_DRAWCLOSEDCURVE         0x4017
#define U_PMR_DRAWCURVE               0x4018
#define U_PMR_DRAWBEZIERS             0x4019
#define U_PMR_DRAWIMAGE               0x401A
#define U_PMR_DRAWIMAGEPOINTS         0x401B
#define U_PMR_DRAWSTRING              0x401C
#define U_PMR_SETRENDERINGORIGIN      0x401D
#define U_PMR_SETANTIALIASMODE        0x401E
#define U_PMR_SETTEXTRENDERINGHINT    0x401F
#define U_PMR_SETTEXTCONTRAST         0x4020
#define U_PMR_SETINTERPOLATIONMODE    0x4021
#define U_PMR_SETPIXELOFFSETMODE      0x4022
#define U_PMR_SETCOMPOSITINGMODE      0x4023
#define U_PMR_SETCOMPOSITINGQUALITY   0x4024
#define U_PMR_SAVE                    0x4025
#define U_PMR_RESTORE                 0x4026
#define U_PMR_BEGINCONTAINER          0x4027
#define U_PMR_BEGINCONTAINERNOPARAMS  0x4028
#define U_PMR_ENDCONTAINER            0x4029
#define U_PMR_SETWORLDTRANSFORM       0x402A
#define U_PMR_RESETWORLDTRANSFORM     0x402B
#define U_PMR_MULTIPLYWORLDTRANSFORM  0x402C
#define U_PMR_TRANSLATEWORLDTRANSFORM 0x402D
#define U_PMR_SCALEWORLDTRANSFORM     0x402E
#define U_PMR_ROTATEWORLDTRANSFORM    0x402F
#define U_PMR_SETPAGETRANSFORM        0x4030
#define U_PMR_RESETCLIP               0x4031
#define U_PMR_SETCLIPRECT             0x4032
#define U_PMR_SETCLIPPATH             0x4033
#define U_PMR_SETCLIPREGION           0x4034
#define U_PMR_OFFSETCLIP              0x4035
#define U_PMR_DRAWDRIVERSTRING        0x4036
#define U_PMR_STROKEFILLPATH          0x4037
#define U_PMR_SERIALIZABLEOBJECT      0x4038
#define U_PMR_SETTSGRAPHICS           0x4039
#define U_PMR_SETTSCLIP               0x403A

#define   U_BT_SolidColor               0x00
#define   U_BT_HatchFill                0x01
#define   U_BT_TextureFill              0x02
#define   U_BT_PathGradient             0x03
#define   U_BT_LinearGradient           0x04

#define   U_OT_Invalid                  0x00
#define   U_OT_Brush                    0x01
#define   U_OT_Pen                      0x02
#define   U_OT_Path                     0x03
#define   U_OT_Region                   0x04
#define   U_OT_Image                    0x05
#define   U_OT_Font                     0x06
#define   U_OT_StringFormat             0x07
#define   U_OT_ImageAttributes          0x08
#define   U_OT_CustomLineCap            0x09

#define   U_UT_World                    0x00
#define   U_UT_Display                  0x01
#define   U_UT_Pixel                    0x02
#define   U_UT_Point                    0x03
#define   U_UT_Inch                     0x04
#define   U_UT_Document                 0x05
#define   U_UT_Millimeter               0x06

#define   U_PD_Transform                0x0001
#define   U_PD_StartCap                 0x0002
#define   U_PD_EndCap                   0x0004
#define   U_PD_Join                     0x0008
#define   U_PD_MiterLimit               0x0010
#define   U_PD_LineStyle                0x0020
#define   U_PD_DLCap                    0x0040
#define   U_PD_DLOffset                 0x0080
#define   U_PD_DLData                   0x0100
#define   U_PD_NonCenter                0x0200
#define   U_PD_CLData                   0x0400
#define   U_PD_CustomStartCap           0x0800
#define   U_PD_CustomEndCap             0x1000

#define   U_PPT_Start                   0x00
#define   U_PPT_Line                    0x01
#define   U_PPT_Bezier                  0x03

#define   U_SA_Near                     0x00
#define   U_SA_Center                   0x01
#define   U_SA_Far                      0x02

#define   U_HSP_Horizontal              0x00000000
#define   U_HSP_Vertical                0x00000001
#define   U_HSP_ForwardDiagonal         0x00000002
#define   U_HSP_BackwardDiagonal        0x00000003
#define   U_HSP_LargeGrid               0x00000004
#define   U_HSP_DiagonalCross           0x00000005
#define   U_HSP_05Percent               0x00000006
#define   U_HSP_10Percent               0x00000007
#define   U_HSP_20Percent               0x00000008
#define   U_HSP_25Percent               0x00000009
#define   U_HSP_30Percent               0x0000000A
#define   U_HSP_40Percent               0x0000000B
#define   U_HSP_50Percent               0x0000000C
#define   U_HSP_60Percent               0x0000000D
#define   U_HSP_70Percent               0x0000000E
#define   U_HSP_75Percent               0x0000000F
#define   U_HSP_80Percent               0x00000010
#define   U_HSP_90Percent               0x00000011
#define   U_HSP_LightDownwardDiagonal   0x00000012
#define   U_HSP_LightUpwardDiagonal     0x00000013
#define   U_HSP_DarkDownwardDiagonal    0x00000014
#define   U_HSP_DarkUpwardDiagonal      0x00000015
#define   U_HSP_WideDownwardDiagonal    0x00000016
#define   U_HSP_WideUpwardDiagonal      0x00000017
#define   U_HSP_LightVertical           0x00000018
#define   U_HSP_LightHorizontal         0x00000019
#define   U_HSP_NarrowVertical          0x0000001A
#define   U_HSP_NarrowHorizontal        0x0000001B
#define   U_HSP_DarkVertical            0x0000001C
#define   U_HSP_DarkHorizontal          0x0000001D
#define   U_HSP_DashedDownwardDiagonal  0x0000001E
#define   U_HSP_DashedUpwardDiagonal    0x0000001F
#define   U_HSP_DashedHorizontal        0x00000020
#define   U_HSP_DashedVertical          0x00000021
#define   U_HSP_SmallConfetti           0x00000022
#define   U_HSP_LargeConfetti           0x00000023
#define   U_HSP_ZigZag                  0x00000024
#define   U_HSP_Wave                    0x00000025
#define   U_HSP_DiagonalBrick           0x00000026
#define   U_HSP_HorizontalBrick         0x00000027
#define   U_HSP_Weave                   0x00000028
#define   U_HSP_Plaid                   0x00000029
#define   U_HSP_Divot                   0x0000002A
#define   U_HSP_DottedGrid              0x0000002B
#define   U_HSP_DottedDiamond           0x0000002C
#define   U_HSP_Shingle                 0x0000002D
#define   U_HSP_Trellis                 0x0000002E
#define   U_HSP_Sphere                  0x0000002F
#define   U_HSP_SmallGrid               0x00000030
#define   U_HSP_SmallCheckerBoard       0x00000031
#define   U_HSP_LargeCheckerBoard       0x00000032
#define   U_HSP_OutlinedDiamond         0x00000033
#define   U_HSP_SolidDiamond            0x00000034

#define   U_LS_Solid                    0x00
#define   U_LS_Dash                     0x01
#define   U_LS_Dot                      0x02
#define   U_LS_DashDot                  0x03
#define   U_LS_DashDotDot               0x04
#define   U_LS_Custom                   0x05

#define   U_LJT_Miter                   0x00
#define   U_LJT_Bevel                   0x01
#define   U_LJT_Round                   0x02
#define   U_LJT_MiterClipped            0x03

#define   U_LCT_Flat                    0x00
#define   U_LCT_Square                  0x01
#define   U_LCT_Round                   0x02

#define   U_MDT_Wmf                     0x01
#define   U_MDT_WmfPlaceable            0x02
#define   U_MDT_Emf                     0x03
#define   U_MDT_EmfPlusOnly             0x04
#define   U_MDT_EmfPlusDual             0x05

#define   U_IDT_Bitmap                  0x01
#define   U_IDT_Metafile                0x02

#define   U_PF_1bppIndexed              0x00030101
#define   U_PF_4bppIndexed              0x00030402
#define   U_PF_8bppIndexed              0x00030803
#define   U_PF_16bppGrayScale           0x00101004
#define   U_PF_16bppRGB555              0x00021005
#define   U_PF_16bppRGB565              0x00021006
#define   U_PF_16bppARGB1555            0x00061007
#define   U_PF_24bppRGB                 0x00021808
#define   U_PF_32bppRGB                 0x00022009
#define   U_PF_32bppARGB                0x0026200A
#define   U_PF_32bppPARGB               0x000E200B
#define   U_PF_48bppRGB                 0x0010300C
#define   U_PF_64bppARGB                0x0034400D
#define   U_PF_64bppPARGB               0x001A400E

#define   U_IE_BlurEffectGuid                     "{633C80A4-1843-482B-9EF2-BE2834C5FDD4}"
#define   U_IE_BrightnessContrastEffectGuid       "{D3A1DBE1-8EC4-4C17-9F4C-EA97AD1C343D}"
#define   U_IE_ColorBalanceEffectGuid             "{537E597D-251E-48DA-9664-29CA496B70F8}"
#define   U_IE_ColorCurveEffectGuid               "{DD6A0022-58E4-4A67-9D9B-D48EB881A53D}"
#define   U_IE_ColorLookupTableEffectGuid         "{A7CE72A9-0F7F-40D7-B3CC-D0C02D5C3212}"
#define   U_IE_ColorMatrixEffectGuid              "{718F2615-7933-40E3-A511-5F68FE14DD74}"
#define   U_IE_HueSaturationLightnessEffectGuid   "{8B2DD6C3-EB07-4D87-A5F0-7108E26A9C5F}"
#define   U_IE_LevelsEffectGuid                   "{99C354EC-2A31-4F3A-8C34-17A803B33A25}"
#define   U_IE_RedEyeCorrectionEffectGuid         "{74D29D05-69A4-4266-9549-3CC52836B632}"
#define   U_IE_SharpenEffectGuid                  "{63CBF3EE-C526-402C-8F71-62C540BF5142}"
#define   U_IE_TintEffectGuid                     "{1077AF00-2848-4441-9489-44AD4C2D7A2C}"

#define   U_RNDT_Kids                   0x00000000
#define   U_RNDT_And                    0x00000001
#define   U_RNDT_Or                     0x00000002
#define   U_RNDT_Xor                    0x00000003
#define   U_RNDT_Exclude                0x00000004
#define   U_RNDT_Complement             0x00000005
#define   U_RNDT_Rect                   0x10000000
#define   U_RNDT_Path                   0x10000001
#define   U_RNDT_Empty                  0x10000002
#define   U_RNDT_Infinite               0x10000003

extern SCRIBUS_API ScribusQApp * ScQApp;

EmfPlug::EmfPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = NULL;
}

QImage EmfPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b = 0;
	double h = 0;
	double x = 0;
	double y = 0;
	parseHeader(fName, x, y, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	docX = x;
	docY = y;
	baseX = 0;
	baseY = 0;
	progressDialog = NULL;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	baseX = m_Doc->currentPage()->xOffset();
	baseY = m_Doc->currentPage()->yOffset();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (Elements.count() > 0)
		{
			m_Doc->m_Selection->delaySignalsOn();
			m_Doc->m_Selection->clear();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				m_Doc->m_Selection->addItem(Elements.at(dre), true);
			}
			m_Doc->m_Selection->setGroupRect();
			double gx, gy, gh, gw;
			m_Doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
			m_Doc->moveGroup(baseX - gx, baseY - gy);
			m_Doc->m_Selection->clear();
			m_Doc->m_Selection->delaySignalsOff();
			m_Doc->currentPage()->setInitialHeight(gh);
			m_Doc->currentPage()->setInitialWidth(gw);
			m_Doc->currentPage()->setHeight(gh);
			m_Doc->currentPage()->setWidth(gw);
			m_Doc->setPageHeight(gh);
			m_Doc->setPageWidth(gw);
			m_Doc->setPageSize("Custom");
			m_Doc->currentPage()->m_pageSize = "Custom";
			m_Doc->reformPages(true);
		}
		if (Elements.count() > 1)
		{
			PageItem* grItem = m_Doc->groupObjectsList(Elements);
			grItem->setXYPos(baseX, baseY, true);
		}
		else if (Elements.count() == 1)
			Elements.at(0)->setXYPos(baseX, baseY, true);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
		if (Elements.count() > 0)
		{
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				tmpSel->addItem(Elements.at(dre), true);
			}
			tmpSel->setGroupRect();
			double xs = tmpSel->width();
			double ys = tmpSel->height();
			tmpImage = Elements.at(0)->DrawObj_toImage(500);
			tmpImage.setText("XSize", QString("%1").arg(xs));
			tmpImage.setText("YSize", QString("%1").arg(ys));
		}
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		m_Doc->m_Selection->delaySignalsOff();
		delete m_Doc;
		return tmpImage;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		delete m_Doc;
	}
	return QImage();
}

bool EmfPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	QFileInfo fi = QFileInfo(fName);
	if ( !ScCore->usingGUI() )
	{
		interactive = false;
		showProgress = false;
	}
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if ( showProgress )
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw );
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing File:");
		QList<bool> barsNumeric;
		barsNumeric << false;
		progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		progressDialog->setOverallTotalSteps(3);
		progressDialog->setOverallProgress(0);
		progressDialog->setProgress("GI", 0);
		progressDialog->show();
		connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
		qApp->processEvents();
	}
	else
		progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = 0.0;
	h = 0.0;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	parseHeader(fName, x, y, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	docX = x;
	docY = y;
	baseX = 0;
	baseY = 0;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		m_Doc->currentPage()->setInitialWidth(docWidth);
		m_Doc->currentPage()->setInitialHeight(docHeight);
		m_Doc->currentPage()->setWidth(docWidth);
		m_Doc->currentPage()->setHeight(docHeight);
		m_Doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
		m_Doc->currentPage()->m_pageSize = "Custom";
		m_Doc->reformPages(true);
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc = ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			m_Doc->setPageHeight(docHeight);
			m_Doc->setPageWidth(docWidth);
			m_Doc->currentPage()->setInitialWidth(docWidth);
			m_Doc->currentPage()->setInitialHeight(docHeight);
			m_Doc->currentPage()->setWidth(docWidth);
			m_Doc->currentPage()->setHeight(docHeight);
			ret = true;
			baseX = m_Doc->currentPage()->xOffset();
			baseY = m_Doc->currentPage()->yOffset();
		}
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!interactive))
	{
		if (docWidth > docHeight)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != NULL))
		m_Doc->view()->Deselect();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != NULL))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (Elements.count() > 0)
		{
			m_Doc->m_Selection->delaySignalsOn();
			m_Doc->m_Selection->clear();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				m_Doc->m_Selection->addItem(Elements.at(dre), true);
			}
			m_Doc->m_Selection->setGroupRect();
			double gx, gy, gh, gw;
			m_Doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
			m_Doc->moveGroup(baseX - gx, baseY - gy);
			m_Doc->m_Selection->clear();
			m_Doc->m_Selection->delaySignalsOff();
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				m_Doc->currentPage()->setInitialHeight(gh);
				m_Doc->currentPage()->setInitialWidth(gw);
				m_Doc->currentPage()->setHeight(gh);
				m_Doc->currentPage()->setWidth(gw);
				m_Doc->setPageHeight(gh);
				m_Doc->setPageWidth(gw);
				m_Doc->setPageSize("Custom");
				m_Doc->currentPage()->m_pageSize = "Custom";
				m_Doc->reformPages(true);
			}
		}
		if ((Elements.count() > 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if ((Elements.count() > 0) && (!ret) && (interactive))
		{
			if (flags & LoadSavePlugin::lfScripted)
			{
				bool loadF = m_Doc->isLoading();
				m_Doc->setLoading(false);
				m_Doc->changed();
				m_Doc->setLoading(loadF);
				if (!(flags & LoadSavePlugin::lfLoadAsPattern))
				{
					m_Doc->m_Selection->delaySignalsOn();
					for (int dre=0; dre<Elements.count(); ++dre)
					{
						m_Doc->m_Selection->addItem(Elements.at(dre), true);
					}
					m_Doc->m_Selection->delaySignalsOff();
					m_Doc->m_Selection->setGroupRect();
					if (m_Doc->view() != NULL)
						m_Doc->view()->updatesOn(true);
				}
			}
			else
			{
				m_Doc->DragP = true;
				m_Doc->DraggedElem = 0;
				m_Doc->DragElements.clear();
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_Doc, tmpSel);
				m_Doc->itemSelection_DeleteItem(tmpSel);
				m_Doc->view()->updatesOn(true);
				if (importedColors.count() != 0)
				{
					for (int cd = 0; cd < importedColors.count(); cd++)
					{
						m_Doc->PageColors.remove(importedColors[cd]);
					}
				}
				if (importedPatterns.count() != 0)
				{
					for (int cd = 0; cd < importedPatterns.count(); cd++)
					{
						m_Doc->docPatterns.remove(importedPatterns[cd]);
					}
				}
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
			m_Doc->changed();
			m_Doc->reformPages();
			if (!(flags & LoadSavePlugin::lfLoadAsPattern))
				m_Doc->view()->updatesOn(true);
		}
		success = true;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		if (m_Doc->view() != NULL)
			m_Doc->view()->updatesOn(true);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	if (interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
	{
		if ((showProgress) && (!interactive))
			m_Doc->view()->DrawNew();
	}
	qApp->restoreOverrideCursor();
	return success;
}

EmfPlug::~EmfPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

void EmfPlug::parseHeader(QString fName, double &x, double &y, double &b, double &h)
{
	inEMFPlus = false;
	emfMixed = false;
	bool hasEMF = false;
	bool hasEMFPlus = false;
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ds(&f);
		ds.setByteOrder(QDataStream::LittleEndian);
		quint32 id, size;
		while (!ds.atEnd())
		{
			ds >> id >> size;
			size -= 8;        // Subtract id and size entry to get real data size
			qint64 posi = ds.device()->pos();
			if (inEMFPlus)
			{
				switch (id)
				{
					case U_EMR_EOF:
						break;
					case U_EMR_COMMENT:
					{
						quint32 commTyp, dtaSize;
						ds >> dtaSize;
						ds >> commTyp;
						if (commTyp == 0x2B464D45)
						{
							inEMFPlus = true;
							hasEMFPlus = true;
							quint16 id2, flagsHL;
							quint32 size2;
							QByteArray emfRecords;
							emfRecords.resize(dtaSize);
							ds.readRawData(emfRecords.data(), dtaSize);
							QDataStream dsEmf(emfRecords);
							dsEmf.setByteOrder(QDataStream::LittleEndian);
							dsEmf.setFloatingPointPrecision(QDataStream::SinglePrecision);
							while (!dsEmf.atEnd())
							{
								qint64 posi2 = dsEmf.device()->pos();
								dsEmf >> id2;
								dsEmf >> flagsHL;
								dsEmf >> size2;
								if ((id2 < 0x4000) || (id2 > 0x403A))
									break;
								switch (id2)
								{
									case U_PMR_HEADER:
										emfPlusDual = (flagsHL & 1);
										break;
									case U_PMR_ENDOFFILE:
										inEMFPlus = false;
										break;
									case U_PMR_GETDC:
										if (emfPlusDual)
											inEMFPlus = false;
										break;
									default:
										break;
								}
								dsEmf.device()->seek(posi2 + size2);
							}
						}
					}
				}
			}
			else
			{
				switch (id)
				{
					case U_EMR_HEADER:
						{
							qint32 bLeft, bTop, bRight, bBottom;
							ds >> bLeft >> bTop >> bRight >> bBottom;
							qint32 pLeft, pTop, pRight, pBottom;
							ds >> pLeft >> pTop >> pRight >> pBottom;
							quint32 sign, vers, bytes;
							ds >> sign >> vers >> bytes >> m_records;
							bBoxDev = QRectF(QPointF(bLeft, bTop), QPointF(bRight, bBottom)).normalized();
							bBoxMM = QRectF(QPointF(pLeft, pTop), QPointF(pRight, pBottom)).normalized();
							dpiX = bBoxDev.width() / (bBoxMM.width() / 100.0) * 25.4;
							dpiY = bBoxDev.height() / (bBoxMM.height() / 100.0) * 25.4;
							b = bBoxMM.width() / 1000.0 / 2.54 * 72.0;
							h = bBoxMM.height() / 1000.0 / 2.54 * 72.0;
							x = bBoxMM.x() / 1000.0 / 2.54 * 72.0;
							y = bBoxMM.y() / 1000.0 / 2.54 * 72.0;
						}
						break;
					case U_EMR_EOF:
						break;
					case U_EMR_COMMENT:
						{
							quint32 commTyp, dtaSize;
							ds >> dtaSize;
							ds >> commTyp;
							if (commTyp == 0x2B464D45)
							{
								inEMFPlus = true;
								hasEMFPlus = true;
								quint16 id2, flagsHL;
								quint32 size2;
								QByteArray emfRecords;
								emfRecords.resize(dtaSize);
								ds.readRawData(emfRecords.data(), dtaSize);
								QDataStream dsEmf(emfRecords);
								dsEmf.setByteOrder(QDataStream::LittleEndian);
								dsEmf.setFloatingPointPrecision(QDataStream::SinglePrecision);
								while (!dsEmf.atEnd())
								{
									qint64 posi2 = dsEmf.device()->pos();
									dsEmf >> id2;
									dsEmf >> flagsHL;
									dsEmf >> size2;
									if ((id2 < 0x4000) || (id2 > 0x403A))
										break;
									switch (id2)
									{
										case U_PMR_HEADER:
											emfPlusDual = (flagsHL & 1);
											break;
										case U_PMR_ENDOFFILE:
											inEMFPlus = false;
											break;
										case U_PMR_GETDC:
											if (emfPlusDual)
												inEMFPlus = false;
											break;
										default:
											break;
									}
									dsEmf.device()->seek(posi2 + size2);
								}
							}
						}
						break;
					default:
						hasEMF = true;
						break;
				}
			}
			ds.device()->seek(posi + size);
		}
		f.close();
	}
	inEMFPlus = false;
	if (hasEMF && hasEMFPlus)
		emfMixed = true;
}

bool EmfPlug::convert(QString fn)
{
	importedColors.clear();
	importedPatterns.clear();
	currentDC.CurrColorFill = "White";
	currentDC.CurrFillTrans = 0.0;
	currentDC.CurrColorStroke = "Black";
	currentDC.CurrStrokeTrans = 0.0;
	currentDC.CurrColorText = "Black";
	currentDC.backColor = CommonStrings::None;
	currentDC.LineW = 1.0;
	currentDC.penStyle = Qt::SolidLine;
	currentDC.penCap = Qt::RoundCap;
	currentDC.penJoin = Qt::RoundJoin;
	currentDC.m_mapMode = 1;
	currentDC.backgroundMode = false;
	currentDC.arcDirection = true;
	currentDC.alphaOn = true;
	currentDC.fillRule = true;
	currentDC.textAlignment = 0;
	currentDC.brushStyle = U_BT_SolidColor;
	currentDC.hatchStyle = 0;
	currentDC.m_WorldMap = QTransform();
	currentDC.m_WorldMapEMFP = QTransform();
	currentDC.Coords.resize(0);
	currentDC.Coords.svgInit();
	currentDC.clipPath.resize(0);
	currentDC.clipPath.svgInit();
	currentDC.fontSize = 12;
	currentDC.fontName = "Arial";
	currentDC.fontRotation = 0;
	currentDC.viewOrigin = QPointF(0, 0);
	currentDC.winOrigin = QPointF(0, 0);
	currentDC.currentPoint = QPointF();
	currentDC.originEMFP = QPointF(0, 0);
	currentDC.patternName = "";
	currentDC.emfPlusUnit = 2;
	inPath = false;
	inEMFPlus = false;
	emfPlusDual = false;
	SerializableObject_Valid = false;
	m_Effects.clear();
	emfPlusScale = 1.0;
	m_ObjSize = 0;
	m_currObjSize = 0;
	QColor col;
	emfStyle sty;
	// WHITE_BRUSH
	sty.styType = U_OT_Brush;
	sty.brushColor = "White";
	emfStyleMap.insert(U_WHITE_BRUSH, sty);
	// LTGRAY_BRUSH
	col = QColor(Qt::lightGray);
	sty.brushColor = handleColor(col);
	emfStyleMap.insert(U_LTGRAY_BRUSH, sty);
	// GRAY_BRUSH
	col = QColor(Qt::gray);
	sty.brushColor = handleColor(col);
	emfStyleMap.insert(U_GRAY_BRUSH, sty);
	// DKGRAY_BRUSH
	col = QColor(Qt::darkGray);
	sty.brushColor = handleColor(col);
	emfStyleMap.insert(U_DKGRAY_BRUSH, sty);
	// BLACK_BRUSH
	sty.brushColor = "Black";
	emfStyleMap.insert(U_BLACK_BRUSH, sty);
	// NULL_BRUSH
	sty.brushColor = CommonStrings::None;
	emfStyleMap.insert(U_NULL_BRUSH, sty);
	// WHITE_PEN
	sty.styType = U_OT_Pen;
	sty.brushColor = CommonStrings::None;
	sty.penColor = "White";
	emfStyleMap.insert(U_WHITE_PEN, sty);
	// BLACK_PEN
	sty.penColor = "Black";
	emfStyleMap.insert(U_BLACK_PEN, sty);
	// NULL_PEN
	sty.penColor = CommonStrings::None;
	emfStyleMap.insert(U_NULL_PEN, sty);
	sty.styType = U_OT_Font;
	// OEM_FIXED_FONT
	emfStyleMap.insert(U_OEM_FIXED_FONT, sty);
	// ANSI_FIXED_FONT
	emfStyleMap.insert(U_ANSI_FIXED_FONT, sty);
	// ANSI_VAR_FONT
	emfStyleMap.insert(U_ANSI_VAR_FONT, sty);
	// SYSTEM_FONT
	emfStyleMap.insert(U_SYSTEM_FONT, sty);
	// DEVICE_DEFAULT_FONT
	emfStyleMap.insert(U_DEVICE_DEFAULT_FONT, sty);
	// DEFAULT_PALETTE
	emfStyleMap.insert(U_DEFAULT_PALETTE, sty);
	// SYSTEM_FIXED_FONT
	emfStyleMap.insert(U_SYSTEM_FIXED_FONT, sty);
	// DEFAULT_GUI_FONT
	emfStyleMap.insert(U_DEFAULT_GUI_FONT, sty);
	// DC_BRUSH
	sty.styType = U_OT_Brush;
	emfStyleMap.insert(U_DC_BRUSH, sty);
	// DC_PEN
	sty.styType = U_OT_Pen;
	emfStyleMap.insert(U_DC_PEN, sty);
	emfStyleMapEMP.clear();
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		if (progressDialog)
		{
			progressDialog->setTotalSteps("GI", m_records);
			qApp->processEvents();
		}
		QDataStream ds(&f);
		ds.setByteOrder(QDataStream::LittleEndian);
		ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
		quint32 id, size;
		recordCount = 0;
		while (!ds.atEnd())
		{
			qint64 posi = ds.device()->pos();
			ds >> id >> size;
			recordCount++;
			if (inEMFPlus)
			{
				if (id == U_EMR_EOF)
					break;
				else if (id == U_EMR_COMMENT)
					handleComment(ds);
			}
			else
			{
				switch (id)
				{
					case U_EMR_HEADER:
						{
							qint32 bLeft, bTop, bRight, bBottom;
							ds >> bLeft >> bTop >> bRight >> bBottom;
							qint32 pLeft, pTop, pRight, pBottom;
							ds >> pLeft >> pTop >> pRight >> pBottom;
							quint32 sign, vers, bytes, records, dummy;
							ds >> sign >> vers >> bytes >> records >> dummy >> dummy >> dummy >> dummy;
							quint32 sxDev, syDev;
							ds >> sxDev >> syDev;
							quint32 sxMM, syMM;
							ds >> sxMM >> syMM;
							viewPextendX = bBoxDev.width();
							viewPextendY = bBoxDev.height();
						}
						break;
					case U_EMR_POLYBEZIER:
						handleBezier(ds, true);
						break;
					case U_EMR_POLYGON:
						handlePolygon(ds, true, true);
						break;
					case U_EMR_POLYLINE:
						handlePolygon(ds, true, false);
						break;
					case U_EMR_POLYBEZIERTO:
						handlePolyBezierTo(ds, true);
						break;
					case U_EMR_POLYLINETO:
						handlePolylineTo(ds, true);
						break;
					case U_EMR_POLYPOLYLINE:
						handlePolyPolygon(ds, true, false);
						break;
					case U_EMR_POLYPOLYGON:
						handlePolyPolygon(ds, true, true);
						break;
					case U_EMR_SETWINDOWEXTEX:
						ds >> winPextendX >> winPextendY;
						currentDC.winOrigin = convertLogical2Pts(QPointF(winOrigX, winOrigY));
						break;
					case U_EMR_SETWINDOWORGEX:
						ds >> winOrigX >> winOrigY;
						if ((viewPextendX != 0) && (viewPextendY != 0))
							currentDC.winOrigin = convertLogical2Pts(QPointF(winOrigX, winOrigY));
						break;
					case U_EMR_SETVIEWPORTEXTEX:
						ds >> viewPextendX >> viewPextendY;
						break;
					case U_EMR_SETVIEWPORTORGEX:
						{
							qint32 w, h;
							ds >> w >> h;
							currentDC.viewOrigin = convertDevice2Pts(QPointF(w, h));
						}
						break;
					case U_EMR_SETMAPMODE:
						ds >> currentDC.m_mapMode;
						break;
					case U_EMR_SETBKMODE:
						{
							quint32 fm;
							ds >> fm;
							currentDC.backgroundMode = (fm == 2);
						}
						break;
					case U_EMR_SETPOLYFILLMODE:
						{
							quint32 fm;
							ds >> fm;
							currentDC.fillRule = (fm == 1);
						}
						break;
					case U_EMR_SETTEXTALIGN:
						ds >> currentDC.textAlignment;
						break;
					case U_EMR_SETTEXTCOLOR:
						{
							quint32 brColor = getColor(ds);
							QColor col((QRgb)brColor);
							currentDC.CurrColorText = handleColor(col);
						}
						break;
					case U_EMR_SETBKCOLOR:
						{
							quint32 brColor = getColor(ds);
							QColor col((QRgb)brColor);
							currentDC.backColor = handleColor(col);
						}
						break;
					case U_EMR_MOVETOEX:
						currentDC.currentPoint = getPoint(ds, true);
						currentDC.Coords.svgMoveTo(currentDC.currentPoint.x(), currentDC.currentPoint.y());
						break;
					case U_EMR_SAVEDC:
						dcStack.push(currentDC);
						break;
					case U_EMR_RESTOREDC:
						{
							qint32 drop;
							ds >> drop;
							drop = qAbs(drop);
							if (dcStack.count() >= drop)
							{
								for (qint32 a = 0; a < drop; a++)
								{
									currentDC = dcStack.pop();
									if (dcStack.count() == 0)
										break;
								}
							}
						}
						break;
					case U_EMR_SETWORLDTRANSFORM:
						{
							float m11, m12, m21, m22, dx, dy;
							ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
							setWTransform(QTransform(m11, m12, m21, m22, dx, dy), 4);
						}
						break;
					case U_EMR_MODIFYWORLDTRANSFORM:
						{
							float m11, m12, m21, m22, dx, dy;
							ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
							quint32 mod;
							ds >> mod;
							setWTransform(QTransform(m11, m12, m21, m22, dx, dy), mod);
						}
						break;
					case U_EMR_SELECTOBJECT:
						{
							quint32 numObj;
							ds >> numObj;
							if (emfStyleMap.contains(numObj))
							{
								emfStyle sty = emfStyleMap[numObj];
								if (sty.styType == U_OT_Pen)
								{
									currentDC.CurrColorStroke = sty.penColor;
									currentDC.CurrStrokeTrans = sty.penTrans;
									currentDC.penCap = sty.penCap;
									currentDC.penJoin = sty.penJoin;
									currentDC.penStyle = sty.penStyle;
									currentDC.LineW = sty.penWidth;
								}
								else if (sty.styType == U_OT_Brush)
								{
									currentDC.CurrColorFill = sty.brushColor;
									currentDC.brushStyle = sty.brushStyle;
									currentDC.hatchStyle = sty.hatchStyle;
									currentDC.patternName = sty.patternName;
									currentDC.CurrFillTrans = sty.fillTrans;
								}
								else if (sty.styType == U_OT_Font)
								{
									currentDC.fontName = sty.fontName;
									currentDC.fontSize = sty.fontSize;
									currentDC.fontRotation = sty.fontRotation;
								}
							}
							else
								qDebug() << QString("U_EMR_SELECTOBJECT -> try to select nonexisting Object Nr %1!").arg(numObj);
						}
						break;
					case U_EMR_CREATEPEN:
						{
							quint32 penID, penStyle, penWidth, dummy, penColor;
							ds >> penID >> penStyle >> penWidth >> dummy;
							penColor = getColor(ds);
							handlePenDef(penID, penStyle, penWidth, penColor);
						}
						break;
					case U_EMR_CREATEBRUSHINDIRECT:
						{
							quint32 brID, brStyle, brHatch, brColor;
							ds >> brID >> brStyle;
							brColor = getColor(ds);
							ds >> brHatch;
							QColor col((QRgb)brColor);
							emfStyle sty;
							sty.styType = U_OT_Brush;
							if (brStyle == 1)
								sty.brushColor = CommonStrings::None;
							else
								sty.brushColor = handleColor(col);
							sty.brushStyle = brStyle;
							sty.hatchStyle = brHatch;
							sty.fillTrans = 0;
							emfStyleMap.insert(brID, sty);
						}
						break;
					case U_EMR_DELETEOBJECT:
						{
							quint32 numObj;
							ds >> numObj;
							if (numObj < U_STOCK_OBJECT)
								emfStyleMap.remove(numObj);
						}
						break;
					case U_EMR_ELLIPSE:
						handleEllipse(ds);
						break;
					case U_EMR_RECTANGLE:
						handleRectangle(ds);
						break;
					case U_EMR_ROUNDRECT:
						handleRoundRect(ds);
						break;
					case U_EMR_ARC:
						handleArc(ds);
						break;
					case U_EMR_CHORD:
						handleChord(ds);
						break;
					case U_EMR_PIE:
						handlePie(ds);
						break;
					case U_EMR_LINETO:
						handleLineTo(ds);
						break;
					case U_EMR_ARCTO:
						handleArcTo(ds);
						break;
					case U_EMR_SETARCDIRECTION:
						{
							quint32 fm;
							ds >> fm;
							currentDC.arcDirection = (fm == 1);
						}
						break;
					case U_EMR_BEGINPATH:
						inPath = true;
						currentDC.Coords.resize(0);
						currentDC.Coords.svgInit();
						break;
					case U_EMR_ENDPATH:
						inPath = false;
						break;
					case U_EMR_CLOSEFIGURE:
						currentDC.Coords.svgClosePath();
						break;
					case U_EMR_FILLPATH:
						{
							int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
							PageItem* ite = m_Doc->Items->at(z);
							ite->PoLine = currentDC.Coords.copy();
							finishItem(ite);
						}
						break;
					case U_EMR_STROKEANDFILLPATH:
						{
							int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
							PageItem* ite = m_Doc->Items->at(z);
							ite->PoLine = currentDC.Coords.copy();
							finishItem(ite);
						}
						break;
					case U_EMR_STROKEPATH:
						{
							int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
							PageItem* ite = m_Doc->Items->at(z);
							ite->PoLine = currentDC.Coords.copy();
							finishItem(ite, false);
						}
						break;
					case U_EMR_SELECTCLIPPATH:
						{
							quint32 mode;
							ds >> mode;
							if (currentDC.Coords.count() != 0)
								currentDC.clipPath = currentDC.Coords.copy();
						}
						break;
					case U_EMR_ABORTPATH:
						inPath = false;
						currentDC.Coords.resize(0);
						currentDC.Coords.svgInit();
						break;
					case U_EMR_COMMENT:
						handleComment(ds);
						break;
					case U_EMR_FILLRGN:
						handleFillRegion(ds);
						break;
					case U_EMR_FRAMERGN:
						handleFrameRegion(ds);
						break;
					case U_EMR_BITBLT:
						{
							qint32 bLeft, bTop, bRight, bBottom, dstX, dstY, dstW, dstH, srcLeft, srcTop;
							quint32 offBitH, sizeBitH, offBits, sizeBits, usage, rasterOp, bgCol;
							ds >> bLeft >> bTop >> bRight >> bBottom;
							ds >> dstX >> dstY >> dstW >> dstH;
							ds >> rasterOp;
							ds >> srcLeft >> srcTop;
							float m11, m12, m21, m22, dx, dy;
							ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
							ds >> bgCol >> usage;
							ds >> offBitH >> sizeBitH >> offBits >> sizeBits;
							QImage img = handleDIB(ds, posi, offBitH, sizeBitH, offBits, sizeBits);
							if (!img.isNull())
								handleImage(dstX, dstY, dstW, dstH, img);
							else
								handlePatternFill(dstX, dstY, dstW, dstH);
						}
						break;
					case U_EMR_STRETCHBLT:
						{
							qint32 bLeft, bTop, bRight, bBottom, dstX, dstY, dstW, dstH, srcLeft, srcTop, srcW, srcH;
							quint32 offBitH, sizeBitH, offBits, sizeBits, usage, rasterOp, bgCol;
							ds >> bLeft >> bTop >> bRight >> bBottom;
							ds >> dstX >> dstY >> dstW >> dstH;
							ds >> rasterOp;
							ds >> srcLeft >> srcTop;
							float m11, m12, m21, m22, dx, dy;
							ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
							ds >> bgCol >> usage;
							ds >> offBitH >> sizeBitH >> offBits >> sizeBits;
							ds >> srcW >> srcH;
							QImage img = handleDIB(ds, posi, offBitH, sizeBitH, offBits, sizeBits);
							if (!img.isNull())
								handleImage(dstX, dstY, dstW, dstH, img);
							else
								handlePatternFill(dstX, dstY, dstW, dstH);
						}
						break;
					case U_EMR_STRETCHDIBITS:
						{
							qint32 bLeft, bTop, bRight, bBottom, dstX, dstY, dstW, dstH, srcLeft, srcTop, srcRight, srcBottom;
							quint32 offBitH, sizeBitH, offBits, sizeBits, usage, rasterOp;
							ds >> bLeft >> bTop >> bRight >> bBottom;
							ds >> dstX >> dstY;
							ds >> srcLeft >> srcTop >> srcRight >> srcBottom;
							ds >> offBitH >> sizeBitH >> offBits >> sizeBits;
							ds >> usage >> rasterOp;
							ds >> dstW >> dstH;
							QImage img = handleDIB(ds, posi, offBitH, sizeBitH, offBits, sizeBits);
							if (!img.isNull())
								handleImage(dstX, dstY, dstW, dstH, img);
							else
								handlePatternFill(dstX, dstY, dstW, dstH);
						}
						break;
					case U_EMR_EXTCREATEFONTINDIRECTW:
						{
							quint32 foID, foFlags1, foFlags2;
							qint32 foHeight, foWidth, foEsc, foOri, foWeight;
							QString fontName = "";
							ds >> foID >> foHeight >> foWidth >> foEsc >> foOri >> foWeight >> foFlags1 >> foFlags2;
							QByteArray foNam;
							for (int c = 0; c < 32; c++)
							{
								quint16 cc;
								ds >> cc;
								if (cc == 0)
									break;
								foNam.append(cc);
							}
							fontName = QString::fromUtf8(foNam.data());
							emfStyle sty;
							sty.styType = U_OT_Font;
							sty.fontRotation = foOri / 10.0;
							sty.fontSize = qAbs(convertLogical2Pts(static_cast<double>(foHeight)));
							sty.fontName = fontName;
							emfStyleMap.insert(foID, sty);
						}
						break;
					case U_EMR_EXTTEXTOUTA:
						handleText(ds, posi, false);
						break;
					case U_EMR_EXTTEXTOUTW:
						handleText(ds, posi, true);
						break;
					case U_EMR_POLYBEZIER16:
						handleBezier(ds, false);
						break;
					case U_EMR_POLYGON16:
						handlePolygon(ds, false, true);
						break;
					case U_EMR_POLYLINE16:
						handlePolygon(ds, false, false);
						break;
					case U_EMR_POLYBEZIERTO16:
						handlePolyBezierTo(ds, false);
						break;
					case U_EMR_POLYLINETO16:
						handlePolylineTo(ds, false);
						break;
					case U_EMR_POLYPOLYLINE16:
						handlePolyPolygon(ds, false, false);
						break;
					case U_EMR_POLYPOLYGON16:
						handlePolyPolygon(ds, false, true);
						break;
					case U_EMR_CREATEMONOBRUSH:
						{
							quint32 brID, usage, offBitH, sizeBitH, offBits, sizeBits;
							ds >> brID >> usage;
							ds >> offBitH >> sizeBitH >> offBits >> sizeBits;
							QImage img = handleDIB(ds, posi, offBitH, sizeBitH, offBits, sizeBits);
							if (!img.isNull())
								createPatternFromDIB(img, brID);
						}
						break;
					case U_EMR_CREATEDIBPATTERNBRUSHPT:
						{
							quint32 brID, usage, offBitH, sizeBitH, offBits, sizeBits;
							ds >> brID >> usage;
							ds >> offBitH >> sizeBitH >> offBits >> sizeBits;
							QImage img = handleDIB(ds, posi, offBitH, sizeBitH, offBits, sizeBits);
							if (!img.isNull())
								createPatternFromDIB(img, brID);
						}
						break;
					case U_EMR_EXTCREATEPEN:
						{
							quint32 penID, penStyle, penWidth, dummy, penColor;
							ds >> penID >> dummy >> dummy >> dummy >> dummy;
							ds >> penStyle >> penWidth >> dummy;
							penColor = getColor(ds);
							handlePenDef(penID, penStyle, penWidth, penColor);
						}
						break;
					case U_EMR_EXTSELECTCLIPRGN:
						handleSetClipRegion(ds);
						break;
					case U_EMR_SMALLTEXTOUT:
						handleSmallText(ds);
						break;
					case U_EMR_POLYDRAW16:
						qDebug() << "U_EMR_POLYDRAW16";
						break;
					case U_EMR_POLYTEXTOUTA:
						qDebug() << "U_EMR_POLYTEXTOUTA";
						break;
					case U_EMR_POLYTEXTOUTW:
						qDebug() << "U_EMR_POLYTEXTOUTW";
						break;
					case U_EMR_GRADIENTFILL:
						qDebug() << "U_EMR_GRADIENTFILL";
						break;
					case U_EMR_ANGLEARC:
						qDebug() << "U_EMR_ANGLEARC";
						break;
					case U_EMR_POLYDRAW:
						qDebug() << "U_EMR_POLYDRAW";
						break;
					case U_EMR_SETROP2:
					case U_EMR_SETSTRETCHBLTMODE:
					case U_EMR_SETCOLORADJUSTMENT:
					case U_EMR_FLATTENPATH:
					case U_EMR_WIDENPATH:
					case U_EMR_EXTFLOODFILL:
					case U_EMR_INVERTRGN:
					case U_EMR_PAINTRGN:
					case U_EMR_MASKBLT:
					case U_EMR_PLGBLT:
					case U_EMR_SETDIBITSTODEVICE:
					case U_EMR_SETICMMODE:
					case U_EMR_CREATECOLORSPACE:
					case U_EMR_SETCOLORSPACE:
					case U_EMR_DELETECOLORSPACE:
					case U_EMR_GLSRECORD:
					case U_EMR_GLSBOUNDEDRECORD:
					case U_EMR_PIXELFORMAT:
					case U_EMR_DRAWESCAPE:
					case U_EMR_EXTESCAPE:
					case U_EMR_UNDEF107:
					case U_EMR_FORCEUFIMAPPING:
					case U_EMR_NAMEDESCAPE:
					case U_EMR_COLORCORRECTPALETTE:
					case U_EMR_SETICMPROFILEA:
					case U_EMR_SETICMPROFILEW:
					case U_EMR_ALPHABLEND:
					case U_EMR_SETLAYOUT:
					case U_EMR_TRANSPARENTBLT:
					case U_EMR_UNDEF117:
					case U_EMR_SETLINKEDUFIS:
					case U_EMR_SETTEXTJUSTIFICATION:
					case U_EMR_COLORMATCHTOTARGETW:
					case U_EMR_CREATECOLORSPACEW:
					case U_EMR_EOF:
						//	qDebug() << "\tUnsupported Op-Code" << id2;
						break;
					default:
						//qDebug() << "\tUnknown Op-Code" << id;
						break;
				}
				if (id == U_EMR_EOF)
					break;
			}
			ds.device()->seek(posi + size);
			if (progressDialog)
			{
				progressDialog->setProgress("GI", recordCount);
				qApp->processEvents();
			}
		}
		if (Elements.count() == 0)
		{
			if (importedColors.count() != 0)
			{
				for (int cd = 0; cd < importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(importedColors[cd]);
				}
			}
			if (importedPatterns.count() != 0)
			{
				for (int cd = 0; cd < importedPatterns.count(); cd++)
				{
					m_Doc->docPatterns.remove(importedPatterns[cd]);
				}
			}
		}
		f.close();
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}

void EmfPlug::aligntoQuadWord(QDataStream &ds)
{
	if ((ds.device()->pos() % 4) != 0)
	{
		qint32 adj = 4 - (ds.device()->pos() % 4);
		ds.skipRawData(adj);
	}
}

double EmfPlug::convertDevice2Pts(double in)
{
	QPointF pp;
	pp.setX(in);
	pp = convertDevice2Pts(pp);
	return pp.x();
}

QPointF EmfPlug::convertDevice2Pts(QPointF in)
{
	QPointF out;
	double scaleX = (bBoxMM.width() / 1000.0 / 2.54 * 72.0) / bBoxDev.width(); // Device -> Pts
	double scaleY = (bBoxMM.height() / 1000.0 / 2.54 * 72.0) / bBoxDev.height(); // Device -> Pts
	out.setX(in.x() * scaleX);
	out.setY(in.y() * scaleY);

	return out;
}

double EmfPlug::convertLogical2Pts(double in)
{
	QPointF pp;
	pp.setX(in);
	pp = convertLogical2Pts(pp);
	return pp.x();
}

QPointF EmfPlug::convertLogical2Pts(QPointF in)
{
	QPointF out;
	double scaleX = qAbs((bBoxMM.width() / 1000.0 / 2.54 * 72.0) / bBoxDev.width()); // Device -> Pts
	double scaleY = qAbs((bBoxMM.height() / 1000.0 / 2.54 * 72.0) / bBoxDev.height()); // Device -> Pts
	if (currentDC.m_mapMode == 1)
	{
		out.setX(in.x() * scaleX);
		out.setY(in.y() * scaleY);
	}
	else if (currentDC.m_mapMode == 2)
	{
		out.setX(in.x() / 100.0 / 2.54 * 72.0);
		out.setY(in.y() / 100.0 / 2.54 * 72.0);
	}
	else if (currentDC.m_mapMode == 3)
	{
		out.setX(in.x() / 1000.0 / 2.54 * 72.0);
		out.setY(in.y() / 1000.0 / 2.54 * 72.0);
	}
	else if (currentDC.m_mapMode == 4)
	{
		out.setX(in.x() / 1000.0 * 72.0);
		out.setY(in.y() / 1000.0 * 72.0);
	}
	else if (currentDC.m_mapMode == 5)
	{
		out.setX(in.x() / 10000.0 * 72.0);
		out.setY(in.y() / 10000.0 * 72.0);
	}
	else if (currentDC.m_mapMode == 6)
	{
		out.setX(in.x() / 1440.0 * 72.0);
		out.setY(in.y() / 1440.0 * 72.0);
	}
	else if ((currentDC.m_mapMode == 0x07) || (currentDC.m_mapMode == 0x08))
	{
		double ratioX = viewPextendX / static_cast<double>(winPextendX); // Logical --> Device
		double ratioY = viewPextendY / static_cast<double>(winPextendY); // Logical --> Device
		// logic --> device --> pts
		out.setX(in.x() * ratioX * scaleX);
		out.setY(in.y() * ratioY * scaleY);
	}
	return out;
}

void EmfPlug::createPatternFromDIB(QImage img, quint32 brID)
{
	if (!img.isNull())
	{
		QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_emf_XXXXXX.png");
		tempFile->setAutoRemove(false);
		if (tempFile->open())
		{
			QString fileName = getLongPathName(tempFile->fileName());
			if (!fileName.isEmpty())
			{
				tempFile->close();
				img.save(fileName, "PNG");
				ScPattern pat = ScPattern();
				pat.setDoc(m_Doc);
				int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
				PageItem* newItem = m_Doc->Items->at(z);
				m_Doc->loadPict(fileName, newItem);
				m_Doc->Items->takeAt(z);
				newItem->isInlineImage = true;
				newItem->isTempFile = true;
				pat.width = newItem->pixm.qImage().width();
				pat.height = newItem->pixm.qImage().height();
				pat.scaleX = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
				pat.scaleY = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
				pat.pattern = newItem->pixm.qImage().copy();
				newItem->setWidth(pat.pattern.width());
				newItem->setHeight(pat.pattern.height());
				newItem->SetRectFrame();
				newItem->gXpos = 0.0;
				newItem->gYpos = 0.0;
				newItem->gWidth = pat.pattern.width();
				newItem->gHeight = pat.pattern.height();
				pat.items.append(newItem);
				QString patternName = "Pattern_"+newItem->itemName();
				m_Doc->addPattern(patternName, pat);
				emfStyle sty;
				sty.styType = U_OT_Brush;
				sty.brushStyle = U_BT_TextureFill;
				sty.patternName = patternName;
				sty.fillTrans = 0;
				emfStyleMap.insert(brID, sty);
				importedPatterns.append(patternName);
			}
		}
	}
}

void EmfPlug::getPolyInfo(QDataStream &ds, QRectF &bounds, quint32 &count)
{
	qint32 bLeft, bTop, bRight, bBottom;
	ds >> bLeft >> bTop >> bRight >> bBottom;
	bounds = QRectF(QPointF(bLeft, bTop), QPointF(bRight, bBottom));
	ds >> count;
}

FPointArray EmfPlug::getPolyPoints(QDataStream &ds, quint32 count, bool length, bool closed)
{
	bool bFirst = true;
	FPointArray polyline;
	polyline.svgInit();
	for (quint32 a = 0; a < count; a++)
	{
		QPointF p = getPoint(ds, length);
		if (inPath)
		{
			if (bFirst)
			{
				currentDC.Coords.svgMoveTo(p.x(), p.y());
				bFirst = false;
			}
			else
				currentDC.Coords.svgLineTo(p.x(), p.y());
		}
		else
		{
			if (bFirst)
			{
				polyline.svgMoveTo(p.x(), p.y());
				bFirst = false;
			}
			else
				polyline.svgLineTo(p.x(), p.y());
		}
	}
	if (inPath)
	{
		if ((currentDC.Coords.size() > 4) && (closed))
			currentDC.Coords.svgClosePath();
	}
	else
	{
		if ((polyline.size() > 4) && (closed))
			polyline.svgClosePath();
	}
	return polyline;
}

QPointF EmfPlug::getPoint(QDataStream &ds, bool size)
{
	QPointF p;
	if (size)
	{
		qint32 x1, y1;
		ds >> x1 >> y1;
		p = currentDC.m_WorldMap.map(QPointF(x1, y1));
		p = convertLogical2Pts(p);
	}
	else
	{
		qint16 x1, y1;
		ds >> x1 >> y1;
		p = currentDC.m_WorldMap.map(QPointF(x1, y1));
		p = convertLogical2Pts(p);
	}
	p += currentDC.viewOrigin;
	return p;
}

quint32 EmfPlug::getColor(QDataStream &ds)
{
	quint8 r, g, b, a;
	ds >> r >> g >> b >> a;
	return qRgba(r, g, b, 255);
}

void EmfPlug::setWTransform(QTransform mm, quint32 how)
{
	if (how == 1)
		currentDC.m_WorldMap = QTransform();
	else if (how == 2)
		currentDC.m_WorldMap = mm * currentDC.m_WorldMap;
	else if (how == 3)
		currentDC.m_WorldMap = currentDC.m_WorldMap * mm;
	else if (how == 4)
		currentDC.m_WorldMap = mm;
}

QPointF EmfPlug::intersectBoundingRect(PageItem *item, QLineF gradientVector)
{
	QPointF interPoint;
	QPointF gradEnd;
	if (gradientVector.intersect(QLineF(0, 0, item->width(), 0), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	else if (gradientVector.intersect(QLineF(item->width(), 0, item->width(), item->height()), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	else if (gradientVector.intersect(QLineF(item->width(), item->height(), 0, item->height()), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	else if (gradientVector.intersect(QLineF(0, item->height(), 0, 0), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	return gradEnd;
}

void EmfPlug::finishItem(PageItem* ite, bool fill)
{
	ite->fillRule = currentDC.fillRule;
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(100.0);
	ite->setLineShade(100.0);
	ite->setLineJoin(currentDC.penJoin);
	ite->setLineEnd(currentDC.penCap);
	ite->setLineStyle(currentDC.penStyle);
	if (!currentDC.dashArray.isEmpty())
	{
		ite->DashValues.clear();
		for (int a = 0; a < currentDC.dashArray.count(); a++)
		{
			ite->DashValues.append(currentDC.dashArray[a] * ite->lineWidth());
		}
	}
	ite->DashOffset = currentDC.dashOffset;
	if (inEMFPlus && currentDC.alphaOn)
	{
		ite->setFillTransparency(currentDC.CurrFillTrans);
		ite->setLineTransparency(currentDC.CurrStrokeTrans);
	}
	FPoint oldPos = getMinClipF(&ite->PoLine);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_Doc->AdjustItemSize(ite);
	ite->moveBy(-docX, -docY, true);
	ite->moveBy(-currentDC.winOrigin.x(), -currentDC.winOrigin.y());
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	if (fill)
	{
		if (inEMFPlus)
		{
			if (currentDC.brushStyle == U_BT_HatchFill)
			{
				switch (currentDC.hatchStyle)
				{
					case U_HSP_Horizontal:
						ite->setHatchParameters(0, 5, 0, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_Vertical:
						ite->setHatchParameters(0, 5, 90, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_ForwardDiagonal:
						ite->setHatchParameters(0, 5, -45, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_BackwardDiagonal:
						ite->setHatchParameters(0, 5, 45, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_LargeGrid:
						ite->setHatchParameters(1, 5, 0, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_DiagonalCross:
						ite->setHatchParameters(1, 5, 45, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					default:
						ite->setHatchParameters(1, 5, 45, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
				}
			}
			else if (currentDC.brushStyle == U_BT_LinearGradient)
			{
				ite->fill_gradient = currentDC.gradient;
				QLineF gradientVectorE;
				gradientVectorE.setP1(QPointF(ite->width() / 2.0, ite->height() / 2.0));
				gradientVectorE.setAngle(currentDC.gradientAngle);
				gradientVectorE.setLength(sqrt(ite->width() * ite->width() + ite->height() * ite->height()) / 2.0 + 1.0);
				QPointF gradEnd = intersectBoundingRect(ite, gradientVectorE);
				QLineF gradientVectorS;
				gradientVectorS.setP1(QPointF(ite->width() / 2.0, ite->height() / 2.0));
				gradientVectorS.setAngle(currentDC.gradientAngle + 180);
				gradientVectorS.setLength(sqrt(ite->width() * ite->width() + ite->height() * ite->height()) / 2.0 + 1.0);
				QPointF gradStart = intersectBoundingRect(ite, gradientVectorS);
				ite->setGradientVector(gradStart.x(), gradStart.y(), gradEnd.x(), gradEnd.y(), gradStart.x(), gradStart.y(), 1, 0);
				ite->setGradientType(6);
			}
			else if (currentDC.brushStyle == U_BT_PathGradient)
			{
				FPoint newPos = getMinClipF(&ite->PoLine);
				double dx = newPos.x() - oldPos.x();
				double dy = newPos.y() - oldPos.y();
				QPointF cx = currentDC.gradientStart + QPointF(dx, dy);
				FPointArray gpath = currentDC.gradientPath.copy();
				gpath.translate(dx, dy);
				for (int sub = 0; sub < 2; sub++)
				{
					FPointArray points;
					double nearT = 0.5;
					uint psize = gpath.size();
					for (uint a = 0; a < psize-3; a += 4)
					{
						if (gpath.isMarker(a))
						{
							points.setMarker();
							continue;
						}
						FPoint base = gpath.point(a);
						FPoint c1 = gpath.point(a+1);
						FPoint base2 =  gpath.point(a+2);
						FPoint c2 = gpath.point(a+3);
						FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
						FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
						FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
						FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
						FPoint bp1 = (1.0 - nearT) * cn2 + nearT * cn3;
						if ((base == c1) && (base2 == c2))
						{
							points.addPoint(base);
							points.addPoint(c1);
							points.addPoint(bp1);
							points.addPoint(bp1);
							points.addPoint(bp1);
							points.addPoint(bp1);
							points.addPoint(base2);
							points.addPoint(c2);
						}
						else
						{
							points.addPoint(base);
							points.addPoint(cn1);
							points.addPoint(bp1);
							points.addPoint(cn2);
							points.addPoint(bp1);
							points.addPoint(cn3);
							points.addPoint(base2);
							points.addPoint(cn4);
						}
					}
					gpath = points;
				}
				ite->meshGradientPatches.clear();
				FPoint center = FPoint(cx.x(), cx.y());
				QList<VColorStop*> colorStops = currentDC.gradient.colorStops();
				if (colorStops.count() == 2)
				{
					int endC = colorStops.count() - 1;
					meshPoint cP;
					cP.resetTo(center);
					cP.transparency = colorStops[0]->opacity;
					cP.shade = 100;
					cP.colorName = colorStops[0]->name;
					cP.color = colorStops[0]->color;
					for (int poi = 0; poi < gpath.size()-3; poi += 4)
					{
						meshGradientPatch patch;
						patch.BL = cP;
						patch.BR = cP;
						if (gpath.isMarker(poi))
							continue;
						meshPoint tL;
						tL.resetTo(gpath.point(poi));
						tL.controlRight = gpath.point(poi + 1);
						tL.transparency = colorStops[endC]->opacity;
						tL.shade = 100;
						tL.colorName = colorStops[endC]->name;
						tL.color = colorStops[endC]->color;
						meshPoint tR;
						tR.resetTo(gpath.point(poi + 2));
						tR.controlLeft = gpath.point(poi + 3);
						tR.transparency = colorStops[endC]->opacity;
						tR.shade = 100;
						tR.colorName = colorStops[endC]->name;
						tR.color = colorStops[endC]->color;
						patch.TL = tL;
						patch.TR = tR;
						ite->meshGradientPatches.append(patch);
					}
				}
				else
				{
					FPointArray gpath2 = gpath.copy();
					QTransform mm;
					mm.translate(cx.x(), cx.y());
					mm.scale(colorStops[1]->rampPoint, colorStops[1]->rampPoint);
					mm.translate(-cx.x(), -cx.y());
					gpath2.map(mm);
					meshPoint cP;
					cP.resetTo(center);
					cP.transparency = colorStops[0]->opacity;
					cP.shade = 100;
					cP.colorName = colorStops[0]->name;
					cP.color = colorStops[0]->color;
					for (int poi = 0; poi < gpath2.size()-3; poi += 4)
					{
						meshGradientPatch patch;
						patch.BL = cP;
						patch.BR = cP;
						if (gpath.isMarker(poi))
							continue;
						meshPoint tL;
						tL.resetTo(gpath2.point(poi));
						tL.controlRight = gpath2.point(poi + 1);
						tL.transparency = colorStops[1]->opacity;
						tL.shade = 100;
						tL.colorName = colorStops[1]->name;
						tL.color = colorStops[1]->color;
						meshPoint tR;
						tR.resetTo(gpath2.point(poi + 2));
						tR.controlLeft = gpath2.point(poi + 3);
						tR.transparency = colorStops[1]->opacity;
						tR.shade = 100;
						tR.colorName = colorStops[1]->name;
						tR.color = colorStops[1]->color;
						patch.TL = tL;
						patch.TR = tR;
						ite->meshGradientPatches.append(patch);
					}
					for (int cstp = 2; cstp < colorStops.count(); cstp++)
					{
						FPointArray gpath3 = gpath2.copy();
						gpath2 = gpath.copy();
						QTransform mm;
						mm.translate(cx.x(), cx.y());
						mm.scale(colorStops[cstp]->rampPoint, colorStops[cstp]->rampPoint);
						mm.translate(-cx.x(), -cx.y());
						gpath2.map(mm);
						for (int poi = 0; poi < gpath2.size()-3; poi += 4)
						{
							if (gpath.isMarker(poi))
								continue;
							meshGradientPatch patch;
							meshPoint bL;
							bL.resetTo(gpath3.point(poi));
							bL.controlRight = gpath3.point(poi + 1);
							bL.transparency = colorStops[cstp - 1]->opacity;
							bL.shade = 100;
							bL.colorName = colorStops[cstp - 1]->name;
							bL.color = colorStops[cstp - 1]->color;
							patch.BL = bL;
							meshPoint bR;
							bR.resetTo(gpath3.point(poi + 2));
							bR.controlLeft = gpath3.point(poi + 3);
							bR.transparency = colorStops[cstp - 1]->opacity;
							bR.shade = 100;
							bR.colorName = colorStops[cstp - 1]->name;
							bR.color = colorStops[cstp - 1]->color;
							patch.BR = bR;
							meshPoint tL;
							tL.resetTo(gpath2.point(poi));
							tL.controlRight = gpath2.point(poi + 1);
							tL.transparency = colorStops[cstp]->opacity;
							tL.shade = 100;
							tL.colorName = colorStops[cstp]->name;
							tL.color = colorStops[cstp]->color;
							meshPoint tR;
							tR.resetTo(gpath2.point(poi + 2));
							tR.controlLeft = gpath2.point(poi + 3);
							tR.transparency = colorStops[cstp]->opacity;
							tR.shade = 100;
							tR.colorName = colorStops[cstp]->name;
							tR.color = colorStops[cstp]->color;
							patch.TL = tL;
							patch.TR = tR;
							ite->meshGradientPatches.append(patch);
						}
					}
				}
				ite->GrType = 12;
			}
			else if (currentDC.brushStyle == U_BT_TextureFill)
			{
				if (m_Doc->docPatterns.contains(currentDC.patternName))
				{
					ite->setPattern(currentDC.patternName);
					ScPattern pat = m_Doc->docPatterns[currentDC.patternName];
					if ((pat.height < ite->height()) || (pat.width < ite->width()))
					{
						if (currentDC.patternMode == 1)
							ite->setPatternFlip(true, false);
						else if (currentDC.patternMode == 2)
							ite->setPatternFlip(false, true);
						else if (currentDC.patternMode == 3)
							ite->setPatternFlip(true, true);
						else if (currentDC.patternMode == 4)
						{
							double sx = ite->width() / pat.width * 100;
							double sy = ite->height() / pat.height * 100;
							ite->setPatternTransform(sx, sy, 0, 0, 0, 0, 0);
						}
					}
					else
					{
						double sx = ite->width() / pat.width * 100;
						double sy = ite->height() / pat.height * 100;
						ite->setPatternTransform(sx, sy, 0, 0, 0, 0, 0);
					}
					ite->GrType = 8;
				}
			}
		}
		else
		{
			if (currentDC.brushStyle == U_BT_HatchFill)
			{
				switch (currentDC.hatchStyle)
				{
					case U_HSP_Horizontal:
						ite->setHatchParameters(0, 5, 0, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_Vertical:
						ite->setHatchParameters(0, 5, 90, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_ForwardDiagonal:
						ite->setHatchParameters(0, 5, -45, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_BackwardDiagonal:
						ite->setHatchParameters(0, 5, 45, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_LargeGrid:
						ite->setHatchParameters(1, 5, 0, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case U_HSP_DiagonalCross:
						ite->setHatchParameters(1, 5, 45, currentDC.backgroundMode, currentDC.backColor, currentDC.CurrColorFill);
						ite->GrType = 14;
						break;
					case 6:
					case 7:
						ite->setFillColor(currentDC.CurrColorFill);
						break;
					case 8:
					case 9:
						ite->setFillColor(currentDC.CurrColorText);
						break;
					case 10:
					case 11:
						ite->setFillColor(currentDC.backColor);
						break;
					default:
						break;
				}
			}
			else if (currentDC.brushStyle == U_BT_TextureFill)
			{
				ite->setPattern(currentDC.patternName);
				ite->GrType = 8;
			}
		}
	}
	if (inEMFPlus)
	{
		if ((currentDC.brushStyle == U_BT_PathGradient) || (currentDC.brushStyle == U_BT_LinearGradient))
		{
			if ((!currentDC.clipPath.isEmpty()) && (ite->itemType() != PageItem::ImageFrame))
			{
				PageItem *iteG;
				QList<PageItem*> gElements;
				gElements.append(ite);
				tmpSel->clear();
				tmpSel->addItem(ite, true);
				iteG = m_Doc->groupObjectsSelection(tmpSel);
				iteG->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
				double oldX = iteG->xPos();
				double oldY = iteG->yPos();
				double oldW = iteG->width();
				double oldH = iteG->height();
				double oldgW = iteG->groupWidth;
				double oldgH = iteG->groupHeight;
				iteG->PoLine = currentDC.clipPath.copy();
				iteG->PoLine.translate(baseX, baseY);
				FPoint xy = getMinClipF(&iteG->PoLine);
				iteG->setXYPos(xy.x(), xy.y(), true);
				iteG->PoLine.translate(-xy.x(), -xy.y());
				FPoint wh = getMaxClipF(&iteG->PoLine);
				iteG->setWidthHeight(wh.x(),wh.y());
				iteG->groupWidth = oldgW * (iteG->width() / oldW);
				iteG->groupHeight = oldgH * (iteG->height() / oldH);
				double dx = (iteG->xPos() - oldX) / (iteG->width() / iteG->groupWidth);
				double dy = (iteG->yPos() - oldY) / (iteG->height() / iteG->groupHeight);
				for (int em = 0; em < iteG->groupItemList.count(); ++em)
				{
					PageItem* embedded = iteG->groupItemList.at(em);
					embedded->moveBy(-dx, -dy, true);
					m_Doc->setRedrawBounding(embedded);
					embedded->OwnPage = m_Doc->OnPage(embedded);
				}
				iteG->ClipEdited = true;
				iteG->OldB2 = ite->width();
				iteG->OldH2 = ite->height();
				iteG->Clip = FlattenPath(iteG->PoLine, iteG->Segments);
				iteG->updateGradientVectors();
				ite = iteG;
				tmpSel->clear();
			}
		}
	}
	Elements.append(ite);
}

void EmfPlug::handleComment(QDataStream &ds)
{
	quint32 commTyp, dtaSize;
	ds >> dtaSize;
	ds >> commTyp;
	if (commTyp == 0x2B464D45)
		handleEMFPlus(ds, dtaSize);
}

void EmfPlug::handleEllipse(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	QPointF p2 = getPoint(ds, true);
	QRectF BoxDev = QRectF(p1, p2);
	if (inPath)
	{
		QPainterPath painterPath;
		painterPath.addEllipse(BoxDev);
		FPointArray  pointArray;
		pointArray.fromQPainterPath(painterPath);
		currentDC.Coords.setMarker();
		currentDC.Coords += pointArray;
	}
	else
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX, baseY, BoxDev.width(), BoxDev.height(), currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		QTransform mm(1.0, 0.0, 0.0, 1.0, BoxDev.x(), BoxDev.y());
		ite->PoLine.map(mm);
		finishItem(ite);
	}
}

void EmfPlug::handleRectangle(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	QPointF p2 = getPoint(ds, true);
	QRectF BoxDev = QRectF(p1, p2);
	if (inPath)
	{
		QPainterPath painterPath;
		painterPath.addRect(BoxDev);
		FPointArray  pointArray;
		pointArray.fromQPainterPath(painterPath);
		currentDC.Coords.setMarker();
		currentDC.Coords += pointArray;
	}
	else
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX, baseY, BoxDev.width(), BoxDev.height(), currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		QTransform mm(1.0, 0.0, 0.0, 1.0, BoxDev.x(), BoxDev.y());
		ite->PoLine.map(mm);
		finishItem(ite);
	}
}

void EmfPlug::handleRoundRect(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	QPointF p2 = getPoint(ds, true);
	qint32 x1, y1;
	ds >> x1 >> y1;
	QPointF p3 = convertLogical2Pts(QPointF(x1, y1));
	QRectF BoxDev = QRectF(p1, p2);
	if (inPath)
	{
		QPainterPath painterPath;
		painterPath.addRoundedRect(BoxDev, p3.x(), p3.y());
		FPointArray  pointArray;
		pointArray.fromQPainterPath(painterPath);
		currentDC.Coords.setMarker();
		currentDC.Coords += pointArray;
	}
	else
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX, baseY, BoxDev.width(), BoxDev.height(), currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		QTransform mm(1.0, 0.0, 0.0, 1.0, BoxDev.x(), BoxDev.y());
		ite->PoLine.map(mm);
		finishItem(ite);
		if ((p3.x() != 0.0) || (p3.y() != 0.0))
		{
			ite->setCornerRadius(qMax(p3.x(), p3.y()));
			ite->SetFrameRound();
			m_Doc->setRedrawBounding(ite);
		}
	}
}

void EmfPlug::handlePolyBezierTo(QDataStream &ds, bool size)
{
	QRectF BoxDev;
	quint32 countP;
	getPolyInfo(ds, BoxDev, countP);
	for (quint32 a = 0; a < countP; a += 3)
	{
		QPointF p1 = getPoint(ds, size);
		QPointF p2 = getPoint(ds, size);
		QPointF p3 = getPoint(ds, size);
		if (currentDC.Coords.count() == 0)
			currentDC.Coords.svgMoveTo(currentDC.currentPoint.x(), currentDC.currentPoint.y());
		currentDC.Coords.svgCurveToCubic(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
		currentDC.currentPoint = p3;
	}
	if (!inPath)
	{
		if (currentDC.Coords.count() != 0)
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = currentDC.Coords.copy();
			finishItem(ite, false);
			currentDC.Coords.resize(0);
			currentDC.Coords.svgInit();
		}
	}
}

void EmfPlug::handlePolylineTo(QDataStream &ds, bool size)
{
	QRectF bBoxDev;
	quint32 countP;
	getPolyInfo(ds, bBoxDev, countP);
	for (quint32 a = 0; a < countP; a++)
	{
		QPointF p1 = getPoint(ds, size);
		if (currentDC.Coords.count() == 0)
			currentDC.Coords.svgMoveTo(currentDC.currentPoint.x(), currentDC.currentPoint.y());
		currentDC.Coords.svgLineTo(p1.x(), p1.y());
		currentDC.currentPoint = p1;
	}
	if (!inPath)
	{
		if (currentDC.Coords.count() != 0)
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = currentDC.Coords.copy();
			finishItem(ite, false);
			currentDC.Coords.resize(0);
			currentDC.Coords.svgInit();
		}
	}
}

void EmfPlug::handleLineTo(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	if (currentDC.Coords.count() == 0)
	{
		currentDC.Coords.svgInit();
		currentDC.Coords.svgMoveTo(currentDC.currentPoint.x(), currentDC.currentPoint.y());
	}
	currentDC.Coords.svgLineTo(p1.x(), p1.y());
	currentDC.currentPoint = p1;
	if (!inPath)
	{
		if (currentDC.Coords.count() != 0)
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = currentDC.Coords.copy();
			finishItem(ite, false);
			currentDC.Coords.resize(0);
			currentDC.Coords.svgInit();
		}
	}
}

void EmfPlug::handleArc(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	QPointF p2 = getPoint(ds, true);
	QRectF BoxDev = QRectF(p1, p2);
	QPointF st = getPoint(ds, true);
	QPointF en = getPoint(ds, true);
	QLineF stlin = QLineF(BoxDev.center(), st);
	QLineF enlin = QLineF(BoxDev.center(), en);
	FPointArray  pointArray;
	QPainterPath painterPath;
	painterPath.arcMoveTo(BoxDev, stlin.angle());
	if (currentDC.arcDirection)
		painterPath.arcTo(BoxDev, stlin.angle(), enlin.angle() - stlin.angle());
	else
		painterPath.arcTo(BoxDev, stlin.angle(), stlin.angle() - enlin.angle());
	pointArray.fromQPainterPath(painterPath);
	if (pointArray.count() != 0)
	{
		if (inPath)
		{
			currentDC.Coords += pointArray;
			currentDC.currentPoint = en;
		}
		else
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, BoxDev.width(), BoxDev.height(), currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = pointArray.copy();
			finishItem(ite, false);
		}
	}
}

void EmfPlug::handleArcTo(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	QPointF p2 = getPoint(ds, true);
	QRectF BoxDev = QRectF(p1, p2);
	QPointF st = getPoint(ds, true);
	QPointF en = getPoint(ds, true);
	QLineF stlin = QLineF(BoxDev.center(), st);
	QLineF enlin = QLineF(BoxDev.center(), en);
	if (inPath)
	{
		if (enlin.angleTo(stlin) > 180)
		{
		//	currentDC.Coords.svgMoveTo(st.x(), st.y());
			currentDC.Coords.svgArcTo(BoxDev.width() / 2.0, BoxDev.height() / 2.0, 0, enlin.angleTo(stlin) < 180 ? 1 : 0, stlin.angleTo(enlin) > 180 ? 1 : 0, en.x(), en.y());
		}
		else
		{
		//	currentDC.Coords.svgMoveTo(st.x(), st.y());
			currentDC.Coords.svgArcTo(BoxDev.width() / 2.0, BoxDev.height() / 2.0, 0, enlin.angleTo(stlin) > 180 ? 1 : 0, stlin.angleTo(enlin) > 180 ? 1 : 0, en.x(), en.y());
		}
		currentDC.currentPoint = en;
	}
	else
	{
		FPointArray  pointArray;
		QPainterPath painterPath;
		double ang1 = stlin.angleTo(enlin);
		if (currentDC.arcDirection)
		{
			painterPath.arcMoveTo(BoxDev, stlin.angle());
			painterPath.arcTo(BoxDev, stlin.angle(), ang1);
		}
		else
		{
			painterPath.arcMoveTo(BoxDev, stlin.angle());
			painterPath.arcTo(BoxDev, stlin.angle(), -(360 - ang1));
		}
		pointArray.fromQPainterPath(painterPath);
		if (pointArray.count() != 0)
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, BoxDev.width(), BoxDev.height(), currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = pointArray.copy();
			finishItem(ite, false);
		}
	}
}

void EmfPlug::handleChord(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	QPointF p2 = getPoint(ds, true);
	QRectF BoxDev = QRectF(p1, p2);
	QPointF st = getPoint(ds, true);
	QPointF en = getPoint(ds, true);
	QLineF stlin = QLineF(BoxDev.center(), st);
	QLineF enlin = QLineF(BoxDev.center(), en);
	FPointArray  pointArray;
	QPainterPath painterPath;
	QPointF firstPoint;
	double ang1 = stlin.angleTo(enlin);
	if (currentDC.arcDirection)
	{
		painterPath.arcMoveTo(BoxDev, stlin.angle());
		firstPoint = painterPath.currentPosition();
		painterPath.arcTo(BoxDev, stlin.angle(), ang1);
	}
	else
	{
		painterPath.arcMoveTo(BoxDev, stlin.angle());
		firstPoint = painterPath.currentPosition();
		painterPath.arcTo(BoxDev, stlin.angle(), -(360 - ang1));
	}
	painterPath.lineTo(firstPoint);
	pointArray.fromQPainterPath(painterPath);
	if (pointArray.count() != 0)
	{
		if (inPath)
		{
			currentDC.Coords += pointArray;
			currentDC.currentPoint = firstPoint;
		}
		else
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, BoxDev.width(), BoxDev.height(), currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = pointArray.copy();
			finishItem(ite);
		}
	}
}

void EmfPlug::handlePie(QDataStream &ds)
{
	QPointF p1 = getPoint(ds, true);
	QPointF p2 = getPoint(ds, true);
	QRectF BoxDev = QRectF(p1, p2);
	QPointF st = getPoint(ds, true);
	QPointF en = getPoint(ds, true);
	QLineF stlin = QLineF(BoxDev.center(), st);
	QLineF enlin = QLineF(BoxDev.center(), en);
	FPointArray  pointArray;
	QPainterPath painterPath;
	QPointF firstPoint;
	double ang1 = stlin.angleTo(enlin);
	if (currentDC.arcDirection)
	{
		painterPath.arcMoveTo(BoxDev, stlin.angle());
		firstPoint = painterPath.currentPosition();
		painterPath.arcTo(BoxDev, stlin.angle(), ang1);
	}
	else
	{
		painterPath.arcMoveTo(BoxDev, stlin.angle());
		firstPoint = painterPath.currentPosition();
		painterPath.arcTo(BoxDev, stlin.angle(), -(360 - ang1));
	}
	painterPath.lineTo(BoxDev.center());
	painterPath.lineTo(firstPoint);
	pointArray.fromQPainterPath(painterPath);
	if (pointArray.count() != 0)
	{
		if (inPath)
		{
			currentDC.Coords += pointArray;
			currentDC.currentPoint = firstPoint;
		}
		else
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, BoxDev.width(), BoxDev.height(), currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = pointArray.copy();
			finishItem(ite);
		}
	}
}

void EmfPlug::handleSmallText(QDataStream &ds)
{
	qint32 bLeft, bTop, bRight, bBottom;
	quint32 grMode, numChar, txOpts;
	float sx, sy;
	QPointF p1 = getPoint(ds, true);
	if (currentDC.textAlignment &0x0001)
		p1 = currentDC.currentPoint;
	ds >> numChar >> txOpts >> grMode;
	ds >> sx >> sy;
	if (!(txOpts & 0x00000100))
		ds >> bLeft >> bTop >> bRight >> bBottom;
	QString aTxt = "";
	for (quint32 a = 0; a < numChar; a++)
	{
		if (txOpts & 0x00000200)
		{
			quint8 cc;
			ds >> cc;
			aTxt.append(QChar(cc));
		}
		else
		{
			quint16 cc;
			ds >> cc;
			aTxt.append(QChar(cc));
		}
	}
	if (aTxt.isEmpty())
		return;
	FPointArray textPath;
	QPainterPath painterPath;
	QFont font = QFont(currentDC.fontName, currentDC.fontSize);
	font.setPixelSize(currentDC.fontSize);
	painterPath.addText(p1.x(), p1.y(), font, aTxt);
	QFontMetricsF fm(font);
	if (currentDC.textAlignment == 0)
		painterPath.translate(0, fm.ascent());
	if (currentDC.textAlignment & 0x0002)
		painterPath.translate(-fm.width(aTxt), 0);
	else if (currentDC.textAlignment & 0x0006)
		painterPath.translate(-fm.width(aTxt) / 2.0, 0);
	if (currentDC.textAlignment & 0x0008)
		painterPath.translate(0, fm.descent());
	textPath.fromQPainterPath(painterPath);
	if (textPath.size() > 0)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorText, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = textPath.copy();
		finishItem(ite);
		if (currentDC.fontRotation != 0)
			ite->setRotation(-currentDC.fontRotation, true);
	}
	if (currentDC.textAlignment & 0x0001)
	{
		if (currentDC.textAlignment & 0x0002)
			currentDC.currentPoint = p1;
		else if (currentDC.textAlignment & 0x0006)
			currentDC.currentPoint = QPointF(p1.x() + (fm.width(aTxt) / 2.0), p1.y());
		else
			currentDC.currentPoint = QPointF(p1.x() + fm.width(aTxt), p1.y());
	}
}

void EmfPlug::handleText(QDataStream &ds, qint64 posi, bool size)
{
	QString aTxt = "";
	QPainterPath painterPath;
	qint32 bLeft, bTop, bRight, bBottom, oLeft, oTop, oRight, oBottom;
	quint32 grMode, numChar, offTxt, txOpts, offDx;
	float sx, sy;
	ds >> bLeft >> bTop >> bRight >> bBottom;
	ds >> grMode;
	ds >> sx >> sy;
	QPointF p1 = getPoint(ds, true);
	if (currentDC.textAlignment &0x0001)
		p1 = currentDC.currentPoint;
	ds >> numChar >> offTxt >> txOpts;
	ds >> oLeft >> oTop >> oRight >> oBottom;
	ds >> offDx;
	ds.device()->seek(posi + offTxt);
	QFont font = QFont(currentDC.fontName, currentDC.fontSize);
	font.setPixelSize(currentDC.fontSize);
	QFontMetricsF fm(font);
	double aTextWidth = 0;
	if (txOpts & 0x00000010)
	{
		QList<quint32> glyphs;
		for (quint32 a = 0; a < numChar; a++)
		{
			if (size)
			{
				quint16 cc;
				ds >> cc;
				glyphs.append(cc);
			}
			else
			{
				quint8 cc;
				ds >> cc;
				glyphs.append(cc);
			}
		}
		ds.device()->seek(posi + offDx);
		QList<quint32> dxTxt;
		for (quint32 a = 0; a < numChar; a++)
		{
			quint32 cc;
			ds >> cc;
			dxTxt.append(cc);
		}
		QRawFont rFont = QRawFont::fromFont(font);
		double startX = p1.x();
		for (quint32 a = 0; a < numChar; a++)
		{
			QPainterPath gPath = rFont.pathForGlyph(glyphs[a]);
			gPath.translate(startX, p1.y());
			painterPath.addPath(gPath);
			startX += convertLogical2Pts(static_cast<double>(dxTxt[a]));
		}
		aTextWidth = painterPath.boundingRect().width();
	}
	else
	{
		for (quint32 a = 0; a < numChar; a++)
		{
			if (size)
			{
				quint16 cc;
				ds >> cc;
				aTxt.append(QChar(cc));
			}
			else
			{
				quint8 cc;
				ds >> cc;
				aTxt.append(QChar(cc));
			}
		}
		ds.device()->seek(posi + offDx);
		QList<quint32> dxTxt;
		for (quint32 a = 0; a < numChar; a++)
		{
			quint32 cc;
			ds >> cc;
			dxTxt.append(cc);
		}
		if (aTxt.isEmpty())
			return;
		if (font.exactMatch())
		{
			double startX = p1.x();
			for (quint32 a = 0; a < numChar; a++)
			{
				painterPath.addText(startX, p1.y(), font, aTxt.at(a));
				startX += convertLogical2Pts(static_cast<double>(dxTxt[a]));
			}
		}
		else
			painterPath.addText(p1.x(), p1.y(), font, aTxt);
		aTextWidth = fm.width(aTxt);
	}
	if (currentDC.textAlignment == 0)
		painterPath.translate(0, fm.ascent());
	if (currentDC.textAlignment & 0x0002)
		painterPath.translate(-aTextWidth, 0);
	else if (currentDC.textAlignment & 0x0006)
		painterPath.translate(-aTextWidth / 2.0, 0);
	if (currentDC.textAlignment & 0x0008)
		painterPath.translate(0, fm.descent());
	QTransform bm = currentDC.m_WorldMap;
	bm = QTransform(bm.m11(), bm.m12(), bm.m21(), bm.m22(), 0, 0);
	painterPath = bm.map(painterPath);
	FPointArray textPath;
	textPath.fromQPainterPath(painterPath);
	if (textPath.size() > 0)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorText, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = textPath.copy();
		finishItem(ite);
		if (currentDC.fontRotation != 0)
			ite->setRotation(-currentDC.fontRotation, true);
	}
	if (currentDC.textAlignment & 0x0001)
	{
		if (currentDC.textAlignment & 0x0002)
			currentDC.currentPoint = p1;
		else if (currentDC.textAlignment & 0x0006)
			currentDC.currentPoint = QPointF(p1.x() + (aTextWidth / 2.0), p1.y());
		else
			currentDC.currentPoint = QPointF(p1.x() + aTextWidth, p1.y());
	}
}

void EmfPlug::handleImage(qint32 dstX, qint32 dstY, qint32 dstW, qint32 dstH, QImage img)
{
	QTransform bm = currentDC.m_WorldMap;
	if ((currentDC.m_mapMode == 0x07) || (currentDC.m_mapMode == 0x08))
	{
		double ratioX = viewPextendX / static_cast<double>(winPextendX); // Logical --> Device
		double ratioY = viewPextendY / static_cast<double>(winPextendY); // Logical --> Device
		bm = QTransform(bm.m11() * ratioX, bm.m12() * ratioY, bm.m21() * ratioX, bm.m22() * ratioY, bm.dx() * ratioX, bm.dy() * ratioY);
	}
	QPointF p = currentDC.m_WorldMap.map(QPointF(dstX, dstY));
	p = convertLogical2Pts(p);
	QPointF p2 = QPointF(qAbs(dstW), qAbs(dstH));
	QLineF wl = QLineF(0, 0, p2.x(), 0);
	wl = bm.map(wl);
	QLineF hl = QLineF(0, 0, p2.y(), 0);
	hl = bm.map(hl);
	p2 = convertDevice2Pts(QPointF(wl.length(), hl.length()));
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX + p.x(), baseY + p.y(), p2.x(), p2.y(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_Doc->Items->at(z);
	finishItem(ite, false);
	QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_emf_XXXXXX.png");
	tempFile->setAutoRemove(false);
	if (tempFile->open())
	{
		QString fileName = getLongPathName(tempFile->fileName());
		if (!fileName.isEmpty())
		{
			tempFile->close();
			img.save(fileName, "PNG");
			ite->isInlineImage = true;
			ite->isTempFile = true;
			ite->AspectRatio = false;
			ite->ScaleType   = false;
			if (currentDC.clipPath.count() != 0)
			{
				FPointArray cp = currentDC.clipPath.copy();
				cp.translate(baseX, baseY);
				cp.translate(-docX, -docY);
				cp.translate(-ite->xPos(), -ite->yPos());
				ite->PoLine = cp.copy();
				FPoint wh = getMaxClipF(&ite->PoLine);
				ite->setWidthHeight(wh.x(),wh.y());
				ite->setTextFlowMode(PageItem::TextFlowDisabled);
				m_Doc->AdjustItemSize(ite);
				ite->OldB2 = ite->width();
				ite->OldH2 = ite->height();
				ite->updateClip();
			}
			m_Doc->loadPict(fileName, ite);
			ite->AdjustPictScale();
		}
	}
	delete tempFile;
}

void EmfPlug::handlePatternFill(qint32 dstX, qint32 dstY, qint32 dstW, qint32 dstH)
{
	if (currentDC.brushStyle == U_BT_TextureFill)
	{
		QTransform bm = currentDC.m_WorldMap;
		if ((currentDC.m_mapMode == 0x07) || (currentDC.m_mapMode == 0x08))
		{
			double ratioX = viewPextendX / static_cast<double>(winPextendX); // Logical --> Device
			double ratioY = viewPextendY / static_cast<double>(winPextendY); // Logical --> Device
			bm = QTransform(bm.m11() * ratioX, bm.m12() * ratioX, bm.m21() * ratioY, bm.m22() * ratioY, bm.dx() * ratioX, bm.dy() * ratioY);
		}
		QPointF p = currentDC.m_WorldMap.map(QPointF(dstX, dstY));
		p = convertLogical2Pts(p);
		QPointF p2 = QPointF(qAbs(dstW), qAbs(dstH));
		QLineF wl = QLineF(0, 0, p2.x(), 0);
		wl = bm.map(wl);
		QLineF hl = QLineF(0, 0, p2.y(), 0);
		hl = bm.map(hl);
		p2 = convertDevice2Pts(QPointF(wl.length(), hl.length()));
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + p.x(), baseY + p.y(), p2.x(), p2.y(), 0, CommonStrings::None, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		finishItem(ite);
	}
}

QImage EmfPlug::handleDIB(QDataStream &ds, qint64 filePos, quint32 offBitH, quint32 sizeBitH, quint32 offBits, quint32 sizeBits)
{
	QImage img = QImage();
	quint32 hSiz, hCompression, imgSize, xres, yres, colorsUsed, colorsReq;
	qint32 hWidth, hHeight;
	quint16 hPlane, hBitCount;
	QVector<QRgb> colorTbl;
	if ((sizeBitH != 0) && (sizeBits != 0))
	{
		QByteArray headerBits;
		headerBits.resize(sizeBitH);
		ds.device()->seek(filePos + offBitH);
		ds.readRawData(headerBits.data(), sizeBitH);
		if (sizeBitH != 12)
		{
			QDataStream dsH(headerBits);
			dsH.setByteOrder(QDataStream::LittleEndian);
			dsH >> hSiz >> hWidth >> hHeight;
			dsH >> hPlane >> hBitCount;
			dsH >> hCompression;
			dsH >> imgSize >> xres >> yres >> colorsUsed >> colorsReq;
			if (((hBitCount == 8) || (hBitCount == 4) || (hBitCount == 1)) && (hCompression == 0))
			{
				if (colorsUsed == 0)
				{
					if (hBitCount == 8)
						colorsUsed = 256;
					else if (hBitCount == 4)
						colorsUsed = 16;
					else if (hBitCount == 1)
						colorsUsed = 2;
				}
				for (quint32 pa = 0; pa < colorsUsed; pa++)
				{
					quint8 r, g, b, a;
					dsH >> b >> g >> r >> a;
					colorTbl.append(qRgba(r, g, b, 255));
				}
			}
		}
		QByteArray bitsBits;
		bitsBits.resize(sizeBits);
		ds.device()->seek(filePos + offBits);
		ds.readRawData(bitsBits.data(), sizeBits);
		if (hCompression == 0)
		{
			QDataStream dsB(bitsBits);
			dsB.setByteOrder(QDataStream::LittleEndian);
			hWidth = qAbs(hWidth);
			hHeight = qAbs(hHeight);
			img = QImage(hWidth, hHeight, QImage::Format_ARGB32);
			img.fill(0);
			if (hBitCount == 32)
			{
				for (qint32 yy = 0; yy < hHeight; yy++)
				{
					QRgb *dst = (QRgb*)img.scanLine(hHeight - yy - 1);
					for (qint32 xx = 0; xx < hWidth; xx++)
					{
						quint8 r, g, b, a;
						dsB >> a >> b >> g >> r;
						*dst = qRgba(r, g, b, 255);
						dst++;
					}
				}
			}
			else if (hBitCount == 24)
			{
				for (qint32 yy = 0; yy < hHeight; yy++)
				{
					QRgb *dst = (QRgb*)img.scanLine(hHeight - yy - 1);
					for (qint32 xx = 0; xx < hWidth; xx++)
					{
						quint8 r, g, b;
						dsB >> b >> g >> r;
						*dst = qRgba(r, g, b, 255);
						dst++;
					}
					aligntoQuadWord(dsB);
				}
			}
			else if (hBitCount == 16)
			{
				for (qint32 yy = 0; yy < hHeight; yy++)
				{
					QRgb *dst = (QRgb*)img.scanLine(hHeight - yy - 1);
					for (qint32 xx = 0; xx < hWidth; xx++)
					{
						quint16 dt;
						quint8 r, g, b;
						dsB >> dt;
						b = (dt & 0x1F) * 8;
						g = ((dt >> 5) & 0x1F) * 8;
						r = ((dt >> 10) & 0x1F) * 8;
						*dst = qRgba(r, g, b, 255);
						dst++;
					}
					aligntoQuadWord(dsB);
				}
			}
			else if (hBitCount == 8)
			{
				img = QImage(hWidth, hHeight, QImage::Format_Indexed8);
				img.fill(0);
				img.setColorTable(colorTbl);
				for (qint32 yy = 0; yy < hHeight; yy++)
				{
					char *dst = (char*)img.scanLine(hHeight - yy - 1);
					dsB.readRawData(dst, hWidth);
					aligntoQuadWord(dsB);
				}
				img = img.convertToFormat(QImage::Format_ARGB32);
			}
			else if (hBitCount == 4)
			{
				for (qint32 yy = 0; yy < hHeight; yy++)
				{
					QRgb *dst = (QRgb*)img.scanLine(hHeight - yy - 1);
					for (qint32 xx = 0; xx < hWidth; xx += 2)
					{
						quint8 r, rh, rl;
						dsB >> r;
						rh = (r >> 4) & 0xF;
						rl = r & 0xF;
						if (rh < colorTbl.count())
							*dst = colorTbl[rh];
						dst++;
						if (xx == hWidth - 1)
							break;
						if (rl < colorTbl.count())
							*dst = colorTbl[rl];
						dst++;
					}
					aligntoQuadWord(dsB);
				}
			}
			else if (hBitCount == 1)
			{
				img = QImage(hWidth, hHeight, QImage::Format_Mono);
				img.fill(0);
				img.setColorTable(colorTbl);
				int bpl = img.bytesPerLine();
				for (qint32 yy = 0; yy < hHeight; yy++)
				{
					char *dst = (char*)img.scanLine(hHeight - yy - 1);
					dsB.readRawData(dst, bpl);
				}
				img = img.convertToFormat(QImage::Format_ARGB32);
			}
		}
		else if (hCompression == 3)
		{
			QDataStream dsB(bitsBits);
			dsB.setByteOrder(QDataStream::LittleEndian);
			img = QImage(hWidth, hHeight, QImage::Format_ARGB32);
			img.fill(0);
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				QRgb *dst = (QRgb*)img.scanLine(hHeight - yy - 1);
				for (qint32 xx = 0; xx < hWidth; xx++)
				{
					quint8 r, g, b, a;
					dsB >> b >> g >> r >> a;
					*dst = qRgba(r, g, b, 255);
					dst++;
				}
			}
		}
		else if (hCompression == 4)
		{
			img.loadFromData(bitsBits);
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		else if (hCompression == 5)
		{
			img.loadFromData(bitsBits);
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
	}
	return img;
}

void EmfPlug::handleBezier(QDataStream &ds, bool size)
{
	QRectF BoxDev;
	quint32 countP;
	getPolyInfo(ds, BoxDev, countP);
	FPointArray pointsPoly;
	pointsPoly.svgInit();
	QPointF p = getPoint(ds, size);
	if (inPath)
		currentDC.Coords.svgMoveTo(p.x(), p.y());
	else
		pointsPoly.svgMoveTo(p.x(), p.y());
	for (quint32 a = 1; a < countP; a += 3)
	{
		QPointF p1 = getPoint(ds, size);
		QPointF p2 = getPoint(ds, size);
		QPointF p3 = getPoint(ds, size);
		if (inPath)
			currentDC.Coords.svgCurveToCubic(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
		else
			pointsPoly.svgCurveToCubic(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
	}
	if (!inPath)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointsPoly.copy();
		finishItem(ite, false);
	}
}

void EmfPlug::handlePolygon(QDataStream &ds, bool size, bool fill)
{
	if (inPath)
	{
		QRectF BoxDev;
		quint32 countP;
		getPolyInfo(ds, BoxDev, countP);
		FPointArray points = getPolyPoints(ds, countP, size, fill);
		currentDC.Coords += points;
	}
	else
	{
		int z = -1;
		QRectF BoxDev;
		quint32 countP;
		getPolyInfo(ds, BoxDev, countP);
		FPointArray pointsPoly = getPolyPoints(ds, countP, size, fill);
		if (fill)
		{
			if (currentDC.LineW == 0)
				z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
			else
				z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
		}
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointsPoly.copy();
		finishItem(ite, fill);
	}
}

void EmfPlug::handlePolyPolygon(QDataStream &ds, bool size, bool fill)
{
	if (inPath)
	{
		QRectF BoxDev;
		quint32 countP, dummy;
		getPolyInfo(ds, BoxDev, countP);
		ds >> dummy;
		QList<quint32> polyCounts;
		for (quint32 a = 0; a < countP; a++)
		{
			ds >> dummy;
			polyCounts.append(dummy);
		}
		for (quint32 a = 0; a < countP; a++)
		{
			FPointArray points = getPolyPoints(ds, polyCounts[a], size, fill);
			currentDC.Coords += points;
			if (countP > 1)
				currentDC.Coords.setMarker();
		}
	}
	else
	{
		int z = -1;
		QRectF BoxDev;
		quint32 countP, dummy;
		getPolyInfo(ds, BoxDev, countP);
		ds >> dummy;
		QList<quint32> polyCounts;
		for (quint32 a = 0; a < countP; a++)
		{
			ds >> dummy;
			polyCounts.append(dummy);
		}
		FPointArray pointsPoly;
		for (quint32 a = 0; a < countP; a++)
		{
			FPointArray points = getPolyPoints(ds, polyCounts[a], size, fill);
			pointsPoly += points;
			if (countP > 1)
				pointsPoly.setMarker();
		}
		if (fill)
		{
			if (currentDC.LineW == 0)
				z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
			else
				z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, currentDC.CurrColorFill, currentDC.CurrColorStroke, true);
		}
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointsPoly.copy();
		finishItem(ite, fill);
	}
}
void EmfPlug::handlePenDef(quint32 penID, quint32 penStyle, quint32 penWidth, quint32 penColor)
{
	QColor col((QRgb)penColor);
	emfStyle sty;
	sty.styType = U_OT_Pen;
	sty.brushColor = CommonStrings::None;
	sty.penColor = handleColor(col);
	sty.penCap = Qt::RoundCap;
	sty.penJoin = Qt::RoundJoin;
	if ((penStyle & 0x0F) == U_LS_Solid)
		sty.penStyle = Qt::SolidLine;
	else if ((penStyle & 0x0F) == U_LS_Dash)
		sty.penStyle = Qt::DashLine;
	else if ((penStyle & 0x0F) == U_LS_Dot)
		sty.penStyle = Qt::DotLine;
	else if ((penStyle & 0x0F) == U_LS_DashDot)
		sty.penStyle = Qt::DashDotLine;
	else if ((penStyle & 0x0F) == U_LS_DashDotDot)
		sty.penStyle = Qt::DashDotDotLine;
	else if ((penStyle & 0x0F) == U_LS_Custom)
		sty.penStyle = Qt::SolidLine;
	else
		sty.penStyle = Qt::SolidLine;
	if ((penStyle & 0x0F00) == 0x100)
		sty.penCap = Qt::SquareCap;
	if ((penStyle & 0x0F00) == 0x200)
		sty.penCap = Qt::FlatCap;
	if ((penStyle & 0x0F000) == 0x1000)
		sty.penJoin = Qt::BevelJoin;
	if ((penStyle & 0x0F000) == 0x2000)
		sty.penJoin = Qt::MiterJoin;
	if ((penStyle & 0x0F0000) == 0x10000)
		sty.penWidth = convertLogical2Pts(static_cast<double>(penWidth));
	else
		sty.penWidth = convertDevice2Pts(static_cast<double>(penWidth));
	QLineF p = QLineF(0, 0, sty.penWidth, 0);
	p = currentDC.m_WorldMap.map(p);
	sty.penWidth = p.length();
	emfStyleMap.insert(penID, sty);
}

QString EmfPlug::handleColor(QColor col)
{
	ScColor tmp;
	tmp.setColorRGB(col.red(), col.green(), col.blue());
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString tmpName = "FromEMF"+col.name().toUpper();
	QString fNam = m_Doc->PageColors.tryAddColor(tmpName, tmp);
	if (fNam == tmpName)
		importedColors.append(tmpName);
	return fNam;
}

void EmfPlug::handleFillRegion(QDataStream &ds)
{
	quint32 dummy, numObj, countRects;
	ds >> dummy >> dummy >> dummy >> dummy >> dummy;
	ds >> numObj;
	if (emfStyleMap.contains(numObj))
	{
		emfStyle sty = emfStyleMap[numObj];
		if (sty.styType == U_OT_Brush)
		{
			currentDC.CurrColorFill = sty.brushColor;
			currentDC.CurrFillTrans = sty.fillTrans;
			ds >> dummy >> dummy >> countRects;
			ds >> dummy >> dummy >> dummy >> dummy >> dummy;
			QPainterPath pathN;
			for (quint32 a = 0; a < countRects; a++)
			{
				QPointF p1 = getPoint(ds, true);
				QPointF p2 = getPoint(ds, true);
				QPainterPath painterPath;
				painterPath.addRect(QRectF(p1, p2));
				pathN = pathN.united(painterPath);
			}
			FPointArray polyline;
			polyline.fromQPainterPath(pathN, true);
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = polyline.copy();
			finishItem(ite);
		}
	}
}

void EmfPlug::handleFrameRegion(QDataStream &ds)
{
	quint32 dummy, numObj, countRects;
	ds >> dummy >> dummy >> dummy >> dummy >> dummy;
	ds >> numObj >> dummy >> dummy;
	if (emfStyleMap.contains(numObj))
	{
		emfStyle sty = emfStyleMap[numObj];
		if (sty.styType == U_OT_Pen)
		{
			currentDC.CurrColorStroke = sty.penColor;
			currentDC.CurrStrokeTrans = sty.penTrans;
			currentDC.penCap = sty.penCap;
			currentDC.penJoin = sty.penJoin;
			currentDC.penStyle = sty.penStyle;
			currentDC.LineW = sty.penWidth;
			ds >> dummy >> dummy >> countRects;
			ds >> dummy >> dummy >> dummy >> dummy >> dummy;
			QPainterPath pathN;
			for (quint32 a = 0; a < countRects; a++)
			{
				QPointF p1 = getPoint(ds, true);
				QPointF p2 = getPoint(ds, true);
				QPainterPath painterPath;
				painterPath.addRect(QRectF(p1, p2));
				pathN = pathN.united(painterPath);
			}
			FPointArray polyline;
			polyline.fromQPainterPath(pathN, true);
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = polyline.copy();
			finishItem(ite);
		}
	}
}

void EmfPlug::handleSetClipRegion(QDataStream &ds)
{
	quint32 dummy, mode, countRects;
	ds >> dummy >> mode;
	ds >> dummy >> dummy >> countRects;
	ds >> dummy >> dummy >> dummy >> dummy >> dummy;
/*	QPainterPath pathN;
	for (quint32 a = 0; a < countRects; a++)
	{
		QPointF p1 = getPoint(ds, true);
		QPointF p2 = getPoint(ds, true);
		QPainterPath painterPath;
		painterPath.addRect(QRectF(p1, p2));
		pathN = pathN.united(painterPath);
	}
	FPointArray polyline;
	if ((mode == 0) || (currentDC.clipPath.isEmpty()))
	{
		polyline.fromQPainterPath(pathN, true);
		currentDC.clipPath = polyline.copy();
	}
	else
	{
		QPainterPath pathA = currentDC.clipPath.toQPainterPath(true);
		QPainterPath resultPath;
		if (mode == 1)
			resultPath = pathA.intersected(pathN);
		else if (mode == 2)
			resultPath = pathA.united(pathN);
		else if (mode == 3)
		{
			QPainterPath part1 = pathA.subtracted(pathN);
			QPainterPath part2 = pathN.subtracted(pathA);
			resultPath.addPath(part1);
			resultPath.addPath(part2);
		}
		else if (mode == 4)
			resultPath = pathA.subtracted(pathN);
		else if (mode == 5)
			resultPath = pathN;
		if (!resultPath.isEmpty())
		{
			polyline.resize(0);
			polyline.fromQPainterPath(resultPath, true);
			polyline.svgClosePath();
			currentDC.clipPath = polyline.copy();
		}
	}*/
}

void EmfPlug::handleEMFPlus(QDataStream &ds, quint32 dtaSize)
{
	inEMFPlus = true;
	quint16 id2, flagsHL;
	quint8 flagsH, flagsL;
	quint32 size2;
	quint32 dummy;
	quint32 dataSize;
	qint32 xorg, yorg;
	float m11, m12, m21, m22, dx, dy;
	QTransform mm;
	QByteArray emfRecords;
	emfRecords.resize(dtaSize);
	ds.readRawData(emfRecords.data(), dtaSize);
	QDataStream dsEmf(emfRecords);
	dsEmf.setByteOrder(QDataStream::LittleEndian);
	dsEmf.setFloatingPointPrecision(QDataStream::SinglePrecision);
	while (!dsEmf.atEnd())
	{
		qint64 posi2 = dsEmf.device()->pos();
		dsEmf >> id2;
		if ((id2 < 0x4000) || (id2 > 0x403A))
			break;
		dsEmf >> flagsHL;
		flagsL = (flagsHL & 0xFF00) >> 8;
		flagsH = (flagsHL & 0x00FF);
		dsEmf >> size2 >> dataSize;
		switch (id2)
		{
			case U_PMR_HEADER:
				emfPlusDual = (flagsH == 1);
				dsEmf >> dummy >> dummy;
				dsEmf >> EmfPdpiX >> EmfPdpiY;
				break;
			case U_PMR_ENDOFFILE:
				inEMFPlus = false;
				break;
			case U_PMR_GETDC:
				if (emfPlusDual)
					inEMFPlus = false;
				break;
			case U_PMR_OBJECT:
				handleEMPObject(dsEmf, flagsH, flagsL, dataSize);
				break;
			case U_PMR_FILLRECTS:
				handleEMFPFillRects(dsEmf, flagsL);
				break;
			case U_PMR_DRAWRECTS:
				handleEMFPDrawRects(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_FILLPOLYGON:
				handleEMFPFillPolygon(dsEmf, flagsL);
				break;
			case U_PMR_DRAWLINES:
				handleEMFPDrawLines(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_FILLELLIPSE:
				handleEMFPFillEllipse(dsEmf, flagsL);
				break;
			case U_PMR_DRAWELLIPSE:
				handleEMFPDrawEllipse(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_FILLPIE:
				handleEMFPFillPie(dsEmf, flagsL);
				break;
			case U_PMR_DRAWPIE:
				handleEMFPDrawPie(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWARC:
				handleEMFPDrawArc(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_FILLREGION:
				handleEMFPFillRegion(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_FILLPATH:
				handleEMFPFillPath(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWPATH:
				handleEMFPDrawPath(dsEmf, flagsH);
				break;
			case U_PMR_FILLCLOSEDCURVE:
				handleEMFPFillClosedCurve(dsEmf, flagsL);
				break;
			case U_PMR_DRAWCLOSEDCURVE:
				handleEMFPDrawClosedCurve(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWCURVE:
				handleEMFPDrawCurve(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWBEZIERS:
				handleEMFPDrawBezier(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWIMAGE:
				handleEMFPDrawImage(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWIMAGEPOINTS:
				handleEMFPDrawImagePoints(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWSTRING:
				handleEMFPDrawString(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_SETRENDERINGORIGIN:
				dsEmf >> xorg >> yorg;
				currentDC.originEMFP = convertDevice2Pts(QPointF(xorg, yorg));
				break;
			case U_PMR_SETCOMPOSITINGMODE:
				currentDC.alphaOn = (flagsH == 0);
				break;
			case U_PMR_SAVE:
				dsEmf >> dummy;
				dcStackEMP.insert(dummy, currentDC);
				break;
			case U_PMR_RESTORE:
				dsEmf >> dummy;
				if (dcStackEMP.contains(dummy))
					currentDC = dcStackEMP[dummy];
				break;
			case U_PMR_SETWORLDTRANSFORM:
				dsEmf >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
				currentDC.m_WorldMapEMFP = QTransform(m11, m12, m21, m22, dx, dy);
				break;
			case U_PMR_RESETWORLDTRANSFORM:
				currentDC.m_WorldMapEMFP = QTransform();
				break;
			case U_PMR_MULTIPLYWORLDTRANSFORM:
				dsEmf >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
				if (flagsL & 0x20)
					currentDC.m_WorldMapEMFP = currentDC.m_WorldMapEMFP * QTransform(m11, m12, m21, m22, dx, dy);
				else
					currentDC.m_WorldMapEMFP = QTransform(m11, m12, m21, m22, dx, dy) * currentDC.m_WorldMapEMFP;
				break;
			case U_PMR_TRANSLATEWORLDTRANSFORM:
				dsEmf >> dx >> dy;
				mm = QTransform();
				mm.translate(dx, dy);
				if (flagsL & 0x20)
					currentDC.m_WorldMapEMFP = currentDC.m_WorldMapEMFP * mm;
				else
					currentDC.m_WorldMapEMFP = mm * currentDC.m_WorldMapEMFP;
				break;
			case U_PMR_SCALEWORLDTRANSFORM:
				dsEmf >> m11 >> m12;
				mm = QTransform();
				mm.scale(m11, m12);
				if (flagsL & 0x20)
					currentDC.m_WorldMapEMFP = currentDC.m_WorldMapEMFP * mm;
				else
					currentDC.m_WorldMapEMFP = mm * currentDC.m_WorldMapEMFP;
				break;
			case U_PMR_ROTATEWORLDTRANSFORM:
				dsEmf >> m11;
				mm = QTransform();
				mm.rotate(m11);
				if (flagsL & 0x20)
					currentDC.m_WorldMapEMFP = currentDC.m_WorldMapEMFP * mm;
				else
					currentDC.m_WorldMapEMFP = mm * currentDC.m_WorldMapEMFP;
				break;
			case U_PMR_SETPAGETRANSFORM:
				currentDC.emfPlusUnit = flagsH;
				dsEmf >> emfPlusScale;
				break;
			case U_PMR_OFFSETCLIP:
				if (!currentDC.clipPath.isEmpty())
				{
					double dx = getEMFPDistance(dsEmf, false);
					double dy = getEMFPDistance(dsEmf, false);
					currentDC.clipPath.translate(dx, dy);
				}
				break;
			case U_PMR_RESETCLIP:
				currentDC.clipPath.resize(0);
				currentDC.clipPath.svgInit();
				break;
			case U_PMR_SETCLIPRECT:
				handleEMFPSetClipRect(dsEmf, flagsL);
				break;
			case U_PMR_SETCLIPREGION:
				handleEMFPSetClipRegion(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_SETCLIPPATH:
				handleEMFPSetClipPath(dsEmf, flagsL, flagsH);
				break;
			case U_PMR_DRAWDRIVERSTRING:
				{
					qDebug() << "\tU_PMR_DRAWDRIVERSTRING";
				}
				break;
			case U_PMR_STROKEFILLPATH:
				{
					qDebug() << "\tU_PMR_STROKEFILLPATH";
				}
				break;
			case U_PMR_SERIALIZABLEOBJECT:
				handleEMFPSerializableObject(dsEmf);
				break;
			case U_PMR_BEGINCONTAINER:
			case U_PMR_BEGINCONTAINERNOPARAMS:
			case U_PMR_ENDCONTAINER:
			case U_PMR_COMMENT:
			case U_PMR_SETANTIALIASMODE:
			case U_PMR_SETTEXTRENDERINGHINT:
			case U_PMR_SETTEXTCONTRAST:
			case U_PMR_SETINTERPOLATIONMODE:
			case U_PMR_SETPIXELOFFSETMODE:
			case U_PMR_SETCOMPOSITINGQUALITY:
			case U_PMR_SETTSGRAPHICS:
			case U_PMR_SETTSCLIP:
			case U_PMR_MULTIFORMATSTART:
			case U_PMR_MULTIFORMATSECTION:
			case U_PMR_MULTIFORMATEND:
			case U_PMR_CLEAR:
			//	qDebug() << "\tUnsupported Op-Code" << id2;
				break;
			default:
				qDebug() << "\tUnknown Op-Code" << id2;
				break;
		}
		dsEmf.device()->seek(posi2 + size2);
	}
}

void EmfPlug::handleEMPObject(QDataStream &ds, quint8 flagsH, quint8 flagsL, quint32 dataSize)
{
	quint16 id = flagsH;
	quint16 type = flagsL & 0x7F;
	quint32 totalSize;
	bool cont = (flagsL & 0x80);
	bool first = true;
	if (!cont)
	{
		m_ObjSize = 0;
		m_currObjSize = 0;
	}
	else
	{
		if (m_ObjSize != 0)
			first = false;
		if (m_objID != id)
			first = true;
		ds >> totalSize;
		m_ObjSize = totalSize;
	}
	if (type == U_OT_Brush)
		m_currObjSize += handleEMPBrush(ds, id, first, cont, dataSize);
	else if (type == U_OT_Pen)
		handleEMPPen(ds, id);
	else if (type == U_OT_Path)
		handleEMPPath(ds, id);
	else if (type == U_OT_Region)
		handleEMPRegion(ds, id);
	else if (type == U_OT_Image)
	{
		quint32 lenS = 0;
		if (cont)
			lenS = 4;
		m_currObjSize += handleEMPImage(ds, id, first, cont, dataSize - lenS);
	}
	else if (type == U_OT_Font)
		handleEMPFont(ds, id);
	else if (type == U_OT_StringFormat)
		handleEMPSFormat(ds, id);
	else if (type == U_OT_CustomLineCap)
		handleEMPLineCap(ds, id);
	if (m_currObjSize >= totalSize)
	{
		m_ObjSize = 0;
		m_currObjSize = 0;
	}
	m_objID = id;
}

quint32 EmfPlug::handleEMPBrush(QDataStream &ds, quint16 id, bool first, bool cont, quint32 dataSize)
{
	emfStyle sty;
	quint32 retVal = 0;
	if (!first)
	{
		quint32 lenS = 0;
		if (cont)
			lenS = 4;
		retVal += getImageData(ds, id, first, cont, dataSize - lenS, sty);
		return retVal;
	}
	quint32 dummy;
	quint32 brushType;
	ds >> dummy;
	ds >> brushType;
	if (brushType == U_BT_SolidColor)
	{
		quint32 brush;
		ds >> brush;
		quint8 r = brush & 0xFF;
		quint8 g = (brush >> 8) & 0xFF;
		quint8 b = (brush >> 16) & 0xFF;
		quint8 a = (brush >> 24) & 0xFF;
		QColor col(b, g, r, a);
		sty.brushColor = handleColor(col);
		sty.penColor = CommonStrings::None;
		sty.fillTrans = 1.0 - col.alphaF();
		sty.brushStyle = U_BT_SolidColor;
		sty.hatchStyle = 0;
	}
	else if (brushType == U_BT_HatchFill)
	{
		quint32 hatchStyle, fgColor, bgColor;
		ds >> hatchStyle >> fgColor >> bgColor;
		quint8 r = fgColor & 0xFF;
		quint8 g = (fgColor >> 8) & 0xFF;
		quint8 b = (fgColor >> 16) & 0xFF;
		quint8 a = (fgColor >> 24) & 0xFF;
		QColor col(b, g, r, a);
		r = bgColor & 0xFF;
		g = (bgColor >> 8) & 0xFF;
		b = (bgColor >> 16) & 0xFF;
		a = (bgColor >> 24) & 0xFF;
		QColor col2(b, g, r, a);
		sty.brushColor = handleColor(col);
		sty.fillTrans = 1.0 - col.alphaF();
		sty.penColor = handleColor(col2);
		sty.penTrans = 1.0 - col2.alphaF();
		if (sty.penTrans > 0.5)
			sty.penColor = CommonStrings::None;
		sty.hatchStyle = hatchStyle;
		sty.brushStyle = U_BT_HatchFill;
	}
	else if (brushType == U_BT_TextureFill)
	{
		if (first)
		{
			quint32 lenS = 16;
			if (cont)
				lenS += 4;
			quint32 gFlags, wrap;
			ds >> gFlags >> wrap;
			bool mTrans = (gFlags & 0x00000002);
			if (mTrans)
			{
				lenS += 24;
				float m11, m12, m21, m22, dx, dy;
				ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
				QLineF lin = QLineF(0, 0, 1, 0);
				lin = QTransform(m11, m12, m21, m22, dx, dy).map(lin);
				sty.gradientAngle = lin.angle();
			}
			sty.brushStyle = U_BT_TextureFill;
			sty.patternMode = wrap;
			retVal = getImageData(ds, id, first, cont, dataSize - lenS, sty);
		}
	}
	else if (brushType == U_BT_PathGradient)
	{
		quint32 gFlags, wrap, startCol, cCount, endCol;
		ds >> gFlags >> wrap;
		ds >> startCol;
		QPointF p1 = getEMFPPoint(ds, false);
		ds >> cCount;
		for (quint32 a = 0; a < cCount; a++)
		{
			ds >> endCol;
		}
		bool mPath  = (gFlags & 0x00000001);
		bool mTrans = (gFlags & 0x00000002);
		bool preCol = (gFlags & 0x00000004);
		bool blFacH = (gFlags & 0x00000008);
		sty.brushStyle = U_BT_PathGradient;
		sty.gradientStart = p1;
		if (mPath)
		{
			quint32 pCount;
			ds >> pCount;
			qint64 ppos = ds.device()->pos();
			FPointArray polyline = getEMPPathData(ds);
			sty.gradientPath = polyline.copy();
			ds.device()->seek(ppos + pCount);
		}
		else
		{
			quint32 pCount;
			ds >> pCount;
			QPolygonF points = getEMFPCurvePoints(ds, 0, pCount);
			QPainterPath path;
			GdipAddPathClosedCurve(path, points, 0);
			FPointArray polyline;
			polyline.fromQPainterPath(path, true);
			sty.gradientPath = polyline.copy();
		}
		if (mTrans)
		{
			float m11, m12, m21, m22, dx, dy;
			ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
			QLineF lin = QLineF(0, 0, 1, 0);
			lin = QTransform(m11, m12, m21, m22, dx, dy).map(lin);
			sty.gradientAngle = lin.angle() + 45;
		}
		if (blFacH && !preCol)
		{
			quint32 cCount;
			ds >> cCount;
			ds.skipRawData(8 * cCount);
		}
		sty.gradient = VGradient(VGradient::linear);
		sty.gradient.clearStops();
		sty.gradient.setRepeatMethod(VGradient::pad);
		if (preCol)
		{
			quint32 cCount;
			ds >> cCount;
			QList<float> posit;
			QList<quint32> facts;
			for (quint32 c = 0; c < cCount; c++)
			{
				float fact;
				ds >> fact;
				posit.append(fact);
			}
			for (quint32 c = 0; c < cCount; c++)
			{
				quint32 fact;
				ds >> fact;
				facts.append(fact);
			}
			for (quint32 c = 0; c < cCount; c++)
			{
				quint8 r = facts[c] & 0xFF;
				quint8 g = (facts[c] >> 8) & 0xFF;
				quint8 b = (facts[c] >> 16) & 0xFF;
				quint8 a = (facts[c] >> 24) & 0xFF;
				QColor col(b, g, r, a);
				QString stColor = handleColor(col);
				sty.gradient.addStop(col, 1.0 - posit[c], 0.5, col.alphaF(), stColor, 100);
			}
		}
		else
		{
			quint8 r = startCol & 0xFF;
			quint8 g = (startCol >> 8) & 0xFF;
			quint8 b = (startCol >> 16) & 0xFF;
			quint8 a = (startCol >> 24) & 0xFF;
			QColor col(b, g, r, a);
			QString stColor = handleColor(col);
			sty.gradient.addStop(col, 0.0, 0.5, col.alphaF(), stColor, 100);
			r = endCol & 0xFF;
			g = (endCol >> 8) & 0xFF;
			b = (endCol >> 16) & 0xFF;
			a = (endCol >> 24) & 0xFF;
			QColor col2(b, g, r, a);
			QString enColor = handleColor(col2);
			sty.gradient.addStop(col2, 1.0, 0.5, col2.alphaF(), enColor, 100);
		}
	}
	else if (brushType == U_BT_LinearGradient)
	{
		quint32 gFlags, wrap, startCol, endCol;
		ds >> gFlags >> wrap;
		QPolygonF rect = getEMFPRect(ds, false);
		ds >> startCol >> endCol;
		ds >> dummy >> dummy;
		bool mTrans = (gFlags & 0x00000002);
		bool preCol = (gFlags & 0x00000004);
		bool blFacH = (gFlags & 0x00000008);
		bool blFacV = (gFlags & 0x00000010);
		sty.brushStyle = U_BT_LinearGradient;
		sty.gradientStart = rect[0];
		sty.gradientEnd = rect[2];
		if (mTrans)
		{
			float m11, m12, m21, m22, dx, dy;
			ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
			QLineF lin = QLineF(rect[0], rect[2]);
			lin = QTransform(m11, m12, m21, m22, dx, dy).map(lin);
			sty.gradientAngle = lin.angle() + 45;
		}
		if ((blFacH || blFacV) && !preCol)
		{
			quint32 cCount;
			ds >> cCount;
			ds.skipRawData(8 * cCount);
		}
		sty.gradient = VGradient(VGradient::linear);
		sty.gradient.clearStops();
		sty.gradient.setRepeatMethod(VGradient::pad);
		if (preCol)
		{
			quint32 cCount;
			ds >> cCount;
			QList<float> posit;
			QList<quint32> facts;
			for (quint32 c = 0; c < cCount; c++)
			{
				float fact;
				ds >> fact;
				posit.append(fact);
			}
			for (quint32 c = 0; c < cCount; c++)
			{
				quint32 fact;
				ds >> fact;
				facts.append(fact);
			}
			for (quint32 c = 0; c < cCount; c++)
			{
				quint8 r = facts[c] & 0xFF;
				quint8 g = (facts[c] >> 8) & 0xFF;
				quint8 b = (facts[c] >> 16) & 0xFF;
				quint8 a = (facts[c] >> 24) & 0xFF;
				QColor col(b, g, r, a);
				QString stColor = handleColor(col);
				sty.gradient.addStop(col, posit[c], 0.5, col.alphaF(), stColor, 100);
			}
		}
		else
		{
			quint8 r = startCol & 0xFF;
			quint8 g = (startCol >> 8) & 0xFF;
			quint8 b = (startCol >> 16) & 0xFF;
			quint8 a = (startCol >> 24) & 0xFF;
			QColor col(b, g, r, a);
			QString stColor = handleColor(col);
			sty.gradient.addStop(col, 1.0, 0.5, col.alphaF(), stColor, 100);
			r = endCol & 0xFF;
			g = (endCol >> 8) & 0xFF;
			b = (endCol >> 16) & 0xFF;
			a = (endCol >> 24) & 0xFF;
			QColor col2(b, g, r, a);
			QString enColor = handleColor(col2);
			sty.gradient.addStop(col2, 0.0, 0.5, col2.alphaF(), enColor, 100);
		}
	}
	sty.styType = U_OT_Brush;
	emfStyleMapEMP.insert(id, sty);
	return retVal;
}

void EmfPlug::handleEMPPen(QDataStream &ds, quint16 id)
{
	emfStyle sty;
	quint32 dummy;
	quint32 penData, penUnit;
	float penWidth;
	ds >> dummy;
	ds >> dummy;
	ds >> penData >> penUnit >> penWidth;
	sty.penCap = Qt::RoundCap;
	sty.penJoin = Qt::RoundJoin;
	sty.penStyle = Qt::SolidLine;
	if (penData & U_PD_Transform)
	{
		float m11, m12, m21, m22, dx, dy;
		ds >> m11 >> m12 >> m21 >> m22 >> dx >> dy;
	}
	if (penData & U_PD_StartCap)
	{
		qint32 startCap;
		ds >> startCap;
		if (startCap == U_LCT_Square)
			sty.penCap = Qt::SquareCap;
		if (startCap == U_LCT_Flat)
			sty.penCap = Qt::FlatCap;
		else if (startCap == U_LCT_Round)
			sty.penCap = Qt::RoundCap;
		else
			sty.penCap = Qt::RoundCap;
	}
	if (penData & U_PD_EndCap)
	{
		qint32 endCap;
		ds >> endCap;
		if (endCap == U_LCT_Square)
			sty.penCap = Qt::SquareCap;
		if (endCap == U_LCT_Flat)
			sty.penCap = Qt::FlatCap;
		else if (endCap == U_LCT_Round)
			sty.penCap = Qt::RoundCap;
		else
			sty.penCap = Qt::RoundCap;
	}
	if (penData & U_PD_Join)
	{
		qint32 join;
		ds >> join;
		if (join == U_LJT_Bevel)
			sty.penJoin = Qt::BevelJoin;
		else if (join == U_LJT_Miter)
			sty.penJoin = Qt::MiterJoin;
		else if (join == U_LJT_Round)
			sty.penJoin = Qt::RoundJoin;
		else
			sty.penJoin = Qt::RoundJoin;
	}
	if (penData & U_PD_MiterLimit)
	{
		float data;
		ds >> data;
	}
	if (penData & U_PD_LineStyle)
	{
		qint32 penStyle;
		ds >> penStyle;
		if ((penStyle) == U_LS_Solid)
			sty.penStyle = Qt::SolidLine;
		else if ((penStyle) == U_LS_Dash)
			sty.penStyle = Qt::DashLine;
		else if ((penStyle) == U_LS_Dot)
			sty.penStyle = Qt::DotLine;
		else if ((penStyle) == U_LS_DashDot)
			sty.penStyle = Qt::DashDotLine;
		else if ((penStyle) == U_LS_DashDotDot)
			sty.penStyle = Qt::DashDotDotLine;
		else if ((penStyle) == 5)
			sty.penStyle = Qt::SolidLine;
		else
			sty.penStyle = Qt::SolidLine;
	}
	if (penData & U_PD_DLCap)
	{
		qint32 data;
		ds >> data;
	}
	if (penData & U_PD_DLOffset)
	{
		float data;
		ds >> data;
		sty.dashOffset = data;
	}
	if (penData & U_PD_DLData)
	{
		quint32 data;
		ds >> data;
		for (quint32 a = 0; a < data; a++)
		{
			float dData;
			ds >> dData;
			sty.dashArray.append(dData);
		}
	}
	if (penData & U_PD_NonCenter)
	{
		float data;
		ds >> data;
	//	qDebug() << QString("\n\t\t\tPenAlignment");
	}
	if (penData & U_PD_CLData)
	{
		quint32 data;
		ds >> data;
		for (quint32 a = 0; a < data; a++)
		{
			float dData;
			ds >> dData;
		}
	//	qDebug() << QString("\n\t\t\tCompoundLine");
	}
	if (penData & U_PD_CustomStartCap)
	{
		quint32 data;
		ds >> data;
		ds.skipRawData(data);
	//	qDebug() << QString("\n\t\t\tCustomStartCap");
	}
	if (penData & U_PD_CustomEndCap)
	{
		quint32 data;
		ds >> data;
		ds.skipRawData(data);
	//	qDebug() << QString("\n\t\t\tCustomEndCap");
	}
	quint32 brushType;
	ds >> dummy;
	ds >> brushType;
	if (brushType == U_BT_SolidColor)
	{
		quint32 brush;
		ds >> brush;
		quint8 r = brush & 0xFF;
		quint8 g = (brush >> 8) & 0xFF;
		quint8 b = (brush >> 16) & 0xFF;
		quint8 a = (brush >> 24) & 0xFF;
		QColor col(b, g, r, a);
		sty.penColor = handleColor(col);
		sty.penTrans = 1.0 - col.alphaF();
	}
	else
		sty.penColor = "Black";
	sty.styType = U_OT_Pen;
	sty.brushColor = CommonStrings::None;
	if ((penUnit == U_UT_World) || (penUnit == U_UT_Display))
		sty.penWidth = convertEMFPLogical2Pts(penWidth, currentDC.emfPlusUnit);
	else
		sty.penWidth = convertEMFPLogical2Pts(penWidth, penUnit);
	emfStyleMapEMP.insert(id, sty);
}

void EmfPlug::handleEMPPath(QDataStream &ds, quint16 id)
{
	FPointArray polyline = getEMPPathData(ds);
	if (polyline.count() > 0)
	{
		emfStyle sty;
		sty.styType = U_OT_Path;
		sty.Coords = polyline.copy();
		emfStyleMapEMP.insert(id, sty);
	}
}

void EmfPlug::handleEMPRegion(QDataStream &ds, quint16 id)
{
	emfStyle sty;
	sty.styType = U_OT_Region;
	quint32 nPoints, rgnType, dummy;
	ds >> dummy;
	ds >> nPoints;
	ds >> rgnType;
	if (rgnType <= U_RNDT_Complement)
	{
		QPainterPath pathL, pathR, resultPath;
		quint32 rgnTypeL, rgnTypeR;
		ds >> rgnTypeL;
		if (rgnTypeL == U_RNDT_Rect)
		{
			QPolygonF rect = getEMFPRect(ds, false);
			pathL.addPolygon(rect);
		}
		else if (rgnTypeL == U_RNDT_Path)
		{
			quint32 rLen;
			ds >> rLen;
			qint64 ppos = ds.device()->pos();
			FPointArray polyline = getEMPPathData(ds);
			ds.device()->seek(ppos + rLen);
			pathL = polyline.toQPainterPath(true);
		}
		ds >> rgnTypeR;
		if (rgnTypeR == U_RNDT_Rect)
		{
			QPolygonF rect = getEMFPRect(ds, false);
			pathR.addPolygon(rect);
		}
		else if (rgnTypeR == U_RNDT_Path)
		{
			quint32 rLen;
			ds >> rLen;
			qint64 ppos = ds.device()->pos();
			FPointArray polyline = getEMPPathData(ds);
			ds.device()->seek(ppos + rLen);
			pathR = polyline.toQPainterPath(true);
		}
		if (rgnType == U_RNDT_And)
			resultPath = pathL.intersected(pathR);
		else if (rgnType == U_RNDT_Or)
			resultPath = pathL.united(pathR);
		else if (rgnType == U_RNDT_Exclude)
		{
			QPainterPath part1 = pathL.subtracted(pathR);
			QPainterPath part2 = pathR.subtracted(pathL);
			resultPath.addPath(part1);
			resultPath.addPath(part2);
		}
		if (!resultPath.isEmpty())
		{
			FPointArray polyline;
			polyline.resize(0);
			polyline.fromQPainterPath(resultPath, true);
			polyline.svgClosePath();
			sty.Coords = polyline.copy();
			emfStyleMapEMP.insert(id, sty);
		}
	}
	else
	{
		if (rgnType == U_RNDT_Rect)
		{
			QPolygonF rect = getEMFPRect(ds, false);
			FPointArray polyline;
			polyline.resize(0);
			polyline.svgInit();
			polyline.svgMoveTo(rect[0].x(), rect[0].y());
			polyline.svgLineTo(rect[1].x(), rect[1].y());
			polyline.svgLineTo(rect[2].x(), rect[2].y());
			polyline.svgLineTo(rect[3].x(), rect[3].y());
			polyline.svgClosePath();
			sty.Coords = polyline.copy();
			emfStyleMapEMP.insert(id, sty);
		}
		else if (rgnType == U_RNDT_Path)
		{
			quint32 rLen;
			ds >> rLen;
			qint64 ppos = ds.device()->pos();
			FPointArray polyline = getEMPPathData(ds);
			ds.device()->seek(ppos + rLen);
			sty.Coords = polyline.copy();
			emfStyleMapEMP.insert(id, sty);
		}
	}
}

quint32 EmfPlug::handleEMPImage(QDataStream &ds, quint16 id, bool first, bool cont, quint32 dataSize)
{
	emfStyle sty;
	sty.styType = U_OT_Image;
	quint32 retVal = getImageData(ds, id, first, cont, dataSize, sty);
	if (first)
		emfStyleMapEMP.insert(id, sty);
	return retVal;
}

quint32 EmfPlug::getImageData(QDataStream &ds, quint16 id, bool first, bool cont, quint32 dataSize, emfStyle &sty)
{
	quint32 retVal = 0;
	if (first)
	{
		quint32 dataV, dummy;
		ds >> dummy;
		ds >> dataV;
		if (dataV == U_IDT_Bitmap)
		{
			qint32 w, h;
			quint32 pixelFormat, imgType;
			ds >> w >> h >> dummy;
			ds >> pixelFormat >> imgType;
			sty.MetaFile = false;
			sty.imageType = imgType;
			sty.imageWidth = w;
			sty.imageHeight = h;
			sty.imagePixelFormat = pixelFormat;
			sty.imageData.resize(dataSize - 28);
			retVal = ds.readRawData(sty.imageData.data(), dataSize - 28);
		}
		else if (dataV == U_IDT_Metafile)
		{
			quint32 imgType, imgSize;
			ds >> imgType >> imgSize;
			if (imgType == U_MDT_WmfPlaceable)
			{
				QByteArray hea;
				hea.resize(22);
				ds.readRawData(hea.data(), 22);
				ds.skipRawData(2);
				QByteArray dta;
				dta.resize(dataSize - 40);
				retVal = ds.readRawData(dta.data(), dataSize - 40);
				retVal += 24;
				sty.imageData = hea;
				sty.imageData += dta;
			}
			else
			{
				sty.imageData.resize(dataSize - 16);
				retVal = ds.readRawData(sty.imageData.data(), dataSize - 16);
			}
			sty.imageType = imgType;
			sty.MetaFile = true;
		}
	}
	else
	{
		if (emfStyleMapEMP.contains(id))
		{
			QByteArray hea;
			hea.resize(dataSize);
			retVal = ds.readRawData(hea.data(), dataSize);
			emfStyleMapEMP[id].imageData += hea;
		}
	}
	return retVal;
}

void EmfPlug::handleEMPFont(QDataStream &ds, quint16 id)
{
	quint32 dummy, unit, flags, length;
	float emSize;
	ds >> dummy;
	ds >> emSize;
	ds >> unit >> flags >> dummy >> length;
	QString fontName = "";
	for (quint32 a = 0; a < length; a++)
	{
		quint16 cc;
		ds >> cc;
		fontName.append(QChar(cc));
	}
	emfStyle sty;
	sty.styType = U_OT_Font;
	sty.fontSize = emSize;
	sty.fontName = fontName;
	sty.fontUnit = unit;
	emfStyleMapEMP.insert(id, sty);
}

void EmfPlug::handleEMPSFormat(QDataStream &ds, quint16 id)
{
	quint32 dummy, flags, hAlign, vAlign;
	ds >> dummy >> flags >> dummy >> hAlign >> vAlign;
	emfStyle sty;
	sty.styType = U_OT_StringFormat;
	sty.hAlign = hAlign;
	sty.vAlign = vAlign;
	sty.verticalText = (flags & 0x00000002);
	emfStyleMapEMP.insert(id, sty);
}

void EmfPlug::handleEMPLineCap(QDataStream &ds, quint16 id)
{
	qDebug() << "\t\tLine Cap";
}

void EmfPlug::handleEMFPFillClosedCurve(QDataStream &ds, quint8 flagsL)
{
	quint32 brushID, count;
	float tension;
	ds >> brushID;
	ds >> tension;
	ds >> count;
	bool directBrush = (flagsL & 0x80);
	currentDC.fillRule = !(flagsL & 0x20);
	getEMFPBrush(brushID, directBrush);
	QPolygonF points = getEMFPCurvePoints(ds, flagsL, count);
	QPainterPath path;
	GdipAddPathClosedCurve(path, points, tension);
	FPointArray polyline;
	polyline.fromQPainterPath(path);
	if (polyline.size() > 3)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = polyline.copy();
		finishItem(ite);
	}
}

void EmfPlug::handleEMFPFillEllipse(QDataStream &ds, quint8 flagsL)
{
	quint32 brushID;
	ds >> brushID;
	bool directBrush = (flagsL & 0x80);
	bool compressedRects = (flagsL & 0x40);
	getEMFPBrush(brushID, directBrush);
	QPointF p = getEMFPPoint(ds, compressedRects);
	double w = getEMFPDistance(ds, compressedRects);
	double h = getEMFPDistance(ds, compressedRects);
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX, baseY, w, h, 0, currentDC.CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_Doc->Items->at(z);
	QTransform mm(1.0, 0.0, 0.0, 1.0, p.x(), p.y());
	ite->PoLine.map(mm);
	finishItem(ite);
}

void EmfPlug::handleEMFPFillPath(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 brushID;
	ds >> brushID;
	bool directBrush = (flagsL & 0x80);
	getEMFPBrush(brushID, directBrush);
	if (emfStyleMapEMP.contains(flagsH))
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = emfStyleMapEMP[flagsH].Coords.copy();
		finishItem(ite);
	}
}

void EmfPlug::handleEMFPFillPie(QDataStream &ds, quint8 flagsL)
{
	quint32 brushID;
	float startA, sweepA;
	ds >> brushID;
	ds >> startA >> sweepA;
	bool directBrush    = (flagsL & 0x80);
	bool rectCompressed = (flagsL & 0x40);
	getEMFPBrush(brushID, directBrush);
	QRectF rect = getEMFPRect(ds, rectCompressed).boundingRect();
	FPointArray  pointArray;
	QPainterPath painterPath;
	painterPath.arcMoveTo(rect, -startA);
	QPointF firstPoint = painterPath.currentPosition();
	painterPath.arcTo(rect, -startA, -sweepA);
	painterPath.lineTo(rect.center());
	painterPath.lineTo(firstPoint);
	pointArray.fromQPainterPath(painterPath);
	if (pointArray.size() > 3)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointArray.copy();
		finishItem(ite);
	}
}

void EmfPlug::handleEMFPFillPolygon(QDataStream &ds, quint8 flagsL)
{
	quint32 brushID, count;
	ds >> brushID >> count;
	bool directBrush         = (flagsL & 0x80);
	bool compressedPoints    = (flagsL & 0x40);
	bool relativeCoordinates = (flagsL & 0x08);
	getEMFPBrush(brushID, directBrush);
	if (!relativeCoordinates)
	{
		bool bFirst = true;
		FPointArray polyline;
		polyline.svgInit();
		for (quint32 a = 0; a < count; a++)
		{
			QPointF p = getEMFPPoint(ds, compressedPoints);
			if (bFirst)
			{
				polyline.svgMoveTo(p.x(), p.y());
				bFirst = false;
			}
			else
				polyline.svgLineTo(p.x(), p.y());
		}
		if (polyline.size() > 3)
		{
			polyline.svgClosePath();
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = polyline.copy();
			finishItem(ite);
		}
	}
}

void EmfPlug::handleEMFPFillRects(QDataStream &ds, quint8 flagsL)
{
	quint32 brushID, count;
	ds >> brushID >> count;
	bool directBrush = (flagsL & 0x80);
	bool compressedRects = (flagsL & 0x40);
	getEMFPBrush(brushID, directBrush);
	for (quint32 a = 0; a < count; a++)
	{
		QPolygonF rect = getEMFPRect(ds, compressedRects);
		FPointArray polyline;
		polyline.svgInit();
		polyline.svgMoveTo(rect[0].x(), rect[0].y());
		polyline.svgLineTo(rect[1].x(), rect[1].y());
		polyline.svgLineTo(rect[2].x(), rect[2].y());
		polyline.svgLineTo(rect[3].x(), rect[3].y());
		polyline.svgClosePath();
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = polyline.copy();
		finishItem(ite);
	}
}

void EmfPlug::handleEMFPFillRegion(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 brushID;
	ds >> brushID;
	bool directBrush = (flagsL & 0x80);
	getEMFPBrush(brushID, directBrush);
	if (emfStyleMapEMP.contains(flagsH))
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = emfStyleMapEMP[flagsH].Coords.copy();
		finishItem(ite);
	}
}

void EmfPlug::handleEMFPDrawArc(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	float startA, sweepA;
	bool compressedRects = (flagsL & 0x40);
	getEMFPPen(flagsH);
	ds >> startA >> sweepA;
	QRectF rect = getEMFPRect(ds, compressedRects).boundingRect();
	FPointArray  pointArray;
	QPainterPath painterPath;
	painterPath.arcMoveTo(rect, -startA);
	painterPath.arcTo(rect, -startA, -sweepA);
	pointArray.fromQPainterPath(painterPath);
	if (pointArray.size() > 3)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointArray.copy();
		finishItem(ite, false);
	}
}

void EmfPlug::handleEMFPDrawBezier(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 count;
	ds >> count;
	getEMFPPen(flagsH);
	bool compressedPoints    = (flagsL & 0x40);
	bool closedPolyline      = (flagsL & 0x20);
	bool relativeCoordinates = (flagsL & 0x08);
	if (!relativeCoordinates)
	{
		FPointArray polyline;
		polyline.svgInit();
		QPointF p1 = getEMFPPoint(ds, compressedPoints);
		polyline.svgMoveTo(p1.x(), p1.y());
		for (quint32 a = 1; a < count; a += 3)
		{
			QPointF p2 = getEMFPPoint(ds, compressedPoints);
			QPointF p3 = getEMFPPoint(ds, compressedPoints);
			QPointF p4 = getEMFPPoint(ds, compressedPoints);
			polyline.svgCurveToCubic(p2.x(), p2.y(), p3.x(), p3.y(), p4.x(), p4.y());
		}
		if (polyline.size() > 3)
		{
			if (closedPolyline)
				polyline.svgClosePath();
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = polyline.copy();
			finishItem(ite, false);
		}
	}
}

void EmfPlug::handleEMFPDrawClosedCurve(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 count;
	float tension;
	ds >> tension;
	ds >> count;
	getEMFPPen(flagsH);
	QPolygonF points = getEMFPCurvePoints(ds, flagsL, count);
	QPainterPath path;
	GdipAddPathClosedCurve(path, points, tension);
	FPointArray polyline;
	polyline.fromQPainterPath(path);
	if (polyline.size() > 3)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = polyline.copy();
		finishItem(ite, false);
	}
}

void EmfPlug::handleEMFPDrawCurve(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 count, offset, numSegs;
	float tension;
	ds >> tension;
	ds >> offset >> numSegs >> count;
	getEMFPPen(flagsH);
	QPolygonF points = getEMFPCurvePoints(ds, flagsL, count);
	QPainterPath path;
	GdipAddPathCurve(path, points, tension);
	FPointArray polyline;
	polyline.fromQPainterPath(path);
	if (polyline.size() > 3)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = polyline.copy();
		finishItem(ite, false);
	}
}

void EmfPlug::handleEMFPDrawEllipse(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	bool compressedRects = (flagsL & 0x40);
	getEMFPPen(flagsH);
	QPointF p = getEMFPPoint(ds, compressedRects);
	double w = getEMFPDistance(ds, compressedRects);
	double h = getEMFPDistance(ds, compressedRects);
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX, baseY, w, h, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
	PageItem* ite = m_Doc->Items->at(z);
	QTransform mm(1.0, 0.0, 0.0, 1.0, p.x(), p.y());
	ite->PoLine.map(mm);
	finishItem(ite, false);
}

void EmfPlug::handleEMFPDrawImage(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	if (!emfStyleMapEMP.contains(flagsH))
		return;
	quint32 imgAttrs, dummy;
	ds >> imgAttrs;
	bool compressedPoints    = (flagsL & 0x40);
	bool relativeCoordinates = (flagsL & 0x08);
	currentDC.CurrFillTrans = 0.0;
	if (!relativeCoordinates)
	{
		ds >> dummy >> dummy >> dummy >> dummy >> dummy;
		QPolygonF rect = getEMFPRect(ds, compressedPoints);
		QPointF p1 = rect[0];
		QPointF p2 = rect[1];
		QPointF p3 = rect[3];
		handleEMFPDrawImageData(p1, p2, p3, flagsH);
	}
}

void EmfPlug::handleEMFPDrawImagePoints(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	if (!emfStyleMapEMP.contains(flagsH))
		return;
	quint32 imgAttrs, dummy, count;
	ds >> imgAttrs;
	bool compressedPoints    = (flagsL & 0x40);
	bool relativeCoordinates = (flagsL & 0x08);
	currentDC.CurrFillTrans = 0.0;
	if (!relativeCoordinates)
	{
		ds >> dummy >> dummy >> dummy >> dummy >> dummy;
		ds >> count;
		QPointF p1 = getEMFPPoint(ds, compressedPoints);
		QPointF p2 = getEMFPPoint(ds, compressedPoints);
		QPointF p3 = getEMFPPoint(ds, compressedPoints);
		handleEMFPDrawImageData(p1, p2, p3, flagsH);
	}
}

void EmfPlug::handleEMFPDrawLines(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 count;
	ds >> count;
	getEMFPPen(flagsH);
	bool compressedPoints    = (flagsL & 0x40);
	bool closedPolyline      = (flagsL & 0x20);
	bool relativeCoordinates = (flagsL & 0x08);
	if (!relativeCoordinates)
	{
		bool bFirst = true;
		FPointArray polyline;
		polyline.svgInit();
		for (quint32 a = 0; a < count; a++)
		{
			QPointF p = getEMFPPoint(ds, compressedPoints);
			if (bFirst)
			{
				polyline.svgMoveTo(p.x(), p.y());
				bFirst = false;
			}
			else
				polyline.svgLineTo(p.x(), p.y());
		}
		if (polyline.size() > 3)
		{
			if (closedPolyline)
				polyline.svgClosePath();
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
			PageItem* ite = m_Doc->Items->at(z);
			ite->PoLine = polyline.copy();
			finishItem(ite, false);
		}
	}
}

void EmfPlug::handleEMFPDrawPath(QDataStream &ds, quint8 flagsH)
{
	quint32 penID;
	ds >> penID;
	getEMFPPen(penID);
	if (emfStyleMapEMP.contains(flagsH))
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = emfStyleMapEMP[flagsH].Coords.copy();
		finishItem(ite, false);
	}
}

void EmfPlug::handleEMFPDrawPie(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	float startA, sweepA;
	bool compressedRects = (flagsL & 0x40);
	getEMFPPen(flagsH);
	ds >> startA >> sweepA;
	QRectF rect = getEMFPRect(ds, compressedRects).boundingRect();
	FPointArray  pointArray;
	QPainterPath painterPath;
	painterPath.arcMoveTo(rect, -startA);
	QPointF firstPoint = painterPath.currentPosition();
	painterPath.arcTo(rect, -startA, -sweepA);
	painterPath.lineTo(rect.center());
	painterPath.lineTo(firstPoint);
	pointArray.fromQPainterPath(painterPath);
	if (pointArray.size() > 3)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = pointArray.copy();
		finishItem(ite, false);
	}
}

void EmfPlug::handleEMFPDrawRects(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 count;
	ds >> count;
	bool compressedRects = (flagsL & 0x40);
	getEMFPPen(flagsH);
	for (quint32 a = 0; a < count; a++)
	{
		QPolygonF rect = getEMFPRect(ds, compressedRects);
		FPointArray polyline;
		polyline.svgInit();
		polyline.svgMoveTo(rect[0].x(), rect[0].y());
		polyline.svgLineTo(rect[1].x(), rect[1].y());
		polyline.svgLineTo(rect[2].x(), rect[2].y());
		polyline.svgLineTo(rect[3].x(), rect[3].y());
		polyline.svgClosePath();
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, currentDC.LineW, CommonStrings::None, currentDC.CurrColorStroke, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = polyline.copy();
		finishItem(ite, false);
	}
}

void EmfPlug::handleEMFPDrawString(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	quint32 brushID, formatID, numChars;
	FPointArray textPath;
	QPainterPath painterPath;
	ds >> brushID >> formatID >> numChars;
	QPolygonF rect = getEMFPRect(ds, false);
	QString stringData = "";
	for (quint32 a = 0; a < numChars; a++)
	{
		quint16 cc;
		ds >> cc;
		stringData.append(QChar(cc));
	}
	bool directBrush = (flagsL & 0x80);
	getEMFPBrush(brushID, directBrush);
	getEMFPFont(flagsH);
	getEMFPStringFormat(formatID);
	quint32 unit = currentDC.fontUnit;
	if ((unit == U_UT_World) || (unit == U_UT_Display))
		unit = U_UT_Pixel;
	double fSize = convertEMFPLogical2Pts(currentDC.fontSize, unit);
	if (fSize < 5)
	{
		QFont font = QFont(currentDC.fontName, fSize * 10);
		font.setPixelSize(fSize * 10);
		painterPath.addText(0, 0, font, stringData);
		QTransform mm;
		mm.scale(0.1, 0.1);
		painterPath = mm.map(painterPath);
	}
	else
	{
		QFont font = QFont(currentDC.fontName, fSize);
		font.setPixelSize(fSize);
		painterPath.addText(0, 0, font, stringData);
	}
	painterPath.translate(0, -painterPath.boundingRect().y());
	if (currentDC.verticalText)
	{
		QTransform vm;
		vm.rotate(90);
		painterPath = vm.map(painterPath);
		painterPath.translate(-painterPath.boundingRect().x(), 0);
	}
	double sh = rect.boundingRect().height();
	double sw = rect.boundingRect().width();
	if (currentDC.verticalText)
	{
		if (sh > 0)
		{
			if (currentDC.hAlign == U_SA_Center)
				painterPath.translate(0, (sh - painterPath.boundingRect().height()) / 2.0);
			else if (currentDC.hAlign == U_SA_Far)
				painterPath.translate(0, sh - painterPath.boundingRect().height());
		}
		if (sw > 0)
		{
			if (currentDC.vAlign == U_SA_Center)
				painterPath.translate((sw - painterPath.boundingRect().width()) / 2.0, 0);
			else if (currentDC.vAlign == U_SA_Far)
				painterPath.translate(sw - painterPath.boundingRect().width(), 0);
		}
	}
	else
	{
		if (sw > 0)
		{
			if (currentDC.hAlign == U_SA_Center)
				painterPath.translate((sw - painterPath.boundingRect().width()) / 2.0, 0);
			else if (currentDC.hAlign == U_SA_Far)
				painterPath.translate(sw - painterPath.boundingRect().width(), 0);
		}
		if (sh > 0)
		{
			if (currentDC.vAlign == U_SA_Center)
				painterPath.translate(0, (sh - painterPath.boundingRect().height()) / 2.0);
			else if (currentDC.vAlign == U_SA_Far)
				painterPath.translate(0, sh - painterPath.boundingRect().height());
		}
	}
	QTransform bm = currentDC.m_WorldMapEMFP;
	bm = QTransform(bm.m11(), bm.m12(), bm.m21(), bm.m22(), 0, 0);
	painterPath = bm.map(painterPath);
	painterPath.translate(rect[0].x(), rect[0].y());
	textPath.fromQPainterPath(painterPath);
	if (textPath.size() > 0)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, currentDC.CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = textPath.copy();
		finishItem(ite);
	}
}

void EmfPlug::handleEMFPSetClipRect(QDataStream &ds, quint8 flagsL)
{
	quint8 mode = flagsL & 0x0F;
	QPolygonF rect = getEMFPRect(ds, false);
	FPointArray clipPath;
	clipPath.resize(0);
	clipPath.svgInit();
	clipPath.svgMoveTo(rect[0].x(), rect[0].y());
	clipPath.svgLineTo(rect[1].x(), rect[1].y());
	clipPath.svgLineTo(rect[2].x(), rect[2].y());
	clipPath.svgLineTo(rect[3].x(), rect[3].y());
	clipPath.svgClosePath();
	if ((mode == 0) || (currentDC.clipPath.isEmpty()))
		currentDC.clipPath = clipPath.copy();
	else
	{
		QPainterPath pathN = clipPath.toQPainterPath(true);
		QPainterPath pathA = currentDC.clipPath.toQPainterPath(true);
		QPainterPath resultPath;
		if (mode == 1)
			resultPath = pathA.intersected(pathN);
		else if (mode == 2)
			resultPath = pathA.united(pathN);
		else if (mode == 3)
		{
			QPainterPath part1 = pathA.subtracted(pathN);
			QPainterPath part2 = pathN.subtracted(pathA);
			resultPath.addPath(part1);
			resultPath.addPath(part2);
		}
		if (!resultPath.isEmpty())
		{
			FPointArray polyline;
			polyline.resize(0);
			polyline.fromQPainterPath(resultPath, true);
			polyline.svgClosePath();
			currentDC.clipPath = polyline.copy();
		}
	}
}

void EmfPlug::handleEMFPSetClipRegion(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	if (emfStyleMapEMP.contains(flagsH))
	{
		if (emfStyleMapEMP[flagsH].Coords.isEmpty())
		{
			currentDC.clipPath.resize(0);
			currentDC.clipPath.svgInit();
			return;
		}
		quint8 mode = flagsL & 0x0F;
		if ((mode == 0) || (currentDC.clipPath.isEmpty()))
			currentDC.clipPath = emfStyleMapEMP[flagsH].Coords.copy();
		else
		{
			FPointArray clipPath = emfStyleMapEMP[flagsH].Coords.copy();
			QPainterPath pathN = clipPath.toQPainterPath(true);
			QPainterPath pathA = currentDC.clipPath.toQPainterPath(true);
			QPainterPath resultPath;
			if (mode == 1)
				resultPath = pathA.intersected(pathN);
			else if (mode == 2)
				resultPath = pathA.united(pathN);
			else if (mode == 3)
			{
				QPainterPath part1 = pathA.subtracted(pathN);
				QPainterPath part2 = pathN.subtracted(pathA);
				resultPath.addPath(part1);
				resultPath.addPath(part2);
			}
			if (!resultPath.isEmpty())
			{
				FPointArray polyline;
				polyline.resize(0);
				polyline.fromQPainterPath(resultPath, true);
				polyline.svgClosePath();
				currentDC.clipPath = polyline.copy();
			}
		}
	}
	else
	{
		currentDC.clipPath.resize(0);
		currentDC.clipPath.svgInit();
	}
}

void EmfPlug::handleEMFPSetClipPath(QDataStream &ds, quint8 flagsL, quint8 flagsH)
{
	if (emfStyleMapEMP.contains(flagsH))
	{
		quint8 mode = flagsL & 0x0F;
		if ((mode == 0) || (currentDC.clipPath.isEmpty()))
			currentDC.clipPath = emfStyleMapEMP[flagsH].Coords.copy();
		else
		{
			FPointArray clipPath = emfStyleMapEMP[flagsH].Coords.copy();
			QPainterPath pathN = clipPath.toQPainterPath(true);
			QPainterPath pathA = currentDC.clipPath.toQPainterPath(true);
			QPainterPath resultPath;
			if (mode == 1)
				resultPath = pathA.intersected(pathN);
			else if (mode == 2)
				resultPath = pathA.united(pathN);
			else if (mode == 3)
			{
				QPainterPath part1 = pathA.subtracted(pathN);
				QPainterPath part2 = pathN.subtracted(pathA);
				resultPath.addPath(part1);
				resultPath.addPath(part2);
			}
			if (!resultPath.isEmpty())
			{
				FPointArray polyline;
				polyline.resize(0);
				polyline.fromQPainterPath(resultPath, true);
				polyline.svgClosePath();
				currentDC.clipPath = polyline.copy();
			}
		}
	}
}

void EmfPlug::handleEMFPSerializableObject(QDataStream &ds)
{
	quint32 l;
	quint16 w1, w2;
	quint8 b1, b2, b3, b4, b5, b6, b7, b8;
	ds >> l;
	ds >> w1 >> w2;
	ds >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7 >> b8;
	QString effID = QUuid(l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8).toString().toUpper();
	m_Effects.clear();
	SerializableObject_Valid = false;
	if (effID == U_IE_BlurEffectGuid)
	{
		SerializableObject_Valid = true;
		float edge;
		ds >> edge;
		ImageEffect ef;
		ef.effectCode = ScImage::EF_BLUR;
		ef.effectParameters = QString("%1 1.0").arg(edge / 255.0 * 30.0);
		m_Effects.append(ef);
	}
	else if (effID == U_IE_BrightnessContrastEffectGuid)
	{
		SerializableObject_Valid = true;
		qint32 brightness, contrast;
		ds >> brightness >> contrast;
		if (brightness != 0)
		{
			ImageEffect ef;
			ef.effectCode = ScImage::EF_BRIGHTNESS;
			ef.effectParameters = QString("%1").arg(brightness);
			m_Effects.append(ef);
		}
		if (contrast != 0)
		{
			ImageEffect ef;
			ef.effectCode = ScImage::EF_CONTRAST;
			ef.effectParameters = QString("%1").arg(qMin(qMax(qRound(contrast * 1.27), -127), 127));
			m_Effects.append(ef);
		}
	}
	else if (effID == U_IE_ColorBalanceEffectGuid)
		qDebug() << "ImageEffect\tColorbalance";
	else if (effID == U_IE_ColorCurveEffectGuid)
		qDebug() << "ImageEffect\tColorCurve";
	else if (effID == U_IE_ColorLookupTableEffectGuid)
		qDebug() << "ImageEffect\tColorLookupTable";
	else if (effID == U_IE_ColorMatrixEffectGuid)
		qDebug() << "ImageEffect\tColorMatrix";
	else if (effID == U_IE_HueSaturationLightnessEffectGuid)
		qDebug() << "ImageEffect\tHSL";
	else if (effID == U_IE_LevelsEffectGuid)
		qDebug() << "ImageEffect\tLevels";
	else if (effID == U_IE_RedEyeCorrectionEffectGuid)
		qDebug() << "ImageEffect\tRedEye";
	else if (effID == U_IE_SharpenEffectGuid)
	{
		SerializableObject_Valid = true;
		float radius, amount;
		ds >> radius >> amount;
		double amo = amount;
		double rad = radius;
		ImageEffect ef;
		ef.effectCode = ScImage::EF_SHARPEN;
		ef.effectParameters = QString("%1 %2").arg(qMin(rad, 10.0)).arg(qMin(amo / 100.0 * 5.0, 5.0));
		m_Effects.append(ef);
	}
	else if (effID == U_IE_TintEffectGuid)
		qDebug() << "ImageEffect\tTint";
	else
		SerializableObject_Valid = false;
}

void EmfPlug::getEMFPBrush(quint32 brushID, bool directBrush)
{
	if (directBrush)
	{
		quint8 r = brushID & 0xFF;
		quint8 g = (brushID >> 8) & 0xFF;
		quint8 b = (brushID >> 16) & 0xFF;
		quint8 a = (brushID >> 24) & 0xFF;
		QColor col(b, g, r, a);
		currentDC.CurrColorFill = handleColor(col);
		currentDC.CurrFillTrans = 1.0 - col.alphaF();
		currentDC.brushStyle = U_BT_SolidColor;
	}
	else
	{
		if (emfStyleMapEMP.contains(brushID))
		{
			emfStyle sty = emfStyleMapEMP[brushID];
			currentDC.CurrColorFill = sty.brushColor;
			currentDC.brushStyle = sty.brushStyle;
			currentDC.hatchStyle = sty.hatchStyle;
			currentDC.CurrFillTrans = sty.fillTrans;
			if (sty.brushStyle == U_BT_HatchFill)
			{
				currentDC.backColor = sty.penColor;
				currentDC.backgroundMode = true;
			}
			else if (sty.brushStyle == U_BT_LinearGradient)
			{
				currentDC.gradientStart = sty.gradientStart;
				currentDC.gradientEnd = sty.gradientEnd;
				currentDC.gradientAngle = sty.gradientAngle;
				currentDC.gradient = sty.gradient;
			}
			else if (sty.brushStyle == U_BT_PathGradient)
			{
				currentDC.gradientStart = sty.gradientStart;
				currentDC.gradientAngle = sty.gradientAngle;
				currentDC.gradient = sty.gradient;
				currentDC.gradientPath = sty.gradientPath.copy();
			}
			else if (sty.brushStyle == U_BT_TextureFill)
			{
				currentDC.patternMode = sty.patternMode;
				if (sty.patternName.isEmpty())
				{
					if (!emfStyleMapEMP[brushID].MetaFile)
					{
						QImage img = getImageDataFromStyle(brushID);
						if (!img.isNull())
						{
							QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_emf_XXXXXX.png");
							tempFile->setAutoRemove(false);
							if (tempFile->open())
							{
								QString fileName = getLongPathName(tempFile->fileName());
								if (!fileName.isEmpty())
								{
									tempFile->close();
									img.save(fileName, "PNG");
									ScPattern pat = ScPattern();
									pat.setDoc(m_Doc);
									int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
									PageItem* newItem = m_Doc->Items->at(z);
									m_Doc->loadPict(fileName, newItem);
									m_Doc->Items->takeAt(z);
									newItem->isInlineImage = true;
									newItem->isTempFile = true;
									pat.width = newItem->pixm.qImage().width();
									pat.height = newItem->pixm.qImage().height();
									pat.scaleX = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
									pat.scaleY = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
									pat.pattern = newItem->pixm.qImage().copy();
									newItem->setWidth(pat.pattern.width());
									newItem->setHeight(pat.pattern.height());
									newItem->SetRectFrame();
									newItem->gXpos = 0.0;
									newItem->gYpos = 0.0;
									newItem->gWidth = pat.pattern.width();
									newItem->gHeight = pat.pattern.height();
									pat.items.append(newItem);
									QString patternName = "Pattern_"+newItem->itemName();
									m_Doc->addPattern(patternName, pat);
									emfStyleMapEMP[brushID].patternName = patternName;
									importedPatterns.append(patternName);
									currentDC.patternName = patternName;
								}
							}
						}
					}
				}
				else
					currentDC.patternName = sty.patternName;
			}
		}
	}
}

void EmfPlug::getEMFPPen(quint32 penID)
{
	if (emfStyleMapEMP.contains(penID))
	{
		emfStyle sty = emfStyleMapEMP[penID];
		currentDC.CurrColorStroke = sty.penColor;
		currentDC.CurrStrokeTrans = sty.penTrans;
		currentDC.LineW = sty.penWidth;
		currentDC.penCap = sty.penCap;
		currentDC.penJoin = sty.penJoin;
		currentDC.penStyle = sty.penStyle;
		currentDC.dashArray = sty.dashArray;
		currentDC.dashOffset = sty.dashOffset;
	}
}

void EmfPlug::getEMFPFont(quint32 fontID)
{
	if (emfStyleMapEMP.contains(fontID))
	{
		emfStyle sty = emfStyleMapEMP[fontID];
		currentDC.fontName = sty.fontName;
		currentDC.fontSize = sty.fontSize;
		currentDC.fontUnit = sty.fontUnit;
	}
}

void EmfPlug::getEMFPStringFormat(quint32 fontID)
{
	if (emfStyleMapEMP.contains(fontID))
	{
		emfStyle sty = emfStyleMapEMP[fontID];
		currentDC.hAlign = sty.hAlign;
		currentDC.vAlign = sty.vAlign;
		currentDC.verticalText = sty.verticalText;
	}
}

FPointArray EmfPlug::getEMPPathData(QDataStream &ds)
{
	FPointArray polyline;
	polyline.resize(0);
	polyline.svgInit();
	quint32 dummy, count;
	quint16 flags, dummy2;
	ds >> dummy >> count;
	ds >> flags >> dummy2;
	bool compressedPoints    = (flags & 0x4000);
	bool rleEncodedType      = (flags & 0x1000);
	bool relativeCoordinates = (flags & 0x0800);
	QList<QPointF> points;
	QList<quint8> pTypes;
	if (!relativeCoordinates)
	{
		for (quint32 a = 0; a < count; a++)
		{
			QPointF p = getEMFPPoint(ds, compressedPoints);
			points.append(p);
		}
		for (quint32 b = 0; b < count; b++)
		{
			if (rleEncodedType)
			{
				quint8 cc, flg;
				ds >> cc >> flg;
				cc = cc & 0x3F;
				for (quint8 ccc = 0; ccc < cc; ccc++)
				{
					pTypes.append(flg);
				}
				b += cc;
			}
			else
			{
				quint8 val;
				ds >> val;
				pTypes.append(val);
			}
		}
		for (quint32 c = 0; c < count; c++)
		{
			QPointF p = points[c];
			quint8 pfl = (pTypes[c] & 0xF0) >> 4;
			quint8 pty = pTypes[c] & 0x0F;
			if (pty == U_PPT_Start)
				polyline.svgMoveTo(p.x(), p.y());
			else if (pty == U_PPT_Line)
				polyline.svgLineTo(p.x(), p.y());
			else if (pty == U_PPT_Bezier)
			{
				QPointF p2 = points[c+1];
				QPointF p3 = points[c+2];
				polyline.svgCurveToCubic(p.x(), p.y(), p2.x(), p2.y(), p3.x(), p3.y());
				c += 2;
				pfl = (pTypes[c] & 0xF0) >> 4;
			}
			if (pfl & 0x08)
				polyline.svgClosePath();
		}
	}
	return polyline;
}

QPolygonF EmfPlug::getEMFPCurvePoints(QDataStream &ds, quint8 flagsL, quint32 count)
{
	bool compressedPoints    = (flagsL & 0x40);
	bool relativeCoordinates = (flagsL & 0x08);
	QPolygonF points;
	if (!relativeCoordinates)
	{
		for (quint32 a = 0; a < count; a++)
		{
			QPointF p = getEMFPPoint(ds, compressedPoints);
			points.append(p);
		}
	}
	return points;
}

QPolygonF EmfPlug::getEMFPRect(QDataStream &ds, bool size)
{
	QPolygonF result;
	QPointF p1, p2, p3, p4;
	if (size)
	{
		qint16 x1, y1, w, h;
		ds >> x1 >> y1 >> w >> h;
		p1 = QPointF(x1, y1);
		p2 = QPointF(p1.x() + w, p1.y());
		p3 = QPointF(p1.x() + w, p1.y() + h);
		p4 = QPointF(p1.x(), p1.y() + h);
	}
	else
	{
		float x1, y1, w, h;
		ds >> x1 >> y1 >> w >> h;
		p1 = QPointF(x1, y1);
		p2 = QPointF(p1.x() + w, p1.y());
		p3 = QPointF(p1.x() + w, p1.y() + h);
		p4 = QPointF(p1.x(), p1.y() + h);
	}
	result.append(convertEMFPLogical2Pts(p1, currentDC.emfPlusUnit));
	result.append(convertEMFPLogical2Pts(p2, currentDC.emfPlusUnit));
	result.append(convertEMFPLogical2Pts(p3, currentDC.emfPlusUnit));
	result.append(convertEMFPLogical2Pts(p4, currentDC.emfPlusUnit));
//	result.translate(-currentDC.originEMFP);
	result.translate(currentDC.viewOrigin);
	return result;
}

QPointF EmfPlug::getEMFPPoint(QDataStream &ds, bool size)
{
	QPointF p;
	if (size)
	{
		qint16 x1, y1;
		ds >> x1 >> y1;
		p = QPointF(x1, y1);
	}
	else
	{
		float x1, y1;
		ds >> x1 >> y1;
		p = QPointF(x1, y1);
	}
	p = convertEMFPLogical2Pts(p, currentDC.emfPlusUnit);
//	p -= currentDC.originEMFP;
	p += currentDC.viewOrigin;
	return p;
}

double EmfPlug::getEMFPDistance(QDataStream &ds, bool size)
{
	double p;
	if (size)
	{
		qint16 x1;
		ds >> x1;
		p = x1;
	}
	else
	{
		float x1;
		ds >> x1;
		p = x1;
	}
	p = convertEMFPLogical2Pts(p, currentDC.emfPlusUnit);
	return p;
}

QPointF EmfPlug::convertEMFPLogical2Pts(QPointF in, quint16 unit)
{
	QPointF out = currentDC.m_WorldMapEMFP.map(in);
	switch (unit)
	{
		case U_UT_World:
		case U_UT_Display:
			break;
		case U_UT_Pixel:
			if (emfPlusDual && emfMixed)
			{
				out.setX(out.x() / dpiX * 72.0);
				out.setY(out.y() / dpiY * 72.0);
			}
			else
			{
				out.setX(out.x() / static_cast<double>(EmfPdpiX) * 72.0);
				out.setY(out.y() / static_cast<double>(EmfPdpiY) * 72.0);
			}
			break;
		case U_UT_Point:
			break;
		case U_UT_Inch:
			out.setX(out.x() * 72.0);
			out.setY(out.y() * 72.0);
			break;
		case U_UT_Document:
			out.setX(out.x() / 300.0 * 72.0);
			out.setY(out.y() / 300.0 * 72.0);
			break;
		case U_UT_Millimeter:
			out.setX(out.x() / 10.0 / 2.54 * 72.0);
			out.setY(out.y() / 10.0 / 2.54 * 72.0);
			break;
		default:
			break;
	}
	return out;
}

double EmfPlug::convertEMFPLogical2Pts(double in, quint16 unit)
{
	QLineF dist = QLineF(0, 0, in, 0);
	dist = currentDC.m_WorldMapEMFP.map(dist);
	double out = dist.length();
	switch (unit)
	{
		case U_UT_World:
		case U_UT_Display:
			break;
		case U_UT_Pixel:
			if (emfPlusDual && emfMixed)
				out = out / dpiX * 72.0;
			else
				out = out / static_cast<double>(EmfPdpiX) * 72.0;
			break;
		case U_UT_Point:
			break;
		case U_UT_Inch:
			out = out * 72.0;
			break;
		case U_UT_Document:
			out = out / 300 * 72.0;
			break;
		case U_UT_Millimeter:
			out = out / 10.0 / 2.54 * 72.0;
			break;
		default:
			break;
	}
	return out;
}

QPolygonF EmfPlug::gdip_open_curve_tangents(QPolygonF &points, double tension)
{
	double coefficient = tension / 3.0;
	int i;
	int count = points.count();
	QPolygonF tangents;
	tangents.fill(QPointF(0,0), count);
	if (count <= 2)
		return tangents;
	for (i = 0; i < count; i++)
	{
		int r = i + 1;
		int s = i - 1;
		if (r >= count)
			r = count - 1;
		if (s < 0)
			s = 0;
		tangents[i] += QPointF(coefficient * (points[r].x() - points[s].x()), coefficient * (points[r].y() - points[s].y()));
	}
	return tangents;
}

QPolygonF EmfPlug::gdip_closed_curve_tangents(QPolygonF &points, double tension)
{
	double coefficient = tension / 3.0;
	int i;
	int count = points.count();
	QPolygonF tangents;
	tangents.fill(QPointF(0,0), count);
	if (count <= 2)
		return tangents;
	for (i = 0; i < count; i++)
	{
		int r = i + 1;
		int s = i - 1;
		if (r >= count)
			r -= count;
		if (s < 0)
			s += count;
		tangents[i] += QPointF(coefficient * (points[r].x() - points[s].x()), coefficient * (points[r].y() - points[s].y()));
	}
	return tangents;
}

void EmfPlug::append_curve(QPainterPath &path, QPolygonF &points, QPolygonF &tangents, bool type)
{
	int i;
	path.moveTo(points[0]);
	for (i = 0; i < points.count() - 1; i++)
	{
		int j = i + 1;
		path.cubicTo(points[i] + tangents[i], points[j] - tangents[j], points[j]);
	}
	if (type)
	{
		/* complete (close) the curve using the first point */
		path.cubicTo(points[i] + tangents[i], points[0] - tangents[0], points[0]);
		path.closeSubpath();
	}
}

void EmfPlug::GdipAddPathCurve(QPainterPath &path, QPolygonF &points, float tension)
{
	QPolygonF tangents = gdip_open_curve_tangents(points, tension);
	append_curve(path, points, tangents, false);
}

void EmfPlug::GdipAddPathClosedCurve(QPainterPath &path, QPolygonF &points, float tension)
{
	QPolygonF tangents = gdip_closed_curve_tangents(points, tension);
	append_curve(path, points, tangents, true);
}

void EmfPlug::handleEMFPDrawImageData(QPointF p1, QPointF p2, QPointF p3, quint8 flagsH)
{
	if (emfStyleMapEMP[flagsH].MetaFile)
	{
		QString ext = "emf";
		if (emfStyleMapEMP[flagsH].imageType < U_MDT_Emf)
			ext = "wmf";
		QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_emf_XXXXXX." + ext);
		if (tempFile->open())
		{
			QString fileName = getLongPathName(tempFile->fileName());
			if (!fileName.isEmpty())
			{
				tempFile->write(emfStyleMapEMP[flagsH].imageData);
				tempFile->close();
				FileLoader *fileLoader = new FileLoader(fileName);
				int testResult = fileLoader->testFile();
				delete fileLoader;
				if (testResult != -1)
				{
					const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
					if(fmt)
					{
						m_Doc->m_Selection->clear();
						fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
						fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
						if (m_Doc->m_Selection->count() > 0)
						{
							PageItem* ite = m_Doc->groupObjectsSelection();
							ite->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
							ite->setXYPos(baseX + p1.x(), baseY + p1.y(), true);
							ite->setWidthHeight(QLineF(p1, p2).length(), QLineF(p1, p3).length(), true);
							if (QLineF(p1, p2).angle() != 0)
								ite->setRotation(-QLineF(p1, p2).angle(), true);
							ite->updateClip();
							finishItem(ite, false);
						}
						m_Doc->m_Selection->clear();
					}
				}
			}
		}
		delete tempFile;
	}
	else
	{
		QImage img = getImageDataFromStyle(flagsH);
		if (!img.isNull())
		{
			QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_emf_XXXXXX.png");
			tempFile->setAutoRemove(false);
			if (tempFile->open())
			{
				QString fileName = getLongPathName(tempFile->fileName());
				if (!fileName.isEmpty())
				{
					tempFile->close();
					img.save(fileName, "PNG");
					int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, baseX + p1.x(), baseY + p1.y(), QLineF(p1, p2).length(), QLineF(p1, p3).length(), 0, CommonStrings::None, CommonStrings::None, true);
					PageItem* ite = m_Doc->Items->at(z);
					finishItem(ite, false);
					if (QLineF(p1, p2).angle() != 0)
						ite->setRotation(-QLineF(p1, p2).angle(), true);
					ite->isInlineImage = true;
					ite->isTempFile = true;
					if (SerializableObject_Valid)
					{
						ite->effectsInUse = m_Effects;
						SerializableObject_Valid = false;
						m_Effects.clear();
					}
					m_Doc->loadPict(fileName, ite);
					ite->setImageScalingMode(false, false);
					ite->updateClip();
					if (currentDC.clipPath.count() != 0)
					{
						FPointArray cp = currentDC.clipPath.copy();
						cp.translate(baseX, baseY);
						cp.translate(-docX, -docY);
						cp.translate(-ite->xPos(), -ite->yPos());
						ite->PoLine = cp.copy();
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(),wh.y());
						ite->setTextFlowMode(PageItem::TextFlowDisabled);
						m_Doc->AdjustItemSize(ite);
						ite->OldB2 = ite->width();
						ite->OldH2 = ite->height();
						ite->updateClip();
					}
				}
			}
			delete tempFile;
		}
	}
}

QImage EmfPlug::getImageDataFromStyle(quint8 flagsH)
{
	QImage img;
	if (emfStyleMapEMP[flagsH].imageType == 1)
		img.loadFromData(emfStyleMapEMP[flagsH].imageData);
	else
	{
		int hWidth = qAbs(emfStyleMapEMP[flagsH].imageWidth);
		int hHeight = qAbs(emfStyleMapEMP[flagsH].imageHeight);
		QDataStream dsB(emfStyleMapEMP[flagsH].imageData);
		dsB.setByteOrder(QDataStream::LittleEndian);
		img = QImage(hWidth, hHeight, QImage::Format_ARGB32);
		img.fill(0);
		if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_32bppARGB)
		{
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				QRgb *dst = (QRgb*)img.scanLine(yy);
				for (qint32 xx = 0; xx < hWidth; xx++)
				{
					quint8 r, g, b, a;
					dsB >> b >> g >> r >> a;
					*dst = qRgba(r, g, b, a);
					dst++;
				}
			}
		}
		else if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_32bppRGB)
		{
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				QRgb *dst = (QRgb*)img.scanLine(yy);
				for (qint32 xx = 0; xx < hWidth; xx++)
				{
					quint8 r, g, b, a;
					dsB >> b >> g >> r >> a;
					*dst = qRgba(r, g, b, 255);
					dst++;
				}
			}
		}
		else if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_24bppRGB)
		{
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				QRgb *dst = (QRgb*)img.scanLine(yy);
				for (qint32 xx = 0; xx < hWidth; xx++)
				{
					quint8 r, g, b;
					dsB >> b >> g >> r;
					*dst = qRgba(r, g, b, 255);
					dst++;
				}
				aligntoQuadWord(dsB);
			}
		}
		else if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_16bppRGB555)
		{
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				QRgb *dst = (QRgb*)img.scanLine(yy);
				for (qint32 xx = 0; xx < hWidth; xx++)
				{
					quint16 dt;
					quint8 r, g, b;
					dsB >> dt;
					b = (dt & 0x1F) * 8;
					g = ((dt >> 5) & 0x1F) * 8;
					r = ((dt >> 10) & 0x1F) * 8;
					*dst = qRgba(r, g, b, 255);
					dst++;
				}
				aligntoQuadWord(dsB);
			}
		}
		else if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_16bppGrayScale)
		{
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				QRgb *dst = (QRgb*)img.scanLine(yy);
				for (qint32 xx = 0; xx < hWidth; xx++)
				{
					quint16 r;
					dsB >> r;
					r = r >> 8;
					*dst = qRgba(r, r, r, 255);
					dst++;
				}
				aligntoQuadWord(dsB);
			}
		}
		else if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_8bppIndexed)
		{
			QVector<QRgb> colorTbl;
			quint32 palFlags, colorsUsed;
			dsB >> palFlags >> colorsUsed;
			for (quint32 pa = 0; pa < colorsUsed; pa++)
			{
				quint32 brushID;
				dsB >> brushID;
				quint8 r = brushID & 0xFF;
				quint8 g = (brushID >> 8) & 0xFF;
				quint8 b = (brushID >> 16) & 0xFF;
				quint8 a = (brushID >> 24) & 0xFF;
				if (palFlags & 0x00000001)
					colorTbl.append(qRgba(b, g, r, a));
				else
					colorTbl.append(qRgba(b, g, r, 255));
			}
			img = QImage(hWidth, hHeight, QImage::Format_Indexed8);
			img.fill(0);
			img.setColorTable(colorTbl);
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				char *dst = (char*)img.scanLine(yy);
				dsB.readRawData(dst, hWidth);
				aligntoQuadWord(dsB);
			}
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		else if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_4bppIndexed)
		{
			QVector<QRgb> colorTbl;
			quint32 palFlags, colorsUsed;
			dsB >> palFlags >> colorsUsed;
			for (quint32 pa = 0; pa < colorsUsed; pa++)
			{
				quint32 brushID;
				dsB >> brushID;
				quint8 r = brushID & 0xFF;
				quint8 g = (brushID >> 8) & 0xFF;
				quint8 b = (brushID >> 16) & 0xFF;
				quint8 a = (brushID >> 24) & 0xFF;
				if (palFlags & 0x00000001)
					colorTbl.append(qRgba(b, g, r, a));
				else
					colorTbl.append(qRgba(b, g, r, 255));
			}
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				QRgb *dst = (QRgb*)img.scanLine(yy);
				for (qint32 xx = 0; xx < hWidth; xx += 2)
				{
					quint8 r, rh, rl;
					dsB >> r;
					rh = (r >> 4) & 0xF;
					rl = r & 0xF;
					if (rh < colorTbl.count())
						*dst = colorTbl[rh];
					dst++;
					if (xx == hWidth - 1)
						break;
					if (rl < colorTbl.count())
						*dst = colorTbl[rl];
					dst++;
				}
				aligntoQuadWord(dsB);
			}
		}
		else if (emfStyleMapEMP[flagsH].imagePixelFormat == U_PF_1bppIndexed)
		{
			QVector<QRgb> colorTbl;
			quint32 palFlags, colorsUsed;
			dsB >> palFlags >> colorsUsed;
			for (quint32 pa = 0; pa < colorsUsed; pa++)
			{
				quint32 brushID;
				dsB >> brushID;
				quint8 r = brushID & 0xFF;
				quint8 g = (brushID >> 8) & 0xFF;
				quint8 b = (brushID >> 16) & 0xFF;
				quint8 a = (brushID >> 24) & 0xFF;
				if (palFlags & 0x00000001)
					colorTbl.append(qRgba(b, g, r, a));
				else
					colorTbl.append(qRgba(b, g, r, 255));
			}
			img = QImage(hWidth, hHeight, QImage::Format_Mono);
			img.fill(0);
			img.setColorTable(colorTbl);
			int bpl = img.bytesPerLine();
			for (qint32 yy = 0; yy < hHeight; yy++)
			{
				char *dst = (char*)img.scanLine(yy);
				dsB.readRawData(dst, bpl);
			}
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		else
		{
			qDebug() << QString("Format 0x%1").arg(emfStyleMapEMP[flagsH].imagePixelFormat, 8, 16, QChar('0'));
			return img;
		}
	}
	return img;
}
