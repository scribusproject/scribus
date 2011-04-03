/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_IMAGE_H
#define PROPERTIESPALETTE_IMAGE_H

#include "ui_propertiespalette_imagebase.h"

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scrspinbox.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"

class PageItem;
class ScComboBox;
class ScribusDoc;
class ScribusMainWindow;
class Selection;
class UserActionSniffer;

class SCRIBUS_API PropertiesPalette_Image : public QWidget, Ui::PropertiesPalette_ImageBase
{
	Q_OBJECT

public:
	PropertiesPalette_Image(QWidget* parent);
	~PropertiesPalette_Image() {};

	virtual void changeEvent(QEvent *e);

	bool userActionOn();

	void updateProfileList();
	void showCMSOptions();

protected:

	bool      m_haveDoc;
	bool      m_haveItem;
	bool      m_userActionOn;
	double    m_unitRatio;
	int       m_unitIndex;

	PageItem *m_item;
	ScribusMainWindow*       m_ScMW;
	ScGuardedPtr<ScribusDoc> m_doc;

	UserActionSniffer *userActionSniffer;
	void installSniffer(ScrSpinBox *spinBox);

private:

	PageItem* currentItemFromSelection();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *i);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	void handleUpdateRequest(int updateFlags);

	void displayImageRotation(double rot);
	void displayScaleAndOffset(double scx, double scy, double x, double y);

	void languageChange();
	void unitChange();

protected slots:
	void spinboxStartUserAction();
	void spinboxFinishUserAction();

private slots:

	void handleLocalXY();
	void handleLocalScale();
	void handleLocalDpi();
	void handleLocalRotation();
	void handleScaling();

	void handleXScale();
	void handleYScale();
	void handleDpiX();
	void handleDpiY();
	void handleImageDPIRatio();
	void handleImageWHRatio();

	void handleImagePageNumber();

	void handleProfile(const QString& prn);
	void handleIntent();
	void handleCompressionMethod();
	void handleCompressionQuality();

	void handleImageEffects();
	void handleExtImgProperties();

signals:
	void DocChanged();
	void UpdtGui(int);
};

#endif
