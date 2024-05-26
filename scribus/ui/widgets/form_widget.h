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
    Q_CLASSINFO("Version", "1.0.0")

	Q_PROPERTY(QString label READ text WRITE setText)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
    Q_PROPERTY(QSize pixmapSize READ pixmapSize WRITE setPixmapSize)
    Q_PROPERTY(LabelPosition direction READ direction WRITE setDirection)
    Q_PROPERTY(QFont font READ font WRITE setFont STORED true)
    Q_PROPERTY(int space READ space WRITE setSpace)
    Q_PROPERTY(bool preserveLabelSpace READ preserveLabelSpace WRITE setPreserveLabelSpace)
    Q_PROPERTY(bool labelVisibility READ labelVisibility WRITE setLabelVisibility)


public:

    enum LabelPosition { Left, Top, Right, Bottom };
    Q_ENUM(LabelPosition)

    explicit FormWidget(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;

	void setText(QString const &text);
	QString text();

    void setLabelVisibility(bool visible);
    bool labelVisibility();

    void setPreserveLabelSpace(bool preserveSpace);
    bool preserveLabelSpace();

    void setDirection(FormWidget::LabelPosition direction);
    FormWidget::LabelPosition direction();

    void setFont(QFont font);
    QFont font();

    void setSpace(int space);
    int space();

    void setPixmap(QPixmap icon);
    QPixmap pixmap();

    void setPixmapSize(QSize size);
    QSize pixmapSize();

    bool hasPixmap() const;

protected:
	void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
	bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
	LabelPosition m_position {LabelPosition::Bottom};
	int m_space {4};
	QSize m_pixmapSize {QSize(16,16)};
	QString m_label {"Label"};
    QFont m_font;
	QPixmap m_pixmap {QPixmap()};
	bool m_preserveLabelSpace {false};
	bool m_labelVisibility {true};
	bool m_hasShortcut {false};
	int m_shortcutId {0};

    void calculateFrame();
	void updateShortcut();
    void labelSize(int &w, int &h) const;
};


#endif
