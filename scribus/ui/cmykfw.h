/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMYKFARBEN_H
#define CMYKFARBEN_H

#include <QDialog>
#include <QPixmap>
class QTreeWidgetItem;

#include "colorsetmanager.h"
#include "sccolor.h"
#include "scribusapi.h"
#include "ui_cmykfwbase.h"

class ScribusDoc;

class SCRIBUS_API CMYKChoose : public QDialog, Ui::CMYKChooseBase
{
	Q_OBJECT

public:
	CMYKChoose(QWidget* parent, ScribusDoc* doc, const ScColor& orig, const QString& name, ColorList *Colors, bool newCol);
	~CMYKChoose();

	QString colorName() const;
	bool isSpotColor() const;


	ScColor m_color;

	bool Wsave { false };
	bool dynamic { true };
	bool isNew { false };
	bool isRegistration { false };
	int BlackComp { 0 };
	ColorList *EColors { nullptr };
	ColorList CurrSwatch;
	QString Fnam;

public slots:
	void slotRightClick();
	void setValSliders(double value);
	void setValueS(int val);
	void toggleSL();
	void selSwatch();
	void setSpot();
	void selModel(const QString& mod);
	void setColor();
	void setColor2(int h, int s, bool end);
	void selFromSwatch(int itemIndex);
	void setValues();
	void setValueFromHex();
	void leave();

protected:
	void updateNewColorImage(const ScColor& color);

	ColorSetManager m_CSM;
	ScribusDoc* m_doc { nullptr };
	QTreeWidgetItem *systemSwatches { nullptr };
	QTreeWidgetItem *userSwatches { nullptr };
	QTreeWidgetItem *hsvSelector { nullptr };
	QStringList customColSet;
	bool m_isHLC { false };
	QPixmap imageA { QPixmap(50, 50) };
	QPixmap imageN { QPixmap(50, 50) };
	QPixmap alertIcon;
	void showEvent(QShowEvent * event) override;

	QPalette sliderPix(int color);
	QPalette sliderBlack();
};

#endif // CMYKFARBEN_H
