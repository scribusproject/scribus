/****************************************************************************
** Form implementation generated from reading ui file 'Bib.ui'
**
** Created: Sun Oct 14 19:47:56 2001
**
****************************************************************************/
#include "scrap.h"
#include "scrap.moc"
#include <qfileinfo.h>
#include <qfile.h>
#include <qurl.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qfiledialog.h>
#include <qpainter.h>
#include <qrect.h>
#include <qimage.h>
#include <qregion.h>
#include <qregexp.h>
#include <qcursor.h>
#include <cstdlib>
#include <cmath>
#include "missing.h"
#include "query.h"
// #include <iostream.h>
extern QPixmap loadIcon(QString nam);
extern bool loadText(QString nam, QString *Buffer);
extern float QStoFloat(QString in);
extern int QStoInt(QString in);
extern QImage LoadPict(QString fn);
extern QString GetAttr(QDomElement *el, QString at, QString def="0");
extern float xy2Deg(float x, float y);
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);

/* The Scrapbook View Class
 * inherited from QIconView */
BibView::BibView(QWidget* parent, SCFonts &avail, preV *prefs) : QIconView(parent)
{
	Objekte.clear();
	EFonts = avail;
  Prefs = prefs;
}

QDragObject *BibView::dragObject()
{
	QString dt = Objekte[currentItem()->text()].Data.utf8();
	QDragObject *dr = new QTextDrag(dt, this);
	dr->setPixmap(loadIcon("DragPix.xpm"));
	return dr;
}

void BibView::AddObj(QString name, QString daten, QPixmap Bild)
{
	struct Elem DrElem;
 	DrElem.Data = daten;
 	DrElem.Preview = Bild;
 	Objekte.insert(name, DrElem);
}

void BibView::SaveContents(QString name)
{
	QDomDocument docu("scribus");
	QString st="<SCRIBUSSCRAPUTF8></SCRIBUSSCRAPUTF8>";
	docu.setContent(st);
	QDomElement ele=docu.documentElement();
	QMap<QString,Elem>::Iterator itf;
	for (itf = Objekte.begin(); itf != Objekte.end(); ++itf)
		{
		QDomElement dc=docu.createElement("OBJEKT");
		dc.setAttribute("NAME",itf.key());
		dc.setAttribute("DATA",itf.data().Data);
		ele.appendChild(dc);
		}
	QFile f(name);
	if(!f.open(IO_WriteOnly))
		return ;
	QTextStream s(&f);
	QString wr = docu.toString().utf8();
	s.writeRawBytes(wr, wr.length());
	f.close();
}

void BibView::ReadContents(QString name)
{
	QDomDocument docu("scridoc");
	QString f = "";
	if (!loadText(name, &f))
		return;
	QString ff = "";
	if (f.startsWith("<SCRIBUSSCRAPUTF8"))
		ff = QString::fromUtf8(f);
	else
		ff = f;
	if(!docu.setContent(ff))
		return;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSSCRAP") && (elem.tagName() != "SCRIBUSSCRAPUTF8"))
		return;
	clear();
	Objekte.clear();
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
		{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="OBJEKT")
			AddObj(GetAttr(&dc, "NAME"), GetAttr(&dc, "DATA"), createPreview(GetAttr(&dc, "DATA")));
		DOC=DOC.nextSibling();
		}
	QMap<QString,Elem>::Iterator itf;
	for (itf = Objekte.begin(); itf != Objekte.end(); ++itf)
		{
    (void) new QIconViewItem(this, itf.key(), itf.data().Preview);
	  }
}

void BibView::RebuildView()
{
	clear();
	QMap<QString,Elem>::Iterator itf;
	for (itf = Objekte.begin(); itf != Objekte.end(); ++itf)
		{
		itf.data().Preview = createPreview(itf.data().Data);
    (void) new QIconViewItem(this, itf.key(), itf.data().Preview);
	  }
}

QPixmap BibView::createPreview(QString data)
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
	QPixmap tmp = QPixmap(Prefs->PSize * 3, Prefs->PSize * 3);
	QPainter pm;
	QPointArray Bez(4);
	int desc, asce, chs;
	QPointArray cl;
	QColor tmpfa;
	QString chx;
	QFont ffo;
	uint a, zae;
	float CurY, EndX, CurX, wide, rota, wid;
	tmp.fill(white);
	pm.begin(&tmp);
	pm.translate(Prefs->PSize,Prefs->PSize);
	pm.setFont(QFont("Helvetica", 8));
	pm.drawText(0, 12, "Preview");
	pm.drawText(0, 20, "not");
	pm.drawText(0, 28, "available");
	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSELEM") && (elem.tagName() != "SCRIBUSELEMUTF8"))
		{
		QRect pr = QRegion(pm.xForm(QRect(0, 0, Prefs->PSize, Prefs->PSize))).boundingRect();
		QImage tmpi1 = tmp.convertToImage();
		QImage tmpi = tmpi1.copy(pr);
		tmp.convertFromImage(tmpi.smoothScale(Prefs->PSize,Prefs->PSize));
		pm.end();
		return tmp;
		}
	float GrX = QStoFloat(elem.attribute("XP"));
	float GrY = QStoFloat(elem.attribute("YP"));
	float GrW = QStoFloat(elem.attribute("W"));
	float GrH = QStoFloat(elem.attribute("H"));
	float pmmax = Prefs->PSize / QMAX(GrW, GrH);
	pm.scale(pmmax, pmmax);
	tmp.fill(white);
	QDomNode DOC=elem.firstChild();
	DoFonts.clear();
	while(!DOC.isNull())
		{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="FONT")
			{
			tmpf = GetAttr(&pg, "NAME");
			if ((!EFonts.find(tmpf)) || (!EFonts[tmpf]->UseFont))
				{
				if ((!Prefs->GFontSub.contains(tmpf)) || (!EFonts[Prefs->GFontSub[tmpf]]->UseFont))
					{
					DmF *dia = new DmF(0, tmpf, EFonts);
					dia->exec();
					tmpf = dia->Ersatz;
					delete dia;
					Prefs->GFontSub[pg.attribute("NAME")] = tmpf;
					}
				else
					tmpf = Prefs->GFontSub[tmpf];
				}
			fo = EFonts[tmpf]->Font;
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
				pm.setBrush(NoBrush);
			if (OB.Pcolor2 != "None")
				{
				SetFarbe(&tmpfa, OB.Pcolor2, OB.Shade2);
				pm.setPen(QPen(tmpfa, static_cast<int>(OB.Pwidth*pmmax), PenStyle(OB.PLineArt), PenCapStyle(OB.PLineEnd), PenJoinStyle(OB.PLineJoin)));
				}
			else
				pm.setPen(NoPen);
			switch (OB.PType)
				{
				case 2:
					pm.setClipRegion(QRegion(pm.xForm(OB.Clip)));
					if (OB.Pcolor != "None")
						{
						pm.setPen(NoPen);
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
						pm.setBrush(NoBrush);
						DrawPolyL(&pm, OB.Clip, &OB, pmmax);
						}
					break;
				case 4:
					if (Ptexti.count() != 0)
						{
						pm.setClipRegion(QRegion(pm.xForm(OB.Clip)));
						if (OB.Pcolor != "None")
							{
							pm.setPen(NoPen);
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
						for (int it = ml.count()-1; it > -1; it--)
							{
							SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
							pm.setPen(QPen(tmpfa,
											 QMAX(static_cast<int>(ml[it].Width * pmmax), 1),
											 static_cast<PenStyle>(ml[it].Dash),
											 static_cast<PenCapStyle>(ml[it].LineEnd),
											 static_cast<PenJoinStyle>(ml[it].LineJoin)));
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
						pm.setBrush(NoBrush);
						DrawPolyL(&pm, OB.Clip, &OB, pmmax);
						}
					else
						{ // Insert Code for Gradients here
						pm.setBrush(NoBrush);
						DrawPolyL(&pm, OB.Clip, &OB, pmmax);
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
								for (int it = ml.count()-1; it > -1; it--)
									{
									SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
									pm.setPen(QPen(tmpfa,
													 QMAX(static_cast<int>(ml[it].Width * pmmax), 1),
													 static_cast<PenStyle>(ml[it].Dash),
													 static_cast<PenCapStyle>(ml[it].LineEnd),
											 		 static_cast<PenJoinStyle>(ml[it].LineJoin)));
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
									for (int it = ml.count()-1; it > -1; it--)
										{
										SetFarbe(&tmpfa, ml[it].Color, ml[it].Shade);
										pm.setPen(QPen(tmpfa,
													 	QMAX(static_cast<int>(ml[it].Width * pmmax), 1),
													 	static_cast<PenStyle>(ml[it].Dash),
													 	static_cast<PenCapStyle>(ml[it].LineEnd),
											 		 	static_cast<PenJoinStyle>(ml[it].LineJoin)));
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
							pm.setPen(NoPen);
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
			}
		DOC=DOC.nextSibling();
		}
	QRect pr = pm.xForm(QRect(0, 0, static_cast<int>(GrW), static_cast<int>(GrH)));
	QImage tmpi1 = tmp.convertToImage();
	QImage tmpi = tmpi1.copy(pr);
	pm.end();
	if (pr.width() >= pr.height())
		{
		float xfa = Prefs->PSize / static_cast<float>(pr.width());
		tmpi1 = tmpi.smoothScale(Prefs->PSize,static_cast<int>(pr.height()*xfa));
		}
	else
		{
		float xfa = Prefs->PSize / static_cast<float>(pr.height());
		tmpi1 = tmpi.smoothScale(static_cast<int>(pr.width()*xfa),Prefs->PSize);
		}
	tmp = QPixmap(Prefs->PSize, Prefs->PSize);
	tmp.fill(white);
	pm.begin(&tmp);
	pm.drawImage(((Prefs->PSize-tmpi1.width()) / 2),((Prefs->PSize-tmpi1.height()) / 2), tmpi1);
	pm.end();
	return tmp;
}

void BibView::SetFarbe(QColor *tmp, QString farbe, int shad)
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

void BibView::DrawPoly(QPainter *p, QPointArray pts, QColor BackF, struct CLBuf *OB)
{
	QBitmap bm(static_cast<int>(OB->Width), static_cast<int>(OB->Height));
	bm.fill(Qt::color0);
	QPainter pbm;
	pbm.begin(&bm);
	pbm.setBrush(Qt::color1);
	pbm.setPen(NoPen);
	pbm.setRasterOp(XorROP);
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
	QPixmap ppm(static_cast<int>(OB->Width), static_cast<int>(OB->Height));
	ppm.fill(BackF);
	ppm.setMask(bm);
	p->drawPixmap(0, 0, ppm);
}

void BibView::DrawPolyL(QPainter *p, QPointArray pts, struct CLBuf *OB, float sca)
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
				for (int it = ml.count()-1; it > -1; it--)
					{
					SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(QPen(tmp,
									 QMAX(static_cast<int>(ml[it].Width * sca), 1),
									 static_cast<PenStyle>(ml[it].Dash),
									 static_cast<PenCapStyle>(ml[it].LineEnd),
									 static_cast<PenJoinStyle>(ml[it].LineJoin)));
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
			for (int it = ml.count()-1; it > -1; it--)
				{
				SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 QMAX(static_cast<int>(ml[it].Width), 1),
								 static_cast<PenStyle>(ml[it].Dash),
								 static_cast<PenCapStyle>(ml[it].LineEnd),
								 static_cast<PenJoinStyle>(ml[it].LineJoin)));
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
			for (int it = ml.count()-1; it > -1; it--)
				{
				SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 QMAX(static_cast<int>(ml[it].Width), 1),
								 static_cast<PenStyle>(ml[it].Dash),
								 static_cast<PenCapStyle>(ml[it].LineEnd),
								 static_cast<PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolyline(pts);
				}
			}
		}
}

/* This is the main Dialog-Class for the Scrapbook */
Biblio::Biblio( QWidget* parent, SCFonts &avail, preV *prefs)
    : QDialog( parent, "Sclib", false, WStyle_Customize | WStyle_DialogBorder )
{
    resize( 230, 190 );
    setCaption( tr( "Scrapbook" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
  	ScFilename = "";
  	Prefs = prefs; 	
    BiblioLayout = new QVBoxLayout( this );
    BiblioLayout->setSpacing( 0 );
    BiblioLayout->setMargin( 0 );
  	fmenu = new QPopupMenu();
  	fmenu->insertItem(loadIcon("DateiNeu16.png"), tr("New"), this, SLOT(NewLib()), CTRL+Key_N);
  	fSave = fmenu->insertItem(loadIcon("DateiSave16.png"), tr("Save"), this, SLOT(Save()), CTRL+Key_S);
  	fmenu->insertItem(tr("Save as..."), this, SLOT(SaveAs()));
  	fmenu->insertItem(loadIcon("DateiOpen16.png"), tr("Load..."), this, SLOT(Load()), CTRL+Key_O);
  	fmenu->insertItem(loadIcon("DateiClos16.png"), tr("Close"), this, SLOT(CloseWin()));
  	vmenu = new QPopupMenu();
    vS = vmenu->insertItem( tr( "Small" ) );
    vM = vmenu->insertItem( tr( "Medium" ) );
    vB = vmenu->insertItem( tr( "Big" ) );
    switch (prefs->PSize)
    	{
    	case 40:
    		vmenu->setItemChecked(vS, true);
    		break;
    	case 60:
    		vmenu->setItemChecked(vM, true);
    		break;
    	case 80:
    		vmenu->setItemChecked(vB, true);
    		break;
    	}
  	menuBar = new QMenuBar(this);
		menuBar->insertItem(tr("File"), fmenu);
		menuBar->insertItem(tr("Preview"), vmenu);
    BiblioLayout->setMenuBar( menuBar );

    Frame3 = new QFrame( this, "Frame3" );
    Frame3->setFrameShape( QFrame::Box );
    Frame3->setFrameShadow( QFrame::Sunken );
    Frame3Layout = new QVBoxLayout( Frame3 );
    Frame3Layout->setSpacing( 6 );
    Frame3Layout->setMargin( 11 );

    BibWin = new BibView(Frame3, avail, prefs);
    BibWin->setAutoArrange(true);
    BibWin->setSorting(true);
    Frame3Layout->addWidget( BibWin );
    BiblioLayout->addWidget( Frame3 );
    connect(BibWin, SIGNAL(dropped(QDropEvent *, const QValueList<QIconDragItem> &)),
						this, SLOT(DropOn(QDropEvent *)));
		connect(BibWin, SIGNAL(rightButtonClicked(QIconViewItem*, const QPoint &)),
						this, SLOT(HandleMouse(QIconViewItem*)));
	  connect(vmenu, SIGNAL(activated(int)), this, SLOT(SetPreview(int)));
}

void Biblio::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

void Biblio::CloseWin()
{
	emit Schliessen();
}

void Biblio::Save()
{
	if ((!ScFilename.isEmpty()) && (BibWin->Objekte.count() != 0))
		BibWin->SaveContents(ScFilename);
}

void Biblio::SaveAs()
{
  QString fn = QFileDialog::getSaveFileName(0, tr("Scrapbooks (*.scs);; All Files (*)"), this);
  if (!fn.isEmpty())
  	{
  	BibWin->SaveContents(fn);
  	ScFilename = fn;
  	setCaption(fn);
  	fmenu->setItemEnabled(fSave, 1);
  	}
}

void Biblio::Load()
{
	QString fileName = QFileDialog::getOpenFileName(0,tr("Scrapbooks (*.scs);; All Files (*)"),this);
	if (!fileName.isEmpty())
		{
		BibWin->ReadContents(fileName);
		ScFilename = fileName;
		setCaption(fileName);
		}
}

void Biblio::SetPreview(int id)
{
	int a = vmenu->indexOf(id);
	switch (a)
		{
		case 0:
			Prefs->PSize = 40;
			break;
		case 1:
			Prefs->PSize = 60;
			break;
		case 2:
			Prefs->PSize = 80;
			break;
		}
	AdjustMenu();
	BibWin->RebuildView();
}

void Biblio::AdjustMenu()
{
	vmenu->setItemChecked(vS, false);
	vmenu->setItemChecked(vM, false);
	vmenu->setItemChecked(vB, false);
  switch (Prefs->PSize)
  	{
   	case 40:
   		vmenu->setItemChecked(vS, true);
   		break;
   	case 60:
   		vmenu->setItemChecked(vM, true);
   		break;
   	case 80:
   		vmenu->setItemChecked(vB, true);
   		break;
   	}
}

void Biblio::HandleMouse(QIconViewItem *ite)
{
	int mret, del;
	if (ite != 0)
		{
		QPopupMenu *pmenu = new QPopupMenu();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		del = pmenu->insertItem(tr("Delete"));
		mret = pmenu->exec(QCursor::pos());
		if (mret == del)
				DeleteObj(ite->text(), ite);
		delete pmenu;		
		}
}

void Biblio::NewLib()
{
	BibWin->Objekte.clear();
	BibWin->clear();
	ScFilename = "";
  setCaption(tr("Scrapbook"));
  fmenu->setItemEnabled(fSave, 0);
}

void Biblio::DeleteObj(QString name, QIconViewItem *ite)
{
	BibWin->Objekte.remove(name);
	delete ite;
	BibWin->arrangeItemsInGrid(true);
}

void Biblio::DropOn(QDropEvent *e)
{
	QString text, tmp, nam;
	bool img;
	tmp = "";
	if (QTextDrag::decode(e, text))
		{
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).lower();
		img = ((ext=="eps")||(ext=="ps")||(ext=="png")||(ext=="gif")||(ext=="jpg")||(ext=="xpm"));
		if ((fi.exists()) && (!img))
			{
			if (loadText(ur.path(), &text))
				{
				if (text.startsWith("<SCRIBUSELEM>"))
					{
					tmp = text;
    			}
    		}	
			}
		else
			{
			if (text.startsWith("<SCRIBUSELEM>"))
				{
				tmp = text;
    		}
    	}
		ObjFromMenu(text);
		if (Prefs->SaveAtQ)
			Save();
		}
}

void Biblio::ObjFromMenu(QString text)
{
	QString nam, tmp;
  nam = tr("Object") + tmp.setNum(BibWin->Objekte.count());
  Query *dia = new Query(this, "tt", 1, 0, "Name:", "New Entry");
  dia->Answer->setText(nam);
  dia->Answer->selectAll();
  if (dia->exec())
  	{
   	nam = dia->Answer->text();
   	while (BibWin->Objekte.contains(nam))
   		{
   		if (!dia->exec())
   			{
   			delete dia;
   			return;
   			}
   		nam = dia->Answer->text();
   		}
   	}
  else
   	{
   	delete dia;
   	return;
   	}
  delete dia;
	tmp = text;
	QString ff = "";
	if (tmp.startsWith("<SCRIBUSELEMUTF8"))
		ff = QString::fromUtf8(tmp);
	else
		ff = tmp;
	QPixmap pm = BibWin->createPreview(ff);
  BibWin->AddObj(nam, ff, pm);
  (void) new QIconViewItem(BibWin, nam, pm);
}
