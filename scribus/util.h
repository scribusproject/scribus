#ifndef _UTIL_H
#define _UTIL_H

#include <qstring.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qstringlist.h>
#include <qwidget.h>
#include <qmap.h>
#include <qpointarray.h>
#include <qvaluelist.h>
#include <qpoint.h>
#include <qdom.h>

#include "scribusapi.h"
#include <libpdf/pdflib.h>

class ScribusDoc;
class QDomElement;
class ScribusView;
struct CopyPasteBuffer;
class PageItem;
class FPointArray;
struct Layer;
class Foi;

QColor SCRIBUS_API SetColor(ScribusDoc *currentDoc, QString color, int shad);
void SCRIBUS_API GetItemProps(bool newVersion, QDomElement *obj, struct CopyPasteBuffer *OB);
QStringList SCRIBUS_API sortQStringList(QStringList aList);
void SCRIBUS_API ReOrderText(ScribusDoc *currentDoc, ScribusView *view);
void SCRIBUS_API WordAndPara(PageItem *currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN);
void SCRIBUS_API CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *currItem);
bool SCRIBUS_API overwrite(QWidget *parent, QString filename);
FPoint SCRIBUS_API getMaxClipF(FPointArray* Clip);
FPoint SCRIBUS_API getMinClipF(FPointArray* Clip);
QString SCRIBUS_API Path2Relative(QString Path);
QPixmap SCRIBUS_API LoadPDF(QString fn, int Page, int Size, int *w, int *h);
QByteArray SCRIBUS_API ComputeMD5Sum(QByteArray *in);
char SCRIBUS_API *toHex( uchar u );
QString SCRIBUS_API String2Hex(QString *in, bool lang = true);
QString SCRIBUS_API CompressStr(QString *in);
void SCRIBUS_API Level2Layer(ScribusDoc *currentDoc, struct Layer *ll, int Level);
//int Layer2Level(ScribusDoc *currentDoc, int LayerNr);
void SCRIBUS_API BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
double SCRIBUS_API xy2Deg(double x, double y);
QPointArray SCRIBUS_API FlattenPath(FPointArray ina, QValueList<uint> &Segs);
QPointArray SCRIBUS_API RegularPolygon(double w, double h, uint c, bool star, double factor, double rota);
FPointArray SCRIBUS_API RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
QPixmap SCRIBUS_API *getSmallPixmap(QColor rgb);
QPixmap SCRIBUS_API *getWidePixmap(QColor rgb);
QPixmap SCRIBUS_API loadIcon(QString nam);
uint SCRIBUS_API getDouble(QString in, bool raw);
bool SCRIBUS_API loadText(QString nam, QString *Buffer);
double SCRIBUS_API QStodouble(QString in);
int SCRIBUS_API QStoInt(QString in);
QString SCRIBUS_API GetAttr(QDomElement *el, QString at, QString def="0");
QImage SCRIBUS_API ProofImage(QImage *Im);
int SCRIBUS_API System(const QStringList & args);
int SCRIBUS_API callGS(const QStringList& args_in, const QString device="");
int SCRIBUS_API callGS(const QString& args_in, const QString device="");
// Return gs version. If gs couldn't be found or there was a problem parsing output, return false
// (in which case minor and major have undefined values).
QString SCRIBUS_API getGSVersion();
bool SCRIBUS_API getNumericGSVersion(int & major, int & minor);
QString SCRIBUS_API getGSDefaultExeName(void);
int SCRIBUS_API copyFile(QString source, QString target);
int SCRIBUS_API moveFile(QString source, QString target);
QString SCRIBUS_API checkFileExtension(const QString &, const QString &);
// On Windows, return short path name, else return longPath;
QString SCRIBUS_API getShortPathName(QString longPath);
// Create a common name for page exports (SVG, bitmap, EPS)
QString SCRIBUS_API getFileNameByPage(uint pageNo, QString extension);

#endif
