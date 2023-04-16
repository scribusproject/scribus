/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef BUTTONICON_H
#define BUTTONICON_H

#include <QDialog>

class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QComboBox;
class QGroupBox;
class QSlider;
class QPushButton;
class QLabel;

#include "scribusapi.h"

class PageItem;

class SCRIBUS_API ButtonIcon : public QDialog
{ 
    Q_OBJECT

public:
	ButtonIcon(QWidget* parent, PageItem* ite);
	~ButtonIcon() {};

	int IcPlace { 0 };
	int IcScaleW { 0 };
	int IcScaleH { 0 };
	qreal IcPlaceX { 0.0 };
	qreal IcPlaceY { 0.0 };

	int place() const;
	int scaleH() const;
	int scaleW() const;

private slots:
	void MoveIconY(int y);
	void MoveIconX(int x);
	void SetAllVals();
	void SetScaleHow(int s);

protected:
	PageItem* m_item { nullptr };

	QLabel* TextLabel1_2 { nullptr };
	QComboBox* Place { nullptr };
	QLabel* TextLabel1 { nullptr };
	QComboBox* ScaleW { nullptr };
	QLabel* TextLabel2 { nullptr };
	QComboBox* ScaleH { nullptr };
	QGroupBox* GroupButton { nullptr };
	QLabel* TextLabel3 { nullptr };
	QSlider* SliderX { nullptr };
	QSlider* SliderY { nullptr };
	QPushButton* OK { nullptr };
	QPushButton* Cancel { nullptr };
	QPushButton* Reset { nullptr };
};

#endif // BUTTONICON_H
