#ifndef COLORPICKER_COLOR_H
#define COLORPICKER_COLOR_H

#include <QWidget>
#include "ui_colorpicker_color.h"
#include "scribusdoc.h"
#include "colorpicker_enum.h"

class ColorPickerColor : public QWidget, Ui::ColorPickerColor
{
	Q_OBJECT

public:
	explicit ColorPickerColor(QWidget *parent = nullptr);
	~ColorPickerColor() {};

    CPColorData colorData() const;
    void setColorList(const ColorList& list, bool insertNone = true);

	Context context();
	void setContext(Context config);

	int type() { return Gradient_None; };

	QString toolTipText() const;

public slots:
	void setDoc(ScribusDoc *doc);
	void setColorData(CPColorData color);
	void languageChange();

private slots:
	void updateColorFromSwatches();
	void updateColorShade();
	void updateColorAlpha();
	void updateColor();
	void updateSize();

private:
	ScribusDoc *m_doc {nullptr};
	CPColorData m_color;
	Context m_context {Context::Simple};
	bool isMask {false};

	void connectSlots();
	void disconnectSlots();

signals:
	void colorChanged();
	void sizeChanged();

};

#endif // COLORPICKER_COLOR_H
