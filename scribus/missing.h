#ifndef MISSINGFONT_H
#define MISSINGFONT_H

#include <qdialog.h>
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
