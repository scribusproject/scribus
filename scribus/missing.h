/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MISSINGFONT_H
#define MISSINGFONT_H

#include <QDialog>

class QLabel;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class FontCombo;
class ScribusDoc;
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API MissingFont : public QDialog
{
    Q_OBJECT

public:
	MissingFont( QWidget* parent, QString fon, ScribusDoc* doc);
	~MissingFont() {};
	
	const QString getReplacementFont();

public slots:
	virtual void newFont(const QString& replacement);

protected:
	QHBoxLayout* missingFontLayout;
	QGridLayout* missingFontGridLayout;
	QPushButton* okButton;
	QLabel* insteadLabel;
	QLabel* pixmapLabel;
	FontCombo* replaceFontCombo;
	QLabel* notInstalledLabel;
	QLabel* useLabel;
	QString replacementFont;
};

#endif // MISSINGFONT_H
