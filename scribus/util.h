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

#include <libpdf/pdflib.h>

class ScribusDoc;
class QDomElement;
class ScribusView;
struct CopyPasteBuffer;
class PageItem;
class FPointArray;
struct Layer;
class FPointArray;
class Foi;

QColor SetColor(ScribusDoc *currentDoc, QString color, int shad);
void GetItemProps(bool newVersion, QDomElement *obj, struct CopyPasteBuffer *OB);
QStringList sortQStringList(QStringList aList);
void ReOrderText(ScribusDoc *currentDoc, ScribusView *view);
void WordAndPara(PageItem *currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN);
void CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *currItem);
bool overwrite(QWidget *parent, QString filename);
FPoint getMaxClipF(FPointArray* Clip);
FPoint getMinClipF(FPointArray* Clip);
QString Path2Relative(QString Path);
QPixmap LoadPDF(QString fn, int Page, int Size, int *w, int *h);
QByteArray ComputeMD5Sum(QByteArray *in);
char *toHex( uchar u );
QString String2Hex(QString *in, bool lang = true);
QString CompressStr(QString *in);
void Level2Layer(ScribusDoc *currentDoc, struct Layer *ll, int Level);
int Layer2Level(ScribusDoc *currentDoc, int LayerNr);
void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
double xy2Deg(double x, double y);
QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota);
FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
QPixmap * getSmallPixmap(QColor rgb);
QPixmap * getWidePixmap(QColor rgb);
QPixmap loadIcon(QString nam);
uint getDouble(QString in, bool raw);
bool loadText(QString nam, QString *Buffer);
double QStodouble(QString in);
int QStoInt(QString in);
QString GetAttr(QDomElement *el, QString at, QString def="0");
QImage ProofImage(QImage *Im);
int System(const QStringList & args);
int callGS(const QStringList& args_in, const QString device="");
int callGS(const QString& args_in, const QString device="");
// Return gs version. If gs couldn't be found or there was a problem parsing output, return false
// (in which case minor and major have undefined values).
QString getGSVersion();
bool getNumericGSVersion(int & major, int & minor);
int copyFile(QString source, QString target);
int moveFile(QString source, QString target);
QString checkFileExtension(const QString &, const QString &);
// On Windows, return short path name, else return longPath;
QString getShortPathName(QString longPath);

#endif
