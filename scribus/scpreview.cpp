/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scpreview.h"
#include <QDomDocument>
#include <QMap>
#include <QPainter>
#include <QRect>
#include <QImage>
#include <QRegion>
#include <QRegExp>
#include <QFileInfo>
#include <QBitmap>
#include <QDir>
#include <QList>
#include <QPolygon>
#include <QPixmap>
#include <QTemporaryFile>
#include <cstdlib>

#include "cmsettings.h"
#include "missing.h"
#include "sccolor.h"
#include "scclocale.h"
#include "scribus.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "splash.h"
#include "util.h"
#include "util_math.h"
#include "prefsmanager.h"
#include "scpainter.h"
#include "commonstrings.h"
#include "sccolorengine.h"


using namespace std;


ScPreview::ScPreview()
{
	prefsManager = PrefsManager::instance();
	Farben.clear();
	MLineStyles.clear();
	Segments.clear();
}

QPixmap ScPreview::createPreview(QString data)
{
	int a;
	struct CopyPasteBuffer OB;
	ScText *hg, *hl;
	QList<ScText*> Ptexti;
	ScColor lf = ScColor();
	QFont fo;
	QMap<QString,QString> DoFonts;
	QMap<int,int> arrowID;
	QString tmpf, tmpx, tmp2, tmp3;
	int x, y, zae;
	double xf, yf, asce, chs;
	FPoint gv;
	int currItem, fillBlendmode, strokeBlendmode;
	QPolygon cl;
	QColor tmpfa;
	QString chstr;
	double CurY, EndX, CurX, wide, rota, wid;
	QList<ArrowDesc> arrowStyles;
	QStack<int> groupStack;
	arrowStyles = prefsManager->appPrefs.arrowStyles;
	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSELEM") && (elem.tagName() != "SCRIBUSELEMUTF8"))
	{
		QPixmap tmp = QPixmap(0, 0);
		return tmp;
	}
	if (elem.hasAttribute("previewData"))
	{
		QString dat = elem.attribute("previewData", "");
		QByteArray inlineImageData;
		inlineImageData.append(dat);
		inlineImageData = QByteArray::fromBase64(inlineImageData);
		QImage tmp;
		tmp.loadFromData(inlineImageData);
		return QPixmap::fromImage(tmp);
	}
	double GrX = ScCLocale::toDoubleC(elem.attribute("XP"));
	double GrY = ScCLocale::toDoubleC(elem.attribute("YP"));
	double GrW = ScCLocale::toDoubleC(elem.attribute("W"));
	double GrH = ScCLocale::toDoubleC(elem.attribute("H"));
	double pmmax = 60 / qMax(GrW+50, GrH+50);
	QImage tmp = QImage(static_cast<int>(GrW)+50, static_cast<int>(GrH)+50, QImage::Format_ARGB32);
	tmp.fill( qRgba(255, 255, 255, 0) );
	ScPainter *pS = new ScPainter(&tmp, tmp.width(), tmp.height(), 1.0, 0);
	pS->beginLayer(1.0, 0);
	pS->translate(25,25);
	QDomNode DOC=elem.firstChild();
	DoFonts.clear();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="Arrows")
		{
			struct ArrowDesc arrow;
			double xa, ya;
			arrow.name = pg.attribute("Name");
			QString tmp = pg.attribute("Points");
			ScTextStream fp(&tmp, QIODevice::ReadOnly);
			for (uint cx = 0; cx < pg.attribute("NumPoints").toUInt(); ++cx)
			{
				fp >> xa;
				fp >> ya;
				arrow.points.addPoint(xa, ya);
			}
			arrowStyles.append(arrow);
			arrowID.insert(pg.attribute("Index").toInt(), arrowStyles.count());
		}
		if(pg.tagName()=="FONT")
		{
			tmpf = GetAttr(&pg, "NAME");
			if ((!prefsManager->appPrefs.AvailFonts.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf].usable()))
			{
				if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.GFontSub[tmpf]].usable()))
				{
					ScCore->showSplash(false);
					MissingFont *dia = new MissingFont(0, tmpf, 0);
					dia->exec();
					tmpf = dia->getReplacementFont();
					delete dia;
					ScCore->showSplash(true);
					prefsManager->appPrefs.GFontSub[pg.attribute("NAME")] = tmpf;
				}
				else
					tmpf = prefsManager->appPrefs.GFontSub[tmpf];
			}
			if (!DoFonts.contains(tmpf))
			{
//				FT_Face face;
//				error = FT_New_Face( library, prefsManager->appPrefs.AvailFonts[tmpf].fontFilePath(), prefsManager->appPrefs.AvailFonts[tmpf].faceIndex(), &face );
				if ( prefsManager->appPrefs.AvailFonts[tmpf].usable() )
					tmpf = prefsManager->appPrefs.toolSettings.defFont;
/*				if (prefsManager->appPrefs.AvailFonts[tmpf]->ReadMetrics()) {
//already done in ScFonts::addScalaableFont():
//					prefsManager->appPrefs.AvailFonts[tmpf]->CharWidth[13] = 0;
				}
				else
				{
					tmpf = prefsManager->appPrefs.toolSettings.defFont;
//					prefsManager->appPrefs.AvailFonts[tmpf]->ReadMetrics();
//already done in ScFonts::addScalaableFont():
//					prefsManager->appPrefs.AvailFonts[tmpf]->CharWidth[13] = 0;
				}
*/
			}
//			fo = prefsManager->appPrefs.AvailFonts[tmpf]->Font;
//			fo.setPointSize(12);
			DoFonts[GetAttr(&pg, "NAME")] = tmpf;
//			DoFonts2[GetAttr(&pg, "NAME")] = fo;
		}
		if(pg.tagName()=="COLOR")
		{
			if (pg.hasAttribute("CMYK"))
				lf.setNamedColor(GetAttr(&pg, "CMYK"));
			else
				lf.fromQColor(QColor(GetAttr(&pg, "RGB")));
			Farben[GetAttr(&pg, "NAME")] = lf;
		}
		if(pg.tagName()=="MultiLine")
		{
			multiLine ml;
			QDomNode MuLn = DOC.firstChild();
			while(!MuLn.isNull())
			{
				QDomElement MuL = MuLn.toElement();
				struct SingleLine sl;
				sl.Color = MuL.attribute("Color");
				sl.Dash = MuL.attribute("Dash").toInt();
				sl.LineEnd = MuL.attribute("LineEnd").toInt();
				sl.LineJoin = MuL.attribute("LineJoin").toInt();
				sl.Shade = MuL.attribute("Shade").toInt();
				sl.Width = ScCLocale::toDoubleC(MuL.attribute("Width"));
				ml.push_back(sl);
				MuLn = MuLn.nextSibling();
			}
			if (!MLineStyles.contains(pg.attribute("Name")))
				MLineStyles.insert(pg.attribute("Name"), ml);
		}
		DOC=DOC.nextSibling();
	}
	DOC=elem.firstChild();
	currItem = -1;
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="ITEM")
		{
			currItem++;
			QString CurDirP = QDir::currentPath();
			QDir::setCurrent(QDir::homePath());
			Segments.clear();
			// TODO: Nicer conversion
			OB.PType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			OB.Xpos = ScCLocale::toDoubleC(pg.attribute("XPOS")) - GrX;
			OB.Ypos = ScCLocale::toDoubleC(pg.attribute("YPOS")) - GrY;
			OB.Width = ScCLocale::toDoubleC(pg.attribute("WIDTH"));
			OB.Height = ScCLocale::toDoubleC(pg.attribute("HEIGHT"));
			OB.RadRect = ScCLocale::toDoubleC(pg.attribute("RADRECT"), 0.0);
			OB.ClipEdited = pg.attribute("CLIPEDIT", "0").toInt();
			OB.FrameType = pg.attribute("FRTYPE", "0").toInt();
			OB.Pwidth = ScCLocale::toDoubleC(pg.attribute("PWIDTH"));
			OB.Pcolor = pg.attribute("PCOLOR");
			OB.Pcolor2 = pg.attribute("PCOLOR2");
			OB.NamedLStyle = pg.attribute("NAMEDLST", "");
			if (!MLineStyles.contains(OB.NamedLStyle))
				OB.NamedLStyle = "";
			OB.Shade = pg.attribute("SHADE").toInt();
			OB.Shade2 = pg.attribute("SHADE2").toInt();
			OB.TxtFill = pg.attribute("TXTFILL", "Black");
			OB.TxtStroke = pg.attribute("TXTSTROKE", CommonStrings::None);
			OB.ShTxtFill = pg.attribute("TXTFILLSH", "100").toInt();
			OB.ShTxtStroke = pg.attribute("TXTSTRSH", "100").toInt();
			OB.TxtScale = qRound(ScCLocale::toDoubleC(pg.attribute("TXTSCALE"), 100.0) * 10);
			OB.TxtScaleV = qRound(ScCLocale::toDoubleC(pg.attribute("TXTSCALEV"), 100.0) * 10);
			OB.TxTBase = qRound(ScCLocale::toDoubleC(pg.attribute("TXTBASE"), 0.0) * 10);
			OB.TxTStyle = pg.attribute("TXTSTYLE", "0").toInt();
			OB.GrType = pg.attribute("GRTYP", "0").toInt();
			OB.fill_gradient.clearStops();
			if (OB.GrType != 0)
			{
				OB.GrStartX = ScCLocale::toDoubleC(pg.attribute("GRSTARTX"), 0.0);
				OB.GrStartY = ScCLocale::toDoubleC(pg.attribute("GRSTARTY"), 0.0);
				OB.GrEndX = ScCLocale::toDoubleC(pg.attribute("GRENDX"), 0.0);
				OB.GrEndY = ScCLocale::toDoubleC(pg.attribute("GRENDY"), 0.0);
				OB.GrColor = pg.attribute("GRCOLOR","");
				if (!OB.GrColor.isEmpty())
				{
					OB.GrColor2 = pg.attribute("GRCOLOR2","");
					OB.GrShade = pg.attribute("GRSHADE", "100").toInt();
					OB.GrShade2 = pg.attribute("GRSHADE2", "100").toInt();
				}
			}
			OB.Rot = ScCLocale::toDoubleC(pg.attribute("ROT"));
			OB.PLineArt = Qt::PenStyle(pg.attribute("PLINEART").toInt());
			OB.PLineEnd = Qt::PenCapStyle(pg.attribute("PLINEEND", "0").toInt());
			OB.PLineJoin = Qt::PenJoinStyle(pg.attribute("PLINEJOIN", "0").toInt());
			OB.LineSp = ScCLocale::toDoubleC(pg.attribute("LINESP"));
			OB.ExtraV = static_cast<int>(ScCLocale::toDoubleC(pg.attribute("EXTRAV"), 0.0)); // temporary compiler silencing
			OB.LocalScX = ScCLocale::toDoubleC(pg.attribute("LOCALSCX"));
			OB.LocalScY = ScCLocale::toDoubleC(pg.attribute("LOCALSCY"));
			OB.LocalX = ScCLocale::toDoubleC(pg.attribute("LOCALX"));
			OB.LocalY = ScCLocale::toDoubleC(pg.attribute("LOCALY"));
			OB.PicArt = pg.attribute("PICART").toInt();
			OB.flippedH = pg.attribute("FLIPPEDH").toInt();
			OB.flippedV = pg.attribute("FLIPPEDV").toInt();
			OB.ScaleType = pg.attribute("SCALETYPE", "1").toInt();
			OB.AspectRatio = pg.attribute("RATIO", "0").toInt();
			OB.isPrintable = pg.attribute("PRINTABLE").toInt();
			OB.isBookmark = false;
			OB.m_isAnnotation = pg.attribute("ANNOTATION", "0").toInt();
			if (pg.attribute("TRANSPARENT", "0").toInt() == 1)
				OB.Pcolor = CommonStrings::None;
			//OB.Textflow = pg.attribute("TEXTFLOW").toInt();
			//OB.Textflow2 = pg.attribute("TEXTFLOW2", "0").toInt();
			if ( pg.hasAttribute("TEXTFLOWMODE") )
				OB.TextflowMode = (PageItem::TextFlowMode) pg.attribute("TEXTFLOWMODE", "0").toInt();
			else if ( pg.attribute("TEXTFLOW").toInt() )
			{
				if (pg.attribute("TEXTFLOW2", "0").toInt())
					OB.TextflowMode = PageItem::TextFlowUsesBoundingBox;
				else if (pg.attribute("TEXTFLOW3", "0").toInt())
					OB.TextflowMode = PageItem::TextFlowUsesContourLine;
				else
					OB.TextflowMode = PageItem::TextFlowUsesFrameShape;	
			}
			else
				OB.TextflowMode = PageItem::TextFlowDisabled;
			OB.Extra = ScCLocale::toDoubleC(pg.attribute("EXTRA"));
			OB.TExtra = ScCLocale::toDoubleC(pg.attribute("TEXTRA"), 1.0);
			OB.BExtra = ScCLocale::toDoubleC(pg.attribute("BEXTRA"), 1.0);
			OB.RExtra = ScCLocale::toDoubleC(pg.attribute("REXTRA"), 1.0);
			OB.PoShow = pg.attribute("PTLSHOW", "0").toInt();
			OB.BaseOffs = ScCLocale::toDoubleC(pg.attribute("BASEOF"), 0.0);
			OB.textAlignment = pg.attribute("ALIGN", "0").toInt();
			OB.IFont = DoFonts[pg.attribute("IFONT")];
			OB.ISize = qRound(ScCLocale::toDoubleC(pg.attribute("ISIZE"), 12.0) * 10.0);
			if (OB.PType == PageItem::LatexFrame) {
				OB.Pfile = pg.attribute("LATEXTEMPFILE", ""); 
				//Hack to create a preview for latex frames
			} else {
				OB.Pfile = pg.attribute("PFILE");
			}
			OB.Pfile2 = pg.attribute("PFILE2","");
			OB.Pfile3 = pg.attribute("PFILE3","");
			OB.IProfile = pg.attribute("PRFILE","");
			OB.EmProfile = pg.attribute("EPROF","");
			OB.IRender = (eRenderIntent) pg.attribute("IRENDER", "1").toInt();
			OB.UseEmbedded = pg.attribute("EMBEDDED", "1").toInt();
			OB.Locked = static_cast<bool>(pg.attribute("LOCK", "0").toInt());
			OB.Reverse = static_cast<bool>(pg.attribute("REVERS", "0").toInt());
			OB.Transparency = ScCLocale::toDoubleC(pg.attribute("TransValue"), 0.0);
			if (pg.hasAttribute("TransValueS"))
				OB.TranspStroke = ScCLocale::toDoubleC(pg.attribute("TransValueS"), 0.0);
			else
				OB.TranspStroke = OB.Transparency;
			fillBlendmode = pg.attribute("TransBlend", "0").toInt();
			strokeBlendmode = pg.attribute("TransBlendS", "0").toInt();
			if (pg.hasAttribute("NUMCLIP"))
			{
				OB.Clip.resize(pg.attribute("NUMCLIP").toUInt());
				tmpx = pg.attribute("CLIPCOOR");
				ScTextStream f(&tmpx, QIODevice::ReadOnly);
				for (uint c=0; c<pg.attribute("NUMCLIP").toUInt(); ++c)
				{
					f >> x;
					f >> y;
					OB.Clip.setPoint(c, x, y);
				}
			}
			else
				OB.Clip.resize(0);
			if (pg.hasAttribute("NUMPO"))
			{
				OB.PoLine.resize(pg.attribute("NUMPO").toUInt());
				tmpx = pg.attribute("POCOOR");
				ScTextStream fp(&tmpx, QIODevice::ReadOnly);
				for (uint cx=0; cx<pg.attribute("NUMPO").toUInt(); ++cx)
				{
					fp >> xf;
					fp >> yf;
					OB.PoLine.setPoint(cx, xf, yf);
				}
			}
			else
				OB.PoLine.resize(0);
			OB.Groups.clear();
			if (OB.Clip.size() == 0)
				OB.Clip = FlattenPath(OB.PoLine, Segments);
			tmpx = "";
			QDomNode IT=DOC.firstChild();
			while(!IT.isNull())
			{
				QDomElement it=IT.toElement();
				if (it.tagName()=="CSTOP")
				{
					QString name = it.attribute("NAME");
					double ramp = ScCLocale::toDoubleC(it.attribute("RAMP"), 0.0);
					int shade = it.attribute("SHADE", "100").toInt();
					SetQColor(&tmpfa, name, shade);
					OB.fill_gradient.addStop(tmpfa, ramp, 0.5, 1.0, name, shade);
				}
				if (it.tagName()=="ITEXT")
				{
					tmp2 = it.attribute("CH");
					tmp2.replace(QRegExp("\r"), QChar(5));
					tmp2.replace(QRegExp("\n"), QChar(5));
					tmp3 = "\t" + DoFonts[it.attribute("CFONT", OB.IFont)] + "\t";
					tmp3 += it.attribute("CSIZE" , QString::number(OB.ISize)) + "\t";
					tmp3 += it.attribute("CCOLOR", OB.TxtFill) + "\t";
//					tmp3 += it.attribute("CEXTRA") + "\t";
					tmp3 += "0\t";
					tmp3 += it.attribute("CSHADE", QString::number(OB.ShTxtFill)) + "\t";
					tmp3 += it.attribute("CSTYLE") + "\t";
					tmp3 += "0\t";
					tmp3 += it.attribute("CSTROKE",CommonStrings::None) + "\t";
					tmp3 += it.attribute("CSHADE2","100") + "\t";
					tmp3 += it.attribute("CSCALE","100") + "\n";
					for (int cxx=0; cxx<tmp2.length(); cxx++)
					{
						tmpx += tmp2.at(cxx)+tmp3;
					}
				}
				if (it.tagName()=="PARA")
				{
					tmp2 = QChar(5);
					tmp3 = "\t" + DoFonts[it.attribute("FONT", OB.IFont)] + "\t";
					tmp3 += it.attribute("FONTSIZE" , QString::number(OB.ISize)) + "\t";
					tmp3 += it.attribute("FCOLOR", OB.TxtFill) + "\t";
					tmp3 += "0\t";
					tmp3 += it.attribute("FSHADE", QString::number(OB.ShTxtFill)) + "\t";
					tmp3 += it.attribute("CSTYLE") + "\t";
					tmp3 += "0\t";
					tmp3 += it.attribute("SCOLOR",CommonStrings::None) + "\t";
					tmp3 += it.attribute("SSHADE","100") + "\t";
					tmp3 += it.attribute("SCALEH","100") + "\n";
					tmpx += tmp2+tmp3;
				}
				IT=IT.nextSibling();
			}
			OB.itemText = tmpx;
			if (!OB.itemText.isEmpty())
			{
				ScTextStream t(&OB.itemText, QIODevice::ReadOnly);
				QString cc;
#ifndef NLS_PROTO
				while (!t.atEnd())
				{
					cc = t.readLine();
					if (cc.isEmpty())
						continue;
					QStringList wt;
					QStringList::Iterator it;
					wt = cc.split("\t", QString::SkipEmptyParts);
					it = wt.begin();
					hg = new ScText;
					hg->ch = it->at(0);
					if (hg->ch == QChar(5))
						hg->ch = QChar(13);
					it++;
					hg->setFont(prefsManager->appPrefs.AvailFonts[*it]);
					it++;
					hg->setFontSize(qRound(ScCLocale::toDoubleC((*it)) * 10.0));
					it++;
					hg->setFillColor(*it);
					it++;
					hg->setTracking((*it).toInt());
					it++;
					hg->setFillShade((*it).toInt());
					it++;
					// Qt4 if (it == NULL)
					if (it == wt.end())
						hg->setFeatures(QStringList(CharStyle::INHERIT));
					else
						hg->setFeatures(static_cast<StyleFlag>((*it).toInt()).featureList());
					it++;
//					if (it == NULL)
//						hg->cab = 0;
//					else
//						hg->cab = (*it).toInt();
					it++;
					if (it == wt.end())
						hg->setStrokeColor(CommonStrings::None);
					else
						hg->setStrokeColor(*it);
					it++;
					if (it == wt.end())
						hg->setStrokeShade(100);
					else
						hg->setStrokeShade((*it).toInt());
					it++;
					if (it == wt.end())
						hg->setScaleH(100);
					else
						hg->setScaleH((*it).toInt());

					Ptexti.append(hg);
				}
#endif
			}
			tmpx = GetAttr(&pg, "TEXTCOOR","0 0");
			ScTextStream ft(&tmpx, QIODevice::ReadOnly);
			for (uint ct=0; ct<GetAttr(&pg, "NUMTEXT","0").toUInt(); ct++)
			{
#ifndef NLS_PROTO
				ft >> Ptexti.at(ct)->glyph.xoffset;
				ft >> Ptexti.at(ct)->glyph.yoffset;
#endif
			}
			tmpx = "";
			if ((OB.PType == 5) && (OB.Height != 0))
			{
				OB.Rot += atan2(OB.Height,OB.Width)*(180.0/M_PI);
				OB.Width = sqrt(pow(OB.Width,2)+pow(OB.Height,2));
				OB.Height = 0;
				OB.Clip.setPoints(4, -1,-1, static_cast<int>(OB.Width+1),-1, static_cast<int>(OB.Width+1), static_cast<int>(OB.Height+1), -1, static_cast<int>(OB.Height+1));
			}
			OB.LayerNr = -1;
			OB.startArrowIndex =  arrowID[pg.attribute("startArrowIndex", "0").toInt()];
			OB.endArrowIndex =  arrowID[pg.attribute("endArrowIndex", "0").toInt()];
			if(static_cast<bool>(pg.attribute("isGroupControl", "0").toInt()))
			{
				pS->save();
				FPointArray cl = OB.PoLine.copy();
				QMatrix mm;
				mm.translate(OB.Xpos, OB.Ypos);
				mm.rotate(static_cast<double>(OB.Rot));
				cl.map( mm );
				pS->beginLayer(1.0 - OB.Transparency, pg.attribute("TransBlend", "0").toInt(), &cl);
				groupStack.push(pg.attribute("groupsLastItem", "0").toInt() + currItem);
				DOC=DOC.nextSibling();
				continue;
			}
			pS->save();
			if (fillBlendmode != 0)
				pS->beginLayer(1.0 - OB.Transparency, fillBlendmode);
			else
				pS->setBrushOpacity(1.0 - OB.Transparency);
			pS->translate(OB.Xpos, OB.Ypos);
			pS->rotate(static_cast<double>(OB.Rot));
			if (OB.Pcolor != CommonStrings::None)
			{
				SetQColor(&tmpfa, OB.Pcolor, OB.Shade);
				pS->setBrush(tmpfa);
				pS->setFillMode(1);
			}
			else
				pS->setFillMode(0);
			if (OB.GrType != 0)
			{
				pS->setFillMode(2);
				pS->fill_gradient.clearStops();
				if ((!OB.GrColor.isEmpty()) && (!OB.GrColor2.isEmpty()))
				{
					if (OB.GrType == 5)
					{
						if ((OB.GrColor != CommonStrings::None) && (!OB.GrColor.isEmpty()))
							SetQColor(&tmpfa, OB.GrColor, OB.GrShade);
						pS->fill_gradient.addStop(tmpfa, 0.0, 0.5, 1.0);
						if ((OB.GrColor2 != CommonStrings::None) && (!OB.GrColor2.isEmpty()))
							SetQColor(&tmpfa, OB.GrColor2, OB.GrShade2);
						pS->fill_gradient.addStop(tmpfa, 1.0, 0.5, 1.0);
					}
					else
					{
						if ((OB.GrColor2 != CommonStrings::None) && (!OB.GrColor2.isEmpty()))
							SetQColor(&tmpfa, OB.GrColor2, OB.GrShade2);
						pS->fill_gradient.addStop(tmpfa, 0.0, 0.5, 1.0);
						if ((OB.GrColor != CommonStrings::None) && (!OB.GrColor.isEmpty()))
							SetQColor(&tmpfa, OB.GrColor, OB.GrShade);
						pS->fill_gradient.addStop(tmpfa, 1.0, 0.5, 1.0);
					}
				}
				else
					pS->fill_gradient = OB.fill_gradient;
				QMatrix grm;
				grm.rotate(OB.Rot);
				FPointArray gra;
				switch (OB.GrType)
				{
				case 1:
					gra.setPoints(2, 0, 0, OB.Width, 0);
					gra.map(grm);
					pS->setGradient(VGradient::linear, gra.point(0), gra.point(1));
					break;
				case 2:
					gra.setPoints(2, 0, 0, OB.Height, 0);
					grm.rotate(90);
					gra.map(grm);
					pS->setGradient(VGradient::linear, gra.point(0), gra.point(1));
					break;
				case 3:
					gra.setPoints(2, 0, 0, OB.Width, OB.Height);
					gra.map(grm);
					pS->setGradient(VGradient::linear, gra.point(0), gra.point(1));
					break;
				case 4:
					gra.setPoints(2, 0, OB.Height, OB.Width, 0);
					gra.map(grm);
					pS->setGradient(VGradient::linear, gra.point(0), gra.point(1));
					break;
				case 5:
					if (OB.Width > OB.Height)
						gv = FPoint(OB.Width, OB.Height / 2.0);
					else
						gv = FPoint(OB.Width / 2.0, OB.Height);
					pS->setGradient(VGradient::radial, FPoint(OB.Width / 2.0,OB.Height / 2.0), gv, FPoint(OB.Width / 2.0,OB.Height / 2.0));
					break;
				case 6:
					gra.setPoints(2, OB.GrStartX, OB.GrStartY, OB.GrEndX, OB.GrEndY);
					gra.map(grm);
					pS->setGradient(VGradient::linear, gra.point(0), gra.point(1));
					break;
				}
			}
			if (OB.Pcolor2 != CommonStrings::None)
			{
				SetQColor(&tmpfa, OB.Pcolor2, OB.Shade2);
				if ((OB.Pwidth == 0) && (OB.PType != 5))
					pS->setLineWidth(0);
				else
					pS->setPen(tmpfa, OB.Pwidth, Qt::PenStyle(OB.PLineArt), Qt::PenCapStyle(OB.PLineEnd), Qt::PenJoinStyle(OB.PLineJoin));
			}
			else
				pS->setLineWidth(0);
			bool doStroke;
			int mode;
			doStroke = true;
			switch (OB.PType)
			{
			case PageItem::ImageFrame:
			case PageItem::LatexFrame:
				if ((OB.Pcolor != CommonStrings::None) || (OB.GrType != 0))
				{
					pS->setupPolygon(&OB.PoLine);
					pS->drawPolygon();
				}
				if (!OB.Pfile.isEmpty())
				{
					QFileInfo fi = QFileInfo(OB.Pfile);
					if (fi.exists())
					{
						pS->setupPolygon(&OB.PoLine);
						pS->setClipPath();
						pS->save();
						if (OB.flippedH)
						{
							pS->translate(OB.Width, 0);
							pS->scale(-1, 1);
						}
						if (OB.flippedV)
						{
							pS->translate(0, OB.Height);
							pS->scale(1, -1);
						}
						ScImage pixm;
						CMSettings cms(0, "", Intent_Perceptual);
						pixm.LoadPicture(OB.Pfile, 1, cms, false, false, ScImage::RGBData, 72); //FIXME: OB doesnt know about pagenr
						pS->scale(OB.LocalScX, OB.LocalScY);
						pS->translate(static_cast<int>(OB.LocalX), static_cast<int>(OB.LocalY));
						QImage img(pixm.qImage());
						pS->drawImage(&img);
						pS->restore();
					}
				}
				else
				{
					if (static_cast<bool>(pg.attribute("isInlineImage", "0").toInt()))
					{
						QByteArray inlineImageData;
						QString dat = pg.attribute("ImageData", "");
						inlineImageData.append(dat);
						QString inlineImageExt = pg.attribute("inlineImageExt", "");
						if (inlineImageData.size() > 0)
						{
							QTemporaryFile *tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + inlineImageExt);
							tempImageFile->open();
							QString fileName = getLongPathName(tempImageFile->fileName());
							tempImageFile->close();
							inlineImageData = qUncompress(QByteArray::fromBase64(inlineImageData));
							QFile outFil(fileName);
							if (outFil.open(QIODevice::WriteOnly))
							{
								outFil.write(inlineImageData);
								outFil.close();
								pS->setupPolygon(&OB.PoLine);
								pS->setClipPath();
								pS->save();
								if (OB.flippedH)
								{
									pS->translate(OB.Width, 0);
									pS->scale(-1, 1);
								}
								if (OB.flippedV)
								{
									pS->translate(0, OB.Height);
									pS->scale(1, -1);
								}
								ScImage pixm;
								CMSettings cms(0, "", Intent_Perceptual);
								pixm.LoadPicture(fileName, 1, cms, false, false, ScImage::RGBData, 72); //FIXME: OB doesnt know about pagenr
								pS->scale(OB.LocalScX, OB.LocalScY);
								pS->translate(static_cast<int>(OB.LocalX), static_cast<int>(OB.LocalY));
								QImage img(pixm.qImage());
								pS->drawImage(&img);
								pS->restore();
							}
							delete tempImageFile;
						}
					}
				}
				break;
			case PageItem::TextFrame:
#ifndef NLS_PROTO
				if (Ptexti.count() != 0)
				{
					pS->save();
					if (OB.Pcolor != CommonStrings::None)
					{
						pS->setupPolygon(&OB.PoLine);
						pS->drawPolygon();
					}
					if (OB.flippedH)
					{
						pS->translate(OB.Width, 0);
						pS->scale(-1, 1);
					}
					if (OB.flippedV)
					{
						pS->translate(0, OB.Height);
						pS->scale(1, -1);
					}
					if ((OB.Width < 4) || (OB.Height < 4))
						break;
					for (int a = 0; a < Ptexti.count(); a++)
					{
						hl = Ptexti.at(a);
						if (hl->ch == QChar(13))
							continue;
						chstr = hl->ch;
						chs = hl->fontSize();
						if (chs < 10)
							continue;
						if (hl->effects() != ScStyle_Default)
						{
							if (hl->effects() & 1)
								chs = static_cast<int>(hl->fontSize() * prefsManager->appPrefs.typographicSettings.scalingSuperScript / 100);
							if (hl->effects() & 2)
								chs = static_cast<int>(hl->fontSize() * prefsManager->appPrefs.typographicSettings.scalingSubScript / 100);
							if (hl->effects() & 64)
							{
								if (chstr.toUpper() != chstr)
								{
									chs = static_cast<int>(hl->fontSize() * prefsManager->appPrefs.typographicSettings.valueSmallCaps / 100);
									chstr = chstr.toUpper();
								}
							}
						}
						mode = 0;
						if (hl->fillColor() != CommonStrings::None)
						{
							SetQColor(&tmpfa, hl->fillColor(), hl->fillShade());
							pS->setBrush(tmpfa);
							mode = 2;
						}
						if (hl->strokeColor() != CommonStrings::None)
						{
							SetQColor(&tmpfa, hl->strokeColor(), hl->strokeShade());
							pS->setPen(tmpfa, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
							mode += 1;
						}
						DrawZeichenS(pS, hl->glyph.xoffset, hl->glyph.yoffset, chstr, hl->font().scName(), OB.Reverse, hl->effects(), mode, chs);
					}
					pS->restore();
				}
				break;
#endif
			case PageItem::Line:
				if (OB.NamedLStyle.isEmpty())
					pS->drawLine(FPoint(0, 0), FPoint(OB.Width, 0));
				else
				{
					multiLine ml = MLineStyles[OB.NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						SetQColor(&tmpfa, ml[it].Color, ml[it].Shade);
						pS->setPen(tmpfa,
						           qMax(static_cast<int>(ml[it].Width), 1),
						           static_cast<Qt::PenStyle>(ml[it].Dash),
						           static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
						           static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						pS->drawLine(FPoint(0, 0), FPoint(OB.Width, 0));
					}
				}
				if (OB.startArrowIndex != 0)
				{
					QMatrix arrowTrans;
					FPointArray arrow = arrowStyles.at(OB.startArrowIndex-1).points.copy();
					arrowTrans.translate(0, 0);
					arrowTrans.scale(OB.Pwidth, OB.Pwidth);
					arrowTrans.scale(-1,1);
					arrow.map(arrowTrans);
					pS->setBrush(pS->pen());
					pS->setLineWidth(0);
					pS->setFillMode(ScPainter::Solid);
					pS->setupPolygon(&arrow);
					pS->drawPolygon();
				}
				if (OB.endArrowIndex != 0)
				{
					QMatrix arrowTrans;
					FPointArray arrow = arrowStyles.at(OB.endArrowIndex-1).points.copy();
					arrowTrans.translate(OB.Width, 0);
					arrowTrans.scale(OB.Pwidth, OB.Pwidth);
					arrow.map(arrowTrans);
					pS->setBrush(pS->pen());
					pS->setLineWidth(0);
					pS->setFillMode(ScPainter::Solid);
					pS->setupPolygon(&arrow);
					pS->drawPolygon();
				}
				doStroke = false;
				break;
			/* OBSOLETE Craig R 2005-02-06
			case 1:
			case 3:
			*/
			case PageItem::ItemType1:
			case PageItem::ItemType3:
			case PageItem::Polygon:
				pS->setupPolygon(&OB.PoLine);
				pS->drawPolygon();
				break;
			case PageItem::PolyLine:
				if (OB.PoLine.size() < 4)
					break;
				if ((OB.Pcolor != CommonStrings::None) || (OB.GrType != 0))
				{
					FPointArray cli;
					FPoint Start;
					bool firstp = true;
					for (uint n = 0; n < OB.PoLine.size()-3; n += 4)
					{
						if (firstp)
						{
							Start = OB.PoLine.point(n);
							firstp = false;
						}
						if (OB.PoLine.point(n).x() > 900000)
						{
							cli.addPoint(OB.PoLine.point(n-2));
							cli.addPoint(OB.PoLine.point(n-2));
							cli.addPoint(Start);
							cli.addPoint(Start);
							cli.setMarker();
							firstp = true;
							continue;
						}
						cli.addPoint(OB.PoLine.point(n));
						cli.addPoint(OB.PoLine.point(n+1));
						cli.addPoint(OB.PoLine.point(n+2));
						cli.addPoint(OB.PoLine.point(n+3));
					}
					if (cli.size() > 2)
					{
						FPoint l1 = cli.point(cli.size()-2);
						cli.addPoint(l1);
						cli.addPoint(l1);
						cli.addPoint(Start);
						cli.addPoint(Start);
					}
					pS->setupPolygon(&cli);
					pS->fillPath();
				}
				pS->setupPolygon(&OB.PoLine, false);
				if (OB.NamedLStyle.isEmpty())
					pS->strokePath();
				else
				{
					multiLine ml = MLineStyles[OB.NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						SetQColor(&tmpfa, ml[it].Color, ml[it].Shade);
						pS->setPen(tmpfa, ml[it].Width,
						           static_cast<Qt::PenStyle>(ml[it].Dash),
						           static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
						           static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						pS->drawPolyLine();
					}
				}
				if (OB.startArrowIndex != 0)
				{
					FPoint Start = OB.PoLine.point(0);
					for (uint xx = 1; xx < OB.PoLine.size(); xx += 2)
					{
						FPoint Vector = OB.PoLine.point(xx);
						if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
						{
							double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
							QMatrix arrowTrans;
							FPointArray arrow = arrowStyles.at(OB.startArrowIndex-1).points.copy();
							arrowTrans.translate(Start.x(), Start.y());
							arrowTrans.rotate(r);
							arrowTrans.scale(OB.Pwidth, OB.Pwidth);
							arrow.map(arrowTrans);
							pS->setBrush(pS->pen());
							pS->setLineWidth(0);
							pS->setFillMode(ScPainter::Solid);
							pS->setupPolygon(&arrow);
							pS->drawPolygon();
							break;
						}
					}
				}
				if (OB.endArrowIndex != 0)
				{
					FPoint End = OB.PoLine.point(OB.PoLine.size()-2);
					for (uint xx = OB.PoLine.size()-1; xx > 0; xx -= 2)
					{
						FPoint Vector = OB.PoLine.point(xx);
						if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
						{
							double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
							QMatrix arrowTrans;
							FPointArray arrow = arrowStyles.at(OB.endArrowIndex-1).points.copy();
							arrowTrans.translate(End.x(), End.y());
							arrowTrans.rotate(r);
							arrowTrans.scale(OB.Pwidth, OB.Pwidth);
							arrow.map(arrowTrans);
							pS->setBrush(pS->pen());
							pS->setLineWidth(0);
							pS->setFillMode(ScPainter::Solid);
							pS->setupPolygon(&arrow);
							pS->drawPolygon();
							break;
						}
					}
				}
				doStroke = false;
				break;
			case PageItem::PathText:
#ifndef NLS_PROTO
				if (!OB.PoShow)
					doStroke = false;
				cl = FlattenPath(OB.PoLine, Segments);
				CurX = OB.Extra;
				if (Ptexti.count() != 0)
					CurX += Ptexti.at(0)->fontSize() * Ptexti.at(0)->tracking() / 10000.0;
				zae = 0;
				wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2.0)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2.0));
				while (wid < 1)
				{
					zae++;
					if (zae == cl.size()-1)
						goto PfadEnd;
					wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2.0)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2.0));
				}
				rota = xy2Deg(cl.point(zae+1).x()-cl.point(zae).x(),cl.point(zae+1).y()-cl.point(zae).y());
				for (a = 0; a < Ptexti.count(); a++)
				{
					CurY = 0;
					hl = Ptexti.at(a);
					chstr = hl->ch;
					if ((chstr == QChar(30)) || (chstr == QChar(13)))
						continue;
					if (hl->fillColor() != CommonStrings::None)
					{
						SetQColor(&tmpfa, hl->fillColor(), hl->fillShade());
						pS->setPen(tmpfa);
					}
					chs = hl->fontSize();
					asce = hl->font().ascent(hl->fontSize() / 10.0);
					int chst = hl->effects() & 127;
					if (chst != 0)
					{
						if (chst & 1)
						{
							CurY -= asce * prefsManager->appPrefs.typographicSettings.valueSuperScript / 100;
							chs = static_cast<int>(hl->fontSize() * prefsManager->appPrefs.typographicSettings.scalingSuperScript / 100);
						}
						if (chst & 2)
						{
							CurY += asce * prefsManager->appPrefs.typographicSettings.valueSubScript / 100;
							chs = static_cast<int>(hl->fontSize() * prefsManager->appPrefs.typographicSettings.scalingSubScript / 100);
						}
						if (chst & 64)
						{
							if (chstr.toUpper() != chstr)
							{
								chs = static_cast<int>(hl->fontSize() * prefsManager->appPrefs.typographicSettings.valueSmallCaps / 100);
								chstr = chstr.toUpper();
							}
						}
					}
					wide = hl->font().charWidth(chstr[0], chs / 10.0);
					if ((CurX+(wide+chs * hl->tracking() / 10000.0)/2) >= wid)
					{
						if (zae < cl.size()-1)
						{
							CurX = CurX - wid;
							wid = 0;
							EndX = CurX;
							do
							{
								do
								{
									zae++;
									if (zae == cl.size()-1)
										goto PfadEnd;
									wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2.0)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2.0));
									rota = xy2Deg(cl.point(zae+1).x()-cl.point(zae).x(),cl.point(zae+1).y()-cl.point(zae).y());
								}
								while (wid == 0);
								EndX -= wid;
							}
							while (wid < EndX);
							CurX = EndX + wid;
						}
						else
							goto PfadEnd;
					}
					pS->save();
					pS->translate(cl.point(zae).x(), cl.point(zae).y());
					pS->rotate(rota);
					DrawZeichenS(pS, CurX+chs * hl->tracking() / 10000.0, CurY+OB.BaseOffs, chstr, hl->font().scName(), OB.Reverse, hl->effects(), 2, chs);
					pS->restore();
					CurX += wide+chs * hl->tracking() / 10000.0;
				}
#endif
			PfadEnd:	break;
			case PageItem::Multiple:
				Q_ASSERT(false);
				break;
			}
			if (fillBlendmode != 0)
				pS->endLayer();
			if (doStroke)
			{
				if (strokeBlendmode != 0)
					pS->beginLayer(1.0 - OB.TranspStroke, strokeBlendmode);
				else
					pS->setPenOpacity(1.0 - OB.TranspStroke);
				if (OB.Pcolor2 != CommonStrings::None)
				{
					SetQColor(&tmpfa, OB.Pcolor2, OB.Shade2);
					pS->setPen(tmpfa, OB.Pwidth, Qt::PenStyle(OB.PLineArt), Qt::PenCapStyle(OB.PLineEnd), Qt::PenJoinStyle(OB.PLineJoin));
					//					if (DashValues.count() != 0)
					//						pS->setDash(DashValues, DashOffset);
				}
				else
					pS->setLineWidth(0);
				pS->setupPolygon(&OB.PoLine);
				if (OB.NamedLStyle.isEmpty())
					pS->drawPolyLine();
				else
				{
					multiLine ml = MLineStyles[OB.NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						SetQColor(&tmpfa, ml[it].Color, ml[it].Shade);
						pS->setPen(tmpfa, ml[it].Width,
						           static_cast<Qt::PenStyle>(ml[it].Dash),
						           static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
						           static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						pS->drawPolyLine();
					}
				}
				if (strokeBlendmode != 0)
					pS->endLayer();
			}
			pS->restore();
			if (groupStack.count() != 0)
			{
				while (currItem == groupStack.top())
				{
					pS->endLayer();
					pS->restore();
					groupStack.pop();
					if (groupStack.count() == 0)
						break;
				}
			}
			QDir::setCurrent(CurDirP);
		}
		DOC=DOC.nextSibling();
	}
	pS->endLayer();
	pS->end();
	QImage tmpi = tmp.scaled(static_cast<int>(tmp.width()*pmmax), static_cast<int>(tmp.height()*pmmax), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap ret;
	ret=QPixmap::fromImage(tmpi);
	delete pS;
//	FT_Done_FreeType( library );
	return ret;
}

void ScPreview::SetQColor(QColor *tmp, const QString& farbe, double shad)
{
	*tmp = ScColorEngine::getShadeColorProof(Farben[farbe], NULL, shad);
}

void ScPreview::DrawZeichenS(ScPainter *p, double xco, double yco, QString ch, QString ZFo, bool Reverse, int Style, int mod, double Siz)
{
	if (mod == 0)
		return;
	QString ccx = ch;
	if (ccx == QChar(29))
		ccx = " ";
	double wide;
	double csi = Siz / 100.0;
//	uint chr = ccx[0].unicode();
	if (prefsManager->appPrefs.AvailFonts[ZFo].canRender(ccx[0]))
	{
		wide = prefsManager->appPrefs.AvailFonts[ZFo].charWidth(ccx[0])*(Siz / 10.0);
		QMatrix chma;
		chma.scale(csi, csi);
		uint gl = prefsManager->appPrefs.AvailFonts[ZFo].char2CMap(ccx[0]);
		FPointArray gly = prefsManager->appPrefs.AvailFonts[ZFo].glyphOutline(gl);
		if (gly.size() < 4)
			return;
		gly.map(chma);
		chma = QMatrix();
		p->setFillMode(1);
		if (Reverse)
		{
			chma.scale(-1, 1);
			chma.translate(-wide, 0);
			gly.map(chma);
			chma = QMatrix();
			chma.translate(xco, yco-(Siz / 10.0));
		}
		else
			chma.translate(xco, yco-(Siz / 10.0));
		gly.map(chma);
		p->setupPolygon(&gly);
		p->setFillMode(1);
		if (mod > 1)
			p->fillPath();
		if ((Style & 4) && ((mod % 2) != 0))
		{
			p->setLineWidth(prefsManager->appPrefs.AvailFonts[ZFo].strokeWidth() * Siz / 20);
			p->strokePath();
		}
		if (Style & 16)
		{
			double st = prefsManager->appPrefs.AvailFonts[ZFo].strikeoutPos() * (Siz / 10.0);
			p->setLineWidth(qMax(prefsManager->appPrefs.AvailFonts[ZFo].strokeWidth() * (Siz / 10.0), 1.0));
			p->drawLine(FPoint(xco, yco-st), FPoint(xco+wide, yco-st));
		}
		if (Style & 8)
		{
			double st = prefsManager->appPrefs.AvailFonts[ZFo].underlinePos() * (Siz / 10.0);
			p->setLineWidth(qMax(prefsManager->appPrefs.AvailFonts[ZFo].strokeWidth() * (Siz / 10.0), 1.0));
			p->drawLine(FPoint(xco, yco-st), FPoint(xco+wide, yco-st));
		}
	}
	else
	{
		p->setLineWidth(1);
		p->setPen(Qt::black);
		p->setFillMode(0);
		p->drawRect(xco, yco-Siz, (Siz / 10.0), (Siz / 10.0));
	}
}
