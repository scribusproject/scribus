#include "scpreview.h"
#include <qtextstream.h>
#include <qdom.h>
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
#include "kimageeffect.h"
extern float QStoFloat(QString in);
extern int QStoInt(QString in);
extern QString GetAttr(QDomElement *el, QString at, QString def="0");
extern float xy2Deg(float x, float y);
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern QImage LoadPict(QString fn);

ScPreview::ScPreview(preV *prefs)
{
  Prefs = prefs;
	Farben.clear();
	MLineStyles.clear();
	Segments.clear();
}

QPixmap ScPreview::createPreview(QString data)
{
	struct CLBuf OB;
	struct Pti *hg;
	struct Pti *hl;
  QPtrList<Pti> Ptexti;
	CMYKColor lf = CMYKColor();
	QFont fo;
	QMap<QString,QString> DoFonts;
	QMap<QString,QFont> DoFonts2;
	QString tmpf, tmpx, tmp2, tmp3;
	int x, y;
	float xf, yf;
	QPainter pm;
	QPointArray Bez(4);
	int desc, asce, chs;
	QPointArray cl;
	QColor tmpfa;
	QString chx;
	QFont ffo;
	uint a, zae;
	float CurY, EndX, CurX, wide, rota, wid;
	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSELEM") && (elem.tagName() != "SCRIBUSELEMUTF8"))
		{
		QPixmap tmp = QPixmap(0, 0);
		return tmp;
		}
	float GrX = QStoFloat(elem.attribute("XP"));
	float GrY = QStoFloat(elem.attribute("YP"));
	float GrW = QStoFloat(elem.attribute("W"));
	float GrH = QStoFloat(elem.attribute("H"));
	float pmmax = Prefs->PSize / QMAX(GrW+30, GrH+30);
	QPixmap tmp = QPixmap(static_cast<int>(GrW)+30, static_cast<int>(GrH)+30);
	tmp.fill(Qt::white);
	pm.begin(&tmp);
	pm.translate(15,15);
	QDomNode DOC=elem.firstChild();
	DoFonts.clear();
	while(!DOC.isNull())
		{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="FONT")
			{
			tmpf = GetAttr(&pg, "NAME");
			if ((!Prefs->AvailFonts.find(tmpf)) || (!Prefs->AvailFonts[tmpf]->UseFont))
				{
				if ((!Prefs->GFontSub.contains(tmpf)) || (!Prefs->AvailFonts[Prefs->GFontSub[tmpf]]->UseFont))
					{
					DmF *dia = new DmF(0, tmpf, Prefs->AvailFonts);
					dia->exec();
					tmpf = dia->Ersatz;
					delete dia;
					Prefs->GFontSub[pg.attribute("NAME")] = tmpf;
					}
				else
					tmpf = Prefs->GFontSub[tmpf];
				}
			fo = Prefs->AvailFonts[tmpf]->Font;
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
				struct singleLine sl;
				sl.Color = MuL.attribute("Color");
				sl.Dash = QStoInt(MuL.attribute("Dash"));
				sl.LineEnd = QStoInt(MuL.attribute("LineEnd"));
				sl.LineJoin = QStoInt(MuL.attribute("LineJoin"));
				sl.Shade = QStoInt(MuL.attribute("Shade"));
				sl.Width = QStoFloat(MuL.attribute("Width"));
				ml.append(sl);
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
			QDir::setCurrent(QString(getenv("HOME")));
			Segments.clear();
			OB.PType = QStoInt(pg.attribute("PTYPE"));
			OB.Xpos = QStoFloat(pg.attribute("XPOS")) - GrX;
			OB.Ypos = QStoFloat(pg.attribute("YPOS")) - GrY;
			OB.Width = QStoFloat(pg.attribute("WIDTH"));
			OB.Height = QStoFloat(pg.attribute("HEIGHT"));
			OB.RadRect = QStoFloat(pg.attribute("RADRECT","0"));
			OB.ClipEdited = QStoInt(pg.attribute("CLIPEDIT", "0"));
			OB.FrameType = QStoInt(pg.attribute("FRTYPE", "0"));
			OB.Pwidth = QStoFloat(pg.attribute("PWIDTH"));
			OB.Pcolor = pg.attribute("PCOLOR");
			OB.Pcolor2 = pg.attribute("PCOLOR2");
			OB.NamedLStyle = pg.attribute("NAMEDLST", "");
			if (!MLineStyles.contains(OB.NamedLStyle))
				OB.NamedLStyle = "";
			OB.Shade = QStoInt(pg.attribute("SHADE"));
			OB.Shade2 = QStoInt(pg.attribute("SHADE2"));
			OB.GrColor = pg.attribute("GRCOLOR","");
			OB.GrColor2 = pg.attribute("GRCOLOR2","");
			OB.GrShade = QStoInt(pg.attribute("GRSHADE","100"));
			OB.GrShade2 = QStoInt(pg.attribute("GRSHADE2","100"));
			OB.GrType = QStoInt(pg.attribute("GRTYP","0"));
			OB.Rot = QStoFloat(pg.attribute("ROT"));
			OB.PLineArt = Qt::PenStyle(QStoInt(pg.attribute("PLINEART")));
			OB.PLineEnd = Qt::PenCapStyle(QStoInt(pg.attribute("PLINEEND","0")));
			OB.PLineJoin = Qt::PenJoinStyle(QStoInt(pg.attribute("PLINEJOIN","0")));
			OB.LineSp = QStoFloat(pg.attribute("LINESP"));
			OB.ExtraV = QStoFloat(pg.attribute("EXTRAV","0"));
			OB.LocalScX = QStoFloat(pg.attribute("LOCALSCX"));
			OB.LocalScY = QStoFloat(pg.attribute("LOCALSCY"));
			OB.LocalX = QStoFloat(pg.attribute("LOCALX"));
			OB.LocalY = QStoFloat(pg.attribute("LOCALY"));
			OB.PicArt = QStoInt(pg.attribute("PICART"));
			OB.flippedH = QStoInt(pg.attribute("FLIPPEDH"));
			OB.flippedV = QStoInt(pg.attribute("FLIPPEDV"));
			OB.ScaleType = QStoInt(pg.attribute("SCALETYPE","1"));
			OB.AspectRatio = QStoInt(pg.attribute("RATIO","0"));
			OB.BBoxX = QStoFloat(pg.attribute("BBOXX"));
			OB.BBoxH = QStoFloat(pg.attribute("BBOXH"));
			OB.isPrintable = QStoInt(pg.attribute("PRINTABLE"));
			OB.isBookmark = false;
			OB.BMnr = 0;
			OB.isAnnotation = QStoInt(pg.attribute("ANNOTATION","0"));
			if (QStoInt(pg.attribute("TRANSPARENT","0")) == 1)
				OB.Pcolor = "None";
			OB.Textflow = QStoInt(pg.attribute("TEXTFLOW"));
			OB.Textflow2 = QStoInt(pg.attribute("TEXTFLOW2","0"));
			OB.Extra = QStoFloat(pg.attribute("EXTRA"));
			OB.TExtra = QStoFloat(pg.attribute("TEXTRA", "1"));
			OB.BExtra = QStoFloat(pg.attribute("BEXTRA", "1"));
			OB.RExtra = QStoFloat(pg.attribute("REXTRA", "1"));
			OB.PoShow = QStoInt(pg.attribute("PTLSHOW","0"));
			OB.BaseOffs = QStoFloat(pg.attribute("BASEOF","0"));
			OB.Ausrich = QStoInt(pg.attribute("ALIGN","0"));
			OB.IFont = DoFonts[pg.attribute("IFONT")];
			OB.ISize = QStoInt(pg.attribute("ISIZE","12"));
			OB.Pfile = pg.attribute("PFILE");
			OB.Pfile2 = pg.attribute("PFILE2","");
			OB.Pfile3 = pg.attribute("PFILE3","");
			OB.IProfile = pg.attribute("PRFILE","");
			OB.EmProfile = pg.attribute("EPROF","");
			OB.IRender = QStoInt(pg.attribute("IRENDER","1"));
			OB.UseEmbedded = QStoInt(pg.attribute("EMBEDDED","1"));
			OB.Locked = static_cast<bool>(QStoInt(pg.attribute("LOCK","0")));
			OB.Reverse = static_cast<bool>(QStoInt(pg.attribute("REVERS","0")));
			OB.InvPict = static_cast<bool>(QStoInt(pg.attribute("INVERS","0")));
			OB.Transparency = QStoFloat(pg.attribute("TransValue","0.0"));
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
				if (it.tagName()=="ITEXT")
						{
						tmp2 = it.attribute("CH");
						tmp2.replace(QRegExp("\r"), QChar(5));
						tmp2.replace(QRegExp("\n"), QChar(5));
						tmp3 = "\t" + DoFonts[it.attribute("CFONT")] + "\t";
						tmp3 += it.attribute("CSIZE") + "\t";
						tmp3 += it.attribute("CCOLOR") + "\t";
						tmp3 += it.attribute("CEXTRA") + "\t";
						tmp3 += it.attribute("CSHADE") + "\t";
						tmp3 += it.attribute("CSTYLE") + "\t";
						tmp3 += "0\n";
						for (uint cxx=0; cxx<tmp2.length(); cxx++)
							{
							tmpx += tmp2.at(cxx)+tmp3;
							}
						}
					else
						{
						tmpx += QString(QChar(QStoInt(it.attribute("CH")))) + "\t";
						tmpx += DoFonts[it.attribute("CFONT")] + "\t";
						tmpx += it.attribute("CSIZE") + "\t";
						tmpx += it.attribute("CCOLOR") + "\t";
						tmpx += it.attribute("CEXTRA") + "\t";
						tmpx += it.attribute("CSHADE") + "\t";
						tmpx += it.attribute("CSTYLE") + "\t";
						tmpx += "0\n";
						}
					IT=IT.nextSibling();
				}
			OB.Ptext = tmpx;
			if (OB.Ptext != "")
				{
				QTextStream t(&OB.Ptext, IO_ReadOnly);
				QString cc;
				while (!t.atEnd())
					{
					cc = t.readLine();
					if (cc == "")
						continue;
					QStringList wt;
					QStringList::Iterator it;
					wt = QStringList::split("\t", cc);
					it = wt.begin();
					hg = new Pti;
					hg->ch = (*it);
					if (hg->ch == QChar(5))
						hg->ch = QChar(13);
					it++;
					hg->cfont = *it;
					it++;
					hg->csize = (*it).toInt();
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
				OB.Rot += atan2(OB.Height,OB.Width)*(180.0/3.1415927);
				OB.Width = sqrt(pow(OB.Width,2)+pow(OB.Height,2));
				OB.Height = 0;
				OB.Clip.setPoints(4, -1,-1, static_cast<int>(OB.Width+1),-1, static_cast<int>(OB.Width+1), static_cast<int>(OB.Height+1), -1, static_cast<int>(OB.Height+1));
				}
			OB.LayerNr = -1;
			pm.save();
			pm.translate(static_cast<int>(OB.Xpos), static_cast<int>(OB.Ypos));
			pm.rotate(static_cast<double>(OB.Rot));
			if (OB.Pcolor != "None")
				{
				SetFarbe(&tmpfa, OB.Pcolor, OB.Shade);
				pm.setBrush(tmpfa);
				pm.setBackgroundColor(tmpfa);
				}
			else
				pm.setBrush(Qt::NoBrush);
			if (OB.Pcolor2 != "None")
				{
				SetFarbe(&tmpfa, OB.Pcolor2, OB.Shade2);
				pm.setPen(QPen(tmpfa, static_cast<int>(OB.Pwidth), Qt::PenStyle(OB.PLineArt), Qt::PenCapStyle(OB.PLineEnd), Qt::PenJoinStyle(OB.PLineJoin)));
				}
			else
				pm.setPen(Qt::NoPen);
			switch (OB.PType)
				{
				case 2:
					pm.setClipRegion(QRegion(pm.xForm(OB.Clip)));
					if (OB.Pcolor != "None")
						{
						pm.setPen(Qt::NoPen);
						DrawPoly(&pm, OB.Clip, pm.brush().color(), &OB);
						}
					if (OB.Pfile != "")
						{
						QFileInfo fi = QFileInfo(OB.Pfile);
						if (fi.exists())
							{
							pm.save();
							if (OB.flippedH % 2 != 0)
								{
								pm.translate(static_cast<int>(OB.Width), 0);
								pm.scale(-1, 1);
								}
							if (OB.flippedV % 2 != 0)
								{
								pm.translate(0, static_cast<int>(OB.Height));
								pm.scale(1, -1);
								}
							pm.scale(OB.LocalScX, OB.LocalScY);
							QString ext = fi.extension(false).lower();
							QImage pixm = LoadPict(OB.Pfile);
							pm.drawImage(static_cast<int>(OB.LocalX), static_cast<int>(OB.LocalY), pixm);
							pm.restore();
							}
						}
					if ((OB.Pcolor2 != "None") || (OB.NamedLStyle != ""))
						{
						pm.setBrush(Qt::NoBrush);
						DrawPolyL(&pm, OB.Clip, &OB);
						}
					break;
				case 4:
					if (Ptexti.count() != 0)
						{
						pm.setClipRegion(QRegion(pm.xForm(OB.Clip)));
						if (OB.Pcolor != "None")
							{
							pm.setPen(Qt::NoPen);
							DrawPoly(&pm, OB.Clip, pm.brush().color(), &OB);
							}
						if (OB.flippedH % 2 != 0)
							{
							pm.translate(static_cast<int>(OB.Width), 0);
							pm.scale(-1, 1);
							}
						if (OB.flippedV % 2 != 0)
							{
							pm.translate(0, static_cast<int>(OB.Height));
							pm.scale(1, -1);
							}
						for (uint a = 0; a < Ptexti.count(); a++)
							{
							hl = Ptexti.at(a);
							if (hl->ch == QChar(13))
								continue;
							ffo = DoFonts2[hl->cfont];
							if (hl->ccolor != "None")
								{
								SetFarbe(&tmpfa, hl->ccolor, hl->cshade);
								pm.setPen(tmpfa);
								}
							pm.setFont(ffo);
							chx = hl->ch;
							ffo.setPointSize(hl->csize);
							if (hl->cstyle != 0)
								{
								if (hl->cstyle & 32)
									ffo.setBold(true);
								if (hl->cstyle & 16)
									ffo.setStrikeOut(true);
								if (hl->cstyle & 8)
									ffo.setUnderline(true);
								if (hl->cstyle & 4)
									ffo.setItalic(true);
								if (hl->cstyle & 1)
									ffo.setPointSize(static_cast<int>(hl->csize * Prefs->DVHochSc / 100));
								if (hl->cstyle & 2)
									ffo.setPointSize(static_cast<int>(hl->csize * Prefs->DVTiefSc / 100));
								if (hl->cstyle & 64)
									{
									if (chx.upper() != chx)
										ffo.setPointSize(static_cast<int>(hl->csize * Prefs->DVKapit / 100));
									}
								pm.setFont(ffo);
								}
							else
								{
								ffo.setPointSize(hl->csize);
								pm.setFont(ffo);
								}
							pm.drawText(qRound(hl->xp), qRound(hl->yp), hl->ch);
							}
						}
					break;
				case 5:
					if (OB.NamedLStyle == "")
						pm.drawLine(0, 0, qRound(OB.Width), 0);
					else
						{
						multiLine ml = MLineStyles[OB.NamedLStyle];
						for (int it = ml.size()-1; it > -1; it--)
							{
							SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
							pm.setPen(QPen(tmpfa,
											 QMAX(static_cast<int>(ml[it].Width), 1),
											 static_cast<Qt::PenStyle>(ml[it].Dash),
											 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
											 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
							pm.drawLine(0, 0, qRound(OB.Width), 0);
							}
						}
					break;
				case 1:
				case 3:
				case 6:
					if (OB.GrType == 0)
						{
						DrawPoly(&pm, OB.Clip, pm.brush().color(), &OB);
						pm.setBrush(Qt::NoBrush);
						DrawPolyL(&pm, OB.Clip, &OB);
						}
					else
						{ // Insert Code for Gradients here
						KImageEffect::GradientType tt = KImageEffect::HorizontalGradient;
						QColor co1 = QColor(Qt::black);
						QColor co2 = QColor(Qt::black);
						QString col1 = OB.GrColor2;
						QString col2 = OB.GrColor;
						int sh1 = OB.GrShade2;
						int sh2 = OB.GrShade;
						int typ = OB.GrType;
						if ((col1 != "None") && (col1 != ""))
							SetFarbe(&co1, col1, sh1);
						if ((col2 != "None") && (col2 != ""))
							SetFarbe(&co2, col2, sh2);
						switch (typ)
							{
							case 1:
								tt = KImageEffect::HorizontalGradient;
								break;
							case 2:
								tt = KImageEffect::VerticalGradient;
								break;
							case 3:
								tt = KImageEffect::DiagonalGradient;
								break;
							case 4:
								tt = KImageEffect::CrossDiagonalGradient;
								break;
							case 5:
								tt = KImageEffect::EllipticGradient;
								break;
							default:
								break;
							}
						QImage pixm = KImageEffect::gradient(QSize(static_cast<int>(OB.Width),static_cast<int>(OB.Height)), co1, co2, tt, 0);
						QBitmap bmd = QBitmap(static_cast<int>(OB.Width), static_cast<int>(OB.Height));
						bmd.fill(Qt::color0);
						QPainter pb;
						pb.begin(&bmd);
						pb.setBrush(Qt::color1);
						pb.setPen(QPen(Qt::color1, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
						DrawPoly(&pb, OB.Clip, pb.brush().color(), &OB, true);
						pb.end();
						QPixmap pmd = QPixmap(static_cast<int>(OB.Width), static_cast<int>(OB.Height));
						pmd.fill();
						QPainter pd;
						pd.begin(&pmd);
						if (!pixm.isNull())
							pd.drawImage(0, 0, pixm);
						pd.end();
						pmd.setMask(bmd);
						pm.drawPixmap(0, 0, pmd);
						pm.setBrush(Qt::NoBrush);
						DrawPolyL(&pm, OB.Clip, &OB);
						}
					break;
				case 7:
					if (OB.PoLine.size() > 3)
						{
						for (uint poi=0; poi<OB.PoLine.size()-3; poi += 4)
							{
							if (OB.PoLine.point(poi).x() > 900000)
								continue;
							Bez.setPoint(0, OB.PoLine.pointQ(poi));
							Bez.setPoint(1, OB.PoLine.pointQ(poi+1));
							Bez.setPoint(2, OB.PoLine.pointQ(poi+3));
							Bez.setPoint(3, OB.PoLine.pointQ(poi+2));
							if (OB.NamedLStyle == "")
								pm.drawCubicBezier(Bez);
							else
								{
								multiLine ml = MLineStyles[OB.NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
									{
									SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
									pm.setPen(QPen(tmpfa,
													 QMAX(static_cast<int>(ml[it].Width), 1),
													 static_cast<Qt::PenStyle>(ml[it].Dash),
													 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
											 		 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
									pm.drawCubicBezier(Bez);
									}
								}
							}
						}
					break;
				case 8:
					if (OB.PoLine.size() > 3)
						{
						if (OB.PoShow)
							{
							for (uint poi=0; poi<OB.PoLine.size()-3; poi += 4)
								{
								if (OB.PoLine.point(poi).x() > 900000)
									continue;
								Bez.setPoint(0, OB.PoLine.pointQ(poi));
								Bez.setPoint(1, OB.PoLine.pointQ(poi+1));
								Bez.setPoint(2, OB.PoLine.pointQ(poi+3));
								Bez.setPoint(3, OB.PoLine.pointQ(poi+2));
								if (OB.NamedLStyle == "")
									pm.drawCubicBezier(Bez);
								else
									{
									multiLine ml = MLineStyles[OB.NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
										{
										SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
										pm.setPen(QPen(tmpfa,
													 	QMAX(static_cast<int>(ml[it].Width), 1),
													 	static_cast<Qt::PenStyle>(ml[it].Dash),
													 	static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
											 		 	static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
										pm.drawCubicBezier(Bez);
										}
									}
								}
							}
						}
					cl = FlattenPath(OB.PoLine, Segments);
					CurX = OB.Extra;
					if (Ptexti.count() != 0)
						CurX += Ptexti.at(0)->cextra;
					zae = 0;
					wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2));
					while (wid < 1)
						{
						zae++;
						if (zae == cl.size()-1)
							goto PfadEnd;
						wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2));
						}
					rota = xy2Deg(cl.point(zae+1).x()-cl.point(zae).x(),cl.point(zae+1).y()-cl.point(zae).y());
					for (a = 0; a < Ptexti.count(); a++)
						{
						CurY = 0;
						hl = Ptexti.at(a);
						chx = hl->ch;
						if ((chx == QChar(30)) || (chx == QChar(13)))
							continue;
						ffo = DoFonts2[hl->cfont];
						if (hl->ccolor != "None")
							{
							SetFarbe(&tmpfa, hl->ccolor, hl->cshade);
							pm.setPen(tmpfa);
							}
						else
							pm.setPen(Qt::NoPen);
						chs = hl->csize;
						ffo.setPointSize(hl->csize);
						pm.setFont(ffo);
						desc = pm.fontMetrics().descent();
						asce = pm.fontMetrics().ascent();
						int chst = hl->cstyle & 127;
						if (chst != 0)
							{
							if (chst & 16)
								ffo.setStrikeOut(true);
							if (chst & 8)
								ffo.setUnderline(true);
							if (chst & 1)
								{
								CurY -= asce * Prefs->DVHoch / 100;
								ffo.setPointSize(static_cast<int>(hl->csize * Prefs->DVHochSc / 100));
								chs = static_cast<int>(hl->csize * Prefs->DVHochSc / 100);
								}
							if (chst & 2)
								{
								CurY += asce * Prefs->DVTief / 100;
								ffo.setPointSize(static_cast<int>(hl->csize * Prefs->DVTiefSc / 100));
								chs = static_cast<int>(hl->csize * Prefs->DVTiefSc / 100);
								}
							if (chst & 64)
								{
								if (chx.upper() != chx)
									{
									ffo.setPointSize(static_cast<int>(hl->csize * Prefs->DVKapit / 100));
									chs = static_cast<int>(hl->csize * Prefs->DVKapit / 100);
									chx = chx.upper();
									}
								}
							}
						pm.setFont(ffo);
						wide = pm.fontMetrics().width(chx);
						if ((CurX+(wide+hl->cextra)/2) >= wid)
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
										wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2));
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
						pm.save();
						pm.translate(cl.point(zae).x(), cl.point(zae).y());
						pm.rotate(rota);
						pm.drawText(static_cast<int>(CurX+hl->cextra), static_cast<int>(CurY+OB.BaseOffs), hl->ch);
						pm.restore();
						CurX += wide+hl->cextra;
						}
PfadEnd:	break;
				}
			pm.restore();
			QDir::setCurrent(CurDirP);
			}
		DOC=DOC.nextSibling();
		}
	pm.end();
	QImage tmpi1 = tmp.convertToImage();
	QImage tmpi = tmpi1.smoothScale(static_cast<int>(tmp.width()*pmmax), static_cast<int>(tmp.height()*pmmax));
	tmp.convertFromImage(tmpi);
	return tmp;
}

void ScPreview::SetFarbe(QColor *tmp, QString farbe, int shad)
{
	int h, s, v, sneu;
	Farben[farbe].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
		{
		Farben[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shad / 100);
		tmp->setHsv(h, s, sneu);
		}
	else
		{
		Farben[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shad / 100;
		tmp->setHsv(h, sneu, v);
		}
}

void ScPreview::DrawPoly(QPainter *p, QPointArray pts, QColor BackF, struct CLBuf *OB, bool bitm)
{
	QBitmap bm(static_cast<int>(OB->Width), static_cast<int>(OB->Height));
	bm.fill(Qt::color0);
	QPainter pbm;
	pbm.begin(&bm);
	pbm.setBrush(Qt::color1);
	pbm.setPen(Qt::NoPen);
	pbm.setRasterOp(Qt::XorROP);
	QPointArray dr;
	QValueList<uint>::Iterator it3;
	uint FirstVal = 0;
	for (it3 = Segments.begin(); it3 != Segments.end(); ++it3)
		{
		dr.resize(0);
		dr.putPoints(0, (*it3)-FirstVal-1, pts, FirstVal);
		pbm.drawPolygon(dr);
		FirstVal = (*it3);
		}
	dr.resize(0);
	dr.putPoints(0, pts.size()-FirstVal-1, pts, FirstVal);
	pbm.drawPolygon(dr);
	pbm.end();
	if (bitm)
		p->drawPixmap(0, 0, bm);
	else
		{
		QPixmap ppm(static_cast<int>(OB->Width), static_cast<int>(OB->Height));
		ppm.fill(BackF);
		ppm.setMask(bm);
		p->drawPixmap(0, 0, ppm);
		}
}

void ScPreview::DrawPolyL(QPainter *p, QPointArray pts, struct CLBuf *OB)
{
	QColor tmp;
	if (Segments.count() != 0)
		{
		QValueList<uint>::Iterator it2;
		uint FirstVal = 0;
		for (it2 = Segments.begin(); it2 != Segments.end(); ++it2)
			{
			if (OB->NamedLStyle == "")
				p->drawPolyline(pts, FirstVal, (*it2)-FirstVal);
			else
				{
				multiLine ml = MLineStyles[OB->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
					{
					SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(QPen(tmp,
									 QMAX(static_cast<int>(ml[it].Width), 1),
									 static_cast<Qt::PenStyle>(ml[it].Dash),
									 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
									 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
					p->drawPolyline(pts, FirstVal, (*it2)-FirstVal);
					}
				}
			FirstVal = (*it2);
			}
		if (OB->NamedLStyle == "")
			p->drawPolyline(pts, FirstVal);
		else
			{
			multiLine ml = MLineStyles[OB->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
				{
				SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 QMAX(static_cast<int>(ml[it].Width), 1),
								 static_cast<Qt::PenStyle>(ml[it].Dash),
								 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
								 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolyline(pts, FirstVal);
				}
			}
		}
	else
		{
		if (OB->NamedLStyle == "")
			p->drawPolyline(pts);
		else
			{
			multiLine ml = MLineStyles[OB->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
				{
				SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 QMAX(static_cast<int>(ml[it].Width), 1),
								 static_cast<Qt::PenStyle>(ml[it].Dash),
								 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
								 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolyline(pts);
				}
			}
		}
}

