#ifndef COLORPICKER_HATCH_H
#define COLORPICKER_HATCH_H

#include <QWidget>
#include "ui_colorpicker_hatch.h"
#include "scribusdoc.h"
#include "colorpicker_enum.h"

class ColorList;
class ColorListBox;

class ColorPickerHatch : public QWidget, Ui::ColorPickerHatch
{
	Q_OBJECT

public:
	explicit ColorPickerHatch(QWidget *parent = nullptr);
	~ColorPickerHatch() {};

	Context context();
	void setContext(Context config);

	void setColorList(const ColorList& list);

	CPHatchData hatchData();
	int type() { return Gradient_Hatch; };

	QString toolTipText() const;

public slots:
	void setDoc(ScribusDoc *doc);
	void setHatchData(CPHatchData hatch);
	void languageChange();
	void unitChange();

private slots:
	void updateSize();
	void updateHatch();

private:
	ScribusDoc *m_doc {nullptr};
	QBrush m_background;
	Context m_context {Context::Simple};
	CPHatchData m_hatch;
	int currentUnit {0};
	ColorList colList;
	ColorListBox *listForegroundSwatches;
	ColorListBox *listBackgroundSwatches;

	QColor colorFromName(QString colorName, double shade);
	QBrush colorBrush(QSize size, QString colorName, double shade = 100.0, double opacity = 1.0);

	void connectSlots();
	void disconnectSlots();

signals:
	void hatchChanged();
	void sizeChanged();

};

#endif // COLORPICKER_HATCH_H
