/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_H
#define _UTIL_H

#include <vector>

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

#include "fpoint.h"
#include "fpointarray.h"
#include "pagestructs.h"
#include "sccolor.h"
#include "scribusapi.h"

class ScribusDoc;
class QDomElement;
class ScribusView;
struct CopyPasteBuffer;
class PageItem;
struct Layer;
class Foi;

QColor SCRIBUS_API SetColor(ScribusDoc *currentDoc, QString color, int shad);
void SCRIBUS_API GetItemProps(bool newVersion, QDomElement *obj, struct CopyPasteBuffer *OB);
/*! \brief Returns a sorted list of QStrings - sorted by locale specific rules!
Uses compareQStrings() as rule. There is STL used!
\author Petr Vanek
\param QStringList aList unsorted string list
\retval QStringList sorted string list
 */
QStringList SCRIBUS_API sortQStringList(QStringList aList);
void SCRIBUS_API ReOrderText(ScribusDoc *currentDoc, ScribusView *view);
void SCRIBUS_API WordAndPara(PageItem *currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN);
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
/*! \brief Create a cool all-infos pixmaps for the specified color.
\param col Scribus color
\retval QPixmap image with various icons depending on the col properties.
*/
QPixmap SCRIBUS_API *getFancyPixmap(ScColor col);
/*! \brief Put toPaint pixmap into target at the x, y place.
There is handled the alpha channel/transparency too. In the beginning
is the target pixmap filled with all_transparent mask. In the case of
the painting of tiPaint into x, y place, there is this pixmap enabled
in alpha mask too.
\param toPaint a pixmap to be painted into target
\param target a base pixmap. Some kind of painter.
\param x coordinate
\param y coordinate
*/
void SCRIBUS_API paintAlert(QPixmap &toPaint, QPixmap &target, int x = 0, int y = 0);
QPixmap SCRIBUS_API loadIcon(QString nam);
uint SCRIBUS_API getDouble(QString in, bool raw);
//! \brief WARNING: loadText is INCORRECT - use loadRawText instead!
bool SCRIBUS_API loadText(QString nam, QString *Buffer);
/*! \brief Replacement version of loadText that returns a QCString as an out parameter.
The QCString is filled with the contents of the specified file. The return
byte string is of unknown encoding; the caller must handle encoding issues.
There is no need to preallocate the buffer, and the new data replaces any
old contents. */
bool SCRIBUS_API loadRawText(const QString & filename, QCString & buf);
QString SCRIBUS_API GetAttr(QDomElement *el, QString at, QString def="0");
QImage SCRIBUS_API ProofImage(QImage *Im);
/**
* @brief Synchronously execute a new process, optionally saving its output
   *
   * Create a new process via QProcess and wait until finished.  Return the
   * process exit code. Exit code 1 is returned if the process could not be
   * started or terminated abnormally.
   *
   * Note that the argument list is handled exactly as documented by QProcess.
   * In particular, no shell metacharacter expansion is performed (so you can't
   * use $HOME for example, and no quoting is required or appropriate), and each
   * list entry is one argument.
   *
   * If output file paths are provided, any existing file will be truncated and
   * overwritten.
   *
   * @param args Arguments, as per QProcess documentation.
   * @param fileStdErr Path to save error output to, or "" to discard.
   * @param fileStdOut Path to save normal output to, or "" to discard.
   * @return Program exit code, or 1 on failure.
   *
*/
int SCRIBUS_API System(const QStringList & args, const QString fileStdErr = "", const QString fileStdOut = "");
/**
 * @brief Call GhostScript synchronously and store output
   *
   * The gs commands are all similar and consist of a few constant
   * arguments, the variable arguments and the end arguments which
   * are also invariant. It will always use -q -dNOPAUSE and
   * will always end with -c showpage -c quit. It also does automatic
   * device selection unless overridden, and uses the user's antialiasing
   * preferences and font search path.
   *
   * Shell metacharacters are not expanded - that includes quotes.
   * @sa System .
   *
   * @param args_in Custom arguments to GhostScript
   * @param device GS device to use (defaults to an image device if omitted)
 */
int SCRIBUS_API callGS(const QStringList& args_in, const QString device="");
int SCRIBUS_API callGS(const QString& args_in, const QString device="");
int SCRIBUS_API convertPS2PS(QString in, QString out, const QStringList& opts, int level);
int SCRIBUS_API testGSAvailability( void );
int SCRIBUS_API testGSDeviceAvailability( QString device );
/*! \brief Return gs version. If gs couldn't be found or there was a problem parsing output, return false
 (in which case minor and major have undefined values).
 \retval QString version or false on error */
QString SCRIBUS_API getGSVersion();
bool SCRIBUS_API getNumericGSVersion(int & major, int & minor);
QString SCRIBUS_API getGSDefaultExeName(void);
int SCRIBUS_API copyFile(QString source, QString target);
int SCRIBUS_API moveFile(QString source, QString target);
/*!
 \fn QString checkFileExtension(const QString &currName, const QString &extension)
 \author Craig Bradney
 \brief A quick function to make sure a filename has the correct extension and add it if not
 \param currName Current filename
 \param extension File extension to ensure exists
 */
QString SCRIBUS_API checkFileExtension(const QString &, const QString &);
//! \brief On Windows, return short path name, else return longPath;
QString SCRIBUS_API getShortPathName(QString longPath);
/*! \brief Creates a common name for page exports (SVG, bitmap, EPS).
   Output format is: documentname-page01.extension
   \param pageNo number of the exported page (begins from 1)
   \param extension "svg" or e.g. "png" etc.
   \retval QString standardized filename
   \author Petr Vanek
 */
QString SCRIBUS_API getFileNameByPage(uint pageNo, QString extension);
void SCRIBUS_API sDebug(QString message);
inline double SCRIBUS_API square(double);
inline double SCRIBUS_API distance(double, double);
const QString SCRIBUS_API getStringFromSequence(DocumentSectionType type, uint position);
const QString SCRIBUS_API arabicToRoman(uint i);
const QString SCRIBUS_API numberToLetterSequence(uint i);
void SCRIBUS_API parsePagesString(QString pages, std::vector<int>* pageNs, int sourcePageCount);
#endif
