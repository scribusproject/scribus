#ifndef COLORPICKER_HATCH_H
#define COLORPICKER_HATCH_H

#include <QWidget>

#include "colorpicker_enum.h"
#include "scribusdoc.h"
#include "ui_colorpicker_hatch.h"

class ColorList;
class ColorListBox;

class ColorPickerHatch : public QWidget, Ui::ColorPickerHatch
{
	Q_OBJECT

public:
	explicit ColorPickerHatch(QWidget *parent = nullptr);
	~ColorPickerHatch() {};

	Context context() const;
	void setContext(Context config);

	void setDoc(ScribusDoc* doc);
	void setColorList(const ColorList& list);

	const CPHatchData& hatchData() const;
	void setHatchData(const CPHatchData& hatch);

	int type() const { return Gradient_Hatch; }

	QString toolTipText() const;

public slots:
	void languageChange();
	void unitChange();

private slots:
	void updateSize();
	void updateHatch();

private:
	ScribusDoc *m_doc { nullptr };
	QBrush m_background;
	Context m_context { Context::Simple };
	CPHatchData m_hatch;
	int currentUnit {0};
	ColorList colList;

	ColorListBox* listForegroundSwatches { nullptr };
	ColorListBox *listBackgroundSwatches { nullptr };

	QColor colorFromName(QString colorName, double shade) const;
	QBrush colorBrush(QSize size, QString colorName, double shade = 100.0, double opacity = 1.0) const;

	void connectSlots();
	void disconnectSlots();

signals:
	void hatchChanged();
	void sizeChanged();
};

#endif // COLORPICKER_HATCH_H
