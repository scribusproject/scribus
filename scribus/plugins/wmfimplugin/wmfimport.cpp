/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/* Code inspired by KOffice libwmf and adapted for Scribus by Jean Ghali */

#include "scconfig.h"
#include "wmfimport.h"

#include <QFile>
#include <QRegExp>
#include <QCursor>
#include <QBuffer>
#include <QDataStream>
#include <QDrag>
#include <QMimeData>

#include "customfdialog.h"
#include "scribus.h"
#include "scribusXml.h"
#include "mpalette.h"
#include "prefsfile.h"

#include <cmath>
#include "commonstrings.h"
#include "fpointarray.h"
#include "menumanager.h"
#include "prefsmanager.h"
#include "pageitem.h"
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

#define WMFIMPORT_DEBUG 1
#define MAX_OBJHANDLE   128

#include "wmfstruct.h"
#include "wmfcmd.h"
#include "wmfcontext.h"
#include "wmfhandle.h"
#include "metafuncs.h"

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
    m_FirstCmd     = NULL;
    m_ObjHandleTab = NULL;
    m_Dpi          = 1000;
}

WMFImport::~WMFImport()
{
	delete m_tmpSel;
	if ( m_FirstCmd ) delete m_FirstCmd;
    if ( m_ObjHandleTab ) delete[] m_ObjHandleTab;
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
		if (it.data().getColorModel() == colorModelRGB)
		{
			it.data().getRGB(&r, &g, &b);
			tmpColor.setRgb(r, g, b);
			if (color == tmpColor)
			{
				retColorName = it.key();
				found = true;
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

bool WMFImport::import(QString fname, int flags)
{
	if (!loadWMF(fname))
		return false;
	QString CurDirP = QDir::currentDirPath();
	QFileInfo efp(fname);
	QDir::setCurrent(efp.dirPath());
	importWMF(flags);
	QDir::setCurrent(CurDirP);
	return true;
}

bool WMFImport::loadWMF( const QString &fileName )
{
	QFile file( fileName );

    if ( !file.exists() )
    {
        cerr << "File " << QFile::encodeName(fileName).data() << " does not exist" << endl;
        return false;
    }

    if ( !file.open( IO_ReadOnly ) )
    {
        cerr << "Cannot open file " << QFile::encodeName(fileName).data() << endl;
        return false;
    }

    QByteArray ba = file.readAll();
    file.close();

    QBuffer buffer( &ba );
    buffer.open( IO_ReadOnly );
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
    WmfCmd *cmd, *last;
    WORD32 rdSize;
    WORD16 rdFunc;

	header.mtSize = 0;
	header.mtHeaderSize = 0;
	header.mtNoParameters = 0;

    if ( m_FirstCmd ) delete m_FirstCmd;
    m_FirstCmd = NULL;

    st.setDevice( &buffer );
    st.setByteOrder( QDataStream::LittleEndian ); // Great, I love Qt !

    //----- Read placeable metafile header
    st >> pheader.key;
    m_IsPlaceable = ( pheader.key==( DWORD )APMHEADER_KEY );
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
    else buffer.at( 0 );

    //----- Read as enhanced metafile header
    filePos = buffer.at();
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
        buffer.at( filePos );
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
    if ( m_Valid )
    {
        //----- Read Metafile Records
        last = NULL;
        rdFunc = -1;
        while ( !st.eof() && (rdFunc != 0) )
        {
            st >> rdSize;
            st >> rdFunc;
            idx = findFunc( rdFunc );
            rdSize -= 3;

            cmd = new WmfCmd;
            cmd->next = NULL;
            if ( last ) last->next = cmd;
            else m_FirstCmd = cmd;

            cmd->funcIndex = idx;
            cmd->numParam = rdSize;
            cmd->params = new WORD16[ rdSize ];
            last = cmd;

            for ( i=0; i<rdSize && !st.eof(); i++ )
                st >> cmd->params[ i ];


            if ( rdFunc == 0x020B ) {         // SETWINDOWORG: dimensions
                m_BBox.setLeft( cmd->params[ 1 ] );
                m_BBox.setTop( cmd->params[ 0 ] );
            }
            if ( rdFunc == 0x020C ) {         // SETWINDOWEXT: dimensions
                m_BBox.setWidth( cmd->params[ 1 ] );
                m_BBox.setHeight( cmd->params[ 0 ] );
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
    else {
        cerr << "WMF Header : incorrect header !" << endl;
    }

    buffer.close();
    return m_Valid;
}

bool WMFImport::importWMF(int flags)
{
	bool ret = false;
	/*double width  = m_BBox.width();
	double height = m_BBox.height();*/
	double scale = (m_Dpi > 288) ? 288.0 / m_Dpi : 1.0;
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
	FPoint minSize = m_Doc->minCanvasCoordinate;
	FPoint maxSize = m_Doc->maxCanvasCoordinate;
	m_Doc->view()->Deselect();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor), true);
	//gc->Family = m_Doc->toolSettings.defFont;
	m_Doc->PageColors.ensureBlackAndWhite();
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
				double lw = currItem->lineWidth() / 2.0;
				if (currItem->rotation() != 0)
				{
					FPointArray pb;
					pb.resize(0);
					pb.addPoint(FPoint(currItem->xPos()-lw, currItem->yPos()-lw));
					pb.addPoint(FPoint(currItem->width()+lw*2.0, -lw, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					pb.addPoint(FPoint(currItem->width()+lw*2.0, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					pb.addPoint(FPoint(-lw, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					for (uint pc = 0; pc < 4; ++pc)
					{
						minx = qMin(minx, pb.point(pc).x());
						miny = qMin(miny, pb.point(pc).y());
						maxx = qMax(maxx, pb.point(pc).x());
						maxy = qMax(maxy, pb.point(pc).y());
					}
				}
				else
				{
					minx = qMin(minx, currItem->xPos()-lw);
					miny = qMin(miny, currItem->yPos()-lw);
					maxx = qMax(maxx, currItem->xPos()-lw + currItem->width()+lw*2.0);
					maxy = qMax(maxy, currItem->yPos()-lw + currItem->height()+lw*2.0);
				}
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
			neu->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
			Elements.prepend(neu);
			m_Doc->GroupCounter++;
		}
	}
	m_Doc->DoDrawing = true;
	m_Doc->scMW()->ScriptRunning = false;
	if (interactive)
		m_Doc->setLoading(false);
	qApp->setOverrideCursor(QCursor(Qt::arrowCursor), true);
	if ((Elements.count() > 0) && (!ret) && (interactive))
	{
		if (flags & LoadSavePlugin::lfScripted)
		{
			bool loadF = m_Doc->isLoading();
			m_Doc->setLoading(false);
			m_Doc->changed();
			m_Doc->setLoading(loadF);
			for (int dre=0; dre<Elements.count(); ++dre)
			{
 				m_Doc->m_Selection->addItem(Elements.at(dre), true);
			}
	 		m_Doc->m_Selection->setGroupRect();
			m_Doc->view()->updatesOn(true);
			importCanceled = false;
		}
		else
		{
			m_Doc->DragP = true;
			m_Doc->DraggedElem = 0;
			m_Doc->DragElements.clear();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				m_Doc->DragElements.append(Elements.at(dre)->ItemNr);
				m_tmpSel->addItem(Elements.at(dre), true);
			}
			ScriXmlDoc *ss = new ScriXmlDoc();
			m_tmpSel->setGroupRect();
			QMimeData* md = new QMimeData();
			md->setText(ss->WriteElem(m_Doc, m_Doc->view(), m_tmpSel));
			QDrag* dr = new QDrag(m_Doc->view()->viewport());
			dr->setMimeData(md);
#ifndef QT_MAC
// see #2526
			m_Doc->itemSelection_DeleteItem(m_tmpSel);
#endif
			m_Doc->view()->resizeContents(qRound((maxSize.x() - minSize.x()) * m_Doc->view()->scale()), qRound((maxSize.y() - minSize.y()) * m_Doc->view()->scale()));
			m_Doc->view()->scrollBy(qRound((m_Doc->minCanvasCoordinate.x() - minSize.x()) * m_Doc->view()->scale()), qRound((m_Doc->minCanvasCoordinate.y() - minSize.y()) * m_Doc->view()->scale()));
			m_Doc->minCanvasCoordinate = minSize;
			m_Doc->maxCanvasCoordinate = maxSize;
			m_Doc->view()->updatesOn(true);
			dr->setPixmap(loadIcon("DragPix.xpm"));
			const QPixmap& dragCursor = loadIcon("DragPix.xpm");
			dr->setDragCursor(dragCursor, Qt::CopyAction);
			dr->setDragCursor(dragCursor, Qt::MoveAction);
			dr->setDragCursor(dragCursor, Qt::LinkAction);
			dr->start();
			delete ss;
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
	return true;
}

QList<PageItem*> WMFImport::parseWmfCommands(void)
{
	int idx, i;
    WmfCmd* cmd;
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

	double scale = (m_Dpi > 288) ? 288.0 / m_Dpi : 1.0;
    m_context.setWindowOrg( m_BBox.left(), m_BBox.top() );
	m_context.setWindowExt( abs(m_BBox.width()) * scale, abs(m_BBox.height()) * scale );
	/*if ( mAbsoluteCoord ) {
        mPainter.setWindow( mBBox.top(), mBBox.left(), mBBox.width(), mBBox.height() );
    }*/

    for ( cmd = m_FirstCmd; cmd; cmd=cmd->next )
    {
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
            cerr << str.ascii() << endl;
        }
    }
/*
    // TODO: cleanup this code when QPicture::setBoundingBox() is possible in KOClipart (QT31)
    // because actually QPicture::boundingBox() != mBBox()
    mWindowsCoord += 1;
    if ( mWindowsCoord == 2 )  {
        kdDebug() << "DRAW ANGLES " << endl;
        mPainter.setPen( Qt::white );
        mPainter.drawPoint( mBBox.left(), mBBox.top()  );
        mPainter.drawPoint( mBBox.right(), mBBox.bottom() );
    }
*/
	return elements;
}

void WMFImport::finishCmdParsing( PageItem* item )
{
	QMatrix gcm  = m_context.matrix();
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double coeff1 = sqrt(gcm.m11() * gcm.m11() + gcm.m12() * gcm.m12());
	double coeff2 = sqrt(gcm.m21() * gcm.m21() + gcm.m22() * gcm.m22());
	if( item->asImageFrame() )
	{
		item->moveBy(gcm.dx(), gcm.dy());
		item->setWidthHeight(item->width() * gcm.m11(), item->height() * gcm.m22());
		item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
		if (item->PicAvail)
			item->setImageXYScale(item->width() / item->pixm.width(), item->height() / item->pixm.height());
	}
	else if( item->asTextFrame() )
	{
		item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
	}
	else
	{
		item->ClipEdited = true;
		item->FrameType = 3;
		item->PoLine.map(gcm);
		item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
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
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, lineWidth, "None", lineColor, true);
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
	QString fillColor   = doFill ? importColor( m_context.brush().color() ) : "None";
	QString strokeColor = doStroke ? importColor( m_context.pen().color() ) : "None";
	double  lineWidth   = m_context.pen().width();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, rx * 2.0, ry * 2.0, lineWidth, fillColor, strokeColor, true);
	PageItem* ite = m_Doc->Items->at(z);
	QMatrix mm = m_context.matrix();
	mm.translate(px, py);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
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
		ite->fillRule = m_context.windingFill();
		finishCmdParsing(ite);
		items.append(ite);
	}
}

void WMFImport::polyPolygon( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	cerr << "WMFImport::polyPolygon unimplemented" << endl;
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
	double x = params[3];
	double y = params[2];
	double width  = params[3] - params[1];
	double height = params[2] - params[0];
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, width, height, lineWidth, fillColor, strokeColor, true);
	PageItem* ite = m_Doc->Items->at(z);
	QMatrix mm = QMatrix();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
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
	double x = params[5];
	double y = params[4];
	double width  = params[5] - params[3];
	double height = params[4] - params[2];
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
	QMatrix mm = QMatrix();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	finishCmdParsing(ite);
	items.append(ite);
}

void WMFImport::arc( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	unsupported = true;
	cerr << "WMFImport::arc unimplemented" << endl;
}

void WMFImport::chord( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	unsupported = true;
	cerr << "WMFImport::chord unimplemented" << endl;
}

void WMFImport::pie( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	unsupported = true;
	cerr << "WMFImport::pie unimplemented" << endl;
}

void WMFImport::setPolyFillMode( QList<PageItem*>& /*items*/, long, short* params )
{
	//cerr << "WMFImport::setPolyFillMode unimplemented" << endl;
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
	cerr << "WMFImport::intersectClipRect unimplemented" << endl;
}

void WMFImport::setTextColor( QList<PageItem*>& /*items*/, long, short* params )
{
	m_context.setTextColor( colorFromParam(params) );
}

void WMFImport::setTextAlign( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	unsupported = true;
	cerr << "WMFImport::setTextAlign unimplemented" << endl;
}

void WMFImport::textOut( QList<PageItem*>& /*items*/, long, short* /*params*/ )
{
	unsupported = true;
	cerr << "WMFImport::textOut unimplemented" << endl;
}

void WMFImport::extTextOut( QList<PageItem*>& /*items*/, long, short* params )
{
	unsupported = true;
	cerr << "WMFImport::extTextOut unimplemented" << endl;
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

    handle->rotation = -params[ 2 ]  / 10;               // text rotation (in 1/10 degree)
                                                // TODO: memorisation of rotation in object Font
    handle->font.setFamily( family );
    handle->font.setFixedPitch( ((params[ 8 ] & 0x01) == 0) );
    // TODO: investigation why some test case need -2. (size of font in logical point)
    handle->font.setPointSize( QABS(params[ 0 ]) - 2 );
    handle->font.setWeight( (params[ 4 ] >> 3) );
    handle->font.setItalic( (params[ 5 ] & 0x01) );
    handle->font.setUnderline( (params[ 5 ] & 0x100) );
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
