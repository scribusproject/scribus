#include "scpreview.h"
#include <qtextstream.h>
#include <qdom.h>
#include <qmap.h>
#include <qpainter.h>
#include <qrect.h>
#include <qimage.h>
#include <qregion.h>
#include <qregexp.h>
#include <qfileinfo.h>
#include <qbitmap.h>
#include <qdir.h>
#include <cmath>
#include <cstdlib>
#include "missing.h"
#include "scribus.h"
#include "splash.h"
#include "util.h"
#include "prefsmanager.h"

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
	struct CopyPasteBuffer OB;
	struct ScText *hg;
	struct ScText *hl;
	QPtrList<ScText> Ptexti;
	bool error;
	ScColor lf = ScColor();
	QFont fo;
	QMap<QString,QString> DoFonts;
	QMap<QString,QFont> DoFonts2;
	QMap<int,int> arrowID;
	QString tmpf, tmpx, tmp2, tmp3;
	int x, y;
	double xf, yf, asce;
	QPainter pm;
	QPainter pb;
	QPainter pd;
	QBitmap bmd;
	QPixmap pmd;
	QImage ip2;
	FPoint gv;
	int chs;
	QPointArray cl;
	QColor tmpfa;
	QString chx;
	uint a, zae;
	double CurY, EndX, CurX, wide, rota, wid;
	QValueList<ArrowDesc> arrowStyles;
	arrowStyles = prefsManager->appPrefs.arrowStyles;
	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSELEM") && (elem.tagName() != "SCRIBUSELEMUTF8"))
	{
		QPixmap tmp = QPixmap(0, 0);
		return tmp;
	}
	double GrX = QStodouble(elem.attribute("XP"));
	double GrY = QStodouble(elem.attribute("YP"));
	double GrW = QStodouble(elem.attribute("W"));
	double GrH = QStodouble(elem.attribute("H"));
	double pmmax = prefsManager->appPrefs.PSize / QMAX(GrW+50, GrH+50);
	QPixmap tmp = QPixmap(static_cast<int>(GrW)+50, static_cast<int>(GrH)+50);
	ScPainter *pS = new ScPainter(&tmp, tmp.width(), tmp.height());
	pS->translate(25,25);
	QDomNode DOC=elem.firstChild();
	DoFonts.clear();
	FT_Init_FreeType( &library );
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="Arrows")
		{
			struct ArrowDesc arrow;
			double xa, ya;
			arrow.name = pg.attribute("Name");
			QString tmp = pg.attribute("Points");
			QTextStream fp(&tmp, IO_ReadOnly);
			for (uint cx = 0; cx < pg.attribute("NumPoints").toUInt(); ++cx)
			{
				fp >> xa;
				fp >> ya;
				arrow.points.addPoint(xa, ya);
			}
			arrowStyles.append(arrow);
			arrowID.insert(QStoInt(pg.attribute("Index")), arrowStyles.count());
		}
		if(pg.tagName()=="FONT")
		{
			tmpf = GetAttr(&pg, "NAME");
			if ((!prefsManager->appPrefs.AvailFonts.find(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf]->UseFont))
			{
				if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
				{
					if (ScApp->splashScreen != NULL)
						ScApp->splashScreen->hide();
					MissingFont *dia = new MissingFont(0, tmpf, 0);
					dia->exec();
					tmpf = dia->getReplacementFont();
					delete dia;
					if (ScApp->splashScreen != NULL)
						ScApp->splashScreen->show();
					prefsManager->appPrefs.GFontSub[pg.attribute("NAME")] = tmpf;
				}
				else
					tmpf = prefsManager->appPrefs.GFontSub[tmpf];
			}
			if (!DoFonts2.contains(tmpf))
			{
				FT_Face      face;
				error = FT_New_Face( library, prefsManager->appPrefs.AvailFonts[tmpf]->Datei, prefsManager->appPrefs.AvailFonts[tmpf]->faceIndex, &face );
				if (error)
					tmpf = prefsManager->appPrefs.toolSettings.defFont;
				if (prefsManager->appPrefs.AvailFonts[tmpf]->ReadMetrics())
					prefsManager->appPrefs.AvailFonts[tmpf]->CharWidth[13] = 0;
				else
				{
					tmpf = prefsManager->appPrefs.toolSettings.defFont;
					prefsManager->appPrefs.AvailFonts[tmpf]->ReadMetrics();
					prefsManager->appPrefs.AvailFonts[tmpf]->CharWidth[13] = 0;
				}
			}
			fo = prefsManager->appPrefs.AvailFonts[tmpf]->Font;
			fo.setPointSize(12);
			DoFonts[GetAttr(&pg, "NAME")] = tmpf;
			DoFonts2[GetAttr(&pg, "NAME")] = fo;
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
				sl.Dash = QStoInt(MuL.attribute("Dash"));
				sl.LineEnd = QStoInt(MuL.attribute("LineEnd"));
				sl.LineJoin = QStoInt(MuL.attribute("LineJoin"));
				sl.Shade = QStoInt(MuL.attribute("Shade"));
				sl.Width = QStodouble(MuL.attribute("Width"));
				ml.push_back(sl);
				MuLn = MuLn.nextSibling();
			}
			if (!MLineStyles.contains(pg.attribute("Name")))
				MLineStyles.insert(pg.attribute("Name"), ml);
		}
		DOC=DOC.nextSibling();
	}
	DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="ITEM")
		{
			QString CurDirP = QDir::currentDirPath();
			QDir::setCurrent(QDir::homeDirPath());
			Segments.clear();
			// TODO: Nicer conversion
			OB.PType = static_cast<PageItem::ItemType>(QStoInt(pg.attribute("PTYPE")));
			OB.Xpos = QStodouble(pg.attribute("XPOS")) - GrX;
			OB.Ypos = QStodouble(pg.attribute("YPOS")) - GrY;
			OB.Width = QStodouble(pg.attribute("WIDTH"));
			OB.Height = QStodouble(pg.attribute("HEIGHT"));
			OB.RadRect = QStodouble(pg.attribute("RADRECT","0"));
			OB.ClipEdited = QStoInt(pg.attribute("CLIPEDIT", "0"));
			OB.FrameType = QStoInt(pg.attribute("FRTYPE", "0"));
			OB.Pwidth = QStodouble(pg.attribute("PWIDTH"));
			OB.Pcolor = pg.attribute("PCOLOR");
			OB.Pcolor2 = pg.attribute("PCOLOR2");
			OB.NamedLStyle = pg.attribute("NAMEDLST", "");
			if (!MLineStyles.contains(OB.NamedLStyle))
				OB.NamedLStyle = "";
			OB.Shade = QStoInt(pg.attribute("SHADE"));
			OB.Shade2 = QStoInt(pg.attribute("SHADE2"));
			OB.TxtFill = pg.attribute("TXTFILL", "Black");
			OB.TxtStroke = pg.attribute("TXTSTROKE", "None");
			OB.ShTxtFill = QStoInt(pg.attribute("TXTFILLSH", "100"));
			OB.ShTxtStroke = QStoInt(pg.attribute("TXTSTRSH", "100"));
			OB.TxtScale = qRound(QStodouble(pg.attribute("TXTSCALE", "100")) * 10);
			OB.TxtScaleV = qRound(QStodouble(pg.attribute("TXTSCALEV", "100")) * 10);
			OB.TxTBase = qRound(QStodouble(pg.attribute("TXTBASE", "0")) * 10);
			OB.TxTStyle = QStoInt(pg.attribute("TXTSTYLE", "0"));
			OB.GrType = QStoInt(pg.attribute("GRTYP","0"));
			OB.fill_gradient.clearStops();
			if (OB.GrType != 0)
			{
				OB.GrStartX = QStodouble(pg.attribute("GRSTARTX","0.0"));
				OB.GrStartY = QStodouble(pg.attribute("GRSTARTY","0.0"));
				OB.GrEndX = QStodouble(pg.attribute("GRENDX","0.0"));
				OB.GrEndY = QStodouble(pg.attribute("GRENDY","0.0"));
				OB.GrColor = pg.attribute("GRCOLOR","");
				if (!OB.GrColor.isEmpty())
				{
					OB.GrColor2 = pg.attribute("GRCOLOR2","");
					OB.GrShade = QStoInt(pg.attribute("GRSHADE","100"));
					OB.GrShade2 = QStoInt(pg.attribute("GRSHADE2","100"));
				}
			}
			OB.Rot = QStodouble(pg.attribute("ROT"));
			OB.PLineArt = Qt::PenStyle(QStoInt(pg.attribute("PLINEART")));
			OB.PLineEnd = Qt::PenCapStyle(QStoInt(pg.attribute("PLINEEND","0")));
			OB.PLineJoin = Qt::PenJoinStyle(QStoInt(pg.attribute("PLINEJOIN","0")));
			OB.LineSp = QStodouble(pg.attribute("LINESP"));
			OB.ExtraV = QStodouble(pg.attribute("EXTRAV","0"));
			OB.LocalScX = QStodouble(pg.attribute("LOCALSCX"));
			OB.LocalScY = QStodouble(pg.attribute("LOCALSCY"));
			OB.LocalX = QStodouble(pg.attribute("LOCALX"));
			OB.LocalY = QStodouble(pg.attribute("LOCALY"));
			OB.PicArt = QStoInt(pg.attribute("PICART"));
			OB.flippedH = QStoInt(pg.attribute("FLIPPEDH"));
			OB.flippedV = QStoInt(pg.attribute("FLIPPEDV"));
			OB.ScaleType = QStoInt(pg.attribute("SCALETYPE","1"));
			OB.AspectRatio = QStoInt(pg.attribute("RATIO","0"));
			OB.BBoxX = QStodouble(pg.attribute("BBOXX"));
			OB.BBoxH = QStodouble(pg.attribute("BBOXH"));
			OB.isPrintable = QStoInt(pg.attribute("PRINTABLE"));
			OB.isBookmark = false;
			OB.BMnr = 0;
			OB.isAnnotation = QStoInt(pg.attribute("ANNOTATION","0"));
			if (QStoInt(pg.attribute("TRANSPARENT","0")) == 1)
				OB.Pcolor = "None";
			OB.Textflow = QStoInt(pg.attribute("TEXTFLOW"));
			OB.Textflow2 = QStoInt(pg.attribute("TEXTFLOW2","0"));
			OB.Extra = QStodouble(pg.attribute("EXTRA"));
			OB.TExtra = QStodouble(pg.attribute("TEXTRA", "1"));
			OB.BExtra = QStodouble(pg.attribute("BEXTRA", "1"));
			OB.RExtra = QStodouble(pg.attribute("REXTRA", "1"));
			OB.PoShow = QStoInt(pg.attribute("PTLSHOW","0"));
			OB.BaseOffs = QStodouble(pg.attribute("BASEOF","0"));
			OB.textAlignment = QStoInt(pg.attribute("ALIGN","0"));
			OB.IFont = DoFonts[pg.attribute("IFONT")];
			OB.ISize = qRound(QStodouble(pg.attribute("ISIZE","12")) * 10.0);
			OB.Pfile = pg.attribute("PFILE");
			OB.Pfile2 = pg.attribute("PFILE2","");
			OB.Pfile3 = pg.attribute("PFILE3","");
			OB.IProfile = pg.attribute("PRFILE","");
			OB.EmProfile = pg.attribute("EPROF","");
			OB.IRender = QStoInt(pg.attribute("IRENDER","1"));
			OB.UseEmbedded = QStoInt(pg.attribute("EMBEDDED","1"));
			OB.Locked = static_cast<bool>(QStoInt(pg.attribute("LOCK","0")));
			OB.Reverse = static_cast<bool>(QStoInt(pg.attribute("REVERS","0")));
			OB.Transparency = QStodouble(pg.attribute("TransValue","0.0"));
			if (pg.hasAttribute("TransValueS"))
				OB.TranspStroke = QStodouble(pg.attribute("TransValueS","0.0"));
			else
				OB.TranspStroke = OB.Transparency;
			if (pg.hasAttribute("NUMCLIP"))
			{
				OB.Clip.resize(pg.attribute("NUMCLIP").toUInt());
				tmpx = pg.attribute("CLIPCOOR");
				QTextStream f(&tmpx, IO_ReadOnly);
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
				QTextStream fp(&tmpx, IO_ReadOnly);
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
					double ramp = QStodouble(it.attribute("RAMP","0.0"));
					int shade = QStoInt(it.attribute("SHADE","100"));
					SetFarbe(&tmpfa, name, shade);
					OB.fill_gradient.addStop(tmpfa, ramp, 0.5, 1.0, name, shade);
				}
				if (it.tagName()=="ITEXT")
				{
					tmp2 = it.attribute("CH");
					tmp2.replace(QRegExp("\r"), QChar(5));
					tmp2.replace(QRegExp("\n"), QChar(5));
					tmp3 = "\t" + DoFonts[it.attribute("CFONT")] + "\t";
					tmp3 += it.attribute("CSIZE") + "\t";
					tmp3 += it.attribute("CCOLOR") + "\t";
//					tmp3 += it.attribute("CEXTRA") + "\t";
					tmp3 += "0\t";
					tmp3 += it.attribute("CSHADE") + "\t";
					tmp3 += it.attribute("CSTYLE") + "\t";
					tmp3 += "0\t";
					tmp3 += it.attribute("CSTROKE","None") + "\t";
					tmp3 += it.attribute("CSHADE2","100") + "\t";
					tmp3 += it.attribute("CSCALE","100") + "\n";
					for (uint cxx=0; cxx<tmp2.length(); cxx++)
					{
						tmpx += tmp2.at(cxx)+tmp3;
					}
				}
				IT=IT.nextSibling();
			}
			OB.itemText = tmpx;
			if (!OB.itemText.isEmpty())
			{
				QTextStream t(&OB.itemText, IO_ReadOnly);
				QString cc;
				while (!t.atEnd())
				{
					cc = t.readLine();
					if (cc.isEmpty())
						continue;
					QStringList wt;
					QStringList::Iterator it;
					wt = QStringList::split("\t", cc);
					it = wt.begin();
					hg = new ScText;
					hg->ch = (*it);
					if (hg->ch == QChar(5))
						hg->ch = QChar(13);
					it++;
					hg->cfont = prefsManager->appPrefs.AvailFonts[*it];
					it++;
					hg->csize = qRound((*it).toDouble() * 10.0);
					it++;
					hg->ccolor = *it;
					it++;
					hg->cextra = (*it).toInt();
					it++;
					hg->cshade = (*it).toInt();
					hg->cselect = false;
					it++;
					if (it == NULL)
						hg->cstyle = 0;
					else
						hg->cstyle = (*it).toInt();
					it++;
					if (it == NULL)
						hg->cab = 0;
					else
						hg->cab = (*it).toInt();
					it++;
					if (it == NULL)
						hg->cstroke = "None";
					else
						hg->cstroke = *it;
					it++;
					if (it == NULL)
						hg->cshade2 = 100;
					else
						hg->cshade2 = (*it).toInt();
					it++;
					if (it == NULL)
						hg->cscale = 100;
					else
						hg->cscale = (*it).toInt();
					hg->xp = 0;
					hg->yp = 0;
					hg->PRot = 0;
					hg->PtransX = 0;
					hg->PtransY = 0;
					Ptexti.append(hg);
				}
			}
			tmpx = GetAttr(&pg, "TEXTCOOR","0 0");
			QTextStream ft(&tmpx, IO_ReadOnly);
			for (uint ct=0; ct<GetAttr(&pg, "NUMTEXT","0").toUInt(); ct++)
			{
				ft >> Ptexti.at(ct)->xp;
				ft >> Ptexti.at(ct)->yp;
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
			OB.startArrowIndex =  arrowID[QStoInt(pg.attribute("startArrowIndex","0"))];
			OB.endArrowIndex =  arrowID[QStoInt(pg.attribute("endArrowIndex","0"))];
			pS->save();
			pS->translate(OB.Xpos, OB.Ypos);
			pS->rotate(static_cast<double>(OB.Rot));
			if (OB.Pcolor != "None")
			{
				SetFarbe(&tmpfa, OB.Pcolor, OB.Shade);
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
						if ((OB.GrColor != "None") && (!OB.GrColor.isEmpty()))
							SetFarbe(&tmpfa, OB.GrColor, OB.GrShade);
						pS->fill_gradient.addStop(tmpfa, 0.0, 0.5, 1.0);
						if ((OB.GrColor2 != "None") && (!OB.GrColor2.isEmpty()))
							SetFarbe(&tmpfa, OB.GrColor2, OB.GrShade2);
						pS->fill_gradient.addStop(tmpfa, 1.0, 0.5, 1.0);
					}
					else
					{
						if ((OB.GrColor2 != "None") && (!OB.GrColor2.isEmpty()))
							SetFarbe(&tmpfa, OB.GrColor2, OB.GrShade2);
						pS->fill_gradient.addStop(tmpfa, 0.0, 0.5, 1.0);
						if ((OB.GrColor != "None") && (!OB.GrColor.isEmpty()))
							SetFarbe(&tmpfa, OB.GrColor, OB.GrShade);
						pS->fill_gradient.addStop(tmpfa, 1.0, 0.5, 1.0);
					}
				}
				else
					pS->fill_gradient = OB.fill_gradient;
				QWMatrix grm;
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
			if (OB.Pcolor2 != "None")
			{
				SetFarbe(&tmpfa, OB.Pcolor2, OB.Shade2);
				if ((OB.Pwidth == 0) && (OB.PType != 5))
					pS->setLineWidth(0);
				else
					pS->setPen(tmpfa, OB.Pwidth, Qt::PenStyle(OB.PLineArt), Qt::PenCapStyle(OB.PLineEnd), Qt::PenJoinStyle(OB.PLineJoin));
			}
			else
				pS->setLineWidth(0);
			pS->setBrushOpacity(1.0 - OB.Transparency);
			pS->setPenOpacity(1.0 - OB.TranspStroke);
			bool doStroke;
			int mode;
			doStroke = true;
			switch (OB.PType)
			{
			case PageItem::ImageFrame:
				if ((OB.Pcolor != "None") || (OB.GrType != 0))
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
						if (OB.flippedH % 2 != 0)
						{
							pS->translate(OB.Width, 0);
							pS->scale(-1, 1);
						}
						if (OB.flippedV % 2 != 0)
						{
							pS->translate(0, OB.Height);
							pS->scale(1, -1);
						}
						ScImage pixm;
						pixm.LoadPicture(OB.Pfile, "", 0, false, false, 1, 72);
						pS->scale(OB.LocalScX, OB.LocalScY);
						pS->translate(static_cast<int>(OB.LocalX), static_cast<int>(OB.LocalY));
						pS->drawImage(&pixm);
						pS->restore();
					}
				}
				break;
			case PageItem::TextFrame:
				if (Ptexti.count() != 0)
				{
					pS->save();
					if (OB.Pcolor != "None")
					{
						pS->setupPolygon(&OB.PoLine);
						pS->drawPolygon();
					}
					if (OB.flippedH % 2 != 0)
					{
						pS->translate(OB.Width, 0);
						pS->scale(-1, 1);
					}
					if (OB.flippedV % 2 != 0)
					{
						pS->translate(0, OB.Height);
						pS->scale(1, -1);
					}
					for (uint a = 0; a < Ptexti.count(); a++)
					{
						hl = Ptexti.at(a);
						if (hl->ch == QChar(13))
							continue;
						chx = hl->ch;
						chs = hl->csize;
						if (hl->cstyle != 0)
						{
							if (hl->cstyle & 1)
								chs = static_cast<int>(hl->csize * prefsManager->appPrefs.typographicSettings.scalingSuperScript / 100);
							if (hl->cstyle & 2)
								chs = static_cast<int>(hl->csize * prefsManager->appPrefs.typographicSettings.scalingSubScript / 100);
							if (hl->cstyle & 64)
							{
								if (chx.upper() != chx)
								{
									chs = static_cast<int>(hl->csize * prefsManager->appPrefs.typographicSettings.valueSmallCaps / 100);
									chx = chx.upper();
								}
							}
						}
						mode = 0;
						if (hl->ccolor != "None")
						{
							SetFarbe(&tmpfa, hl->ccolor, hl->cshade);
							pS->setBrush(tmpfa);
							mode = 2;
						}
						if (hl->cstroke != "None")
						{
							SetFarbe(&tmpfa, hl->cstroke, hl->cshade2);
							pS->setPen(tmpfa, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
							mode += 1;
						}
						DrawZeichenS(pS, hl->xp, hl->yp, chx, hl->cfont->SCName, OB.Reverse, hl->cstyle, mode, chs);
					}
					pS->restore();
				}
				break;
			case PageItem::Line:
				if (OB.NamedLStyle.isEmpty())
					pS->drawLine(FPoint(0, 0), FPoint(OB.Width, 0));
				else
				{
					multiLine ml = MLineStyles[OB.NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
						pS->setPen(tmpfa,
						           QMAX(static_cast<int>(ml[it].Width), 1),
						           static_cast<Qt::PenStyle>(ml[it].Dash),
						           static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
						           static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						pS->drawLine(FPoint(0, 0), FPoint(OB.Width, 0));
					}
				}
				if (OB.startArrowIndex != 0)
				{
					QWMatrix arrowTrans;
					FPointArray arrow = (*arrowStyles.at(OB.startArrowIndex-1)).points.copy();
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
					QWMatrix arrowTrans;
					FPointArray arrow = (*arrowStyles.at(OB.endArrowIndex-1)).points.copy();
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
				pS->setupPolygon(&OB.PoLine);
				if (OB.NamedLStyle.isEmpty())
					pS->drawPolyLine();
				else
				{
					multiLine ml = MLineStyles[OB.NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
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
							QWMatrix arrowTrans;
							FPointArray arrow = (*arrowStyles.at(OB.startArrowIndex-1)).points.copy();
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
							QWMatrix arrowTrans;
							FPointArray arrow = (*arrowStyles.at(OB.endArrowIndex-1)).points.copy();
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
				if (!OB.PoShow)
					doStroke = false;
				cl = FlattenPath(OB.PoLine, Segments);
				CurX = OB.Extra;
				if (Ptexti.count() != 0)
					CurX += Ptexti.at(0)->csize * Ptexti.at(0)->cextra / 10000.0;
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
					chx = hl->ch;
					if ((chx == QChar(30)) || (chx == QChar(13)))
						continue;
					if (hl->ccolor != "None")
					{
						SetFarbe(&tmpfa, hl->ccolor, hl->cshade);
						pS->setPen(tmpfa);
					}
					chs = hl->csize;
					asce = prefsManager->appPrefs.AvailFonts[hl->cfont->SCName]->numAscent * (hl->csize / 10.0);
					int chst = hl->cstyle & 127;
					if (chst != 0)
					{
						if (chst & 1)
						{
							CurY -= asce * prefsManager->appPrefs.typographicSettings.valueSuperScript / 100;
							chs = static_cast<int>(hl->csize * prefsManager->appPrefs.typographicSettings.scalingSuperScript / 100);
						}
						if (chst & 2)
						{
							CurY += asce * prefsManager->appPrefs.typographicSettings.valueSubScript / 100;
							chs = static_cast<int>(hl->csize * prefsManager->appPrefs.typographicSettings.scalingSubScript / 100);
						}
						if (chst & 64)
						{
							if (chx.upper() != chx)
							{
								chs = static_cast<int>(hl->csize * prefsManager->appPrefs.typographicSettings.valueSmallCaps / 100);
								chx = chx.upper();
							}
						}
					}
					wide = prefsManager->appPrefs.AvailFonts[hl->cfont->SCName]->CharWidth[chx[0].unicode()]*(chs / 10.0);
					if ((CurX+(wide+chs * hl->cextra / 10000.0)/2) >= wid)
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
					DrawZeichenS(pS, CurX+chs * hl->cextra / 10000.0, CurY+OB.BaseOffs, chx, hl->cfont->SCName, OB.Reverse, hl->cstyle, 2, chs);
					pS->restore();
					CurX += wide+chs * hl->cextra / 10000.0;
				}
			PfadEnd:	break;
			}
			if (doStroke)
			{
				if (OB.Pcolor2 != "None")
				{
					SetFarbe(&tmpfa, OB.Pcolor2, OB.Shade2);
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
						SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
						pS->setPen(tmpfa, ml[it].Width,
						           static_cast<Qt::PenStyle>(ml[it].Dash),
						           static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
						           static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						pS->drawPolyLine();
					}
				}
			}
			pS->restore();
			QDir::setCurrent(CurDirP);
		}
		DOC=DOC.nextSibling();
	}
	pS->end();
	QImage tmpi1 = tmp.convertToImage();
	QImage tmpi = tmpi1.smoothScale(static_cast<int>(tmp.width()*pmmax), static_cast<int>(tmp.height()*pmmax));
	tmp.convertFromImage(tmpi);
	delete pS;
	FT_Done_FreeType( library );
	return tmp;
}

void ScPreview::SetFarbe(QColor *tmp, QString farbe, int shad)
{
	*tmp = Farben[farbe].getShadeColorProof(shad);
}

void ScPreview::DrawZeichenS(ScPainter *p, double xco, double yco, QString ch, QString ZFo, bool Reverse, int Style, int mod, int Siz)
{
	if (mod == 0)
		return;
	QString ccx = ch;
	if (ccx == QChar(29))
		ccx = " ";
	double wide;
	double csi = static_cast<double>(Siz) / 100.0;
	uint chr = ccx[0].unicode();
	if (prefsManager->appPrefs.AvailFonts[ZFo]->CharWidth.contains(chr))
	{
		wide = prefsManager->appPrefs.AvailFonts[ZFo]->CharWidth[chr]*(Siz / 10.0);
		QWMatrix chma;
		chma.scale(csi, csi);
		FPointArray gly = prefsManager->appPrefs.AvailFonts[ZFo]->GlyphArray[chr].Outlines.copy();
		if (gly.size() < 4)
			return;
		gly.map(chma);
		chma = QWMatrix();
		p->setFillMode(1);
		if (Reverse)
		{
			chma.scale(-1, 1);
			chma.translate(-wide, 0);
			gly.map(chma);
			chma = QWMatrix();
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
			p->setLineWidth(prefsManager->appPrefs.AvailFonts[ZFo]->strokeWidth * Siz / 20);
			p->strokePath();
		}
		if (Style & 16)
		{
			double st = prefsManager->appPrefs.AvailFonts[ZFo]->strikeout_pos * (Siz / 10.0);
			p->setLineWidth(QMAX(prefsManager->appPrefs.AvailFonts[ZFo]->strokeWidth * (Siz / 10.0), 1));
			p->drawLine(FPoint(xco, yco-st), FPoint(xco+wide, yco-st));
		}
		if (Style & 8)
		{
			double st = prefsManager->appPrefs.AvailFonts[ZFo]->underline_pos * (Siz / 10.0);
			p->setLineWidth(QMAX(prefsManager->appPrefs.AvailFonts[ZFo]->strokeWidth * (Siz / 10.0), 1));
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
