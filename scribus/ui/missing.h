/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MISSINGFONT_H
#define MISSINGFONT_H

#include <QDialog>

#include "scribusapi.h"
#include "scribusstructs.h"

class FontComboH;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;
class ScribusDoc;

class SCRIBUS_API MissingFont : public QDialog
{
    Q_OBJECT

public:
	MissingFont(QWidget* parent, const QString& fon, ScribusDoc* doc);
	~MissingFont() override = default;
	
	const QString &getReplacementFont();

public slots:
	virtual void newFont(const QString& replacement);

protected:
	QHBoxLayout* missingFontLayout { nullptr };
	QGridLayout* missingFontGridLayout { nullptr };
	QPushButton* okButton { nullptr };
	QLabel* insteadLabel { nullptr };
	QLabel* pixmapLabel { nullptr };
	FontComboH* replaceFontCombo { nullptr };
	QLabel* notInstalledLabel { nullptr };
	QLabel* useLabel { nullptr };
	QString replacementFont;
};

#endif // MISSINGFONT_H
