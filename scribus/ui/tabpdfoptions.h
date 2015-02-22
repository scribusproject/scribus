/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABPDF_OPTS_H
#define TABPDF_OPTS_H

#include <QTabWidget>
#include <QMap>
#include <QList>
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QCheckBox;
class QComboBox;
class QRadioButton;
class QPushButton;
class QToolButton;
class QLineEdit;
class QLabel;
class QSpinBox;
class QListWidget;
class QListWidgetItem;

#include "scribusapi.h"
#include "bookmwin.h"

#include "ui_tabpdfoptions.h"

class PDFOptions;
class PDFExportDialog;
class ScribusDoc;
class ScrSpinBox;

class TabPDFOptions : public QTabWidget, public Ui_TabPDFOptions
{
	Q_OBJECT

public:
	TabPDFOptions(QWidget* parent, PDFOptions & Optionen,
					const SCFonts &AllFonts,
					const ProfilesL & PDFXProfiles,
					const QMap<QString, int> & DocFonts,
					ScribusDoc *doc);
	~TabPDFOptions() {};

	void restoreDefaults(PDFOptions & Optionen,
						 const SCFonts &AllFonts,
						 const ProfilesL & PDFXProfiles,
						 const QMap<QString, int> & DocFonts);

	void storeValues(PDFOptions& options);

	void unitChange(int docUnitIndex);

	QStringList fontsToEmbed();
	QStringList fontsToOutline();

signals:
	void noInfo();
	void hasInfo();

public slots:
	void doDocBleeds();
	void ToggleEncr();
	void EnablePDFX(int a);
	void DoDownsample();
	void RemoveEmbed();
	void PutToEmbed();
	void RemoveOutline();
	void PutToOutline();
	void SelAFont(QListWidgetItem*);
	void SelEFont(QListWidgetItem*);
	void SelSFont(QListWidgetItem*);
	void EmbedAll();
	void OutlineAll();
	void PagePr();
	void SetPgEff(QListWidgetItem* current, QListWidgetItem* previous);
	void SetEffOpts(int nr);
	void ValidDI(int nr);
	void PDFMirror();
	void Rotation(int value);
	void DoEffects();
	void EffectOnAll();
	void SelRange(bool e);
	void EnablePr(int a);
	void EnablePG();
	void EnablePGI();
	void EnablePGI2();
	void EnableLPI(int a);
	void EnableLPI2();
	void SelLPIcol(int);
	void enableCMS(bool enable);
	void checkInfo();

protected slots:
	void createPageNumberRange();
	void handleCompressionMethod(int ind);

protected:
	// PDFExportDialog should really privately inherit from us, but it can't
	// since it needs to be a dialog not a tab widget. Allow it access to
	// protected and private members instead. If we ever clean up the direct
	// widget access in this class, it might be able to privately inheirit and
	// be added to a very thin dialog class instead.
	friend class PDFExportDialog;
	QList<PDFPresentationData> EffVal;
	QString SelLPIcolor;

private:
	// Non-GUI protected members
	double unitRatio;
	ScribusDoc* const m_Doc;
	const SCFonts & AllFonts;
	PDFOptions & Opts;
	bool cms;
	QMap<QString, QString> AnnotationFonts;

};
#endif
