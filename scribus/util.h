/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_H
#define _UTIL_H

#include <vector>

#include <QByteArray>
#include <QColor>
#include <QComboBox>
#include <QDataStream>
#include <QDir>
#include <QList>
#include <QPainterPath>
#include <QPair>
#include <QPixmap>
#include <QRegularExpression>
#include <QStack>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QTransform>

#include "pagestructs.h"
#include "scribusapi.h"

class  QDomElement;
class  QWidget;
class  PageItem;
class  ParagraphStyle;
class  ScribusDoc;
class  ScribusView;

/*!
\brief Cleanup language strings from extensions such as ".UTF-8" we can get from environment variables
*/
QString cleanupLang(const QString& lang);

/*! \brief Compare double values by pre-multiplying by 10000 and converting to long if possible.
If premultiplication does not allow to store result in a long value, perform a standard comparison.
*/
bool SCRIBUS_API compareDouble(double a, double b);

/*! \brief Returns a sorted list of QStrings - sorted by locale specific rules!
Uses compareQStrings() as rule. There is STL used!
\author Petr Vanek
\param aList unsorted string list
\retval QStringList sorted string list
 */
QStringList SCRIBUS_API sortQStringList(QStringList aList);
bool SCRIBUS_API sortingQPairOfStrings( QPair<QString, QString> aP, QPair<QString, QString> bP);
void SCRIBUS_API ReOrderText(ScribusDoc *currentDoc, ScribusView *view);
void SCRIBUS_API WordAndPara(PageItem *currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN);
bool SCRIBUS_API overwrite(QWidget *parent, const QString& filename);
QString SCRIBUS_API Path2Relative(const QString& Path, const QString& baseDir = QDir::currentPath());
QString SCRIBUS_API Relative2Path(const QString& File, const QString& baseDir = QDir::currentPath());
char SCRIBUS_API *toAscii85( quint32 value, bool& allZero );
char SCRIBUS_API *toHex( uchar u );
QString SCRIBUS_API String2Hex(QString *in, bool lang = true);
QString SCRIBUS_API CompressStr(QString *in);
QByteArray SCRIBUS_API CompressArray(const QByteArray& in);
//! \brief WARNING: loadText is INCORRECT - use loadRawText instead!
bool SCRIBUS_API loadText(const QString& filename, QString *buffer);
/*! \brief Replacement version of loadText that returns a QCString as an out parameter.
The QCString is filled with the contents of the specified file. The return
byte string is of unknown encoding; the caller must handle encoding issues.
There is no need to preallocate the buffer, and the new data replaces any
old contents. */
bool SCRIBUS_API loadRawText(const QString & filename, QByteArray & buf);
bool SCRIBUS_API loadRawBytes(const QString & filename, QByteArray & buf);
bool SCRIBUS_API loadRawBytes(const QString & filename, QByteArray & buf, int maxLength);
/**
 * @brief Split a string representing a command line into its arguments
 * The code of this function is copied from Qt 5.15
 */
QStringList splitCommandLineArgs(const QString& command);
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
	\param exename name of the executable
   * @param args Arguments, as per QProcess documentation.
   * @param fileStdErr Path to save error output to, or "" to discard.
   * @param fileStdOut Path to save normal output to, or "" to discard
	\param cancel a reference(!) to bool - flag used to kill the process
   * @return Program exit code, or 1 on failure.
   *
*/
int SCRIBUS_API System(const QString& exename, const QStringList & args,
					   const QString& fileStdErr = QString(), const QString& fileStdOut = QString(),
					   const bool* cancel = nullptr);

/*!
 \fn QString checkFileExtension(const QString &currName, const QString &extension)
 \author Craig Bradney
 \brief A quick function to make sure a filename has the correct extension and add it if not
 \param currName Current filename
 \param extension File extension to ensure exists
 */
QString SCRIBUS_API checkFileExtension(const QString & currName, const QString & extension);
/*! \brief On Windows, return short path name, else return longPath
\param longPath the whole path
\retval QString transformed path
*/
QString SCRIBUS_API getShortPathName(const QString & longPath);
/*! \brief On Windows, return long path name, else return shortPath
\param shortPath the short path
\retval QString transformed path
*/
QString SCRIBUS_API getLongPathName(const QString & shortPath);
/*! \brief Creates a common name for page exports (SVG, bitmap, EPS).
   Output format is: documentname-page01.extension
   \param currDoc a reference to the ScribusDoc document
   \param pageNo number of the exported page (begins from 1)
   \param extension "svg" or e.g. "png" etc.
   \retval QString standardized filename
   \author Petr Vanek
 */
QString SCRIBUS_API getFileNameByPage(ScribusDoc* currDoc, uint pageNo, const QString& extension, const QString& prefix = QString());
//asterix is QString used in numeration when number is presented as few chars, like *, **, *** etc
//default is '*' but can be used any string
QString getStringFromSequence(NumFormat type, uint position, const QString& asterix="*");
QString SCRIBUS_API numberToHebrew(uint i);
QString numberToRoman(uint i);
QString numberToCJK(uint i);
QChar SCRIBUS_API cjkDigit(uint i);
QString numberToLetterSequence(const QString& letters, uint num);
void SCRIBUS_API parsePagesString(const QString& pages, std::vector<int>* pageNs, int sourcePageCount);

QString SCRIBUS_API readLineFromDataStream(QDataStream &s);
void SCRIBUS_API setCurrentComboItem(QComboBox *box, const QString& text);
void SCRIBUS_API setCurrentComboItemFromData(QComboBox *box, const QString& data);
void SCRIBUS_API setCurrentComboItemFromData(QComboBox *box, quint64 data);
void SCRIBUS_API removeComboItem(QComboBox *box, const QString& text);

QString SCRIBUS_API readAdobeUniCodeString(QDataStream &s);
QString SCRIBUS_API readAdobeUniCodeString16(QDataStream &s);

QString SCRIBUS_API getDashString(int dashtype, double linewidth);
void    SCRIBUS_API getDashArray(int dashtype, double linewidth, QVector<double> &dashArray);
void    SCRIBUS_API getDashArray(int dashtype, double linewidth, QVector<float> &dashArray);

/*!
 *\brief Convert pre-1.5.x tables to table as implemented in 1.5.x
 *\retval true on success, false it items have to be converted to a group
*/
bool SCRIBUS_API convertOldTable(ScribusDoc *m_Doc, PageItem* gItem, QList<PageItem*> &gpL, QStack< QList<PageItem*> > *groupStackT = nullptr, QList<PageItem *> *target = nullptr);

void SCRIBUS_API setWidgetBoldFont(QWidget* w, bool wantBold);
/*!
 *\brief
 * check if name exists in list
 * if it exists then return "name (#)", where # counts up from
 * existing "name (#)"s (starting from 2)
 * \retval the current name or a new unique name
*/
template<class STRINGLIST>
QString getUniqueName(const QString& name, const STRINGLIST& list)
{
	if (!list.contains(name))
		return name;

	QString newName(name);

	QString prefix(newName);
	int suffixNum = 1;

	// capture the name and the index, if any
	// fred (5)
	// ^^^^  ^   (where ^ means captured)
	static QRegularExpression rx("^(.*)\\s+\\((\\d+)\\)$");
	QRegularExpressionMatch match = rx.match(newName);
	if (match.hasMatch())
	{
		QStringList matches = rx.namedCaptureGroups();
		prefix = matches[1];
		suffixNum = matches[2].toInt();
	}

	do
	{
		suffixNum++;
		newName = prefix + " (" + QString::number(suffixNum) + ")";
	}
	while (list.contains(newName));

	return newName;
}

/*!
 *\brief
 * check if name exists in list
 * if exist then separator and numbers are pre/append to name while it will be unique
*/
void SCRIBUS_API getUniqueName(QString &name, const QStringList& list, const QString& separator = QString(), bool prepend = false);

#endif
