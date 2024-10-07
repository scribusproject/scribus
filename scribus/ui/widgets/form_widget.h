#ifndef FORMWIDGET_H
#define FORMWIDGET_H

#include <QWidget>
#include <QFont>

//#include "scribusapi.h"

/* ********************************************************************************* *
 *
 * Section Container Header
 *
 * ********************************************************************************* */

class FormWidget : public QWidget
{
	Q_OBJECT
	Q_CLASSINFO("Version", "1.2.0")

	Q_PROPERTY(QString label READ text WRITE setText)
	Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
	Q_PROPERTY(QSize pixmapSize READ pixmapSize WRITE setPixmapSize)
	Q_PROPERTY(LabelPosition direction READ direction WRITE setDirection)
	Q_PROPERTY(LabelPosition iconDirection READ iconDirection WRITE setIconDirection)
	Q_PROPERTY(QFont font READ font WRITE setFont STORED true)
	Q_PROPERTY(bool useSmallFont READ useSmallFont WRITE setUseSmallFont)
	Q_PROPERTY(int space READ space WRITE setSpace)
	Q_PROPERTY(bool preserveLabelSpace READ preserveLabelSpace WRITE setPreserveLabelSpace)
	Q_PROPERTY(bool labelVisibility READ labelVisibility WRITE setLabelVisibility)
	Q_PROPERTY(bool iconVisibility READ iconVisibility WRITE setIconVisibility)


public:

	enum LabelPosition { Left, Top, Right, Bottom };
	Q_ENUM(LabelPosition)

	explicit FormWidget(QWidget *parent = nullptr);

	QSize minimumSizeHint() const override;

	void setText(QString const &text);
	QString text() { return m_label; };

	void setLabelVisibility(bool visible);
	bool labelVisibility() { return m_labelVisibility; };

	void setIconVisibility(bool visible);
	bool iconVisibility() const { return m_iconVisibility; };

	void setPreserveLabelSpace(bool preserveSpace);
	bool preserveLabelSpace() { return m_preserveLabelSpace; };

	void setDirection(FormWidget::LabelPosition direction);
	FormWidget::LabelPosition direction() { return m_position; };

	void setIconDirection(FormWidget::LabelPosition direction);
	FormWidget::LabelPosition iconDirection() { return m_iconPosition; };

	void setFont(QFont font);
	QFont font() { return m_font; };

	void setUseSmallFont(bool smallFont);
	bool useSmallFont() { return m_useSmallFont; };

	int smallFontSize() const;

	void setSpace(int space);
	int space() { return m_space; };

	void setPixmap(QPixmap icon);
	QPixmap pixmap() { return m_pixmap; };

	void setPixmapSize(QSize size);
	QSize pixmapSize() { return m_pixmapSize; };

	bool canUsePixmap() const { return (!m_pixmap.isNull() && m_iconVisibility); };
	bool canUseLabel() const { return (!m_label.isEmpty() && m_labelVisibility) || (m_preserveLabelSpace && m_labelVisibility); };

	void addWidget(QWidget* widget);

protected:
	void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
	bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
	LabelPosition m_position {LabelPosition::Bottom};
	LabelPosition m_iconPosition {LabelPosition::Left};
	int m_space {4};
	QSize m_pixmapSize {QSize(16,16)};
	QString m_label {"Label"};
	QFont m_font;
	QPixmap m_pixmap {QPixmap()};
	bool m_iconVisibility {true};
	bool m_preserveLabelSpace {false};
	bool m_labelVisibility {true};
	bool m_hasShortcut {false};
	int m_shortcutId {0};
	bool m_useSmallFont {true};

	void calculateFrame();
	void updateShortcut();
	void labelSize(int &l, int &t, int &r, int &b, int &w, int &h) const;
};


#endif
