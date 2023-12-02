#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include <QToolButton>
#include "sccolor.h"
#include "scribusdoc.h"

class ColorListBox;

class SCRIBUS_API ColorButton : public QToolButton
{
	Q_OBJECT
public:

	ColorButton(QWidget *parent = nullptr);
	~ColorButton(){};

	bool hasDot();
	QSize backgroundDotSize();
	QSize foregroundDotSize();

	QString currentColor() const;
	void setColors(const ColorList& list, bool insertNone = true);

public slots:
	void setDoc(ScribusDoc *doc);
	void setCurrentColor(QString colorName);

private slots:
	void setBackground(QBrush background);
	void setForeground(QBrush foreground);
	void setHasDot(bool enabled);

protected:
	void paintEvent(QPaintEvent *e);

	void renderFill(QPainter *painter, QPointF center, qreal radius, QBrush background);

private:
	QBrush m_background {Qt::NoBrush};
	QBrush m_foreground {Qt::NoBrush};
	bool m_hasDot {false};
	ScribusDoc *m_doc {nullptr};
	QWidget *m_context {nullptr};
	ColorListBox *colorList {nullptr};
	QString m_colorName {""};

	QBrush background();
	QBrush foreground();

signals:
	void colorChanged();

};

#endif // COLOR_BUTTON_H
